// give.c

inherit F_CLEAN_UP;

int do_give(object me, object obj, object who);

void create() { seteuid(getuid()); }

int main(object me, string arg)
{
	string target, item;
	object obj, who, *inv, obj2;
	int i, amount;

	if(!arg) return notify_fail("你要给谁什么东西？\n");

	if( sscanf(arg, "%s to %s", item, target)==2
	|| sscanf(arg, "%s %s", target, item)==2 );
	else return notify_fail("你要给谁什么东西？\n");

	if(!objectp(who = present(target, environment(me))) || !living(who))
		return notify_fail("这里没有这个人。\n");

	if( who == me) return notify_fail("给自己？\n");
	if (userp(me) && me->is_busy()) return notify_fail("你正忙着呢。\n");
	if (userp(me) && !wizardp(me) && who->query("env/no_accept"))
		return notify_fail("对方现在不接受任何东西。\n");
	
	if(sscanf(item, "%d %s", amount, item)==2) {
		if( !objectp(obj = present(item, me)) )
			return notify_fail("你身上没有这样东西。\n");
		if( !wizardp(me) && userp(who) && obj->query("no_drop"))
			return notify_fail("这样东西不能随便给人。\n");
		if (!wizardp(me) && userp(who) && userp(me) && obj->query("no_give"))
			return notify_fail("这样东西不能随便给人。\n");
		
		if( !obj->query_amount() )
			return notify_fail( obj->name() + "不能被分开给人。\n");
		if( amount < 1 )
			return notify_fail("东西的数量至少是一个。\n");
		if( amount > obj->query_amount() )
			return notify_fail("你没有那么多的" + obj->name() + "。\n");
		if( obj->query("weapon_prop") && me->is_exert() &&
			obj == me->query_temp("weapon"))
			       return notify_fail("你正在使用"+me->query_exert()+"，无法给出"+obj->name()+"。\n");
		if( obj->query("weapon_prop") && me->is_perform() &&
			obj == me->query_temp("weapon"))
			       return notify_fail("你正在使用"+me->query_perform()+"，无法给出"+obj->name()+"。\n");

		else if( amount == (int)obj->query_amount() )
			return do_give(me, obj, who);
		else {
			obj->set_amount( (int)obj->query_amount() - amount );
			obj2 = new(base_name(obj));
			obj2->set_amount(amount);
			obj2->set("broken", obj->query("broken"));
			obj2->move(environment(me));
			if (obj2 && do_give(me, obj2, who))
				return 1;
			if (obj2)
				obj2->move(me);
			return 0;
		}
	}

	if(item=="all") {
		inv = all_inventory(me);
		for(i=0; i<sizeof(inv); i++) {
			do_give(me, inv[i], who);
		}
		write("Ok.\n");
		return 1;
	}

	if( !objectp(obj = present(item, me)) )
			return notify_fail("你身上没有这样东西。\n");
	if( obj->query("weapon_prop") && me->is_exert() &&
	    obj == me->query_temp("weapon"))
		 return notify_fail("你正在使用"+me->query_exert()+"，无法给出"+obj->name()+"。\n");
	if( obj->query("weapon_prop") && me->is_perform() &&
	    obj == me->query_temp("weapon"))
		 return notify_fail("你正在使用"+me->query_perform()+"，无法给出"+obj->name()+"。\n");

	return do_give(me, obj, who);
}

int do_give(object me, object obj, object who)
{
	string unit;

	if( !wizardp(me) && userp(who) && obj->query("no_drop"))
		return notify_fail("这样东西不能随便给人。\n");

	if (!wizardp(me) && userp(who) && userp(me) && obj->query("no_give"))
		return notify_fail("这样东西不能随便给人。\n");

	if (!wizardp(me) && obj->query("embedded")){
		if (!stringp(obj->query("embedded")))
			return notify_fail("看清楚，在肉里的东西也能给人？先起出来再说吧。\n");
		else 
		  return notify_fail("对方不接受这样东西。\n");
  }
	
	if (base_name(obj)[0..12]=="/clone/money/" 
	&& userp(me)  
	&& userp(who) 
	&& (who->query("combat_exp") > 500000 || me->query("combat_exp") < who->query("combat_exp") ) 
	)
		return notify_fail("你还是留着自己用吧。\n");
		
//if (base_name(obj)[0..12]=="/clone/money/"
//&& userp(me)
//&& userp(who)
//&& me->query("eff_online_rate")<95)
//return notify_fail("你还是留着自己用吧。\n");
//
//if (base_name(obj)[0..12]=="/clone/money/"
//&& userp(me)
//&& userp(who)
//&& me->query("max_pot")-110 > me->query_skill("literate",1))
//return notify_fail("你还是留着自己用吧。\n");
//
//if (base_name(obj)[0..12]=="/clone/money/"
//&& userp(me)
//&& userp(who)
//&& me->query("max_pot")-who->query("max_pot") > me->query_skill("literate",1))
//return notify_fail("你还是留着自己用吧。\n");
//				
	if( !interactive(who) && !who->accept_object(me, obj) && userp(me) )
		return 0;
	if (!obj) return 0;
	if (obj->query("invisible") && !wizardp(me) && userp(me)) return 0;
	if ( !userp(who) && obj->value() ) {
		message_vision("$N拿出" + obj->short() + "给$n。\n", me, who);
		destruct(obj);
		return 1;
	}
	if (obj->query_amount()) unit = chinese_number(obj->query_amount()) + obj->query("base_unit");
	else unit = "一" + obj->query("unit");
	if( obj->move(who) ) {
		write(sprintf("你给%s%s%s。\n", who->name(), unit,
			obj->name()));
		message("vision", sprintf("%s给你%s%s。\n", me->name(),
			unit, obj->name()), who );
		message("vision", sprintf("%s给%s一%s%s。\n", me->name(), who->name(),
			obj->query("unit"), obj->name()), environment(me), ({me, who}) );
		return 1;
	}
	return 0;
}

int help(object me)
{
write(@HELP
指令格式 : give <物品名称> to <某人>
      或 : give <某人> <物品名称>

这个指令可以让你将某样物品给别人, 当然, 首先你要拥有这样物品.

HELP
    );
    return 1;
}
