// tiechui.c

#include <ansi.h>
#include <weapon.h>
inherit HAMMER;

void set_status(int i);
int query_status();
void use();
int set_level(int lv);
int wield ();

void create()
{
	set_name("铁锤", ({ "tiechui", "tie chui", "chui" }));
	set_weight(10000);
	if (clonep())
		set_default_object(__FILE__);
	else {
		set("unit", "柄");
		set("long", "这是一柄铁锤。\n");
		set("olong", "这是一柄铁锤。\n");
		set("value", 5000);
		set("material", "steel");
		set("wield_msg", "$N拿起一柄$n。\n");
		set("unwield_msg", "$N将手中的$n放下。\n");
		set("worker_tool",1);
	}
	set("wield_msg", "$N拿起一柄$n。\n");
	set("unwield_msg", "$N将手中的$n放下。\n");
	set("used",70);
	set("forg_tool",1);
	set("oused",70);
	init_hammer(20);
	set_status(70);
	set("stime",time());
	setup();
}

int set_level(int i)
{
	switch (i) {		
		case 1:
		set_name(HIY"金铁锤"NOR, ({"jintie chui","tiechui","chui"}));
		set("long",HIY"稀有的金铁所铸的锤子。\n"NOR);
		set("material","jintie");
		set("value",7000);
		set("lvl",1);
		break;
		case 2:
		set_name(RED"神铁锤"NOR, ({"shentie chui","tiechui","chui"}));
		set("long",RED"极其罕见的万年神铁所铸的锤子。\n"NOR);
		set("material","shentie");
		set("value",10000);
		set("lvl",2);
		break;
		case 3:
		set_name(WHT"玄铁锤"NOR, ({"xuantie chui","tiechui","chui"}));
		set("long",WHT"绝无仅有的玄铁所铸的锤子。\n"NOR);
		set("material","xuantie");
		set("value",20000);
		set("lvl",3);
		break;
		case 100:
		set_name(HIC"寒铁锤"NOR, ({"hantie chui","tie chui","chui"}));
		set("long",HIC"出产在雪山之巅的冰雪淬炼万年寒铁所铸的锤子。\n"NOR);
		set("material","hantie");
		set("value",40000);
		set("lvl",100);
		break;
		default:
		set_name("铁锤", ({ "tiechui", "tiechui", "chui" }));
		set("long", "这是一柄铁锤。\n");
		set("material", "steel");
	}
	return 1;
}


void init()
{
	add_action("do_ttt","ttt");
}

void set_status(int i)
{
	int j;
	if(query("lvl")>0) return;
	j = (int)(i*100/query("oused"));
	if(j>90) set("long",query("olong")+"看起来还是完好的。\n");
	else if(j>50) set("long",query("olong")+"看起来已经用过一段时间了。\n");
	else if(j>20) set("long",query("olong")+"看起来已经有些破损了。\n");
	else if(j>0) set("long",query("olong")+"看起来就要坏了。\n");
	else set("long",query("olong")+"看起来已经坏掉了。\n");
	set("used",i);
}

int query_status()
{
	return query("used");
}

void use()
{
	object me = environment(this_object());
	if (wizardp(me)) return;
	if( query_status()<=0 ) return;
	if( query_status()>2 && (time()-query("stime"))>7200 ) {
		tell_object(me,"你的"+query("name")+"似乎有些生锈了！\n");
		set_status(2);
	}
	if(query("lvl")>0) set("used",0);
	set_status(query_status()-1);	
	if( query_status()<=0 ) {
		if( objectp(me) && userp(me) )
			tell_object(me,"你的"+query("name")+"坏掉了！\n");
		broken("坏掉的");
	}
}

int wield()
{ 
	object me = environment(this_object());
	if(query("lvl")>0 && query("gift_target") != getuid(me) ) {
		return notify_fail("你从什么地方得到的？\n");
  }
	else 
	return(::wield());
}