//风雷 fenglei.c 

#include <ansi.h>
#include <skill_class.h>

inherit F_SSERVER;
int query_ap() {return 4;}
string perform_name() {return BLU"风雷四击"NOR;}

int perform(object me, object target)
{
  object weapon;
  int lvl,i;

  weapon = me->query_temp("weapon");

  if( !target ) target = offensive_target(me);
  if( !objectp(target)) return notify_fail("你要对谁攻击？\n");

  if( !me->is_fighting() )
    return notify_fail("「风雷四击」只能在战斗中使用。\n");

  weapon = me->query_temp("weapon");
  
  if (!weapon || weapon->query("skill_type") != "sword"
  || me->query_skill_mapped("sword") != "duanjia-jianfa")
    return notify_fail("手中无剑,你怎么使得出「风雷四击」！\n");

  if( (int)me->query("max_neili") < 1000 )
    return notify_fail("你的内力不够！\n");

  if( (int)me->query("neili") < 500 )
    return notify_fail("你的真气不够！\n");

  if( (int)me->query("jingli") < 300 )
    return notify_fail("你的精力不够！\n");

  if(!me->query_skill("kurong-changong", 1))
    if(me->query_skill("qiantian-yiyang", 1) < 100)
      return notify_fail("以你现在的内功修为还使不出「阳关三叠」。\n");

  if (me->query_skill_mapped("force") != "kurong-changong" )
    if(me->query_skill_mapped("force") != "qiantian-yiyang")
      return notify_fail("你所使用的内功不对。\n");

  if( (int)me->query_skill("duanjia-jianfa",1) < 100 )
    return notify_fail("你的段家剑法还不到家，无法使用「风雷四击」绝技！\n");
  
  if( (int)me->query_skill("qingyan-zhang",1) < 100 
  || me->query_skill_mapped("strike") != "qingyan-zhang")
    return notify_fail("你的五罗清烟掌还不到家，无法使用「风雷四击」绝技！\n");

  if (me->query_skill_prepared("strike") != "qingyan-zhang"
  || me->query_skill_mapped("parry") != "qingyan-zhang")
    return notify_fail("你现在无法使用「风雷四击」进行攻击。\n");

  if( (int)me->query_skill("tianlong-xiang",1) < 100 )
    return notify_fail("你的天龙翔还不到家，无法使用「风雷四击」绝技！\n");

  lvl = ((int)me->query_skill("duanjia-jianfa",1)+(int)me->query_skill("qingyan-zhang",1))/2;
  
  message_vision(BLU"\n$N蹂身而上，左手五罗清烟掌，右手段家剑，施展一绝招「风雷四击」！\n"NOR, me,target);
  
  me->add("neili",-(150+random(50)));
  me->add("jingli",-50);
  
  me->set_temp("fenglei",4);
  me->add_temp("apply/damage", 350+lvl/2);
  me->add_temp("apply/pfm_attack",150+lvl/10);
  me->add_temp("apply/pfm_damage", 180+lvl/5);
  me->set_temp("apply/action/skill","qingyan-zhang");
  
  for(i=0;i<4;i++){
    if(random(2)) 
      me->set_temp("fl_sword",1);  
    
    if (me->query_temp("fl_sword")){
      if(me->is_fighting(target))
        COMBAT_D->do_attack(me, target, me->query_temp("weapon"), random(2)?3:1);    
    }
    else {
      weapon->unequip();
      if(me->is_fighting(target))
      COMBAT_D->do_attack(me, target, me->query_temp("weapon"), random(2)?3:1);
      weapon->wield();
    }
    me->delete_temp("fl_sword");
  }
  
  me->delete_temp("fenglei");
  me->delete_temp("fl_sword");
  me->add_temp("apply/damage", -350-lvl/2);
  me->add_temp("apply/pfm_attack",-150-lvl/10);
  me->add_temp("apply/pfm_damage", -180-lvl/5);
  me->delete_temp("apply/action");
  
  me->start_busy(random(2)+1);
  return 1;
}

int help(object me)
{
   write(HIY"\n「风雷四击」："NOR"\n");
   write(@HELP

     要求: 最大内力1000 以上；
           当前内力500  以上；
           乾天一阳等级 100 以上；
           段家剑法等级 100 以上；
           五罗轻烟掌等级 100 以上；
           天龙翔等级 100 以上；
           激发掌法为五罗轻烟掌；
           激发剑法为段家剑法。
HELP
   );
   return 1;
}