//yiyang-zhi's perform qianlong
//潜龙勿用
//COOL@SJ,20000401

#include <ansi.h>
inherit F_SSERVER;
int qianlong_hit(object me, object victim, object weapon, int damage);
 
string perform_name() {return WHT"潜龙勿用"NOR;}

int perform(object me, object target)
{       
        string msg;
        
        if( !target ) target = offensive_target(me);
 
        if( !objectp(target)
         || !target->is_character()
         || !present(target,environment(me)) )
                return notify_fail("「潜龙勿用」？\n");
        if( !living(target) )
        	return notify_fail("对方已经晕过去了。\n");
 
 	if( me->query_temp("yyz_ql") ){ 
 		if( !me->is_fighting(target) )
 			return notify_fail("你只能对战斗中的对手出「潜龙勿用」。\n"); 
 		write("你一提气，准备用「潜龙勿用」了。\n");
 		me->start_call_out( (: call_other, __FILE__, "main", me, target :), 2);
 		return 1;
 	}
 	
 	if( target->query_temp("yyz_ql/recive") )
 		return notify_fail("这人已经承受不下你的「潜龙勿用」功力了。\n"); 
 		
        if(me->is_fighting())
                return notify_fail("你正忙着呢。\n");   
        if( target->is_fighting())
                return notify_fail("看清楚点，对方正忙着呢。\n");
        
        if( me->query_temp("weapon") )
                return notify_fail("只有空手才能施展「潜龙勿用」。\n"); 

        if( (int)me->query_skill("yiyang-zhi", 1) < 160 ||
            (int)me->query_skill("finger", 1) < 160 )
                return notify_fail("你有的功夫还不够娴熟，不会使用「潜龙勿用」。\n");

        if(!me->query_skill("kurong-changong", 1))
           	if(me->query_skill("qiantian-yiyang", 1) < 160)
                	return notify_fail("以你现在的内功修为还使不出「潜龙勿用」。\n");        
         
        if (me->query_skill_mapped("force") != "kurong-changong" )
            	if (me->query_skill_mapped("force") != "qiantian-yiyang")
                	return notify_fail("你所使用的内功不对。\n");

        if (me->query_skill_prepared("finger") != "yiyang-zhi" || 
            me->query_skill_mapped("finger") != "yiyang-zhi" )
                return notify_fail("你现在无法使出「潜龙勿用」进行攻击。\n");                                                                                 
      
        if( (int)me->query("max_neili") < 2000 )
                return notify_fail("你的内力修为太弱，不能使用「潜龙勿用」！\n");

        if( (int)me->query("neili") < (int)me->query("max_neili")*3/2 )
                return notify_fail("你的真气不够！\n");
        
        if( (int)me->query("jingli") <= (int)me->query("max_jingli")*3/2 )
                return notify_fail("你的精力不够！\n");
        
        
        if ((string)me->query("class")=="bonze")
         	msg = WHT "\n$N忽然低唤了一声："+target->name()+",声音犹似在千重梦魔浮沉中气若游丝地传来。"NOR;    
	else 	msg = WHT "\n$N忽然大喝一声："+target->name()+",声音犹似晴空霹雳，震的双耳”嗡嗡“做响。"NOR; 
        tell_object(target, WHT"你只觉眉心印堂间有一股力量像要把自己的双眉撕裂一般。\n"NOR);
        msg += WHT "$n征了一怔，$N忽一举袖，$n刹那间掌心向外，拦在额前,却听见“波”的一声，$N一指击在了$n的手心。\n"NOR, me, target;
        
        message_vision(msg, me, target);
        
        if( target->query_skill("force") < me->query_skill("finger")/3 ){
        	tell_object(target, "你虽然用手掌挡住了这一指，但一股热流仍自掌背迅速侵入眉心，你只觉头晕脑涨，眼前一黑。。。。\n");
        	target->unconcious();
        	tell_object(me, "你这一指击中，劲力源源不断的输入，却发现对方根本无法承受你的劲力,赶忙中止了这一式。\n");
        	me->set("neili",(int)(me->query("neili")/2));
        	me->set("jingli",(int)(me->query("jingli")/2));
        	me->apply_condition("no_exert", (int)(me->query_skill("finger")/40));   
        }

        if( target->query_skill("yiyang-zhi") )
        	target->set_temp("yyz_ql/lvl", me->query_skill("yiyang-zhi")/2 );
        else 
        	target->set_temp("yyz_ql/lvl", me->query_skill("yiyang-zhi")/4 );

	target->set_temp("yyz_ql/recive", 1);
	tell_object(target, "你虽然用手掌挡住了这一指，但一股热流仍自掌背迅速侵入眉心，漫向全身各处，你隐隐感觉到一股气流在体内激荡。\n");
	me->set("neili",0);
        me->set("jingli",0);
       	tell_object(me, "你这一指，耗尽了所有的元气，只觉的自己疲弱之极，整个人就像一株突然枯了的朽木一般。\n");
        tell_object(target,me->name()+"看起来疲弱之极，整个人就像一株突然枯奏了的朽木一般。\n");
        me->apply_condition("no_exert", (int)(me->query_skill("finger")/35));
        target->start_call_out( (: call_other, __FILE__, "remove_qianlong", target :), 20+random(40) );
       	return 1;
}

void remove_qianlong(object ob)
{ 
	if( !ob ) return;
	if( !ob->query_temp("yyz_ql/recive") ) return;
	ob->delete_temp("yyz_ql");
	tell_object(ob, "你体内的那股气流满满消散了。\n");
}

int main(object me, object target)
{ 
        int i;
        mapping action;
	        
        if( !me ) return 0;
         
        if( me->is_ghost() || !living(me) ){    
        	me->delete_temp("yyz_ql");
                return 1;
        }
        if( !me->is_fighting() ) return 0;

	if( !target || !living(target) || environment(target) != environment(me) || !me->is_fighting(target) ){
		write("你才发觉对手已经没有了。\n");
		return 1;
	}
	
        if( me->query_temp("weapon") ){
		write("你得放下武器才能出「潜龙勿用」啊。\n");
		return 1;
	}
        
        if( ((int)me->query("eff_qi")* 100 / (int)me->query("max_qi")) > 40 ){
		write("「潜龙勿用」是拼命招数，你留到关键时刻用吧。\n");
		return 1;
	}
	me->reset_action();
	action = me->query("actions");
	if( !mapp(action) ) {
		me->reset_action();
		action = me->query("action");
		if( !mapp(action) ) {
			CHANNEL_D->do_channel( this_object(), "err",
				sprintf("%O(%s): bad action = %O", me, me->query("id"), me->query("actions", 1)));
			write(HIR"警告：Combat action 出错，请通知巫师 -> Yiyangzhi PFM qianlong.\n");
			return 0;
		}
	}
	action += ([ "post_action": (: qianlong_hit :) ]);
	me->set("actions", action);
	me->delete_temp("yyz_ql");
        i = me->query_temp("yyz_ql/lvl");
        message_vision(RED "$N受伤极重，急忙竭力抵挡，突然之间，$N眉心穴一阵滚热，自玄关热流直沉任脉，而照流连接督脉，两股异流迅速周折一大周天后，在带脉合流为一，在冲脉化流为劲，$N本来一招即出，竟中途易招为指“吱”的一声，指风破空而出！\n\n"NOR, me,target);     
	me->add_temp("apply/attack", i);
	me->add_temp("apply/damage", i/2 );
	COMBAT_D->do_attack(me, target, 0, 1);
	me->add_temp("apply/attack", -i);
	me->add_temp("apply/damage", -i/2 );
	return 1;                 

}

int qianlong_hit(object me, object victim, object weapon, int damage)
{    	
     	if(!victim ) return 0;
     	if( damage == -1 || damage == -2 ){
     		message_vision(WHT "\n\n$n见到$N忽然使出一招一阳指，精妙绝伦，指风本来甚为轻微，一旦遇自己的护身真气，骤改为锐劲，“波”地一声，\n"+
                "戳破而入，$n一怔之下，指风已直逼眉心，$n错手不及，被一击命中。\n"NOR, me, victim);   
         	victim->apply_condition("no_exert", victim->query_condition("no_exert") + 5);
        }
	
        else {
                message_vision(WHT "\n$n见到$N忽然使出一招一阳指，精妙绝伦，指风本来甚为轻微，一旦遇自己的护身真气，骤改为锐劲，“波”地一声，戳破而入，$n一怔之下，\n"
                +"指风已直逼眉心，$n应变奇速，左掌叠在右掌之后，右掌掌心外吐，左掌掌背格在额前，“啪”的一声，指风射人他掌心内。$n右掌已运聚全力，\n"
                +"抗拒指风，左掌又加以支撑，但那一缕指风，连闯三关，所发的破空之声一次比一次更烈，仍然有一小部分内劲透进眉心要穴。\n"NOR, me, victim);        
                victim->apply_condition("no_exert", victim->query_condition("no_exert") + 1);
                victim->apply_condition("no_perform", victim->query_condition("no_perform") + 1);
        }
        return 1; 
}
