// mie.c 灭剑
// rewrite by olives@SJ
// 5/12/2001
// Powered by snowman@SJ 16/06/2001
// fixed bug by YUJ@SJ 2001-07/02

#include <ansi.h>
inherit F_SSERVER;

int query_ap() {return 3;}

int perform(object me, object target)
{
	object weapon;
	object *pfmer = ({});
	int sword;

	weapon = me->query_temp("weapon");

	if (!target) target = offensive_target(me);

	if (!objectp(target)
	|| !target->is_character()
	|| !me->is_fighting(target))
		return notify_fail("灭剑只能对战斗中的对手使用。\n");

	if( !objectp(weapon) || weapon->query("skill_type") != "sword" )
		return notify_fail("你手中无剑，怎能运用「灭剑」心法？！\n");

	if( me->query_skill("huifeng-jian",1) < 120 )
		return notify_fail("你回风拂柳剑修为还不够，还未领悟「灭剑」！\n");

	if( me->query_skill_mapped("sword") != "huifeng-jian")
		return notify_fail("你现在不能使用「灭剑」心法。\n");

	if ( me->query_skill_mapped("parry") != "huifeng-jian")
		return notify_fail("你现在不能使用「绝剑」心法。\n");

	if( me->query_skill("linji-zhuang", 1) < 120 )
		return notify_fail("你的内功修为火候未到，施展「灭剑」只会伤及自身！\n");

	if( me->query_skill_mapped("force") != "linji-zhuang")
		return notify_fail("你所用的内功与「灭剑」心法相悖！\n");

	if( me->query("max_neili") < 1500 )
		return notify_fail("你的内力修为不足，劲力不足以施展「灭剑」！\n");

	if( me->query("neili") < 700 )
		return notify_fail("你的内力不够，劲力不足以施展「灭剑」！\n");

	if( me->query("jingli") < 500 )
		return notify_fail("你的精力有限，不足以施展「灭剑」！\n");

	if (me->query_temp("huifeng/mie"))
		return notify_fail("你正在使用「灭剑」心法！\n");

	if ( userp(me) ) {
		me->add("neili", -300);
		me->add("jingli", -100);
	}

	sword = me->query_skill("huifeng-jian",1); 
	
	message_vision(HIW"$N飘身而起，使出峨嵋绝技「灭剑」心法，顿时剑意空灵飘逸而，似万千柳絮飞舞，又如和风阵阵拂过！\n"NOR,me);
	if(arrayp(pfmer = target->query_temp("pfm-target/huifeng-mie")))
		pfmer -= ({0});
	else pfmer = ({});
	//这里，如果开始没有人对这个人用pfm mie，那么则加上
	if(wizardp(me))
	{
		tell_object(me,sprintf("pfmer = %O\n",pfmer));
		tell_object(me,sprintf("victim temp dbase  = %O\n",target->query_entire_temp_dbase()));
	}
	if(!sizeof(pfmer))
	{
			if(!target->query_temp("huifeng-mie"))
			//这次上线曾经被灭劍打过没有？
			//如果有，那么效果将不是很大，如果没有，那么将很大
			{
				me->add_temp("apply/pfm_damage", 140);
				me->add_temp("apply/pfm_attack", 180);
				me->set_temp("huifeng/mie", 1);
				message_vision(HIW"$N眼花缭乱，被攻了个措手不及！\n" NOR,target);
				target->add_temp("apply/pfm_dodge", -200);
				target->add_temp("apply/pfm_parry", -200);
				target->add_temp("apply/pfm_attack", -200);
				target->set_temp("huifeng-mie",1);
				me->add("neili", -100);
				call_out("remove_effect", sword/10, me, target, 1);
			}
			else
			{
				me->add_temp("apply/pfm_damage", 70);
				me->add_temp("apply/pfm_attack", 90);
				me->set_temp("huifeng/mie", 1);
				message_vision(HIW"$N虽已加倍提防，但仍感攻势频频收挫。\n" NOR,target);
				target->add_temp("apply/pfm_dodge", -100);
				target->add_temp("apply/pfm_parry", -100);
				target->add_temp("apply/pfm_attack", -100);
				me->add("neili", - 100);
				call_out("remove_effect", sword/5, me, target,2);
			}
			pfmer += ({me});//在列表里面加上用pfm的人！
			target->set_temp("pfm-target/huifeng-mie",pfmer);
			COMBAT_D->do_attack(me, target, me->query_temp("weapon"), 0);
			if (target && me->is_fighting(target))
				COMBAT_D->do_attack(me, target, me->query_temp("weapon"), 3);	
			me->start_busy(random(2));
			return 1;
	}
	
	if(member_array(me,pfmer)==-1)//如果我们不是曾经对这个人用过这个灭劍的pfmer之一，那么会给予这个人busy的效果！
	{
		message_vision("$N被打了个措手不及，连连后退！\n",target);
		me->add_temp("apply/pfm_damage", 150);
		me->add_temp("apply/pfm_attack", 180);
		me->add_temp("apply/damage", 500);
		me->set_temp("huifeng/mie", 1);
		pfmer += ({me});//在列表里面加上用pfm的人！
		target->set_temp("pfm-target/huifeng-mie", pfmer);
		COMBAT_D->do_attack(me, target, me->query_temp("weapon"), 1);
		if (me->is_fighting(target))
			COMBAT_D->do_attack(me, target, me->query_temp("weapon"), 1);
		call_out("finish_mie2", sword/20, me, target, sword);
		me->start_busy(random(2)+1);
		return 1;
	}
	return notify_fail("你正在运用「灭剑」心法！\n");//我们正在对这个人使用灭劍！效果将不会叠加！
}

void remove_effect(object me, object victim, int i)
{
	object *pfmer;

	if (victim && objectp(victim))
	{
		victim->add_temp("apply/pfm_dodge", 200/i);
		victim->add_temp("apply/pfm_parry", 200/i);
		victim->add_temp("apply/pfm_attack", 200/i);
		pfmer = victim->query_temp("pfm-target/huifeng-mie");
	}//victim的降低攻防的效果消失
	//但是huifeng-mie的标志不消失，代表已经对这个人使用过一次pfm mie了

	if (me && objectp(me) && me->query_temp("huifeng/mie"))
	{
		me->add_temp("apply/pfm_damage", -140/i);
		me->add_temp("apply/pfm_attack", -180/i);
		me->delete_temp("huifeng/mie");
	}

	if(me && arrayp(pfmer)
	&& objectp(me))
	{
		pfmer -= ({me});
		pfmer -= ({0});
		victim->set_temp("pfm-target/huifeng-mie", pfmer);
	}
	if (objectp(me) && objectp(victim) && victim->is_fighting(me))
		message_vision(HIY "\n$N渐渐适应了$n的剑路，攻守自如多了。\n" NOR, victim, me);
	else if(objectp(me))
		message_vision(HIY "\n$N运功完毕，四周飞絮般的剑气似乎也慢慢消失了。\n" NOR, me);
}

void finish_mie2(object me, object victim, int sword)
{
	object *pfmer;

	if (victim && objectp(victim))
		pfmer = victim->query_temp("pfm-target/huifeng-mie");
	if (me && objectp(me) && me->query_temp("huifeng/mie"))
	{
		me->add_temp("apply/pfm_damage", -150);
		me->add_temp("apply/pfm_attack", -180);
		me->add_temp("apply/damage", -500);
		me->delete_temp("huifeng/mie");
	}
	if(me && arrayp(pfmer)
	&& objectp(me))
	{
		pfmer -= ({me});
		pfmer -= ({0});
		victim->set_temp("pfm-target/huifeng-mie", pfmer);
	}

	if (me && objectp(me) && objectp(victim) && victim->is_fighting(me))
		message_vision(HIY "\n\n$N渐渐适应了$n的剑路，攻守自如多了。\n" NOR, victim, me);
	else if(objectp(me))
		message_vision(HIY "\n\n$N运功完毕，四周飞絮般的剑气似乎也慢慢消失了。\n" NOR, me);
}

string perform_name(){ return HIW"灭剑心法"NOR; }

int help(object me)
{
        write(HIW"\n回风拂柳剑之「灭剑心法」："NOR"\n\n");
        write(@HELP
        要求：  当前内力 700 以上；
                最大内力 1500 以上；
                当前精力 500 以上；
                回风拂柳剑等级 120 以上；
                临济十二庄等级 120 以上；
                激发内功为临济十二庄；
                激发剑法为回风拂柳剑；
                激发招架为回风拂柳剑；
                手持兵器。

HELP
        );
        return 1;
}
