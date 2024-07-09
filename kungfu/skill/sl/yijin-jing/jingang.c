// snowman 04/01/99
// Modified by snowman@SJ 28/09/2000
// Modified by olives@SJ 5/2/2001
// for exert jingang 的百毒不侵的bug
// By Spiderii增强效果，毕竟书里很NB的force
// By Server Fix 连续 Exert bug
#include <ansi.h>
int query_ap() {return 2;}
void remove_effect(object me,int count);
string exert_name(){ return YEL"金刚护体"NOR; }
int exert(object me)
{
	if ( !me->is_fighting())
	return notify_fail("你只能在战斗中使用「金刚护体」！\n");

	if( (int)me->query_skill("yijin-jing", 1) < 200 )
	return notify_fail("你的易筋经内功修为不够，无法使用「金刚护体」！\n");

	if (me->query_skill_mapped("force") != "yijin-jing")
	return notify_fail("你当前的内功无法使用「金刚护体」！\n");

	if( (int)me->query("neili") < 1000 )
	return notify_fail("你目前的内力太少了，无法使用「金刚护体」！\n");

	if( (int)me->query("max_neili") < 3000 )
	return notify_fail("你的内力修为这么差，还想使用「金刚护体」？\n");

	if( me->query_temp("jingang"))
	return notify_fail("你正在运用护体神功！\n");

	message_vision(YEL"\n$N突然深吸一口气，目露慈光，面带微笑，内力运转，竟使全身上下宝光流动，正是「金刚护体」！\n\n" NOR, me);
	me->set_temp("jingang", me->query_skill("yijin-jing", 1)/2);
	me->set_temp("no_skill_hit_ob", 1);
	me->add_temp("apply/eq_armor", me->query_skill("yijin-jing", 1)/4);
	me->add_temp("apply/pfm_parry", me->query_skill("yijin-jing", 1)/4);
	me->add("neili", -500);
	call_out("remove_effect", 1, me, me->query_skill("yijin-jing", 1)/2+20);
	return 1;
}

void remove_effect(object me,int count)
{
	if( !me) return;
	if(! me->is_fighting()
	|| count < 0 ){
		me->delete_temp("jingang");
		me->delete_temp("no_skill_hit_ob");
		me->add_temp("apply/pfm_parry", -me->query_skill("yijin-jing", 1)/4);
		me->add_temp("apply/eq_armor", -me->query_skill("yijin-jing", 1)/4);
		message_vision(HIR"\n$N双手合十，散去了凝聚于全身上下的「金刚护体」。\n"NOR, me);
		return;
	}
	else {
		call_out("remove_effect", 1, me,count -1);
	}
}

int help(object me)
{
        write(HIY"\n易筋经之「金刚护体」："NOR"\n");
        write(@HELP
        金刚，坚利之意，历百劫千生，流转六道，而觉性不坏。即以金刚之坚，喻
        般若体；以金刚之利，喻般若用。金刚护体为少林第一大绝技，以易
        筋经为本，使神、体、气三者相融，循序渐进，持之以恒，便能抵御心魔，
        抗击外力。
        
        要求：  易筋经等级 200 以上；
                最大内力 3000 以上；
                内力 1000 以上。
HELP
        );
        return 1;
}
