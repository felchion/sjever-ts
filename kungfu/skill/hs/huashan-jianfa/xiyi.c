#include <ansi.h>

inherit F_SSERVER;

string perform_name() {return HIY"希夷剑"NOR;}
int query_ap() {return 2;}

private int remove_effect(object me, int amount);
int perform(object me)
{
	object weapon = me->query_temp("weapon");

	if(!me->is_fighting() )
	return notify_fail("「希夷剑法」只能在战斗中使用。\n");

	if((int)me->query_skill("zixia-gong", 1) < 120 )
	return notify_fail("你的内功等级不够，不能使用「希夷剑法」。\n");

	if ((int)me->query_skill("huashan-jianfa", 1) < 120 )
	return notify_fail("你的华山剑法不够娴熟，不能真正发挥「希夷剑法」。\n");

	if (!weapon
	|| weapon->query("skill_type") != "sword"
	|| me->query_skill_mapped("sword") != "huashan-jianfa"
	|| me->query_skill_mapped("parry") != "huashan-jianfa")
	return notify_fail("你现在无法使用「希夷剑法」来提高防御力。\n");

	if ((int)me->query("neili") < 500 )
	return notify_fail("你现在内力太弱，不能使用「希夷剑法」。\n");

	if ((int)me->query_temp("hsj_junzi"))
	return notify_fail("你正在使用「君子剑法」绝技。\n");

	if ((int)me->query_temp("hsj_shunv"))
	return notify_fail("你正在使用「淑女剑法」绝技。\n");

	if ( (int)me->query_temp("hsj_xiyi") )
	return notify_fail("你正在使用「希夷剑法」。\n");


	if (me->query_skill_mapped("force") != "zixia-gong")
	return notify_fail("你的内功有误，无法使用「希夷剑法」。\n");

	message_vision(HIC "$N这时一声清喝，手中"+weapon->name()+HIC"招数一变,使出华山气宗嫡传的希夷剑法，\n"+
	"守御立时严密无缝。"+weapon->name()+"暴出道道凌厉剑气，层层叠叠护在身前，任敌人全力变招抢攻，却根本无隙可入。\n"NOR, me);

	me->add("neili", -500);

	me->add_temp("apply/pfm_parry",  180);

	me->set_temp("hsj_xiyi", 1);

	me->start_call_out( (: call_other, __FILE__, "check_fight", me, 40, weapon :), 1);
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

private int remove_effect(object me, int amount)
{

	if(!me->query_temp("hsj_xiyi"))	return 0;
	me->add_temp("apply/pfm_parry", -180);
	me->delete_temp("hsj_xiyi");
	if(living(me) && !me->is_ghost())
	message_vision(HIW"一套希夷剑法演毕，$N凝神收式,希夷剑法的剑气渐渐消失。\n"NOR, me);
	return 0;
}

int help(object me)
{
   write(HIY"\n华山剑法之「希夷剑」："NOR"\n");
   write(@HELP

    要求：紫霞神功 120级
          华山剑法 120级，
          当前内力 500以上。
HELP
   );
   return 1;
}