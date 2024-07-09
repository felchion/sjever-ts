#include <ansi.h>
#include <combat.h>
inherit F_SSERVER;
int query_ap() {return 3;}
int perform(object me, object target)
{
	object weapon;
	int lvl;
       
	if( !target ) target = offensive_target(me);

	if(!objectp(weapon = me->query_temp("weapon"))
	 ||(string)weapon->query("skill_type") != "blade")
		return notify_fail("你使用的武器不对。\n");

	if( !objectp(target) || !me->is_fighting(target) )
		return notify_fail("「飞雁随风刃」只能在战斗中对对手使用。\n");

	if((int)me->query_skill("linji-zhuang", 1) < 150 )
		return notify_fail("你的临济十二庄等级不够，不能使用飞雁随风刃。\n");       

	if(me->query_skill_prepared("strike") != "sixiang-zhang"
	 || me->query_skill_mapped("strike") != "sixiang-zhang")
		return notify_fail("你不会四象掌，现在无法使用飞雁随风刃。\n");

	if (me->query_skill_mapped("force") != "linji-zhuang")
		return notify_fail("你所使用的内功不对。\n");

	if ((int)me->query_skill("yanxing-daofa", 1) < 150 )
		return notify_fail("你的雁行刀法等级不够，不能使用飞雁随风刃！\n");    

	if( me->query_skill_mapped("blade") != "yanxing-daofa"
	 || me->query_skill_mapped("parry") != "yanxing-daofa")
		return notify_fail("你现在不能使用飞雁随风刃！\n"); 

	if ((int)me->query_skill("blade", 1) < 150 )
		return notify_fail("你的基本刀法不够娴熟，不能使用飞雁随风刃！\n");    

	if ((int)me->query_skill("sixiang-zhang", 1) < 150 )
		return notify_fail("你的四象掌不够娴熟，不能使用飞雁随风刃！\n");    

	if ((int)me->query_skill("strike", 1) < 150 )
		return notify_fail("你的四象掌不够娴熟，不能使用飞雁随风刃！\n");    

	if ((int)me->query("neili") < 1200 )
		return notify_fail("你现在真气太弱，不能使用飞雁随风刃！\n");

	if ((int)me->query("jingli") < 800 )
		return notify_fail("你现在精力不够，不能使用飞雁随风刃！\n");

	if ((int)me->query("max_neili") < 2000 )
		return notify_fail("你现在内力太弱，不能使用飞雁随风刃！\n");

	lvl = me->query_skill("linji-zhuang", 1) + me->query_skill("yanxing-daofa", 1) + me->query_skill("sixiang-zhang", 1);
	lvl /= 3;
	
	me->add("neili", -300);
	me->add("jinli", -100);
	me->add_temp("apply/pfm_attack", 150+lvl/10);
	me->add_temp("apply/damage", 300+lvl/2);
	me->add_temp("apply/pfm_damage", 160+lvl/5);
	me->add_temp("apply/action/color", HIW);
	me->set_temp("apply/action/skill","sixiang-zhang");
	  
	
	message_vision(HIY"$N突然把手中"+weapon->query("name")+HIY"往上一扔！\n"NOR, me, target);
	message_vision(HIR"$N看$n一怔，立刻使出一招四象掌的绝学“飘雪穿云掌”！\n"NOR, me, target);
	weapon->unequip(); 
	COMBAT_D->do_attack(me, target, 0, random(2)?1:3);
	weapon->wield();
		
	message_vision(HIR"\n$N接了空中落下来的刀，唰唰连劈两刀！\n"NOR, me, target);	
	if ( me->is_fighting(target))
		COMBAT_D->do_attack(me, target, me->query_temp("weapon"), 1);
	if ( me->is_fighting(target))
		COMBAT_D->do_attack(me, target, me->query_temp("weapon"), random(2)?1:3);
	
	me->add_temp("apply/pfm_attack", -150-lvl/10);
	me->add_temp("apply/damage", -300-lvl/2);
	me->add_temp("apply/pfm_damage", -160-lvl/5);
	me->delete_temp("apply/action");
	
	me->start_busy(random(2));   
	return 1;
}

string perform_name(){ return HIY"飞雁随风刃"NOR; }

int help(object me)
{
        write(HIY"\n雁行刀法之「飞雁随风刃」："NOR"\n\n");
        write(@HELP
        要求：  当前内力 1200 以上；
                最大内力 2000 以上；
                当前精力 800 以上；
                雁行刀法等级 150 以上；
                四象掌等级 150 以上；
                临济十二庄等级 150 以上；
                基本刀法等级 150 以上；
                基本掌法等级 150 以上；
                激发刀法为雁行刀法；
                激发招架为雁行刀法；
                激发掌法为四象掌；
                备掌法为四象掌；
                激发内功为临济十二庄；
                且手持兵器。

HELP
        );
        return 1;
}
