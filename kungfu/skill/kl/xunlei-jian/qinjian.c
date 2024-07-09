//ged@zy by 2008.2.14
#include <ansi.h>

inherit F_SSERVER;

int query_ap() {return 3;}

int perform(object me, object target)
{
	object weapon;
	int lvl;

	if( !target ) target = offensive_target(me);

	if( !objectp(target)
	|| !target->is_character()
	|| !me->is_fighting(target)
	|| !living(target))
		return notify_fail("琴剑追魂只能对战斗中的对手使用。\n");

	if( lvl=me->query_skill("xunlei-jian", 1) < 350 )
		return notify_fail("你的迅雷剑功力太浅，无法使用琴剑追魂。\n");

	if( me->query_skill("sword", 1) < 350 )
		return notify_fail("你的基本剑法功力太浅，无法使用琴剑追魂。\n");

	if( me->query_skill("xuantian-wuji", 1) < 350 )
		return notify_fail("你的内功功力太浅，无法使用琴剑追魂击。\n");

	if( me->query("max_neili") < 3000 )
		return notify_fail("你的内力太浅，无法使用琴剑追魂。\n");

	if( me->query("neili") < 2000 )
		return notify_fail("你的真气不足，无法使用琴剑追魂。\n");

	if (me->query_skill_mapped("force") != "xuantian-wuji")
		return notify_fail("你用什么为基础来使迅雷剑绝技么?\n");

	if (!objectp(weapon = me->query_temp("weapon"))
	|| weapon->query("skill_type") != "sword"
	|| me->query_skill_mapped("sword") != "xunlei-jian" )
		return notify_fail("你不能使用琴剑追魂!\n");
	
	message_vision(HIM"\n$N终于显出了生平绝技，左手凌空虚拨，虚空中竟有琴声传来，右手剑势更是宛转灵动！\n"NOR,me,target);
	message_vision(HIG"$n心神大乱，不由得露出了破绽！\n" NOR,me,target);
	
	me->add("neili", -400-lvl/2);
	me->add("jingli", -100);
	
	me->add_temp("apply/pfm_attack", 120+lvl/10);
	me->add_temp("apply/damage", 500+lvl/2);
	me->add_temp("apply/pfm_damage", 180+lvl/5);
	
	COMBAT_D->do_attack(me, target, me->query_temp("weapon"), random(2)?1:3);

	if(target && me->is_fighting(target) && target->query("qi")>0) {
		message_vision(HIY"\n$N神色自如，剑随意走， 行如对月自舞，$n觉得眼前一花，分不清楚那个是真实身影！\n" NOR,me,target);
		COMBAT_D->do_attack(me, target, me->query_temp("weapon"), random(2)?1:3);
	}
	
	if(target && me->is_fighting(target) && target->query("qi")>0){
		message_vision(HIR"\n$N弹剑如歌，仰天长啸，$n只觉得自己心神大乱，无法续力！\n" NOR,me,target);
		COMBAT_D->do_attack(me, target, me->query_temp("weapon"), random(2)?1:3);
	}
	
	me->add_temp("apply/rush", 20);
	if(target && me->is_fighting(target) && target->query("qi")>0){
		message_vision(HIG"\n$N曲指在剑身上一弹，剑身立刻弯曲，旋即复又伸直，一曲一直间，好似琴弦拨动之声，似有十面埋伏曲之音！\n" NOR,me,target);
		COMBAT_D->do_attack(me, target, me->query_temp("weapon"), random(2)?1:3);
	}
	me->add_temp("apply/rush", -20);
	
	me->add_temp("apply/kill", 20);
	if(target && me->is_fighting(target) && target->query("qi")>0 ) {
		message_vision(HIC"\n$N剑道如飞，气势如虹，直对$n周身大穴而去，$n神色大变，慌忙中脚步大乱！\n" NOR,me,target);
		COMBAT_D->do_attack(me, target, me->query_temp("weapon"), 3);
	}
  me->add_temp("apply/kill", -20);
  
	me->add_temp("apply/pfm_attack", -120-lvl/10);
	me->add_temp("apply/damage", -500-lvl/2);
	me->add_temp("apply/pfm_damage", -180-lvl/5);	

	me->start_busy(1+random(2));
	me->start_perform(3+random(2), HIB"琴剑追魂"NOR);
	return 1;
}
string perform_name(){ return HIB"琴剑追魂"NOR; }

int help(object me)
{
        write(HIG"\n迅雷剑之「琴剑追魂」："NOR"\n");
        write(@HELP
        迅雷剑以其快、狠诸称，尤其是其速度,号称四处只见剑影不见人影。
        这「琴剑追魂」，却是融合了昆仑派琴剑两大绝艺的特色。

        要求：  最大内力 3000 以上；
                当前内力 2000 以上；
                迅雷剑等级 350 以上；
                玄天无极功等级 350 以上；
                激发招架为迅雷剑。
HELP
        );
        return 1;
}

