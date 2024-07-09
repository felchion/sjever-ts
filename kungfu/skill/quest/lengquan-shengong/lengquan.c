// lengquan.c  冷泉神功之「冷泉」.
// Create by lsxk@hsbbs 2007/5/11
// By Spiderii@ty 降低效果,威力太强了，比9yin都猛
// 特意上sj test测试了下.lsxk的效果跨行了些.lqsg定位为 中级quest
#include <ansi.h>

inherit F_SSERVER;
private int remove_effect(object me, int amount);
int exert(object me)
{
//        object weapon = me->query_temp("weapon");  
        int skill;
    
         if ((int)me->query_skill("lengquan-shengong", 1) < 200 )
                return notify_fail("以你目前冷泉神功的修为，实乃难以理解高深奥秘的「冷泉」绝技！\n");

        if ((int)me->query_skill("force", 1) < 200 )
                return notify_fail("冷泉神功的「冷泉」绝技乃需要极其深厚的内功做基础，你的基本内功还尚欠火候！\n");
    
        if (me->query_skill_mapped("force") != "lengquan-shengong" )
                return notify_fail("你现在能使得了「冷泉」绝技吗？\n");
    
        if ((int)me->query("neili") < 2000 )
                return notify_fail("你这么少的内力，如何能随心所欲的驾驭「冷泉」绝技？\n");

        if ((int)me->query("jingli") < 1500 )
                return notify_fail("你这么少的精力，如何能随心所欲的驾驭「冷泉」绝技？\n");

        if ((int)me->query("max_neili") < 4000 )
                return notify_fail("就你目前的内力修为想使用「冷泉」绝技，小心走火入魔而死！\n");

        if ((int)me->query("max_jingli") < 2500 )
                return notify_fail("就你目前的精力修为想使用「冷泉」绝技，小心走火入魔而死！\n");

        if ( me->query_temp("lqsg/lq") )
                return notify_fail("你现在正在使用「冷泉」绝技！\n");

        message_vision(HIW "$N当下正遇劲敌，好不痛快！便连连催动冷泉神功内劲，竟使出「"HIB"冷泉"HIW"」绝技！\n"+
        "$N数十载苦练的冷泉神功使将出来，周身竟被一股厚厚的白雾紧紧包围！\n"+
        "梢有武学常识的人，一看便知，这招乃内家功夫修炼到极高境界后产生的护体真\n"+
        "气，可冷泉神功乃当真当今奇功，再加上$N数十年如一日地苦练，这白雾竟然有\n"+
        "一堵墙之厚，可当真刀枪不入！！而此时，$N更将自己数十载所习武功全部融合\n"+
        "进来，招式浑然融为一体，欲攻便配合冷泉绝技，布内力于四肢及器械，劲能开\n"+
        "山！若守，更将冷泉绝技内劲散布周身，便如铜墙铁壁，哪有半点破绽可寻？\n"NOR,me);

        me->start_exert(random(3)+1,"「冷泉」");
        me->add("neili", -800); 
        me->add("jingli", -500);
    
        skill = (int)me->query_skill("lengquan-shengong", 1) / 5;
        me->add_temp("apply/attack",  skill);
        me->add_temp("apply/armor",  skill);
        me->add_temp("apply/dodge",  skill);
        me->add_temp("apply/damage", skill);
       // me->add_temp("apply/dexerity",skill/10);
       // me->add_temp("apply/strength",skill/10);
       // me->add_temp("apply/constitution",skill/10);
        me->set_temp("lqsg/lq", 1);

        me->start_call_out( (: call_other, this_object(), "remove_effect", me :), skill);
        return 1;
}



private int remove_effect(object me, int amount)
{         
        int skill,i;
        skill = (int)me->query_skill("lengquan-shengong", 1) / 5;
 
        me->add_temp("apply/attack",  -skill);
        me->add_temp("apply/armor",  -skill);
        me->add_temp("apply/dodge",  -skill);
        me->add_temp("apply/damage", -skill);
      //  me->add_temp("apply/dexerity",-skill/10);
      //  me->add_temp("apply/strength",-skill/10);
      //  me->add_temp("apply/constitution",-skill/10);
        me->delete_temp("lqsg/lq");
        if(living(me) && !me->is_ghost())
          message_vision(HIW"$N的冷泉神功之「"HIB"冷泉"HIW"」绝技使毕，只觉得气血通畅，舒服无比！\n"NOR, me);
        if(me->query_skill_mapped("force") != "lengquan-shengong") return 0;
        if(living(me) && !me->is_ghost() && random((int)me->query("kar"))>25 && !random(3)){
          if (random(2)==0){
              me->add("max_neili",random(2)+1);
              tell_object(me,HIC"你「冷泉」绝技使完后，只觉得神清气爽，内劲无比充沛，你的内力修为无意间增加了！\n"NOR);
              }
          else{
              i = random(2)+1;
              me->add("max_jingli",i);
              me->add("eff_jingli",i);
              tell_object(me,HIC"你「冷泉」绝技使完后，只觉得神清气爽，内劲无比充沛，你的精力修为无意间增加了！\n"NOR);
              }
        }
        return 0;
}

int help(object me)
{
   write(WHT"\n冷泉神功「"HIB"冷泉"WHT"」："NOR"\n");
   write(@HELP
   冷泉神功乃当世内功一绝,非常人能道也!此「冷泉」绝技是其
   最深奥，也是最难练的功夫，个人修为若不到登峰造极地步，
   根本无法领悟这一招式！但是，若苦练神功，到高深境界后领
   悟该绝招，则战斗中会暂时极大幅度的提高自身的攻击力，躲
   闪率，命中率，防御力甚至自身其他属性。只怕从此行走江湖
   将鲜有对手，进入武学之真正返璞归真境界！此外，若使用该
   「冷泉」绝技到妙处，则无意间提高自身的武学修为也非不可
   能之事！

   要求：  内力 2000 以上；
           精力 1500 以上； 
           最大内力 4000 以上； 
           最大精力 2500 以上； 
           冷泉神功 200 以上； 
           基本内功 200 以上； 

HELP
   );
   return 1;
}

