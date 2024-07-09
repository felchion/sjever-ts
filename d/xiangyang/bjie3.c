// /d/xiangyang/bjie3.c 北街
// Lklv 2001.9.24

inherit ROOM;
void create()
{
        set("short", "北街");
        set("long", @LONG
这是由北门通往城中心的街道，路面很宽，两旁种着柳树。街道上人来人
往，很是热闹。人群里面好象还有不少武林中人，偶尔有几个衣帽光鲜的少年
人牵马结伴穿闹市而过。站在这里向南面望去，可以看见一座牌楼，西面是一
条青石路，通往城西。
LONG
        );
        set("outdoors", "襄阳");

        set("exits", ([
                "south" : __DIR__"dpailou",
                "north" : __DIR__"bjie2",
                "west" : __DIR__"cross1",
        ]));

        set("incity",1);
	setup();
}