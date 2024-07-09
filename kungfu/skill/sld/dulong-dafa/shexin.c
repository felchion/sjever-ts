//modify By Ziqing  Y2k
// Modified by snowman@SJ 05/12/2000

#include <ansi.h>

inherit F_SSERVER;

string exert_name() {return HIR"摄心"NOR;}

int exert(object me, object target)
{

        int hits;

        if( !target ) target = offensive_target(me);

	if( !objectp(target)
        || !target->is_character()
        || !me->is_fighting(target) 
        || !living(target) )
        	return notify_fail("「摄心大法」只能对战斗中的对手使用。\n");

        if((me->query_temp("sld/wudi") < 1) && (me->query_temp("sld/wudi2") <1))
        	return notify_fail("你没有运无敌怎么能使用「摄心」？！\n");
        
                
        if( (int)me->query_skill("dulong-dafa", 1) < 150 
        || (int)me->query_skill("shenlong-yaoli", 1) < 150 
        || (int)me->query_str() <=30 )
                return notify_fail("你修为还不够，还未能运用「摄心」！\n");

        if( me->query_skill("force", 1) < 150 )
                return notify_fail("你的内功修为火候未到，施展只会伤及自身！\n");
     
        if( me->query("max_neili") < 2000 )
                return notify_fail("你的内力修为不足以运用「摄心」！\n");
        if( me->query("eff_jingli") < 1500 )
                return notify_fail("你的精力修为不足以运用「摄心」！\n");
        if( me->query("neili") < 600 )
                return notify_fail("你的内力不够，劲力不足以运用「摄心」！\n");

        if( me->query("jingli") < 500 )
                return notify_fail("你的精力有限，不足以运用「摄心」！\n");
        if( target->query_temp("shexin"))
                return notify_fail("对方已经身中摄心大法了");
        
        message_vision(HIR"$n听到$P每念一句，心中就是一凛，但觉的这人的行为希奇古怪，前所未有！\n\n" NOR, me , target);
        me->add("neili",-300);
        me->add("jingli",-100);
        me->start_exert(5, "「摄心」");
        if( target->query_temp("hmg/nizhuan")){ 
           	message_vision("结果$P只觉$n招式怪异摄心大法无法成功！\n", me, target);
           	return 1;
        }  
        if( target->query_temp("jiuyin/fast")){
           	message_vision("结果$P只觉$n身形飘忽不定难以捉摸摄心大法无法成功！\n", me, target);
        	return 1;
        }
     	if( target->query_skill("buddhism", 1) > 200 && !target->is_killing(me->query("id")) ){
     		message_vision("结果$n佛法高深，摄心大法根本对$p没用！\n", me, target);
        	return 1;
        }
        if( random(me->query_int()) > target->query_int()/2 ){
        	message_vision("结果$n受到$P的摄心大法的影响，原先的斗志顿然消失！\n", me , target);
       		target->remove_all_killer();
       		target->receive_damage("jing", me->query_int()*20, me);
       		target->receive_wound("jing", me->query_int()*5, me);
        	if( !target->is_busy() )
        		target->start_busy(1);    
        	hits = random(100)+(me->query("dex")-target->query("dex"))*5+(me->query("jingli")-target->query("jingli"))/20;
       		if (hits > 30) {
        		message_vision(HIR"$n受到$P摄心大法感应，越斗越是害怕，力气顿时无法凝聚! \n", me , target);
         		target->apply_condition("sld_shexin", 2+random(3) );
         		target->apply_condition("no_perform", 3);
         		target->apply_condition("no_enforce", 3);
         		target->set("jiali", 0);
         		me->start_busy(1);
        	}
        }
        return 1;
}  

