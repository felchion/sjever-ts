// Created by snowman@SJ 08/12/2000

#include <ansi.h>
#include <condition.h>

inherit F_CLEAN_UP;

#include <combat_msg.h>

int update_condition(object me, int duration)
{
      	int damage, j;
      	
      	damage = me->query_condition("neishang") + 2;
      	damage *= me->query_con() + random(20);
      	if( me->is_fighting() ) damage *= 2;
      	if( !userp(me) ) damage += random(damage);
      
      	if( living(me) ){
      		tell_object(me, "突然你感觉胸口疼痛异常，刚才被拍中的肋骨处好象要裂开了似的，鲜血也从口中喷了出来！\n");
      		tell_room(environment(me), "\n"+HIR + me->name()+"突然一言不发，双手捂胸，蹬蹬磴倒退了数步，接着哇得一声吐出口鲜血来！\n" NOR,({ me }));
	}
	else tell_room(environment(me), "\n"+HIR + me->name()+"突然全身一震，一大口鲜血喷了出来！\n" NOR,({ me }));
	me->add_busy(2);
	
	me->receive_damage("qi", damage);
	if( damage/3 < me->query("qi") )
		me->receive_wound("qi", damage/3);

	else {
		me->receive_wound("qi", me->query("qi")-1);
		message_vision("( $N"RED"受伤过重，已经有如风中残烛，随时都可能断气。 )\n"NOR, me);
		return 0;
	}
      	j = (int)me->query("qi")*100/(int)me->query("max_qi");
      	if(living(me)) 
             	message_vision("( $N"+(random(2)?status_msg(j):eff_status_msg(j))+" )\n", me);
      	me->apply_condition("neishang", duration - 1);
   	if( duration < 1 )
   		return 0;
   	return CND_CONTINUE;
}

string query_type(object me)
{
	return "hurt";
}
