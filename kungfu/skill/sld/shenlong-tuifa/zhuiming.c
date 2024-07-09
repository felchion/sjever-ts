 // zhuiming.c    神龙追命腿
 // 98.9.18  by Aug    99.9.11 changed by aug
 // modified by olives@SJ
//update by lsxk@hsbbs 2007/6/6

#include <ansi.h>
#include <combat.h>
 inherit F_SSERVER;

string perform_name() {return HIY"神龙追命腿"NOR;}

#define TICKTIME 5

 void exam1(object me , object tartget );

 int perform(object me, object target)
 {
         if( !target ) target = offensive_target(me);

      if( !objectp(target) || !target->is_character() || !me->is_fighting(target) )
                 return notify_fail("「神龙追命腿」只能对战斗中的对手使用。\n");

         if((int)me->query_skill("shenlong-tuifa", 1) < 150 )
                 return notify_fail("你的神龙腿法不够娴熟，不会使用「神龙追命腿」。\n");

         if((int)me->query_skill("dulong-dafa", 1) < 150 )
                 return notify_fail("你的毒龙大法不够深厚，不会使用「神龙追命腿」。\n");

         if((int)me->query("neili") < 1000)
                 return notify_fail("你现在内力太弱，不能使用神龙追命腿。\n");

         if((int)me->query("jingli") < 500)
                 return notify_fail("你现在精力太弱，不能使用神龙追命腿。\n");

         if( me->query_temp("weapon")) 
                 return notify_fail("你拿着兵器无法使用神龙追命腿。\n");

         if((int)me->query_temp("zhuiming/time")>0)
                 return notify_fail("你正在使用神龙追命腿呢。\n");

         if(me->query_skill_mapped("force") != "dulong-dafa")
                 return notify_fail("你现在所使用的内功同毒龙大法向抵触，不能使用「神龙追命腿」。\n");

         me->add("neili" , -200);
         me->add("jingli" , -80);
         me->set_temp("zhuiming/target",target);
         me->set_temp("zhuiming/time",time());
         me->set_temp("no_fight", 1);
         me->apply_condition("no_perform", 100);
         message_vision(HIY+"\n$N"+HIY+"陡然长啸一声，双腿飞快的移行换位，却不发一腿，似乎在积聚内力以使出绝技－－神龙追命腿！\n\n"NOR,me);
                 
         call_out("exam1",TICKTIME,me,target);

         return 1;
 }

 void exam1(object me , object target )
 {
         string str;     
         int  times,dis,i;

         if(!target
         || !me
         || !me->is_fighting(target)
         ||  environment(target) != environment(me)
         || !living(target)
         || target->is_ghost()
         || me->is_ghost()
         || !(dis = (int)me->query("env/zhuiming")))
         {
                 message_vision(HIY+"\n$N"+HIY+"慢慢收回聚集的内力。\n"NOR,me);
                 me->clear_condition("no_perform");
                 me->delete_temp("zhuiming");
                 me->delete_temp("no_fight");
                 me->start_perform(6,"「神龙追命腿」");
                 return;
         }
         dis = (dis>60?60:(dis<5?5:dis));
         times = time()-me->query_temp("zhuiming/time",1);
         if( me->query_skill_mapped("leg") != "shenlong-tuifa"
          || me->query_skill_prepared("leg") != "shenlong-tuifa"
          || me->query_temp("weapon")){
                 me->clear_condition("no_perform");
                 me->delete_temp("zhuiming");
                 me->delete_temp("no_fight");
                 me->start_perform(6,"「神龙追命腿」");
                 tell_object(me,"你决定不用神龙腿法攻击，「神龙追命腿」也自动解除。\n");
                 return;
         }
         if( target 
          && me->is_fighting(target)
          && (times < dis)
          && me->query_temp("zhuiming")){
                 str = HIY+"\n$N"+HIY+"已经聚气"+CHINESE_D->chinese_number(times)+"秒钟，$N双腿快速的移动卷起一团团黄土，似乎在脚下聚集起一团";
                 if( times < 20)  str +="小小的气团。"NOR;
                   else if( times < 40) str +="充满真气的气团。"NOR;
                       else if( times <= 50) str +="正在不断变大的大气团！"NOR;
                          else str +="显示着逼人锋芒的巨大气团！"NOR;
                 message_vision(str+"\n",me);
                 call_out("exam1",TICKTIME,me,target);
         }
         else if(times>=dis)
         {
                 message_vision(HIY"\n$N聚气完毕，瞬间踢出了"+chinese_number(times/5)+"腿，快的几乎让人无法想象！\n"NOR,me);
                 for(i=0;i<times/5-1;i++){
                   if(!random(3)) target->set_temp("must_be_hit",1);
                         COMBAT_D->do_attack(me,target,0,TYPE_SUPER);
                 target->delete_temp("must_be_hit");
             }
                 me->set_temp("sld/pfm/zhuiming",1);
                 target->set_temp("must_be_hit",1);
                 if(dis>30)
                         COMBAT_D->do_attack(me,target,0,TYPE_SUPER);
                 else
                         COMBAT_D->do_attack(me,target,0,TYPE_REGULAR);
                 target->delete_temp("must_be_hit");
                 me->delete_temp("sld/pfm/zhuiming");
                 me->clear_condition("no_perform");
                 me->delete_temp("zhuiming");
                 me->delete_temp("no_fight");
                 me->start_perform(6,"「神龙追命腿」");
                 return;
         }
         
 }

int help(object me)
{
   write(WHT"\n神龙夺命腿「"HIY"神龙追命腿"WHT"」："NOR"\n");
   write(@HELP

    要求：当前内力 1000 以上；
          当前精力 500 以上；
          神龙夺命腿等级 150 以上；
          毒龙大法等级 150 以上；
          set zhuiming xxx 来设定蓄力时间，最多60秒。
HELP
   );
   return 1;
}
