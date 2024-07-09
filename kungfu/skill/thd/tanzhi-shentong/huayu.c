// huayu.c 漫天花雨

#include <ansi.h>
inherit F_SSERVER;

string perform_name(){ return CYN"漫天花雨"NOR; }

int query_ap() {return 3;}


int perform(object me, object target)
{
	string msg;
	object weapon;
  int num, lvl;

	if( !target ) target = offensive_target(me);

	if( !objectp(target)
	 || !target->is_character() 
	 || !me->is_fighting(target))
		return notify_fail("漫天花雨只能对战斗中的对手使用。\n");

	if (!(me->query("thd/perform") & 16))
		return notify_fail("你虽然听说过“漫天花雨”这一招，可是却未获传授。\n");

	lvl = me->query_skill("tanzhi-shentong", 1);

	if (lvl < 120)
		return notify_fail("你的弹指神通不够娴熟，无法使用漫天花雨。\n");

	if (me->query_skill("throwing", 1) < 150)
		return notify_fail("你的基本暗器不够娴熟，无法使用漫天花雨。\n");

   if ( me->query_skill_mapped("throwing") != "tanzhi-shentong")
		return notify_fail("你现在的情况，无法使用漫天花雨。\n");

	if ( me->query_skill("bihai-chaosheng", 1) < 150 )
		return notify_fail("你的碧海潮生功等级不够，无法使用漫天花雨。\n");

	weapon = me->query_temp("weapon");

	if (!weapon)
		return notify_fail("漫天花雨必须装备暗器才能使用。\n");

	if ((num = (weapon->query_amount()- lvl / 20)) < 0 )
		return notify_fail("你掏了掏背囊，发现里面的暗器已经不够使用漫天花雨的要求了！\n");

	if (me->query("neili") < 1000)
		return notify_fail("你的内力不够，无法使用漫天花雨。\n");

	if (me->query("jingli") < 900)
		return notify_fail("你的精力不够，无法使用漫天花雨。\n");

	me->set_temp("apply/action/pfm",
	    HIW"$N双手在背囊抓出数" + weapon->query("base_unit") + weapon->query("name") + HIW"，运用指力漫天地弹了出去，$n只觉眼前似乎在下一场暗器雨"NOR);
  me->set_temp("thd/huayu",1);
  me->add_temp("apply/damage",500+lvl/2);
  me->add_temp("apply/pfm_damage",400+lvl/4);
  me->add_temp("apply/pfm_attack",210+lvl/10);
  if (COMBAT_D->do_attack(me, target,weapon,random(2)?0:3))  {
		msg = HIG"$n躲闪不及，“噗噗”地几声，被几道暗器击中！\n"NOR;
		me->add("neili", -500-lvl);
		me->add("jingli", -lvl/2);
	  target->add_busy(2);
	}
	else {
		me->add("neili", -250-lvl/2);
		me->add("jingli", -lvl/4);
		msg = HIG"$n吓得赶忙抱头团身，万分狼狈地向旁边一滚，方才堪堪地躲了过去。\n" NOR;
	}
	me->delete_temp("huayu/action");
	me->delete_temp("thd/huayu",1);
  me->add_temp("apply/damage",-500-lvl/2);
  me->add_temp("apply/pfm_damage",-400-lvl/4);
  me->add_temp("apply/pfm_attack",-210-lvl/10);
	if (objectp(target))
		message_vision(msg, me, target);

	me->start_busy(random(2));
	if (num<0) num = 0;
	if (num == 0) {
		weapon->unequip();
		tell_object(me, "\n你的" + weapon->query("name") + "用完了！\n");
	}
	weapon->set_amount(num);
	return 1;
}

int help(object me)
{
        write(HIG"\n弹指神通之「漫天花雨」："NOR"\n");
        write(@HELP

        一次同时弹出许多的暗器，让对手防不胜防，避无可避。同时，由於使用暗
        器数量极多，若是连续使用必须注意，以免战中发现暗器不够了。

        要求：  碧海潮生等级 150 以上；
                弹指神通等级 150 以上；
                当前内力 1000 以上；
                必须准备暗器。
HELP
        );
        return 1;
}
