// Write By Yanyang@SJ 2001.10.13
// Room: /d/changan/southroad3.c

inherit ROOM;

void create()
{
	set("short", "官道");
	set("long", @LONG
这是一条宽阔笔直的官道，足可容得下十马并驰。四周人来人往。挑担子
的行商，赶着大车的马夫，上京赶考的书生，熙熙攘攘，非常热闹。不时还有
两三骑快马从身边飞驰而过，扬起一路尘埃。道路两旁是整整齐齐的杨树林。
LONG
	);
	set("outdoors", "长安");
	set("exits", ([
		"west"   : "/d/xiangyang/caiyongmanor",
		"south"  : "/d/xiangyang/lantian",
		"north"  : __DIR__"southroad2",
	]));
	set("objects", ([
		CLASS_D("gaibang/qigai") : 1,
	]));
	setup();
}
