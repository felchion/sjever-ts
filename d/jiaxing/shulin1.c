// /d/jiaxing/shulin1.c 江南树林

inherit ROOM;

void create()
{
        set("short", "树林");
        set("long", @LONG
江南难得一见的大片树林，周围黑漆漆一团, 你几乎迷了路。
LONG
        );

        set("exits", ([
		"east" : __DIR__"lumu",
                "south" : __DIR__"luzhuang",
                "northwest" : __DIR__"xiaojing1",
        ]));
        set("outdoors", "嘉兴");
	setup();
}

void init()
{
	object me = this_player();
	int luck = me->query_temp("oyf_meet");
	if(!me->query("oyf_son") && me->query("oyf_fail") < 4) {
		if(random((int)me->query("kar")) == 5 &&
		me->query_condition("bing_poison") > 30 &&
		me->query("combat_exp") > 10000){
			if(!luck) {
				write("你突然觉得好象有人在你身后，转过头来却发现什么也没有。\n");
				me->set_temp("oyf_meet", 1);
			}
			if (luck >= 1) {
				write("你突然觉得好象有人在你身后，转过头来却发现什么也没有。\n");
				me->add_temp("oyf_meet", 1);
			}
		}
	}
}
