// cailiao hang
// by augx@sj  3/21/2002

#include <ansi.h>
inherit ROOM;

void create()
{
	set("short", HIW"材料行"NOR);
	set("long", 
"这是家生意兴隆的材料行，附近的人都到这里来买建造房屋的材料。这儿
还经常有帮会人物出没，进进出出的不停的搬货。听说这儿的掌柜很很多帮会
的老大关系都不错，帮会在这里买东西都不需要付现钱，记帐就可以了。
");
	set("indoors", "成都城");

	set("exits", ([
		"south" : "/d/chengdu/ddajie1",
	]));
	set("objects", ([
		__DIR__"npc/wang-zhanggui" : 1,
	]));

	set("incity",1);
	setup();
}
