#include <ansi.h>
#include <condition.h>

inherit F_CLEAN_UP;

int update_condition(object me, int duration)
{

        me->apply_condition("fofa_gdp", duration - 1);
        
        if(random(10)==1)
	{
		tell_room(environment(me), me->name() + "身上散发出一阵阵飘渺的香气。\n", ({ me }));
		tell_object(me, HIG"在这虚无飘渺的香气中，尘世间的争斗似乎和你再没关系。\n"NOR);
	}

        if( duration < 1 ) 
        {
        	tell_object(me, HIW"你身上的香气渐渐淡去。\n"NOR);
        	return 0;
        }
        return CND_CONTINUE;
}

string query_type(object me)
{
	return "fofa";
}
