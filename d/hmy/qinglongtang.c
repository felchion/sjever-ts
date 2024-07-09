//  /d/hmy/qinglongtang.c

inherit ROOM;

void create()
{
	set("short", "青龙堂");
	set("long", @LONG
大堂设许多各式的刑具，在微弱的烛光之下，散发着冰冷的寒光，令人
越看越是心中发毛，想像受刑之人那种惨状及那凄惨的叫声，心中更感
到一阵惊怖。
LONG
	);
	set("exits", ([ 
	  "east" : __DIR__"changlang2",
]));
        set("objects", ([ 
          __DIR__"npc/shang" : 1,
]));
	setup();
}
