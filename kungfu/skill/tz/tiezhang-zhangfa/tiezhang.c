// by darken@SJ
// modified by Olives@SJ 5/4/2001
// 解决连续死的bug

#include <ansi.h>
#include <combat.h>
inherit F_SSERVER;
#include <combat_msg.h>

int query_ap() {return 8;}

string perform_name(){ return HBBLU"铁掌"NOR; }
int perform(object me, object target)
{
        int p, j;
        string msg;
        j = me->query_skill("tiezhang-zhangfa", 1);

        if( !target ) target = offensive_target(me);

        if( !objectp(target)
          || !me->is_fighting(target)
          || !living(target)
          || environment(target)!= environment(me))
                return notify_fail("「铁掌」只能在战斗中对对手使用。\n");

        if( objectp(me->query_temp("weapon")) )
                return notify_fail("你必须空手才能使用「铁掌」！\n");

        if( (int)me->query_skill("tiezhang-zhangfa", 1) < 350 )
                return notify_fail("你的铁掌不够娴熟，使不出铁掌绝技。\n");

        if( (int)me->query_skill("guiyuan-tunafa", 1) < 350 )
                return notify_fail("你归元吐纳法不够，使不出铁掌绝技。\n");

        if( (int)me->query_skill("shuishangpiao", 1) < 350 )
                return notify_fail("你水上飘等级不够，使不出铁掌绝技。\n");

        if( (int)me->query("jiali") < 200 )
                return notify_fail("你加力不够，使不出铁掌绝技。\n");

        if (me->query_skill_mapped("force") != "guiyuan-tunafa")
                return notify_fail("你所使用的内功不对。\n");


        if (me->query_skill_prepared("strike") != "tiezhang-zhangfa"
         || me->query_skill_mapped("strike") != "tiezhang-zhangfa"
         || me->query_skill_mapped("parry") != "tiezhang-zhangfa"
         || me->query_temp("weapon"))
                return notify_fail("你现在无法使用「铁掌」！\n");

        if( (int)me->query("max_neili") < 7000)
                return notify_fail("你现在内力太弱，使不出「铁掌」。\n");

        if( (int)me->query("neili") < 3000 )
                return notify_fail("你现在真气太弱，使不出「铁掌」。\n");

        j *= 10;
        j += me->query("jiali")*10;
        j -= target->query_con()*10;
        j = j*2/3 + random(j/3);

        msg = HBBLU "\n$N将毕生功力聚集于掌上，丝毫无任何花巧，平平的向$n慢慢推去。\n"NOR;
        msg+= HBBLU "$n全身四周已全被这掌力封住，只得硬碰硬挡下这一招。\n\n"NOR;
        
        if (COMBAT_D->do_attack(me, target, 0, 0, 1) || COMBAT_D->do_attack(me, target, 0, 0, 1))
        {
                msg += HBRED"$n当场被$N这一掌打成血肉模糊的一团！\n"NOR;
                me->kill_ob(target);
                target->set_temp("last_damage_from", me);
                target->set("eff_qi",-1);
                target->set("qi",-1);
                target->receive_wound("qi", 100, me);
                target->receive_damage("qi",100, me);
        }
         else
        {
                target->receive_wound("qi", j, me);
                target->receive_damage("qi",j, me);
                msg += HBRED"只听$n一声惨嚎，想必受了极重的伤！\n"NOR;
        }
        p = (int)target->query("qi")*100/(int)target->query("max_qi");
        msg += "( $n"+eff_status_msg(p)+" )\n";
        me->add("neili", -2500-random(500));
        message_vision(msg, me, target);
        me->start_busy(1);
        return 1;
}

int help(object me)
{
        write(HBBLU"\n铁掌掌法之「铁掌」："NOR"\n\n");
        write(@HELP
        要求：  当前内力 3000 以上；
                归元吐呐法等级 350 以上；
                水上漂等级 350 以上；
                铁掌掌法等级 350 以上；
                激发掌法为铁掌掌法；
                且备铁掌掌法。

HELP
        );
        return 1;
}