//Update By lsxk@hsbbs 2007/6/11

#include <ansi.h>
inherit F_SSERVER;
int check_fight(object me, object target);
string perform_name(){ return HIW"化字诀"NOR; }

int perform(object me, object target)
{
	int ap , dp;

	if( !target) target = offensive_target(me);

	if(!objectp(target) || !me->is_fighting(target))
		return notify_fail("你没在和对方战斗呢！\n");

	if( me->query_skill("qiankun-danuoyi", 1) < 210 )
		return notify_fail("你的乾坤大挪移还未练到第七层。\n");

	if (!me->query_skill("jiuyang-shengong", 1))
	if( me->query_skill("shenghuo-shengong", 1) < 210 )
		return notify_fail("你的圣火神功等级不够。\n");

	if( me->query_skill_mapped("parry") != "qiankun-danuoyi")
		return notify_fail("你没有使用乾坤大挪移作为招架！\n");

	if( me->query("neili") > me->query("max_neili"))
		return notify_fail("你目前的内力太强，无法使用化字诀！\n");

	if( me->query("neili") < 200 )
		return notify_fail("你的内力不够，无法使用化字诀！\n");

	if( me->query("jiali") >= 10)
		return notify_fail("你目前的加力太大，无法使用化字诀！\n");

	if( me->query_temp("qkdny/hua") )
		return notify_fail("你正在施展「化」字诀呢！\n");

	if( me->query_temp("qkdny/yi") )
		return notify_fail("你正在施展「移」字诀呢！\n");

	if( me->query_temp("qkdny/xu") )
		return notify_fail("你正在施展「蓄」字诀呢！\n");

	if( me->query_temp("qkdny/yin") )
		return notify_fail("你正在施展「引」字诀呢！\n");

	me->add("neili", -100-random(100));
	me->start_perform(2, "「化」字诀");
        ap = me->query("combat_exp", 1) * me->query_int(1)/200;//防止数据溢出
        dp = target->query("combat_exp", 1) * target->query_int(1)/200;//防止数据溢出

	tell_object(me,HIW"\n你深深的吸了口气，口中默念「化」字诀，招数变的轻柔缓慢。\n"NOR);

	me->set_temp("qkdny/hua", 1);

	if ( random(ap + dp) > dp ) {
		tell_object(me, "你突然轻柔缓慢的出招，让"+target->name(1)+"一下子觉得很难适应。\n"NOR);
		tell_object(target, me->name(1)+"突然轻柔缓慢的出招，让你一下子觉得很难适应。\n"NOR);
		target->add_condition("no_perform",random(3)+1);
            me->add_condition("no_perform",random(2)+1);
	}
	check_fight(me, target); 
        return 1;
}

int check_fight(object me, object target)
{
	if (!me || !me->query_temp("qkdny/hua")) return 0;

	if(me->is_ghost() || !living(me))
		me->delete_temp("qkdny/hua");

	if(!me->is_fighting()
	 || me->query("jiali") > 9
	 || me->query("jingli") < 500
	 || me->query("jing") < 500 
	 || !target ){
		tell_object(me, HIW"\n慢慢你的招数又恢复了平常。\n"NOR);
		me->delete_temp("qkdny/hua");
	}
	else call_out("check_fight", 1, me, target);
	return 1;
}

int help(object me)
{
        write(HIW"\n乾坤大挪移「化字诀」："NOR"\n");
        write(@HELP
        要求：  当前内力 200 以上；
                加力 10 以下；
                乾坤大挪移等级 210 以上；
                当前内力不大于最大内力；
                激发招架为乾坤大挪移。
HELP
        );
        return 1;
}
