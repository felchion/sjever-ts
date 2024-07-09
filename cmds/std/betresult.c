#include <ansi.h>
#include <mudlib.h>
  
#define DB_DOBET_NAME "dobet_main"
#define DB_ODDS_NAME "dobet_odds"
#define DB_CHIP_NAME "dobet_chip"
  
int main(object me, string arg)
{
	string outstr;
	string betcode;
	int oddscode;
	mixed ret;
	mixed op;
	int i;
	
	if( !stringp(arg) )
		return notify_fail("你要设置什么?\n");

	//if ( !wizardp(me) && ( (!me->query("gamblers"))||(me->query("gamblers")!=1) ) )
	if ( !wizardp(me) )
		return notify_fail("你没有这个权限?\n");

	sscanf( arg, "%s %d",betcode,oddscode);

	if( oddscode!=0 ){
		op = DOBET_D->leasedb("select odds_id,odds_remark,odds from "+DB_ODDS_NAME+" where U_Site='"+INTERMUD_MUD_NAME+"' AND code='"+betcode+"' AND odds_id='"+oddscode+"'");
		if(!op || sizeof(op)<1) 
			return notify_fail("没找到任何符合条件的赌局赔率\n");
	};
	
	ret = DOBET_D->leasedb("select code,context,setup_id,flag,begin_time,stop_time,end_time,deposit,bet_result,remark from "+DB_DOBET_NAME+" where U_Site='"+INTERMUD_MUD_NAME+"' AND code='"+betcode+"'");

	if(!ret || sizeof(ret)<1) 
		return notify_fail("没找到任何符合条件的赌局，无法删除赔率\n");

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
	if( oddscode!=0 ){
		outstr+="┣━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫\n";
		outstr += "┃你设置中奖的赔率为：                                                                      ┃\n";
		outstr+= sprintf("┃                     赌注代码:"HIW"%2i"NOR" 赌注内容："CYN"%-20s"NOR" 赔率："HIW"%3d.%-2d"NOR"              ┃\n",
			op[0][0],op[0][1],op[0][2]/100,op[0][2]%100);
		outstr += "┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛\n";
		outstr+=HIW"如果确认中奖设置，请输入y:"NOR;
	}else{
		outstr += "┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛\n";
		outstr += HIR"你计划"HIW"取消"HIR"本次赌局,如果确认，请输入y:\n"NOR;
	}
	write(outstr);
	input_to("odds_result_yes_no", 1, betcode,oddscode,me);
	return 1;
}

private void odds_result_yes_no(string yes_no,string valcode, int valodds_id,object me)
{
	string sql;
	
	if( (yes_no !="y") && (yes_no !="Y") ){
		write("你放弃了本次删除\n");
		return;
	};
	sql = "UPDATE "
	+DB_DOBET_NAME+" SET "	
	+"flag='3', "
	+"bet_result='"+valodds_id+ "'"
	+" WHERE code='"+valcode+"' AND U_Site='"+INTERMUD_MUD_NAME+"'";

	if(DOBET_D->leasedb(sql)){
		write("中奖设置成功\n");
		log_file("betlog",sprintf("%s设置赌局结果为%i",
			me->query("id"),valodds_id));
	}else
		write("中奖设置失败\n");
}

int help(object me)
{
  write(@HELP
--------------------------------------------------
指令格式：betresult <赌局代码> <中奖的赔率代码>
--------------------------------------------------
 betresult 设置赌局的结果
如果将 <中奖的赔率代码>设置为0表示取消本次赌局
--------------------------------------------------

HELP
    );
    return 1;
}
