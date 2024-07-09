// sancai.c  奇门三才刀

#include <ansi.h>
inherit F_SSERVER;

int query_ap() {return 3;}

string perform_name(){ return CYN"奇门三才刀"NOR; }

int perform(object me, object target)
{
	object weapon;
	int lvl;
	
	if( !target ) target = offensive_target(me);

	if( !objectp(target) || !target->is_character() || !me->is_fighting(target) )
		return notify_fail("「奇门三才刀」只能对战斗中的对手使用。\n");

	if (!objectp(weapon = me->query_temp("weapon"))
	|| (string)weapon->query("skill_type") != "blade")
		return notify_fail("你使用的武器不对。\n");

	if( (lvl=(int)me->query_skill("liuhe-daofa", 1)) < 100 )
		return notify_fail("你的六合刀法不够娴熟，无法使用奇门三才刀。\n");

	if ( me->query_skill_mapped("parry") != "liuhe-daofa"
	|| me->query_skill_mapped("blade") != "liuhe-daofa")
		return notify_fail("你现在无法使用奇门三才刀。\n");

	if( (int)me->query_skill("huntian-qigong", 1) < 80 )
	return notify_fail("你的混天气功不够高。\n");

	if((int)me->query("max_neili",1) < 1200 )
		return notify_fail("你的内力修为不够，无法使用「奇门三才刀」。\n");

	if( (int)me->query("neili", 1) < 800 )
	return notify_fail("你现在内力太弱，不能使用「奇门三才刀」。\n");

	me->set_temp("apply/action/pfm",  CYN "$N面色忽变，步法飘忽不定，手腕疾抖，千万个刀花如漫天飘雪，铺天盖地飞向$n");
	me->set_temp("apply/action/hit",HIR"\n只见$N足踏天地人奇门三才刀光聚为一线，$n只觉一股热流扑面而来，躲闪不及，连中数刀鲜血狂喷而出！\n"NOR);
	target->set_temp("apply/action/parry", CYN"\n可是$p早有防备，侧身一闪笑道：“你在六合刀法内暗藏奇门三才，难道就奈何的了我？\n"NOR);
	target->set_temp("apply/action/dodge", CYN"\n可是$p早有防备，侧身一闪笑道：“你在六合刀法内暗藏奇门三才，难道就奈何的了我？\n"NOR);
	
	me->receive_damage("neili", 200+lvl/2);
	
	me->add_temp("apply/damage",400+lvl);
	me->add_temp("apply/pfm_damage",250+lvl/4);
	me->add_temp("apply/pfm_attack",240+lvl/10);
	COMBAT_D->do_attack(me, target,weapon,random(2)?1:3);
	me->add_temp("apply/damage",-400-lvl);
	me->add_temp("apply/pfm_damage",-250-lvl/4);
	me->add_temp("apply/pfm_attack",-240-lvl/10);
	
	me->delete_temp("apply/action");
	target->delete_temp("apply/action");
	me->start_busy(random(2));
	return 1;
}

int help(object me)
{
        write(CYN"\n四象六合刀「奇门三才刀」："NOR"\n");
        write(@HELP
        要求：  最大内力  1200 以上；
                当前内力  800 以上；
                四象六合刀等级 100 以上；
                混天气功等级 80 以上；
HELP
        );
        return 1;
}
                                                                                                                                                    