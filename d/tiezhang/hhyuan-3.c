//hhyuan-3.c 假山
#include <ansi.h>

inherit ROOM;

void create()
{
    set("short", YEL"假山"NOR);
	set("long", @LONG
你面前矗立着一座巨大的假山，一道片练也似的银瀑从假山边泻将下来，
注入前面的一座大池塘中，池塘底下想是另有泻水通道，是以塘水却不见满溢。
铁掌帮为建这个花园，也不知花费了多少人力、物力。瀑布的后面似乎隐约有
个小门。
LONG	);
	set("exits", ([ 
"east" : __DIR__"hhyuan-1",
]));

	set("no_clean_up", 0);
        set("outdoors", "tiezhang");

	set("coor/x",-340);
  set("coor/y",-230);
   set("coor/z",10);
   setup();
}

void init()
{
         add_action("do_jump", "jump");
}

int do_jump(string arg)
{
        object me;
        me = this_player();

        if (arg !="瀑布") 
            return notify_fail("你要往哪儿跳？？\n");
        {
            write("你纵身向瀑布里跃去。\n");
                message("vision",me->name() + "身行一闪突然不见了。\n",environment(me), ({me}));
            me->move(__DIR__"mishi");
            message("vision",me->name() + "走了过来。\n", environment(me), ({me}));
        }
        return 1;
}
