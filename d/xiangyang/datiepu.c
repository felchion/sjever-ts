// Room: /d/xiangyang/datiepu.c
// Lklv 2001.9.26

inherit ROOM;

void create()
{
        set("short", "打铁铺");
        set("long", @LONG
这是一家打铁铺，地当中摆着一个火炉，炉火把四周照得一片通红，火炉
旁边有个很大的风箱，这里空气都是火热的。东面的架子上放了几把未完工的
刀具，地上扔着一个大锤，旁边的桌子上有杯茶冒着热气，看来铁匠好像刚离
开不久，
LONG
        );

        set("exits", ([
		"west" : __DIR__"ncx5",
        ]));

	set("incity",1);
	setup();
}
