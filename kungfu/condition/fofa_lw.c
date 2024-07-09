#include <ansi.h>
#include <condition.h>

inherit F_CLEAN_UP;

int update_condition(object me, int duration)
{

        me->apply_condition("fofa_lw", duration - 1);
        
        if(random(10)==1)
	{
		tell_room(environment(me), me->name() + "身形似乎显得异常高大。\n", ({ me }));		
	}

        if( duration < 1 ) 
        {
        	tell_object(me, HIY"你长长的舒了口气，身形似乎也缩小了一些。\n"NOR);
		me->add_temp("apply/intelligence",-(me->query_temp("lw/int")));
		me->add_temp("apply/strength",-(me->query_temp("lw/str")));
		me->add_temp("apply/dexerity",-(me->query_temp("lw/dex")));
		me->add_temp("apply/constitution",-(me->query_temp("lw/con",)));
		me->delete_temp("lw");
        	return 0;
        }
        return CND_CONTINUE;
}

string query_type(object me)
{
	return "fofa";
}
