inherit ROOM;
#include <ansi.h>
#include <room.h>

void create()
{
	set("short", "御膳房");
	set("long", @LONG
这里是大理皇宫的御膳房，只见桌上放着十来碟点心糕饼。点心和糕饼看
上去都很精致，显然不是寻常人家的食品。
LONG
	);

	set("exits", ([
		"east" : __DIR__"huating",
	]));
	set("objects", ([
            __DIR__"obj/mixian" : 1
	]) );
	set("coor/x",-320);
  set("coor/y",-420);
   set("coor/z",-20);
   setup();
}


