// puhuo
// 2000.1.19  by emnil

#include <ansi.h>

inherit F_SSERVER;

string perform_name() {return RED"飞蛾扑火"NOR;}

void canfight(object me);

int perform(object me, object target)
{
	object weapon;
	int busy=0;
	int f,nl,skill;
	string msg;

	if( !target ) target = offensive_target(me);

	weapon = me->query_temp("weapon");

	if (weapon)
		return notify_fail("只有空手才能使用「飞蛾扑火」！\n");

	if( !target 
	  || !target->is_character()
	  || !me->is_fighting(target)
	|| !me->is_killing(target->query("id")))
		return notify_fail("「飞蛾扑火」只能对以性命相搏的对手使用。\n");

	if(me->query_temp("sxbb2",1) || me->query_temp("sxbb1",1))
		return notify_fail("「飞蛾扑火」不能和「神行百变」合用。\n");

	if(me->query("neili")<1000)
		return notify_fail("你现在内力不够，没法使用「飞蛾扑火」。\n");
	if(me->query("jingli")<1000)
		return notify_fail("你现在精力不够，没法使用「飞蛾扑火」。\n");

	if (target->is_busy()) busy=1;

	if((int)me->query_skill("huagu-mianzhang", 1) < 100 )
		return notify_fail("你的化骨绵掌不够娴熟，不会使用「飞蛾扑火」。\n");

	if(me->query_skill_mapped("strike") != "huagu-mianzhang")
		return notify_fail("你现在使用的掌法不是化骨绵掌，不能使用「飞蛾扑火」。\n");

	f = random(me->query("combat_exp"))*100/(random(target->query("combat_exp"))+1);
	f+= random(me->query_skill("dodge"))*100/(random(target->query_skill("dodge"))+1);
	f+= random(me->query_dex())*100/(random(target->query_dex())+1);
	f+= random(me->query_kar())*100/(random(target->query_kar())+1);
	f+= random(me->query_skill("strike"))*100/(random(target->query_skill("parry"))+1);
	
	skill = me->query_skill("huagu-mianzhang", 1)/20;
	msg = RED+"\n$N突然双目赤红，两臂张开，不顾死活地向$n扑去！\n"NOR;
	me->receive_damage("jingli",200);
	me->receive_damage("neili",400);
	me->start_perform(5,"「飞蛾扑火」");
	if (f*(busy+1) > 250){
		msg +=RED+"$n一个没留神，被$N抱得结结实实，动弹不得！\n"NOR;

		if (f*(busy+1) < 500 ) {
			msg +=RED+"$n心中一急，向$N后心重重地猛击而下，打得$N脏腑移位，七窍流血！\n"NOR;
			message_vision(msg,me,target);
			target->add_busy(skill/2);
			nl=f*(busy+1)*3;
			if (target->add("neili",-nl) < 0)
				target->set("neili", 0);
			me->set_temp("last_damage_from","飞蛾扑火自尽而");
			me->die();
		} else {
			target->add_busy(skill);
			if (f*(busy+1) < 800) {
				msg +=RED+"$n心中一急，向$N后心重重地猛击而下，打得$N鲜血狂喷！\n"NOR;
				message_vision(msg,me,target);
				me->receive_damage("qi",(800-f*(busy+1))*20,target);
				me->receive_wound("qi",(800-f*(busy+1))*10, target);
				nl= f*(busy+1)*2;
				if (target->add("neili",-nl) < 0)
					target->set("neili", 0);
				me->start_busy(skill*2);
				me->set_temp("no_fight", 1);
				call_out("canfight", skill*2, me);
			}
		}
	} else {
		msg += CYN+"可是$n"+CYN+"看破了$N"+CYN+"的企图，逃了过去。\n"NOR;
		me->set_temp("last_damage_from","飞蛾扑火自尽而");
		message_vision(msg,me,target);
		me->die();
	}
	return 1;
}

void canfight(object me)
{
	if(me)
		me->delete_temp("no_fight");
}
