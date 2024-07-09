// By Spiderii修改 shenzhao.c

#include <ansi.h>
void remove_effect(object me,int skill);
int query_ap() {return 2;}
int exert(object me)
{
	int skill;
	skill = me->query_skill("shenzhao-jing");
	if ( !me->is_fighting())
	return notify_fail("你只能在战斗中使用「神照」！\n");

	if( (int)me->query_skill("shenzhao-jing", 1) < 120 )
	return notify_fail("你的神照经修为不够，无法使用「神照」！\n");

	if (me->query_skill_mapped("force") != "shenzhao-jing")
	return notify_fail("其它内功有「神照」吗？\n");

	if( (int)me->query("neili") < 1500 )
	return notify_fail("你目前的内力太少了，无法使用「神照」！\n");

	if( (int)me->query("max_neili") < 2000 )
	return notify_fail("你的内力修为这么差，还想使用「神照」？\n");

	if( me->query_temp("sjz_power"))
	return notify_fail("你正在运用「神照」！\n");

	message_vision(HIW"\n$N深吸一口气，内息缓缓而动，劲力随着增加，顿时感觉精神力气勃然而兴，沛然而至！\n\n" NOR, me);
	me->add_temp("apply/pfm_damage", 50);
	me->add_temp("apply/pfm_dodge", 150);
	me->add_temp("apply/pfm_attack", 150);
	me->add("neili", -400);
	me->set_temp("sjz_power",1);
	call_out("remove_effect", 1, me, skill);
	return 1;
}

void remove_effect(object me, int skill)
{

	if (objectp(me)) {
		if (me->is_fighting()) {
			call_out("remove_effect", 1, me, skill);
			return;
		}

		me->delete_temp("sjz_power");

			me->add_temp("apply/pfm_damage", -50);
		me->add_temp("apply/pfm_dodge", -150);
		me->add_temp("apply/pfm_attack", -150);
		message_vision(HIR"\n$N缓缓地舒了一口气，散去了护体真气。\n"NOR, me);
		return;

	}
}


int help(object me)
{
        write(WHT"\n神照经「神照」："NOR"\n");
        write(@HELP
        这是铁骨墨萼梅念笙老先生曾经教丁典的一套内功的秘经
        丁典习得此内功，后又传授狄云，狄云苦心多年，神照经
        已习得第二境界。巧遇机缘得到狄云指点，可更上一层楼
        
        要求:   内力 1500 以上；
        效果:   效果一增加攻击。
                效果二增加躲避。
        
HELP
        );
        return 1;
}

