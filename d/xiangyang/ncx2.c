// /d/xiangyang/ncx2.c 南城巷
// Lklv 2001.9.25

#include <ansi.h>
inherit ROOM;
void create()
{
        set("short", "南城巷");
        set("long", @LONG
这里是由朱雀门通往青龙门的一条小巷，地处襄阳城南。从这里一直往东
走，可以通往青龙门。远处的巷口有几个穷汉在躺着晒太阳，路北是一家车马
行。门口着挂一个木牌(pai)。
LONG
        );
        set("outdoors", "襄阳");

        set("item_desc", ([
                "mupai" : HIY"全国各地，行李托运\n"NOR,
                "pai" : HIY"拉脚：通行各地\n"NOR
        ]));

        set("exits", ([
                "west" : __DIR__"ncx1",
                "east" : __DIR__"ncx3",
                "north" : __DIR__"chemahang",
        ]));

	set("objects",([
		CLASS_D("gaibang/guojing") : 1,
	]));

        set("incity",1);
	setup();
}