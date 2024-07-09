// Room: /city2/zhengmen.c

#include <room.h>

inherit ROOM;

string look_gaoshi();

void create()
{
	set("short", "皇宫正门");
	set("long", @LONG
	这便是皇宫正门。巍峨雄伟的城墙上，'紫禁城'三个大字十分显眼，
气势辉煌。一张发黄的告示(gaoshi)贴在墙上，许多官兵立在两旁。
LONG
	);
        set("outdoors", "扬州");
        
         set("item_desc", ([
        	"gaoshi" : (:look_gaoshi:),
	]));

	set("exits", ([
//                 "south" : _DIR_"kedian",
	]));
        set("objects", ([
                __DIR__"npc/wujiang" : 1,
//              __DIR__"/d/city/npc/bing" : 2,
        ]));
	set("outdoors", "city");
	setup();
}

string look_gaoshi()
{
	return "辑拿天地会一众反贼! \n举报一人，赏金十两;\n"
          "捉拿一人，赏金百两。\n知情不报者，立斩!\n鳌拜发\n";
}
