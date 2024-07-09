// xixue.c 吸血青蝠
// By River
#include <ansi.h>

inherit F_SSERVER;

int query_ap() {return 3;}

int perform(object me,object target)
{
	int lvl,qi,m;
	lvl = me->query_skill("hanbing-mianzhang",1);

	if( !target ) target = offensive_target(me);

	if( !objectp(target)
	 || !me->is_fighting(target))
		return notify_fail("你只能对战斗中的对手使用「吸血青蝠」。\n");

	if( me->query_temp("weapon"))
		return notify_fail("你拿着武器怎么能使用「吸血青蝠」！\n");

	if(lvl < 120 )
		return notify_fail("你的「寒冰绵掌」等级不够, 还不能使用「吸血青蝠」！\n");

	if( me->query_skill_mapped("strike") != "hanbing-mianzhang")
		return notify_fail("你现在无法使用「吸血青蝠」！\n");

	if (me->query_skill_prepared("strike") != "hanbing-mianzhang")
		return notify_fail("你现在无法使用「吸血青蝠」！\n");

	if ( me->query_skill_mapped("parry") != "qiankun-danuoyi"
	&& me->query_skill_mapped("parry") != "hanbing-mianzhang")
		return notify_fail("你现在无法使用「吸血青蝠」！\n");

	if( me->query_skill("dodge", 1) < 120 )
		return notify_fail("你的轻功不够，无法熟练运用「吸血青蝠」！\n");

	if( !me->query_skill("jiuyang-shengong", 1))
	if( me->query_skill("shenghuo-shengong", 1) < 120)
		return notify_fail("你的圣火神功等级不够，无法使用「吸血青蝠」！\n");

	if((int)me->query("neili") < 1500)
		return notify_fail("你的内力不够使用「吸血青蝠」。\n");

	if( me->query_temp("qkdny/yin") )
		return notify_fail("你正在施展乾坤大挪移「引」字诀呢！\n");

	me->set_temp("apply/action/pfm", HIC"$N突然如同一只吸血青蝠，飘近$n的身前，张嘴便向$n的咽喉咬去"NOR);
  me->set_temp("apply/action/hit", HIR"$n只觉得咽喉一阵剧痛，$N正在狂吸$n的鲜血！\n"NOR);       
	me->set_temp("apply/action/damage_type","啮伤");
	target->set_temp("apply/action/parry", CYN"只见$n不慌不忙，轻轻一闪，躲过了$N的利齿！\n"NOR);
	target->set_temp("apply/action/dodge", CYN"只见$n不慌不忙，轻轻一闪，躲过了$N的利齿！\n"NOR);	

	me->receive_damage("neili", 500+lvl/2);
	me->add_temp("apply/damage",500+lvl);
	me->add_temp("apply/pfm_damage",160+lvl/5);
	me->add_temp("apply/pfm_attack",150+lvl/10);
	qi = COMBAT_D->do_attack(me, target,0,random(2)?0:3);
	me->add_temp("apply/damage",-500-lvl);
	me->add_temp("apply/pfm_damage",-160-lvl/5);
	me->add_temp("apply/pfm_attack",-150-lvl/10);	      
	me->delete_temp("apply/action/");
	target->delete_temp("apply/action/");
   
  qi /= 5;
  m = me->query("max_qi")-me->query("eff_qi");
  
  if (qi>0 && m>0) {
  	if (qi > m)	me->add("eff_qi",m);
  	else 	me->add("eff_qi",qi);
    message_vision(HIG"$N吸入大量的鲜血后，身上的伤势也好转了一些！\n"NOR,me);
  }

	me->start_busy(random(2));
	return 1;
}

string perform_name(){ return HIC"吸血青蝠"NOR; }

int help(object me)
{
        write(HIC"\n寒冰绵掌之「吸血青蝠」："NOR"\n\n");
        write(@HELP
        要求：  当前内力 1500 以上；
                寒冰绵掌等级 120 以上；
                若无九阳神功需圣火神功等级 120 以上；
                基本轻功等级 120 以上；
                激发掌法为寒冰绵掌；
                激发招架为寒冰绵掌或乾坤大挪移。

HELP
        );
        return 1;
}
