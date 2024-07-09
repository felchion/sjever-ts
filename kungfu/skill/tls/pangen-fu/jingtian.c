//  jingtian.c 惊天断云斧 
//  by Leontt 2000.6.11

#include <ansi.h>
inherit F_SSERVER;

int query_ap() {return 3;}

string perform_name(){ return HIW"惊天断云"NOR; }

int perform(object me, object target)
{
	int lvl;
	object weapon;
	weapon = me->query_temp("weapon");

	if( !target ) target = offensive_target(me);

	if( !target || !objectp(target) || !me->is_fighting(target) )
		return notify_fail("「惊天断云斧」只能在战斗中对对手使用。\n");

	if (!weapon || weapon->query("skill_type") != "axe"
	|| me->query_skill_mapped("axe") != "pangen-fu")
		return notify_fail("你使用的兵刃不对，无法使用「惊天断云斧」！\n");

	if( (int)me->query_skill("pangen-fu",1) < 100 )
		return notify_fail("你的盘根斧法不够娴熟，不能使用「惊天断云斧」！\n");

	if( (int)me->query_skill("axe",1) < 100 )
		return notify_fail("你的基本斧法等级不够，不能使用「惊天断云斧」！\n");

	if( (int)me->query_skill("qiantian-yiyang", 1) < 100  
	&& (int)me->query_skill("kurong-changong", 1) < 100  )
		return notify_fail("你的乾天一阳功等级不够，不能使用「惊天断云斧」！\n");

	if( (int)me->query("max_neili") < 1000 )
		return notify_fail("你的内力太弱，不能使用「惊天断云斧」！\n");

	if( (int)me->query("neili") < 500)
		return notify_fail("你的内力太少了，无法使用出「惊天断云斧」！\n");

	message_vision(HIY"\n$N突然大喝一声犹如惊雷一般，将手中"+weapon->query("name") +HIY"斜向上举起，然后在半空中划出一"+
		"道圆弧，随即径直斜下斩向$n！\n"NOR, me,target);
	
	me->add("neili", - (300+random(200)));
	me->add("jingli", -80);
	
	lvl = (int)me->query_skill("pangen-fu", 1);
	me->add_temp("apply/pfm_damage", 350+lvl/2);
	me->add_temp("apply/damage", 250+lvl/4);
	me->add_temp("apply/pfm_attack", 140+lvl/10);
	me->set_temp("apply/action/color", YEL);
	me->set_temp("tls/jingtian", 1);
	
	COMBAT_D->do_attack(me, target, me->query_temp("weapon"), random(3)?3:1);
	
	if (present(target,environment(me)) && me->is_fighting(target))
		COMBAT_D->do_attack(me, target, me->query_temp("weapon"), random(2)?1:3);
	
	if (present(target,environment(me))
	&& me->is_fighting(target)
	&& me->query_skill("pangen-fu", 1) > 200)
		COMBAT_D->do_attack(me, target, me->query_temp("weapon"), 3);
	
	me->add_temp("apply/pfm_damage", -350-lvl/2);
	me->add_temp("apply/damage", -250-lvl/4);
	me->add_temp("apply/pfm_attack", -140-lvl/10);
	me->delete_temp("tls/jingtian");
	me->delete_temp("apply/action/color");
	me->start_busy(random(2)+1);

	return 1;
}

int help(object me)
{
        write(HIW"\n盘根错节斧之「惊天断云」："NOR"\n");
        write(@HELP
        
        要求：最大内力 1000 以上；
              当前内力  500 以上；
              盘根错节斧等级 100 以上；                
              基本斧法等级 100 以上；
              激发盘根错节斧为斧法；
              激发盘根错节斧为招架；                
HELP
        );
        return 1;
}