// roudao.c 胡家刀法之「柔刀诀」
// Create By lsxk@hsbbs 2007/5/12

#include <ansi.h>

inherit F_SSERVER;
string perform_name() {return HIG"柔刀诀"NOR;}
int perform(object me, object target)
{
        string msg;
        object weapon = me->query_temp("weapon");
        int busy;

        if( !target ) target = offensive_target(me);

         if( !objectp(target)
          ||!target->is_character()
          ||!me->is_fighting(target) )
                return notify_fail("「柔刀诀」只能对战斗中的对手使用。\n");

        if ((int)me->query_skill("hujia-daofa", 1) < 100 )
                return notify_fail("你的胡家刀法不够娴熟，不会使用「柔刀诀」。\n");

        if ((int)me->query_skill("blade", 1) < 100 )
                return notify_fail("你的基本刀法不够娴熟，不会使用「柔刀诀」。\n");

        if ((int)me->query_skill("force", 1) < 100 )
                return notify_fail("你的基本内功不够娴熟，不会使用「柔刀诀」。\n");

        if (!weapon
         || weapon->query("skill_type") != "blade"
         || me->query_skill_mapped("blade") != "hujia-daofa")
//         || me->query_skill_mapped("parry") != "hujia-daofa")
                return notify_fail("你必须使用胡家刀法才能使用「柔刀诀」。\n");

        if( (int)me->query("neili") < 400 )
                return notify_fail("你现在内力太弱，不能使用「柔刀诀」。\n");

        if( target->is_busy())
                return notify_fail(target->name() + "目前正自顾不暇，放胆攻击吧！\n");

        msg = HIG "$N招式一变竟使出胡家刀法之「柔刀诀」!此招一出，$N出招立即变得缓慢，但慢中却带柔劲，刀刃竟不出$n周身半尺！。\n";

        me->add("neili", -200);
        me->add("jingli", -140);
        busy = (int)me->query_skill("hujia-daofa", 1)/30;
        if(random(me->query("combat_exp")) >  target->query("combat_exp")*2/5
           ||random(me->query_skill("force"))>target->query("force")/2 ){
                msg += "结果$p一个大意，竟被$P攻得无法出招，当下暗暗吃惊！！\n" NOR;
                target->start_busy(4+random(4));
                me->start_perform(6,"「柔刀诀」");
               me->set_temp("hjdf/rdj",1);
                call_out("check_fight", 1, me, target, weapon, 7); 
        } else {
                msg += "$p大吃一惊，立即运足轻功逃脱了$P的刀刃范围，但也惊地心神不宁！\n" NOR;
                me->start_busy(2);
                target->add("neili",-600);
                target->apply_condition("no_perform",1);
        }
        message_vision(msg, me, target);
        return 1;
}

void check_fight(object me,object target,object weapon,int count)
{
        if (!target) return;
       if( !me
        || !me->is_fighting(target)
        || weapon != me->query_temp("weapon")
        || me->query_skill_mapped("blade") != "hujia-daofa"
        || count < 1){
                if (me) {
                        me->delete_temp("hjdf/rdj");
                }
                return;
       }
       call_out("check_fight", 1, me,target,weapon,count -1);
}

int help(object me)
{
   write(WHT"\n胡家刀法「"HIG"柔刀诀"WHT"」："NOR"\n");
   write(@HELP
   此「柔刀诀」乃巧用内劲，配合胡家刀法巧妙的招式，攻敌一个措手不及。
   此招乃十分依靠最基本的内功修为，越比敌人深厚，成功率就越高！

   要求：  内力 400 以上；
           胡家刀法 100 以上；
           基本刀法 100 以上；
           基本内功 100 以上；

HELP
   );
   return 1;
}
