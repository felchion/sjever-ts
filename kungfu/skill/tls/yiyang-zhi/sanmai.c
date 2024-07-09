// Modified by snowman@SJ 05/12/2000
// Update By lsxk@hsbbs 2007/6/14

#include <ansi.h>
#include <combat.h> 
inherit F_SSERVER;


string perform_name() {return HIR"一指点三脉"NOR;}
int query_ap() {return 3;}

int perform(object me, object target)
{
  int lev,i=1;
  string msg;
  lev = (int)me->query_skill("yiyang-zhi", 1);
  
  if( !target ) target = offensive_target(me);

  if( !objectp(target)
  || !target->is_character()
  || !me->is_fighting(target)
  || !living(target) )
    return notify_fail("「一指点三脉」只能在战斗中使用。\n");

  if( me->query_temp("weapon"))
    return notify_fail("你拿着武器怎么能使用「一指点三脉」！\n");

  if( (int)me->query_skill("yiyang-zhi", 1) < 150 )
    return notify_fail("你的一阳指还不够娴熟，使不出「一指点三脉」绝技。\n");

  if((int)me->query_skill("force",1) < 150 )
    return notify_fail("你的内功修为还不够，使不出「一指点三脉」绝技。\n");

  if( me->query_temp("pf_rong"))
    return notify_fail("你正在运用「荣」字大法！\n");

  if(!me->query_skill("kurong-changong", 1))
    if(me->query_skill("qiantian-yiyang", 1) < 150)
      return notify_fail("以你现在的内功修为还使不出「一指点三脉」。\n");

  if (me->query_skill_prepared("finger") != "yiyang-zhi"
  || me->query_skill_mapped("finger") != "yiyang-zhi" )
    return notify_fail("你现在无法使用「一指点三脉」！\n");

  if (me->query_skill_mapped("force") != "kurong-changong" )
    if(me->query_skill_mapped("force") != "qiantian-yiyang")
      return notify_fail("你所使用的内功不对。\n");

  if( (int)me->query("max_neili") < 2000 )
    return notify_fail("你现在内力太弱，使不出「一指点三脉」。\n");

  if( (int)me->query("neili") < 1000 )
    return notify_fail("你现在真气太弱，使不出「一指点三脉」。\n");

  if( (int)me->query("jingli") < 400 )
    return notify_fail("你现在精力太少，使不出「一指点三脉」。\n");

  message_vision(HIR"\n$N出指舒缓自如，收臂潇洒飘逸，手臂颤动，犹如蜻蜓点水，一指点出，竟分袭三道大脉，快捷无伦！\n"NOR,me,target);
  
  me->add_temp("apply/damage", 200+lev/2);
  me->add_temp("apply/pfm_damage",150+lev/4);
  me->add_temp("apply/pfm_attack", 180+lev/10);
  if (lev>350) me->set_temp("apply/action/color", HIY);
  me->set_temp("yyz/yz3m",1);  
  msg = "$N右手食指微曲，突然点出，使的竟然便是「"HIW"一阳指"NOR"」。但见$N食指幌动，来势虽缓，$n上半身正面大穴\n"+
        "却已全在$N一指笼罩之下，竟不知要点的是那一处穴道，九处大穴皆大指之虞";
  me->set_temp("apply/action/pfm", msg);
  
  me->add("neili", -500-lev/2);
  
  COMBAT_D->do_attack(me, target);
  
  if( me->is_fighting(target) && (int)me->query_skill("yiyang-zhi",1) > 199 ) {
    msg = "$N招数未完，抢上数步，又是一指点出，这次却是指向$n小腹，这一指所盖罩的要穴更广，肚腹间冲脉十二大穴，自幽门、\n"+
          "通谷，下至中注、四满，直抵横骨、会阴，尽处于这一指威力之下"; 
    me->set_temp("apply/action/pfm", msg);
    COMBAT_D->do_attack(me, target);
  }
  
  if( me->is_fighting(target) && (int)me->query_skill("yiyang-zhi",1) > 249 ) {
    msg = "跟着第三指又出，这一指迅如闪电，直指$n眉心，指尖闪动，似乎头部太阳，人中，百会．．"+
          "等各大要穴尽在这一指笼罩";
    me->set_temp("apply/action/pfm", msg);
    COMBAT_D->do_attack(me, target);
  }
    
  me->delete_temp("yyz/yz3m");
  me->delete_temp("apply/action");
  me->add_temp("apply/damage", -200-lev/2);
  me->add_temp("apply/pfm_damage",-150-lev/4);
  me->add_temp("apply/pfm_attack", -180-lev/10);
  
  me->start_busy(random(2)+1);

  return 1;
}

int help(object me)
{
   write(HIR"\n一阳指之「一指点三脉」："NOR"\n");
   write(@HELP

     要求: 最大内力 2000 以上；
           当前内力大于 1000以上；
           一阳指等级 150 以上；
           基本内功等级 150 以上；
           特殊内功等级 150 以上；
           激发指法为一阳指。
HELP
   );
   return 1;
}
