// /kungfu/skill/yiyang-zhi/sandie.c
// COOL@SJ
// Modified by snowman@SJ 05/12/2000

#include <ansi.h>
inherit F_SSERVER;
#include <combat.h>
#include <combat_msg.h>
int query_ap() {return 3;}
string perform_name(){ return HIY"阳关三叠"NOR; }

int perform(object me, object target)
{
	string msg;
	int lvl,damage;
	if( !target ) target = offensive_target(me);

	if( !target
	|| !me->is_fighting(target))
		return notify_fail("「阳关三叠」只能在战斗中使用。\n");
	
	if( !living(target) )
		return notify_fail("对方已经晕过去了。\n");

	if( objectp( me->query_temp("weapon")) )
		return notify_fail("只有空手才能施展「阳关三叠」。\n");

	if((int)me->query_skill("yiyang-zhi", 1) < 100
	||(int)me->query_skill("finger", 1) < 100 )
		return notify_fail("你有的功夫还不够娴熟，不会使用「阳关三叠」。\n");

	if(!me->query_skill("kurong-changong", 1))
	if(me->query_skill("qiantian-yiyang", 1) <100)
		return notify_fail("以你现在的内功修为还使不出「阳关三叠」。\n");

	if (me->query_skill_mapped("force") != "kurong-changong" )
	if(me->query_skill_mapped("force") != "qiantian-yiyang")
		return notify_fail("你所使用的内功不对。\n");

	if (me->query_skill_prepared("finger") != "yiyang-zhi"
	|| me->query_skill_mapped("finger") != "yiyang-zhi" )
		return notify_fail("你现在无法使用「阳关三叠」进行攻击。\n");

	if(me->query_temp("start_6mai"))
		return notify_fail("你已经在运用六脉神剑的无形剑气了！\n");

	if (me->query_skill_mapped("parry") != "kurong-changong" )
		if (me->query_skill_mapped("parry") != "yiyang-zhi")
			return notify_fail("你所使用的招架不对。\n");

	if((int)me->query("max_neili") < 1500 )
		return notify_fail("你的内力修为太弱，不能使用「阳关三叠」！\n");

	if((int)me->query("neili") < 800 )
		return notify_fail("你的真气不够！\n");	

	if( me->query_temp("sandie/js") || me->query_temp("sandie/jt")  )
		return notify_fail("你的真气不够！\n");

	if(me->query("env/一阳指")=="惊神"){
		if ( target->query_temp("sandie/jingshen"))
			return notify_fail("你正在向对手使用着「阳关三叠之惊神」绝招！\n");
		else {
			if ( me->query_temp("sandie/js"))
				return notify_fail("你正在使用着「阳关三叠之惊神」绝招！\n");
			me->set_temp("sandie/js",1);
			call_out("perform2", 0, me, target);
			return 1;
		}
	}
	else if(me->query("env/一阳指")=="截天"){
		if ( target->is_busy() )
			return notify_fail("对方正忙着呢！\n");
		else {
			if ( me->query_temp("sandie/jt") )
				return notify_fail("你正在使用着「阳关三叠之截天」绝招！\n");
			me->set_temp("sandie/jt",1);
			call_out("perform3",0,me, target);
			return 1;
		}
	}
	lvl = me->query_skill("yiyang-zhi",1);
	
	if((string)me->query("class")=="bonze")
		msg = YEL "\n$N使出「阳关三叠」绝招,双掌合什，凝气于指，一式「破煞」点出，但听得嗤的一声轻响，一股纯阳的内力直袭$n胸口" NOR;
	else if ((string)me->query("class")=="officer")
		msg = YEL "\n$N使出「阳关三叠」绝招，须发飞舞, 威猛之极，大喝一声「破煞」,一指点出，但听得嗤嗤有声，一股纯阳的内力直袭$n胸口" NOR;
	else 
		msg = YEL "\n$N使出「阳关三叠」绝招，大喝一声「破煞」,一指点出，但听得嗤嗤有声，一股纯阳的内力直袭$n胸口" NOR;
	
	me->set_temp("sandie_action",msg);
	me->set_temp("tls/sandie",1);
	me->add_temp("apply/pfm_attack",180+lvl/10);
	me->add_temp("apply/pfm_force",200+lvl/4);
	me->add_temp("apply/force",lvl*2);
	damage = COMBAT_D->do_attack(me, target,0,3);
	me->delete_temp("sandie_action",msg);
	me->delete_temp("tls/sandie");
	me->add_temp("apply/pfm_attack",-180-lvl/10);
	me->add_temp("apply/pfm_force",-200-lvl/4);
	me->add_temp("apply/force",-lvl*2);
	
	if( damage>0 ){
		if(me->query_skill("yiyang-zhi", 1) >=150
		&& me->query("env/一阳指") == "三叠" ){
			me->set_temp("yyz/sd",1);
			call_out("perform2",1,me, target);
		}
		else
		me->start_busy(2);
	}
	else {
		me->delete_temp("yyz/sd");
		me->add("neili", -70);
		msg = HIY"可是$p一个巧云翻身，远远的躲了开去。\n"NOR;
		if(me->query_skill("yiyang-zhi", 1) >= 150
		&& me->query("env/一阳指") == "三叠"){
				remove_call_out("perform2");
				me->set_temp("yyz/sd",1);
				call_out("perform2", 2, me, target);
		}
		me->start_busy(2);
		message_vision(msg, me, target);
	}

	return 1;
}

int perform2(object me, object target)
{
	int lvl,i;
	string msg;

	if (!me ) return 0;

	me->delete_temp("sandie/js");

	if( !living(me) ) return 0;

	if(!target
	|| !target->is_character()
	|| !me->is_fighting(target)
	|| !living(target) )
		return notify_fail("你正要发一招，却见对手已经不用打了。\n");;

	if((int)me->query("neili", 1) < 400 )
		return notify_fail("你待要发出一指，却发现自己的内力不够了！\n");;

	if( (int)me->query_skill("yiyang-zhi", 1) < 150)
		return notify_fail("你的一阳指修为不够，无发使出「惊神」。\n");;

	if((int)target->query_temp("sandie/jingshen"))
		return notify_fail("你正在对对手使用着「惊神」绝招！\n");

	lvl = (int)me->query_skill("yiyang-zhi",1);


	if( target->is_busy() ){
		msg = YEL "\n$N使出「阳关三叠」绝招，反手一指，一式「惊神」点出，但听得嗤的一声轻响，一股纯阳的指力透入$n体内任脉紫宫穴！\n" NOR;
		i = 2+random(2);
	}
	else {
		msg = YEL "\n紧跟着$N一阳指第二次点出，以纯阳内力，同时透入$n体内任脉紫宫穴、阴维脉大横穴、冲脉幽门穴、带脉章门穴和阴跤脉晴明穴！\n"NOR;
		i = 2;
	}
	if(  COMBAT_D->do_attack(me, target, 0, 0, 1) 
	||  COMBAT_D->do_attack(me, target, 0, 0, 1) 
	||  COMBAT_D->do_attack(me, target, 0, 0, 1) ){
		me->delete_temp("yyz/sd");
		me->add("neili", -lvl*2);
		msg += HIR "$n只觉一股灼热之气侵入体内奇经八脉,全身上下暖洋洋的，竟然提不起一丝力气！\n" NOR;
		target->add_temp("apply/pfm_attack", -lvl/4);
		target->add_temp("apply/pfm_dodge", -lvl/4);
		target->add_temp("apply/pfm_parry", -lvl/4);

		if( random(2) )
			target->apply_condition("no_exert", 2);
		else
			target->apply_condition("no_perform", 2);

		target->set_temp("sandie/jingshen",lvl/4);
		call_out("sandie_back", 5+random(20), target, lvl/4);

		if((int)me->query_skill("yiyang-zhi", 1) >= 200
		&& me->query("env/一阳指")=="三叠") {
			remove_call_out("perform3");
			me->set_temp("yyz/sd",1);
			call_out("perform3", 1, me, target);
		}
	}
	else{
		if(!me->query_temp("yyz/sd")){
			if( !target->is_busy() )
				target->start_busy(1);
			me->start_busy(1+random(2));
		}
		me->delete_temp("yyz/sd");
		me->add("neili", - 100);
		msg += HIY"\n两人身形一合瞬间分开,却见$p运起独门内功，消去了攻入体内的一阳指劲。\n" NOR;
		if((int)me->query_skill("yiyang-zhi", 1) >= 200
		&& me->query("env/一阳指")=="三叠" && random(2)){
			me->set_temp("yyz/sd",1);
			call_out("perform3", 2, me, target);
		}
	}

	message_vision(msg, me, target);
	return 1;
}

int perform3(object me, object target)
{
	int i;
	string msg;
	if( !me ) return 0;

	me->delete_temp("sandie/jt");
	me->delete_temp("yyz/sd");
	if( !living(me) ) return 0;

	if(!target
	|| !target->is_character()
	|| !me->is_fighting(target)
	|| !living(target) )
		return notify_fail("你正要发一招，却见对手已经不用打了。\n");

	if((int)me->query("neili", 1) < 300 )
		return notify_fail("你待要发出一指，却发现自己的内力不够了！\n");

	if( (int)me->query_skill("yiyang-zhi", 1) <200)
		return notify_fail("你的一阳指修为不够，无发使出「截天」。\n");

	if(target->is_busy())
		return notify_fail(target->name()+ "正自顾不瑕，你可以放胆攻击。\n");

	if ((string)me->query("env/一阳指")=="截天")
		msg = YEL"\n$N使出「阳关三叠」绝招，凌空一指，一式「截天」点向$n胸前"HIR"「玉堂穴」"YEL"，认穴竟无厘毫之差，这一指出手缓慢，无声无息！\n"NOR;
	else
		msg = YEL"\n接着$N踏前一步，出第三指隔空点向$n胸前"HIR"「玉堂穴」"YEL"，认穴竟无厘毫之差！\n"NOR;

	if( COMBAT_D->do_attack(me, target, 0, 0, 1)
	|| COMBAT_D->do_attack(me, target, 0, 0, 1)
	|| COMBAT_D->do_attack(me, target, 0, 0, 1) ){
		msg += HIR "结果一指点中，$n只觉得气脉通道阻塞，真气立受干挠！\n"NOR;
		target->start_busy(3+random(5));
		target->add("neili", -me->query_skill("force"));
		me->add("neili", -300);
	}
	else {
		me->start_busy(1+random(2));
		me->add("neili", -200);
		target->add("neili", -random(100));
		msg += HIY"\n$p眼看逼无可逼，便也隔空一击，两股力道相碰，$p顺势跳了开去。\n" NOR;
	}
	message_vision(msg, me, target);
	return 1;
}

void sandie_back(object target, int i)
{
	if( !target ) return;
	target->add_temp("apply/pfm_attack", i);
	target->add_temp("apply/pfm_dodge", i);
	target->add_temp("apply/pfm_parry", i);
	target->delete_temp("sandie/jingshen");
}

int help(object me)
{
        write(HIY"\n一阳指之「阳关三叠」："NOR"\n");
        write(@HELP
        
        「破煞」、「惊神」、「截天」一波三折。

        要求：最大内力 1500 以上；
              当前内力  800 以上；
              一阳指等级 100 以上；                
              基本指法等级 100 以上；
              激发一阳指为指法；
              激发一阳指为招架；
                
              一阳指等级 200 以上可三叠尽出。
                
HELP
        );
        return 1;
}