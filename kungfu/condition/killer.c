// killer
#include <ansi.h>
#include <login.h>
#define JAIL_ROOM "/d/city/laofang"

private int clear_at_room(object ob)
{
	if(!ob) return 0;
	if(!interactive(ob)) return 0;
		
	if(ob->query("no_pk")) return 0;
		
	return 1;
}

int update_condition(object me, int duration)
{
	mixed where;
	int pk;

	if (!userp(me))
		return 0;
	where = environment(me);
  pk = me->query_condition("pk");
        
  if(duration > 90 && base_name(where) != JAIL_ROOM && me->query("no_pk") ) {
    	message_vision(HIW"\n一群捕快围住$N，带头的一个大喝一声“"+HIR+"拿下了！"+HIW+"”$N正想反抗，突然一想到自己已经金盆洗手，长叹一声任被捕快带走了。\n\n"NOR, me);
     	me->unconcious();
    	me->move(JAIL_ROOM);
	  	me->set("jail_timeout_room", base_name(where));
			me->set("startroom", JAIL_ROOM);
			message("channel:chat", HIC "\n【官府消息】"+(string)me->query("name")+"因为作恶多端、罪大恶极，被捕快抓到监狱关押。\n\n" NOR, users());
			me->apply_condition("killer", 270);
			return 1;
   }
/*
	if (duration > 270 && base_name(where) != JAIL_ROOM) {
		message_vision(HIW"\n$N作恶多端，血债难偿，官府通缉你很长时间了，这回跑不了了，老老实实跟我走吧。\n\n"NOR, me);
		me->move(JAIL_ROOM);
		me->set("jail_timeout_room", base_name(where));
		me->set("startroom", JAIL_ROOM);
		message("channel:chat", HIC "\n【官府消息】"+(string)me->query("name")+"因为作恶多端、罪大恶极，被捕快抓到监狱关押。\n\n" NOR, users());
		me->apply_condition("killer", 270);
		return 1;
	}
*/
	if (!me->query("no_pk") && duration <= 150 && base_name(where) == JAIL_ROOM) {
		message_vision(HIW"\n本府看$N尚有悔过之意，先准$N出去，切记不可再为非作歹，否则定不轻饶！\n\n"NOR, me);
		message("channel:chat", HIC "\n【官府消息】"+me->query("name")+"尚有悔改之意，本府先准他出去，以观后效。\n\n" NOR, users(), me);
		where = me->query("jail_timout_room");
		if (where && (where = load_object(where)) && !where->is_character()) {
			me->move(where);
			me->delete("jail_timout_room");
		}
		else me->move(REVIVE_ROOM);
		return 1;
	}
			
	if(me->query_condition("relax"))//关禁闭不减少 tj time
		return 1;

	if ( pk && (!living(me) || sizeof(filter_array(all_inventory(environment(me)) - ({ me }), (: clear_at_room :)))))
		return 1;
	if (duration < 2) {
		if(base_name(where)==JAIL_ROOM && me->query("no_pk"))
		{
			message_vision(HIW"\n本府看$N尚有悔过之意，先准$N出去，切记不可再为非作歹，否则定不轻饶！\n\n"NOR, me);
			message("channel:chat", HIC "\n【官府消息】"+me->query("name")+"尚有悔改之意，本府先准他出去，以观后效。\n\n" NOR, users(), me);
			where = me->query("jail_timout_room");
			if (where && (where = load_object(where)) && !where->is_character()) {
				me->move(where);
				me->delete("jail_timout_room");
			}
			else me->move(REVIVE_ROOM);
		}
		else tell_object(me, "官府不再通缉你了！\n");
		return 0;
	}
	if (pk && (duration < 10 || !(duration % 10) && base_name(where) != JAIL_ROOM))
		message("channel:rumor",
			HIM "【谣言】某人：有人看见"+me->query("name")+"在"+TASK_D->get_regions(base_name(where))
			+strip(where->query("short"))+"附近出现！\n" NOR, users(), me
		);
	if (!pk || duration % 10 || interactive(me) && query_idle(me) < 15)
		me->apply_condition("killer", duration - 1);
	else
		me->apply_condition("killer", duration - random(2));
	return 1;
}

string query_type(object me)
{
	return "killer";
}
