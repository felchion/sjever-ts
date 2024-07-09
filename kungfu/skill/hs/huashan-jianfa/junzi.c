#include <ansi.h>

inherit F_SSERVER;

string perform_name() {return HIW"君子剑"NOR;}
int query_ap() {return 2;}

private int remove_effect(object me, int amount);
int perform(object me)
{
	object weapon = me->query_temp("weapon");

	if(!me->is_fighting() )
	return notify_fail("「君子剑法」只能在战斗中使用。\n");

	if((int)me->query_skill("zixia-gong", 1) < 140 )
	return notify_fail("你的内功等级不够，不能使用「君子剑法」。\n");

	if ((int)me->query_skill("huashan-jianfa", 1) < 140 )
	return notify_fail("你的华山剑法不够娴熟，不能真正发挥「君子剑法」。\n");

	if (!weapon
	|| weapon->query("skill_type") != "sword"
	|| me->query_skill_mapped("parry") != "huashan-jianfa"
	|| me->query_skill_mapped("sword") != "huashan-jianfa")
	return notify_fail("你现在无法使用「君子剑法」来提高伤害力。\n");

	if ((int)me->query("neili") < 500 )
	return notify_fail("你现在内力太弱，不能使用「君子剑法」。\n");

	if ((int)me->query("jingli") < 500 )
	return notify_fail("你现在精力太少，不能使用「君子剑法」。\n");

	if ((int)me->query_temp("hsj_junzi"))
	return notify_fail("你正在使用「君子剑法」绝技。\n");

	if ((int)me->query_temp("hsj_shunv"))
	return notify_fail("你正在使用「淑女剑法」绝技。\n");

	if ( (int)me->query_temp("hsj_xiyi") )
	return notify_fail("你正在使用「希夷剑法」。\n");


	if (me->query_skill_mapped("force") != "zixia-gong")
	return notify_fail("你的内功有误，无法使用「君子剑法」。\n");

	message_vision(HIC "$N这时一声清喝，手中"+weapon->name()+HIC"招数一变,使出华山气宗嫡传的君子剑法，\n"+
	"立时将气宗以气驭剑的要旨发挥的淋漓尽致。紫霞神功内力催动之下"+weapon->name()+HIC"暴出道道凌厉剑气，威力登时大增。\n"NOR, me);
	me->add("neili", -500);


	me->add_temp("apply/pfm_attack", 180 );
	me->set_temp("hsj_junzi", 180);

	me->start_call_out( (: call_other, __FILE__, "check_fight", me, 30, weapon :), 1);
	return 1;
}

void check_fight(object me, int amount, object weapon)
{
	object wep;

	if (!me) return;
	wep = me->query_temp("weapon");
	if(!me->is_fighting()
	|| !living(me)
	|| me->is_ghost()
	|| !wep
	|| weapon != wep )
	remove_effect(me, amount);
	else call_out("check_fight", 1, me, amount-1, weapon);
}

private void remove_effect(object me, int amount)
{
	int skill;
	if(!me) return;
	skill = me->query_temp("hsj_junzi");
	if(!skill) return;

	me->add_temp("apply/pfm_attack",  -skill);
	me->delete_temp("hsj_junzi");
	if(living(me) && !me->is_ghost())
	message_vision(HIW"一套君子剑法演毕，$N凝神收式,君子剑法的剑气也渐渐消失。\n"NOR, me);
	return;
}
