// /d/xiangyang/cross1.c
// rebuild by Lklv 2001.9.24

inherit ROOM;

void create()
{
        set("short", "北丁字街");
        set("long", @LONG
这里是军事重镇襄阳城中通往北门和西门的丁字路口，不时地有顶盔贯甲
的士兵走来走去。向东转走北街出城可以进入陕西、河南境内，还能去参观著
名的武当山。西面直走出城是四川方向，南面方向隐约有一座牌楼。
LONG
        );
        set("outdoors", "襄阳");

        set("exits", ([
                "west" : __DIR__"xjie",
                "east" : __DIR__"bjie3",
                "south" : __DIR__"xpailou",
        ]));
        set("objects", ([
                NPC_D("poorman") : 1,
        ]));

        set("incity",1);
	setup();
}