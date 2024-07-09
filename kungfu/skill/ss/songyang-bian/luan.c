// luan.c
// Created by Numa 2000.1.25

#include <ansi.h>
inherit F_SSERVER;

string perform_name() {return BLU"乱鞭诀"NOR;}
int check_fight(object me, object ob, int lvl, int i);
int query_ap() {return 3;}

int perform(object me, object target)
{
	object weapon;
	int i, lvl;

	if( !target ) target = offensive_target(me);

	if( !target
	||  !target->is_character()
	||  !me->is_fighting(target)
	|| !living(target))
	return notify_fail("乱鞭诀只能对战斗中的对手使用。\n");

	if( (int)me->query_skill("songyang-bian", 1) < 120 )
	return notify_fail("你的嵩阳鞭火候不够。\n");

	if( (int)me->query_skill("whip", 1) < 120 )
	return notify_fail("你的基本鞭法火候不够。\n");
	if( (int)me->query_skill("hanbing-zhenqi", 1) < 120 )
	return notify_fail("你的寒冰真气火候不够。\n");

	if( (int)me->query("max_neili") < 1500 )
	return notify_fail("你的内力修为不够。\n");
	if( (int)me->query("neili") < 1000 )
	return notify_fail("你的真气不够。\n");
	if (me->query_skill_mapped("force") != "hanbing-zhenqi")
	return notify_fail("你无法使用乱鞭诀。\n");
	if (!objectp(weapon = me->query_temp("weapon"))
	|| weapon->query("skill_type") != "whip"
	|| me->query_skill_mapped("whip") != "songyang-bian")
	return notify_fail("你无法使用乱鞭诀。\n");

	if (me->query_temp("ss_luan"))
	return notify_fail("你正在使用乱鞭诀。\n");

	me->add("neili", -lvl/2-500);
	message_vision(HIW "\n$N突然脚下步伐混乱，出招呆滞，招式破绽百出，全身各大要穴均暴露无疑！\n$n一见此机，心切之下，仓促连攻几招。哪知$N此为缓招，但见手中长鞭一个倒卷，乱鞭诀随之连绵而出，其势不可挡！\n" NOR,me, target);

	me->add_temp("apply/damage", 500 + lvl/2);
  me->add_temp("apply/pfm_damage", 120+lvl/5);
  me->add_temp("apply/pfm_attack", 120+lvl/5);
//  me->add_temp("apply/action/color",HIW);

	me->set_temp("ss_luan",1);
	i = me->query_skill("whip")/50;
	call_out("check_fight", 1, me, target, lvl, i);
	me->start_busy(random(2));
	return 1;
}

int check_fight(object me, object ob, int lvl, int i)
{
	object weapon;
  string msg;

	if (!me)
	return 0;
	if (me->is_ghost() || !living(me) || !me->query_temp("ss_luan") || i <1)
	{
		call_out("remove_effect",0,me);
		return 1;
	}
	if (!me->is_fighting() || me->query("neili") <= 100)
	{
		call_out("remove_effect",0,me,lvl);
		return 1;
	}
	if ( me->query_skill_mapped("force") != "hanbing-zhenqi"
	||  me->query_skill_mapped("whip") != "songyang-bian"
	||  !objectp(weapon = me->query_temp("weapon"))
	||  weapon->query("skill_type") != "whip" )
	{
		call_out("remove_effect",0,me,lvl);
		return 1;
	}
	if (!ob || ob->is_ghost())
	{
		call_out("remove_effect",0,me,lvl);
		return 1;
	}

	if (random(2))
	{
		switch(random(5))
		{
			case 0 : msg = HIY"\n$N见$n此招出势勉强，急将鞭梢一带，划向$n"NOR;
			break;
			case 1 : msg = HIR"\n$n此招既出，意欲收回已是不能，勉强攻出，$N早已将"+weapon->name()+HIR"抽向$n四处要穴"NOR;
			break;
			case 2 : msg = HIG"\n$N见自己身形已逐渐混乱，情急之下，身体凌空飞出，"+weapon->name()+HIG"的末梢却已扫向$n的玉堂穴"NOR;
			break;
			case 3 : msg = HIB"\n$N如此一来，已无退路，只有拼死一搏，念既至此，"+weapon->name()+HIB"攻势陡然凌厉，$n已无招架之力"NOR;
			break;
			case 4 : msg = HIW"\n$n见$N出招已然无力，心中暗暗叫好，身形一越而出，方觉上当，但团团鞭影已使$n无处遁形"NOR;
			break;
		}
		me->set_temp("apply/action/pfm",msg);
		COMBAT_D->do_attack(me, ob, weapon, random(3)?1:3);
		me->delete_temp("apply/action/pfm");
	}
	call_out("check_fight", 1, me, ob, lvl, i-1);
	return 1;
}

void remove_effect(object me,int lvl)
{
	if (!me) return;
	message_vision(CYN"\n$N一套乱鞭诀使罢，神情若定，立于场中。\n"NOR,me);
	me->add_temp("apply/damage", -500 - lvl/2);
  me->add_temp("apply/pfm_damage", -120-lvl/5);
  me->add_temp("apply/pfm_attack", -120-lvl/5);
  me->delete_temp("apply/action/color");
	me->delete_temp("ss_luan");
	return;
}

int help(object me)
{
        write(HIW"\n嵩阳鞭之「乱鞭诀」："NOR"\n\n");
        write(@HELP
        要求：  当前内力 1000 以上；
                最大内力 1500 以上；
                嵩阳鞭等级 120 以上；
                寒冰真气等级 120 以上；
                基本鞭法等级 120 以上；
                激发鞭法为嵩阳鞭；
                激发内功为寒冰真气；
                且手持兵器。

HELP
        );
        return 1;
}
