// /d/xiangyang/zhuquemen.c

#include <ansi.h>
inherit ROOM;
string look_gaoshi();

void create()
{
        set("short", YEL"朱雀门"NOR);
        set("long", @LONG
这里是军事重镇襄阳城的南门，因为这边离战场较远，所以显得繁华热闹
得多。向南出城可以去湖南。城门的墙上张贴着一纸告示(gaoshi)。
LONG
        );
        set("outdoors", "襄阳");

        set("item_desc", ([
                "gaoshi" : (:look_gaoshi:),
        ]));

        set("exits", ([
                "north" : __DIR__"zqmnj",
                "south" : __DIR__"outsroad1",
        ]));

        set("objects", ([
                NPC_D("guanfu/wujiang") : 1,
                NPC_D("guanfu/bing") : 2,
	]));

	set("incity",1);
	setup();
}

string look_gaoshi()
{
        return FINGER_D->get_killer() + "\n襄阳安抚使\n吕文德\n";
}
