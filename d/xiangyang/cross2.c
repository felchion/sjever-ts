// /d/xiangyang/cross2.c 南丁字
// rebuild by Lklv 2001.9.24

inherit ROOM;
void create()
{
        set("short", "南丁字街");
        set("long", @LONG
这里是军事重镇襄阳城中通往南门和东门的丁字路口，不时地有顶盔贯甲
的士兵走来走去。向南出城可以去湖南，向东出城可以去安徽、江苏。北面方
向有一座高大的牌楼。
LONG
        );
        set("outdoors", "襄阳");

        set("exits", ([
                "east" : __DIR__"djie1",
                "north" : __DIR__"dpailou",
		"west" : __DIR__"njie1",
        ]));
        set("objects",([
		__DIR__"npc/wuxiuwen" : 1,
        ]));
        set("incity",1);
	setup();
}