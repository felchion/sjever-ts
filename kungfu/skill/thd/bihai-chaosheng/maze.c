// maze.c 奇门八卦迷阵

#include <ansi.h>
inherit F_SSERVER;

int exert(object me, object target)
{
	string msg;
	object *ob,where;
  int i;
	
	where=environment(me);
	
	if (!objectp(where))
		return notify_fail("这里不能布阵。\n");	
	if (!where->query("outdoors"))
		return notify_fail("你想拆房子？\n");	
	if (me->query_condition("thd_maze")>0)
		return notify_fail("你已经布下一个迷阵了。\n");		
	if (where->query_temp("thd/maze"))
		return notify_fail("这里已经有人布一下迷阵了。\n");	
	if (me->is_fighting())		
		return notify_fail("现在好象已经太迟了吧。\n");	
	if (me->query("neili")<	me->query_skill("bihai-chaosheng",1)*2)
		return notify_fail("你现在内力不足以布阵。\n");	
		
	ob = all_inventory(environment(me));
	for (i=0;i<sizeof(ob);i++)
		if ( userp(ob[i])  
		&& ob[i] != me
		&& (!me->query_team() 
			 || member_array(ob[i],me->query_team()) == -1 ) )
					return notify_fail("奇门阵法是桃花岛不传之秘，岂容他人窥视。\n");
					
	msg = HIY"\n$N东奔西跑将周围的石块、树枝稍微移动了一下位置，这里的地貌就有了很大的改观！\n";
	message_vision(msg, me);
	
	call_out("check_env", 3,me,100);
	me->set_temp("thd/maze/place",where);
	where->set_temp("thd/maze",1);
	me->start_busy(3);
	me->add("neili",-me->query_skill("bihai-chaosheng",1)*2);
	return 1;
}


int check_env(object me, int duration)
{
	object *ob,where=me->query_temp("thd/maze/place");
	int i,lvl=me->query_skill("bihai-chaosheng",1);

	duration --;

	if( duration < 1 ) {
		tell_object(me,HIW"你布下的迷阵已经失效了。\n"NOR);
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
		ob[i]->add_temp("apply/pfm_parry",-120);
		ob[i]->add_temp("apply/pfm_attack",-120);
		ob[i]->add_temp("apply/pfm_damage",-80);
		ob[i]->add_temp("apply/pfm_dodge",-120);
		ob[i]->add_busy(2+random(3));
		message_vision(HIY"\n$n不知不觉地陷入$N布的迷阵之中，登时手足无措。\n"NOR,me,ob[i]);
		call_out("remove_effect", 120,ob[i]);
	}
	call_out("check_env", 3,me,duration);
	return 1;
}

void remove_effect(object ob)
{
	if (objectp(ob)) {		
		ob->delete_temp("thd/maze");
		ob->add_temp("apply/pfm_parry",120);
		ob->add_temp("apply/pfm_attack",120);
		ob->add_temp("apply/pfm_damage",80);
		ob->add_temp("apply/pfm_dodge",120);	
		message_vision(HIW"$N已经识破了这个迷阵。\n"NOR,ob);
	}
	return;
}

string exert_name(){ return HIY"迷阵"NOR; }

int help(object me)
{
        write(HIY"\n碧海潮生功之「迷阵」："NOR"\n\n");
        write(@HELP
        要求：  当前内力高于碧海潮生功等级2倍；
                碧海潮生功等级 120 以上；
                奇门八卦等级 120 以上；
                激发内功为碧海潮生功。

HELP
        );
        return 1;
}
