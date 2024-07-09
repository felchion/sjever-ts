#include <ansi.h>
#include <condition.h>

inherit F_CLEAN_UP;

int update_condition(object me, int duration)
{
	int resist = me->query_skill("poison",1)+me->query_skill("medicine",1);

	if( !living(me) ) 
		message("vision", me->name() + "痛苦地哼了一声。\n", environment(me), me);
	else {
		tell_object(me, HIB "忽然一阵刺骨的奇寒袭来，你中的星宿掌毒发作了！\n" NOR );
		message("vision", me->name() + "的身子突然晃了两晃，牙关格格地响了起来。\n", environment(me), me);
	}


  if (random(500) < resist && me->query("neili")>500) {
    me->add("neili",-500);
    tell_object(me, HIW"你运起内功强行压下毒性。\n" NOR );
    me->add_busy(1);
	}
	else {
		me->receive_wound("qi", me->query("max_qi")/8, "星宿毒发作");
		me->receive_wound("jing", me->query("max_jing")/8, "星宿毒发作");
		me->add_busy(1);
  }
	me->apply_condition("xx_poison", duration - 1);
	return CND_CONTINUE;
}

string query_type(object me)
{
	return "poison";
}
