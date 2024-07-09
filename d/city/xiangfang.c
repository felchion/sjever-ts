// Room: /d/city/xiangfang.c

inherit ROOM;

void create()
{
	set("short", "客房");
	set("long", @LONG
这里是城隍庙的客房。因为最近庙里要负责给新玩家们发银子，所以比较
忙，而且由于房间较少的缘故所以暂时将客房作为发银子的地方。客房里一位
老僧正忙着给大家分发银子。
LONG	);
	set("exits", ([ /* sizeof() == 1 */
		"out" : __DIR__"chmiao.c",
	])); 
	set("objects", ([ 
		__DIR__"npc/monk" : 1, 
	]));
	set("no_clean_up", 0);
	set("no_fight",1);
	setup();
}
