#include <ansi.h>
#include <combat.h>

inherit F_SSERVER;

string perform_name() {return HIW"银火"NOR;}
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
	||  !target->is_character()
	||  !me->is_fighting(target)
	||  !living(target)
	|| environment(me)!=environment(target) )
	 	return notify_fail("「银火」攻击只能对战斗中的对手使用。\n");

	if( objectp(me->query_temp("weapon")) )
		return notify_fail("你必须空手才能使用「银火」！\n");
	if(!objectp(ob = present("huo yan", environment(me))))
		return notify_fail("没有火堆怎么驱动「银火」进行攻击？\n");
	if(me->query_temp("pfm_chousui"))
		return notify_fail("你正在驱动火堆进行攻击！\n");
	
	if( lvl < 100 )
		return notify_fail("你发现自己的星宿毒掌还不够娴熟，无法使用「银火」进行攻击。\n");

	if( (int)me->query_skill("huagong-dafa",1) < 100 )
		return notify_fail("你发现自己的化功大法修为不够，无法使用「银火」进行攻击。\n");

	if (me->query_skill_prepared("strike") != "chousui-zhang"
	|| me->query_skill_mapped("strike") != "chousui-zhang"
	|| me->query_skill_mapped("parry") != "chousui-zhang")
		return notify_fail("你必须用抽髓掌才能使用「银火」进行攻击。\n");

	if( me->query_skill_mapped("force") != "huagong-dafa")
		return notify_fail("你发现自己所用的内功无法进行「银火」攻击。\n");

	if( (int)me->query("max_neili") < 1500)
		return notify_fail("你发现自己内力太弱，无法驱动「银火」进行攻击。\n");

	if(neili  < 800)
		return notify_fail("你发现自己现在真气太弱，使不出「银火」进行攻击。\n");

	message_vision(YEL"\n$N衣袖一拂，一股劲气直射入"HIR"火焰"YEL"之中。从中又分出一道细细的"WHT"银火"YEL"，便似一条"WHT"银色长蛇"YEL"
	横卧空际，轻轻摆动，颜色又是鲜艳，又是诡异，"RED"光芒"YEL"闪烁不定！\n"NOR,me);

	limbs = target->query("limbs");
	msg = YEL"\n$N右手食指轻弹，突然火焰中升起"GRN"绿色鬼火"YEL"，此时"WHT"银火"YEL"倏地缩回，跟着又向$n$l射去，力道极猛，去势奇快！\n"NOR; 
  msg = replace_string(msg, "$l", limbs[random(sizeof(limbs))]);
	
	me->set_temp("apply/action/pfm",msg); 
	target->set_temp("apply/action/parry", HIY"\n$p全力向右一滚，那"HIW"银火"HIY"堪堪擦肩而过，端的是凶险无比！\n" NOR);
	target->set_temp("apply/action/dodge", HIY"\n$p全力向右一滚，那"HIW"银火"HIY"堪堪擦肩而过，端的是凶险无比！\n" NOR);	
	
	me->receive_damage("neili", 500+lvl/2);
	me->add_temp("apply/force",500+lvl/2);
	me->add_temp("apply/pfm_force",150+lvl/5);
	me->add_temp("apply/pfm_attack",150+lvl/10);
	if(COMBAT_D->do_attack(me, target,0,random(2)?0:3)) target->add_condition("xx_poison", 6);
	me->add_temp("apply/force",-500-lvl/2);
	me->add_temp("apply/pfm_force",-150-lvl/5);
	me->add_temp("apply/pfm_attack",-150-lvl/10);	         
  me->delete_temp("apply/action/");
  target->delete_temp("apply/action/");

	me->start_busy(1);
	return 1;
}

int help(object me)
{
        write(HIW"\n抽髓掌之「银火」："NOR"\n\n");
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