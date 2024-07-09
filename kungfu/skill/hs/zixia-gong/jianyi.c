//Create By lsxk@hsbbs 2007/6/14

#include <ansi.h>

inherit F_SSERVER;

string exert_name() {return HIM"紫霞剑意"NOR;}

int exert(object me, object target)
{
        object weapon = me->query_temp("weapon");  
        int skill;
        string jianyi_set;
    
        if( !target ) target = offensive_target(me);

        if(!me->is_fighting() )
                return notify_fail("「紫霞剑意」只能在战斗中使用。\n");

        if(userp(me))
   //     if(me->query("quest/huashan")!="气宗")
  //              return notify_fail("紫霞神功乃华山气宗绝学，你非华山气宗弟子，哪里偷学来的紫霞神功？\n");

        if((int)me->query_skill("zixia-gong", 1) < 250 )
                return notify_fail("你的紫霞神功不够，不能使用「紫霞剑意」。\n");
    
        if((int)me->query_skill("force", 1) < 250 )
                return notify_fail("你的基本内功不够，不能使用「紫霞剑意」。\n");

        if ((int)me->query_skill("huashan-jianfa", 1) < 250 )
                return notify_fail("你的华山剑法不够娴熟，不能真正发挥「紫霞剑意」。\n");
    
        if (!weapon 
         || weapon->query("skill_type") != "sword"
         || me->query_skill_mapped("parry") != "huashan-jianfa"
         || me->query_skill_mapped("sword") != "huashan-jianfa")
                return notify_fail("你现在无法使用「紫霞剑意」。\n");
    
        if ((int)me->query("neili") < 800 )
                return notify_fail("你现在内力太弱，不能使用「紫霞剑意」。\n");

        if ((int)me->query("jingli") < 500 )
                return notify_fail("你现在精力太少，不能使用「紫霞剑意」。\n");

        if ((int)me->query_temp("hsj_junzi"))
                return notify_fail("你正在使用「君子剑法」绝技。\n"); 

        if ((int)me->query_temp("hsj_shunv"))
                return notify_fail("你正在使用「淑女剑法」绝技。\n"); 
    
        if ( (int)me->query_temp("hsj_xiyi") ) 
                return notify_fail("你正在使用「希夷剑法」。\n");

        if ( (int)me->query_temp("zxg/jianyi") ) 
                return notify_fail("你正在使用「紫霞剑意」。\n");

        if (me->query_skill_mapped("force") != "zixia-gong")
                return notify_fail("你的内功有误，无法使用「紫霞剑意」。\n");

        if(me->query("env/紫霞剑意")) jianyi_set = me->query("env/紫霞剑意");

        if(jianyi_set!="君子"
        && jianyi_set!="淑女"
        && jianyi_set!="希夷"
        && jianyi_set!="融合") jianyi_set = "融合";

        if(jianyi_set=="君子"){
            message_vision(HIM"$N暗催紫霞神功内劲附于手中"+weapon->name()+HIM"上，紫霞剑意随心而起，招数凌厉吓人！\n"+
            HIC"$N刹那间领会了以气驭剑之精要，便更将剑意发挥的淋漓尽致，剑招威力大增！\n"NOR,me,target);
            me->start_exert(5,"「紫霞剑意」");
            me->start_perform(6+random(3));
            me->set_temp("zxg/jianyi/君子",1);
            me->add("neili", -400);
            me->add("jingli", -200);

            skill = (int)me->query_skill("huashan-jianfa", 1);
            me->add_temp("apply/damage",  skill);

            call_out("check_fight", 1, me, weapon, skill);
            return 1;
        }
        if(jianyi_set=="淑女"){
            message_vision(HIM"$N暗运紫霞剑意，手中"+weapon->name()+HIM"招数一变，剑招轻盈灵巧之极！\n"+
            "$N手中"+weapon->name()+HIM"随意走，华山剑招一一展开，行云流水，动作之灵巧，实所罕见！\n"NOR,me,target);
            me->start_exert(1,"「紫霞剑意」");
            me->start_perform(1,"「紫霞剑意」");
            me->add("neili", -200);
            me->add("jingli", -100);
            me->set_temp("zxg/jianyi/淑女",1);

            skill = (int)me->query_skill("huashan-jianfa", 1);
            me->add_temp("apply/dexerity",  skill/5);

            call_out("check_fight", 1, me, weapon, skill);
            return 1;
        }
        if(jianyi_set=="希夷"){
            message_vision(HIM"但见$N突然一个变招，剑招虽快，却无凌厉之势，实乃匪夷所思！"+
            HIY"\n$N更运紫霞剑意，剑招竟舞得似一座铜墙铁壁，$n哪有半点可乘之机？！\n"NOR,me,target);
            me->start_exert(1,"「紫霞剑意」");
            me->start_perform(1,"「紫霞剑意」");
            me->add("neili", -200);
            me->add("jingli", -100);
            me->set_temp("zxg/jianyi/希夷",1);

            skill = (int)me->query_skill("huashan-jianfa", 1);
            me->add_temp("apply/armor",  skill/2);
            me->add_temp("apply/parry",  skill/4*3);
            me->add_temp("apply/dodge",  skill/4*3);
            me->add_temp("apply/damage",  -skill/4);

            call_out("check_fight", 1, me, weapon, skill);
            return 1;
        }
        if(jianyi_set=="融合"){
            message_vision(HIM"$N暗运紫霞神功，更将紫霞剑意运用于华山剑法之中，招数使得纯熟之极！"+
            HIW"\n$N暗将华山剑法之君子，淑女，希夷三式剑意融合一气，招式恢弘气扬，剑招竟也具王者气息！\n"NOR,me,target);
            me->start_exert(3+random(2),"「紫霞剑意」");
            me->start_perform(3+random(3),"「紫霞剑意」");
            me->add("neili", -800);
            me->add("jingli", -600);
            me->set_temp("zxg/jianyi/融合",1);

            skill = (int)me->query_skill("huashan-jianfa", 1);
            me->add_temp("apply/damage",  skill/3);
            me->add_temp("apply/armor",  skill/6);
            me->add_temp("apply/parry",  skill/4);
            me->add_temp("apply/dodge",  skill/4);
            me->add_temp("apply/dexerity",  skill/10);

            call_out("check_fight", 1, me, weapon, skill);
            return 1;
        }
}

void check_fight(object me, object weapon,int skill)
{  
        object wep;

        if (!me) return;
        wep = me->query_temp("weapon");  
        if( !me->is_fighting()
         || !living(me)
         || me->is_ghost()
         || !wep
         ||me->query_skill_mapped("parry") != "huashan-jianfa"
         ||me->query_skill_mapped("sword") != "huashan-jianfa"
         || weapon != wep ){
             if(me->query_temp("zxg/jianyi/君子")){
                 me->add_temp("apply/damage",  -skill);
                 me->delete_temp("zxg/jianyi");
             }
             else if(me->query_temp("zxg/jianyi/淑女")){
                 me->add_temp("apply/dexerity",  -skill/5);
                 me->delete_temp("zxg/jianyi");
             }
             else if(me->query_temp("zxg/jianyi/希夷")){
                 me->add_temp("apply/armor",  -skill/2);
                 me->add_temp("apply/parry",  -skill/4*3);
                 me->add_temp("apply/dodge",  -skill/4*3);
                 me->add_temp("apply/damage",  skill/4);
                 me->delete_temp("zxg/jianyi");
             }
             else if(me->query_temp("zxg/jianyi/融合")){
                 me->add_temp("apply/damage",  -skill/3);
                 me->add_temp("apply/armor",  -skill/6);
                 me->add_temp("apply/parry",  -skill/4);
                 me->add_temp("apply/dodge",  -skill/4);
                 me->add_temp("apply/dexerity",  -skill/10);
                 me->delete_temp("zxg/jianyi");
             }
             else{
                 message_vision(HIR"程序出错，请立即通知在线wiz!\n"NOR, me);
             }
           if(living(me) && !me->is_ghost())
              message_vision(HIM"$N紫霞剑意用毕，凝神收式，四周弥漫的剑气渐渐消失。\n"NOR, me);
           return;
        } 
        call_out("check_fight", 1, me, weapon, skill);
 return ;
}

int help(object me)
{
    write(YEL"\n紫霞神功「"HIM"紫霞剑意"YEL"」："NOR"\n");
    write(@HELP

    要求：紫霞神功等级 250 以上；
          基本内功等级 250 以上；
          华山剑法等级 250 以上；
          当前内力 800 以上；
          当前精力 500 以上。

HELP
    );
    return 1;
}
