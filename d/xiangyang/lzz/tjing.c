//tjing.c
//Csl 2000.04.26

inherit ROOM;
void create()
{
        set("short", "天井");
        set("long",@LONG
你一走进天井，便会看见彩色方砖铺成的路面，路周围种着四季花草，两
边有几个不错的院落，虽是个小小的天井，布置的也是精巧细致，别具匠心。
LONG
        );
	set("outdoors", "襄阳");
        set("exits",([
		"south"  :  __DIR__"qiant",
		"north"  :  __DIR__"zhengt",
		"west"   :  __DIR__"chelang1",
	]));
	setup();
}