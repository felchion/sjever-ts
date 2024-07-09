// wdroad8.c 黄土路

inherit ROOM;

void create()
{
        set("short", "山脚下");
	set("long", @LONG
这里是武当山脚下，两旁是阴森森的树林。西边是一座高山, 东南方是一
茶亭。所谓偷得浮生半日闲! 没有比这更好的歇脚方法了。
LONG
	);
        set("outdoors","武当");

	set("exits", ([
		"east" : "/d/group/entry/wdroad7",
                "southeast" : __DIR__"chating",
                "west" : __DIR__"xuanyue",
	]));

	set("objects", ([
                __DIR__"npc/yetu" : 2 
	]));
        setup();
}
