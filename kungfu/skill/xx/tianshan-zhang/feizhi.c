//by caiji@SJ
//出处：天龙八部，第3册，975页1996年11月重印版/三联出版社出版
#include <ansi.h>

inherit F_SSERVER;
string perform_name(){ return HIC"飞掷"NOR; }
int query_ap() {return 2;}

int perform(object me, object target)
{
	object weapon;
	int lvl,flag;
	string msg;

	if( !target ) target = offensive_target(me);

	if( !objectp(target)
	||  !target->is_character()
	||  !me->is_fighting(target)
	||  !living(target))
	return notify_fail("「飞掷」只能对战斗中的对手使用。\n");

	if( (int)me->query_skill("tianshan-zhang", 1) < 100 )
	return notify_fail("你的天山杖法功力太浅，无法使用「飞掷」。\n");

	if( (int)me->query_skill("staff", 1) < 100 )
	return notify_fail("你的基本杖法功力太浅，无法使用「飞掷」。\n");

	if( (int)me->query_skill("huagong-dafa", 1) < 100 )
	return notify_fail("你的内功功力太浅，无法使用「飞掷」。\n");

	if( (int)me->query("max_neili") < 1000 )
	return notify_fail("你的内力太浅，无法使用「飞掷」。\n");
	if( (int)me->query("neili") < 600 )
	return notify_fail("你的真气不够，无法使用「飞掷」。\n");
	if (me->query_skill_mapped("force") != "huagong-dafa")
	return notify_fail("你用什么为内功基础来使「飞掷」?\n");
	if (!objectp(weapon = me->query_temp("weapon")) || weapon->query("skill_type") != "staff"
	|| me->query_skill_mapped("staff") != "tianshan-zhang")
	return notify_fail("你使得了「飞掷」么?\n");

	lvl = me->query_skill("tianshan-zhang",1);
  if (me->query("env/feizhi") != "海底捞针") flag =1;
	
	me->set_temp("apply/action/pfm", BLU"$N大喝一声，紧接着「呼」的一声猛响，$N把手中的"+weapon->name()+"掷向$n");
  me->set_temp("apply/action/hit", HIR"$n躲闪不及，被飞来的"+weapon->name()+"重重击中！\n"NOR);	

	me->receive_damage("neili", 200+lvl/2);
	me->add_temp("apply/damage",500+lvl/2-flag*200);
	me->add_temp("apply/pfm_damage",200+lvl/5-flag*50);
	me->add_temp("apply/pfm_attack",150+lvl/10-flag*30);
	COMBAT_D->do_attack(me, target,weapon,random(2)?0:3);
	me->add_temp("apply/damage",-500-lvl/2+flag*200);
	me->add_temp("apply/pfm_damage",-200-lvl/5+flag*50);
	me->add_temp("apply/pfm_attack",-150-lvl/10+flag*30);	      
	me->delete_temp("apply/action/");

	if (flag) {
		msg=HIG"\n$N身形一滞，动作无法连贯，眼看着"+weapon->name()+"脱手而去，掉落地上。\n"NOR;
		weapon->move(environment(me));
		me->start_busy(random(2));	
	} 
	else {
		msg=HIG"\n$N身如鬼魅，动若脱兔，一闪之间，已经将"+weapon->name()+"重新抓回手中！\n"NOR;	
		me->start_busy(1);
	}
	return 1;
}


int help(object me)
{
        write(BLU"\n天山杖法之「飞掷」："NOR"\n");
        write(@HELP
        要求：  当前内力  600 以上;
                最大内力 1000 以上；
                化功大法等级 100 以上；                
                天山杖法等级 100 以上;
                且手中有杖。

        
HELP
        );
        return 1;
}