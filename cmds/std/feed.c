// feed.c
// by augx@sj  6/27/2002

#include <ansi.h>
inherit F_CLEAN_UP;

int main(object me, string arg)
{
	object target,obj,env;
	string s1,s2;

	if( me->is_busy() )
		return notify_fail("你正忙着呢。\n");

	if( !arg || sscanf(arg,"%s to %s",s1,s2)<2 )
		return notify_fail("你要给什么动物喂食？\n");

	if( !(target=present(s2,environment(me))) )
		return notify_fail("你要给什么动物喂食？\n");

	if( !(obj=present(s1,me)) && !(obj=present(s1,environment(me))) )
		return notify_fail("你要用什么来喂食？\n");

	if( !living(target) )
		return notify_fail("那不是活物。\n");

	if( target->query("master") != me->query("id") )
		return notify_fail("你还没有降伏它呢。\n");

	if( obj->query("food_supply") ) {
		if( !obj->query("food_remaining") )
	     		return notify_fail( obj->name() + "已经没什么好吃的了。\n");
	     	if( target->query("food") >= target->max_food_capacity() )
	     		return notify_fail("它已经吃太饱了，再也塞不下任何东西了。\n");

		message_vision(CYN"$N把一"+obj->query("unit")+obj->query("name")+CYN"喂给$n。\n"NOR, me, target);
		env = environment(obj);
		obj->move(target);
		"/cmds/std/eat.c"->main(target,obj->query("id"));
		if(obj) obj->move(env);
	}
	else if( obj->query("liquid") ) {
		if( !obj->query("liquid/remaining") )
			return notify_fail( obj->name() + (obj->query("liquid/name") ? "已经被喝得一滴也不剩了。\n":"是空的。\n"));
		if( target->query("water") >= target->max_water_capacity() )
			return notify_fail("它已经喝得太多了，再也灌不下一滴水了。\n");

		message_vision(CYN"$N把一"+obj->query("unit")+obj->query("name")+CYN"喂给$n。\n"NOR, me, target);
		env = environment(obj);
		obj->move(target);
		"/cmds/std/drink.c"->main(target,obj->query("id"));
		if(obj) obj->move(env);
	}
	else
		return notify_fail("这样东西既不能吃也不能喝呀。\n");

	return 1;
}

int help(object me)
{
write(@HELP
指令格式 : feed <食物或水> to <宠物>

这个命令用来给你的宠物喂食。
HELP
);
	return 1;
}
