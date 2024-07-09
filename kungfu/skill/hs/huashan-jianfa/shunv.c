#include <ansi.h>

inherit F_SSERVER;

string perform_name() {return HIM"淑女剑"NOR;}
int query_ap() {return 2;}

private int remove_effect(object me, int amount);
int perform(object me)
{
	object weapon = me->query_temp("weapon");

	if(!me->is_fighting() )
	return notify_fail("「淑女剑法」只能在战斗中使用。\n");

	if((int)me->query_skill("zixia-gong", 1) < 130 )
	return notify_fail("你的内功等级不够，不能使用「淑女剑法」。\n");

	if ((int)me->query_skill("huashan-jianfa", 1) < 130 )
	return notify_fail("你的华山剑法不够娴熟，不能真正发挥「淑女剑法」。\n");

	if (!weapon
	|| weapon->query("skill_type") != "sword"
	|| me->query_skill_mapped("parry") != "huashan-jianfa"
	|| me->query_skill_mapped("sword") != "huashan-jianfa")
	return notify_fail("你现在无法使用「淑女剑法」来提高命中率。\n");

	if ((int)me->query("neili") < 500 )
	return notify_fail("你现在内力太弱，不能使用「淑女剑法」。\n");

	if ((int)me->query_temp("hsj_junzi"))
	return notify_fail("你正在使用「君子剑法」绝技。\n");

	if ((int)me->query_temp("hsj_shunv"))
	return notify_fail("你正在使用「淑女剑法」绝技。\n");

	if ( (int)me->query_temp("hsj_xiyi") )
	return notify_fail("你正在使用「希夷剑法」。\n");


	if (me->query_skill_mapped("force") != "zixia-gong")
	return notify_fail("你的内功有误，无法使用「淑女剑法」。\n");

	message_vision(HIC "$N这时一声清喝，手中"+weapon->name()+HIC"招数一变,使出华山气宗嫡传的淑女剑法，\n"+
	"这宁女侠亲传神技好生了得。但见"+weapon->name()+HIC"幻起道道飞虹，不论对手腾挪闪跃，竭力招架，却始终脱不出$N的剑气笼罩，\n"NOR, me);
	me->start_perform(1,"「淑女剑法」");
	me->add("neili", -500);

	me->add_temp("apply/pfm_dodge",  180);
	me->set_temp("hsj_shunv", 1);

	call_out("check_fight", 1, me, weapon);
	return 1;
}

void check_fight(object me, object weapon)
{
	object wep;

	if (!me) return;
	wep = me->query_temp("weapon");
	if( !me->is_fighting()
	|| !living(me)
	|| me->is_ghost()
	|| !wep
	|| weapon != wep ){
		me->add_temp("apply/pfm_dodge", -180);
		me->delete_temp("hsj_shunv");
		if(living(me) && !me->is_ghost())
		message_vision(HIW"一套淑女剑法演毕，$N凝神收式,淑女剑法的剑气渐渐消失。\n"NOR, me);
		return;
	}
	call_out("check_fight", 1, me, weapon);
}

int help(object me)
{
   write(HIM"\n华山剑法之「淑女剑」："NOR"\n");
   write(@HELP

    要求：紫霞神功 120级
          华山剑法 120级，
          当前内力 500以上。
HELP
   );
   return 1;
}