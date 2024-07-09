// /d/xiangyang/bcx4.c 北城巷
// Lklv 2001.9.22

inherit ROOM;
void create()
{
        set("short", "北城巷");
        set("long", @LONG
这里是由西门通往北门的一条小巷，因为地处襄阳城北，所以人们把这里
叫做北城巷。走在这条小街上举目望去，巷子两旁是各式各样的店铺，人来人
往，很是热闹。西面有家店铺开着门，是个小吃店。
LONG
        );
        set("outdoors", "襄阳");

        set("exits", ([
                "west" : __DIR__"xiaocd",
                "north" : __DIR__"bcx3",
                "south" : __DIR__"bhmnj",
        ]));

        set("objects", ([
		NPC_D("poorman") : 1,
                __DIR__"npc/gongzi" : 1,
        ]));
        set("incity",1);
	setup();
}