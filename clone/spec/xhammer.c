#include <ansi.h>


inherit SPEC; 

void create()
{
	set_name (HIY"神匠之锤"NOR, ({"xhammer"}));
	set("long",HIY"一把奇异的锤子。\n");

	set("unit","把");
	set_weight(100);
	set("value",3000000);
		
	set("no_get",1);
	set("no_drop",1);
	set("no_get_from",1); 
	
	set("degree",1);
	set("flag","spec/xhammer");

	set("desc","可以重塑用户武器装备。");
	set("rest",10);
	set("credit",5);	
	setup();
}

int do_use(string arg);
int restrict() {	
	if( sizeof(this_player()->query("worker"))<1 )  
	return notify_fail("你没有任何武器装备。\n");
	
	return  ::restrict();
}

void init()
{
	do_use("hammer");
	add_action("do_use","use");  
}

int do_use(string arg){
  
  if(!arg || arg!="hammer")
  return notify_fail("你要使用什么。\n");
	
	if(WORKER_D->check_user_object(this_player())){
	  write(HIR"神匠之锤化为一道流火，把你所有的装备都重新塑造了一遍。\n"NOR);
    destruct(this_object());
    return 1;
	}	
	else 
	return notify_fail("你现在无法使用神匠之锤。\n");	

}