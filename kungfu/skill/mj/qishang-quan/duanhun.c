// 断魂夺魄
// by Looklove@SJ

#include <ansi.h>

inherit F_SSERVER;
int query_ap() {return 3;}

int perform(object me, object target)
{
	int lv, p;
	lv = me->query_skill("qishang-quan", 1);

	if( !target ) target = offensive_target(me);

	if( !objectp(target)
	|| !me->is_fighting(target)
	|| !living(target)
	|| environment(target)!=environment(me))
	return notify_fail("「断魂夺魄」只能在战斗中对对手使用。\n");

	if( objectp(me->query_temp("weapon")))
	return notify_fail("你必须空手才能使用「断魂夺魄」！\n");

	if( me->query_skill("qishang-quan", 1) < 180 )
	return notify_fail("你的七伤拳还不够娴熟，使不出「断魂夺魄」绝技。\n");

	if(!me->query_skill("jiuyang-shengong", 1))
	if( me->query_skill("shenghuo-shengong", 1) < 180 )
	return notify_fail("你的圣火神功等级还不够，使不出「断魂夺魄」绝技。\n");

	if( me->query_skill("force") < 200 )
	return notify_fail("你的内功等级不够，不能使用「断魂夺魄」。\n");

	if( me->query_str(1) < 40 )
	return notify_fail("你的膂力还不够强劲，使不出「断魂夺魄」来。\n");

	if( me->query_skill_prepared("cuff") != "qishang-quan"
	|| me->query_skill_mapped("cuff") != "qishang-quan")
	return notify_fail("你现在无法使用「断魂夺魄」！\n");

	if( me->query_skill_mapped("parry") != "qishang-quan"
	&& me->query_skill_mapped("parry") != "qiankun-danuoyi")
	return notify_fail("你需要激发招架为七伤拳，方能使用「断魂夺魄」。\n");

	if (target->query_temp("qishang"))
	return notify_fail("对方已经身中七伤拳内伤了。\n");

	if( me->query_temp("qkdny/yin") )
	return notify_fail("你正在施展乾坤大挪移「引」字诀呢！\n");

	if( me->query_temp("qkdny/hua") )
	return notify_fail("你正在施展乾坤大挪移「化」字诀呢！\n");

	if ( me->query_temp("qsq/duanhun"))
	return notify_fail("你正在使用「断魂夺魄」。\n");

	if( me->query("max_neili") < 3000)
	return notify_fail("你的内力太弱，使不出「断魂夺魄」。\n");

	if( me->query("neili") < 1200 )
	return notify_fail("你现在真气太弱，使不出「断魂夺魄」。\n");

	message_vision(HIR"\n$N口中念起七伤断魂夺魄歌诀，将七伤拳意发挥得淋漓尽致，大喝一声全力打出！！\n"NOR,me);

	p = me->query_skill("qishang-quan", 1) / 50;

	if (!me->query_skill("jiuyang-shengong", 1))
	  if ( p > 4 ) p = 4;
	else
	  if ( p > 5 ) p = 5;

	me->add("neili", -500 -lv/2);
	me->add_temp("apply/force",250+lv/2);
	me->add_temp("apply/pfm_force",120+lv/5);
	me->add_temp("apply/pfm_attack",90+lv/10);

	me->set_temp("apply/action/pfm",HIR"$N双目紧闭深运内力，然后猛的大喝了一声，以七种无比之劲力击向$n的全身");
	COMBAT_D->do_attack(me, target);

	if ( me->is_fighting(target)) {
		me->set_temp("apply/action/pfm",HIR"$N大喝一声，双拳连环虚击，风声未起，拳上暗劲已如怒潮倒灌，海啸逆卷，袭向$n");
		COMBAT_D->do_attack(me, target);
	}
	if ( me->is_fighting(target) && p > 3) {
		me->set_temp("apply/action/pfm",HIR"$N一声狂啸，双目赤红，拳路大开大阖，每一拳击出如诸神震怒，石破天惊");
		COMBAT_D->do_attack(me, target, 0, random(3)?1:3);
	}
	if ( me->is_fighting(target) && p > 4){
		me->set_temp("apply/action/pfm",HIR"$N一拳击出，拳风中竟带有刚柔三种不同的劲力，分击$n的全身上下三十六道要穴");
		COMBAT_D->do_attack(me, target, 0, random(2)?1:3);
	}

	me->add_temp("apply/force",-250-lv/2);
	me->add_temp("apply/pfm_force",-120-lv/5);
	me->add_temp("apply/pfm_attack",-90-lv/10);
	me->delete_temp("apply/action");

	me->start_bsuy(random(2));
	return 1;
}

string perform_name(){ return HIR"断魂夺魄"NOR; }

int help(object me)
{
	write(HIR"\n七伤拳之「断魂夺魄」："NOR"\n\n");
	write(@HELP
	要求：	当前内力 1200 以上；
		最大内力 3000 以上；
		当前精力 1000 以上；
		七伤拳等级 180 以上；
		圣火神功等级 180 以上；
		有效内功等级 200 以上；
		后天膂力 40 以上；
		激发拳法为七伤拳；
		激发招架为七伤拳或乾坤大挪移：
		备拳法为七伤拳且空手。

HELP
	);
	return 1;
}
