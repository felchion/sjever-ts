// shexin.c 摄心

#include <ansi.h>

inherit F_SSERVER;

int query_ap() {return 5;}

void affectob(object ,object ,int);
string perform_name(){ return HIW"摄心"NOR; }
int perform(object me, object target)
{
	int rst,skill;
	string msg;
	
	if(!me->is_fighting())
		return notify_fail("你没在战斗中呢！\n");

	if(!target) target = offensive_target(me);

	if( (skill=(int)me->query_skill("meinu-quanfa", 1)) < 150 )
		return notify_fail("你的美女拳法还不够纯熟，无法施展「摄心」绝技。\n");

	if(me->query_skill_mapped("parry") != "meinu-quanfa")
		return notify_fail("你没有使用美女拳法作为招架！\n");
	
	if ( me->query_temp("weapon"))
		return notify_fail("你手持武器，如何使用「摄心」绝技？\n");
	
	if(me->query_skill_prepared("cuff") != "meinu-quanfa")
		return notify_fail("你现在无法施展「摄心」绝技。\n");
	
	if(me->query("max_neili")<1200)
		return notify_fail("你内力不够，无法使用「摄心」绝技。\n");
	
	if(me->query("max_neili")<500)
		return notify_fail("你内力不够，无法使用「摄心」绝技。\n");
		
	if(target->query_temp("shexin"))
		return notify_fail("你正在使用「摄心」绝技。\n");
		
	rst = target->query_skill("buddhism",1) /20;
	rst = MAX(rst,target->query_skill("dacheng-fofa")/20);
	rst = MAX(rst,target->query_skill("taoism")/20);
	
	if (random(me->query_per()*3) > target->query_per()+rst) 	{		
		
		msg = HIY"\n$N忽然对$n轻轻一笑，眼光四处流动，所经过之处似乎充满了妩媚，$n不由得一呆。\n"NOR;       
		message_vision(msg, me, target);

		if ( COMBAT_D->do_attack(me, target,0, 0,1)
		||COMBAT_D->do_attack(me, target,0, 0,1)
		||COMBAT_D->do_attack(me, target,0, 0,1)){
			target->add_temp("apply/pfm_dodge", -240);
			target->add_temp("apply/pfm_parry", -240);
			target->add_temp("apply/pfm_attack", -240);
			if (skill > 450 && random(2))
				target->apply_condition("no_perform", 3);
			target->set_temp("shexin", 240);
			me->start_busy(1);
			me->add("jingli",-100);
			me->add("neili",-300);
			call_out("affectob",5, me, target,5);
		}
		else 
		{
			message_vision(CYN"\n但见$n大喝一声，想用雄浑的内力打破$N四周这妩媚的气氛！\n"+
					"结果$n只发出一招，就无法忍受$N妩媚的眼光了。\n"NOR, me, target);
			COMBAT_D->do_attack(target,me,0,1);
			me->add("neili",-100);
			target->add_temp("apply/pfm_dodge", -120);
			target->add_temp("apply/pfm_parry", -120);
			target->add_temp("apply/pfm_attack", -120);
			target->set_temp("shexin", -120);
			call_out("affectob",5, me, target, 5);
		}
	}	
	else {
		msg = HIY"\n$N对$n轻轻一笑，眼光之中流露出妩媚之意，想施展「摄心」绝技。\n"+
			HIW"不料$n定了定神，突然大喝一声，顿时把$N惊出一身冷汗。\n"NOR;
		message_vision(msg,me,target);
		me->start_busy(2);
	}     
	return 1;
}

void affectob(object me,object target,int now)
{
	if (!target) return;
	if( !me 
	|| now < 1
	|| objectp(me->query_temp("weapon")) 
	|| target->is_ghost() 
	|| me->is_ghost()  
	|| !me->is_fighting(target) 
	|| me->query_skill_prepared("cuff")!="meinu-quanfa")	{
		if(me) message_vision(HIC"\n$N内力突然一顿，眼光又恢复平静。\n"NOR, me);
		target->add_temp("apply/pfm_attack", target->query_temp("shexin"));
		target->add_temp("apply/pfm_dodge",  target->query_temp("shexin"));
		target->add_temp("apply/pfm_parry",  target->query_temp("shexin"));
		target->delete_temp("shexin");
		return;
	}
	call_out("affectob",5,me,target,now-1);
}

int help(object me)
{
        write(HIW"\n美女拳法之「摄心绝技」："NOR"\n\n");
        write(@HELP
        要求：  当前内力  500 以上；
                最大内力 1200 以上；
                美女拳法等级 150 以上；
                激发拳法为美女拳法；

HELP
        );
        return 1;
}   