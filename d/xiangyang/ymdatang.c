//Room: /d/xiangyang/ymdatang.c

inherit ROOM;

void create()
{
	set("short", "衙门正厅");
	set("long", @LONG
这里是衙门的正堂，堂上东西有两根楹住，挂着一幅对联，但是你无心细
看。正面墙上悬挂一个横匾，上书“正大光明”四个金光闪闪的大字。两旁衙
役们手持板子侧立两旁。
LONG
	);

	set("exits", ([
        "north" : __DIR__"yamen",
	]));
	set("objects", ([
        __DIR__"npc/lvwende" : 1,
	]));

   setup();
}

