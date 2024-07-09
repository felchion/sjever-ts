#include <ansi.h>
#include <skill.h>
#include <combat.h>

inherit F_SSERVER;

int perform(object me)
{
	object weapon;
	string msg;
	int i;

	weapon = me->query_temp("weapon");

	if(!me->is_fighting() )
	return notify_fail("绝剑只能在对战斗中使用。\n");

	if( !objectp(weapon) || weapon->query("skill_type") != "sword" )
	return notify_fail("你手中无剑，怎能运用「绝剑」心法？！\n");

	if( me->query_skill("linji-zhuang",1) < 140 )
	return notify_fail("你的临济十二庄不够火候，使不出绝剑。\n");

	if( me->query_skill_mapped("sword") != "huifeng-jian")
	return notify_fail("你现在不能使用「绝剑」心法。\n");

	if ( me->query_skill_mapped("parry") != "huifeng-jian")
	return notify_fail("你现在不能使用「绝剑」心法。\n");

	if( me->query_skill("huifeng-jian",1) < 140 )
	return notify_fail("你的回风拂柳剑不够火候，使不出绝剑。\n");

	if( me->query_temp("huifeng/jue"))
	return notify_fail("你已经在使用了。\n");

	if(me->query_temp("emei/parry"))
	return notify_fail("你正在使用诸天化身步。\n");

	if( me->query("jingli") < 500 )
	return notify_fail("你现在精力不够。\n");

	if( me->query("neili") < 500 )
	return notify_fail("你现在内力不够。\n");

	msg = HIW"$N深吸一口气，运起峨嵋临济十二庄使出「绝剑」";
	me->add("neili", -200+random(100));
	me->add("jingli", -100);
	me->start_busy(random(2));
	i = me->query_skill("linji-zhuang",1) + me->query_skill("huifeng-jian", 1);
	i /= 2;

	me->add_temp("apply/pfm_parry", 120+i/10);
	me->add_temp("apply/pfm_dodge", 120+i/10);
	me->set_temp("huifeng/jue", 1);
	// 灭、绝互相配合
	if (me->query_temp("huifeng/mie")) {
		msg += "，灭剑、绝剑互相配合，威力大增！\n"NOR;
		me->add_temp("apply/damage",280+i/2);
		me->add_temp("apply/pfm_damage",100+i/5);
		me->add_temp("apply/pfm_attack",100+i/10);
		me->set_temp("huifeng/miejue",1);
	}
	else msg +="！\n"NOR;
	message_vision(msg, me);
	call_out("remove_effect", (int)me->query_skill("linji-zhuang",1)/4, me, i);
	return 1;
}

void remove_effect(object me, int i)
{
	if (!objectp(me)) return;
	if( !me->query_temp("huifeng/jue")) return;
	me->delete_temp("huifeng/jue");
	me->add_temp("apply/pfm_parry", -120-i/10);
	me->add_temp("apply/pfm_dodge", -120-i/10);
	if (me->query_temp("huifeng/miejue")){
		me->add_temp("apply/damage",-280-i/2);
		me->add_temp("apply/pfm_damage",-100-i/5);
		me->add_temp("apply/pfm_attack",-100-i/10);
		me->delete_temp("huifeng/miejue");
	}
	message_vision(HIW"$N「绝剑」运使完毕，恢复了平常的攻击。\n"NOR, me);
}


string perform_name(){ return HIW"绝剑心法"NOR; }

int help(object me)
{
        write(HIW"\n回风拂柳剑之「绝剑心法」："NOR"\n\n");
        write(@HELP
        要求：  当前内力 500 以上；
                当前精力 500 以上；
                回风拂柳剑等级 150 以上；
                临济十二庄等级 150 以上；
                激发内功为临济十二庄；
                激发剑法为回风拂柳剑；
                激发招架为回风拂柳剑；
                手持兵器。

HELP
        );
        return 1;
}
