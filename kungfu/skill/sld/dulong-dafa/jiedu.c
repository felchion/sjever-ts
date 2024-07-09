// jd.c 
// by emnil   98.10.4

#include <ansi.h>
inherit F_CLEAN_UP;

string exert_name() {return HIW"解毒"NOR;}

int exert(object me, object target)
{
        int skill;

        if(target != me && !present(target,environment(me)))
                return notify_fail("这里好象没有这个人？\n");

        if((int)target->query("huagu") < 200 && target != me)
                return notify_fail(target->name()+"没有中化骨绵掌毒。\n");

        if((int)target->query("huagu") < 200)
                return notify_fail("你没有中化骨绵掌毒。\n");

        if((int)me->query_skill("dulong-dafa",1) < 150  )
                return notify_fail("你的毒龙大法功力不够，无法解化骨绵掌毒。\n");

        if((int)me->query_skill("shenlong-yaoli",1) < 150  )
                return notify_fail("你对神龙药理学的领悟尚缺，无法解化骨绵掌毒。\n");

        if((int)me->query("neili") < 500
         || (int)me->query("max_neili")< 100  )
                return notify_fail("你的内力不够。\n");

        skill= (me->query_skill("dulong-dafa",1)/2 + me->query_skill("shenlong-yaoli",1))*14;
        if((int)target->query("huagu") > skill)
            target->add("huagu",-skill);
        else
            target->delete("huagu");
        if( target != me)
          message_vision(HIW"\n$N伸手抵住$n的双掌，慢慢将$n身上的毒素逼出体外。\n\n"NOR,me,target);
        else 
          message_vision(HIW"\n$N满头大汗，用劲将身体内的毒素逼出体外。\n\n"NOR, me);
        target->clear_condition("huagu_poison");
        me->add("max_neili",-10);
        me->start_busy(5);
        if( target != me) target->start_busy(5);
        me->add("neili",-400);
        return 1;
}
