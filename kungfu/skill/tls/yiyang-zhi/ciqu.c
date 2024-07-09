// Create By lsxk@hsbbs 2007/6/14
#include <ansi.h>
#include <combat.h>
inherit F_SSERVER;

string perform_name() {return HIG"此去彼来"NOR;}
int query_ap() {return 4;}

int perform(object me, object target)
{
	int lev,hits,i;
	string restore, *liumaiset;

	if( !target ) target = offensive_target(me);

	if( !objectp(target)
	|| !target->is_character()
	|| !me->is_fighting(target)
	|| !living(target) )
	return notify_fail("「此去彼来」只能在战斗中使用。\n");

	if( me->query_temp("weapon"))
	return notify_fail("你拿着武器怎么能使用「此去彼来」！\n");

	if( (int)me->query_skill("liumai-shenjian", 1) < 30 )
	return notify_fail("你还尚未精通「六脉神剑」之一脉，如何使得出该绝技?\n");

	if((int)me->query_skill("force") < 250 )
	return notify_fail("你的内功修为还不够，使不出「此去彼来」绝技。\n");

	if( (int)me->query("jiali")< 100 )
	return notify_fail("你使用过少的内力伤敌，无法将「此去彼来」的威力体现出来！\n");

	if( me->query_temp("pf_rong"))
	return notify_fail("你正在运用「荣」字大法！\n");

	if( me->query_temp("yyz/6mai/6mai"))
	return notify_fail("你正在专心运用「六脉神剑」，分不出神来使用「此去彼来」！\n");

	if(!me->query_skill("kurong-changong", 1))
	if(me->query_skill("qiantian-yiyang", 1) < 250)
	return notify_fail("以你现在的内功修为还使不出「此去彼来」。\n");

	if (me->query_skill_prepared("finger") != "yiyang-zhi"
	|| me->query_skill_mapped("finger") != "yiyang-zhi" )
	return notify_fail("你现在无法使用「此去彼来」！\n");

	if (me->query_skill_mapped("force") != "kurong-changong" )
	if (me->query_skill_mapped("force") != "qiantian-yiyang")
	return notify_fail("你所使用的内功不对。\n");

	if( (int)me->query("max_neili") < 4500 )
	return notify_fail("你现在内力太弱，使不出「此去彼来」。\n");

	if( (int)me->query("neili") < 2000)
	return notify_fail("你现在真气太弱，使不出「此去彼来」。\n");

	if( (int)me->query("jingli") < 1000)
	return notify_fail("你现在精力太少，使不出「此去彼来」。\n");

	lev = (int)me->query_skill("yiyang-zhi", 1);
	hits= sizeof(keys(me->query("liumai-shenjian")));
	if (me->query("env/liumai"))
	restore = me->query("env/liumai");
	if (hits>6) hits=6;
	liumaiset = keys(me->query("liumai-shenjian"));
 
	message_vision(HIG"\n$N十指连动，时伸时缩，顿时激起"+
	HIR+chinese_number(hits)+HIG"股无形剑气，正是天龙寺镇寺武功六脉神剑之精髓招数「"HIM"此去彼来"HIG"」！\n"NOR,me,target);

  me->add_temp("apply/damage", lev*2);
  me->add_temp("apply/pfm_damage",230+lev/4);
  me->add_temp("apply/pfm_attack", 180+lev/10);

	me->set_temp("yyz/6m",1);
	for (i=0;i<hits;i++)
	{
		me->set("env/liumai",liumaiset[i]);
		COMBAT_D->do_attack(me, target, 0, (i>3)?3:1);
		me->add("neili", -500);
	}
	if (restore)
	me->set("env/liumai",restore);
	me->start_busy(1);
  me->add_temp("apply/damage", -lev*2);
  me->add_temp("apply/pfm_damage",-230-lev/4);
  me->add_temp("apply/pfm_attack", -180-lev/10);
	me->delete_temp("yyz/6m");
	me->add("jingli", -300);
	return 1;
}

int help(object me)
{
   write(WHT"\n一阳指之「"HIG"此去彼来"WHT"」："NOR"\n\n");
   write(@HELP

     要求: 最大内力 4500 以上；
           当前内力 2000 以上；
           当前精力 1000 以上；
           六脉神剑等级 30 以上；
           特殊内功等级 250 以上；
           基本内功等级 250 以上；
           加力 100 以上；
           激发指法为一阳指，备一阳指；
           不能和「六脉神剑」绝技一起使用。

HELP
   );
   return 1;
}
