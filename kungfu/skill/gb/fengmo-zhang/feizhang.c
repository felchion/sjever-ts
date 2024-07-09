// feizhang.c  螺旋飞杖

#include <ansi.h>
#include <combat.h>
inherit F_SSERVER;

string perform_name(){ return CYN"螺旋飞杖"NOR; }

int perform(object me, object target)
{
	object weapon;
	int ap, dp, damage, p;
	string msg;

	if( !target ) target = offensive_target(me);

	if( !objectp(target) || !target->is_character() || !me->is_fighting(target) )
		return notify_fail("螺旋飞杖只能对战斗中的对手使用。\n");

	if( !objectp(weapon = me->query_temp("weapon"))
	 || (string)weapon->query("skill_type") != "staff")
		return notify_fail("你使用的武器不对。\n");

	if( me->query_skill("fengmo-zhang", 1) < 100 )
		return notify_fail("你的疯魔杖法不够娴熟，不会使用「螺旋飞杖」。\n");

	if( me->query_skill_mapped("parry") != "fengmo-zhang"
	 || me->query_skill_mapped("staff") != "fengmo-zhang")
		return notify_fail("你现在不能使用飞杖伤敌。\n");

	if( me->query_skill("huntian-qigong", 1) < 80 )
		return notify_fail("你的混天气功不够高，不能使用飞杖伤敌。\n");

	if( me->query("max_neili", 1) < 800 )
		return notify_fail("你的内力修为不够，不能使用「螺旋飞杖」。\n");

	if( me->query("neili",1) < 500 )
		return notify_fail("你的真气不够，无法使用飞杖。\n");

	msg = CYN"\n$N一声长啸，猛吸一口气,将手中"+weapon->query("name")+CYN"凌空掷向$n。\n"NOR;

	ap = COMBAT_D->skill_power(me, "staff", SKILL_USAGE_ATTACK);
	dp = COMBAT_D->skill_power(target, "parry", SKILL_USAGE_DEFENSE);
	ap += me->query("combat_exp", 1) / 1000 * me->query_str(1);
	dp += target->query("combat_exp", 1) /1000 * target->query_con(1);

	if ( random(ap + dp ) > dp ) {
		damage = me->query_skill("staff") * 4;
		damage += me->query_skill("force") * 2;
		damage = damage/2 + random(damage);
		if (damage > 3500) damage = 3500 + random(damage-3500)/10;
		target->receive_damage("qi", damage, me);
		target->receive_wound("qi", damage/3, me);
		me->add("neili", -250);
		me->add("jingli", -80);
		msg += HIR"只见$N手中"+ weapon->name()+HIR"螺旋飞出，倏的罩向$n。\n"+
			"$n只觉一股大力铺天盖地般压来，登时眼前一花，两耳轰鸣，哇的喷出一口鲜血！\n"NOR;
		if( random(2) ) {
			p = (target->query("eff_qi")+target->query_temp("apply/qi"))*100/(target->query("max_qi")+target->query_temp("apply/qi"));
			msg += "( $n"+COMBAT_D->eff_status_msg(p)+" )\n";
		}
		else {
			p = target->query("qi")*100/(target->query("max_qi")+target->query_temp("apply/qi"));
			msg += "( $n"+COMBAT_D->status_msg(p)+" )\n";
		}
		msg +=	HIW"\n只见"+weapon->name()+HIW"撞到$n后，反弹回来又回到了$N手中。\n"NOR;
	}
	else{
		msg += CYN"可是$p见事不妙，猛地向前一跃，跳出了$P的攻击范围，$P大步向前接住空中的"+weapon->query("name")+CYN"。\n"NOR;
		me->add("neili", -100);
	}
	me->start_busy(random(2));
	me->start_perform(3, "飞杖");
	message_vision(msg, me, target);
	return 1;
}

int help(object me)
{
	write(CYN"\n疯魔杖之「螺旋飞杖」："NOR"\n");
        write(@HELP
	要求：  最大内力  800 以上；
                当前内力  500 以上；
                疯魔杖等级 100 以上；
                混天气功等级 80 以上；
HELP
        );
        return 1;
}
