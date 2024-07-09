// puti-zi.c 菩提子

#include <ansi.h>

inherit ITEM;

void init()
{
	add_action("do_eat", "eat");
}

void create()
{
	set_name("菩提子", ({"puti zi", "zi"}));
	set_weight(100);
	if (clonep())
		set_default_object(__FILE__);
	else {
		set("unit", "颗");
		set("unique", 1);
		set("drug", 1);
		set("treasure", 1);
		set("long", "这是一颗晶莹如玉的菩提子，采自灵山佛祖座前，珍稀无比，\n"+
			    "乃万年菩提树之实，功能化百劫，渡千灾，与天地同寿。\n");
		set("value", 500000);
	}
}

int do_eat(string arg)
{
	int force_limit, neili_limit, improve;
	object me = this_player();

	force_limit = me->query_skill("force")* 10 + 20* me->query("age") + me->query("combat_exp") /1000;
	neili_limit = me->query("max_neili");

	if (!id(arg))
		return notify_fail("你要吃什么？\n");

	if (me->query("neili") < me->query("max_neili"))
		return notify_fail("你内力太少，恐怕承受不了菩提子的功效。\n");
  if (neili_limit < force_limit) {
		if (random(10) > 5) {
			improve = 10 + random(10);
			message_vision(HIY "$N吃下一颗菩提子，顿然间只觉一股浩荡无比的真气直冲顶门……\n" NOR, me);
			me->unconcious();
		} else {
			improve = 5 + random(5);
			me->set("neili", 0);
			message_vision(HIY "$N吃下一颗菩提子，顿然间只觉一股浩荡的真气直冲顶门……\n" NOR, me);
		}
		me->add("max_neili", improve);
		log_file("quest/neili",sprintf("%s %s(%s)吃了%s提升内力%s点。\n",
			ctime(time())[4..19],me->name(1),capitalize(me->query("id")),name(),chinese_number(improve)));
	} else {
		message_vision(HIY "$N不顾内力修为已经到了极限，吃下一颗菩提子。\n" NOR, me);
		me->unconcious();
	}
	destruct(this_object());
	return 1;
}
