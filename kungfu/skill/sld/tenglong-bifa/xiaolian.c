// guifei 小怜横陈
// created by olives@SJ
// Update By lsxk@hsbbs 2007/6/6
/*
     蓦见夫人脑袋向着她自己胸口钻落，敌人架颈中的一刀自然落空，她顺势在
地下一个筋斗在教主胯下钻过，握着匕首的右手成拳，轻轻一拳击在教主后心，
只是剑尖向上。倘若当真对敌，这一剑自然插入了敌人背心。韦小宝又大叫一声
：“好！”

*/
#include <ansi.h>
#include <combat.h>
inherit F_SSERVER;
#include <combat_msg.h>

string perform_name() {return HIG"小怜横陈"NOR;}
int query_ap() {return 3;}


int perform(object me, object target)
{
	object weapon;
	string msg;
	int lv;

	if(!target) target = offensive_target(me);

	if( !objectp(target)
	|| !target->is_character()
	|| !me->is_fighting(target) )
	return notify_fail("「小怜横陈」只能在战斗时使用！\n");

	if(me->query("family/master_id")!="hong antong")
	return notify_fail("「小怜横陈」只有拜了教主后才能用！\n");

	if(!objectp(weapon=me->query_temp("weapon"))
	|| weapon->query("skill_type") != "dagger")
	return notify_fail("你手上没有匕首，如何使用「小怜横陈」。\n");

	if((int)(me->query("dex")+me->query_skill("dodge",1)/10) < 30 )
	return notify_fail("你身法不够，使用「小怜横陈」，恐怕会伤到自己！\n");

	if((lv=me->query_skill("tenglong-bifa", 1)) < 250 )
	return notify_fail("你的腾龙匕法还不够熟练，不能使用「小怜横陈」！\n");

	if((int)me->query_skill("dagger",1) < 250)
	return notify_fail("你的基本匕法不够熟练，不能使用「小怜横陈」！\n");
	/*
	if(me->query_skill_mapped("strike") != "huagu-mianzhang"
	|| me->query_skill_prepared("strike") != "huagu-mianzhang")
	return notify_fail("你现在没有做好准备，无法使用「小怜横陈」！\n");
	*/

	if(me->query_skill_mapped("dagger") != "tenglong-bifa")
	return notify_fail("只有使用腾龙匕法时才能使用「小怜横陈」！\n");

	if( (int)me->query_skill("dulong-dafa", 1) < 250 )
	return notify_fail("你的毒龙大法功力不足，不能用「小怜横陈」。\n");

	if( (int)me->query_skill("force", 1) < 250 )
	return notify_fail("你的基本内功功力不足，不能用「小怜横陈」。\n");

	if((int)me->query("max_neili", 1) < 4000)
	return notify_fail("你现的内力修为不足，不能使用「小怜横陈」！\n");

	if((int)me->query("eff_jingli", 1) < 2500)
	return notify_fail("你现的精力修为不足，不能使用「小怜横陈」！\n");

	if((int)me->query("jingli", 1) < 800)
	return notify_fail("你现在精力不足，不能使用「小怜横陈」！\n");

	if((int)me->query("neili", 1) < 1500)
	return notify_fail("你现在内力不足，不能使用「小怜横陈」！\n");


	if( me->query_skill_mapped("force") != "dulong-dafa")
	return notify_fail("你现在所使用的内功同毒龙大法相互抵触，不能使用「小怜横陈」。\n");


	msg = HIR"$N趁$n稍稍走神之机，拼尽全力使出苏夫人不传之绝技－－「小怜横陈」"NOR;
	msg +=MAG "\n$N使出一招「小怜横陈」，脑袋蓦地向自己胸口钻落，顺势在地上一个斛斗，\n从$n胯下钻过，右手握着兵器向$n后心连刺数下。\n"NOR;
	lv = (int)me->query_skill("tenglong-bifa",1);

	message_vision(msg,me,target);
	me->add_temp("apply/pfm_attack", 120+lv/10);
	me->add_temp("apply/damage", 350+lv/2);
	me->add_temp("apply/pfm_damage", 120+lv/5);

	COMBAT_D->do_attack(me, target, weapon, 1);

	if(objectp(target) && me->is_fighting(target))
	COMBAT_D->do_attack(me, target, weapon, 1);

	if(objectp(target) && me->is_fighting(target))
	COMBAT_D->do_attack(me, target, weapon, 1);

	if(objectp(target) && me->is_fighting(target) && lv > 300)
	COMBAT_D->do_attack(me, target, weapon, 1);

	if(objectp(target) && me->is_fighting(target) && lv > 500)
	COMBAT_D->do_attack(me, target, weapon,3);

	me->add_temp("apply/pfm_attack", -120-lv/10);
	me->add_temp("apply/damage", -350-lv/2);
	me->add_temp("apply/pfm_damage", -120-lv/5);
	me->start_busy(1);
	
	return 1;
}

int help(object me)
{
   write(WHT"\n腾龙匕法「"HIG"小怜横陈"WHT"」："NOR"\n");
   write(@HELP

    要求：最大内力 4000 以上；
          最大精力 2500 以上；
          当前内力 1500 以上；
          当前精力 800 以上；
          腾龙匕法等级 250 以上；
          基本匕法等级 250 以上；
          毒龙大法等级 250 以上；
          基本内功等级 250 以上；
          后天身法 30 以上；
HELP
   );
   return 1;
}
