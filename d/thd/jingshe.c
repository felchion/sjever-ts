// /u/jpei/thd/jingshe.c
// Modified by Numa 19990902

#include <ansi.h>
inherit ROOM;

int do_pai(string arg);

void create()
{
        set("short",GRN"精舍"NOR);
        set("long",@LONG
这是桃花岛主黄药师的书房。整间屋子全是以不刨皮的松树搭成，屋中满是诗经典籍玉
器古玩。屋边摆着一张桌子(zhuozi)。西边的墙壁上挂着一幅淡墨山水。
LONG
        );
        set("exits",([
                "south" : __DIR__"shilu.c",
        ]) );
        set("objects",([
                __DIR__"obj/box.c" : 1,
        ]) ); 
        set("no_clean_up",0);
        set("item_desc",([
                "table" : "这是一张石桌。\n",
                "zhuozi" : "这是一张石桌。\n",
        ]) );
        setup();
}

void init()
{
        add_action("do_pai", "pai");
        add_action("do_pai", "press");
}

void close_door(object room)
{
        message("vision", "卷轴自动拉下来，遮住了入口。\n", room);
        room->delete("exits/enter");
}

int do_pai(string arg)
{
        object me = this_player();

        if (me->is_busy() || me->is_fighting())
                return notify_fail("你正忙着呢！\n");
        if( arg == "table" || arg == "zhuozi")
        {
                write("正面壁上挂着的淡墨山水忽地徐徐升起，露出一个暗间来。\n");
                set("exits/enter", __DIR__"liandanfang.c");
                remove_call_out("close_door");
                call_out("close_door", 5, this_object());

                return 1;
        }
}


