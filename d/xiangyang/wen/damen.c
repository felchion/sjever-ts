// Room: damen.c
// Lklv Modify 2001.9.21

inherit ROOM;
void create()
{
	set("short", "温府大门");
	set("long", @LONG
你走到一座高宅大院的门口，大门敞开，门口站着两个家丁，一个个腆胸
叠肚，一副要找人麻烦的样子。门前左右有两个石狮子，四五级青石台阶上是
两扇钉满铜钉的大门。看得出这里是当地大豪的住所。
LONG
        );
        set("objects", ([
                __DIR__"npc/jiading":2,
        ]));

        set("exits", ([
                "south": __DIR__"shidetang",
                "north": "/d/xiangyang/ncx3",
        ]) );
	set("incity",1);
	setup();
}