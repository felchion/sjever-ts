//juebi-2.c 绝壁


inherit ROOM;

void create()
{
	set("short", "绝壁");
	set("long", @LONG
光突突的石壁。你只听见耳旁忽忽的风声。
LONG
	);

	set("no_clean_up", 0);
        set("outdoors", "tiezhang");
	setup();
}
void init()
{
        add_action("do_climb","climb");
}

int do_climb(string arg)
{
        object me;
        me=this_player();

        if ( (arg !="up") && (arg !="down") ) 
        return notify_fail("你要往哪儿爬？\n");
        if (arg =="up")  
        {
        write("你吸了口气，小心的向上爬去。\n");
        message("vision",me->name() + "向上一纵突然不见了。\n",environment(me), ({me}) );
        me->move(__DIR__"juebi-3");
        me->receive_damage("jingli",30);
        message("vision",me->name() + "爬了上来。\n", environment(me), ({me}) );
        return 1;
        }
        if (arg =="down")
        {
        write("你攀缘着石壁，缓缓向下爬去。\n");
        message("vision",me->name() + "向下爬去。\n", environment(me), ({me}) );
        me->move(__DIR__"juebi-1");
        me->receive_damage("jingli",10);
        message("vision",me->name() + "爬了下来。\n", environment(me), ({me}) );
        }
        return 1;
}
