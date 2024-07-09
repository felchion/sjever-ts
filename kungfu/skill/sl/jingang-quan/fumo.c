// fumo.c 大金刚拳 perform 金刚伏魔

#include <ansi.h>
#include <combat.h>

inherit F_SSERVER;
int query_ap() {return 3;}

void check_fight(object me,int lv,int count);

int perform(object me, object target)
{
	string msg;
	int lv,i;
	if( !target ) target = offensive_target(me);

	if( !target || !me->is_fighting(target) )
	return notify_fail("「金刚伏魔」只能在战斗中对对手使用。\n");

	if( objectp(me->query_temp("weapon")) )
	return notify_fail("你必须空手使用「金刚伏魔」！\n");

	if( lv=(int)me->query_skill("jingang-quan",1) < 120 )
	return notify_fail("你的大金刚拳不够娴熟，不会使用「金刚伏魔」！\n");

	if( (int)me->query_skill("yijin-jing",1) < 120 )
	return notify_fail("你的易筋经内功等级不够，不能使用「金刚伏魔」！\n");

	if( (int)me->query_str() < 28 )
	return notify_fail("你的臂力不够强，不能使用「金刚伏魔」！\n");

	if( (int)me->query("max_neili") < 1500 )
	return notify_fail("你的内力太弱，不能使用「金刚伏魔」！\n");

	if( (int)me->query("neili") < 1000 )
	return notify_fail("你的内力太少了，无法使用出「金刚伏魔」！\n");

	if (me->query_skill_prepared("cuff") != "jingang-quan"
	|| me->query_skill_mapped("cuff") != "jingang-quan")
	return notify_fail("你现在无法使用「金刚伏魔」进行攻击。\n");

	if( me->query_temp("fumo"))
	return notify_fail("你正在使用大金刚拳的特殊攻击「金刚伏魔」！\n");

	msg = HIR"$N突然全身骨骼作响，口中默念「金刚伏魔」，运起" +
	"易筋功，脸上一阵红色，全身好象有使不完的力量一样！\n" NOR;

	message_vision(msg, me,target);
		
	me->add("neili", -500 -lv/2);
	me->set_temp("fumo",1);
	me->add_temp("apply/damage",300+lv/2);
	me->add_temp("apply/pfm_damage",150+lv/5);
	me->add_temp("apply/pfm_attack",120+lv/10);
	me->add_temp("apply/action/color",HIW);


	for(i = 0; i < 3; i++)
	  if (objectp(target) && me->is_fighting(target))
	    COMBAT_D->do_attack(me, target, 0, random(3)?0:3);

	call_out("check_fight", 1, me, lv,10);
	return 1;
}

void check_fight(object me,int lv,int count)
{
	if (!me) return;
	if (me->query_temp("weapon")
	|| me->query_temp("secondary_weapon")
	|| me->query_skill_prepared("cuff") !="jingang-quan"){
		me->delete_temp("fumo");
		me->delete_temp("apply/action/color");
		me->add_temp("apply/damage",-300-lv/2);
		me->add_temp("apply/pfm_damage",-150-lv/5);
		me->add_temp("apply/pfm_attack",-120-lv/10);
		tell_object(me, HIR"你的「金刚伏魔」运功完毕，你的脸色好看多了。\n"NOR);
		return;
	}
	else if (count < 1 || !me->is_fighting()) {
		me->delete_temp("apply/action/color");
		me->add_temp("apply/damage",-300-lv);
		me->add_temp("apply/pfm_damage",-150-lv/5);
		me->add_temp("apply/pfm_attack",-120-lv/10);
		me->delete_temp("fumo");
		tell_object(me, HIR"你的「金刚伏魔」运功完毕，你的脸色好看多了。\n"NOR);
		return;
	}
	else {
		//        me->start_perform(1,"「金刚伏魔」");
		call_out("check_fight", 1, me,lv, count-1);
	}
}

int help(object me)
{
        write(HIY"\n「金刚伏魔」："NOR"\n");
        write(@HELP        

        要求：  最大内力 1500 以上；
                当前内力 1000 以上；
                后天臂力 28 以上;
                大金刚拳等级 120 以上；
                易筋经等级 120 以上；
                激发大金刚拳为拳法；
                
HELP
        );
}   