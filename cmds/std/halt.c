// halt.c
// LinuX@SJ 2004/03/02
#include <ansi.h>
inherit F_SSERVER;

int main(object me, string arg)
{
	object ob,target;
	mixed exits;
	int i,j;
	string myway;

	if( arg && wizardp(me) ) {
		ob = present(arg, environment(me));
		if (!ob) ob = LOGIN_D->find_body(arg);
		if (!ob) ob = find_living(arg);
		if (!ob) return notify_fail("没有这个生物。\n");
		if (ob->is_busy()) {
			message_vision("\n$N用巫师的神力解除了$n的忙乱。\n", me, ob);
			ob->interrupt_me();
			ob->start_busy(-1);
			ob->interrupt_me();
			return 1;
		}
		if( !ob->is_fighting() ) return notify_fail(ob->name()+"现在不忙。\n");
		ob->remove_all_enemy();
		message_vision("\n$N用巫师的神力停止了$n的战斗。\n", me, ob);
		return 1;
	}
	if (!me->is_busy()) {
		if (me->is_fighting()) {
			if (me->is_nohalt())
			return notify_fail("你正在使用"+me->query_perform()+"，暂时无法停止战斗。\n");
			if (strsrch(file_name(environment(me)), "/cmds/leitai/bwdh") >= 0 )
			return notify_fail("在试剑山庄内，无法停止战斗了。\n");
			target = offensive_target(me);
			if (!userp(target)) {
				me->remove_all_enemy();
				target->add_temp("perform_AP",20);
				message_vision("\n$N身形向后一跃，跳出战圈不打了。\n", me);
				return 1;
			}
			if (me->query_temp("perform_AP") < 80 && userp(target))
			return notify_fail("你现在不能跳出战圈。\n");
			if (!mapp(environment(me)->query("exits"))) 
			return notify_fail("你想跳出战圈，但却发现无路可逃。\n");
			exits = keys(environment(me)->query("exits"));
			j = sizeof(exits)-sizeof(me->query_enemy());
			if (j<1) j=1;
			for(i=0;i<j;i++) {
				target = offensive_target(me);

				if(!COMBAT_D->do_attack(target,me, target->query_temp("weapon"),0, 1)) {
					me->remove_all_enemy();
					myway = exits[random(sizeof(exits))];
					message_vision("\n$N身形向后一跃，跳出战圈不打了。\n", me);
					me->force_me("go "+myway);
					me->add_temp("perform_AP",-80);
					return 1;
				}
			}
			me->add_temp("perform_AP",-80);
			message_vision("\n$N想退出战圈，却被"+target->name()+"拦着脱不开身。\n", me);
			return 1;
		}
		if (me->query_temp("on_baitan"))
		{
			message_vision(WHT "$N" WHT "提起摊布的四个角，把货物一股脑的收了"
			"起来，站起身来。\n" NOR, me);
			me->delete_temp("on_baitan");
			me->delete_temp("vendor_goods");
			me->delete("disable_type");
			return 1;
		}
		return notify_fail("你现在不忙。\n");
	}

	notify_fail("你现在很忙，停不下来。\n");
	me->interrupt_me();
	if (me->is_busy()) return 0;
	return 1;
}
