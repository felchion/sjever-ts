// riyue 日月鞭-日月
// campsun

#include <ansi.h>
#include <combat.h>
inherit F_SSERVER;


string perform_name() {return HIW"日月轮转"NOR;}
int query_ap() {return 4;}

int perform(object me, object target)
{

	int lvl = me->query_skill("riyue-bian", 1)/2;
	string msg,dodge_skill;
	object weapon = me->query_temp("weapon");

	if( !target ) target = offensive_target(me);

	if( !objectp(target) || !me->is_fighting(target) )
	return notify_fail("「日月轮转」只能在战斗中对对手使用。\n");

	if (!weapon || weapon->query("skill_type") != "whip")
	return notify_fail("手中没有武器,如何使用「日月轮转」！\n");

	if( (int)me->query_skill("riyue-bian", 1) < 180 )
	return notify_fail("你的日月鞭法不够娴熟，不会使用「日月轮转」。\n");

	if( (int)me->query_skill("yijin-jing", 1) < 180 )
	return notify_fail("你的易筋经等级不够，不能使用「日月轮转」。\n");

	if( (int)me->query_skill("buddhism", 1) < 180 )
	return notify_fail("你的禅宗心法等级不够，不能使用「日月轮转」。\n");

	if( (int)me->query("max_neili") < 2500 )
	return notify_fail("你现在内力修为太弱，不能使用「日月」。\n");
	if( (int)me->query("jingli") < 500 )
	return notify_fail("你现在体力太少，不能使用「日月」。\n");
	if ( me->query_skill_mapped("parry") != "riyue-bian")
	return notify_fail("你招架不对，不能使用「日月」。\n");
	if ( me->query_skill_mapped("force") != "yijin-jing")
	return notify_fail("你必须使用少林心法才能催动「日月轮转」。\n");

	if( (int)me->query("neili") < 1000 )
	return notify_fail("你现在内力太弱，不能使用「日月」。\n");

	msg = HIC "$N口中高宣佛号，使出日月鞭中「日月轮转」绝技，手中长索滚动飞舞，宛若灵蛇乱颤，自上而下劈向$n,\n" NOR;
	msg += HIC "接着$N手腕一抖，便如张牙舞爪的墨龙相似，急升而上，又往$n扑到。\n"NOR;
	message_vision(msg, me, target);
	me->set_temp("ryb_riyue", 1);

	me->add("neili",-500-lvl/2);
	me->add_temp("apply/pfm_damage", 250+lvl/5);
	me->add_temp("apply/pfm_attack", 150+lvl/10);
	me->add_temp("apply/damage", 300+lvl/2);


	COMBAT_D->do_attack(me, target, me->query_temp("weapon"), 1);
	if( me->is_fighting(target) )    
	  COMBAT_D->do_attack(me, target, me->query_temp("weapon"), 1);
	if( me->is_fighting(target) && (int)me->query_skill("riyue-bian", 1) > 220)
	  COMBAT_D->do_attack(me, target, me->query_temp("weapon"), 1);

	if (random(2)) {
		msg = HIY"\n紧跟着$N高宣“阿弥陀佛！”，使出「日 轮 」法诀，手中长索一扬，向$n的咽喉点去，索头未到，索上\n所挟"+
		"劲风犹如红日当空，令对方一阵气窒，只听见「啪啪」几声巨响，索头昂起，便如一条假死的毒蛇忽地飞\n"+
		"射反噬，带动无数风沙，呼啸而出。\n"NOR;
		message_vision(msg, me, target);
		me->add_temp("apply/pfm_damage", 150);
		if( me->is_fighting(target) )  COMBAT_D->do_attack(me, target, me->query_temp("weapon"), 3);
		me->add_temp("apply/pfm_damage", -150);
	}
	else {
		msg = HIC "\n紧跟着$N又宣“我佛慈悲！”，使出「月 转」法诀，内力返照空明，功力精纯，不露棱角，手中长索舞动时\n"
		"瞧不见半点影子无声无息，似缓实急，却又无半点风声，如鬼似魅，说不尽的诡异莫名。\n"NOR;
		message_vision(msg,me,target);
		if (!random(2)){
			msg=HIR"结果$n一不小心，登时淹没在$N发出的漫天鞭影之中。\n"NOR;
			message_vision(msg,me,target);
			target->start_busy(10);
		}
		else {
			dodge_skill = target->query_skill_mapped("dodge");
			if( !dodge_skill ) dodge_skill = "dodge";
			message_vision(SKILL_D(dodge_skill)->query_dodge_msg(target, 1), me, target);
		}
	}
	
	me->add_temp("apply/pfm_damage", -250-lvl/5);
	me->add_temp("apply/pfm_attack", -150-lvl/10);
	me->add_temp("apply/damage", -300-lvl/2);
	
	me->delete_temp("ryb_riyue");
	return 1;

}
int help(object me)
{
	write(HIC"\n日月鞭「日月轮转」："NOR"\n");
	write(@HELP
	
	要求：	最大内力 2500 以上；      
		当前内力 1000 以上；  
		易筋经等级 180 以上；
		日月鞭法等级 180 以上；
		禅宗心法等级 180 以上；
		激发日月鞭法为招架；
		激发易筋经为内功；
		装备长鞭类兵器；     
		并未设制打不还手。     
HELP
	);
	return 1;
}

