// unwield.c

inherit F_FROZEN;
inherit F_SSERVER;

int main(object me, string arg)
{
	object ob,target;
	string str;

	if( !arg ) return notify_fail("你要脱掉什么？\n");

	if (userp(me) && me->is_busy())
		return notify_fail("你正忙着呢。\n");

	if (userp(me) && me->is_exert()) 
		return notify_fail("你正在使用"+me->query_exert()+"。\n");

	if (userp(me) && me->is_perform()) 
		return notify_fail("你正在使用"+me->query_perform()+"。\n");

	if( !objectp(ob = present(arg, me)) )
		return notify_fail("你身上没有这样东西。\n");

	if( (string)ob->query("equipped")!="wielded" )
		return notify_fail("你并没有装备这样东西作为武器。\n");

	if( ob->unequip() ) {
		if (userp(me) && me->is_fighting()) {
			target = offensive_target(me);
			if (target && userp(target)) {
				if (me->query_temp("perform_AP") < 40)
				  return notify_fail("你合气度不足无法放下武器。\n");
				else 
				   me->add_temp("perform_AP",-40);				  
			}
			if (ob->query("unique"))
			  me->set_temp("perform_AP",0);
		}
		if( !stringp(str = ob->query("unwield_msg")) )
			str = "$N放下手中的$n。\n";
		message_vision(str, me, ob);
		return 1;
	} else
		return 0;
}

int help(object me)
{
  write(@HELP
指令格式 : unwield <物品名>
 
这个指令让你放下手中的武器。
 
HELP
    );
    return 1;
}
