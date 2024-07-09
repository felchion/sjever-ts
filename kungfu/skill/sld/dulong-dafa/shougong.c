// shougong.c
// By Spiderii fix bug
#include <ansi.h>

inherit F_CLEAN_UP;

string exert_name() {return CYN"收功"NOR;}

void remove_effect(object me);

int exert(object me, object target)
{
        if( target != me )
                return notify_fail("你只能用毒龙大法来收回自己的功力。\n");
        
        if( me->query_temp("sld/shougong")) return 0;

        if( me->query_temp("sld/wudi2"))
                return notify_fail("你已经不能控制自己，无法收功了。\n");

      
        if( me->query_temp("sld/wudi")) {
                message_vision(CYN"$N缓缓收回内力，眼中的红光慢慢暗淡下来。\n"NOR,me);
                me->delete_temp("apply/attack", me->query_temp("sld/wudiadd")/4);
                me->add("max_qi",- me->query_temp("sld/wudiqi"));
                me->delete_temp("sld");
                me->set_temp("sld/shougong", 1);
                me->start_call_out( (: call_other, __FILE__, "remove_effect", me :), 15);
        }
        else {
                return notify_fail("你现在无法收功。\n");
        }
        return 1;
}

void remove_effect(object me)
{
        if(!me) return;
        if(!(int)me->query_temp("sld/shougong")) return;
        me->delete_temp("sld/shougong");
        return;
}
