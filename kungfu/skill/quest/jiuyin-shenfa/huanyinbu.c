#include <ansi.h>
#include <skill.h>
#include <combat.h>

inherit F_SSERVER;

int perform(object me)
{
	string msg;
	int i;

	if( me->query_skill("jiuyin-zhengong",1) < 130 )
		return notify_fail("你的九阴真功还不够火候，使不出幻阴步。\n");  

	if( me->query_skill("jiuyin-shenfa",1) < 130 )
		return notify_fail("你的九阴身法还不够火候，使不出幻阴步。\n");  

	if (me->query_skill_mapped("dodge") != "jiuyin-shenfa")
		return notify_fail("你现在无法使用幻阴步。\n");

	if( me->query("jingli", 1) < 200 )
		return notify_fail("你现在精力不够。\n");

	if( me->query("neili", 1) < 200 )
		return notify_fail("你现在内力不够。\n");

	if (me->query_temp("jiuyin/fast"))
		return notify_fail("你正在使用幻阴步。\n");

	if ( me->query_skill_mapped("dodge") != "jiuyin-shenfa")
		return notify_fail("你现在无法使用幻阴步。\n");

	msg = HIW"\n$N施展出幻阴步，身法变得飘忽异常！\n"NOR; 

	me->add("neili", -100);
	me->add("jingli", -100);
	message_vision(msg, me);
	i = me->query_skill("jiuyin-shenfa", 1)/5;
	me->add_temp("apply/pfm_dodge", 120);
	me->add_temp("apply/pfm_parry", 120);
// 启动 auto perform
	me->set_temp("jiuyin/fast", i);
	if ( me->is_fighting())
		me->start_busy(random(2));
	call_out("remove_effect", 1, me, me->query_skill("jiuyin-zhengong", 1)/3);
	return 1;
}

void remove_effect(object me, int count)
{
	int i;
	if (!me) return;
	if ( me->query_skill_mapped("dodge") != "jiuyin-shenfa"
	|| count-- < 1 ){
		i = me->query_temp("jiuyin/fast");
		me->add_temp("apply/pfm_dodge", - 120);
		me->add_temp("apply/pfm_parry", - 120);
		me->delete_temp("jiuyin/fast");
		message_vision(HIW"$N停止了幻阴步，身法恢复了正常。\n"NOR, me);
		return;
	}
	call_out("remove_effect", 1, me, count);
}

string perform_name(){ return HIW"幻阴步"NOR; }

int help(object me)
{
        write(HIW"\n九阴身法之「幻阴步」："NOR"\n\n");
        write(@HELP
        要求：  当前内力 200 以上；
                当前精力 200 以上；
                九阴真功等级 130 以上；
                九阴身法等级 130 以上；
                激发身法为九阴身法。

HELP
        );
        return 1;
}
