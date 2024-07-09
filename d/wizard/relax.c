// Modify by Looklove 2000/8/25
#include <ansi.h>
inherit ROOM;

void create()
{
	set("short","桃花源");
	set("long",
"汝缘溪行，忘路之远近。忽逢桃花林，夹岸数百步，中无杂树，芳草鲜美，
落英缤纷，汝甚异之；复前行，欲穷其林。林尽水源，便得一山，山有良田美
池桑竹之属，阡陌交通，鸡犬相闻。男女衣著，悉如外人；黄发垂髫，并怡然
自乐。
    这就是传说中的桃花源，一个让世人神往的地方。
");
	set("no_update", 1);
	set("no_fight", 1);
	set("no_sleep_room", 1);
	set("sleep_room", 1);
	set("valid_startroom", 1);
	setup();
}

void init()
{
	object me = this_player();
	me->set("water",200);
	me->set("food",200);
	if (!wizardp(me)) {
		me->set("startroom", __FILE__[0..<3]);
		add_action("do_action", "");
		if (!me->query_condition("relax"))
			me->apply_condition("relax", 1);
		me->save();
	}
}

int do_action(string arg)
{
	string action = query_verb();
//	object me;
//	int i, j;

	switch (action) {
		case "look":
		case "quit":
			if (!arg)
				return 0;
			return 1;
//		case "pay":
//			if (!arg || !sscanf(arg, "%d", i))
//				return 0;
//			if (!i) return 0;
//			if (i < 0) i = -i;
//			me = this_player();
//			j = (me->query_condition("relax")-1) / 3;
//			if (j < 1)
//				return notify_fail("你马上就要放出去了。\n");
//			if (i > j) i = j;
//			if (me->query("combat_exp") < i * 150)
//				return notify_fail("你的经验不够。\n");
//			me->add("combat_exp", -i * 150);
//			me->add_condition("relax", -i * 3);
//			message_vision("$N用"+chinese_number(i*150)
//				+"点经验换取了大约"+chinese_number(i)+"分钟的提早释放时间。\n", me
//			);
//			return 1;
		default:
			write("这里的环境多么好啊，你歇歇再说吧。\n");
			return 1;
	}
}
