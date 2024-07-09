// huntian.c 混天气功加力
// Change Powerup to huntian by Looklove 2001.5.3

#include <ansi.h>
inherit F_CLEAN_UP;

int query_ap() {return 2;}

string exert_name(){ return HIR"混天"NOR; }

int exert(object me, object target)
{
	int skill;

	if( !me->is_fighting() )
		return notify_fail("混天气功只能在战斗中使用。\n");
		
	if( me->query("neili") < 500 )
		return notify_fail("你的内力不够。\n");

	if( me->query_skill("huntian-qigong", 1) < 100 )
		return notify_fail("你的混天气功等级不够。\n");

	if( me->query_skill_mapped("force") != "huntian-qigong" )
		return notify_fail("你的内功不对。\n");

	if( me->query_temp("htqg/ht") )
		return notify_fail("你已经在运功中了。\n");

	skill = me->query_skill("force")/10;
	if( skill > 60 ) skill = 60;

	me->receive_damage("neili", -500);
	message_vision(HIR "$N微一凝神，运起「混天」神功，全身骨节发出一阵爆豆般的声响。\n" NOR, me);

	me->add_temp("apply/eq_armor", 120);
	me->add_temp("apply/pfm_parry", 150);
	me->set_temp("htqg/ht", 1);

	call_out("remove_effect", 1, me, skill);
	me->start_exert(1, "混天气功");
	if( me->is_fighting() )
		me->start_busy(random(2));
	return 1;
}

void remove_effect(object me, int count)
{
	if( objectp(me) && me->query_temp("htqg/ht") ) {
		if( count > 0 && me->is_fighting()) {
			call_out("remove_effect", 1, me, count--);
			return;
		}
		me->add_temp("apply/eq_armor", - 120);
		me->add_temp("apply/pfm_parry", - 150);
		me->delete_temp("htqg/ht");
		tell_object(me, HIR"你的混天气功运行完毕，将内力收回丹田。\n"NOR);
	}
}

int help(object me)
{
        write(HIR"\n混天气功之「混天」："NOR"\n");
        write(@HELP
        混天为混天气功重要技能之一。通过运功可以将自身防御加倍提升。
        要求：
        	混天气功等级　100 以上；
                当前内力  500 以上。
HELP
        );
        return 1;
}