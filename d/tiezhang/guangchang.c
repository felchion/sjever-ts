//guangchang.c 广场

inherit ROOM;

void create()
{
	set("short", "广场");
	set("long", @LONG
山路行到这里，忽然变得开阔，你眼前出现了一片很大的广场。一些铁掌
帮的弟子来回穿梭于其间，显得很忙碌。在广场的周围有很多岩石建的房子。
正面的这间尤其显得高大雄伟。左面的房子中不时的涌出阵阵热气，其间还加
杂着鼓风的声音。
LONG
	);

	set("exits", ([ 
"south" : __DIR__"shanlu-2.c",
"west" : __DIR__"lgfang",
"east" : __DIR__"guajia",
"northup" : __DIR__"wztang",
]));

    set("objects", ([
__DIR__"npc/mayi4" : 1,
__DIR__"npc/huiyi" : 2,
    ]));

        set("valid_startroom", 1);
	set("no_clean_up", 0);
	set("outdoors", "tiezhang");

	set("coor/x",-330);
  set("coor/y",-260);
   set("coor/z",0);
   setup();
        "/clone/board/tiezhang_b"->foo();
}
