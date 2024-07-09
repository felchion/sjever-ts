// wield.c
// Modified by snowman@SJ 01/05/2001
inherit F_FROZEN;
inherit F_SSERVER;

int do_wield(object me, object ob);

int main(object me, string arg)
{
	object ob, *inv;
	int i, count;

	if (userp(me) && (me->is_busy() || me->query_temp("thd/xinghe")))
		return notify_fail("你正忙着呢。\n");
                
	if (userp(me) && me->is_exert()) 
		return notify_fail("你正在使用"+me->query_exert()+"。\n");

	if (userp(me) && me->is_perform() ) 
		return notify_fail("你正在使用"+me->query_perform()+"。\n");

	if (!arg) return notify_fail("你要装备什么武器？\n");

	if (arg=="all") {
		inv = all_inventory(me);
		for(count=0, i=0; i<sizeof(inv); i++) {
			if( inv[i]->query("equipped") ) continue;
			if((int)me->query("max_neili") < (int)inv[i]->query("wield_maxneili")) continue;
			if((int)me->query("neili") < (int)inv[i]->query("wield_neili")) continue;
			if((int)me->query_str() < (int)inv[i]->query("wield_str")) continue;
			if(inv[i]->query("no_wield") && me->query("family/family_name")!=inv[i]->query("family_required")) continue;
			if(do_wield(me, inv[i]) ) count ++;
		}
		write("装备完毕。\n");
		return 1;
	}

	if( !objectp(ob = present(arg, me)) )
		return notify_fail("你身上没有这样东西。\n");

	if( ob->query("equipped"))
		return notify_fail("你已经装备著了。\n");
	if (ob->query("no_wield"))
		if(me->query("family/family_name")!=ob->query("family_required"))
			return notify_fail("你不能使用这件武器。\n");
	if((int)me->query("max_neili") < (int)ob->query("wield_maxneili"))
		return notify_fail("你感觉全身气息翻腾，原来你内力不够，不能装备"+ob->name()+"。\n");
	if((int)me->query("neili") < (int)ob->query("wield_neili"))
		return notify_fail("你感觉全身气息翻腾，原来你真气不够，不能装备"+ob->name()+"。\n");
	if((int)me->query_str() < (int)ob->query("wield_str"))
		return notify_fail("你想举起"+ob->name()+"，怎奈力量不够，只好作罢。\n");
	return do_wield(me, ob);
}

int do_wield(object me, object ob)
{
	string str;
	object target;
	notify_fail("你只能装备可当作武器的东西。\n");
	if (ob->wield()) {
		if (userp(me) && me->is_fighting()) {
			target = offensive_target(me);
			if (target && userp(target)) {
				if (me->query_temp("perform_AP") < 40)
				  return notify_fail("你合气度不足无法装备武器。\n");
				else
				  me->add_temp("perform_AP",-40);
			}
		}
		if( !stringp(str = ob->query("wield_msg")) )
		str = "$N装备$n作武器。\n";
		message_vision(str, me, ob);
		return 1;
	}
	return 0;
}

int help(object me)
{
	write(@HELP
指令格式：wield <装备名称>
 
这个指令让你装备某件物品作武器, 你必需要拥有这样物品.
 
HELP
	);
	return 1;
}

