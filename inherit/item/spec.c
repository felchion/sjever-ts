// spec.c
#include <ansi.h>

inherit ITEM;

int restrict()
{
	string this_flag = this_object()->query("flag");
	int this_week=time()/86400/7,k,l; 
	object me = this_player();
	int j = me->query("registered");
	
	l = (j-1)*query("rest");
	if (me->query("spec/last_week")!=this_week){
		me->delete("spec");
		me->set("spec/last_week",this_week);
	}
	
	if (!query("rest")) return 1;	 
	
	k = 1+(me->query(this_flag))/query("rest"); 

  if (j>11) j=11;
  if ( ( k > 1 && !me->query("env/over_buy")) 
  || k > (j-1) ) {
    if (j>2 && !me->query("env/over_buy") && !query("no_overbuy") ) {
      string msg;
      msg = "您现在暂时无法兑换"+query("name")+"。\n本周您已经使用"+query("name")+chinese_number(me->query(this_flag))+"次。\n";
      if(l) msg += (sprintf("您现有会员等级可最多使用%s%s次。\n",
        query("name"),
        chinese_number(l)
      ));   
      return notify_fail(msg + "使用命令"+HIY+"set over_buy 1"+NOR+"后可以高价超限兑换道具。\n");       
    }	
    return notify_fail("本周你已无法使用"+query("name")+"。\n");
  }
  else return k;
}
 
void degree()
{
	string this_flag = query("flag");
  object me = this_player();
  int j = me->query("registered");
  
  if (j>11) j=11;
	me->add(this_flag,1);
	if(query("rest")) write("本周您已经使用"+query("name")+chinese_number(this_player()->query(this_flag))+"次。\n");
  if (j>3 && query("rest") && !query("no_overbuy")) {
    write(sprintf("您现有会员等级可最多使用%s%s次。\n",
      query("name"),
      chinese_number((j-1)*query("rest"))));     
  }
	if (add("degree",-1)<1){
		write("你的"+query("name")+"用完了。\n");
		destruct(this_object());
	}
	else  
		write("你的"+query("name")+"还能使用"+chinese_number(query("degree"))+"次。\n");
}