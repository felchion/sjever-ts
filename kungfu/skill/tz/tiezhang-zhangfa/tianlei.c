// tianlei.c 铁掌--天雷气
// rewrite by snowman@SJ 19/12/2000

inherit F_SSERVER;

#include <ansi.h>
#include <combat_msg.h>
int query_ap() {return 3;}

string perform_name(){ return HBBLU"天雷气"NOR; }
int perform(object me, object target)
{
        if( !target ) target = offensive_target(me);

        if( !target
        || !target->is_character()
        || !me->is_fighting(target)
        || !living(target)
        || environment(target)!=environment(me) )
                return notify_fail("天雷气只能对战斗中的对手使用。\n");

        if( me->query_temp("weapon") )
                return notify_fail("你必须空手才能使用天雷气！\n");

        if((int)me->query_skill("guiyuan-tunafa", 1) < 150 )
                return notify_fail("你的天雷气还未练成，不能使用！\n");
                
        if((int)me->query("neili", 1) < 1000 )
                return notify_fail("你现在内力不足，不能使用天雷气！\n");       

        if (me->query_skill_prepared("strike") != "tiezhang-zhangfa"
        || me->query_skill_mapped("strike") != "tiezhang-zhangfa"
        || me->query_skill_mapped("parry") != "tiezhang-zhangfa")
                return notify_fail("你现在无法使用「天雷气」！\n");                                                                                 

        if( me->query_skill_mapped("force") != "guiyuan-tunafa")
                return notify_fail("你现在的内功用不了「天雷气」！\n");   
                
        if(me->query_temp("tzzf"))
                return notify_fail("你正在使用掌刀。\n");

        if((int)me->query_skill("tiezhang-zhangfa", 1) < 150 )
                return notify_fail("你的铁掌掌法不够娴熟，不能使用天雷气。\n");

        tell_object(me, HIW"\n你深吸了一口气，暗暗将师傅的秘传“天雷气”凝聚到双掌之中……\n"NOR);
        me->start_perform(3, "天雷气");
        me->start_call_out( (: call_other, __FILE__, "tianlei_hit", me, 1 :), 1+ random(4) );
        return 1;
}

int tianlei_hit(object me, int flag)
{
	object target;
	int lvl;
	string dodge_skill;

	if( !me || !living(me) ) return 0;

	if( !me->is_fighting() ){
		write("天雷气只能在战斗中使用。\n");
		return 0;
	}

	target = offensive_target(me);
	if( !target
	|| !target->is_character()
	|| !me->is_fighting(target)
	|| !living(target)
	|| environment(target) != environment(me) ){
		write("天雷气只能对战斗中的对手使用。\n");
		return 0;
	}

	if( me->query_temp("weapon")
	&& !me->query_temp("tzzf") ){
		write("你必须空手才能使用天雷气！\n");
		return 0;
	}
	if( (int)me->query("neili", 1) < 600 ){
		write("你现在内力不足，不能使用天雷气！\n");
		return 0;
	}
	if (me->query_skill_prepared("strike") != "tiezhang-zhangfa"
	|| me->query_skill_mapped("strike") != "tiezhang-zhangfa"
	|| me->query_skill_mapped("parry") != "tiezhang-zhangfa"){
		write("你现在无法使用「天雷气」！\n");
		return 0;
	}

	lvl = me->query_skill("tiezhang-zhangfa",1);

	dodge_skill = (flag == 1)?"天 …… 雷 …… 气":"天 …… 雷 …… 合 …… 一";
	me->set_temp("apply/action/pfm", HIY "\n$N忽然仰天大喝一声“"HIW + dodge_skill + HIY"”，一双手掌刹那间变得雪白！掌劲之下，一阵炙热的罡气随掌势狂飙而出，势不可挡");
	target->set_temp("apply/action/parry", HIY"$p在危急中避过了这致命一击，已吓得脸色苍白！\n" NOR);
	target->set_temp("apply/action/dodge", HIY"$p在危急中避过了这致命一击，已吓得脸色苍白！\n" NOR);


	me->receive_damage("neili", 500+lvl);
	me->add_temp("apply/force",600+lvl/2);
	me->add_temp("apply/pfm_force",150+lvl/5+(flag==1?50:0));
	me->add_temp("apply/pfm_attack",150+lvl/10+(flag==1?30:0));
	COMBAT_D->do_attack(me, target,0,random(2)?0:3);
	me->add_temp("apply/force",-600-lvl/2);
	me->add_temp("apply/pfm_force",-150-lvl/5-(flag==1?50:0));
	me->add_temp("apply/pfm_attack",-150-lvl/10-(flag==1?30:0));	      
	me->delete_temp("apply/action/");
	target->delete_temp("apply/action/");

	me->start_busy(random(2));
	return 1;
}
int help(object me)
{
        write(WHT"\n铁掌掌法「天雷气」："NOR"\n");
        write(@HELP
        这是裘千仞铁掌功的十三绝招之一，叫作「天雷气」，乃是聚数十年的内功
        罡气于掌，出招制敌。是江湖上有名的杀招之一！
        
        要求：  内力 600 以上；   
                铁掌掌法等级 150 以上；
                归元吐纳法等级 140 以上；    
                且手无兵器。
HELP
        );
        return 1;
}

