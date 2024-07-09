// riyue 日月同辉
// by leontt
// Modify by leontt 1/05/2000
// By Spiderii fix bug

#include <ansi.h>
#include <combat.h>

string perform_name(){ return HIC"日月同辉"NOR; }
int query_ap() {return 3;}

inherit F_SSERVER;
int perform(object me, object target)
{
	int lv;
	if( !target ) target = offensive_target(me);

	if( !objectp(target) || !me->is_fighting(target) )
	return notify_fail("「日月同辉」只能在战斗中使用。\n");

	if( objectp(me->query_temp("weapon")) )
	return notify_fail("使用「日月同辉」时必须空手！\n");

	if( (int)me->query_skill("xingyi-zhang", 1) < 140 )
	return notify_fail("你的星移掌不够娴熟，使不出「日月同辉」绝技。\n");

	if( (int)me->query_skill("shenyuan-gong", 1) < 140 )
	return notify_fail("你的神元功等级不够，不能使用「日月同辉」。\n");

	if (me->query_skill_mapped("force") != "shenyuan-gong")
	return notify_fail("你目前的内功无法支持你使用「日月同辉」进行攻击。\n");

	if (me->query_skill_prepared("strike") != "xingyi-zhang"
	|| me->query_skill_mapped("strike") != "xingyi-zhang")
	return notify_fail("你现在无法使用「日月同辉」进行攻击。\n");

	if( (int)me->query("max_neili") < 1500 )
	return notify_fail("你现在内力修为太弱，不能使用「日月同辉」。\n");

	if( (int)me->query("neili") < 1200 )
	return notify_fail("你目前内力太少，不能使用「日月同辉」。\n");

	if( (int)me->query("jingli") < 200 )
	return notify_fail("你现在精力太少，不能使用「日月同辉」。\n");

	message_vision(CYN"\n$N双掌一搓，在双掌之间形成一个耀眼的「"HIW"气旋"CYN"」！\n" NOR,me, target);

	lv = (int)me->query_skill("xingyi-zhang", 1);

	me->add_temp("apply/pfm_attack", 150+lv/10);
	me->add_temp("apply/force", 250+lv/2);
	me->add_temp("apply/pfm_force", 150+lv/5);
	me->set_temp("mr_riyue", 4);

	me->set_temp("apply/action/pfm", HIR"$N双掌收在胸前，移形换步闪电般将「"HIW"气旋"HIR"」击向$n的檀中穴");
	COMBAT_D->do_attack(me, target,);
	me->add("neili", -300);
	if(me->is_fighting(target)){
		me->set_temp("apply/action/pfm",HIG"紧接着$N抽出左掌，身形一矮，由下而上猛袭向$n的小腹");
		COMBAT_D->do_attack(me, target, 0, random(4)?0:3);
		me->add("neili", -300);
	}
	if(me->is_fighting(target) && me->query_skill("xingyi-zhang", 1) >= 150){
		me->set_temp("apply/action/pfm",HIR"只见$N右掌随后而至，凌空一跃，犹如晴空霹雳拍向$n的头部");
		COMBAT_D->do_attack(me, target, 0, random(3)?0:3);
		me->add("neili", -300);
	}
	if(me->is_fighting(target) && me->query_skill("xingyi-zhang", 1) >= 200){
		me->set_temp("apply/action/pfm", HIY"$N在空中一个回转，携「"HIR"日月之辉"HIY"」, 双掌按向$n的丹田");
		COMBAT_D->do_attack(me, target, 0, random(2)?0:3);
		me->add("neili", -300);
	}

	message_vision(YEL "\n$N一式「日月同辉」发挥的淋漓尽致，双掌一分，气定神闲，信心倍增。\n" NOR,me, target);
	me->add_temp("apply/pfm_attack", -150-lv/10);
	me->add_temp("apply/force", -250-lv/2);
	me->add_temp("apply/pfm_force", -150-lv/5);
	me->delete_temp("apply/action");
	me->delete_temp("mr_riyue");
	me->start_busy(random(2));
	return 1;
}


int help(object me)
{
        write(HIC"\n「日月同辉」："NOR"\n\n");
        write(@HELP
        要求：  当前内力 1200 以上；
                最大内力 1500 以上；
                星移掌等级 140 以上；
                神元功等级 140 以上；
                激发掌法为星移掌；
                备掌法为星移掌；
                且手无兵器。

HELP
        );
        return 1;
}