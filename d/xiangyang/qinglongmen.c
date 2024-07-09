// /d/xiangyang/qinglongmen.c

#include <ansi.h>
inherit ROOM;
string look_gaoshi();

void create()
{
        set("short", YEL"青龙门"NOR);
        set("long", @LONG
这里是军事重镇襄阳城的东门，不时地有顶盔贯甲的士兵走来走去。向东
边走可以进入安徽、江苏境内。城门的墙上张贴着一纸告示(gaoshi)。
LONG
        );
        set("outdoors", "襄阳");

        set("item_desc", ([
                "gaoshi" : (:look_gaoshi:),
        ]));

        set("exits", ([
                "east" : "/d/xiangyang/outeroad1",
                "west" : "/d/xiangyang/eroad1",
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
