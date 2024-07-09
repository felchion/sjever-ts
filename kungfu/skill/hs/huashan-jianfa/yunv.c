// shijiushi.c 玉女十九式
// Update by lsxk@hsbbs 调整剑宗和气宗使用效果。

#include <ansi.h>

inherit F_DBASE;
inherit F_SSERVER;
#define MAX_HITS    19
 
string perform_name() {return HIC"玉女十九式"NOR;}
int query_ap() {return 10;}

int perform(object me, object target)
{
	string weapon;
	int i,lvl;
	object wep=me->query_temp("weapon");

	if( !target ) target = offensive_target(me);

	if( !objectp(target)
	||      !target->is_character()
	||      !me->is_fighting(target) )
	return notify_fail("玉女十九式只能对战斗中的对手使用。\n");

	if (me->query_skill_mapped("force") != "zixia-gong"
	&&me->query_skill_mapped("force") != "huashan-qigong")
	return notify_fail("你的内功有误，无法使用「玉女十九式」。\n");

	if( me->query_dex() < 60 )
	return notify_fail("你在身法上先天禀赋不足，不能使用玉女十九式！\n");

	if( me->query_skill("zixia-gong", 1) < 300
	&& me->query_skill("huashan-qigong", 1) < 300)
	return notify_fail("你的特殊内功火候未到，无法施展玉女十九式！\n");

	if( (lvl = (int)me->query_skill("huashan-jianfa",1)) < 300 )
	return notify_fail("你的华山剑法火候不足，无法使用精妙绝伦的玉女十九式！\n");

	if( me->query_skill("dodge") < 300 )
	return notify_fail("玉女十九式需要超绝的轻功配合，方能有效施展！\n");

	if( me->query_skill("sword") < 300 )
	return notify_fail("你剑法未到炉火纯青之境，还不会使用玉女十九式！\n");

	
	if( (int)me->query("neili") < 1500 )
	return notify_fail("你的内力不够！\n");

	if( me->query_temp("hsj_19"))
	return notify_fail("你正在运用玉女十九式！\n");

	if(!wep || wep->query("skill_type") != "sword"
	|| me->query_skill_mapped("sword") != "huashan-jianfa")
	return notify_fail("你手中无剑怎能使用玉女十九式？！\n");

	weapon = wep->query("name");

	message_vision(HIC "\n$N运剑如风，精妙非常的将「玉女剑十九式」逐式施展开来。" + weapon + HIC"如银龙出海,幻起道道电光！\n" +
	"顷刻之间，$N将「玉女剑十九式」中那数百招一气呵成地使了出来，这数百招便如一招，剑法之快，直是匪夷所思。\n" NOR, me, target);
	me->set_temp("hsj_19",1);

	me->add("neili", -1500);
	me->add_temp("apply/damage", 300+lvl/3);
  me->add_temp("apply/pfm_damage", 150+lvl/5);
  me->add_temp("apply/pfm_attack", 120+lvl/10);
  
	for( i = 0; i < MAX_HITS; i ++ ) {
    if(!target) target = offensive_target(me);
    if(objectp(target) && me->is_fighting(target))
      COMBAT_D->do_attack(me, target, wep, random(3)?0:3);
	}

	me->add_temp("apply/damage", -300-lvl/3);
  me->add_temp("apply/pfm_damage", -150-lvl/5);
  me->add_temp("apply/pfm_attack", -120-lvl/10);
  
	me->delete_temp("hsj_19");

	return 1;
}

int help(object me)
{
   write(WHT"\n华山剑法「"HIC"玉女十九式"WHT"」："NOR"\n");
   write(@HELP

    要求：紫霞神功 300级
          后天身法大于60，
          华山剑法300级，
          基本剑法300级，
          基本轻功300级，
          当前内力1500以上，
HELP
   );
  return 1;
}
