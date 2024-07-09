// fuxue.c 兰花拂穴
// Created by Numa 19990927

#include <ansi.h>

inherit F_SSERVER;

int query_ap() {return 4;}

string *xue_name = ({
"曲池穴","冲阳穴","臂儒穴","白海穴","灵台穴","风府穴","神风穴","玉书穴",
"劳宫穴","膻中穴","曲池穴","关元穴","曲骨穴","中极穴","承浆穴","天突穴","百会穴",
"幽门穴","章门穴","大横穴","紫宫穴","冷渊穴","天井穴","极泉穴","清灵穴","至阳穴",
}); 

void remove_effect(object me);

int perform(object me, object target)
{
  string name;
  int lvl;

  if( !target ) target = offensive_target(me);

  if( !objectp(target)
   || !me->is_fighting(target)
   || !living(target)
   || environment(target)!=environment(me))
    return notify_fail("「兰花拂穴」只能在战斗中对对手使用。\n");

  if( me->query_temp("weapon") )
    return notify_fail("你必须空手使用「兰花拂穴」！\n");

  if( ( lvl=me->query_skill("lanhua-shou",1) )< 180 )
    return notify_fail("你的兰花拂穴手还不够娴熟，恐怕做不到「兰花拂穴」的「准」字。\n");

  if( me->query_skill("suibo-zhuliu",1) < 180 )
    return notify_fail("你的随波逐流还不够娴熟，恐怕做不到「兰花拂穴」的「快」字。\n");

  if( me->query_skill("qimen-bagua",1) < 180 )
    return notify_fail("你的奇门八卦还不够娴熟，恐怕做不到「兰花拂穴」的「奇」字。\n");

  if (me->query_skill_mapped("force") != "bihai-chaosheng")
    return notify_fail("你所使用的内功不对。\n");

  if( me->query("neili") < 700 )
    return notify_fail("你的内力太少了，无法使出「兰花拂穴」！\n");

  if( me->query("jingli") < 500 )
    return notify_fail("你的精力太少了，无法使出「兰花拂穴」！\n");

  if (me->query_skill_prepared("hand") != "lanhua-shou"
   || me->query_skill_mapped("hand") != "lanhua-shou"
   || me->query_skill_mapped("parry") != "lanhua-shou" )
    return notify_fail("你现在无法使用「兰花拂穴」进行攻击。\n");   
  
  name = xue_name[random(sizeof(xue_name))];
  
  me->set_temp("apply/action/pfm", HIC"$N右手挥出，拇指与食指扣起，余下三指略张，手指如一枝兰花般伸出，拂向$n，姿势美妙已极");
  me->set_temp("apply/action/hit", CYN"$n只感「"RED + name + CYN"」上一麻，被$N拂中穴道。\n");       
	target->set_temp("apply/action/parry", CYN"$n只感「"RED + name + CYN"」上一麻，总算变招迅速，没给$N拂中穴道。\n");
	target->set_temp("apply/action/dodge", CYN"$n只感「"RED + name + CYN"」上一麻，总算变招迅速，没给$N拂中穴道。\n");
  me->add("neili", -150);
  me->set_temp("thd/fuxue",1);
  me->add_temp("apply/damage",400+lvl);
  me->add_temp("apply/pfm_damage",200+lvl/5);
  me->add_temp("apply/pfm_attack",240+lvl/10);
  COMBAT_D->do_attack(me, target,0,random(2)?1:3);
  me->add_temp("apply/damage",-400-lvl);
  me->add_temp("apply/pfm_damage",-200-lvl/5);
  me->add_temp("apply/pfm_attack",-240-lvl/10);
  me->delete_temp("thd/fuxue");   
  me->start_busy(random(2));
  return 1;
	
}

string perform_name(){ return HIC"兰花拂穴"NOR; }

int help(object me)
{
	write(HIC"\n兰花拂穴手之「兰花拂穴」："NOR"\n\n");
	write(@HELP
	要求：	当前内力 700 以上；
		当前精力 500 以上；
		兰花拂穴手等级 180 以上；
		奇门八卦等级 180 以上：
		随波逐流等级 180 以上；
		激发内功为碧海潮生功；
		激发手法为兰花拂穴手；
		激发招架为兰花拂穴手；
		备手法为兰花拂穴手且无兵器。

HELP
	);
	return 1;
}
