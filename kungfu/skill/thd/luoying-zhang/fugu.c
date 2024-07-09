// fugu.c by Darken@Sj

inherit F_SSERVER;
#include <ansi.h>

string perform_name() { return HIG"附骨钉、施"NOR; }

int perform(object me, object target)
{
        string msg;
        
        if( !target ) target = offensive_target(me);

        if( !objectp(target))
                return notify_fail("这里没有这个人。\n");
        if(target==me)
                return notify_fail("你不能攻击自己。\n");
        if( target->query("combat_exp")< 100000)
                 return notify_fail("你的心肠太黑了，对手无缚鸡之力的玩家也下得了手？\n");
        if( me->query_temp("weapon"))
                return notify_fail("你拿着武器怎么能使用附骨钉？\n");   
        if( environment(me)->query("no_fight") )
                return notify_fail("这里不能战斗。\n");
        if( (int)me->query_skill("bihai-chaosheng", 1) < 200 )
                return notify_fail("你的碧海潮生功修为未到，无法使用附骨钉。\n");
        if( (int)me->query_skill("luoying-zhang",1) < 200 )
                return notify_fail("你的掌法修为不足。\n");
        if( (int)me->query_skill("throwing",1) < 200 )
                return notify_fail("你的暗器修为不足。\n");
        if( (int)me->query("neili") < 2000 )
                return notify_fail("你现在内力太弱。\n");
        if( (int)me->query("neili") < 1000 )
                return notify_fail("你现在精力太弱。\n");
        msg = HIY"$N慢慢的走到$n身後，伸出手来。\n"NOR;
        if (random(me->query_dex()) > target->query_int()/2) {
                target->add("thd/fugu",1);
                msg+= HIR"$N突然手轻轻的在$n背後拍了一下，$n觉得好像被什么刺了一下。\n"NOR;
                me->start_busy(3);
        } else {
                me->start_busy(5);
                msg += CYN"$n看$N神色诡异，快步走了开去。\n"NOR;
        }
        message_vision(msg, me, target);
        me->add("neili",-1000);
        me->add("jing",-500);
        return 1;
}

int help(object me)
{
        write(HIG"\n「附骨钉、施」："NOR"\n");
        write(@HELP

        以桃花岛特制独行的细小钢钉，用落英神剑掌的速度配合碧海潮生功的内劲
        ，植入他人体内。当受到碧海潮生功内劲催动时，毒性就会发作。

        要求：  碧海潮生功等级 200 以上；
                落英神剑掌等级 200 以上；
                基本暗器等级 200 以上；
                最大内力 2000 以上；
                非战斗方能使用。
HELP
        );
        return 1;
}


