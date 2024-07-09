// chan.c 太极剑法「缠」字诀
// Created and Modified by snowman@SJ 23/02/2001

#include <ansi.h>
inherit F_SSERVER;

string perform_name(){ return HIW"天缠地绕诀"NOR; }

int query_ap() {return 5;}

int checking(object me, object target, object weapon);

string *number_msg = ({"一","二","三","四","五","六","七","八","九","十","十数","千百","无数"});
string *sword_msg = ({"反撩","斜刺","上挑","下划","正劈","直刺","后拉","前推"});

string *chan_msg = ({
CYN"$N以意驭剑，$S画出$R个"RED"圆圈"CYN"，其千变万化，无穷无尽，要去包裹缠住$n！\n" NOR,
CYN"$N以意驭剑，连绵不断地$S画出$R个"RED"圆圈"CYN"，前后排列，要将$n包裹起来！\n" NOR,
CYN"$N心中不存半点渣滓，以意驭剑，$W"CYN"每发一招，便似放出一条"RED"细丝"CYN"，要去缠住$n！\n"NOR,
CYN"$N神形归一，以意驭剑，$S画出$R个"RED"圆圈"CYN"，正反相交，要将$n缠在正中！\n" NOR,
CYN"$N手中$W"CYN"挥动，驱太极剑意散发出根根"RED"细丝"CYN"，要将$n裹起来！\n"NOR,
CYN"$N以意驭剑，$S一式，画出$R个"RED"圆圈"CYN"，上下包容，要将$n包裹起来！\n" NOR,
CYN"$N的一柄$W"CYN"画着一个个"RED"圆圈"CYN"，每一招均是以弧形刺出，以弧形收回，缠住$n！\n"NOR,
CYN"$N以意驭剑，连续$S画出$R个"RED"圆圈"CYN"，左右兼顾，要将$n缠在正中！\n" NOR,
});

string *dodged = ({
HIY"可是$n已然看穿了$N的招数，孤注一掷直入圈心，破了$N的太极剑意！\n\n" NOR,
HIY"$n微觉$N招数涩滞，急忙变招，竭尽全身之力中宫疾进，从剑光圈中冲了出去！\n\n"NOR,
HIY"$n微觉$N招数涩滞，急忙变招，竭尽全身之力飞跃后退，远离了剑圈！\n\n"NOR,
HIY"$n看得真切，回招挡格了$N中宫一剑，接着纵身一跃，已逃出圈外。\n\n"NOR,
});

string *chaned = ({
HIW"结果$n的招数渐见涩滞，偶尔发出一招，真气运得不足，便被$W"HIW"带着连转几个圈子。\n"NOR,
HIW"结果太极剑意散发出的细丝越积越多，似是积成了一团团丝棉，将$n紧紧裹了起来！\n"NOR,
HIW"结果，剑气似是积成了一团团丝棉，将$n紧紧裹了起来！\n"NOR,
HIW"$n越斗越是害怕，一个疏忽，被太极剑意紧紧裹了起来！\n"NOR,
});

int check_fight(object me, object target, object weapon);

int perform(object me, object target)
{
	object weapon = me->query_temp("weapon");

	if( !target ) target = offensive_target(me);

	if( !objectp(target)
	|| !target->is_character()
	|| !me->is_fighting(target) )
		return notify_fail("牵制攻击只能对战斗中的对手使用。\n");

	if(!living(target))
		return notify_fail("「缠」不能动的人？\n");

	if(!weapon || weapon->query("skill_type") != "sword" )
		return notify_fail("你手中无剑，如何使得「缠」字诀？\n");

	if( me->query_temp("tjj/chan"))
		return notify_fail("你正在使用「缠」字诀。\n");

	if( me->query_temp("tjj/lian"))
		return notify_fail("你正在使用连。\n");

	if( target->is_busy() )
		return notify_fail(target->name() + "目前正自顾不暇，放胆攻击吧！\n");

	if( (int)me->query_skill("taiji-jian", 1) < 100 )
		return notify_fail("你的太极剑法极不够娴熟，不会使用「缠」字诀。\n");

	if( me->query("neili") <= 500 )
		return notify_fail("你的内力不够！\n");

	if( me->query("max_neili") <= 1000 )
		return notify_fail("你的内力修为不够！\n");

	if( me->query_temp("combat_yield"))
		return notify_fail("你不出剑，何以使得「缠」字诀？\n");

	if( me->query_skill_mapped("sword") != "taiji-jian")
		return notify_fail("你现在无法使用「缠」字诀。\n");

	if(userp(me)){
		if (me->query_skill_mapped("parry") != "taiji-jian")
			return notify_fail("你现在无法使用「缠」字诀。\n");       
		if( (int)me->query_skill("yinyun-ziqi", 1) < 100 )
			return notify_fail("你的内功修为不够，不会使用「缠」字诀。\n");
		if( me->query_skill_mapped("force") != "yinyun-ziqi")
			return notify_fail("你所用的内功与太极剑意气路相悖！\n");
	}

	message_vision(CYN"\n$N使出太极剑法"RED"「缠」"CYN"字诀，一柄"+weapon->name()+CYN"在画着一个个圆圈，每一招均是以弧形刺出，弧形收回。\n"+
				"神在剑先，绵绵不绝，便如撒出了一张大网，逐步向$n收紧！\n\n"NOR, me, target);
	me->set_temp("tjj/chan", 1);
	checking(me, target, weapon);
	return 1;
}

int check_fight(object me, object target, object weapon)
{
	string msg;
	object wep;
	int i = 2;

	if (!me || !me->query_temp("tjj/chan")) return 1;

	me->delete_temp("tjj/chan");

	if ( !living(me)) return 1;

	if( me->query_skill("taiji-jian", 1) > 450 ) i = 3;

	if(!objectp(wep = me->query_temp("weapon")) || wep->query("skill_type") != "sword"){
		if(!wep) msg = HIY"$N手中武器既失，圆转的太极剑幕也随之消散。\n"NOR;
		else if(!target)
			msg = HIY"$N将手中"+wep->name()+HIY"向后一摆，抱圆守一，撤回了散开的太极剑意．\n"NOR;
		else  msg = HIY"$N亮出"+wep->name()+HIY"而撤回太极剑意，$n身边密布的剑幕也随之消散，压力骤然减轻！\n"NOR;
			me->start_busy(random(2));
		if(target->is_busy()) target->start_busy(1);
	}
	else if(!target)
		msg = HIY"$N将手中"+wep->name()+HIY"向后一摆，抱圆守一，撤回了散开的太极剑意．\n"NOR;

	else if(environment(me) != environment(target))
		msg = HIY"$N将手中"+wep->name()+HIY"向后一摆，抱圆守一，撤回了散开的太极剑意．\n"NOR;

	else if(!living(target) || (!me->is_fighting(target) && !me->is_killing(target->query("id")))){
		if(target && !living(target)) msg = HIY"$N不屑地望了$n一眼，将手中"+wep->name()+HIY"一向后摆，撤回了散开的剑幕．\n"NOR;
		else msg = HIY"$N将手中"+wep->name()+HIY"向后一摆，抱圆守一，撤回了散开的太极剑意．\n"NOR;
	}

	else if(wep != weapon && target->is_busy()
	&& random(me->query_skill("taiji-jian", 1)) < (int)target->query_skill("dodge", 1)/2){
			msg = HIY"$N撤后换剑，剑势顿时无法圆转如意，$n身边的太极剑幕威力骤减，使$n有了喘息之机会！\n"NOR;
		if(target->is_busy()) target->start_busy(1);
			me->start_busy(random(2));
	}
	else if (userp(me) && (me->query_skill_mapped("sword") != "taiji-jian" || me->query_skill_mapped("parry") != "taiji-jian")){
			msg = HIY"$N突然变招，划破剑幕，使出和太极剑法完全不同的招数来！\n"NOR;
		if(target->is_busy()) target->start_busy(1);
			me->start_busy(random(2));
	}
	else if( me->query("neili") <= me->query_skill("taiji-jian", 1)/2 ){
		msg = HIY"$N剑招渐见涩滞，偶尔一剑刺出竟然软弱无力，原来是真气不足了！\n"NOR;
		me->start_busy(random(2));
		if(target->is_busy()) target->start_busy(1);
	}

	else if(!target->is_busy() && !me->is_busy() && living(target)){
		if (random(i)) {
			checking(me, target, wep);
			return 1;
		}
		else {
			me->delete_temp("tjj/chan", 1);
			me->start_busy(random(2));
			return 1;
		}		
	}
	else {
		me->set_temp("tjj/chan", 1);
		call_out("check_fight", 1, me, target, wep);
		return 1;
	}
	if (me && target)
		message_vision(msg, me, target);
	else
		message_vision(msg, me);		
	return 1;
}      


int checking(object me, object target, object weapon)
{
	string msg;
	int cost;

	cost = me->query_skill("taiji-jian", 1)/2;

	msg = chan_msg[random(sizeof(chan_msg))];
	msg = replace_string(msg, "$S", sword_msg[random(sizeof(sword_msg))]);
	msg = replace_string(msg, "$R", number_msg[random(sizeof(number_msg))]);
	msg = replace_string(msg, "$W", weapon->name());

	me->add("neili", -350);
	me->add("jingli", -100);
	if( COMBAT_D->do_attack(me, target, me->query_temp("weapon"), 0, 1)
	|| COMBAT_D->do_attack(me, target, me->query_temp("weapon"), 0, 1) 
	|| COMBAT_D->do_attack(me, target, me->query_temp("weapon"), 0, 1) ){
		target->start_busy(3);
		msg += chaned[random(sizeof(chaned))];
		msg = replace_string(msg, "$W", weapon->name());
		me->set_temp("tjj/chan", 1);
		call_out("check_fight", 5, me, target, weapon);
	}
	else{
		me->start_busy(2+random(2));
		msg += dodged[random(sizeof(dodged))];
		msg = replace_string(msg, "$W", weapon->name());
		me->delete_temp("tjj/chan");
	}
	message_vision(msg, me, target);
	return 1;
}       

int help(object me)
{
	write(HIW"\n太极剑「缠字诀」："NOR"\n\n");
	write(@HELP
	太极剑意圆转不断，连绵不绝，临敌人之际以意驭剑，使出「缠」字诀每发
	一招，便似放出一条细丝，缠住对手，千变万化，无穷无尽，使其无法动作，
	并且可以自动不断连续发出，直至击败对手。
	
	要求：	最大内力 1000 以上；      
		内力 500 以上； 
		太极剑等级 100 以上；
		氤氲紫气等级 100 以上；
    激发剑法为太极剑法；
    激发招架为太极剑法；
    激发内功为氤氲紫气；
		并未设制打不还手。     

HELP
	);
	return 1;
}
