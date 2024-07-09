// fenduo2.c 扬州坟墓墓室(gb大勇分舵)
#include <ansi.h>
inherit ROOM;

void create()
{
	set("short","墓室");
	set("long", @LONG
这是一个很大的墓室，里面坐着不少人。墙上挂满了火炬，照得光
亮亮的。原来这就是丐帮的大勇分舵，一群乞丐围着地图，似乎正在激
烈地讨论着什么。墙角处还有一口枯井(well)。
LONG
	);
	set("valid_startroom",1);
	set("item_desc",([
	"well" : "里面黑蒙蒙的，不知通向哪里。\n",
	]));
            set("exits", ([
                      "north" : __DIR__"mushizl",                
          ]));
	set("objects",([
                CLASS_D("gaibang") + "/xi" : 1,
//		__DIR__"npc/fangqiu" : 1,
                __DIR__"npc/jydizi" : 1,
	]));
	set("coor/x",110);
  set("coor/y",60);
   set("coor/z",-10);
   setup();
          call_other("/clone/board/gb_board2", "???");
}

void init()
{
        object me = this_player();
        if ( me->query("title") == "丐帮大勇分舵零袋弟子")
        {
                me->set("title","丐帮大勇分舵一袋弟子");
        }
	add_action("do_jump","jump");
}

int do_jump(string arg)
{
        object me;
        me = this_player();

        if( arg=="well" )
        {
		if (me->is_busy())
			return notify_fail("你正忙着呢。\n");
                message("vision",
                        me->name() + 
	 	        "深深地吸了一口气，一闭眼，往井里跳了下去。\n",
                        environment(me), ({me}) );
                me->move("/d/gb/shangang");
                message("vision",
                        me->name() + "从坟墓里冒了出来。\n",
                        environment(me), ({me}) );
                return 1;
        }
}

