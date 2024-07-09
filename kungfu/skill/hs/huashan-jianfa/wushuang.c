// wushuang.c 无双无对
// by hunthu
// modified by Olives@SJ 5/4/2001
// for some judgement fault
// 做成一个简单的quest,调整威力。 By lsxk@hsbbs 2007/7/25

#include <ansi.h>
#include <combat.h>
inherit F_SSERVER;

int query_ap() {return 2;}
string perform_name() {return HIR"无双无对"NOR;}

int perform(object me, int amount)
{
	object weapon, target;
	int lvl;
	weapon = me->query_temp("weapon");
	lvl = me->query_skill("huashan-jianfa",1);

	if( !target ) target = offensive_target(me);

	if( !objectp(target)
	|| !target->is_character()
	|| !me->is_fighting(target) )
	return notify_fail("「无双无对,宁氏一剑」只能对战斗中的对手使用。\n");

	if( !me->query("quest/hsjf/wushuang") )
	return notify_fail("你只是听说有这「无双无对,宁氏一剑」之技，对该绝技你尚还一窍不通！\n");

	if (!weapon || weapon->query("skill_type") != "sword"
	|| me->query_skill_mapped("sword") != "huashan-jianfa")
	return notify_fail("你现在无法使出「无双无对,宁氏一剑」之技！\n");

	if((int)me->query_skill("huashan-jianfa", 1) < 300 )
	return notify_fail("你的华山剑法火候不够，无法「无双无对,宁氏一剑」。\n");

	if((int)me->query_skill("sword", 1) < 300 )
	return notify_fail("你的基本剑法还不够娴熟，使不出「无双无对,宁氏一剑」绝技。\n");

	if( (int)me->query_skill("zixia-gong", 1) < 300 )
	return notify_fail("你的紫霞功修为不够，使不出「无双无对,宁氏一剑」绝技。\n");

	if (me->query_skill_mapped("force") != "zixia-gong")
	return notify_fail("没有紫霞功作为根基，是无法使出「无双无对,宁氏一剑」绝技的！\n");

	if (me->query("max_neili") < 3000)
	return notify_fail("「无双无对，宁氏一剑」乃是气宗绝学，你内力修为不够，无法发出如此凌厉的剑招。\n");

	if (me->query("neili") < 1500)
	return notify_fail("你的内力不够。\n");

	if (me->query("jingli") < 1000)
	return notify_fail("你的精力不够。\n");

	me->add("neili", -800);
	me->add("jingli", -200);
	me->set_temp("apply/action/pfm", HIR"\n$N突然间一声清啸，手中的"+weapon->name()+HIR"闪烁不定，围着$n身围疾刺，银光飞舞！\n猛地里$N一剑挺出，直刺$n心口，当真是捷如闪电，势若奔雷。"NOR);
  me->set_temp("apply/action/hit", HIW"\n$n大吃一惊，惊叫一声：“哎呀！”其时长剑剑尖已刺破他衣衫。\n$N右手向前疾送，"+weapon->query("name")+HIW+"护手已碰到$n的胸膛，眼见这一剑是在$n身上对穿而过，直没至柄。\n"NOR);       
	target->set_temp("apply/action/parry", CYN"\n$n大声叫道：“来的好！”知这一招凌厉绝伦，不敢正撄其锋，急忙斜身闪开,却也大是狼狈。\n");
	target->set_temp("apply/action/dodge", CYN"\n$n大声叫道：“来的好！”知这一招凌厉绝伦，不敢正撄其锋，急忙斜身闪开,却也大是狼狈。\n");		
	
	me->receive_damage("neili", 500+lvl);
	me->add_temp("apply/pfm_hurt",200+lvl/2);
	me->add_temp("apply/pfm_attack",150+lvl/10);
	COMBAT_D->do_attack(me, target,weapon,random(3)?0:3,2);
	me->add_temp("apply/pfm_hurt",-200-lvl/2);
	me->add_temp("apply/pfm_attack",-150-lvl/10);	 
	
	me->start_busy(1);
	return 1;
}

int help(object me)
{
   write(WHT"\n华山剑法「"HIR"无双无对"WHT"」："NOR"\n");
   write(@HELP
    这「无双无对,宁氏一剑」绝技乃是华山宁女侠为克制田伯光的快刀
    而自创的一招绝技.此招讲究以快制快,乃冒被反噬的危险刺出威力巨
    大的一招,若能成功命中,则对手非死也残.乃是华山剑法中少有的凶
    狠招数.
    指令：perform wushuang

    要求：紫霞神功300级
          华山剑法300级，
          基本剑法300级，
          最大内力3000以上，
          当前内力1500以上，
          当前精力1000以上。

    说明：
          宁中则亲自传授后方可使用。

HELP
    );
    return 1;
}
