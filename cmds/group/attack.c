// attack.c
// by augx@sj  4/2/2002

#include <ansi.h>
inherit F_CLEAN_UP;

int help(object me)
{
	write(@HELP
指令格式 :
attack here
攻击所在的房间。

attack [物品id]
攻击房间里的物品。

攻击者必须手持锤类兵器。

HELP
	);
	return 1;
}

int main(object me, string arg)
{
	object env = environment(me), obj, ob, weapon;
	int t;

	if( !arg ) return help(me);
	if( arg == "here" ) obj = env; else obj = present(arg,env);
	if( !obj ) return notify_fail("你要攻击什么？\n");
	if( living(obj) ) return notify_fail("用kill指令可能比较快。\n");
	if( obj->query("hp") <= 0 ) return notify_fail("你不能摧毁这样东西。\n");
	if( env->query("attack") ) return notify_fail("已经有人在这里摧毁敌人的装备了。\n");
	if( !me->query("group/id") ) return notify_fail("你还没有加入帮派，到这里捣什么乱啊。\n");
	if( !(weapon=me->query_temp("weapon")) || weapon->query("skill_type")!="hammer" )
		return notify_fail("你没有锤子，如何摧毁房间和物品啊？\n");
	if( !GROUP_D->site_now(me->query("group/id")) )
		return notify_fail( GROUP_D->get_last_error() );
	if( GROUP_D->is_group_fight(me) != 1 )
		return notify_fail("你并没有处于战斗状况呀。\n");
	if( GROUP_D->query_league(me->query("group/id")) == env->query("group1") )
		return notify_fail("摧毁自己人的装备？\n");

	message_vision("$N抡起"+weapon->name()+"砸向"+(clonep(obj)?obj->name():obj->query("short"))+"......\n",me);
	t = 15 + random(30);
	me->start_busy(t/2+1);
	env->set("attack",time()+t);

	switch( obj->query("id") ) {
		case "shi men":	 env->set("attack_type","door"); break;
		case "pai bian": env->set("attack_type","flag"); break;
		case "qian gui": env->set("attack_type","cash_box"); break;
		default: env->set("attack_type","room"); break;
	}

	foreach ( ob in all_inventory(env) ) {
		if( !ob || !living(ob) || !userp(ob) ) continue;
		env->set_attack(ob);
	}

	call_out("attack_finish",t,obj,env,me);
	return 1;
}

void attack_finish(object obj,object env,object me)
{
	if( !env ) return;
	env->set("attack",0);
	env->set("attack_type","");

	if( !me ) return;
	tell_room(env,me->name()+"的攻击结束了，",({me}));
	tell_object(me,"你攻击结束了，");

	if( environment(me) != env ) return;
	GROUP_D->attack_room(obj,me);
}
