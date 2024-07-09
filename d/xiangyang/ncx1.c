// /d/xiangyang/ncx1.c 南城巷
// Lklv 2001.9.25

#include <ansi.h>
inherit ROOM;
void create()
{
        set("short", "南城巷");
        set("long", @LONG
这里是由朱雀门通往青龙门的一条小巷，地处襄阳城南，巷口旁边立着一
块石碑(bei)，上写着几个小字。远处的巷口有几个穷汉在躺着晒太阳，从这
里一直往东走，可以通往青龙门。路北是家说书馆。
LONG
        );
        set("outdoors", "襄阳");

        set("item_desc", ([
                "bei" : HIW"南城巷\n"NOR,
                "shibei" : HIW"南城巷\n"NOR
        ]));

        set("exits", ([
                "west" : __DIR__"njie3",
                "east" : __DIR__"ncx2",
                "north" : __DIR__"shuoshug",
        ]));

        set("incity",1);
	setup();
}