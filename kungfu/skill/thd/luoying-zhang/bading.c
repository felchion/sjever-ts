// bading.c by Darken@Sj

inherit F_SSERVER;
#include <ansi.h>

string perform_name() { return HIG"附骨钉、拔"NOR; }

int perform(object me, object target)
{
        string msg;
        
        if(!objectp(target))
                return notify_fail("这里没有这个人。\n");
        if(!target->query("thd/fugu"))
                return notify_fail("这个人没有中附骨钉。\n");
        if( (int)me->query_skill("bihai-chaosheng", 1) < 250 )
                return notify_fail("你的碧海潮生功修为未到，无法使用附骨钉。\n");
        if( (int)me->query_skill("luoying-zhang",1) < 250 )
                return notify_fail("你的掌法修为不足。\n");
        if( (int)me->query("neili") < 2000 )
                return notify_fail("你现在内力太弱。\n");

        me->add("neili",-2000);
        me->add("max_neili",-30);
        msg = HIY"$N走到$n身後，伸出手来。\n"NOR;
        msg+= HIW"$N运起碧海潮生功，缓缓的将$n所中附骨钉拔出。\n"NOR;
        if( target->query_con() > 30) {
                msg+= HIC"$n惨叫一声，内功修为大大受损。\n"NOR;
                target->add("max_neili",-5);
        } else {
                msg+= HIR"$n惨叫一声，晕了过去。\n"NOR;
                target->unconcious();
                target->add("max_neili",-5);
        }
        target->add("thd/fugu",-1);
        message_vision(msg, me, target);
        return 1;
}

int help(object me)
{
        write(HIG"\n「附骨钉、拔」："NOR"\n");
        write(@HELP

        反运碧海潮生功，帮助受附骨钉之害者起出附骨钉。若是根骨不足或是内功
        不够深厚，多半会损失修炼的内力。

        要求：  碧海潮生功等级 250 以上；
                落英神剑掌等级 250 以上；
                最大内力 2000 以上；
                非战斗方能使用。
HELP
        );
        return 1;
}


