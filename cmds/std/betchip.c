#include <ansi.h>
#include <mudlib.h>
#include  <database.h>

#define DB_DOBET_NAME "dobet_main"
#define DB_ODDS_NAME "dobet_odds"
#define DB_CHIP_NAME "dobet_chip"

string getchipflag(int valflag,int valodds_id,int valbet_result)
{
//	0设置1开始投注2投注截止3结果输入4派彩结束10暂停使用
	switch(valflag){
		case 0:
			return MAG"  ";
		case 1:
//			return HIG"  ";
			return GRN"  ";
			
		case 2:
//			return HIB"  ";
			return CYN"关";
			
		case 3:
			if( valbet_result == 0 )
				return YEL"退";
			if( valodds_id == valbet_result )
				return BWHT""HIC"赢"HIR""HIC;
			else
				return YEL"赔";
		case 4:
			if( valbet_result == 0 )
				return YEL"退";
			if( valodds_id == valbet_result )
				return BWHT""HIR"赢"NOR""HIR;
			else
				return YEL"赔";
		case 10:
			return HIR"停";
		};
	return HIR"??";
}

int main(object me, string arg)
{
	string outstr,*option,querycode;
	string sql,sql2;
	int ipos;
	int opt_all;
	mixed ret;
	int i;

	sql = "select "+DB_CHIP_NAME+".code,"+DB_DOBET_NAME+".context,"+DB_CHIP_NAME+".chip_id,"+DB_CHIP_NAME+".odds_remark,"
		+DB_CHIP_NAME+".odds_id,"+DB_CHIP_NAME+".odds,"+DB_CHIP_NAME+".chip,"+DB_CHIP_NAME+".chip_time,"
		+DB_DOBET_NAME+".flag,"+DB_DOBET_NAME+".bet_result from "
		+DB_CHIP_NAME+","+DB_DOBET_NAME
		+" where "+DB_CHIP_NAME+".U_Site='"+INTERMUD_MUD_NAME+"' and "+DB_CHIP_NAME+".code="+DB_DOBET_NAME+".code";
	if( !stringp(arg) ) {
		sql += " AND "+DB_CHIP_NAME+".chip_id='"+me->query("id")+"'";
	} else {
		opt_all=0;
		option = explode(arg, " ");
		option -= ({ "" });
		i = sizeof(option);
		querycode="";
		while( i-- )
			if(option[i]=="-a") 
				opt_all=1;
			else
				querycode = option[i];
/*				
考虑让大家都可以看别人的投注
		if( (opt_all==1) && (!wizardp(me) )
			return notify_fail("只有巫师能察看全部人的投注情况。\n");
*/				
		if( opt_all!=1 )
			sql +=" AND "+DB_CHIP_NAME+".chip_id='"+me->query("id")+"'";
		if( querycode!="" )
			sql +=" AND "+DB_CHIP_NAME+".code='"+querycode+"'";
	};

	sql+=" order by "+DB_DOBET_NAME+".flag,"+DB_CHIP_NAME+".chip_id,"+DB_CHIP_NAME+".odds_id,"+DB_CHIP_NAME+".code ";
	ret = DOBET_D->leasedb(sql);
	if(!ret || sizeof(ret)<1) 
		return notify_fail("没找到任何下注记录\n");

	outstr="┏━━"HIW"【书剑赌局下注记录】"NOR"━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓\n";
	outstr += "┃"HIG"   赌局内容                     投注者   下注内容         赔率      赌注   下注时间   "NOR"┃\n";
	outstr+="┣━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫\n";
	ipos=0;
	while(sizeof(ret)>0){
		for(i=0;i<sizeof(ret);i++)
		{
			outstr+= sprintf("┃%2s %-28s %-8s %-16s %3d.%-2d %6d锭 %s"NOR"┃\n",
				getchipflag(ret[i][8],ret[i][4],ret[i][9]),ret[i][1],ret[i][2],ret[i][3],
				ret[i][5]/100,ret[i][5]%100,ret[i][6],DOBET_D->shorttime(ret[i][7]));
		};
		ipos+=sizeof(ret);
		sql2 = sql+sprintf(" LIMIT %d,%d",ipos,ROW_LIMIT);
		ret = DOBET_D->leasedb(sql2);
	};
	outstr += "┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛\n";

//	write(outstr);
	me->start_more(outstr);
	return 1;
}

int help(object me)
{
  write(@HELP
--------------------------------------------------
指令格式：betchip
	betchip <赌局代码>
	betchip -a 
	betchip <赌局代码> -a 
--------------------------------------------------
 betchip 看自己下注的情况
 betchip <赌局代码> 看自己在某个赌局中的下注情况
 betchip -a  看全部玩家下注的情况
 betchip <赌局代码> -a 看全部玩家在某个赌局中的下注情况
--------------------------------------------------

HELP
    );
    return 1;
}
