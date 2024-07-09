// 腐尸毒
#include <ansi.h>
#include <combat.h>

inherit F_SSERVER;

string perform_name() {return BLU"腐尸毒"NOR;}
int query_ap() {return 10;}

int perform(object me, object target)
{
	object co;

	if( !target ) target = offensive_target(me);

	if( !target || !me->is_fighting(target) )
	return notify_fail("「腐尸毒」只能在战斗中对对手使用。\n");

	if( objectp(me->query_temp("weapon")) )
	return notify_fail("你必须空手才能使用「腐尸毒」！\n");

	if( (int)me->query_skill("chousui-zhang", 1) < 120 )
	return notify_fail("你的星宿毒掌还不够娴熟，使不出「腐尸毒」绝技。\n");

	if( (int)me->query_skill("huagong-dafa",1) < 120 )
	return notify_fail("你的化功大法修为不够，不能使用「腐尸毒」。\n");

	if((int)me->query_skill("poison",1)<120)
	return notify_fail("你积毒不够无法使用腐尸毒。\n");
	co=present("corpse",me);

	if (!co)
	return notify_fail("你身上没有尸体，如何使用腐尸毒!\n");

	if (co->query("name")!="腐烂的男尸"
	&& co->query("name")!="腐烂的女尸")
	return notify_fail("尸体没有腐烂，无法使用腐尸毒。\n");

	if (me->query_skill_prepared("strike") != "chousui-zhang"
	|| me->query_skill_mapped("strike") != "chousui-zhang"
	|| me->query_skill_mapped("parry") != "chousui-zhang")
	return notify_fail("你现在无法使用「腐尸毒」！\n");

	if( (int)me->query("max_neili") < 1500)
	return notify_fail("你内力太弱，使不出「腐尸毒」。\n");

	if( (int)me->query("neili") < 1000)
	return notify_fail("你现在真气太弱，使不出「腐尸毒」。\n");

	message_vision(BLU"\n$N一声阴笑，右手突出，挟起一具尸体掷出，一股死鱼烂肉的臭气弥散四周，闻者欲呕！\n"NOR,me);
	call_out("destroying", 1, co);
	if (!target->is_killing(me))	me->kill_ob(target);
	me->add("neili", - (500 + me->query_skill("force")/2));
	if( COMBAT_D->do_attack(me, target, 0, 0, 1)
	|| COMBAT_D->do_attack(me, target, 0, 0, 1)
	|| COMBAT_D->do_attack(me, target, 0, 0, 1) ){
		message_vision(HIB"\n$n见此怪异的招数，手足无措，躲闪无及，只见散发着恶臭的尸体，正击在自己身上。\n"NOR, me, target);
		if (userp(target))
		target->set_temp("last_damage_from", me);
		target->set("eff_qi",-1);
		target->set("qi",-1);
		target->receive_wound("qi", 100, me);
		target->receive_damage("qi",100, me);
		me->start_busy(1);
		//if (userp(target)) 
		CHANNEL_D->do_channel(this_object(), "rumor", target->query("name")+"腐尸毒入脑死了。"); 
		return 1;
	}
	else {
		message_vision(HIB"\n$n见事不好，连忙纵身直起，腐尸从身下掠过，没有打中。\n"NOR,me,target);
		tell_object(target,HIB"\n你虽然及时避开了毒尸，但一股腥臭气入鼻，你已中了剧毒了。\n"NOR);
		if (userp(target)) me->add_condition("killer", 15);
		target->apply_condition("fs_poison", 20);
		me->start_busy(1);
		target->add_busy(1);
		return 1;
	}
	message_vision(HIB"\n$n胸有成竹，一声长啸，双掌连环劈出，把毒尸毒气荡向来路。\n"NOR,me,target);
	me->start_busy(1);
	return 1;
}

void destroying(object obj)
{
	destruct(obj);
}

int help(object me)
{
        write(BLU"\n抽髓掌之「腐尸毒」："NOR"\n\n");
        write(@HELP
        要求：  最大内力 1500 以上；
                当前内力 1000 以上
                毒技等级 120 以上；
                抽髓掌等级 120 以上；
                化功大法等级 120 以上；                
                且手中无兵器。

        
HELP
        );
        return 1;
}