// cailian.c 「彩莲」
// creat by Caiji@SJ
// Modified by Lklv 2001.10.21
// Modified by Lane@SJ 2005.7.19

#include <ansi.h>
#include <combat.h>
inherit F_SSERVER;


int query_ap() {return 6;}

string perform_name(){ return HIM"彩莲"NOR; }

string *color = ({ HIC,HIR,HIG,HIY,HIB,HIM });

int perform(object me, object target)
{
	int lvl;
	string msg;
	if( !target ) target = offensive_target(me);

	if( !objectp(target)
	 ||!target->is_character()
	 ||!me->is_fighting(target) )
		return notify_fail("「彩莲」只能对战斗中的对手使用。\n");

	if( objectp(me->query_temp("weapon")) )
		return notify_fail("「彩莲」只能空手使用。\n");

	if( me->query_skill_prepared("strike") != "lianhua-zhang"
	 || me->query_skill_mapped("strike") != "lianhua-zhang"
	 || me->query_skill_mapped("parry") != "lianhua-zhang" )
		return notify_fail("你必须用莲花掌才能出此奇功。 \n");

	if( me->query_skill("huntian-qigong",1) < 100 )
		return notify_fail("你的混天气功功力还不足以使用「彩莲」!\n");

	if( (lvl = me->query_skill("lianhua-zhang",1)) < 100 )
		return notify_fail("你的莲花掌修为不够，还不能使出「彩莲」绝技!\n");

	if( me->query_skill_mapped("force") != "huntian-qigong" )
		return notify_fail("你的内功不对。\n");

	if( me->query("max_neili") < 1500 )
		return notify_fail("你的内力修为尚欠火候。\n");

	if( me->query("neili") < 1000 )
		return notify_fail("你现在内力不够, 不能使用「彩莲」! \n");

	if( me->query("jingli") < 500 )
		return notify_fail("你现在精力不够, 不能使用「彩莲」! \n");

	if( me->query_temp("lhz/cailian") )
		return notify_fail("你现在气血翻涌，现在还不能使用「彩莲」，! \n");
	
	if( target->query_condition("no_perform") )
		return notify_fail("对方已经被你的彩莲所迷惑，大胆进攻吧！\n");

	msg = HIM "\n但见$N脚下后退一步，左手一记“千叶莲花”，朝前劈出，在$n身前像火树银花般爆发开来，\n"+
		"刹那间光芒四射，大如车轮，有如一朵盛开的彩色莲花，奇亮的闪光，照得人睁不开眼睛";

	me->set_temp("lhz/cailian",1);

  me->set_temp("apply/action/pfm",msg);
  target->set_temp("apply/action/parry", HIM"$n虽被这精妙的一招震得连退了数步，但招式章法却丝毫不乱，竟也勉强接了下来。\n");
	target->set_temp("apply/action/dodge", HIM"$n虽被这精妙的一招震得连退了数步，但招式章法却丝毫不乱，竟也勉强接了下来。\n");	
 
  
  me->receive_damage("neili", 500+lvl/2);
	me->add_temp("apply/damage",500+lvl);
	me->add_temp("apply/pfm_damage",250+lvl/4);
	me->add_temp("apply/pfm_attack",240+lvl/10);
  
	if( COMBAT_D->do_attack(me, target,0,random(2)?3:0) ) {
		msg = BLINK+" ";
		if( !userp(target) ) target->add_busy(random(2));
		msg+=color[random(sizeof(color))]+
		"             /\\/\\/\\/\\/\\/\\ \n"+
		"            </\\/\\/\\/\\/\\/\\/\\>\n"+
		"           </\\/\\/\\/\\/\\/\\/\\/\\>\n"+
		"           \\_<>_\\_\\<>/_/_<>_/\n"+
		"           / <> / /<>\\ \\ <> \\ \n"+NOR;
		msg += HIY"结果$n被$N的彩色莲花罩住，瞬间便失去了攻击的能力! \n" NOR;
		target->apply_condition("no_perform",10);
		message_vision(msg, me, target);
	}
	
	me->delete_temp("lhz/cailian");
	me->add_temp("apply/damage",-500-lvl);
	me->add_temp("apply/pfm_damage",-250-lvl/4);
	me->add_temp("apply/pfm_attack",-240-lvl/10);
	
	return 1;
}

int help(object me)
{
        write(HIM"\n莲花掌「彩莲」："NOR"\n");
        write(@HELP
        彩莲为莲花掌绝技之一。功力达到者可用自身内力发出莲花状的掌风。
        对手往往被掌风所迷惑，攻守不能自如。
        要求：  最大内力 1500 以上；
                当前内力 1000 以上；
                当前精力 500 以上；
                莲花掌等级 100 以上；
                混天气功等级 100 以上；
                激发招架为莲花掌。
HELP
        );
        return 1;
}
