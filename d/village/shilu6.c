// Room: /d/village/shilu6.c

inherit ROOM;

void create()
{
        set("short", "碎石路");
        set("long", @LONG
这是一个宁静的小村子，稀稀落落的分布着几十间土坯泥房，看来村中人
家不多，而且大都生活很艰辛。这是一条东西向的碎石路，南面是间普通的民
房。
LONG
        );
        set("exits", ([
		"east"  : __DIR__"eexit.c",
		"west"  : __DIR__"shilu4.c",
		"south" : __DIR__"minfang2",
	]));
        set("outdoors", "华山");

	setup();
}
