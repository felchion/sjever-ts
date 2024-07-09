#include <ansi.h>
inherit F_SSERVER;

int perform(object me, object target)
{
	string msg;
	int ap, dp;

	if( !target ) target = offensive_target(me);

	if( !objectp(target) || !target->is_character() || !me->is_fighting(target) )
		return notify_fail("伏魔只能对战斗中的对手使用。\n");

	if( me->query_skill("dafumo-quan", 1) < 200 )
		return notify_fail("你的大伏魔拳不够娴熟，无法使用伏魔。\n");

	if( me->query_skill("jiuyin-zhengong", 1) < 200 )
		return notify_fail("你的九阴真功不够娴熟，无法使用伏魔。\n");

	if( me->query_temp("weapon"))
		return notify_fail("你拿着武器！\n");

	if (me->query_skill_prepared("cuff") != "dafumo-quan"
	|| me->query_skill_mapped("parry") != "dafumo-quan")
		return notify_fail("你现在无法使用伏魔！\n");

	if (me->query("max_neili") < 6000)
		return notify_fail("你的真气不够，无法使用伏魔。\n");

	if (me->query("neili") < 10000)
		return notify_fail("你的内力不够，无法使用伏魔。\n");

	msg = HIY"\n$N突然眼睛一闪，猛吸一口气，使出全身的力气出拳向$n击去！\n";

	ap = me->query("combat_exp") /1000 * me->query_dex(1);
	dp = target->query("combat_exp") /1000 * target->query_dex(1);

	if( random( ap + dp)  > dp ) {
		msg += HIR"$n躲闪不及，正中小腹！$n只觉得一股强极的九阴真气猛向自己丹田冲击！\n";
		msg += HIR"$n只觉得丹田有如刀割，内息直向体外冲出！\n"NOR;
		target->add("max_neili",-(random(me->query_skill("dafumo-quan",1)/15)+5));
		target->set("neili",0);
		target->set("jiali",0);
		me->set("neili", 0);
		me->set("jiali", 0);
	}
	else
		msg += CYN"$n见来势猛烈，连忙避在一旁。\n" NOR;

	me->start_perform(5, "伏魔");
	message_vision(msg, me, target);
	me->start_busy(random(3));
	return 1;
}

string perform_name(){ return HIY"伏魔"NOR; }

int help(object me)
{
        write(HIY"\n大伏魔拳之「伏魔」："NOR"\n\n");
        write(@HELP
        要求：  当前内力 10000 以上；
                最大内力 6000 以上；
                当前精力 1500 以上；
                大伏魔拳等级 200 以上；
                九阴真功等级 200 以上；
                激发拳法为大伏魔拳；
                激发招架为大伏魔拳；
                备拳法为大伏魔拳；
                且手无兵器。

HELP
        );
        return 1;
}
