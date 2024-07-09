// liaodu.c, 疗毒
// by snowman
// Modified by darken@SJ

#include <ansi.h>

string exert_name(){ return HBRED"疗毒"NOR; }
int exert(object me)
{
        if( me->is_fighting() )
                return notify_fail("战斗中运功？找死吗？\n");

        if ((int)me->query_skill("huagong-dafa", 1) < 200)
                return notify_fail("你的化功大法修为还不够。\n");

        if ((int)me->query_skill("poison", 1) < 200)
                return notify_fail("你的基本毒技修为还不够。\n");

        if ((int)me->query_skill("medicine", 1) < 120)
                return notify_fail("你的本草术理修为还不够。\n");

//        if ((int)me->query_skill("jingmai-xue", 1) < 120)
//                return notify_fail("你的经脉学修为还不够。\n");

        if( (int)me->query("neili") < 5000 )
                return notify_fail("你的真气不够。\n");
      
        message_vision(HIB"$N坐在地上，脸上青一阵白一阵，开始运功疗毒。\n" NOR, me);
        me->clear_conditions_by_type("poison");
        me->add("neili", -3000);
        me->add("max_neili",-random(2));
        me->start_busy(45-random(me->query("con")));
                
        return 1;
}

