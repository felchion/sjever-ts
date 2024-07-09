// Room: /d/hengshan/zhandao.c
#include <room.h>
inherit ROOM;
void create()
{
        set("short", "梯式栈道");
        set("long", @LONG
栈道悬于壁，由无数的掾铺成，每节掾宽尺许，九节并排在一起，每根掾
插入岩壁，上铺木板，长约十丈，下临深渊。人必须踏上悬空的木板，紧攀铁
索，屏息静气，缓步挪动，才能过去。
LONG);
        set("exits", ([ /* sizeof() == 4 */
           "southup"   : __DIR__"yunge",
           "northdown" : __DIR__"xuankong2",
        ]));
        set("outdoors", "恒山");
        set("no_clean_up", 0);
        setup();
}
