#include <ansi.h>
#include <mudlib.h>
  
#define DB_DOBET_NAME "dobet_main"
#define DB_ODDS_NAME "dobet_odds"
#define DB_CHIP_NAME "dobet_chip"
  
int main(object me, string arg)
{
	string outstr;
	string betcode;
	int oddscode,chip,imaxchip;
	mixed ret;
	mixed op;
	string querystr;
	int i;
	
	if( !stringp(arg) )
		return notify_fail("你要投注什么?\n");

	sscanf( arg, "%s %d %d",betcode,oddscode,chip);

	if( (chip<0)||(chip==0) )
		return notify_fail("下注金额必须大于零\n");
	if( chip*10000>me->query("balance") )
		return notify_fail("你钱庄里没有这么多钱\n");
		
	ret = DOBET_D->leasedb("select code,context,setup_id,flag,begin_time,stop_time,end_time,deposit,bet_result,remark,max_bet from "+DB_DOBET_NAME+" where U_Site='"+INTERMUD_MUD_NAME+"' AND code='"+betcode+"'");

	if(!ret || sizeof(ret)<1) 
		return notify_fail("没找到任何符合条件的赌局，无法下注\n");
	
	if( ret[0][3]!=1 )
		return notify_fail("你要参与的赌局现在不允许下注，无法下注\n");
	
	op = DOBET_D->leasedb("select odds_id,odds_remark,odds from "+DB_ODDS_NAME+" where U_Site='"+INTERMUD_MUD_NAME+"' AND code='"+betcode+"' AND odds_id='"+oddscode+"'");
	
	if(!op || sizeof(op)<1) 
		return notify_fail("没找到任何符合条件的赌局赔率，无法下注\n");

	querystr=sprintf("bet/%s/%d",betcode,oddscode);
	if( !me->query(querystr) )
		imaxchip=chip;
	else
		imaxchip=me->query(querystr)+chip;

	if( imaxchip>ret[0][10] )
		return notify_fail(sprintf("你在该赔率下的注累计不能超过上限%i锭黄金\n",ret[0][10]));

	outstr="┏━━"HIW"【你要参加的赌局为】"NOR"━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓\n";
	outstr += "┃"HIG"赌局代码 赌局内容                       状态     赌局结果          备注                   "NOR"┃\n";
	for(i=0;i<sizeof(ret);i++)
	{
		outstr+= sprintf("┃"HIW"%-8s"NOR" %-30s %8s "HIC"%-15s"NOR" %-25s┃\n",ret[i][0],ret[i][1],
			DOBET_D->getflag(ret[i][3]),
			DOBET_D->getresult(ret[i][0],ret[i][3],ret[i][8]),
			DOBET_D->getmytime(ret[i][3],ret[i][4],ret[i][5],ret[i][6]));
	};
	outstr += "┃赔率的设置为：                                                                            ┃\n";
	outstr+=DOBET_D->getoddsstr(ret[0][0],"┃                           ","        ┃\n");
	outstr += "┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛\n";
	outstr += "你要下的注为：\n";
	outstr += sprintf("投注代码:"HIW"%2i"NOR" 下注内容："HIW"%-20s"NOR" 赔率："HIW"%3d.%-2d"NOR" 你计划下注："HIW"%d锭黄金"NOR"\n",
			oddscode,op[0][1],op[0][2]/100,op[0][2]%100,chip);

	outstr+=sprintf("如果压中，庄家将连本带利付你:%s\n",DOBET_D->money_str(chip*op[0][2]*100));
	outstr+="如果确认下注，请输入y:";
	write(outstr);
	input_to("bet_chip_yes_no", 1, betcode,ret[0][1],oddscode,op[0][1],op[0][2],me->query("id"),chip,me);
	return 1;
}

private void bet_chip_yes_no(string yes_no,string valcode, string valcontext, int valodds_id, string valodds_remark, int valodds,string valchip_id,int valchip, object me)
{
	string querystr;
	int imaxchip;

	if( (yes_no !="y") && (yes_no !="Y") ){
		write("你放弃了本次下注\n");
		return;
	};

	if( DOBET_D->set_chip(valcode, valchip_id, valodds_id, valodds_remark, valodds, valchip) != 1)
		write("本次下注失败\n");
	else{
		write("本次下注成功,投注金已从你的钱庄存款中扣除\n");
		me->add("balance",0-valchip*10000);
		querystr=sprintf("bet/%s/%i",valcode,valodds_id);
		if( !me->query(querystr) )
			imaxchip=valchip;
		else
			imaxchip=me->query(querystr)+valchip;
		me->set(querystr,imaxchip);
		log_file("betchiplog",sprintf("%s在[%s]赌局中压注[%s]赌注%d赔率%d.%d",
			me->query("id"),
			valcontext,
			valodds_remark,
			valchip,
			valodds/100,
			valodds%100));
	};
}

int help(object me)
{
  write(@HELP
--------------------------------------------------
指令格式：bet <赌局代码> <赔率代码> <下注金额>
--------------------------------------------------
 bet 下注
 下注金额以锭黄金为单位
--------------------------------------------------

HELP
    );
    return 1;
}
