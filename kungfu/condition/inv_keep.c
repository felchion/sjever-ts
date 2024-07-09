#include <ansi.h>
#include <condition.h>

inherit F_CLEAN_UP;

int get_object(object ob, object me)
{
	return ob->query("unique");
}

int update_condition(object me, int duration)
{
  object *inv;
  int i;
  inv = deep_inventory(me);
  inv -= ({ 0 });
  i = sizeof(inv = filter_array(inv,(:get_object:), me));
  
  if(i<2) 
	  me->apply_condition("inv_keep", duration - 1);
	else
	  me->apply_condition("inv_keep", duration - i);


	if( duration < 1 )
	{
		return 0;
	}
	return CND_CONTINUE;
}

string query_type(object me)
{
	return "inv_keep";
}
