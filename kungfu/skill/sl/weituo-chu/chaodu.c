#include <ansi.h>
#include <combat.h>
inherit F_DBASE;
inherit F_SSERVER;

int query_ap() {return 10;}

int perform(object me, object target)
{
	object weapon;


	if( !target && me->is_fighting() ) target = offensive_target(me);

	if( !target
	|| !target->is_character()
	|| !me->is_fighting(target) )
	return notify_fail("「超度极乐」只能对战斗中的对手使用。\n");

	if( !objectp(weapon = me->query_temp("weapon"))
	|| weapon->query("skill_type") != "club" )
	return notify_fail("你手中无棍，怎能运用「超度极乐」？！\n");

	if(me->query("menggu"))
	return notify_fail("你这种佛门败类，如何能够运用佛门神功。\n");

	if(me->query_temp("sl_leidong"))
	return notify_fail("你正在使用韦陀杵的「雷动九天」，全身内力已经灌注到手中兵器中，无暇他顾了。\n");

	if((int)me->query_skill("weituo-chu", 1) < 300 )
	return notify_fail("你「韦陀杵」修为还不够，还未能领悟「超度极乐」！\n");

	if((int)me->query_str()-(int)me->query_temp("apply/strength")<50)
	return notify_fail("你的膂力不够，无法用出「超度极乐」\n");

	if((int)me->query("con")+(int)me->query_skill("force",1)/10<50)
	return notify_fail("你的根骨不够，无法承受「超度极乐」的金刚大力。\n");

	if((int)me->query_skill("buddhism",1) < 200)
	return notify_fail("你禅宗修为还不够，还未能领悟「超度极乐」！\n");

	if((int)me->query_skill("jingang-quan",1) < 300)
	return notify_fail("「超度极乐」是以金刚大力发出，无坚不摧，你金刚拳修为尚浅吧?\n");

	if( me->query_skill("yijin-jing", 1) < 300 )
	return notify_fail("你的内功修为火候未到，施展只会伤及自身！\n");

	if( me->query_skill_mapped("force") != "yijin-jing")
	return notify_fail("你所用的内功与「超度极乐」心法相悖！\n");

	if (me->query_skill_mapped("club") != "weituo-chu"
	|| me->query_skill_mapped("parry") != "weituo-chu")
	return notify_fail("你现在无法使用「超度极乐」进行攻击。\n");

	if( me->query("max_neili") < 5000 )
	return notify_fail("你的内力修为不足，劲力不足以施展「超度极乐」！\n");

	if( me->query("neili") < 2000 )
	return notify_fail("你的内力不够，劲力不足以施展「超度极乐」！\n");

	if( me->query("jingli") < 1500 )
	return notify_fail("你的精力有限，不足以施展「超度极乐」！\n");

	message_vision(HIC"\n突然$N面露佛光暴喝一声：「超度极乐」！只见$N身形暴涨，如同韦陀尊者再世一般，右拳突出击在"+weapon->name()+HIC"当中。\n只听「哄」的一声，"+weapon->name()+HIC"被打的粉碎象千颗"+HIY"流星"+HIC"飞向$n！\n" NOR, me,target);
	weapon->move(environment(me));
	weapon->set("name", weapon->query("name")+"的碎片");
	weapon->unequip();
	weapon->set("value", 0);
	weapon->set("weapon_prop", 0);
  me->add("neili", -1000);
	if(COMBAT_D->do_attack(me, target, weapon, 0, 1) 
	|| COMBAT_D->do_attack(me, target, weapon, 0, 1)
	|| COMBAT_D->do_attack(me, target, weapon, 0, 1))
	{
    target->set("qi",-1);
		message_vision(HIY"\n$n哪里见过这么刚猛霸道的招式，招架不及，被拳力击中，肋骨当即根根断裂！\n" NOR, me,target);
		COMBAT_D->report_status(target);
		me->start_busy(2);
	}
	else {
		message_vision(HIY"\n$n眼见$N来势凶狠，不及招架，身体贴地滑行，倒滑出数丈，躲过了这致命一击！\n" NOR, me,target);
		me->start_busy(1+random(2));
	}
	return 1;
}

string perform_name(){ return HIC"超度极乐"NOR; }

int help(object me)
{
        write(
@HELP
超度极乐
       是佛门神功－－「韦陀杵」的特殊攻击，将手中兵器击碎，化成千万片碎片攻击对手。
       乃是「韦陀杵」的终极招式。

	要求：
		最大内力 5000 以上；      
		当前内力 2000 以上； 
		当前精力 1500 以上； 
		易筋经等级 300 以上；
		韦陀杵等级 300 以上；
		金刚拳等级 300 以上
		禅宗心法等级 200 以上；
		激发韦陀杵为招架；
		激发易筋经为内功；
		装备棍类兵器；     

HELP
        );
        return 1;
}
