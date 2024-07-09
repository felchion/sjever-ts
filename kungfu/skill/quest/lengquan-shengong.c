
#include <ansi.h>
inherit FORCE;
#include "/kungfu/skill/force.h"
#include <combat_msg.h>

int valid_enable(string usage) {
   if ( this_player()->query("quest/雪山飞狐/武功/lengquanshengong_teach"))
             return usage == "force"; }
int valid_learn(object me)
{
        if ((int)me->query_skill("force", 1) < 20)
                return notify_fail("你的基本内功火候还不够。\n");
        return 1;
}

void skill_improved(object me)
{
        if (me->query_skill("lengquan-shengong", 1)>=220 && !me->query("quest/雪山飞狐/武功/lengquan"))
        {
                tell_object(me, HIW"\n忽然，你感觉体内寒气越聚越盛，全身似乎堕入冰窖，寒冷刺骨！\n"NOR);
                tell_object(me, HIW"\n过了一会，体内寒气逐渐聚在丹田处，全身精神为之一振，功力修为大大提升！\n"NOR);
                me->add("max_neili", me->query_skill("lengquan-shengong", 1)*2 + random(me->query_skill("lengquan-shengong", 1)*2));
                me->set("quest/雪山飞狐/武功/lengquan", 1);
        }
}
int practice_skill(object me)
{
        if((int)me->query_skill("lengquan-shengong", 1) >= 200)
        {
                if( (int)me->query("potential", 1) > 5
                 && (int)me->query("neili", 1) > 200)
                {
                        me->improve_skill("lengquan-shengong", (int)me->query_int()*2);
                        me->add("neili", -100); 
                        me->add("potential", -1*(1+random(3)));
                        return 1;
                }
                else
                        return notify_fail("你现在的修为不足以提高冷泉神功了。\n");
        }
        else
                return notify_fail("你现在的冷泉神功修为只能用学(learn)的来增加熟练度。\n");
}

string exert_function_file(string func)
{
       return __DIR__"quest/lengquan-shengong/" + func;
}

mapping exercise_msg(object me)
{
  return ([
    "status_msg" : HIW + me->name()+"脸上寒气浮现，隐隐有微光流动。" NOR,
    "start_my_msg" : "你手捏剑诀，将寒冰真气提起在体内慢慢转动。\n",
    "start_other_msg" : HIB + me->name()+"摆了个盘膝打坐的姿势，全身笼罩在一片寒气之内。\n"NOR,
    "halt_msg" : "$N双眼一睁，极速压下内息站了起来。\n",
    "end_my_msg" : "你将寒冰真气按周天之势搬运了一周，感觉精神充沛多了。\n"NOR,
    "end_other_msg" : HIW"只见"+me->name()+"缓缓睁开双眼站起，浑身散发出一股冰冷的寒气。\n"NOR,
    "heal_msg" : HIW"$N运起寒冰真气，开始缓缓运气疗伤。\n"NOR,
    "heal_finish_msg" : HIY"$N内息一停，却见伤势已经全好了。\n"NOR,
    "heal_unfinish_msg" : "$N眉头一皱，“哇”地吐出一口瘀血，看来这伤还没有全好。\n",
    "heal_halt_msg" : "$N急急把内息一压，也不顾身上的伤势立即站了起来。\n",
    ]);
}


int ob_hit(object ob, object me, int damage)
{
	int skill = me->query_skill("force");
	int skill2= ob->query_skill("force");
	int neili = me->query_skill("neili");
	int neili2= ob->query_skill("neili");
        int i = 1;
	string msg;

	if(!me->query_temp("quest/雪山飞狐/武功/lengquan"))	return 1;
	if(me->query_skill("lengquan-shengong")<200) return 1;
	if ( random(skill) > (skill2*14/10/2-1) )
	{
			msg = MAG"$N"+MAG+"默运冷泉神功，化去了$n"MAG;
			if(neili > neili2*2)
			{
				msg += "大部分的攻击！\n"NOR;
				i = -(damage/2 + random(damage));
			}
			else 
			{
				if(neili < neili2/4) 
				{
					msg += "很少的一点攻击！\n"NOR;
					i = -random(damage/4);
				}
				else 
				{
					msg += "一部分的攻击！\n"NOR;
					i = -(damage/3+random(damage/2));
				}
			}
		   message_vision(msg, me, ob);		
	}
	return i;
}
mixed hit_ob(object me, object victim, int damage_bonus, int factor, string attack_skill)
{
	if(!me->query_temp("quest/雪山飞狐/武功/lengquan"))	return;
	if(random(me->query("combat_exp"))<victim->query("combat_exp")/2) return;
	if(me->query_temp("quest/雪山飞狐/武功/force_attack"))
	{
		me->delete_temp("quest/雪山飞狐/武功/force_attack");
		return;
    }
	me->set_temp("quest/雪山飞狐/武功/force_attack",1);
	if (random(me->query_dex()) > victim->query_dex()/2)
	{
		message_vision(WHT"$n被$N这一招砍伤，急忙运起内力，驱出寒气，不由一阵手忙脚乱！\n"NOR, me, victim);
	    victim->start_lost(2+random(2));
		me->add("jingli",-50);
		me->add("neili",-100);
	 }
	 else if (random(me->query_con()) > victim->query_con()/2)
	 {
        message_vision(WHT"$n突然发现一缕寒气已经深入到五脏六腑，急忙运起内力，却为时已晚！\n"NOR,me,victim);	
        victim->apply_condition("cold_poison", victim->query_condition("cold_poison")+random(2)+1);
	 }
/*
         else if(userp(me))
//增加，fix bug
	 {
		 	
	    skill = me->query_skill("force");

	    i = damage_bonus * (skill+1) / 1000;
	    i = i/2+ random(i);
	    if(me->query_temp("weapon"))
		    i = i*3;
	    if(me->query_temp("apply/damage")>=100)
		i = i/3;
        message_vision(WHT"$n突然感觉真气隔阂，急忙运起内力驱寒，却只感觉胸口郁闷，伤势更加严重！\n"NOR,me,victim);	
          if(victim->query("qi")<i) i=victim->query("qi");
//增加，fix bug
		victim->recieve_wound("qi",i,me);
//       victim->receive_damage("qi",i,me);
//增加，fix bug
	 }
 */
} 
