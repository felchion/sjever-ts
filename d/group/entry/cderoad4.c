// Room: eroad4.c 小路
// By jpei

inherit ROOM;

void create()
{
	set("short", "大道");
	set("long", @LONG
你来到了川鄂交界的小路上，偶尔会有人骑着马匆匆而过，卷起飞扬的尘
土。西边是通向故蜀成都的大道，往东北则进入湖北。
LONG);
	set("outdoors", "成都城");
	set("group", 1);
	set("exits", ([
		"east" : "/d/xiangyang/xiaolu2",
		"west" : "/d/chengdu/eroad3",
	]));

	setup(); 
}