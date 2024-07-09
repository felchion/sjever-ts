// by XiaoYao

#include <room.h>

// by XiaoYao

inherit ROOM;
void create()
{
	set("short", "六盘山");
	set("long", @LONG
这是一座不太高的小山。山顶望去，瓦蓝的天空飘着几朵淡淡的白云。一
群大雁发出「嘎嘎」的叫声，从头上飞过。西面山下长城的残垣断壁依稀可辨
。路旁有一处悬崖之上刻着首词。北边山下是永登城，东面山下是一处小村庄。
LONG );
	set("exits", ([
		"northdown" : __DIR__"yongdeng",
	]));

	set("outdoors", "黄河");
	setup();
}

