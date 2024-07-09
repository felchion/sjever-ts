//这个指令几百年没人动了 LinuX@SJ

inherit F_CLEAN_UP;

#include <ansi.h>

int main(object me, string arg)
{
	object ob;

	if (!arg || !(ob = present(arg, environment(me))) || me == ob || !ob->is_character())
		return notify_fail("你要嫁给谁？\n");

	if (me->query("gender") != "女性") {
		message_vision("$N不怀好意地盯着$n说：小相公，是不是很寂寞啊？\n", me, ob);
		return 1;
	}

	if (me->query("marry"))
		return notify_fail("你已经结婚了。\n");

	if (me->query_temp("marry"))
		return notify_fail("你已经订婚了。\n");

	switch (me->query("class")) {
		case "bonze":
                case "huanxi":if(me->query_skill("huanxi-chan",1)>180) break;
		case "lama":
		case "taoist":
			return notify_fail("你先还俗再说吧。\n");
	}

	switch (ob->query("class")) {
		case "bonze":
                case "huanxi":if(ob->query_skill("huanxi-chan",1)>180) break;
		case "lama":
		case "taoist":
			return notify_fail("等他先还俗再说吧。\n");
	}

	if (me->query("age") < 18) {
		message_vision("$N想嫁给$n，不过$N还小，再等几年吧。\n", me, ob);
		return 1;
	}

	if (ob->query("gender") != "男性")
		return notify_fail("你想嫁的人不是男性。\n");
//
//	if (ob->query("registered") < 3)
//		return notify_fail("你只能嫁给正式玩家。\n");

	if (ob->query_temp("pending/marry") != me)
		return notify_fail("你想嫁给"+ob->name()+"，不过"+ob->name()+"并没有向你求婚。\n");

	message_vision("$N羞答答地答应嫁给$n。\n", me, ob);
	tell_object( ob, YEL+"哇，" + me->name() + "答应了你的求婚了。\n"+NOR );
	ob->delete_temp("pending/marry");
	me->set_temp("marry", ob);
	ob->set_temp("marry", me);
	return 1;
}

int help(object me)
{
    write(@HELP
指令格式: marry <某个求婚者ID>

答应你的某个求婚者的求婚。
这个指令女性专用。

相关的指令：propose
相关的帮助：help marriage

HELP
    );
    return 1;
}
