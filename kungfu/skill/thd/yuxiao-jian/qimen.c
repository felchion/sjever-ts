// By Darken@SJ
// Modify By felchion@SJever 2008/2/9
// Fixed bug that target can not remove perform effect after performer was die。
#include <ansi.h>
inherit F_SSERVER;

int query_ap() {return 5;}

string perform_name() { return HIG"奇门玉箫"NOR; }

int perform(object me, object target)
{
  object weapon;
  string msg;
  int i, lvl;
  
  if( !target ) target = offensive_target(me);
  
  if( !objectp(target)
  ||  !target->is_character()
  ||  !me->is_fighting(target) )
    return notify_fail("「奇门玉箫」只能对战斗中的对手使用。\n");
  
  if (!objectp(weapon = me->query_temp("weapon"))
  || ((string)weapon->query("skill_type") != "sword" 
    && (string)weapon->query("skill_type") != "xiao"))
    return notify_fail("你使用的武器不对。\n");
 
  if (me->query_temp("thd/qimen"))
    return notify_fail("你正在使用「奇门玉箫」。\n");
  
  if((int)me->query_skill("yuxiao-jian",1) < 100 )
    return notify_fail("你的玉箫剑法还不够火候，使不出「奇门玉箫」。\n");
  
  if((int)me->query_skill("qimen-bagua",1) < 100 )
    return notify_fail("你的奇门八卦还不够火候，使不出「奇门玉箫」。\n");
  
  if((string)me->query_skill_mapped("sword") != "yuxiao-jian" && userp(me))
    return notify_fail("你现在无法使用「奇门玉箫」！\n");
 
  if((int)me->query("max_neili", 1) < 1000 )
    return notify_fail("你的内力修为不够使用「奇门玉箫」！\n");
 
  if((int)me->query("max_jingli") < 500 )
    return notify_fail("你的精力修为不够使用「奇门玉箫」！\n");
  
  if((int)me->query("neili")<500)
    return notify_fail("你的真气不够！\n");

  msg = HIG "$N手中"+weapon->name()+HIG"连刺，潇洒自如，招招都是攻势，一招不待$n化解开去，剑招已连绵而至！\n"+
  "$N东刺一剑，西削一招，连出五招，与武学常理完全相反。\n"+
  "$n愈斗愈惊，周身要害似已全在"+weapon->name()+HIG"笼罩之下！\n"NOR;
  message_vision(msg, me, target);
  lvl = me->query_skill("yuxiao-jian", 1) + me->query_skill("qimen-bagua", 1);
  me->add_temp("apply/damage", 350+lvl);
  me->add_temp("apply/pfm_damage", 150+lvl/5);
  me->add_temp("apply/pfm_attack", 120+lvl/10);
  target->add_temp("apply/dodge", -100-lvl/5);
  target->add_temp("apply/parry", -100-lvl/5);
  me->add("neili", -300);
  me->add("jingli", -80);
  
  for(i = 0; i < 5; i++)
    if (objectp(target) && me->is_fighting(target))
      COMBAT_D->do_attack(me, target, me->query_temp("weapon"), 1);
      
  me->add_temp("apply/damage", -350-lvl);
	me->add_temp("apply/pfm_damage", -150-lvl/5);
	me->add_temp("apply/pfm_attack", -120-lvl/10);    
  
  call_out("remove_effect", 15,me, target,lvl);
  return 1;
}

void remove_effect(object me, object target, int lvl)
{
  if (!target) return;

  if (target && me && me->is_fighting(target))
  message_vision(HIC"$n慢慢看透了$N的攻势，不再被玉箫剑法的奇门剑路所迷惑。\n"NOR,me,target);

  if (target){
    target->add_temp("apply/dodge", 100+lvl/5);
    target->add_temp("apply/parry", 100+lvl/5);
  }
}

int help(object me)
{
        write(HIG"\n「奇门玉箫」："NOR"\n");
        write(@HELP

        使用奇门八卦方位采取特殊攻势，向对手快速攻击之余迷惑对手，降低对手
        闪躲以及招架的能力。

        要求：  碧海潮声等级 100 以上；
                玉箫剑法等级 100 以上；
                奇门八卦等级 100 以上；
HELP
        );
        return 1;
}


