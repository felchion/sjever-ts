// yaopu8.c
// 药圃
inherit ROOM;
void create()
{
	set("short","药圃");
	set("long",@long
这里是神龙岛的药圃，种了不少珍奇药材。
long);
	set("exits",([
	    "east" : __DIR__"yaopu5",
	    "north" : __DIR__"yaopu9",
	    "south" : __DIR__"yaopu7",
]));
	set("no_clean_up",1);
	set("can_zhong",1);
	set("8type","震");
	set("outdoors", "神龙岛");
	setup();
}
	    	
