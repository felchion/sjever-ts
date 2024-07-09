// feidao.c   胡家刀法之「飞刀诀」.
// Create by lsxk@hsbbs 2007/5/11
#include <ansi.h>

inherit F_SSERVER;
private int remove_effect(object me, int amount);
string perform_name() {return HIC"飞刀诀"NOR;}
int perform(object me)
{
        object weapon = me->query_temp("weapon");  
        int skill;
    
        if(!me->is_fighting() )
                return notify_fail("「飞刀诀」只能在战斗中使用。\n");

        if ((int)me->query_skill("hujia-daofa", 1) < 100 )
                return notify_fail("你的胡家刀法不够娴熟，还用不来「飞刀诀」。\n");
    
        if (!weapon 
         || weapon->query("skill_type") != "blade"
         || me->query_skill_mapped("parry") != "hujia-daofa"
         || me->query_skill_mapped("blade") != "hujia-daofa")
                return notify_fail("你的武功运用不对，无法使用「飞刀诀」。\n");
    
        if ((int)me->query("neili") < 400 )
                return notify_fail("你现在内力太弱，不能使用「飞刀诀」。\n");

        if ((int)me->query("jingli") < 400 )
                return notify_fail("你现在精力太少，不能使用「飞刀诀」。\n");

        if ((int)me->query_temp("hjdf/fdj"))
                return notify_fail("你正在使用「飞刀诀」绝技。\n"); 


        message_vision(HIC "$N立时一声清喝，手中"+weapon->name()+HIC"招数一变,使出胡家刀法之「飞刀诀」绝技！\n"+
        "胡家刀法讲究的是一个“慢”字，待敌人出招后，寻敌人破绽而攻之，$N这路刀法使将出来，动作已然缓慢下来。\n"+
        "而以后每出一招便是攻向对手破绽之处，刀法之妙，乃绝千古也！\n"NOR, me);

        if (me->query("quest/雪山飞狐/武功/lengquanshengong_teach")
        && me->query_temp("lqsg/lq")
        && me->query_skill_mapped("force") == "lengquan-shengong")
            message_vision(HIW"在$N的冷泉神功之「"HIB"冷泉"HIW"」绝技配合下，手中"+weapon->name()+HIW"隐隐地泛着白色。\n"NOR, me);
        me->start_perform(random(3),"「飞刀诀」");
        me->add("neili", -200);          
        me->add("jingli", -100);
    
        skill = me->query_skill("hujia-daofa", 1);
        me->add_temp("apply/damage",  skill/4);
        me->add_temp("apply/attack",  skill/4);
        me->set_temp("hjdf/fdj", 1);

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
         || me->query_skill_mapped("parry") != "hujia-daofa"
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
        me->delete_temp("hjdf/fdj");
        if(living(me) && !me->is_ghost())
           message_vision(HIW"$N一套「飞刀诀」使完，动作又恢复了正常状态。\n"NOR, me);
        return 0;
}

int help(object me)
{
   write(WHT"\n胡家刀法「"HIC"飞刀诀"WHT"」："NOR"\n");
   write(@HELP
   此「飞刀诀」虽为胡家刀法之初级招试，但却秉承了胡家刀法
   的精髓“以慢打快，寻敌破绽，后发制人”，使用时，能短时
   间内提高自身攻击力和命中率，但这并不是全部，只有当对手
   成功发动攻击后，才能真正显示出该招的威力！另若使用者会
   武林绝学冷泉神功之「冷泉」绝技，则相辅相成，威力惊人！

   要求：  内力 400 以上；
           精力 400 以上； 
           胡家刀法 100 以上；
           基本刀法 100 以上；
           激发胡家刀法为招架；

HELP
   );
   return 1;
}
