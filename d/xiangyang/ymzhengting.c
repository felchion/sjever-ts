//Room: /d/xiangyang/ymzhengting.c
// Lklv 2001.9.25

#include <ansi.h>
inherit ROOM;
void create()
{
	set("short",HIR"正厅"NOR);
	set("long", @LONG
这里是衙门的正厅，堂上东西有两根楹柱，挂有一幅对联，但是你无心细
看。正面墙上悬挂一个横匾，上书“正大光明”四个金光闪闪的大字。两旁衙
役们手持板子侧立在两旁，气氛庄严肃穆。
LONG
	);

	set("exits", ([
		"south" : __DIR__"yamen",
	]));
	set("objects", ([
		__DIR__"npc/lvwende" : 1,
	]));

	set("incity",1);
	setup();
}