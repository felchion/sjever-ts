// kuangfeng.c  狂风绝技
// Modified by Darken@SJ

#include <ansi.h>
inherit F_SSERVER;

int query_ap() {return 18;}

string perform_name() { return HIG"狂风绝技"NOR; }
void kuangfeng_attack(object me, object target, string skill, int lvl,int times);

int perform(object me, object target)
{
	int lvl;

	if( !target ) target = offensive_target(me);
	if( !objectp(target)
	|| !target->is_character()
	|| !me->is_fighting(target) 
	|| !living(target))
		return notify_fail("狂风绝技只能对战斗中的对手使用。\n");
	if (!(me->query("thd/perform") & 4))
		return notify_fail("你虽然听说过“狂风绝技”是桃花岛看门神技，可是却未获黄岛主传授。\n");
	if( objectp(me->query_temp("weapon")) )
		return notify_fail("你必须空手使用「狂风绝技」！\n");	
	if( me->query_temp("thd/kuangfeng") )
		return notify_fail("你正在使用「狂风绝技」！\n");
	if( (int)me->query_skill("luoying-zhang", 1) < 450 )
		return notify_fail("你的落英神剑掌不够娴熟，无法施展出狂风绝技。\n");
	if( (int)me->query_skill("xuanfeng-tui", 1) < 450 )
		return notify_fail("你的旋风扫叶腿不够娴熟，无法施展出狂风绝技。\n");
	if ((int)me->query_skill("bihai-chaosheng", 1) < 450)
		return notify_fail("你的碧海潮生功火候不够，无法施展出狂风绝技。\n");
	if ( (int)me->query("max_neili") < 8000)
		return notify_fail("你的内力不够，无法施展出狂风绝技。\n");
	if ( (int)me->query("neili") < 2000)
		return notify_fail("你的真气不够，无法施展出狂风绝技。\n");
	if (me->query_skill_prepared("leg") != "xuanfeng-tui"
	|| me->query_skill_mapped("leg") != "xuanfeng-tui"
	|| me->query_skill_mapped("strike") != "luoying-zhang" )
		return notify_fail("你现在无法使用狂风绝技进行攻击！\n");
	if (me->query_skill_mapped("force") != "bihai-chaosheng")
		return notify_fail("你的碧海潮生功修为不够，无法使用狂风绝技进行攻击！\n");

	lvl = (me->query_skill("xuanfeng-tui",1) + me->query_skill("luoying-zhang",1) )/2;
	message_vision(HBGRN "$N一声清啸，拳脚齐施，使出桃花岛「狂风绝技」迅捷无伦地攻向$n！\n"NOR,me, target);
	me->set_temp("thd/kuangfeng",1);
	me->start_perform(99,"狂风绝技");
	kuangfeng_attack(me, target, "leg", lvl ,6);
	return 1;
}

void kuangfeng_attack(object me, object target, string skill, int lvl,int times)
{
	int i;

	if (!me) return;
	if( !target || !living(target) ) target = offensive_target(me);
	
	if ( !target 
	|| times<1
	|| !me->is_fighting()
	|| me->query("neili") < 700
	|| !living(target) ) {
		me->start_perform(1,"狂风绝技");
		message_vision(HIG"$N身形一缓，停止了狂风绝技。\n"NOR, me);
		me->prepare_skill("strike");
		me->prepare_skill("leg", "xuanfeng-tui");
		me->delete_temp("no_hit");
		me->delete_temp("thd/kuangfeng");
		return;
	} 
  else {
  	if (times<6)
  	  message_vision(HBGRN"$N深吸一口气，又是三掌三腿攻向$n。\n"NOR, me,target);
  	me->receive_damage("neili", 200+lvl/2);
  	me->add_temp("apply/damage",1800);
  	me->add_temp("apply/pfm_damage",700);
  	me->add_temp("apply/pfm_attack",600);
  	me->add_temp("apply/action/color",HIG);
  	for (i = 0; i < 6; i++) {
  		if( !target ) target = offensive_target(me);
  		if (target && me->is_fighting(target)) {
  			if (me->query_skill_prepared("leg") == "xuanfeng-tui"){
  				me->prepare_skill("strike","luoying-zhang");
  				me->prepare_skill("leg");
  			}
  			else {
  				me->prepare_skill("strike");
  				me->prepare_skill("leg", "xuanfeng-tui");
  			}
  			me->delete_temp("no_hit");
  			COMBAT_D->do_attack(me, target,0,3);
  			me->set_temp("no_hit",1);
  		}
  	}
  	me->add_temp("apply/damage",-1800);
  	me->add_temp("apply/pfm_damage",-700);
  	me->add_temp("apply/pfm_attack",-600);
  	me->delete_temp("apply/action");
  	call_out("kuangfeng_attack", 2, me, target, "strike", lvl,times-1);
  }
}

int help(object me)
{
        write(HIG"\n旋风扫叶腿之「狂风绝技」："NOR"\n");
        write(@HELP

        以落英神剑掌配合旋风扫叶腿向对手快速攻击，为黄药师极得意的独门绝
        技。三招落英神剑掌，三招旋风扫叶腿，循环不止。

        要求：  碧海潮生功等级 450 以上；
                落英神剑掌等级 450 以上；
                旋风扫叶腿等级 450 以上；
                最大内力 8000 以上；
                当前内力 2000 以上；

HELP
        );
        return 1;
}


