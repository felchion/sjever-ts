// Room: /d/village/eexit.c

inherit ROOM;

void create()
{
	set("short", "东村口");
	set("long", @LONG
这里是华山脚下，往东就是上山的路了。西边是一个小村庄，村子里人家
并不多，村民们过着日出而作，日落而息的宁静生活。
LONG
	);
	set("exits", ([
		"west" : __DIR__"shilu6",
		"east" : "/d/huashan/path1",
	]));

	set("objects", ([
                CLASS_D("gaibang/qigai") : 1,
        ]));
        set("outdoors", "华山");
	setup();
}
