//kaishu.c 楷书
#include <ansi.h>
#include <combat.h>

inherit F_SSERVER;

int query_ap() {return 3;}
string perform_name(){ return HIC"楷书"NOR; }

int perform(object me, object target)
{
	string msg;
	int lvl;
	object weapon = me->query_temp("weapon");

	if (!weapon || weapon->query("skill_type") != "brush")
	return notify_fail("你手里没有笔，无法写出楷书！\n");

	if( !target ) target = offensive_target(me);

	if( !target || !me->is_fighting(target) )
	return notify_fail("楷书只能在战斗中对对手使用。\n");

	if((lvl = (int)me->query_skill("yingou-bifa",1) )< 100 )
	return notify_fail("你的银钩笔法还不够娴熟，不会写出楷书！\n");

	if((int)me->query_skill("literate",1) < 100 )
	return notify_fail("你的读书写字等级不够，不能写出房玄龄碑文！\n");

	if((int)me->query_skill("guiyuan-tunafa",1) < 100)
	return notify_fail("你的归元吐纳法等级不够，不能写出房玄龄碑文！\n");

	if((int)me->query("max_neili") < 1500 )
	return notify_fail("你的内力太弱，写不出刚劲有力的楷书! \n");

	if((int)me->query("neili") < 1000 )
	return notify_fail("你的内力太少了，写出的楷书柔弱无力！\n");

	if( target->is_busy() )
	return notify_fail(target->name() + "目前正自顾不暇，放胆攻击吧。\n");

	if (me->query_skill_mapped("brush") != "yingou-bifa")
	return notify_fail("你现在无法写出「楷书」！\n");

	if (!objectp(weapon = me->query_temp("weapon")) || weapon->query("skill_type") != "brush")
	return notify_fail("你手中无笔，如何写字。\n");


	msg = HIC"$N一转身，手中笔摇幌，凌空写起「房玄龄碑」碑文，书法之中有点穴，点穴之中有书法，\n"+
	"当真是银钩铁划，劲峭凌厉，而雄伟中又蕴有一股秀逸的书卷气，招招法度严谨，宛如楷书\n"+
	"般的一笔不苟！\n" NOR;
  me->set_temp("apply/action/pfm",msg);
	
	msg = HIR"$n对书法只是入门，哪懂得碑文的意境，只见$N手中的笔满天在自己身上各大要穴附近\n"+
	"飞舞，只得勉力护住面门胸口要害，突感膝头一麻，原来已被$N倒转笔口，点中了穴道。\n"NOR;
  me->set_temp("apply/action/hit",msg);
	
	msg = HIY"哪知$p曾临写过「房玄龄碑」，预计得到$P那一横之後会跟著写那一直，倒也守得井井有条，\n"+
	"丝毫不见败象。\n"NOR;
	target->set_temp("apply/action/parry",msg);
	target->set_temp("apply/action/dodge",msg);
	
	me->receive_damage("neili", 500+lvl/2);
	me->add_temp("apply/damage",400+lvl);
	me->add_temp("apply/pfm_damage",200+lvl/4);
	me->add_temp("apply/pfm_attack",180+lvl/10);
	if(COMBAT_D->do_attack(me, target,weapon,random(2)?1:3)) target->start_busy(2+random(2));
	me->add_temp("apply/damage",-400-lvl);
	me->add_temp("apply/pfm_damage",-200-lvl/4);
	me->add_temp("apply/pfm_attack",-180-lvl/10);
	me->delete_temp("apply/action/");
	target->delete_temp("apply/action/");
  me->start_busy(random(2));
  
	return 1;
}


int help(object me)
{
        write(HIC"\n银钩笔法之「楷书」："NOR"\n");
        write(@HELP
        
        要求：最大内力 1500 以上；
              当前内力 1000 以上；
              归元吐纳法等级 100 以上；                
              银钩笔法等级 100 以上；
              读书写字等级 100 以上；
              激发银钩笔法为笔法；
               
HELP
        );
        return 1;
}