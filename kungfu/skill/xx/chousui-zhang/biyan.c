#include <ansi.h>
#include <combat.h>

inherit F_SSERVER;

string perform_name() {return GRN"碧焰"NOR;}
int query_ap() {return 2;}

int perform(object me, object target)
{
	object ob;
	string msg, *limbs;
	int lvl,neili;
	
	lvl = (int)me->query_skill("chousui-zhang", 1);
	neili = (int)me->query("neili");
	if( !target ) target = offensive_target(me);
	if( !objectp(target)
	||      !target->is_character()
	||      !me->is_fighting(target)
	|| !living(target)
	|| environment(me)!=environment(target) )
	 	return notify_fail("「碧焰」攻击只能对战斗中的对手使用。\n");

	if( objectp(me->query_temp("weapon")) )
		return notify_fail("你必须空手才能使用「碧焰」！\n");
	if(!objectp(ob = present("huo yan", environment(me))))
		return notify_fail("没有火堆怎么驱动「碧焰」进行攻击？\n");
	if(me->query_temp("pfm_chousui"))
		return notify_fail("你正在驱动火堆进行攻击！\n");
	
	if( lvl < 100 )
		return notify_fail("你发现自己的星宿毒掌还不够娴熟，无法使用「碧焰」进行攻击。\n");

	if( (int)me->query_skill("huagong-dafa",1) < 100 )
		return notify_fail("你发现自己的化功大法修为不够，无法使用「碧焰」进行攻击。\n");

	if((int)me->query_skill("poison",1) < 100)
		return notify_fail("你发现自己体内聚毒不够，无法使用「碧焰」进行攻击。\n");

	if (me->query_skill_prepared("strike") != "chousui-zhang"
	|| me->query_skill_mapped("strike") != "chousui-zhang"
	|| me->query_skill_mapped("parry") != "chousui-zhang")
		return notify_fail("你必须用抽髓掌才能使用「碧焰」进行攻击。\n");

	if( me->query_skill_mapped("force") != "huagong-dafa")
		return notify_fail("你发现自己所用的内功无法进行「碧焰」攻击。\n");

	if( (int)me->query("max_neili") < 2000)
		return notify_fail("你发现自己内力太弱，无法驱动「碧焰」进行攻击。\n");

	if(neili  < 1500)
		return notify_fail("你发现自己现在真气太弱，使不出「碧焰」进行攻击。\n");

	message_vision(HIC"\n$N右手指向"HIR"火堆"HIC"，默不作声的注视着火焰，左掌按胸，口中喃喃的不知说些什么。。。\n"NOR,me);


	limbs = target->query("limbs");
	msg = HIC"\n然后$N突然双掌向前平平推出，但见"RED"火焰"HIC"微微颤动了两下，吐出一道"GRN"碧焰"HIC"，射向$n的$l"NOR;  
  msg = replace_string(msg, "$l", limbs[random(sizeof(limbs))]);
	
	me->set_temp("apply/action/pfm",msg);
 // me->set_temp("apply/action/hit", HIR"跟着$P横劲发出，$p给这么一挤，身子被自己的劲力带得不由主的向前一冲，跨出数步！\n"NOR);       
	target->set_temp("apply/action/parry", HIY"\n$p全力向后一纵一闪，凶险万分地避开了那急速而来的"GRN"碧焰"HIY"！\n" NOR);
	target->set_temp("apply/action/dodge", HIY"\n$p全力向后一纵一闪，凶险万分地避开了那急速而来的"GRN"碧焰"HIY"！\n" NOR);	
	
	me->receive_damage("neili", 500+lvl);
	me->add_temp("apply/pfm_hurt",300+lvl/2);
	me->add_temp("apply/pfm_attack",120+lvl/10);
	if(COMBAT_D->do_attack(me, target,0,random(2)?0:3,2)) target->add_condition("xx_poison", 6);
	me->add_temp("apply/pfm_hurt",-300-lvl/2);
	me->add_temp("apply/pfm_attack",-120-lvl/10);	         
  me->delete_temp("apply/action/");
  target->delete_temp("apply/action/");

	me->start_busy(random(2));
	me->start_perform(3,"碧焰");
	return 1;
}

int help(object me)
{
        write(HIW"\n抽髓掌之「碧焰」："NOR"\n\n");
        write(@HELP
        要求：  当前内力 1500 以上;
                最大内力 2000 以上；
                毒技等级 100 以上；
                抽髓掌等级 100 以上；
                化功大法等级 100 以上；                
                基本内功等级 100 以上;
                且手中无兵器。

        
HELP
        );
        return 1;
}