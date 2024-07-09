// qishang.c 

#include <ansi.h>
#include <combat.h>

inherit F_SSERVER;
void remove_effect(object target);
int query_ap() {return 6;}

string *songs = ({"五行之气调阴阳","损心伤肺摧肝肠","藏离精失意恍惚","三焦齐逆兮魂魄飞扬"});

int perform(object me, object target)
{
	int lvl;
	string song = songs[random(sizeof(songs))];

	if( !target ) target = offensive_target(me);

	if( !objectp(target)
	 || !target->is_character()
	 || !me->is_fighting(target) || !living(target) )
		return notify_fail("「七伤总诀」只能在战斗中对对手使用。\n");

	if( objectp(me->query_temp("weapon")) )
		return notify_fail("你必须空手使用「七伤总诀」！\n");

	if( (int)me->query_skill("qishang-quan",1) < 450 )
		return notify_fail("你的七伤拳不够娴熟，不会使用「七伤总诀」！\n");

	if(!me->query_skill("jiuyang-shengong", 1))
	if(userp(me) && (int)me->query_skill("shenghuo-shengong",1) < 450 )
		return notify_fail("你的圣火神功等级不够，不能使用「七伤总诀」！\n");

	if(!me->query_skill("jiuyang-shengong", 1))
	if(userp(me) && me->query_skill_mapped("force") != "shenghuo-shengong")
		return notify_fail("你没有圣火神功内功，无法使用「七伤总诀」进行攻击。\n");

	if( (int)me->query("max_neili") < 1500 )
		return notify_fail("你的内力太弱，不能使用「七伤总诀」！\n");

	if( (int)me->query("neili") < 1200 )
		return notify_fail("你的内力太少了，无法使用出「七伤总诀」！\n");

	if( me->query_temp("qkdny/yin") )
		return notify_fail("你正在施展乾坤大挪移「引」字诀呢！\n");

	if( me->query_temp("qkdny/hua") )
		return notify_fail("你正在施展乾坤大挪移「化」字诀呢！\n");

	if( me->query_skill_mapped("parry") != "qishang-quan"
	&& me->query_skill_mapped("parry") != "qiankun-danuoyi")
		return notify_fail("你需要激发招架为七伤拳，方能使用「七伤总诀」进行攻击。\n");

	if (me->query_skill_prepared("cuff") != "qishang-quan"
	 || me->query_skill_mapped("cuff") != "qishang-quan")
		return notify_fail("你现在无法使用「七伤总诀」进行攻击。\n");

	if(target->query_temp("qishang"))
		return notify_fail("对方已经身受「七伤总诀」，你不用费劲了！\n");

	
	me->set_temp("apply/action/pfm", HIR"\n突然$N眼射精光，朗声念出句似歌非歌、似诗非诗的拳诀：“"+song+"”，砰的一拳击向$n");
  me->set_temp("apply/action/hit", HIW"拳面刚一触到$n的身体，$N便暗暗发力，将隐藏在在拳内的七种力道送了过去！\n"NOR);       
	target->set_temp("apply/action/parry", HIY"$p见此情景，一声惊噫，连忙收回自己的劲力，闪身避让！\n"NOR);
	target->set_temp("apply/action/dodge", HIY"$N只觉$n体内内力极强，$N暗藏在拳内的各种力道根本送不出去！\n"NOR);	
	
	me->receive_damage("neili", 500+lvl);
	me->add_temp("apply/pfm_hurt",300+lvl/2);
	me->add_temp("apply/pfm_attack",150+lvl/10);
	if (COMBAT_D->do_attack(me, target,0,random(2)?0:3,2)){
			target->add_temp("apply/str", -10);
			target->add_temp("apply/dex", -10);
			target->add_temp("apply/int", -10);
			target->add_temp("apply/con", -10);
			target->set_temp("qishang",1);
			target->add_condition("qishang_poison", 10);
			target->add_condition("no_perform", 2+random(3));
			target->start_call_out( (: call_other, __FILE__, "remove_effect", target :), random(5)+6);
  }
	me->add_temp("apply/pfm_hurt",-300-lvl/2);
	me->add_temp("apply/pfm_attack",-150-lvl/10);	    
	me->delete_temp("apply/action/");
	target->delete_temp("apply/action/"); 

	me->start_busy(random(2));
	return 1;
}

void remove_effect(object target)
{
	if( ! target || !target->query_temp("qishang")) return;
	target->add_temp("apply/str", 10);
	target->add_temp("apply/dex", 10);
	target->add_temp("apply/int", 10);
	target->add_temp("apply/con", 10);
	target->clear_condition("no_perform");
	target->delete_temp("qishang");
	tell_object(target, HIW"\n终于体内那几股暗劲在遍布你的全身之后，开始慢慢散去了。\n"NOR);
}

string perform_name(){ return HIR"七伤总诀"NOR; }

int help(object me)
{
	write(HIR"\n七伤拳之「七伤总诀」："NOR"\n\n");
	write(@HELP
	要求：	
		当前内力 1200 以上；
		最大内力 1500 以上；
		七伤拳等级 450 以上；
		圣火神功等级 450 以上；
		激发拳法为七伤拳；
		激发招架为七伤拳或乾坤大挪移；
		备拳法为七伤拳。

HELP
	);
	return 1;
}
