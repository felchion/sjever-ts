// /d/xiangyang/zqmnj.c 朱雀门内街
// Lklv 2001.9.24

inherit ROOM;
void create()
{
        set("short", "朱雀门内街");
        set("long", @LONG
这是襄阳城的南门朱雀门的内街，这条路向北通往城中心，向西可以通往
西城，南面就是高大的城门了，出门是往湘赣方向去的大道，城门边的城墙上
长满了青苔，显示着岁月斑驳的痕迹。
LONG
        );
        set("outdoors", "襄阳");

        set("exits", ([
                "south" : __DIR__"zhuquemen",
                "west" : __DIR__"xcx5",
                "north" : __DIR__"njie3",
        ]));
        set("objects", ([
		NPC_D("guanfu/bing") : 2,
        ]));
        set("incity",1);
	setup();
}
