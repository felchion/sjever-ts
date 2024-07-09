// huixuan.c 回旋十三式

#include <ansi.h>
#include <combat.h>
inherit F_SSERVER;

string perform_name(){ return HIY"回旋十三式"NOR; }

int perform(object me, object target)
{
	string msg;
	object weapon;
	int times, ap, dp;

	if( !target ) target = offensive_target(me);

	weapon = me->query_temp("weapon");

	if( !objectp(target) || !target->is_character() || !me->is_fighting(target) || !living(target) )
		return notify_fail("「回旋十三式」只能在战斗中使用。\n");

	if( !weapon || weapon->query("skill_type") != "whip"
	 || me->query_skill_mapped("whip") != "huifeng-bian"
	 || me->query_skill_mapped("parry") != "huifeng-bian")
		return notify_fail("你现在无法使用「回旋十三式」！\n");

//	if( target->is_busy() )
//		return notify_fail(target->name() + "目前正自顾不暇，放胆攻击吧！\n");

	if( (int)me->query_skill("huifeng-bian", 1) < 100 )
		return notify_fail("你的回风鞭法不够娴熟，不会使用「回旋十三式」绝技。\n");

	if( (int)me->query_skill("huntian-qigong", 1) < 100 )
		return notify_fail("你的混天气功不够深厚，不会使用「回旋十三式」。\n");

	if( (int)me->query("max_neili", 1) < 800 )
		return notify_fail("你的内力修为不够，不能使用「回旋十三式」。\n");

	if( (int)me->query("neili") < 500 )
		return notify_fail("你现在内力太弱，不能使用「回旋十三式」。\n");

	if( me->query_skill_mapped("force") != "huntian-qigong" )
		return notify_fail("你现在使用的内功与回风鞭法相抵触，不能使用「回旋十三式」。\n");

	if( (int)me->query_temp("huixuan") )
		return notify_fail("你现在正在使用「回旋十三式」。\n");

	msg = HIY+"\n$N""突然身形如陀螺般急转数圈，手中的"+weapon->name()+HIY+"连出十三种"HIW"幻化莫测"HIY"的招式，罩向$n的四周！\n"NOR;

	ap = me->query("combat_exp", 1)/ 1000 * me->query_skill("whip");
	dp = target->query("combat_exp", 1)/ 1000 * target->query_skill("parry");

	if( random( ap + dp)  > dp ) {
		msg += HIR+"$n一个没留神，被$N变幻莫测的招式所惑，罩在幻象般的鞭影里！\n"NOR;
		me->start_perform(10 , "回旋十三式");
		times = random(me->query_skill("whip")/20) + 4;
		if( times <  6 ) times = 6;
		if( times > 13 ) times = 13;
		me->set_temp("huixuan", times);
		if( wizardp(me) ) tell_object(me, HIY"回旋十三式有 "+times+" 次。\n"NOR);
		message_vision(msg, me, target);
		COMBAT_D->do_attack(me, target, me->query_temp("weapon"), 1);
		me->start_busy(random(2));
		if( !userp(target) ) target->add_busy(random(2));
		call_out("huixuan", 1, me, target);
	}
	else {
		msg += CYN + "可是$n成竹在胸，抢中宫直进，从容不迫的摆脱了缠绕。\n"NOR;
		me->start_perform(2 , "回旋十三式");
		me->start_busy(random(2));
		message_vision(msg, me, target);
	}
	me->add("neili", -200);
	return 1;
}

int huixuan(object me, object target)
{
	int lvl, ap, dp, times;
	object weapon;

	if( !me ) return 0;

	weapon = me->query_temp("weapon");

	if( !target ) {
		message_vision(HIY+"\n$N"+HIY+"内力已尽，"+weapon->query("name")+HIY+"软绵绵的垂将下来。\n"NOR,me);
		me->start_perform(1 , "回旋十三式");
		me->delete_temp("huixuan");
		return 1;
	}

	if( !me->is_fighting(target) ) {
		message_vision(HIY+"\n$N"+HIY+"内力已尽，"+weapon->query("name")+HIY+"软绵绵的垂将下来。\n"NOR,me);
		me->start_perform(1 , "回旋十三式");
		me->delete_temp("huixuan");
		return 1;
	}

	if( !living(me) || me->is_ghost() ) {
		me->start_perform(1 , "回旋十三式");
		me->delete_temp("huixuan");
		return 1;
	}

	if( !weapon || weapon->query("skill_type") != "whip"
	 || me->query_skill_mapped("whip") != "huifeng-bian"
	 || me->query_skill_mapped("parry") != "huifeng-bian" ) {
		me->start_perform(1 , "回旋十三式");
		me->delete_temp("huixuan");
		return 1;
	}

	if( me->query_temp("huixuan") < 1 ) {
		message_vision(HIY+"\n$N"+HIY+"内力已尽，"+weapon->query("name")+HIY+"软绵绵的垂将下来。\n"NOR,me);
		me->start_perform(1 , "回旋十三式");
		me->delete_temp("huixuan");
		return 1;
	}

	if( me->is_busy() ) {
		call_out("huixuan", 1, me, target);
		return 1;
	}

	lvl = me->query_skill("huifeng-bian", 1);

	ap = me->query("combat_exp", 1)/ 1000 * me->query_skill("whip");
	dp = target->query("combat_exp", 1)/ 1000 * target->query_skill("dodge");

	me->start_perform(10 , "回旋十三式");

	if( random(ap + dp)  > dp/2 ) {
		if( weapon->query("id") == "po budai" ) {
			message_vision( HIR+me->query("name")+"将手中的"+weapon->query("name")+HIR"向$n头上一罩。\n"NOR, me, target);
			tell_object(target,BLU"你觉得脸上一阵剧痛，啊！！！你被蝎子咬了。\n"NOR);
			target->receive_wound("qi", me->query_skill("whip")/3, me);
			target->receive_wound("jing", me->query_skill("whip")/3, me);
			target->add_condition("scorpion_poison", 5);
		}
		message_vision(HIR"\n$n被眼前的"HIW"幻影"HIR"完全迷惑住了，$N乘此良机连连出招！"NOR, me, target);
		if( !userp(target) ) target->add_busy(random(2));
		me->add_temp("apply/whip", lvl/3);
		me->add_temp("apply/attack", lvl/3);
		me->add_temp("apply/damage", lvl/6);
		target->add_temp("apply/dodge", - me->query_skill("whip", 1)/4);
		COMBAT_D->do_attack(me, target, me->query_temp("weapon"), random(3)?1:3);
		me->add_temp("huixuan", -1);

		if( me->is_fighting(target) ) {
			COMBAT_D->do_attack(me, target, me->query_temp("weapon"), random(2)?1:2);
			me->add_temp("huixuan", -1);
		}
		me->add_temp("apply/whip", -lvl/3);
		me->add_temp("apply/attack", -lvl/3);
		me->add_temp("apply/damage", -lvl/6);
		target->add_temp("apply/dodge", me->query_skill("whip", 1)/4);
		times = me->query_temp("huixuan");
		call_out("huixuan", 2+random(3), me, target);
		if( wizardp(me) ) tell_object(me, HIY"回旋十三式还有 "+times+" 次。\n"NOR);
		return 1;
	}

	times = me->query_temp("huixuan");
	call_out("huixuan", 1+random(3), me, target);
	if( wizardp(me) ) tell_object(me, HIY"回旋十三式被破解，还有 "+times+" 次。\n"NOR);
	return 1;
}

int help(object me)
{
	write(HIY"\n回风鞭法「回旋十三式」："NOR"\n");
        write(@HELP
	要求：  最大内力  800 以上；
                当前内力  500 以上；
                回风鞭法等级 100 以上；
                混天气功等级 100 以上；
                激发内功为混天气功。
HELP
        );
        return 1;
}