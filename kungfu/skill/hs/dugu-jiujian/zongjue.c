#include <ansi.h>
inherit F_SSERVER;

int query_ap() {return 5;}

int perform(object me, object target)
{
	string msg, *skill;
	object weapon;
	mapping skills;
	int i;


	if( !target ) target = offensive_target(me);

	if( !objectp(target)
	 || !target->is_character()
	 || !me->is_fighting(target)
	 || !living(target) )
		return notify_fail("「总诀式」只能对战斗中的对手使用。\n");

	if (!objectp(weapon = me->query_temp("weapon"))
	|| (string)weapon->query("skill_type") != "sword")
		return notify_fail("你使用的武器不对。\n");

	if((int)me->query_skill("dugu-jiujian",1) < 350 )
		return notify_fail("你的独孤九剑还不够火候。\n");

	if((string)me->query_skill_mapped("sword") != "dugu-jiujian" && userp(me))
		return notify_fail("你现在无法使用「总诀式」！\n");

	if(target->query_temp("dgjj/zongjue") )
		return notify_fail("你现在已经在向对手使用「总诀式」了。\n");

	if((int)me->query("neili")<2000)
		return notify_fail("你的精力不够！\n");

	msg = HIW"\n$N剑上完全不出内力，剑随意行，毫无章法！\n"NOR;
	me->add("neili", -1000);
	if (COMBAT_D->do_attack(me, target, me->query_temp("weapon"), 0, 1)
	|| COMBAT_D->do_attack(me, target, me->query_temp("weapon"), 0, 1)
	|| COMBAT_D->do_attack(me, target, me->query_temp("weapon"), 0, 1)){
		msg+= HIG"\n$n只觉得处处受制，武功中厉害之处完全无法发挥出来！\n"NOR;
		skills = target->query_skill_map();
		if (!skills) return notify_fail("他并没有激发特殊技能。\n");
		skill = keys(skills);
		i = sizeof(skill);
		while (i--)
			target->map_skill(skill[i]);
		target->delete("jiali");

		target->set_temp("no_enable",1);
		target->set_temp("dgjj/zongjue",1);
		me->set("jiali",0);
		call_out("remove_effect",10,me,target);
	} 
	else
		msg+= HIG"\n$n灵机一动，身随剑走，果然$N再也无法随意出招。\n"NOR;
	message_vision(msg,me,target);
	return 1;
}

void remove_effect(object me, object target)
{
	if (me) me->delete_temp("dgjj/zongjue");
	if (target) target->delete_temp("no_enable");
	if (target) target->delete_temp("dgjj/zongjue");
}

string perform_name(){ return HIR"总诀式"NOR; }

int help(object me)
{
	write(HIR"\n独孤九剑之「总诀式」："NOR"\n\n");
	write(@HELP	

	
	要求：	
		当前内力 2000 以上；
		独孤九剑等级 350 以上；
		激发剑法为独孤九剑；
		激发招架为独孤九剑；
		且手中持剑。
 
HELP
	);
	return 1;
}