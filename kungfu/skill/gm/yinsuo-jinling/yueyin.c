// yufeng.c 银索金铃--「乐音绝技」
// By River
// Modified by darken@SJ

#include <ansi.h>
string perform_name(){ return HIW"乐音绝技"NOR; }
inherit F_SSERVER;
int query_ap() {return 4;}
void check_fight(object me,object target,int count);
int perform(object me, object target)
{
	string msg ;
	object weapon,weapon1;
	int lv;


	if(!me->query("yueyin"))
		return notify_fail("你还没学会在鞭法中融入乐音！ \n");
		
	weapon = me->query_temp("weapon");

	if( !target )
		target = offensive_target(me);

	if( !objectp(target)
	|| !target->is_character()
	|| !me->is_fighting(target))
		return notify_fail("「乐音绝技」只能对战斗中的对手使用。\n");

	if( !weapon
	|| me->query_skill_mapped("whip") != "yinsuo-jinling"
	|| me->query_skill_mapped("parry") != "yinsuo-jinling"
	|| weapon->query("skill_type") != "whip")
		return notify_fail("你现在无法使用「乐音绝技」攻击对方。\n");

	if((int)me->query_skill("yunu-xinjing", 1)  < 150 )
		return notify_fail("你的玉女心经等级还不够，使不出「乐音绝技」。\n");

	if((int)me->query_skill("yinsuo-jinling",1) < 150 )
		return notify_fail("你的银索金铃还不够娴熟，不会使用「乐音绝技」。\n");

	if((int)me->query_skill("yunu-jianfa",1) < 150 )
		return notify_fail("你的玉女素心剑还不够娴熟，难以配合银索金铃的「乐音绝技」。\n");

	if((int)me->query("neili") < 500+lv/2)
		return notify_fail("你的内力不够用来「乐音绝技」。\n");
	
	if((int)me->query("max_neili") < 1500)
		return notify_fail("你的内力不够用来「乐音绝技」。\n");

	if(me->query_skill_mapped("force") != "yunu-xinjing")
		return notify_fail("你所使用的内功不对。\n");

	if( target->query_temp("yueyin"))
		return notify_fail(target->name()+"被你的乐音所迷惑，你可以放胆攻击。\n");

	if( userp(me) && (string)me->query("family/family_name") != "古墓派")
		return notify_fail("你不是古墓派的传人，怎么会使用「乐音绝技」！\n");
	/*
	if ( weapon->query("id") !="whip")
	return notify_fail("你随便拿一条鞭子，怎么能发出声音？\n");
	*/
	msg = HIW"\n$N手中"+weapon->name()+HIW"飘动，幻成一片竹雾，又发出玎玎声响，忽怎忽缓，忽轻忽响，竟尔如乐曲一般。\n"NOR;
	msg +=HIW"音乐乃依循天籁及人身自然节拍而组成，是故乐音则听之悦耳，嘈杂则闻之心烦。\n"NOR;

	me->add("neili", -200-lv/4);

	lv = ((int)me->query_skill("yinsuo-jinling", 1)+(int)me->query_skill("yunu-jianfa", 1))/2;
	if (me->query("gender") == "男性") lv = lv/2;
	
	weapon1 = target->query_temp("weapon");

	if( weapon1 )//对方如果拿武器的话，夺下别人兵器，do_attack三次	//如果有互搏的话，就拿自己兵器再do_attack三次
	{
		if( random(me->query_per()*3) > target->query_per()*2
		|| COMBAT_D->do_attack(me,target,0,0,1)
		|| COMBAT_D->do_attack(me,target,0,0,1) 
		|| COMBAT_D->do_attack(me,target,0,0,1)  ){
			msg += HIR"\n$N手中"+weapon->name()+HIR"如银蛇般四下游走，只闻叮当之声，$n手腕上中了一击，被迫放弃了手中的"+weapon1->name()+HIR"。\n"NOR;
			msg += HIR"$N顺势夺过$n的"+weapon1->name()+HIR"，乘$n情急狼狈之际，左手握住"+weapon->name()+HIR"速攻三招。\n"NOR;
			message_vision(msg, me, target);
			weapon1->move(me);
			me->set_temp("juan", 1);
			me->add("neili", -200-lv/4);
			me->add_temp("apply/pfm_damage", 180+lv/5);
			me->add_temp("apply/pfm_attack", 180+lv/10);
			me->add_temp("apply/damage", 330+lv/2);
			me->add_temp("apply/action/color", HIW);
			COMBAT_D->do_attack(me, target, me->query_temp("weapon"), random(1+(me->query("gender")=="女性"))?3:1);
			if(present(target,environment(me)) && me->is_fighting(target))
				COMBAT_D->do_attack(me, target, me->query_temp("weapon"), random(1+(me->query("gender")=="女性"))?3:1);
			if(present(target,environment(me)) && me->is_fighting(target))	
				COMBAT_D->do_attack(me, target, me->query_temp("weapon"), random(1+(me->query("gender")=="女性"))?3:1);
			me->add_temp("apply/pfm_damage", -180-lv/5);
			me->add_temp("apply/pfm_attack", -180-lv/10);
			me->add_temp("apply/damage", -330-lv/2);
			me->delete_temp("apply/action/color");
			me->delete_temp("juan");
		}
		else
		{
			msg +=CYN"\n$n急忙大吼一声，发出一阵嘈音来冲散乐曲，霎时间两般声音交作，忽轻忽响，或高或低。\n"NOR;
			message_vision(msg, me, target);
			me->start_busy(1);
		}
		return 1;
	}
	// 如果对手不是用的兵器的话
	else if( (random(me->query_per()*3) > target->query_per()*2
	|| COMBAT_D->do_attack(me,target,0,0,1)
	|| COMBAT_D->do_attack(me,target,0,0,1) 
	|| COMBAT_D->do_attack(me,target,0,0,1) )
	&& me->is_fighting(target)){
		msg +=YEL"\n$n只听得铃声玎玎，有如乐曲，听了几下，竟便要顺著乐音出手。\n"NOR;
		message_vision(msg, me, target);
		target->set_temp("yueyin", lv);
		target->add_temp("apply/pfm_parry", -180);
		target->add_temp("apply/pfm_dodge", -180);
		if (lv > 400)
			target->apply_condition("no_perform", 2+lv/200);
		call_out("check_fight", 1, me, target, lv/30);
	}
	else{
		msg +=CYN"\n$n急忙大吼一声，发出一阵嘈音来冲散乐曲，霎时间两般声音交作，忽轻忽响，或高或低。\n"NOR;
		me->start_busy(1+random(2));
		message_vision(msg, me, target);
	}
	return 1;
}

void check_fight(object me,object target,int count)
{
	if(!target) return;
	if( !me
	|| !me->is_fighting(target)
	|| me->query_skill_mapped("whip") != "yinsuo-jinling"
	|| !me->query_temp("weapon")
	|| count -- < 1 ){
		target->add_temp("apply/pfm_parry", 180 );
		target->add_temp("apply/pfm_dodge", 180 );
		target->clear_condition("no_perform");
		if( target->is_busy()) target->start_busy(1);
		target->delete_temp("yueyin");
		message_vision(HIW"\n$N心情於瞬息之间接连数变，终于恢复了明智。\n\n"NOR, target);
		return ;
	}
	call_out("check_fight", 1, me, target, count);
}

int help(object me)
{
        write(HIW"\n银铃金索之「乐音绝技」："NOR"\n\n");
        write(@HELP
        要求：  当前内力 1000 以上；
                最大内力 1500 以上；
                玉女剑法等级 150 以上；
                玉女心经等级 150 以上；
                银索金铃等级 150 以上；
                激发鞭法为银索金铃；
                须在特定地点领悟习得；

HELP
        );
        return 1;
}   