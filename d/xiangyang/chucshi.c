//Room: /d/xiangyang/chucshi.c
// Lklv 2001.9.25

inherit ROOM;
void create()
{
	set("short","储藏室");
	set("long", @LONG
这里以前是衙门里的一间客厅，后来被改成了储藏室，房间里乱七八糟堆
满了东西，角落里有几只老鼠蹿来蹿去。西首的墙上挂着一幅对联，因为年代
久远，已经无法分辨出那上面的字迹。
LONG
	);

	set("exits", ([
		"north" : __DIR__"yamen",
	]));
	set("incity",1);
	setup();
}