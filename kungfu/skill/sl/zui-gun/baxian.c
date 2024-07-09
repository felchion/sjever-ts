// zuijiu.c 醉酒

#include <ansi.h>
#include <combat.h>

inherit F_SSERVER;

string perform_name() {return HIY"八仙醉打"NOR;}

int query_ap() {return 4;}

string  *msg = ({
HIY"「蓝采和，提篮劝酒醉朦胧」，$N手中$w"HIY"半提，脚下蹒跚，缓缓划向$n的$l",
HIY"$P顺势接着「何仙姑，拦腰敬酒醉仙步」，摆姿扭腰，右臂挟$w"HIY"顺势猛地扫向$n的$l",
HIY"然后一式「曹国舅，千杯不醉倒金盅」，往前一晃，倒竖$w"HIY"起，指天打地，向$n的$l劈去",
HIY"紧跟着一招「韩湘子，铁棍提胸醉拔萧」，$P摇头晃脑，横提$w"HIY"，棍端划了个半圈，击向$n的$l",
HIY"接着一招「汉钟离，跌步翻身醉盘龙」，$P手中棍花团团，脚步也摇摇摆摆，扫起疾风般向卷向$n",
HIY"顺势一试「铁拐李，踢倒金山醉玉池」，$P单腿支地，另一腿踢在$w"HIY"根部荡起，甩向$n的$l",
HIY"然后「张果老，醉酒抛杯倒骑驴」，$P扭身反背，摇摇欲坠，$w"HIY"从胯底钻出，戳向$n的胸口",
HIY"最后一式「吕洞宾，酒醉提壶力千钧」，$P醉眼朦胧，但却举重若轻，手中$w"HIY"挟千钧之力压向$n",
});

int perform(object me, object target)
{
	int drunk, limit, i, lvl;
	object weapon;

	if( !target ) target = offensive_target(me);

	if( !target || !me->is_fighting(target) || !living(target))
	return notify_fail("「八仙醉打」只能在战斗中对对手使用。\n");

	if (!objectp(weapon = me->query_temp("weapon"))
	|| weapon->query("skill_type") != "club"
	|| me->query_skill_mapped("club") != "zui-gun")
	return notify_fail("你现在无法使用「八仙醉打」。\n");


	if( (int)me->query_skill("zui-gun",1) < 100 )
	return notify_fail("你的醉棍不够娴熟，不会使用「八仙醉打」！\n");

	if( me->query_skill_mapped("force") != "yijin-jing" )
	return notify_fail("你现在所使的内功无法使出「八仙醉打」。\n");

	if( (int)me->query_skill("yijin-jing",1) < 100 )
	return notify_fail("你的易筋经等级不够，不能使用「八仙醉打」！\n");

	if( (int)me->query_str() < 25 )
	return notify_fail("你的臂力不够强，不能使用「八仙醉打」！\n");

	if( (int)me->query("max_neili") < 1500 )
	return notify_fail("你的内力太弱，不能使用「八仙醉打」！\n");

	drunk = me->query_condition("drunk");
	limit = (me->query("con") + (int)me->query("max_neili") / 50) * 2;
	lvl = (int)me->query_skill("zui-gun", 1);
	if( drunk <= limit/4 && lvl < 180)
	return notify_fail("你没有一点醉意，怎么使得出「八仙醉打」？\n");
	if( drunk >= limit && lvl < 240)
	return notify_fail("你现在是真正的烂醉如泥，酒鬼！\n");


	if( (int)me->query("neili") < 1000 )
	return notify_fail("你的内力太少了，无法使用出「八仙醉打」！\n");

	message_vision(HIY"\n$N突然一个醉嗝，顿时借着酒劲猛打，醉棍威力大增！\n"NOR,me);
	me->set_temp("zuijiu", 1);
	me->receive_damage("neili", 800);
	me->add_temp("apply/pfm_attack", 90+lvl/10);
	me->add_temp("apply/damage", 350+lvl/2);
	me->add_temp("apply/pfm_damage", 200+lvl/5);
	me->set_temp("apply/action/damage_type" , "砸伤");
	for(i=0; i < 8; i++){
		if(!target->is_fighting(me)) break;
		me->set_temp("apply/action/pfm",msg[i]);
		COMBAT_D->do_attack(me, target, me->query_temp("weapon"), random(4)?1:3);
	}
	me->add_temp("apply/pfm_attack", -90-lvl/10);
	me->add_temp("apply/damage", -350-lvl/2);
	me->add_temp("apply/pfm_damage", -200-lvl/5);
	
	
	me->delete_temp("zuijiu");
	me->delete_temp("apply/action");
	me->start_busy(random(2));
	return 1;
}

int help(object me)
{
   write(WHT"\n醉棍「"HIY"八仙醉打"WHT"」："NOR"\n");
   write(@HELP

    要求：最大内力 1500 以上；
          当前内力 1000 以上；
          醉棍等级 100 以上；
          基本棍法等级 100 以上；
          易筋经等级 100 以上；
          后天臂力 25 以上；
HELP
   );
   return 1;
}

