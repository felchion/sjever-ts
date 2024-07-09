// leitai.c 擂台前广场

#include <ansi.h>
inherit ROOM;
void create()
{
	set("short","擂台前广场");
	set("long",@long
这里是襄阳擂台前面的一个大型广场，有许多人正在擂台上比武，到处
是人山人海，如果你也想试试身手，那么就请上擂台比试比试吧。
long);
	set("exits",([
		"tiandi" : "/d/wizard/lt",
		"fengyun" : "/d/wizard/lt1",
		"longhu" : "/d/wizard/lt2",
		"north" : __DIR__"bcx2",
	]));
	set("outdoors", "襄阳");
	set("incity",1);
	setup();
}

int valid_leave(object me,string dir)
{
	object *ob;
	int i;

	if (dir != "north" && (!userp(me) || sizeof(me->query_entire_conditions())))
		return notify_fail(HIW"\n你还有要事在身，不得进入擂台重地！\n"NOR);
	if ( dir != "north" && me->query("no_quest"))
		return notify_fail("\n");
	ob = deep_inventory(me);
	i = sizeof(ob);
	while (i--)
	if ((ob[i]->is_character() || ob[i]->query("unique")) && dir != "north")
		return notify_fail(HIW"\n你不得带人或带宝物进入擂台重地。\n"NOR);
	return ::valid_leave(me, dir);
}
