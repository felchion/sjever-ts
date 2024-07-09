// dt.c
//
// 大厅
#include <ansi.h>
inherit ROOM;

void init()
{
	object me = this_player();
	int lvl,lvl1;
	lvl = me->query_skill("canglang-goufa", 1);
	lvl1 = me->query_skill("lingshe-bianfa", 1);
	if (lvl > lvl1 && userp(me)){
		me->delete_skill("canglang-goufa");
		me->set_skill("lingshe-bianfa", lvl);
		write(HIG "系统已将您的「沧浪钩法」转换为「灵蛇鞭法」。\n" NOR);
	}
	lvl = me->query_skill("hook", 1);
	lvl1 = me->query_skill("whip", 1);
	if (lvl > lvl1 && userp(me)){
		me->delete_skill("hook");
		me->set_skill("whip", lvl);
		write(HIG "系统已将您的「基本钩法」转换为「基本鞭法」。\n" NOR);
	}	
}

void create()
{
	set("short","大厅");
	set("long",@long
这是一间硕大无朋的屋子，里面足可容千人之众。大厅彼端居中并排
放着两张竹椅，铺了锦缎垫子，显然是给教主夫妇坐的。此处是神龙教平
日议事之处。
long);
	set("exits",([
	    "north" : __DIR__"th",
           "south" : __DIR__"slj",          
]));
set("objects",([
"/kungfu/class/sld/hant" : 1,
__DIR__"npc/sq" : 1,
__DIR__"npc/ptt" : 1,
__DIR__"npc/stt" : 1,
__DIR__"npc/cyjz" : 2,
]));
	set("coor/x",520);
  set("coor/y",440);
   set("coor/z",90);
   setup();
}
	    	
