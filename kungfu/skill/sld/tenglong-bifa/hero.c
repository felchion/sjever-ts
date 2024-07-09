// hero 英雄三招
// 19991002001
// Update By lsxk@hsbbs 2007/6/5

#include <ansi.h>

inherit F_SSERVER;

string perform_name() {return HIR"英雄三招"NOR;}
int query_ap() {return 16;}

int lvl;
string msg;
object weapon;

int perform2(object me, object target);
int perform3(object me, object target);

int perform(object me, object target)
{
	
	weapon=me->query_temp("weapon");
	
	if( !target ) target = offensive_target(me);

	if( !objectp(target)
	 || !me->is_fighting(target)
	 || !living(target) )
		return notify_fail("「英雄三招」只能在战斗时使用！\n");

	if(me->query("family/master_id")!="hong antong")
		return notify_fail("「英雄三招」只有拜了教主后才能用！\n");

	if(!weapon
	|| weapon->query("skill_type") != "dagger"
	|| me->query_skill_mapped("dagger") != "tenglong-bifa")
		return notify_fail("你必须拿着匕首才能用「英雄三招」！\n");
	
	
	if( (int)me->query_skill("huagu-mianzhang", 1) < 250 )
  	return notify_fail("你的化骨绵掌还不够熟练，不能使用「英雄三招」！\n");

	if (me->query_skill_mapped("strike") != "huagu-mianzhang")
	  return notify_fail("只有使用化骨绵掌时才能使用「英雄三招」！\n");

	if( (int)me->query_skill("shenlong-tuifa", 1) < 250 )
	  return notify_fail("你的神龙腿法还不够熟练，不能使用「英雄三招」！\n");

	if (me->query_skill_mapped("leg") != "shenlong-tuifa")
  	return notify_fail("只有使用神龙腿法时才能使用「英雄三招」！\n");

	if( (int)me->query_skill("dulong-dafa", 1) < 250 )
		return notify_fail("你的毒龙大法功力不足，不能用「英雄三招」。\n");

	if( (lvl = (int)me->query_skill("tenglong-bifa", 1)) < 250 )
		return notify_fail("你的腾龙匕法还不够熟练，不能使用「英雄三招」！\n");

	if (me->query_skill_mapped("dagger") != "tenglong-bifa")
		return notify_fail("只有使用腾龙匕法时才能使用「英雄三招」！\n");

	if( (int)me->query_skill("dagger", 1) < 250 )
		return notify_fail("你的基本匕法还不够熟练，不能使用「英雄三招」！\n");

	if((int)me->query("max_neili", 1) < 4000)
		return notify_fail("你现在内力修为尚低，不能使用「英雄三招」！\n");


	if((int)me->query("neili", 1) < 2000)
		return notify_fail("你现在内力不足，不能使用「英雄三招」！\n");

	if( me->query_skill_mapped("force") != "dulong-dafa")
		return notify_fail("你现在所使用的内功同毒龙大法向抵触，不能使用「英雄三招」。\n");

	weapon=me->query_temp("weapon");

	message_vision( HIR"$N乘$n稍稍走神之机，拼尽全力使出洪教主不传之绝技－－「英雄三招」\n"NOR,me,target);


	me->set_temp("apply/action/pfm", HIR "\n$N使出一招「"HIY"子胥举鼎"HIR"」，一手轻搔$n腋底极泉穴");
	me->set_temp("apply/action/hit", HIY"\n$N乘$n慌张之际，另一手同时拿向$n肘后小海穴，将$n摔向地上！\n"NOR);
	target->set_temp("apply/action/parry", HIW"但是$n看破了$N的企图，没有上当，急急忙忙向旁边一闪，躲了过去。\n"NOR);
	target->set_temp("apply/action/dodge", HIW"但是$n看破了$N的企图，没有上当，急急忙忙向旁边一闪，躲了过去。\n"NOR);

	me->receive_damage("neili", 500+lvl);
	me->add_temp("apply/pfm_hurt",800);
	me->add_temp("apply/pfm_attack",400);
	weapon->unequip();
	COMBAT_D->do_attack(me, target,0,random(3)?0:3,2);
	weapon->wield();
	me->add_temp("apply/pfm_hurt",-800);
	me->add_temp("apply/pfm_attack",-400);

	perform2(me, target);
	return 1;
}

int perform2(object me, object target)
{
	if(!target) target = offensive_target(me);


	if( !objectp(target)
	|| !me->is_fighting(target)
	|| !living(target) )
		return 0;

	if((int)me->query("jingli", 1) < 200)
		return 0;

	if((int)me->query("neili", 1) < 500)
		return 0;

	if(!weapon) return 0;
	
	msg =HIR "\n$N使出一招「"HIW"鲁达拔柳"HIR"」，缩腿假装向$n叩头，突然一个斛斗，似欲向$n胯下钻过，\n"+
	           "但只一作势，左手抓向$n右脚足踝，右手兵器虚点向$n小腹，一用力，把$n头下\n"+
	           "脚上，倒提起来";

	me->set_temp("apply/action/pfm", msg);
	me->set_temp("apply/action/hit", HIY"$N左手握住"+weapon->name()+HIY"反手直刺，正中$n后心！\n"NOR);
	target->set_temp("apply/action/parry", HIW"$n吃了一惊，但随即冷静下来，在空中猛一蹬腿踢向$N面门，$N只得松开了手。\n"NOR);
	target->set_temp("apply/action/dodge", HIW"$n吃了一惊，但随即冷静下来，在空中猛一蹬腿踢向$N面门，$N只得松开了手。\n"NOR);


	me->receive_damage("neili", 250+lvl/4);
	me->add_temp("apply/damage",1500);
	me->add_temp("apply/pfm_damage",400);
	me->add_temp("apply/pfm_attack",450);
	COMBAT_D->do_attack(me, target,weapon,random(3)?0:3);
	me->add_temp("apply/damage",-1500);
	me->add_temp("apply/pfm_damage",-400);
	me->add_temp("apply/pfm_attack",-450);
	me->delete_temp("apply/action/");
	target->delete_temp("apply/action/");
	perform3(me, target);
	return 1;
}

int perform3(object me, object target)
{

	if(!target) target = offensive_target(me);

	if(!target
	|| !target->is_character()
	|| !me->is_fighting(target) )
	return 0;

	if((int)me->query("jingli", 1) < 200)
	return 0;

	if((int)me->query("neili", 1) < 1000)
	return 0;

	if(!weapon) return 0;

	msg =HIR "\n$N使出一招「"HIC"狄青降龙"HIR"」，背脊后撞，十指向$n胸口虚抓，乘其避让之际，\n"+
	"突然一个倒翻斛斗，身子跃起，双腿一分，跨坐在$n肩头";

	me->set_temp("apply/action/pfm", msg);
	me->set_temp("apply/action/hit", HIY"$N双手拇指压住$n太阳穴，食指按眉，中指刺眼。\n"NOR);
	target->set_temp("apply/action/parry", HIW"$n被$N制住，但临危不乱，以内力护住要害，同时高高跃起，翻身以头撞地，把$N撞了出去！\n"NOR);
	target->set_temp("apply/action/dodge", HIW"$n被$N制住，但临危不乱，以内力护住要害，同时高高跃起，翻身以头撞地，把$N撞了出去！\n"NOR);

	me->receive_damage("neili", 250+lvl/4);
	me->add_temp("apply/damage",1500);
	me->add_temp("apply/pfm_damage",400);
	me->add_temp("apply/pfm_attack",500);
	weapon->unequip();
	COMBAT_D->do_attack(me, target,0,random(2)?0:3);
	weapon->wield();
	me->add_temp("apply/damage",-1500);
	me->add_temp("apply/pfm_damage",-400);
	me->add_temp("apply/pfm_attack",-500);
	me->delete_temp("apply/action/");
	target->delete_temp("apply/action/");

	me->start_busy(random(2));
	return 1;
}


int help(object me)
{
   write(WHT"\n腾龙匕法「"HIR"英雄三招"WHT"」："NOR"\n");
   write(@HELP

    要求：最大内力 4000 以上；
          当前内力 2000 以上；
          化骨绵掌等级 250 以上；
          神龙腿法等级 250 以上；
          毒龙大法等级 250 以上；
          腾龙匕法等级 250 以上；
          基本匕法等级 250 以上；
          性别需求 男性。
HELP
   );
   return 1;
}
