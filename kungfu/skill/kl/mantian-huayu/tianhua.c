// huayu.c 漫天花雨

#include <ansi.h>
inherit F_SSERVER;

string perform_name(){ return HIM"天花乱坠"NOR; }
int query_ap() {return 2;}

int perform(object me, object target)
{
	object weapon;
  int num, lvl;

	if( !target ) target = offensive_target(me);

	if( !objectp(target)
	 || !target->is_character() 
	 || !me->is_fighting(target))
		return notify_fail("天花乱坠只能对战斗中的对手使用。\n");

	lvl = me->query_skill("mantian-huayu", 1);

	if (lvl < 100)
		return notify_fail("你的漫天花雨不够娴熟，无法使用天花乱坠。\n");

	if (me->query_skill("throwing", 1) < 100)
		return notify_fail("你的基本暗器不够娴熟，无法使用天花乱坠。\n");

   if ( me->query_skill_mapped("throwing") != "mantian-huayu")
		return notify_fail("你现在的情况，无法使用漫天花雨。\n");

	if ( me->query_skill("xuantian-wuji", 1) < 100 )
		return notify_fail("你的玄天无极功等级不够，无法使用天花乱坠。\n");

	weapon = me->query_temp("weapon");

	if (!weapon)
		return notify_fail("天花乱坠必须装备暗器才能使用。\n");

	if ((num = weapon->query_amount()- lvl / 20) < 0 )
		return notify_fail("你掏了掏背囊，发现里面的暗器已经不够使用天花乱坠的要求了！\n");

	if (me->query("neili") < 1000)
		return notify_fail("你的内力不够，无法使用天花乱坠。\n");

	if (me->query("jingli") < 900)
		return notify_fail("你的精力不够，无法使用天花乱坠。\n");


 	me->add("neili", -500-lvl);
	me->add("jingli", -lvl/2);
		
		
	message_vision(HIM"$N高高跃起，抓出一把" + weapon->query("name") + HIM"，双手轻挥，无数"+weapon->query("name") +HIM"暗含劲道像花瓣一般慢悠悠向$n飘去，其中更是藏有无限杀机。\n"NOR , me , target);


  me->add_temp("apply/action/color",HIW);
  me->add_temp("apply/damage",350+lvl/2);
  me->add_temp("apply/pfm_damage",150+lvl/5);
  me->add_temp("apply/pfm_attack",120+lvl/10);
  COMBAT_D->do_attack(me, target,weapon,1);

	if (me->is_fighting(target))
	  COMBAT_D->do_attack(me, target, me->query_temp("weapon"),random(3)?1:3);
  
  if (me->is_fighting(target))
	  COMBAT_D->do_attack(me, target, me->query_temp("weapon"),random(2)?1:3);
  

	me->delete_temp("apply/action/color");
  me->add_temp("apply/damage",-350-lvl/2);
  me->add_temp("apply/pfm_damage",-150-lvl/5);
  me->add_temp("apply/pfm_attack",-120-lvl/10);

	me->start_busy(1);
	if (num == 0) {
		weapon->unequip();
		tell_object(me, "\n你的" + weapon->query("name") + "用完了！\n");
	}
	weapon->set_amount(num);
	return 1;
}

int help(object me)
{
        write(HIM"\n漫天花雨之「天花乱坠」："NOR"\n");
        write(@HELP

        要求：  玄天无极功等级 100 以上；
                漫天花雨等级 100 以上；
                当前内力 1000 以上；
                必须准备暗器。
HELP
        );
        return 1;
}
