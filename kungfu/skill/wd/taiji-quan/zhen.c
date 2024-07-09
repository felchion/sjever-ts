// zhen.c 太极拳「震」字诀
// snowman
#include <ansi.h>

inherit F_SSERVER;


int query_ap() {return 5;}

string *taiji_msg = ({"十字手","如封似闭","搬拦捶","野马分鬃","斜飞势","云手","马步靠","闪通臂"});

string perform_name(){ return HIR"震字诀"NOR; }

int perform(object me, object target)
{
	int lvl;
	string msg;

	if( !target ) target = offensive_target(me);

	if( !objectp(target) || !target->is_character() || !me->is_fighting(target) || !living(target) )
		return notify_fail("神功震敌只能对战斗中的对手使用。\n");

	if( userp(me) && (me->query_skill_mapped("force") != "yinyun-ziqi"
	 || me->query_skill_prepared("cuff") != "taiji-quan") )
		return notify_fail("你的内功或拳法不对，又如何能使用「震」字诀？\n");

	if( (int)me->query("neili", 1) < 1000 )
		return notify_fail("你现在真气不够，强迫使用「震」字诀只会伤到你自己！\n");

	if( me->query_temp("weapon") )
		return notify_fail("你先放下手中的武器再说吧？！\n");

	msg = CYN "\n突然，$N一招「"+taiji_msg[random(sizeof(taiji_msg))]+"」，双手左右连画，一个圆圈已将$n套住，太极拳的"RED"「震」"CYN"字诀随即使出";

	me->set_temp("apply/action/pfm", msg);
  me->set_temp("apply/action/hit", "太极之意连绵不断，有如自去行空，一个圆圈未完，第二个圆圈已生，喀喇一响，$p一处骨头被绞断！\n");       
	target->set_temp("apply/action/parry", HIY"$p一看不对，马上一阵急攻，$P登时手忙脚乱，再也来不及出招！\n"NOR);
	target->set_temp("apply/action/dodge", HIY"$p一看不对，马上一阵急攻，$P登时手忙脚乱，再也来不及出招！\n"NOR);	

	me->receive_damage("neili", 500+lvl);
	me->add_temp("apply/damage",700+lvl*2);
	me->add_temp("apply/pfm_damage",280+lvl/3);
	me->add_temp("apply/pfm_attack",240+lvl/10);
	COMBAT_D->do_attack(me, target,0,3);
	me->add_temp("apply/damage",-700-lvl*2);
	me->add_temp("apply/pfm_damage",-280-lvl/3);
	me->add_temp("apply/pfm_attack",-240-lvl/10);	      
	me->delete_temp("apply/action/");
	target->delete_temp("apply/action/");

//msg += "$N恨他歹毒，「震」字诀使出时连绵不断，跟着喀喀喀几声，$p全身各处骨头也被一一绞断！\n";
	me->start_busy(random(2));
	return 1;
}

int help(object me)
{
	write(WHT"\n太极「震字诀」："NOR"\n");
	write(@HELP
	要求：	内力 1000 以上；      
		精力 200 以上；      
HELP
	);
	return 1;
}
