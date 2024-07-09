// Room: /d/dali/nj.c
// bbb 1997/06/10 

inherit ROOM;

void create()
{
	set("short", "怒江");
	set("long", @LONG
怒江发源于青藏高原的唐古拉山南麓，上游藏语叫做“那曲”，即“黑
水河”，流入他念他翁山和伯舒拉岭之间的峡谷中，始称“怒江”。怒江奔
流于怒山和高黎贡山之间，两岸山第拔多在三千米以上，山谷幽深，河流落
差大，水急滩高，有“一滩接一滩，一滩高十丈”的说法，十分气派。两岸
多危崖，有“水无不怒石，山有欲飞峰”之称。
LONG
	);
        set("outdoors", "dali");

	set("exits", ([
           "north" : __DIR__"futiao",
            "southup" : __DIR__"xueshan",
           "northeast" : "/d/tls/diancang",
	]));

	setup();
}
