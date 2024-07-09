//ROOM: /d/dzd/shulin3.c

inherit __DIR__"dzd_room";

void create()
{
	set("short","树林");
	set("long",@LONG
树木稀疏地长着，但还算得上是片树林。脚下是半土半沙的地面，但比起溪流对岸的
草地来是好走多了。一些山石杂错在树丛中，里面似乎躲藏着一些东西，奇怪的是竟然听
不到一丝声音。
LONG
	);
	set("outdoors","大智岛");
	set("exits",([	
		"west" : __DIR__"shulin2",
		"northeast" : __DIR__"senlin3",
		"northwest" : __DIR__"senlin2",
		"southwest" : __DIR__"haitan3",
	]));
	setup();
}