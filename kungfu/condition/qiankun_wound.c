#include <ansi.h>
#include <condition.h>
inherit F_CLEAN_UP;
int update_condition(object me, int duration)
{
	if (!duration) {
		message_vision(HIY"$N终于打通了胸口穴道，内息回复了正常。\n" NOR, me);
		if(me->query_temp("thd/qiankun")) {
	 		 me->add_temp("apply/neili",(me->query("max_neili")/2));
	  	 me->add_temp("apply/qi",(me->query("max_qi")/2));
	  	 me->delete_temp("thd/qiankun");
	  }		
		return 0;
	}
	message_vision(HIR"$N按住胸口，闷哼一声，似乎内息不顺。\n" NOR, me);
	if(!me->query_temp("thd/qiankun")) {
	  me->add_temp("apply/neili",(me->query("max_neili")/-2));
	  me->add_temp("apply/qi",(me->query("max_qi")/-2));
	  me->set_temp("thd/qiankun",1);
	}
	me->apply_condition("qiankun_wound", duration - 1);
	me->start_buy(1+random(3));
	return CND_CONTINUE;
}

string query_type(object me)
{
	return "wound";
}
