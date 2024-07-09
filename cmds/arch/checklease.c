
#include <ansi.h>
#ifndef ROOMLEASE_D
#define ROOMLEASE_D "/adm/daemons/roomleased"
#endif


int help(object me);

int main(object me,object arg)
{
	mapping ma;
	string* leasemap;
	float x;
	if(!arg){
		leasemap=ROOMLEASE_D->query_keys();
		if(leasemap) write(sprintf("目前可用：%s\n",implode(leasemap,",")));
		return help(me);
	}
	
	if( !ROOMLEASE_D->query_ifhavebidname(arg))
		return notify_fail(HIY"没有这个店铺。\n"NOR );
	
	ma = ROOMLEASE_D->query_amap(arg);
	x = (time()-ma["settime"]) / 3600;
	if(!x || x<=0 ) x = 1.0;//不到一个小时算一个小时先
	//write(sprintf("money:%d,时间差别：%d\n",ma["repayamount"],time()-ma["settime"] ));
	//x = (time()-ma["settime"])/3600;
	write(sprintf("%s 从 %s 开始 到 %s 总收入 %s，每单位小时收入：%s。\n",arg,
	ROOMLEASE_D->longtime(ma["settime"]),
	ROOMLEASE_D->longtime(time()),
	MONEY_D->money_str(ma["repayamount"]),
	MONEY_D->money_str( to_int(ma["repayamount"]/x)),
	 ));
		
	return 1;
}

int help(object me)
{
	write(@HELP
	格式 checklease <id>
	
	检查该房间的经营状况。
HELP);
	return 1;
}

