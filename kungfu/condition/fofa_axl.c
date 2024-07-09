#include <ansi.h>
#include <condition.h>

inherit F_CLEAN_UP;

int update_condition(object me, int duration)
{

        me->apply_condition("fofa_axl", duration - 1);
        
        if(random(10)==1)
	{
		tell_room(environment(me), me->name() + "面目狰狞有若修罗怒目象。\n", ({ me }));		
	}

        if( duration < 1 ) 
        {
        	tell_object(me, HIW"你口宣一声佛号，面目又恢复原来的慈祥之态。\n"NOR);
        	me->add_temp("apply/attack",-(me->query_temp("xiuluo/attack")));
		me->add_temp("apply/damage",-(me->query_temp("xiuluo/damage")));		
		me->delete_temp("xiuluo");
        	return 0;
        }
        return CND_CONTINUE;
}

string query_type(object me)
{
	return "fofa";
}
