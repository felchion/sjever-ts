#include <ansi.h>
#include <condition.h>

inherit F_CLEAN_UP;

int update_condition(object me, int duration)
{

        me->apply_condition("fofa_mfjl", duration - 1);
        me->set("channels", ({}));
        
        if(random(10)==1)
	{
		tell_room(environment(me), me->name() + "对周围事物是不感不知，心中却清明如许。\n", ({ me }));
	}

        if( duration < 1 ) 
        {
        	tell_object(me, HIW"你双眼一睁露出两道精光。\n"NOR);
        	return 0;
        }
        return CND_CONTINUE;
}

string query_type(object me)
{
	return "fofa";
}
