// sanwu.c 三无三不手
// By River@SJ 02/16/2001

#include <ansi.h>
inherit F_SSERVER;
#include <combat_msg.h>
int query_ap() {return 5;}
string perform_name(){ return HIW"三无三不手"NOR; }
int perform3(object me,object target);
int perform2(object me,object target);

int perform(object me, object target)
{
	string msg;
	int lv;
	object weapon;

	if( !me->query("gmsanwu")) 
		return notify_fail("你还不会「三无三不手」这项绝技。\n");
	
	if( !target ) 
		target = offensive_target(me);

	if( !objectp(target)
	|| !target->is_character()
	|| !me->is_fighting(target))
		return notify_fail("「三无三不手」只能在战斗中使用。\n");

	if( !living(target))
		return notify_fail("对方已经晕过去了。\n");

	if( !objectp( weapon = me->query_temp("weapon"))
	|| weapon->query("skill_type") != "whip" )
		return notify_fail("你装备的武器不对，无法施展「三无三不手」。\n");

	if( (int)me->query_skill("yinsuo-jinling", 1) < 120
	|| (int)me->query_skill("meinu-quanfa", 1) < 120
	|| (int)me->query_dex() < 28)
		return notify_fail("你有的功夫还不够娴熟，不会使用「三无三不手」。\n");

	if(me->query_skill("yunu-xinjing", 1) < 120)
		return notify_fail("以你现在的内功修为还使不出「三无三不手」。\n");

	if(me->query_skill_mapped("force") != "yunu-xinjing")
		return notify_fail("你所使用的内功不对。\n");

	if (me->query_skill_prepared("cuff") != "meinu-quanfa"
	|| me->query_skill_mapped("cuff") != "meinu-quanfa"
	|| me->query_skill_mapped("whip") != "yinsuo-jinling" )
		return notify_fail("你现在无法使用「三无三不手」进行攻击。\n");

	if (me->query_skill_mapped("parry") != "yinsuo-jinling")
		return notify_fail("你所使用的招架不对。\n");

	if((int)me->query("max_neili") < 1300 )
		return notify_fail("你的内力修为太弱，不能使用「三无三不手」！\n");

	if((int)me->query("neili") < 500+lv/2 )
		return notify_fail("你的真气不够！\n");

	if( target->query_temp("sanwu/wukong")
	|| target->query_temp("sanwu/wusuo"))
		return notify_fail( target->name()+"已被你的三无三不手所伤，你可以放胆攻击了。\n");

	me->start_perform(4, "三无三不手");

	msg = BLU"突然间只见鞭影幌动，身前身后都是"+weapon->query("name")+BLU"的影子。$N这招「无孔不入」，乃是向$n周身百骸进攻，\n"+
	"虽是一招，其实千头万绪，一招之中包含了数十招，竟是同时点$n全身各处大穴";

	lv = me->query_skill("yinsuo-jinling", 1);
	if (me->query("gender") == "男性") lv = lv/2;
	
	me->set_temp("apply/action/pfm",msg);
	me->receive_damage("neili", 200+lv/4);
	me->add_temp("apply/force",350+lv);
	me->add_temp("apply/pfm_force",210+lv/5);
	me->add_temp("apply/pfm_attack",180+lv/5);
	COMBAT_D->do_attack(me, target,0,random(3)?1:3);
	me->add_temp("apply/force",-350-lv);
	me->add_temp("apply/pfm_force",-210-lv/5);
	me->add_temp("apply/pfm_attack",-180-lv/5);	  
	
	if( me->query_skill("meinu-quanfa", 1) >= 140
	&& me->query_skill("yinsuo-jinling", 1) >= 140
	&& me->query("neili") > 800 )
		call_out("perform2",2,me, target);
	return 1;
}

int perform2(object me, object target)
{
	int lv;
	string msg,dodge_skill;

	if(!target
	|| !target->is_character()
	|| !me->is_fighting(target)
	|| !living(target) ){
		tell_object(me, "你正要再发一招，却发现对手已经不用打了。\n");
		return 1;
	}

	lv = me->query_skill("meinu-quanfa", 1);
	if (me->query("gender") == "男性") lv = lv/2;
	
	msg = BLU"\n$N跟着一招「无所不至」，这一招点的是$n周身诸处偏门穴道。\n"NOR;
	
	if( random(me->query_per()*3) > target->query_per()*2
	|| COMBAT_D->do_attack(me,target,0,0,1) 
	|| COMBAT_D->do_attack(me,target,0,0,1) 
	|| COMBAT_D->do_attack(me,target,0,0,1) ){
		msg += HIR"$n躲闪不及，周身诸处的偏门穴道尽数被点中，全身竟然提不起一丝力气！\n"NOR;
		target->add_temp("apply/pfm_attack", -180);
		target->add_temp("apply/pfm_damage", -180);
		if (lv>400) {
			if( random(2))
				target->apply_condition("no_exert", 3);
			else
				target->apply_condition("no_perform", 3);
		}
		target->set_temp("sanwu/wusuo", 1);
		call_out("check_fight", 1, me, target, lv/25);
		me->add("neili", -200-lv/4);
	}
	else{
		dodge_skill = target->query_skill_mapped("dodge");
		if( !dodge_skill ) dodge_skill = "dodge";
		msg += SKILL_D(dodge_skill)->query_dodge_msg(target, 1);
	}
	message_vision(msg, me, target);
	if( me->query_skill("meinu-quanfa", 1) >= 160
	&& me->query_skill("yinsuo-jinling", 1) >=160
	&& me->query("neili") > 800 )
		call_out("perform3",2,me, target);
	return 1;
}

int perform3(object me, object target)
{
	object weapon;
	int lv;

	if(!me) return 1;
	if(!target
	|| !target->is_character()
	|| !me->is_fighting(target)
	|| !living(target) ){
		tell_object(me,"你正要再发一招，却发现对手已经不用打了。\n");
		return 1;
	}

	weapon = me->query_temp("weapon");

	lv = me->query_skill("meinu-quanfa",1);
	lv += me->query_skill("yinsuo-jinling", 1);
	lv /= 2;
	if (me->query("gender") == "男性") lv = lv/2;
	
	message_vision(BLU"\n$N的第三手「无所不为」立即使出，这一招不再点穴，专打眼睛、咽喉、小腹、下阴等人身诸般\n"+
	"柔软之处，是以叫作「无所不为」，阴狠毒辣。\n"NOR, me, target);

	me->add("neili", -200-lv/4);
	me->add_temp("apply/action/color", HIW);
	me->add_temp("apply/pfm_attack", 180+lv/10);
	me->add_temp("apply/pfm_damage", 170+lv/5);
	me->add_temp("apply/damage", 250+lv);
	me->set_temp("sanwu/buwei", 1);
	
	COMBAT_D->do_attack(me, target, me->query_temp("weapon"), random(4)?1:3);
	if (target && present(target,environment(me)) && me->is_fighting(target))
		COMBAT_D->do_attack(me, target, me->query_temp("weapon"), random(3)?1:3);
	if (target && present(target,environment(me)) && me->is_fighting(target))
		COMBAT_D->do_attack(me, target, me->query_temp("weapon"), random(2)?1:3);
	
	me->delete_temp("apply/action/color");
	me->add_temp("apply/pfm_attack", -180-lv/10);
	me->add_temp("apply/pfm_damage", -170-lv/5);
	me->add_temp("apply/damage", -250-lv);
	me->delete_temp("sanwu/buwei");

	me->add("jingli", -100);
	me->start_busy(random(2)+1);
	return 1;
}

void check_fight(object me,object target,int amount)
{
	if( !target ) return;
	if( !me
	|| !me->is_fighting(target)
	|| amount < 1){
		target->add_temp("apply/pfm_attack", 180);
		target->add_temp("apply/pfm_damage", 180);
		target->delete_temp("sanwu/wusuo");
		message_vision(HIW"\n$N穴道已解，精神不由为之一振。\n"NOR,target);
		return;
	}
	call_out("check_fight", 1, me, target, amount -1);
}

int help(object me)
{
        write(HIW"\n银铃金索之「三无三不手」："NOR"\n\n");
        write(@HELP
        要求：  当前内力 1000 以上；
                最大内力 1300 以上；
                美女拳法等级 120 以上；
                银索金铃等级 120 以上；
                激发鞭法为银索金铃；
                激发拳法为美女拳法；
                须得经李莫愁指点习得；

HELP
        );
        return 1;
}   