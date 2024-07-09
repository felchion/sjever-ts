// Room: /d/wizard/wizard_room.c

inherit ROOM;

void create()
{
        set("short", "巫师会议室");
        set("long", @LONG
在宽敞明亮的大会议室里，正中放着一张红木大圆桌。巫师通常都在这里
讨论通过一些重要建设和修改的提案。这里有留言板总汇，南北和东边的通道
都通往比武擂台，上面的通道通向试剑山庄控制室。
LONG );
	set("exits", ([
		"west" : __DIR__"wizard_room",
		"tiandi" : "/cmds/leitai/leitai",
		"fengyun" : "/cmds/leitai/leitai1",
		"longhu" : "/cmds/leitai/leitai2",
		"down" : __DIR__"guest_room",
		"up" : "/cmds/leitai/bwdh/ctrl",
        ]));
	set("valid_startroom", 1);
        set("no_fight", 1);
	set("incity",1);
	setup();
        call_other("/clone/board/post_b", "???");
}

void init()
{
  	object me = this_player();

  	if (userp(me) && !wiz_level(me)) {
    		me->move("/d/wizard/guest_room");
    		message_vision("$N从巫师休息室重重地摔了下来。\n", me);
    		me->unconcious();
  	}
}
