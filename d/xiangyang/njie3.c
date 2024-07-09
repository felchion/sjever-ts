// /d/xiangyang/njie3.c 南街
// Lklv 2001.9.24

inherit ROOM;
void create()
{
        set("short", "南街");
        set("long", @LONG
这是由襄阳城中心通往南门的一条大街，路面很宽，两旁种满了郁郁葱葱
的垂柳。地面上铺着大块的青石，平坦整洁。向南望去，隐约可以看见城门上
的城楼和女儿墙。路西面有一家铺子开着门，门上画着一个很大的铜钱，那是
家钱庄。
LONG
        );
        set("outdoors", "襄阳");

        set("exits", ([
                "south" : __DIR__"zqmnj",
                "west" : __DIR__"bank",
                "east" : __DIR__"ncx1",
                "north" : __DIR__"njie2",
        ]));

        set("incity",1);
	setup();
}