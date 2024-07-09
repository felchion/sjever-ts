// /d/xiangyang/bcx2.c 北城巷
// Lklv 2001.9.22

inherit ROOM;
void create()
{
        set("short", "北城巷");
        set("long", @LONG
这里是由西门通往北门的一条小巷，因为地处襄阳城北，所以人们把这里
叫做北城巷。走在这条小街上举目望去，巷子两旁是各式各样的店铺，人来人
往，很是热闹。这里的北面有一家猪肉铺子，南面是个小广场，广场上人声鼎
沸，远远看去可以看见一个擂台。
LONG
        );
        set("outdoors", "襄阳");

        set("exits", ([
                "south" : __DIR__"guangc",
                "north" : __DIR__"zrdian",
                "west" : __DIR__"bcx3",
                "east" : __DIR__"bcx1",
        ]));

        set("objects", ([
		__DIR__"npc/wudunru" : 1,
        ]));
        set("incity",1);
	setup();
}