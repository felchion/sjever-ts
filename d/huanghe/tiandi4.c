// by XiaoYao

#include <room.h>

inherit ROOM;
void create()
{
	set("short", "田地");
	set("long", @LONG
这里是一大片农田。不远处有一些村庄，炊烟袅袅升起。村庄周围是一块
块的田地，田里有一些正在耕作的农人。村里不时传出一声声的犬吠。
LONG );
	set("exits", ([
		"southwest" : __DIR__"tiandi3",
		"north"     : __DIR__"huanghe1",
                "east"     : "/d/taishan/hsroad2",
	]));
	set("objects", ([
		NPC_D("dog") : 1,
	]));
	set("no_clean_up", 0);
	set("outdoors", "黄河");
	setup();
}
