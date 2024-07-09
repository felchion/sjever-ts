// jixue.c  血刀经 [祭血神刀]
// coded by emnil@sj	2/5/2000

#include <ansi.h>
#include <skill.h>
#include <combat.h>
#include <weapon.h>
inherit F_SSERVER;

string perform_name(){ return HIR"祭血神刀"NOR; }


int query_ap() {return 3;}

void jixue(object me,object target,int times,int now);
 
int perform(object me, object target)
{
	object ob,weapon;
	string msg;
	int lvl,times;
    
	if( !me->is_fighting() )
		return notify_fail("「祭血神刀」只能在战斗中使用。\n");

	if (!objectp(weapon = me->query_temp("weapon"))	|| (string)weapon->query("skill_type") != "blade")
		return notify_fail("你使用的武器不对。\n");
 
	if((int)me->query_skill("xuedao-jing",1) < 120)
		return notify_fail("你的刀法修为不够， 目前不能使用「祭血神刀」! \n");

	if(me->query_temp("xs/jixue"))
		return notify_fail("你正在使用「祭血神刀」! \n");

	if((int)me->query_skill("longxiang-boruo",1) < 120)
		return notify_fail("你的龙象般若功修为不够， 目前不能使用「祭血神刀」! \n");

	if( me->query_skill_mapped("blade") != "xuedao-jing")
		return notify_fail("你必须使用血刀经，才能使用「祭血神刀」的绝技！\n");

	if( me->query_skill_mapped("force") != "longxiang-boruo")
		return notify_fail("你现在所使用的内功同「龙象般若功」向抵触，不能使出「祭血神刀」的绝技！\n");

	if( (int)me->query("max_neili") < 2000 )
		return notify_fail("你的内力修为不够！\n");
	if((int)me->query("neili")<500)
		return notify_fail("你的真气不够！\n"); 


	ob = me->select_opponent();
	lvl = me->query_skill("xuedao-jing",1);
	me->set_temp("xs/jixue",1);
	times = me->query("env/jixue");
	if(times < 1) times = 1;
	if(times > lvl/100) times = lvl/100;
	if(times > 6) times = 6;

	msg = HIR"\n$N脸色陡然一变，口中念念有词，同时倒转兵刃，不知道要做什么！\n\n"NOR;
	message_vision(msg, me);
	me->add("neili", -100);
	me->start_perform(10,"祭血神刀");

	call_out("jixue",0,me,ob,times,0);

	return 1;
}

void jixue(object me,object target,int times,int now)
{
	string msg,weaname;
	object weapon;
	int lvl,damage,attack,qi;

	if( !objectp(me) ) return;
	if( !objectp(target) || !me->is_fighting(target) || (now >= times) || (me->query("neili")<200) || 
	  (!objectp(weapon = me->query_temp("weapon"))) || ((string)weapon->query("skill_type")!="blade") ) {
		me->set_temp("xs/jixue",0);
		me->start_perform(1,"祭血神刀");
		if(living(me))
			message_vision(HIC"\n$N慢慢收回内劲，脸上也似乎有了血色。\n"NOR, me);
		return;
	}

	weapon = me->query_temp("weapon");
	weaname = weapon->query("name");
	lvl = me->query_skill("xuedao-jing",1);


	now++;
	if(now <= 1)
		msg = HIR"\n$N狂啸一声，以兵刃自砍手臂，顿时一飙鲜血飞溅在"NOR+weaname+HIR"之上，"NOR+weaname+HIR"隐隐泛出一层血色。";
	else
	if(now <= 3)
		msg = HIR"\n$N狂啸一声，以兵刃自砍胸膛，顿时一腔鲜血飞溅在"+weaname+HIR"之上，"NOR+weaname+HIR"泛出一层血色！";
	else
		msg = RED"\n$N狂啸一声，当头砍了自己一刀，顿时一股鲜血飞溅在"+weaname+RED"之上，"NOR+weaname+RED"整个变的通红！！！";
	
	attack = 12-now;
	damage = 9-now;
	if(me->query_temp("xs/longxiang")) {
		damage--;
		if(damage<2) damage=2;
		msg += "$N更把龙象内劲附于兵刃之上！";
	}
	msg += "\n"NOR;
	message_vision(msg, me);
//	if(wizardp(me) && me->query("env/test")) tell_object(me,sprintf("damage:%d , attack:%d , qi:%d \n",force/damage,lvl/attack,-50*now));

	me->add_temp("apply/damage",300+lvl/(damage-2));
	me->add_temp("apply/pfm_damage",120+lvl/damage);
	me->add_temp("apply/pfm_attack",120+lvl/attack);

	if (present(target,environment(me)) && me->is_fighting(target))
		COMBAT_D->do_attack(me, target, me->query_temp("weapon"),random(3)?1:3);
	if (present(target,environment(me)) && me->is_fighting(target))
		COMBAT_D->do_attack(me, target, me->query_temp("weapon"), random(2)?1:3);

	me->add_temp("apply/damage", -300-lvl/(damage-2) );
	me->add_temp("apply/pfm_damage",-120-lvl/damage );
	me->add_temp("apply/pfm_attack",-120-lvl/attack);

	if(me->query("qi") < (1000*now)) 
		qi = me->query("qi");
	else
		qi = 1000*now;
	if(qi<0) qi = 0;

	me->receive_damage("qi", qi,"因以血祭刀而");
	me->receive_wound("qi", qi/2,"因以血祭刀而");

	me->add("neili", -200*now);
	if(me->query("neili") < 0)	me->set("neili",0);

	call_out("jixue",4+random(3),me,target,times,now);
}

int help(object me)
{
        write(HIR"\n「祭血神刀」："NOR"\n");
        write(@HELP
        
        要求：  最大内力 2000 以上；
                当前内力 500 以上；
                血刀经等级 120 以上；
                龙象般若功等级 120 以上；
                
          注：  可使用set jixue N(1-6)来设定祭血次数        
                
HELP
        );
        return 1;
}