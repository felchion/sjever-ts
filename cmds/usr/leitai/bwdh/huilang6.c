//huilang.c 回廊

#include <ansi.h>
#include <room.h>

inherit ROOM;
#include "condition.h"
void create()
{
        set("short",HIC"回廊"NOR);
        set("long", @LONG
这是一条幽静的回廊，两边种满了花卉，还有些药材，显得非常的幽静，
向前是长长的青石路，小路一端的尽头似乎有一道小圆门，路两侧是几间房
子。
LONG
        );

        set("objects",([
             __DIR__"npc/npc"+random(15) : 1,
        ]));

        set("exits", ([
        "west" : __DIR__"caodi4",
        "east" : __DIR__"caodi10",
        "south" : __DIR__"huilang4",
        ]));
        
        set("no_death",1);

        setup();
}

