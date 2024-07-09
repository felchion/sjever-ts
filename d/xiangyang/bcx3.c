// /d/xiangyang/bcx3.c 北城巷
// Lklv 2001.9.22

inherit ROOM;
void create()
{
        set("short", "北城巷");
        set("long", @LONG
这里是由西门通往北门的一条小巷，因为地处襄阳城北，所以人们把这里
叫做北城巷。走在这条小街上举目望去，巷子两旁是各式各样的店铺，人来人
往，很是热闹。北面是一家客栈，西面有家店铺，门口挂有一幅幌子，上面画
着一条鱼。
LONG
        );
        set("outdoors", "襄阳");

        set("exits", ([
                "west" : __DIR__"xyudian",
                "north" : __DIR__"jianghukz",
                "south" : __DIR__"bcx4",
                "east" : __DIR__"bcx2",
        ]));

        set("objects", ([
		NPC_D("girl") : 1,
        ]));
        set("incity",1);
	setup();
}