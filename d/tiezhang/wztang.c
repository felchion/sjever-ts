// Room: /tiezhang/wztang.c

#include <ansi.h>
inherit ROOM;

string look_bian();

void create()
{
	set("short", HIY"五指堂"NOR);
	set("long", @LONG
这是一间完全用花岗岩砌成的大厅，透出阵阵逼人的寒气。整座房屋没有
窗户，显得阴森黑暗。两旁的墙壁上插着松油火把，散发出昏黄的光亮，寂静
的大厅中不时响起火把噼啪的燃烧声。大厅的正中摆放着帮主的宝座，两旁分
列着十几把椅子，只有遇到帮中大事时，帮主才会在这里与部下商议。在正面
的墙上挂着一面缕金的横匾(bian)。北面有一扇小门通向后面。
LONG
	);
	set("item_desc", ([
		"bian" : (: look_bian :),
	]));

	set("exits", ([
		"east" : __DIR__"zoulang-1",
		"southdown" : __DIR__"guangchang.c",
		"west" : __DIR__"zoulang-2.c",
		"north" : __DIR__"hxfang.c",
	]));
	set("objects", ([
		__DIR__"npc/mayi1" :1,
		__DIR__"npc/huiyi" :2,
	]));
	setup();
}

int valid_leave(object me, string dir)
{
	if (me->query("family/family_name") != "铁掌帮"
	 && present("hong xiaotian", environment(me))
	 && (dir =="west" || dir =="north" || dir=="east"))
		return notify_fail("洪哮天拦住你的去路，抱拳道：非本帮弟子不得入内。\n");

	return ::valid_leave(me, dir);
}

string look_bian()
{
	return
	"\n"
	     HIY"          ※※※※※※※※※※※※※※※※※\n"
		"          ※※　  　　　　　　　　　    ※※\n"
		"          ※※   五    指    擎    天   ※※\n"
		"          ※※  　　　　　　　　　　    ※※\n"
		"          ※※※※※※※※※※※※※※※※※\n"NOR;
}
