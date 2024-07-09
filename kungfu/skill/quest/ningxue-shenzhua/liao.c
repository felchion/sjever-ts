// liao.c  凝血神爪之「疗字诀」
// Create By lsxk@hsbbs 2007/5/13 
#include <ansi.h>
#include <combat.h>

inherit F_SSERVER;

string *xue_name = ({"大锥穴","百会穴","志室穴","肩井穴","劳宫穴","内关穴","外关穴","会宗穴",
                     "曲池穴","冲阳穴","臂儒穴","白海穴","灵台穴","风府穴","神风穴","玉书穴",
                     "青门穴","商阳穴","丝空竹穴","气海穴","冲门穴","大椎穴","百里穴","槐阳穴", });

string perform_name() {return HIY"疗字诀"NOR;}

 int perform(object me, object target)
 {
         int j;
         string msg,xuedao;
         j = me->query_skill("ningxue-shenzhua", 1);

         if( !target ) target = offensive_target(me);

         if( !target 
           || target->is_fighting()
           || me->is_fighting()
           || !living(target)
           || environment(target)!= environment(me))
                 return notify_fail("你不能在战斗中使用,或者对在战斗中的玩家使用「疗字诀」。\n");

         if( target->is_busy() )
                 return notify_fail("对方正忙着呢！\n");

         if( me->is_busy() )
                 return notify_fail("你正忙着呢！\n");

         if( !target->query_condition("nxsz_hurt") )
                 return notify_fail("对方未曾受到凝血伤害，你要使用「疗字诀」做甚?\n");

         if( objectp(me->query_temp("weapon")) )
                 return notify_fail("你必须空手才能使用「疗字诀」！\n");

         if( (int)me->query_skill("ningxue-shenzhua", 1) < 200 )
                 return notify_fail("你的凝血神爪不够娴熟，使不出「疗字诀」绝技。\n");

         if( (int)me->query_skill("force", 1) < 300 )
                 return notify_fail("你基本内功不够，使不出「疗字诀」绝技。\n");

         if( (int)me->query_con() < 35 )
                return notify_fail("你在根骨不足，无法使用「疗字诀」绝技!\n");

         if( (int)me->query_skill("medicine", 1) < 120 )
                 return notify_fail("你对本草术理的研究还甚少，使不出「疗字诀」绝技。\n");

         if (me->query_skill_prepared("claw") != "ningxue-shenzhua"
          || me->query_skill_mapped("claw") != "ningxue-shenzhua" )
                 return notify_fail("你要全面运用凝血神爪功夫才能使出「疗字诀」绝技！\n");

         if( (int)me->query("max_neili") < 3000)
                 return notify_fail("你内力修为太差，无法使用「凝血诀」绝技。\n");

         if( (int)me->query("neili") < 2500 )
                 return notify_fail("你现在真气太弱，无法使用「凝血诀」绝技。\n");
         
         xuedao = xue_name[random(sizeof(xue_name))];

         msg = HIY "\n$N将左掌贴于$n左胸前，右掌紧贴$n脐处，暗运内功，将$n所受之「"HIR"凝血内伤"HIY+"」缓缓化去......\n"NOR;

         message_vision(msg, me, target);

         target->apply_condition("nxsz_hurt", -j/50);
         me->add("neili", -2000);
         me->start_busy(random(5)+4);
         target->start_busy(random(5)+4);
         
         me->start_perform(random(4),"「凝血诀」");
         return 1;
 }

int help(object me)
{
   write(WHT"\n凝血神爪「"HIY"疗字诀"WHT"」："NOR"\n");
   write(@HELP
   此「疗字诀」是专门用来治疗身受凝血神爪伤害的人。

   要求：  内力 2500 以上；
           最大内力 3000 以上； 
           后天根骨 35 以上； 
           凝血神爪等级 200 以上；
           基本内功等级 300 以上；
           本草术理等级 120 以上；
           激发爪法为凝血神爪；
           备爪法且手无兵器。

HELP
   );
   return 1;
}
