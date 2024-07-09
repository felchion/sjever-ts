#include <ansi.h>
#include <condition.h>

inherit F_CLEAN_UP;

int update_condition(object me, int duration)
{
				object *ob,where=me->query_temp("thd/maze/place");
				int i,lvl=me->query_skill("bihai-chaosheng",1);				
       
        me->apply_condition("thd_maze", duration - 1);        
        
        if( duration < 1 ) {
        	message_vision(HIW"$N布下的迷阵已经失效了。\n"NOR,me);
        	if (objectp(where))
        		where->delete_temp("thd/maze");
        	me->delete_temp("thd/maze");
        	return 0;
        }
        
        if (environment(me) != where ){      	
       		if (objectp(where))
       			where->delete_temp("thd/maze");
        	me->delete_temp("thd/maze");
        	me->apply_condition("thd_maze", 0);
        	tell_object(me, HIW"你布下的阵法因无人主持所以失效了。\n"NOR);
        	return 0;
        }       	
        
        ob = all_inventory(where);
        for (i=0;i<sizeof(ob);i++)
       	  if ( living(ob[i]) 
      	  	&& ob[i]->is_fighting(me)
      	  	&& !ob[i]->query_temp("thd/maze") ){
      	  		ob[i]->set_temp("thd/maze",lvl);
							ob[i]->set_temp("apply/pfm_parry",-lvl/10*3);
							ob[i]->set_temp("apply/pfm_attack",-lvl/10*3);
							ob[i]->set_temp("apply/pfm_damage",-lvl/10*3);
							ob[i]->set_temp("apply/pfm_dodge",-lvl/10*3);
							ob[i]->add_busy(2+random(3));
							message_vision(HIY"\n$n不知不觉地陷入$N布的迷阵之中，登时手足无措。\n"NOR,me,ob[i]);
							call_out("remove_effect", me->query_condition("thd_maze")*20,ob[i],lvl);
					 }
        return CND_CONTINUE;
}

void remove_effect(object ob, int lvl)
{
	if (objectp(ob)) {		
		ob->delete_temp("thd/maze",lvl);
		ob->set_temp("apply/pfm_parry",lvl/10*3);
		ob->set_temp("apply/pfm_attack",lvl/10*3);
		ob->set_temp("apply/pfm_damage",lvl/10*3);
		ob->set_temp("apply/pfm_dodge",lvl/10*3);	
		message_vision(HIW"$N已经识破了这个迷阵。\n"NOR,ob);
	}
	return;
}

string query_type(object me)
{
	return "thd_maze";
}
