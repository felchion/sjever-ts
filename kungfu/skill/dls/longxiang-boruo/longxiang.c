// longxiang.c
// modefied by emnil@sj 2/2/00

#include <ansi.h>

inherit F_CLEAN_UP;

int query_ap() {return 2;}

void remove_effect(object me, int amount, int count);

int exert(object me, object target)
{
        int skill;

	if ( !me->is_fighting())
		return notify_fail("你只有在战斗中，才能运真气增加攻击力。\n");

	if ( me->query_skill("longxiang-boruo", 1) < 140)
		return notify_fail("只有修炼到龙象般若功第七层之后，才能增强自己的攻击力。\n");

	if ( me->query("max_neili") < 2000)
		return notify_fail("你的内力修为太差，多多打坐吧。\n");

	if ( me->query("neili") < 1000 ) 
		return notify_fail("你的真气不够。\n"); 

	if ( me->query_temp("xs/longxiang"))
		return notify_fail("你已经在运真气增加攻击力。\n"); 

	skill = me->query_skill("longxiang-boruo",1) /8;

	if(wizardp(me)) tell_object(me,sprintf("add attack: %d\n",skill));

	message_vision(MAG"\n$N潜运真气，脸上连泛"+CHINESE_D->chinese_number(skill*2/5)+"次紫气后恢复如常，但神色间隐约透出一种自信！\n" NOR, me);

	me->add_temp("apply/pfm_damage", skill);
	me->set_temp("xs/longxiang", skill);
	me->add("neili", -250);

	call_out("remove_effect", 1, me, skill, skill*7);

	me->start_busy(random(2));

	return 1;
}

void remove_effect(object me, int amount, int count)
{
	if ( !me || !me->query_temp("xs/longxiang"))
		return;

	if ( count < 1 || !me->is_fighting()) {
		me->add_temp("apply/pfm_damage", -amount);
		me->delete_temp("xs/longxiang");
		message_vision(MAG"$N脸上紫气一闪，又恢复如常。\n" NOR, me);
		return;
	}
	call_out("remove_effect", 1, me, amount, count - 1);
}

string exert_name(){ return MAG"龙象"NOR; }

int help(object me)
{
        write(MAG"\n龙象般若功之「龙象」："NOR"\n\n");
        write(@HELP
        要求：  当前内力 1000 以上；
                最大内力 2000 以上；
                龙象般若功等级 140 以上。

HELP
        );
        return 1;
}
