// Filename : /cmds/std/ride.c
// Modify By Mope@SJ /17/04/2002

#include <ansi.h>

inherit F_CLEAN_UP;

int do_ride(object me, object ob);

void create() { seteuid(getuid()); }

int main(object me, string arg)
{
	object obj, env;

	if (!arg) return notify_fail("你要骑什么东西？\n");

	if (me->query_temp("is_riding"))
		return notify_fail("你已经有座骑了！\n");

	if (me->is_busy())
		return notify_fail("你上一个动作还没有完成！\n");

	if (me->is_fighting())	return notify_fail("你还在战斗中！没空骑上去。\n");

	// Check if a container is specified.
	env = environment(me);        

	if (!objectp(obj = present(arg, env)) || !living(obj))
			return notify_fail("这里没有这样东西可骑。\n");

        if (env->query("no_ride")) return notify_fail("这里禁止骑马。\n");

	if (!obj->query("ridable") && !obj->query("horse"))
		return notify_fail("这个东西你也要骑？当心你的屁股！\n");

	return do_ride(me, obj);
	write("骑上去了。\n");
}
	
int do_ride(object me, object obj)
{
	object *guard;

	if (!obj) return 0;

	if (guard = obj->query_temp("guarded")) {
		guard = filter_array(guard, (: objectp($1) && present($1, environment($2)) && living($1) && ($1!=$2) :), me);
		if( sizeof(guard) )
			return notify_fail( guard[0]->name() 
				+ "正守在" + obj->name() + "一旁，防止任何人骑走。\n");
	}
	
	if (!obj->query("master"))
		return notify_fail(obj->name() + "是只无主野兽，你得先驯服(train)它啊！\n");
		
	if (obj->query("owner") && (obj->query("owner") != getuid(me))) {
			return notify_fail("你不是"+obj->name() + "的主人！\n");
	}
	
	message_vision( "$N一纵身，威风凛凛地骑上$n。\n", me, obj );
	obj->set_temp("is_rided_by", me);
	obj->set_leader(me);
	me->set_temp("is_riding", obj->query("name"));
	me->set_temp("riding_beast", obj);
	return 1;
}

int help(object me)
{
	write(@HELP
指令格式 : ride | qi <生物名>
 
这个指令可以让你骑上代步的已驯养的动物。
 
HELP
    );
    return 1;
}
 
