// huadu.c 化毒
// by snowman@SJ
// By Spiderii修改效果
#include <ansi.h>

inherit F_CLEAN_UP;
void remove_effect(object me,int skill);

int exert(object me)
{      
       int skill;
       skill = me->query_skill("huagong-dafa");
	if ( !me->is_fighting())
		return notify_fail("你只能在战斗中使用「化毒」！\n");

	if( (int)me->query_skill("huagong-dafa", 1) < 100 )
		return notify_fail("你的化功大法修为不够，无法使用「化毒」！\n");

	if (me->query_skill_mapped("force") != "huagong-dafa")
		return notify_fail("其它内功有「化毒」吗？\n");

       if( (int)me->query_skill("poison", 1) < 100 )
		return notify_fail("你体内的毒素不够，无法使用「化毒」！\n");

	if( (int)me->query("neili") < 500 )
		return notify_fail("你目前的内力太少了，无法使用「化毒」！\n");

	if( (int)me->query("max_neili") < 1000 )
		return notify_fail("你的内力修为这么差，还想使用「化毒」？\n");

	if( me->query_temp("hgdf_power"))
		return notify_fail("你正在运用「化毒」！\n");

	message_vision(BLU"$N闭目凝神，将体内蕴积的毒质慢慢化为真气，分散在四肢内。\n" NOR, me);
	me->start_busy(1);
       me->add_temp("apply/claw", me->query_skill("huagong-dafa", 1)/4);
       me->add_temp("apply/attack", me->query_skill("huagong-dafa", 1)/4);
       me->add_temp("apply/armor_vs_force", me->query_skill("poison", 1)/8);
	me->add("neili", -400);
	me->set_temp("hgdf_power",1);
	call_out("remove_effect", 1, me, skill);
	me->start_exert(2, "「化毒」");
	return 1;
}
void remove_effect(object me, int skill)
{
	
	 if (objectp(me)) {
                if (me->is_fighting()) {
                        call_out("remove_effect", 1, me, skill);
                        return;
                }

           me->delete_temp("hgdf_power");
	    
           me->add_temp("apply/claw", -me->query_skill("huagong-dafa", 1)/4);
           me->add_temp("apply/attack", -me->query_skill("huagong-dafa", 1)/4);
           me->add_temp("apply/armor_vs_force", -me->query_skill("poison", 1)/8);
	    message_vision(BLU"$N运功化毒完毕，将真气化回毒质蕴积体内。\n"NOR, me);

	    return;

	}
}
string exert_name(){ return BLU"化毒"NOR; }
int help(object me)
{
        write(BLU"\n化功大法「化毒」："NOR"\n");
        write(@HELP
        星宿派的功夫主要是在<毒>字上下功夫，深的阴损毒辣其中三味
        化功大法吸人内力，阴毒异常，江湖上人提起，无不谈虎色变。
        这化毒一式就是将平时积蓄于体内的剧毒以深厚内力催化于四肢
        乃是为了在与敌战斗中，起到辅助伤敌的作用！

        要求:   化功大法 100 级；
                毒技     100 级；
                当前内力 500 点；
                最大内力 1000 点。
        
HELP
        );
        return 1;
}

