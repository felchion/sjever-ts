// train.c 训兽
// by augx@sj  6/27/2002

#include <ansi.h>
inherit F_CLEAN_UP;

int main(object me, string arg)
{
	object target;
	int lvl,exp;

	if( me->is_busy() )
		return notify_fail("你正忙着呢。\n");

	if( me->query_temp("train/busy") )
		return notify_fail("你刚刚试图驯服过野兽，感觉有些疲劳。\n");

	if( !arg || !(target=present(arg,environment(me))) )
		return notify_fail("你要驯服什么动物？\n");

	if( !living(target) )
		return notify_fail("你只能驯服活物。\n");

	if( target->query("race") != "野兽"
	 && target->query("race") != "飞禽"
	)
		return notify_fail("你只能驯服动物。\n");

	if( target->query("master") == me->query("id") )
		return notify_fail("你已经驯服它了。\n");

	if( target->query("master") && present(target->query("master"),environment(me)) )
		return notify_fail("它的主人就在旁边，你这么做不大合适吧？\n");

	message_vision(CYN"$N慢慢靠近$n，双眼紧盯$n，口中发出怪异的叫声......\n" NOR, me,target);

	lvl = me->query_skill("training",1);
	lvl = lvl * lvl * lvl /10;
	if( lvl < 50 ) lvl = 50;

	if( target->query("train_lvl") )
		exp = target->query("train_lvl") * target->query("train_lvl") * target->query("train_lvl") / 10;
	else
		exp = target->query("combat_exp");

	if( lvl > (exp/2) ) lvl = exp / 2;

	if( lvl > random(exp) ) {
		message_vision(HIW"$n驯服的靠近$N，轻轻舔了舔$N的手。\n" NOR, me,target);
		if( lvl < (exp/3) ) {
			lvl = me->query_skill("training",1);
			if( lvl > 300 ) lvl = 300 + (lvl-300)/2;
			if( lvl > 200 ) lvl = 200 + (lvl-200)/2;
			if( lvl > 100 ) lvl = 100 + (lvl-100)/2;
			me->improve_skill("training",random(lvl+me->query_int(1)));
		}
		target->set("master",me->query("id"));
	}
	else {
		if( random(3) != 1 )
			message_vision(HIW"$n根本不理会$N的动作和呼叫之声。\n" NOR, me,target);
		else {
			message_vision(RED"$n突然恶狠狠的瞪了$N一眼，冲向$N。\n" NOR, me,target);
			target->fight_ob(me);
		}
	}

	me->set_temp("train/busy",1);
	if( me->is_fighting() ) me->start_busy(1);
	call_out("delete_busy",6+random(13),me);

	return 1;
}

void delete_busy(object me)
{
	if( me ) me->delete_temp("train/busy");
}

int help(object me)
{
write(@HELP
指令格式 : train <动物id>

这个命令用来驯服动物。
HELP
);
	return 1;
}
