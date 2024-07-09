// /d/xiangyang/njie1.c 南街
// Lklv 2001.9.24

inherit ROOM;
void create()
{
        set("short", "南街");
        set("long", @LONG
这是由襄阳城中心通往南门的一条大街，路面很宽，两旁种满了郁郁葱葱
的垂柳。行人很多，街上的人群中，偶尔可以看到几个年少英俊的公子哥腰里
佩着宝剑，招摇过市。道西有对红木大门，门前左右摆有一对石狮子。那里是
襄阳府衙的所在。
LONG
        );
        set("outdoors", "襄阳");

        set("exits", ([
                "south" : __DIR__"njie2",
                "north" : __DIR__"xpailou",
                "west" : __DIR__"yamen",
                "east" : __DIR__"cross2",
        ]));

        set("incity",1);
	setup();
}