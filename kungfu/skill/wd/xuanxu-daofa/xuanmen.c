#include <ansi.h>

inherit F_SSERVER;

int query_ap() {return 4;}

int perform(object me, object target)
{
	object weapon;
	int i, lvl,attack_time;
	string msg;

	if( !target ) target = offensive_target(me);

	if( !objectp(target)
	 || !target->is_character()
	 || !me->is_fighting(target)
	 || !living(target) )
		return notify_fail("「玄门众妙」只能对战斗中的对手使用。\n");

	if(me->query_temp("wd/xm"))
		return notify_fail("你正在使用「玄门众妙」。\n");
		
	if( (int)me->query_skill("xuanxu-daofa", 1) < 120 )
		return notify_fail("你的玄虚刀法功力太浅。\n");

	if( (int)me->query_skill("blade", 1) < 120 )
		return notify_fail("你的基本刀法功力太浅。\n");

	if( (int)me->query_skill("yinyun-ziqi", 1) < 120 )
		return notify_fail("你的内功功力太浅。\n");

	if( (int)me->query("max_neili") <1500 )
		return notify_fail("你的内力太浅。\n");

	if( (int)me->query("neili") < 800 )
		return notify_fail("你的真气不够。\n");		

	if( me->query_skill_mapped("force") != "yinyun-ziqi" )
		return notify_fail("你用什么为内功基础来使「玄门众妙」?\n");

	if( !objectp(weapon = me->query_temp("weapon"))
	 || weapon->query("skill_type") != "blade"
	 || me->query_skill_mapped("blade") != "xuanxu-daofa" )
		return notify_fail("你现在无法使用「玄门众妙」。\n");


  msg = HIY"$N心随意转，手中"+weapon->name()+HIY"愈舞愈快，险而不知其势，妙而不知其踪。\n"+	
  				 "$n心头一震，竟不知何以为应。\n"NOR;
  message_vision(msg, me, target);
  
  lvl = me->query_skill("xuanxu-daofa", 1) + me->query_skill("taoism", 1);
  lvl = lvl*5/6;
	attack_time = 3+random(lvl/50);  
	if (attack_time>8) attack_time=8;

  me->add_temp("apply/damage", 260+lvl/2);
  me->add_temp("apply/pfm_damage", 130+lvl/5);
	me->add_temp("apply/pfm_attack", 120+lvl/10);
	me->add_temp("apply/action/color",YEL);
  me->add("neili", -300);
  me->add("jingli", -50);
 
  for(i = 0; i < attack_time; i++)
    if (objectp(target) && me->is_fighting(target))
      COMBAT_D->do_attack(me, target, me->query_temp("weapon"), random(3)?1:3);
	
  me->set_temp("wd/xm", 1);
  call_out("remove_effect", random(attack_time)+2,me, target,lvl);
	return 1;
}

void remove_effect(object me, object target, int lvl)
{
  if (me) {
    me->delete_temp("wd/xm");
 		me->delete_temp("apply/action/color");
 		me->add_temp("apply/damage", -260-lvl/2);
 		me->add_temp("apply/pfm_damage", -130-lvl/5);
		me->add_temp("apply/pfm_attack", -120-lvl/10);
  }

  if (target && me && me->is_fighting(target))
  message_vision(HIC"$N深深吸了一口气，手上攻势渐渐缓了下来。\n"NOR,me,target);
}

string perform_name(){ return HIG"玄门众妙"NOR; }

int help(object me)
{
        write(HIY"\n玄虚刀法之「玄门众妙」："NOR"\n\n");
        write(@HELP
        道可道，非常道。名可名，非常名。无名，天地之始；
        有名，万物之母；故恒无欲，以观其妙，恒有欲，以观
        其微。此两者，同出而异名，同谓之玄。玄之又玄，众
        妙之门。

        要求：  当前内力  800 以上;
                最大内力 1500 以上；
                玄虚刀法等级 120 以上；
                氤氲紫气等级 120 以上；
                基本刀法等级 120 以上；
                激发刀法为玄虚刀法；
                激发招架为玄虚刀法；
                激发内功为氤氲紫气。

HELP
        );
        return 1;
}