inherit ROOM;

void create()
{
        set("short", "武僧堂");
        set("long", @LONG
这是少林寺护寺武僧练武及居住之所，室内光线明亮，不时听
到呼喝练功之声。一个身强体壮的武僧正自苦练少林武学之秘奥。
LONG
        );
        set("exits", ([
		"south" : __DIR__"wstang3",
		"north" : __DIR__"wstang1",
	]));
        set("objects", ([
		CLASS_D("shaolin") + "/wuseng1":1 ]) );
	set("coor/x",-30);
	set("coor/y",270);
	set("coor/z",120);
	setup();
}
