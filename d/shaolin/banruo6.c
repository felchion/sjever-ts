// Room: /d/shaolin/banruo6.c
// Date: YZC 96/01/19

inherit ROOM;




void create()
{
	set("short", "般若堂六部");
	set("long", @LONG
这是一间极为简陋的禅房。环视四周，除了几幅佛竭外，室内空荡荡地
别无他物。地上整齐的摆放着几个破烂且发黑的黄布蒲团，几位须发花白的
老僧正在坐在上面闭目入定。这里是本寺十八罗汉参禅修行的地方，不少绝
世武功便是在此悟出。
LONG
	);

	set("exits", ([
		"south" : __DIR__"banruo5",
		"north" : __DIR__"banruo7",
	]));

	set("objects",([
		CLASS_D("shaolin") + "/cheng-xin2" : 1,
	]));
	set("coor/x",110);
  set("coor/y",280);
   set("coor/z",110);
   setup();
}



