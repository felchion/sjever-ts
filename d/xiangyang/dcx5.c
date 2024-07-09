// /d/xiangyang/dcx4.c 东城巷
// Lklv 2001.9.25

#include <ansi.h>
inherit ROOM;
void create()
{
        set("short", "东城巷");
        set("long", @LONG
这里是由青龙门通往玄武门的一条小巷，地处襄阳城东，巷口旁边立着一
块石碑(bei)，上写着几个小字。远处的巷口有几个穷汉在躺着晒太阳，从这
里一直往北走，可以通往玄武门。
LONG
        );
        set("outdoors", "襄阳");

        set("item_desc", ([
                "bei" : HIW"东城巷\n"NOR,
                "shibei" : HIW"东城巷\n"NOR
        ]));

        set("exits", ([
                "north" : __DIR__"dcx4",
                "south" : __DIR__"djie2",
        ]));

        set("incity",1);
	setup();
}