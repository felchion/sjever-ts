// bingdi.c 花开并蒂

#include <ansi.h>

inherit F_SSERVER;

int query_ap() {return 3;}

int perform(object me, object target)
{
	int lvl;

	if( objectp(me->query_temp("weapon")) )
		return notify_fail("「花开并蒂」只能空手使用。\n");

	lvl = (int)me->query_skill("zhenshan-mianzhang", 1);

	if( !target ) target = offensive_target(me);

	if( !objectp(target)
	 || !me->is_fighting(target) )
		return notify_fail("「花开并蒂」只能在战斗中对对手使用。\n");

	if( lvl < 100 )
		return notify_fail("你震山绵掌修为尚不纯熟,无法使用「花开并蒂」！\n");

	if( (int)me->query_skill("yinyun-ziqi",1) < 100 )
		return notify_fail("你的氤氲紫气修为尚浅，无法使用「花开并蒂」！\n");

	if( (int)me->query_skill("strike",1) < 100 )
		return notify_fail("你的基本掌法等级不够，不能使用「花开并蒂」！\n");

	if( (int)me->query("max_neili") < 1000 )
		return notify_fail("你的内力修为太弱，不能使用「花开并蒂」！\n");

	if( (int)me->query("neili") < 700 )
		return notify_fail("你的内力太少了，无法使用出「花开并蒂」！\n");

	if( (int)me->query("jingli") < 500 )
		return notify_fail("你的精力太少了，无法使用出「花开并蒂」！\n");

	if( me->query_skill_prepared("strike") != "zhenshan-mianzhang"
	 || me->query_skill_mapped("strike") != "zhenshan-mianzhang"
	 || me->query_skill_mapped("force") != "yinyun-ziqi")
		return notify_fail("你现在无法使用「花开并蒂」进行攻击。\n");

	if( me->query_temp("wd_hkbd"))
		return notify_fail("你刚使完「花开并蒂」，目前气血翻涌，无法再次运用！\n");

	message_vision(HIC"\n$N深深吸一口气，左手挥掌猛击$n右颊，右手出指疾点$n左肩“缺盆穴”，\n"
		+"跟着右掌击$n左颊，左手食指点$n右肩后“缺盆穴”。连续三式，便如暴风\n"
		+"骤雨般使出，这一招叫作“花开并蒂”，名称好听，招数却十分厉害。\n"NOR,me,target);

  me->add_temp("apply/damage", 250+lvl/2);
  me->add_temp("apply/pfm_damage", 150+lvl/5);
  me->add_temp("apply/pfm_attack", 150+lvl/10);

	me->set_temp("wd_hkbd",1);
	COMBAT_D->do_attack(me, target, 0, 1);

	if( me->is_fighting(target) )
		COMBAT_D->do_attack(me, target, 0, random(3)?1:3);

	if( lvl > 150 && me->is_fighting(target) )
		COMBAT_D->do_attack(me, target, 0,  random(2)?1:3);


  me->add_temp("apply/damage", -250-lvl/2);
  me->add_temp("apply/pfm_damage", -150-lvl/5);
  me->add_temp("apply/pfm_attack", -150-lvl/10);

	me->delete_temp("wd_hkbd");
	me->add("neili", -300);
	me->add("jingli", -100);
	me->start_busy(random(2));
	return 1;
}

string perform_name(){ return HIC"花开并蒂"NOR; }

int help(object me)
{
        write(HIC"\n震山绵掌之「花开并蒂」："NOR"\n\n");
        write(@HELP
        当年宋青书在光明顶与张无忌中使用的特殊攻击，在瞬间连攻数招。

        要求：  当前内力 700 以上；
                最大内力 1000 以上；
                当前精力 500 以上；
                震山绵掌等级 100 以上；
                氤氲紫气等级 100 以上；
                基本掌法等级 100 以上；
                激发掌法为震山绵掌；
                激发内功为氤氲紫气。

HELP
        );
        return 1;
}
