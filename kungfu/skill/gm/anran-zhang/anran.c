// 黯然销魂
// By River 
#include <ansi.h>
#include <combat.h> 

inherit F_SSERVER;

int query_ap() {return 3;}
string perform_name(){ return HIB"黯然"NOR; }

int perform(object me, object target)
{
	int lvl, i;

	lvl = me->query_skill("anran-zhang", 1);

	if( !target ) target = offensive_target(me);

	if( !target
	|| !me->is_fighting(target) )
		return notify_fail("「黯然销魂」只能在战斗中使用。\n");

	if(me->query("gender") != "男性")
		return notify_fail("只有男性才能使用「黯然销魂」绝技。\n");
		
	if( me->query_temp("weapon"))
		return notify_fail("你拿着武器怎么能使用「黯然销魂」！\n");

	if( (int)me->query_skill("anran-zhang", 1) < 150 )
		return notify_fail("你的黯然销魂掌还不够娴熟，使不出「黯然销魂」绝技。\n");

	if( (int)me->query_skill("yunu-xinjing", 1) < 150 )
		return notify_fail("你的玉女心经等级还不够，使不出「黯然销魂」绝技。\n");

	if ( me->query_skill_mapped("strike") != "anran-zhang"
	|| me->query_skill_mapped("parry") != "anran-zhang"
	|| me->query_skill_mapped("force") != "yunu-xinjing"
	|| me->query_skill_prepared("strike") != "anran-zhang"	)
		return notify_fail("你现在无法使用「黯然销魂」绝技！\n");

	if((int)me->query_temp("xinjing"))
		return notify_fail("你正在使用「心经」字诀。\n");

	if((int)me->query("max_neili") < 1500 )
		return notify_fail("你现在内力太弱，使不出「黯然销魂」。\n");

	if((int)me->query_temp("xiaohun"))
		return notify_fail("你现在正在使用「消魂」绝技。\n");
		
	i = 100 - (int)me->query("eff_qi")*100 / (int)me->query("max_qi");
	
	if((int)me->query("neili") < 500-i*5 )
		return notify_fail("你现在真气太弱，使不出「黯然销魂」绝技。\n");



	me->add("neili", -(500 -i*5));
	
	me->add_temp("anran", 1);
	me->add_temp("apply/pfm_force", 80 + i*3+lvl/5);
	me->add_temp("apply/force", 250 + i*10 +lvl/2);
	me->add_temp("apply/pfm_attack", 50+i*2+lvl/10);

	me->set_temp("apply/action/pfm",HIW"$N心下万念俱灰，没精打采地拍出一掌击向$n肩头");
	COMBAT_D->do_attack(me, target, 0, random(100-i)>30?1:3);
	if (present(target,environment(me)) && me->is_fighting(target)){
		me->set_temp("apply/action/pfm",HIC"紧跟着$N恍恍惚惚，懵懵懂懂，又随手挥起一掌向$n胸口袭去");
		COMBAT_D->do_attack(me, target, 0, random(100-i)>50?1:3);
  }
  if (present(target,environment(me)) && me->is_fighting(target) && i>75 && random(100-i)<5 ){
		me->set_temp("apply/action/pfm",BLU"$N心萌死志,再无任何牵挂，反手一掌向$n拍去，其势竟然有若天地雷动");
		me->add_temp("apply/pfm_attack", 100);
		me->add_temp("apply/rush",20);
		COMBAT_D->do_attack(me, target, 0,3);
		me->add_temp("apply/rush",-20);
		me->add_temp("apply/pfm_attack", -100);
  }
  
	me->add_temp("apply/pfm_force", -80 - i*3 - lvl/5);
	me->add_temp("apply/force", -250 - i*10 - lvl/2);
	me->add_temp("apply/pfm_attack", -50-i*2-lvl/10);

	me->add("jingli", -50);
	me->delete_temp("anran");
	me->delete_temp("apply/action/pfm");
	me->start_busy(1+random(2));
	return 1;
}

int help(object me)
{
         write(HIB"\n黯然销魂掌「黯然」："NOR"\n");
        write(@HELP

要求：
        最大内力: 1500
        当前内力: 500 （受伤时内力要求下降）
        玉女心经：150级;
        黯然销魂掌：150级;
        激发招架为黯然销魂掌;
        激发掌法为黯然销魂掌;

注: 受伤越重威力越大,实是置死地而得后生!

HELP
        );
        return 1;
}
