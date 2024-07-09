// fuxue.c 拈花指 「拈花拂穴
// by hunthu //99.01.03
//felchion 2008/3/6
inherit F_SSERVER;

#include <ansi.h>
#include <combat.h>
string perform_name() { return HIY"拈花拂穴"NOR; }

int query_ap() {return 3;}

int perform(object me, object target)
{
	int lvl;
	string msg;
	if( !target ) target = offensive_target(me);

	if( !target
	||!target->is_character()
	||!me->is_fighting(target))
	return notify_fail("拈花拂穴只能对战斗中的对手使用。\n");

	if (target->query_condition("fx_busy"))
	return notify_fail(target->name() + "目前正自顾不暇，放胆攻击吧!\n");

	if( me->query_temp("sl_fuxue"))
	return notify_fail("你刚刚使用过拈花拂穴，需要调息片刻!\n");

	if( me->query_temp("weapon"))
	return notify_fail("你现在不能使用拈花拂穴\n");

	if( (lvl =(int)me->query_skill("yijin-jing",1)) < 120)
	return notify_fail("你易筋经的功力不够不能使用拈花拂穴!\n");

	if((int)me->query_skill("nianhua-zhi",1) < 120)
	return notify_fail("你的指法修为不够,目前还不能使用拈花拂穴绝技!\n");

	if ( me->query_skill_mapped("finger") != "nianhua-zhi" )
	return notify_fail("你必须用拈花指才能出此奇功。 \n");

	if( me->query_skill_mapped("force") != "yijin-jing" )
	return notify_fail("你的内功不对。\n");

	if((int)me->query("max_neili")<2000)
	return notify_fail("你的内力修为尚欠火候。\n");

	if((int)me->query("neili") < 500)
	return notify_fail("你内力现在不够, 不能使用拈花拂穴! \n");

	msg = HIY "$N面露慈祥微笑，手指轻轻弹出，似乎在弹落花朵上的朝露，生怕弹损花瓣一般，"+
	"指力若有若无轻柔之至。\n一股阴柔的内力罩住了$n的周身要穴。\n";

	me->set_temp("apply/action/pfm", msg);
	me->set_temp("apply/action/hit",HIG"结果$p被$P拂中要穴，周身真气乱窜，狂喷一口鲜血。\n"NOR);
	
	me->receive_damage("neili", 500+lvl);
	me->add_temp("apply/damage",300+lvl/2);
	me->add_temp("apply/pfm_damage",180+lvl/5);
	me->add_temp("apply/pfm_attack",180+lvl/10);
	if (COMBAT_D->do_attack(me, target,0,random(2)?0:3)
	&& target	&& me->is_fighting(target)	&& target->query("qi")>0 ) {
    target->apply_condition("fx_busy", 6);
	}
	me->add_temp("apply/damage",-300-lvl/2);
	me->add_temp("apply/pfm_damage",-180-lvl/5);
	me->add_temp("apply/pfm_attack",-180-lvl/10);	      
	me->delete_temp("apply/action/");
	me->start_busy(random(2));

	return 1;
}


int help(object me)
{
        write(HIY"\n「拈花拂穴」："NOR"\n");
        write(@HELP
        
        拈花微笑以阴柔之力冲击对手经脉，使其真气受滞，可与其
        它少林绝技配合使用。

        要求：  最大内力 2000 以上；
                当前内力 500 以上；
                拈花指等级 120 以上；
                易筋经等级 120 以上；
                激发拈花指为指法；
                手中不能持有武器；
                
HELP
        );
        return 1;
}