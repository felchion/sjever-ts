// ci.c 飞针刺目

#include <ansi.h>

inherit F_SSERVER;

int query_ap() {return 4;}

void back(object target);

int perform(object me, object target)
{
	int skill;
	object weapon = me->query_temp("weapon");

	if( !target ) target = offensive_target(me);

	if( !target
	||  !target->is_character()
	||  !me->is_fighting(target) )
	return notify_fail("你只能对战斗中的对手使用。\n");

	if( (int)me->query_skill("pixie-jian", 1) < 200 )
	return notify_fail("你的辟邪剑法不够娴熟，不会使用飞针刺目。\n");

	if( (int)me->query("neili", 1) < 2000 )
	return notify_fail("你现在内力不足！\n");
	if( (int)me->query("max_jingli", 1) < 1000 )
	return notify_fail("你现在精力不足！\n");
	if( (int)me->query_dex() < 30  )
	return notify_fail("你的身法太低，不能使用飞针刺目。\n");

	if (!weapon || !(weapon->id("needle"))
	|| me->query_skill_mapped("sword") != "pixie-jian")
	return notify_fail("你现在无法使用飞针刺目。\n");

	if( (int)target->query_temp("cimu") )
	return notify_fail("你已经刺中他了！\n");

	skill = (int)me->query_skill("pixie-jian", 1);
	skill = skill/15;

	me->add("neili", -350);
	me->add("jingli", -200);

	message_vision(RED"$N一声阴笑，忽若一团红云般欺近$n，手中细针点向$n双目！\n"NOR, me, target);
	message_vision(RED"$n一惊，忙双臂回转，想护住双眼！\n"NOR, me, target);;

	if(COMBAT_D->do_attack(me, target, weapon, 0, 1)
	||COMBAT_D->do_attack(me, target, weapon, 0, 1)
	||COMBAT_D->do_attack(me, target, weapon, 0, 1)) {
		if (target->query_skill_mapped("sword") != "dugu-jiujian"
		|| target->query_skill("dugu-jiujian", 1) < 99) {
			message_vision(HIR"\n只听$n惨叫一声，只见两丝细血从$n眼中流出，看来已经被$N刺中双目了！\n"NOR, me, target);
			tell_object(target, HIY "啊！你虽然闭上了双眼，可细针还是刺入了你的眼皮，你的视线已经模糊不清了！\n" NOR);
			target->set_temp("cimu", 1);
			target->add_temp("apply/pfm_attack", - 300);
			target->add_temp("apply/pfm_dodge", - 180);
			target->add_temp("apply/pfm_parry", - 180);
			target->receive_damage("qi", 1000,me);
			target->receive_wound("qi", 500,me);
			target->kill_ob(me);
			target->set_temp("block_msg/all",1);
			call_out("back", 3 + skill, target);
		}
		else { message_vision(HIR"\n$n集中生智，左手化指为剑也向$N的左目急刺，竟是两败俱伤的打法。\n"NOR, me, target);
			tell_object(target, HIR "你但觉左边眉心微微一痛，知道自己左眉已为他绣花针所刺中，但所幸避开了眼睛要害。\n" NOR);
			target->receive_damage("qi", 100*skill,me);
			target->receive_wound("qi", 40*skill,me);
			target->kill_ob(me);
			me->start_busy(1+random(2));
		}
	}
	else {
		message_vision(HIY"$n抱头缩颈一个滚翻，总算躲开了$N这一招。\n"NOR, me, target);
		me->start_busy(3);
		target->kill_ob(me);
	}

	return 1;
}

void back(object target)
{
	if(!target) return;
	target->add_temp("apply/pfm_attack", 300);
	target->add_temp("apply/pfm_dodge", 180);
	target->add_temp("apply/pfm_parry",  180);
	target->set_temp("block_msg/all", 0);
	if(living(target))
	tell_object(target, HIW "啊！总算双目没有被真正刺伤，你又可以看见事物啦！\n" NOR);
	target->delete_temp("cimu");
}

string perform_name(){ return HIW"飞针刺目"NOR; }

int help(object me)
{
	write(HIW"\n辟邪剑法之「飞针刺目」："NOR"\n\n");
	write(@HELP
	
	要求：
		当前精力 1000 以上；
		当前内力 2000 以上；
		辟邪剑法等级 200 以上；
HELP
	);
	return 1;
}