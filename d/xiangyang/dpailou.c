// /d/xiangyang/dpailou.c 东牌楼
// Lklv 2001.9.25

#include <ansi.h>
inherit ROOM;
void create()
{
        set("short", HIW"东牌楼"NOR);
        set("long",
"这里是襄阳城中心的一个广场，广场中央立有一座汉白玉的牌楼，牌楼很\n"
"高，最上面镂刻着两条腾空飞舞的"HIC"青龙"NOR"，龙的中间雕有一头昂首挺胸的"HIY"麒麟"NOR"瑞\n"
"兽，看起来威武神气，气势不凡。北边有一条青石大道，一直通往襄阳城的北\n"
"大门：玄武门。南面是一条热闹的丁字街。\n"
        );
        set("outdoors", "襄阳");

        set("exits", ([
                "south" : __DIR__"cross2",
                "north" : __DIR__"bjie3",
        ]));
        set("objects", ([
                CLASS_D("gaibang/qigai") : 1,
        ]));

        set("incity",1);
	setup();
}