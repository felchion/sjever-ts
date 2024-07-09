//嫉世愤俗
#include <ansi.h>
#include <combat.h> 

inherit F_SSERVER;

int query_ap() {return 3;}

string perform_name() {return HIY"嫉世愤俗"NOR;}

int perform(object me, object target)
{
	int lvl;
	string *xue_name = ({
	HIW"徘徊空谷"NOR,BLU"力不从心"NOR,HIC"行尸走肉"NOR,HIG"庸人自扰"NOR,HIY"倒行逆施"NOR,
	HIR"废寝忘食"NOR,YEL"孤形只影"NOR,CYN"饮恨吞声"NOR,MAG"六神不安"NOR,GRN"穷途末路"NOR,
	HIM"面无人色"NOR,RED"想入非非"NOR,WHT"呆若木鸡"NOR,
	});


	lvl = me->query_skill("anran-zhang", 1);
	if (me->query("gender")=="女性") lvl/=2;

	if( !target ) target = offensive_target(me);

	if( !target
	|| !me->is_fighting(target) )
		return notify_fail("「嫉世愤俗」只能在战斗中使用。\n");

	if( me->query_temp("weapon"))
		return notify_fail("你拿着武器怎么能使用「嫉世愤俗」！\n");

	if( (int)me->query_skill("anran-zhang", 1) < 150 )
		return notify_fail("你的黯然销魂掌还不够娴熟，使不出「嫉世愤俗」绝技。\n");

	if( (int)me->query_skill("yunu-xinjing", 1) < 150 )
		return notify_fail("你的玉女心经等级还不够，使不出「嫉世愤俗」绝技。\n");

	if ( me->query_skill_mapped("strike") != "anran-zhang"
	|| me->query_skill_mapped("parry") != "anran-zhang"
	|| me->query_skill_mapped("force") != "yunu-xinjing"
	|| me->query_skill_prepared("strike") != "anran-zhang"	)
		return notify_fail("你现在无法使用「嫉世愤俗」绝技！\n");

	if((int)me->query("max_neili") < 2000 )
		return notify_fail("你现在内力太弱，使不出「嫉世愤俗」。\n");

	if((int)me->query("neili") < 500+lvl/2 )
		return notify_fail("你现在真气太弱，使不出「嫉世愤俗」绝技。\n");

	message_vision(HIY"\n$N突然一阵恶意涌上心头，发出的掌力竟然隐隐夹着风雷之声！\n"NOR,me,target);
	
	me->add("neili", -300-lvl/2);
	me->add_temp("jishi", 1);
	me->add_temp("apply/pfm_force", 180+lvl/4);
	me->add_temp("apply/force", 300+lvl);
	me->add_temp("apply/pfm_attack", 180+lvl/10);

	me->set_temp("apply/action/pfm",HIB"$N长啸一声，纵势将一式「"+xue_name[random(sizeof(xue_name))]+HIB"」使得虎虎生风，将$n罩入掌影之中");
	COMBAT_D->do_attack(me, target, 0, random(2)?1:3);
	if (present(target,environment(me)) && me->is_fighting(target)){
		me->set_temp("apply/action/pfm",HIB"$N不待$n看的真切，又是一式「"+xue_name[random(sizeof(xue_name))]+HIB"」，$n只觉头晕目眩不知身处何处");
		COMBAT_D->do_attack(me, target, 0, random(2)?1:3);
  }
  
	me->add_temp("apply/pfm_force", -180-lvl/4);
	me->add_temp("apply/force", -300-lvl);
	me->add_temp("apply/pfm_attack", -180-lvl/10);


	me->add("jingli", -50);
	me->delete_temp("jishi");
	me->delete_temp("apply/action/pfm");
	me->start_busy(1+random(2));
	return 1;
}

int help(object me)
{
         write(HIB"\n黯然销魂掌「嫉世愤俗」："NOR"\n");
        write(@HELP
        
要求：
        最大内力: 2000
        当前内力: 1000 
        玉女心经：150级;
        黯然销魂掌：150级;
        激发招架为黯然销魂掌;
        激发掌法为黯然销魂掌;
HELP
        );
        return 1;
}
