// huagu.c 化骨

#include <ansi.h>

inherit F_SSERVER;

string perform_name() {return HIY"化骨绵掌"NOR;}

int perform(object me, object target)
{
        string msg;
        int cs,poisonlev,fzcs,dif,i,tneili,exp,mneili,user,mark;

       	if( !target ) 
       		target = offensive_target(me);
     
        if( !target || !me->is_fighting(target) || !living(target)
        || environment(target)!=environment(me) 
        || target == me
        || !target->is_character())
        	return notify_fail("你想化谁的骨？\n");

        if(target->query("combat_exp") < 80000)
           	return notify_fail("杀鸡焉用牛刀？没必要使用化骨吧。\n");

        if(me->query("age") < 16) 
        	return notify_fail("你现在年龄太小，尚不能使用化骨。\n");

        if(userp(me) 
        && userp(target) 
        && target->query("age") < 17)
                return notify_fail("你的心肠太黑了, 对手无缚鸡之力的玩家也下得了手。\n"); 

        if(userp(me) 
        && userp(target) 
        && target->query("combat_exp")<me->query("combat_exp")/10)
                return notify_fail("你的心肠太黑了, 对手无缚鸡之力的玩家也下得了手。\n");

	if( objectp(me->query_temp("weapon")) )
                return notify_fail("你必须空手才能化骨！\n");

        if((int)me->query_skill("huagu-mianzhang",1)<120)
                return notify_fail("你的化骨绵掌还不够熟练，不能使用！\n");

        if( (int)me->query("neili") < 2000 )
                return notify_fail("你现在内力不足，不能化骨！\n");     

        if( (int)me->query("jingli") < 500 )
                return notify_fail("你现在精力不足，不能化骨！\n");     

        if( (int)me->query_skill("dulong-dafa", 1) < 120 )
                return notify_fail("你的毒龙大法功力不足，不能化骨。\n");

        if (me->query_skill_mapped("force") != "dulong-dafa")
                return notify_fail("没有毒龙大法作为根基，是无法使出「化骨」绝技的！\n"); 
    
    	fzcs = (int)me->query_temp("huagucs",1);
        userp(target)?user = 1:user = 0;
        
        me->start_perform(5,"化骨绵掌");
        msg = HIY "\n$N忽然脸带诡笑，双掌不带一点劲气地向$p轻轻地拍了一掌！\n"NOR;
    
        exp = me->query("combat_exp");
	me->receive_damage("neili",200);
	me->receive_damage("jingli",100);        
        if( exp/2 > random((int)target->query("combat_exp")))
        {
                tneili = target->query("max_neili");
                mneili = me->query("max_neili");
         	
        	cs = me->query_skill("huagu-mianzhang", 1) * target->query("neili") / me->query("neili");
              	if((poisonlev=(int)me->query_skill("poison", 1)) < 20 )
                  	poisonlev=20;
              	cs= cs * poisonlev / 1000;
              	dif=0;
              	me->add("neili", -400);
              	me->add("jingli", -100);
              	if( cs > 0 ){
                  	cs--;
                  	if(user==1)
                        	cs = cs + cs / 2;
                  	if( fzcs!=0 && cs > fzcs){
                      		dif= (cs-fzcs)/2;
                      		cs = fzcs;
                  	}
                  	msg += "结果$p躲闪不及，身中剧毒\n"NOR;
       	        	mark = (mneili/1000-tneili/1000+1)*5;
                          target->set("sld/huagu_mark",mark);
                  	if(target->query("huagu") >= 400)
                                    target->apply_condition("huagu_poison",target->query_condition("huagu_poison")+cs);
                        else {
                      		target->set("huagu",400);
                      		target->apply_condition("huagu_poison",cs);
                  	}
                  	for(i = dif;i > 0; i--){
                      		if(target->query("neili")<600)
                          		break;
                      		target->add("neili",-600);
                      		if(user==1)
                          		target->add("huagu",-200);
                  	}
                  	target->add("huagu", 200*i );
              	}
              	else if( cs < 0 ){
                      	cs++;
                      	if( fzcs!= 0 && cs < -fzcs) {
                          	dif=(-fzcs-cs)/2;
                          	cs=-fzcs;
                      	}
                      	msg += "结果被$p反震回来，$N反而身中剧毒\n"NOR;
                      	if(me->query("huagu") >= 200)
              			me->apply_condition("huagu_poison",me->query_condition("huagu_poison")-cs/2);
                        else{
                        	me->set("huagu",200);
                        	me->apply_condition("huagu_poison",-cs/2);
                     	}
                     	for(i=dif;i>0;i--){
                          	if(me->query("neili")<300)
                             		break;
                          	me->add("neili",-300);
                          	me->add("huagu",-200);
                     	}
                     	me->add("huagu",100*i);
              	}
       	} 
        else {
                msg += "$p往旁边一闪身，躲过了这招。\n"NOR;
                me->start_busy(3);
        }
        message_vision(msg, me, target);
        me->start_busy(2+random(2));
        me->kill_ob(target);
        target->kill_ob(me);
        return 1;       
}
