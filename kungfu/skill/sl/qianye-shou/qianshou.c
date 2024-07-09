// qianshou 千手如来
// bren

#include <ansi.h>
#include <combat.h>
inherit F_SSERVER;

int query_ap() {return 6;}
string perform_name(){ return HIY"千手如来"NOR; }

void remove_effect(object me,object target, int skill);
void check_fight(object me,object target,int improve);
int perform(object me, object target)
{
	string msg;
	int skill;
	skill = (int)me->query_skill("qianye-shou", 1);

	if( !target ) target = offensive_target(me);

	if( !target || !me->is_fighting(target) )
	return notify_fail("「千手如来」只能在战斗中对对手使用。\n");

	if( objectp(me->query_temp("weapon")) )
	return notify_fail("使用「千手如来」时双手必须空着！\n");

	if( (int)me->query_skill("qianye-shou", 1) < 200 )
	return notify_fail("你的如来千叶手不够娴熟，不会使用「千手如来」。\n");

	if( (int)me->query_skill("yijin-jing", 1) < 200 )
	return notify_fail("你的易筋经内功等级不够，不能使用「千手如来」。\n");

	if( (int)me->query_dex() < 27 )
	return notify_fail("你的身法不够强，不能使用「千手如来」。\n");

	if (me->query_skill_prepared("hand") != "qianye-shou"
	|| me->query_skill_mapped("hand") != "qianye-shou")
	return notify_fail("你现在无法使用「千手如来」进行攻击。\n");

	if( (int)me->query("neili") < 2000 )
	return notify_fail("你现在内力太弱，不能使用「千手如来」。\n");

	if( target->query_temp("qianshou")   )
	return notify_fail(target->name() + "目前正自顾不暇，放胆攻击吧！\n");

	if( me->query_temp("sl/qianshou")   )
	return notify_fail("你正在使用「千手如来」！\n");

	msg = HIY "$N双手合十互击，陡然向外一分，双掌顿时变得轻灵飘逸，灵动异常，霎时幻起漫天的手影。\n" NOR;
	message_vision(msg, me, target);
	if(COMBAT_D->do_attack(me, target, 0, 0, 1) 
	|| COMBAT_D->do_attack(me, target, 0, 0, 1)
	|| COMBAT_D->do_attack(me, target, 0, 0, 1)){
		msg = HIR "$n只觉得四周都是$N的身影，不由得慌乱起来。\n" NOR;
		message_vision(msg, me, target);
		target->add_temp("apply/pfm_attack", -200-skill/3);
		target->add_temp("apply/pfm_dodge", -200-skill/3);
		target->add_temp("apply/pfm_parry", -200-skill/3);
		me->add_temp("apply/pfm_damage",150+skill/5);
		me->add_temp("apply/damage",250+skill/2);
		me->set_temp("sl/qianshou",1);
		target->set_temp("qianshou",1);

		me->start_call_out( (: call_other, this_object(), "check_fight",  me, target, skill :), 1);
		return 1;
	}
	message_vision(HIC"$N慌乱中镇定心神，仔细看清了$n的身影。\n"NOR,target,me);
	me->start_busy(random(2));

	return 1;
}

void check_fight(object me, object target, int improve)
{
	object weapon;

	if (!target && !me) return;
	if( !me || !target) {
		remove_effect(me,target,improve);
	}
	else
	if(target->query_temp("qianshou") && !me->is_fighting(target)){
		remove_effect(me,target, improve);
	}
	else if( (target->query_temp("qianshou"))
	&& (!target->is_fighting()
	|| !me->is_fighting()
	|| !living(me)
	|| target->is_ghost()
	|| me->is_ghost() )){
		remove_effect(me,target, improve);
	}
	else if((target->query_temp("qianshou"))
	&& objectp(weapon = me->query_temp("weapon"))){
		remove_effect(me,target, improve);
	}
	else{
		call_out("check_fight", 1, me, target, improve);
		return;
	}
}

void remove_effect(object me,object target, int skill)
{
	if (target) {
		target->add_temp("apply/attack",200+skill/3);
		target->add_temp("apply/dodge", 200+skill/3);
		target->add_temp("apply/parry", 200+skill/3);
		target->delete_temp("qianshou");
		tell_object(target, HIB"你渐渐适应了这种变化，状态又恢复如常。\n"NOR);
	}
	if(me) {
		me->add_temp("apply/pfm_damage",-150-skill/5);
		me->add_temp("apply/damage",-250-skill/2);
		me->delete_temp("sl/qianshou",1);
		tell_object(me, HIB"你的气血翻腾，手上的攻势慢了来下。\n"NOR);
	}
}

int help(object me)
{
        write(HIW"\n「千手如来」："NOR"\n");
        write(@HELP
        
        要求：  
                当前内力 2000 以上；
                如来千叶手等级 120 以上；
                易筋经等级 120 以上；
                激发如来千叶手为手法；
                
HELP
        );
        return 1;
}