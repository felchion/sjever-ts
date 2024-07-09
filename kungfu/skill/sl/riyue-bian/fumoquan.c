#include <ansi.h>

inherit F_SSERVER;

string perform_name() {return BLU"金刚伏魔圈"NOR;}
int query_ap() {return 4;}

private int remove_effect(object me, int amount);
int perform(object me)
{
	object weapon = me->query_temp("weapon");
	int lvl;

	if(!me->is_fighting() )
	return notify_fail("「金刚伏魔圈」只能在战斗中使用。\n");

	if((int)me->query_skill("yijin-jing", 1) < 150 )
	return notify_fail("你的内功等级不够，不能使用「金刚伏魔圈」。\n");

	if ((int)me->query_skill("riyue-bian", 1) < 150 )
	return notify_fail("你的日月鞭法不够娴熟，不能真正发挥「金刚伏魔圈」。\n");

	if (!weapon
	|| weapon->query("skill_type") != "whip"
	|| me->query_skill_mapped("whip") != "riyue-bian")
	return notify_fail("你现在无法使用「金刚伏魔圈」来提高防御力。\n");

	if ((int)me->query("neili") < 2000 )
	return notify_fail("你现在内力太弱，不能使用「金刚伏魔圈」。\n");

	if ((int)me->query_temp("ryb_yue"))
	return notify_fail("你正在使用「日月神鞭」绝技。\n");

	if( me->query_temp("fumo"))
	return notify_fail("你正在使用大金刚拳的特殊攻击「金刚伏魔」！\n");

	if ( (int)me->query_temp("ryb_fumo") )
	return notify_fail("你正在使用「金刚伏魔圈」。\n");

	if( me->query_temp("fanzhen")   )
	return notify_fail("你正在运用「金刚不坏体神功」！\n");

	if (me->query_skill_mapped("force") != "yijin-jing")
	return notify_fail("你的内功有误，无法使用「金刚伏魔圈」。\n");

	message_vision(HIG "$N这时一声高喝，脸上殷红如血，僧袍都鼓了起来，手中"+weapon->name()+HIG"急速转动，渐渐收短，\n"+
	"守御相当严密。"+weapon->name()+HIG"似有无穷弹力，似真似幻，无论敌人怎么变招抢攻，总是被弹了出去。\n"NOR, me);
	
	me->add("neili", -1000);

	lvl = me->query_skill("riyue-bian", 1);
	me->add_temp("apply/damage",  300+lvl/2);
	me->add_temp("apply/pfm_parry", 120+lvl/10);
	me->add_temp("apply/pfm_dodge", 120+lvl/10);
	me->add_temp("apply/pfm_attack", 120+lvl/10);
	me->add_temp("apply/pfm_damage", 200+lvl/5);
	me->add_temp("apply/action/color", BLU);
	me->set_temp("ryb_fumo", 1);

	me->start_call_out( (: call_other, __FILE__, "check_fight", me, lvl, weapon :), 1);
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
	else call_out("check_fight", 1, me, amount, weapon);
}

private int remove_effect(object me, int lvl)
{
	me->add_temp("apply/damage",  -300-lvl/2);
	me->add_temp("apply/pfm_parry", -120-lvl/10);
	me->add_temp("apply/pfm_dodge", -120-lvl/10);
	me->add_temp("apply/pfm_attack", -120-lvl/10);
	me->add_temp("apply/pfm_damage", -200-lvl/5);
	me->delete_temp("apply/action");
	me->delete_temp("ryb_fumo");
	if(living(me) && !me->is_ghost())
	message_vision(HIG"$N所使金刚伏魔圈的威力渐渐平息，一切又恢复了原状。\n"NOR, me);
	return 0;
}

int help(object me)
{
	write(HIC"\n日月鞭「金刚伏魔圈」："NOR"\n");
	write(@HELP
	
	要求：	      
		当前内力 2000 以上； 
		易筋经等级 150 以上；
		日月鞭法等级 150 以上；
		禅宗心法等级 150 以上；
		激发日月鞭法为招架；
		激发易筋经为内功；
		装备长鞭类兵器；     
		并未设制打不还手。     
HELP
	);
	return 1;
}