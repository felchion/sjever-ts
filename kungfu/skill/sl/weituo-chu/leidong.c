#include <ansi.h>

inherit F_DBASE;
inherit F_SSERVER;

int query_ap() {return 5;}

int perform(object me, object target)
{
	object weapon;
	int lvl;

	if( !target && me->is_fighting() ) 
	  target = offensive_target(me);

	if( !target
	|| !target->is_character()
	|| !me->is_fighting(target) )
	return notify_fail("「雷动九天」只能对战斗中的对手使用。\n");

	if( !objectp(weapon = me->query_temp("weapon"))
	|| weapon->query("skill_type") != "club" )
	return notify_fail("你手中无棍，怎能运用「雷动九天」？！\n");

	if( me->query_temp("sl_leidong"))
	return notify_fail("你刚使完「雷动九天」，目前气血翻涌，无法再次运用！\n");

	if( me->query_temp("fumo"))
	return notify_fail("你正在使用大金刚拳的特殊攻击「金刚伏魔」！\n");

	if( me->query_temp("sl/xiouliqiankun"))
	return notify_fail("你正使用般若掌的绝技袖里乾坤呢。\n");

	if((int)me->query_skill("weituo-chu", 1) < 150
	||(int)me->query_skill("weituo-chu", 1) < 150
	||(int)me->query_str() <=30 )
	return notify_fail("你修为还不够，还未能领悟「雷动九天」！\n");

	if( me->query_skill("yijin-jing", 1) < 150 )
	return notify_fail("你的内功修为火候未到，施展只会伤及自身！\n");

	if( me->query_skill_mapped("force") != "yijin-jing")
	return notify_fail("你所用的内功与「雷动九天」心法相悖！\n");

	if (me->query_skill_mapped("club") != "weituo-chu"
	|| me->query_skill_mapped("parry") != "weituo-chu")
	return notify_fail("你现在无法使用「雷动九天」进行攻击。\n");

	if( me->query("max_neili") < 2000 )
	return notify_fail("你的内力修为不足，劲力不足以施展「雷动九天」！\n");

	if( me->query("neili") < 1000 )
	return notify_fail("你的内力不够，劲力不足以施展「雷动九天」！\n");


	message_vision(HIC"\n突然$N大喝一声：「雷动九天」，只见$N双目尽赤，须发皆飞，真气溶入"+weapon->name()+HIC"当中，只听“嗡”的一声，"+weapon->name()+HIC"发出闪闪光亮！\n" NOR, me);

	if(userp(me)){
		me->add("neili", -300);
		me->add("jingli", -50);
		if(weapon->query("rigidity")<10)
		weapon->add("rigidity", 1);
	}
	lvl = me->query_skill("weituo-chu", 1);
	me->set_temp("sl_leidong", lvl);
	me->add_temp("apply/damage", 400+lvl/2);
	me->add_temp("apply/pfm_damage", 200+lvl/5);
	me->add_temp("apply/pfm_attack", 150+lvl/10);
	me->add_temp("apply/pfm_kill", 30);
	call_out("remove_effect", 1,  me, weapon, 30);
	return 1;
}

void remove_effect(object me,object weapon,int count)
{
	int lvl;
	if(!me) return;
	if(! me->is_fighting()
	|| !weapon
	|| me->query_skill_mapped("club") != "weituo-chu"
	|| me->query_skill_mapped("parry") != "weituo-chu"
	|| count < 1 ){
		lvl = me->query_temp("sl_leidong");
		me->add_temp("apply/damage", -400-lvl/2);
		me->add_temp("apply/pfm_damage", -200-lvl/5);
		me->add_temp("apply/pfm_attack", -150-lvl/10);
		me->add_temp("apply/pfm_kill", -30);
		me->delete_temp("sl_leidong");
		if (weapon)
		message_vision(HIY"\n$N一套「雷动九天」使完，手中"NOR+weapon->name()+HIY"上的光芒渐渐也消失了。\n"NOR, me);
		return;
	}
	else {
		me->start_perform(1,"「雷动九天」");
		call_out("remove_effect", 1, me, weapon, count--);
	}
}

string perform_name(){ return HIC"雷动九天"NOR; }

int help(object me)
{
        write(
@HELP
雷动九天
       是佛门神功－－韦陀杵的特殊攻击，将一身佛门内功注入手中所用禅杖中，
       斩妖伏魔，无坚不摧，刚猛无倗，乃是佛门第一等的伏魔神功。
       
要求：
		最大内力 2000 以上；      
		当前内力 1000 以上；  
		易筋经等级 150 以上；
		韦陀杵等级 150 以上；
		金刚拳等级 300 以上
		禅宗心法等级 200 以上；
		激发韦陀杵为招架；
		激发易筋经为内功；
		装备棍类兵器；  
HELP
        );
        return 1;
}
