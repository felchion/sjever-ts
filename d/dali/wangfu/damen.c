// Room: /dali/damen.c
// bbb 1997/06/11
// update by cool 98.2.20
// Modify By River 98/12
inherit ROOM;
#include <wanted.h>
void create()
{
	set("short", "镇南王府大门");
	set("long", @LONG
这是一座金碧辉煌的府邸，府门前两面大旗，分别绣的是“镇南”,“保
国”两字，府额上写的是“镇南王府”,站了不少亲兵卫士。门口虽站满了人，
但人人屏息凝气，只声不出。你到了这里，不由得也恭谨起来。
LONG
	);
	set("exits", ([ /* sizeof() == 1 */
	    "south" : __DIR__"dating",
	    "west" : "/d/dali/dongjie5",
	]));

	set("objects", ([
	     "/d/dali/npc/bing" : 2,
	]));
	setup();
	"/clone/board/dali_b.c"->foo();
}

int valid_leave(object me, string dir)
{
	if (dir == "south" && is_wanted(me))
		return 0;
	return ::valid_leave(me, dir);
}
