// luandao.c   胡家刀法之「乱刀诀」.
// Create by lsxk@hsbbs 2007/5/11
#include <ansi.h>

inherit F_SSERVER;
private int remove_effect(object me, int amount);
string perform_name() {return HIR"乱刀诀"NOR;}
int perform(object me)
{
        object weapon = me->query_temp("weapon");  
        int skill;
    
        if(!me->is_fighting() )
                return notify_fail("「乱刀诀」只能在战斗中使用。\n");

        if ((int)me->query_skill("hujia-daofa", 1) < 200 )
                return notify_fail("你的胡家刀法不够娴熟，还用不来「乱刀诀」。\n");
    
        if (!weapon 
         || weapon->query("skill_type") != "blade"
//         || me->query_skill_mapped("parry") != "hujia-daofa"
         || me->query_skill_mapped("blade") != "hujia-daofa")
                return notify_fail("你的武功运用不对，无法使用「乱刀诀」。\n");
    
        if ((int)me->query("neili") < 800 )
                return notify_fail("你现在内力太弱，不能使用「乱刀诀」。\n");

        if ((int)me->query("jingli") < 800 )
                return notify_fail("你现在精力太少，不能使用「乱刀诀」。\n");

        if ((int)me->query("max_neili") < 2500 )
                return notify_fail("你现在的内力修为不够，不能使用「乱刀诀」。\n");

        if ((int)me->query_temp("hjdf/ldj"))
                return notify_fail("你正在使用「乱刀诀」绝技。\n"); 


        message_vision(HIY "$N顿时身形变快，手中"+weapon->name()+HIY"蓄势待发,直等对手防范大意时,克敌制胜,此乃正是胡家刀法之「乱刀诀」绝技！\n"NOR,me);

        if (me->query("quest/雪山飞狐/武功/lengquanshengong_teach")
        && me->query_temp("lqsg/lq")
       && me->query_skill_mapped("force") == "lengquan-shengong")
            message_vision(HIB"在$N的冷泉神功之「"HIW"冷泉"HIB"」绝技配合下，手中"+weapon->name()+HIB"隐隐地泛着蓝色。\n"NOR, me);
        me->start_perform(random(4)+1,"「乱刀诀」");
        me->add("neili", -400);
        me->add("jingli", -200);
    
        skill = me->query_skill("hujia-daofa", 1);
        me->add_temp("apply/damage",  skill/4);
        me->add_temp("apply/attack",  skill/4);
        me->set_temp("hjdf/ldj", 1);

        me->start_call_out( (: call_other, __FILE__, "check_fight", me, skill, weapon :), 1);
        return 1;
}

void check_fight(object me, int amount, object weapon)
{  
        object wep;

        if (!me) return;
        wep = me->query_temp("weapon");  
        if(!me->is_fighting()
         || !living(me)
         || me->is_ghost()
         || !wep
         || weapon != wep )
           remove_effect(me, amount);
        else call_out("check_fight", 1, me, amount, weapon);
}

private int remove_effect(object me, int amount)
{         
        int skill;
        skill = (int)me->query_skill("hujia-daofa", 1);
 
        me->add_temp("apply/damage",  -skill/4);
        me->add_temp("apply/attack",  -skill/4);
        me->delete_temp("hjdf/ldj");
        if(living(me) && !me->is_ghost())
           message_vision(HIG"$N一套「乱刀诀」使完，动作又恢复了正常。\n"NOR, me);
        return 0;
}

int help(object me)
{
   write(WHT"\n胡家刀法「"HIR"乱刀诀"WHT"」："NOR"\n");
   write(@HELP
   此「乱刀诀」乃为胡家刀法之中级招试，刀法深藏不露，只待
   对手大意时，给于对手以沉重打击，从而克敌制胜！使用该招
   时，也会短时间内提高使用者的攻击力和命中率，厉害非常！
   若使用者会武林绝学冷泉神功之「冷泉」绝技，则相辅相成，
   威力惊人！

   要求：  内力 800 以上；
           精力 800 以上；
           最大内力 2500 以上；
           胡家刀法 200 以上；
           基本刀法 200 以上；

HELP
   );
   return 1;
}
