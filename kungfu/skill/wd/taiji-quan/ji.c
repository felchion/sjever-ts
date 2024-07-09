// zhen.c 太极拳「挤」字诀
// snowman
#include <ansi.h>

int query_ap() {return 3;}

inherit F_SSERVER;
string *taiji_msg = ({"揽雀尾","单鞭","提手上势","白鹤亮翅","搂膝勾步","白蛇吐信","高探马",
"伏虎式","揽月势","金鸡独立","玉女穿梭","进步七星","弯弓射虎","抱虎归山",});

string perform_name(){ return HIR"挤字诀"NOR; }

int perform(object me, object target)
{
	int lvl;
	string *limbs;

	if( !target ) target = offensive_target(me);

	if( !objectp(target) || !target->is_character() || !me->is_fighting(target) || !living(target))
		return notify_fail("「挤」字诀只能对战斗中的对手使用。\n");

	if( userp(me) 
	&&( me->query_skill_mapped("parry") != "taiji-quan"
	&& me->query_skill_mapped("parry") != "taiji-jian") )
		return notify_fail("「挤字诀」必须在用太极拳或太极剑激发招架的情况下才能使用。\n");

	if( userp(me) && (me->query_skill_mapped("force") != "yinyun-ziqi"
	|| me->query_skill_prepared("cuff") != "taiji-quan") )
		return notify_fail("你的内功或拳法不对，根本不能发挥出「挤字诀」的优势。\n");  

	if( (int)me->query("neili", 1) < 500 )
		return notify_fail("你现在真气不够，强行使用「挤」是发挥不了它的威力的！\n");

	if( (int)me->query("jingli", 1) < 100 )
		return notify_fail("你现在精力不够，强行使用「挤」是发挥不了它的威力的！\n");                     

	if( me->query_temp("weapon"))
		return notify_fail("你先放下手中的武器再说吧？！\n");     

	lvl = me->query_skill("force",1);
	limbs = target->query("limbs");
 
	me->set_temp("apply/action/pfm", CYN "$N使一招太极拳「"+taiji_msg[random(sizeof(taiji_msg))]+"」，右脚实，左脚虚，运起"RED"「挤」"CYN"字诀，粘连粘随，右掌已搭住$n"+limbs[random(sizeof(limbs))]);
  me->set_temp("apply/action/hit", HIR"跟着$P横劲发出，$p给这么一挤，身子被自己的劲力带得不由主的向前一冲，跨出数步！\n"NOR);       
	target->set_temp("apply/action/parry", HIY"$p见此情景，一声惊噫，连忙收回自己的劲力，闪身避让！\n"NOR);
	target->set_temp("apply/action/dodge", HIY"$p见此情景，一声惊噫，连忙收回自己的劲力，闪身避让！\n"NOR);	

	me->receive_damage("neili", 500+lvl);
	me->add_temp("apply/force",300+lvl);
	me->add_temp("apply/pfm_force",150+lvl/5);
	me->add_temp("apply/pfm_attack",210+lvl/10);
	COMBAT_D->do_attack(me, target,0,random(2)?0:3);
	me->add_temp("apply/force",-300-lvl);
	me->add_temp("apply/pfm_force",-150-lvl/5);
	me->add_temp("apply/pfm_attack",-210-lvl/10);	      
	me->delete_temp("apply/action/");
	target->delete_temp("apply/action/");

	me->start_busy(random(2));
	return 1;
}

int help(object me)
{
	write(WHT"\n太极「挤字诀」："NOR"\n");
	write(@HELP
	要求： 内力 500 以上；      
		     精力 200 以上；      
HELP
	);
	return 1;
}