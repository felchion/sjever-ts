#include <ansi.h>
#include <mudlib.h>
  
#define DB_DOBET_NAME "dobet_main"
#define DB_ODDS_NAME "dobet_odds"
#define DB_CHIP_NAME "dobet_chip"
  
int main(object me, string arg)
{
	string outstr;
	string betcode,oddsremark;
	int oddscode,odds1,odds2;
	mixed ret;
	int i;
	
	if( !stringp(arg) )
		return notify_fail("你要设置什么?\n");

	if ( !wizardp(me) && ( (!me->query("gamblers"))||(me->query("gamblers")!=1) ) )
		return notify_fail("你没有这个权限?\n");

	sscanf( arg, "%s %d %s %d.%d",betcode,oddscode,oddsremark,odds1,odds2);

	if( !stringp(oddsremark) )
		return notify_fail("赔率说明不能为空?\n");
	
	ret = DOBET_D->leasedb("select code,context,setup_id,flag,begin_time,stop_time,end_time,deposit,bet_result,remark from "+DB_DOBET_NAME+" where U_Site='"+INTERMUD_MUD_NAME+"' AND code='"+betcode+"'");

	if(!ret || sizeof(ret)<1) 
		return notify_fail("没找到任何符合条件的赌局，无法添加赔率\n");
	
	while(odds2>=100)
		odds2=odds2/10;
/*	outstr = "你要添加赔率的赌局设置为：\n";
	for(i=0;i<sizeof(ret);i++)
	{
		outstr+= sprintf("%10s %20s %10s %20s\n",ret[i][0],ret[i][1],DOBET_D->getflag(ret[i][0],ret[i][3],ret[i][8]),
			DOBET_D->getmytime(ret[i][3],ret[i][3],ret[i][4],ret[i][5]));
	};
	outstr += "原来赔率的设置为：\n";
	outstr += DOBET_D->getoddsstr(betcode);
	outstr += "你要添加赔率为：\n";
*/
	outstr="┏━━"HIW"【你要操作的赌局为】"NOR"━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓\n";
	outstr += "┃"HIG"赌局代码 赌局内容                       状态     赌局结果        备注                     "NOR"┃\n";
	for(i=0;i<sizeof(ret);i++)
	{
		outstr+= sprintf("┃"HIW"%-8s"NOR" %-30s %8s "HIC"%-15s"NOR" %-25s┃\n",ret[i][0],ret[i][1],
			DOBET_D->getflag(ret[i][3]),
			DOBET_D->getresult(ret[i][0],ret[i][3],ret[i][8]),
			DOBET_D->getmytime(ret[i][3],ret[i][4],ret[i][5],ret[i][6]));
	};
	outstr+="┣━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫\n";
	outstr += "┃原来赔率的设置为：                                                                        ┃\n";
	outstr+=DOBET_D->getoddsstr(ret[0][0],"┃                           ","        ┃\n");
	outstr+="┣━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫\n";
	outstr += "┃你计划添加的赔率为：                                                                      ┃\n";

	outstr+= sprintf("┃                     赌注代码:"HIW"%2i"NOR" 赌注内容："CYN"%-20s"NOR" 赔率："HIW"%3d.%-2d"NOR"              ┃\n",
			oddscode,oddsremark,odds1,odds2);

	outstr += "┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛\n";
//	outstr += "警告：一个赌局的赔率设置不能超过10个，否则就可能存在问题\n";
	outstr+="如果确认设置，请输入y:";
	write(outstr);
	input_to("odds_add_yes_no", 1, betcode,oddscode,oddsremark,odds1*100+odds2,me);
	return 1;
}

private void odds_add_yes_no(string yes_no,string valcode, int valodds_id, string valodds_remark, int valodds,object me)
{
	if( (yes_no !="y") && (yes_no !="Y") ){
		write("你放弃了本次赔率设置\n");
		return;
	};
	if( DOBET_D->set_odds(valcode, valodds_id, valodds_remark, valodds) != 1)
		write("本次赔率设置失败\n");
	else{
		write("本次赔率设置成功\n");
		log_file("betlog",sprintf("%s %s添加修改赌局%s的赔率%i内容为%s赔率为",
			DOBET_D->longtime(time()),
			me->query("id"),valcode,valodds_id,valodds_remark, valodds));
	};
}

int help(object me)
{
  write(@HELP
--------------------------------------------------
指令格式：betaddodds <赌局代码> <赔率代码> <赔率说明> <赔率>
--------------------------------------------------
 betaddodds 赌局的赔率设置
	赌局代码
	赔率代码:不能设置为0，因为设置结果时0表示比赛没进行或赌局取消,
	赔率说明最简单的就是1主赢2平3客赢
	赔率，含本金，如果是1.8返回的时候就是1.8,存储为赔率*100
警告：一个赌局的赔率设置不能超过10个，否则就可能存在问题 
--------------------------------------------------

HELP
    );
    return 1;
}
