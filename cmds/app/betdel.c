#include <ansi.h>
#include <mudlib.h>
  
#define DB_DOBET_NAME "dobet_main"
#define DB_ODDS_NAME "dobet_odds"
#define DB_CHIP_NAME "dobet_chip"
  
int main(object me, string arg)
{
	string outstr;
	mixed ret;
	int i;
	
	if( !stringp(arg) )
		return notify_fail("你要删除什么?\n");

	ret = DOBET_D->leasedb("select code,context,setup_id,flag,begin_time,stop_time,end_time,deposit,bet_result,remark from "+DB_DOBET_NAME+" where U_Site='"+INTERMUD_MUD_NAME+"' AND code='"+arg+"'");

	if(!ret || sizeof(ret)<1) 
		return notify_fail("没找到任何符合条件的赌局\n");

	outstr="┏━━"HIW"【你要删除的赌局为】"NOR"━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓\n";
	outstr += "┃"HIG"赌局代码 赌局内容                       状态     赌局结果        备注                     "NOR"┃\n";
	for(i=0;i<sizeof(ret);i++)
	{
		outstr+= sprintf("┃"HIW"%-8s"NOR" %-30s %8s "HIC"%-15s"NOR" %-25s┃\n",ret[i][0],ret[i][1],
			DOBET_D->getflag(ret[i][3]),
			DOBET_D->getresult(ret[i][0],ret[i][3],ret[i][8]),
			DOBET_D->getmytime(ret[i][3],ret[i][4],ret[i][5],ret[i][6]));
	};
	outstr += "┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛\n";
	outstr+="如果确认删除，请输入y:";
	write(outstr);
	input_to("bet_del_yes_no", 1, arg,me);
	return 1;
}

private void bet_del_yes_no(string yes_no,string valcode,object me)
{
	if( (yes_no !="y") && (yes_no !="Y") ){
		write("你放弃了本次删除\n");
		return;
	};
	if( DOBET_D->delete_bet(valcode) != 1)
		write("本次删除失败\n");
	else{
		write("本次删除成功\n");
		log_file("betlog",sprintf("%s %s删除赌局%s",
			DOBET_D->longtime(time()),
			me->query("id"),valcode));
	};
}

int help(object me)
{
  write(@HELP
--------------------------------------------------
指令格式：betdel <赌局代码>
--------------------------------------------------
 betdel 删除赌局
 
--------------------------------------------------

HELP
    );
    return 1;
}
