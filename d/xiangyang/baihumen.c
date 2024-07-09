// /d/xiangyang/baihumen.c 白虎门
// Lklv Modify 2001.9.22

#include <ansi.h>
inherit ROOM;
string look_gaoshi();

void create()
{
	set("short", YEL"白虎门"NOR);
        set("long", @LONG
这里是军事重镇襄阳城的西门，由此向西可往四川、云贵。不时地有顶盔
贯甲的士兵走来走去。城外由于连年的战争，百姓流离失所，造成了大片的荒
地，人烟稀少。城门的墙上张贴着一纸告示(gaoshi)。
LONG
        );
        set("outdoors", "襄阳");
        set("item_desc", ([
                "gaoshi" : (:look_gaoshi:),
        ]));

        set("objects", ([
                NPC_D("guanfu/wujiang") : 1,
                NPC_D("guanfu/bing") : 2,
        ]));
        set("exits", ([
                "west" : __DIR__"outwroad1",
                "east" : __DIR__"bhmnj",
        ]));
        set("incity",1);
	setup();
}

string look_gaoshi()
{
        return FINGER_D->get_killer() + "\n襄阳安抚使\n吕文德\n";
}
