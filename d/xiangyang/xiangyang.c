// /d/xiangyang/xiangyang.c

inherit ROOM;

void create()
{
	set("short", "襄阳城");
	set("long", @LONG
这里是军事重镇襄阳城，不时地有顶盔贯甲的士兵走来走去。向北可以进
入陕西、河南境内，还能去参观著名的武当山，向东边走可以进入安徽境内，
往西边走是通往四川的大路，南边则是去湖南的大路。
LONG
	);
        set("outdoors", "襄阳");

	set("exits", ([
		"east" : __DIR__"outeroad1",
//		"west" : "/d/wudang/wdroad6",
		"north" : __DIR__"outnroad1",
		"south" : __DIR__"outsroad1",
	]));

	setup();
}

