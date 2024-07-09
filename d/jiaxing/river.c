// Add Log By River@SJ 9.21.2001
#include <ansi.h>
inherit ROOM;
void create()
{
        set("short", GRN "河岸" NOR);
        set("long", @LONG
江南水乡之地到处河泊纵横，前面是小河拦路，无法再行。后面是一片绿
柳林，传出阵阵花香鸟鸣。左边一架小桥，从桥下几叶小舟，隐隐可以听见少
女们正唱着那宛转动人的江南小调。
LONG); 
        set("no_sleep_room", 1);
        set("exits", ([
              "west" : __DIR__"xiaojing1",
        ]));
        set("objects", ([
               __DIR__"npc/xia" : 1,                             
        ]));  
        set("outdoors", "嘉兴");
	setup(); 
}

void init()
{
	object me = this_player();
	int luck = me->query_temp("oyf/meet");

	me->receive_damage("jingli", 10);
	if(!me->query("oyf_son")
	&& (me->query("quest/hama/start") || me->query("quest/hama/move")) 
	&& !random(10)
	&& me->query_temp("lmc/throw")){
		if(!luck && me->query_condition("bing_poison") > 20 &&
		me->query("combat_exp") > 10000){
			write("\n你突然觉得好象有人跟在你身后，转过头来却发现什么也没有。\n\n");
			me->set_temp("oyf/meet", 1);
		}
		else if (luck >= 10) {
			write("\n忽听背後一人说道：「小娃娃，知道厉害了罢？」这声音铿锵刺耳，似从地底下钻出来一般。\n");
			message_vision("\n$N突然吃了一惊，回头便发足狂奔。\n",me);
			me->delete_temp("oyf/meet");
			me->set_temp("oyf/ok", 1);
			me->delete_temp("lmc/throw");
			call_out("wait", 10, me);
		}
		else if (luck >= 1) {
			write("\n你突然觉得好象有人在你身后，转过头来却发现什么也没有。\n\n");
			me->add_temp("oyf/meet", 1);
		}
	}
}

void wait(object me)
{
	int try;

	if(!me || environment(me) != this_object() || !living(me)) return;

	try = me->query("quest/hama/fail");
	if( !random(50) || me->query("quest/hama/move")){
		message_vision(HIR"\n$N只觉手臂麻木，早已不听使唤，只急得大汗淋漓，不知如何是好，慌乱中跑进了柳树林子。\n\n"NOR,me);
		me->set("quest/hama/move", 1);
		log_file("quest/hamagong", sprintf("%8s%-10s失败%d次后，进入柳树林。\n",
		me->name(1),"("+me->query("id")+")", me->query("quest/hama/fail")), me);
		me->move(__DIR__"river1");
	}
	else {
		me->add("quest/hama/fail", 1);
		me->set("quest/hama/time", time());
		me->delete("quest/hama/start");
		log_file("quest/hamagong", sprintf("%8s%-10s第%d次失败进入柳树林。\n",
		me->name(1), "("+me->query("id")+")", me->query("quest/hama/fail")), me);
		message_vision(HIR"\n$N脚下突然一软，骨碌碌地滚出了数十丈！\n\n"NOR,me);
		me->delete_temp("oyf/ok");
		me->move(__DIR__"shulin1");
		me->receive_damage("qi", 100);
		me->receive_wound("qi", 100);
		tell_room(environment(me), HIR + me->name()+"骨碌碌地滚了过来，躺在地上半天爬不起来！\n" NOR,({ me }));
	}
}

int valid_leave(object me, string dir)
{
	if (me->query_temp("oyf/ok") && dir == "west")
	return notify_fail("忽听呼的一声响，一个人影从你头顶跃过，落在你身前。\n");
	return ::valid_leave(me, dir);
}
