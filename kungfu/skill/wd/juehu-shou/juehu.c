// juehu.c 三阴绝户手「绝户」perform

#include <ansi.h>
inherit F_SSERVER;

int perform(object me, object target)
{
	int damage, p ,ap, dp, i;
	string msg;

	if( !target ) target = offensive_target(me);

	if( !objectp(target) || !target->is_character() || !me->is_fighting(target) )
		return notify_fail("神功伤敌只能对战斗中的对手使用。\n");

	if( objectp(me->query_temp("weapon")) )
		return notify_fail("你必须空手才能使用「绝户」神功！\n");  
                
	if( me->query_skill("juehu-shou", 1) < 180 )
		return notify_fail("你的虎爪绝户手不够娴熟，不会使用「绝户」神功\n");

	if( me->query_skill("yinyun-ziqi", 1) < 180 )
		return notify_fail("你的氤氲紫气不够高，不能用「绝户」神功伤敌。\n");

	if( me->query("shen") < 0 )
		return notify_fail("你邪气太重，无法使出「绝户」招式。\n");
/*       
	if( me->query("juehu_nouse"))
		return notify_fail("你内息无法运至赤阳二穴，不能使出「绝户」招式。\n");
*/
	if ( me->query_skill_prepared("hand") != "juehu-shou"
	|| me->query_skill_mapped("parry") != "juehu-shou" )
		return notify_fail("你现在不能使出「绝户」招式。\n");

	if( me->query("neili") < 1500  || me->query("max_neili") < 2000)
		return notify_fail("你现在内力太弱，不能使出「绝户」招式。\n");

	if( target->query_condition("juehu_hurt"))
		return notify_fail( target->name(1)+"已经被你元气大伤，不用再对其使用「绝户」招式了。\n");

	msg = HIY"\n$N默运神功，脸色白得吓人，突然使出虎爪绝户手的最后一击「绝户」，企图当场把$n废于掌下。\n"NOR;

	ap = me->query("combat_exp", 1) /1000 * me->query_skill("force");
	dp = target->query("combat_exp", 1) / 1000 * target->query_skill("force");

	if ( random(ap + dp) > dp  && ! target->query("job_npc")) {
		damage = me->query_skill("juehu-shou", 1) * 5;
		damage += random(damage/2);
		i = me->query_skill("juehu-shou", 1)/2 + 50;

		if ( target->query("gender") == "男性") {
			msg += HIR"结果$N一击命中，$n只感到下体一阵刺痛，体内精元不断外泄，顿时觉的四肢无力，两腿不停颤抖。\n"NOR;
			damage *= 3;
		}
		else if ( target->query("gender") == "女性") {
			msg += HIR"$n被你一击命中，不禁既羞又怒，$N居然对一女流之辈也用此阴毒招数！\n"NOR;
			damage *= 2;
			i /= 3;
		}
		else {
			msg += HIR"$n虽被击中要害，但感觉受力甚轻，似乎已无要害可言！\n"NOR;
			damage -= random(damage/2);
			i /= 5;
		}
		target->receive_damage("qi", damage, me);
		target->receive_wound("qi", damage/2+random(damage/3), me);
		target->add_condition("juehu_hurt", i );
		me->add("shen", -damage);
		me->add("neili", -400);
		me->add("jingli", -200);
		if (random(2)) {
			p = (target->query("eff_qi")+target->query_temp("apply/qi"))*100/(target->query("max_qi")+target->query_temp("apply/qi"));
			msg += "( $n"+COMBAT_D->eff_status_msg(p)+" )\n";
		}
		else {
			p = target->query("qi")*100/(target->query("max_qi")+target->query_temp("apply/qi"));
			msg += "( $n"+COMBAT_D->status_msg(p)+" )\n";
		}
	}
	else {
		me->add("neili", -150);
		me->add("jingli", -50);
		msg += CYN"可是$p看破了$P的狠毒企图，向旁跳开数步，躲开了$P的凌厉一击！\n"NOR;
	}
	message_vision(msg, me, target);
	me->start_busy(random(2));
	me->start_perform(3, "绝户");
	return 1;
}

string perform_name(){ return HIY"绝户"NOR; }

int help(object me)
{
        write(HIY"\n虎爪绝户手之「绝户」："NOR"\n\n");
        write(@HELP
        要求：  当前内力 1500 以上；
                当前精力 2000 以上；
                虎爪绝户手等级 180 以上；
                氤氲紫气等级 180 以上；
                激发招架为虎爪绝户手；
                不可为负神且空手。

HELP
        );
        return 1;
}
                                                                                                                                           