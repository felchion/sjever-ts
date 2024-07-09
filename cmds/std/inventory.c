// inventory.c
// Modified by snowman@SJ 31/05/2000
// equipped, base_unit, embedded are away from mapping list.
// Total number of inventories YUJ@SJ 2001-05-17

#include <ansi.h>

inherit F_CLEAN_UP;

int main(object me, string arg)
{
	object *inv, ob;
	string *key, unit, str;
	mixed first;
	int i, w, wiz = wizardp(me);

	if( arg ) {
		ob = present(arg, environment(me));
		if( wiz ) {
			if (!ob) ob = LOGIN_D->find_body(arg);
			if (!ob) ob = find_living(arg);
		}
		else if( ob ) {
			if( userp(ob)
			  || !me->query("group/id")
			  || me->query("group/id") != ob->query("group/id")
		  	  || me->query("group/class") >= ob->query("group/class") )
				return notify_fail("你只能查看同帮派，且等级比你低的NPC所携带的物品。\n");
		}
	}

	if (!ob) ob = me;

	inv = filter_array(all_inventory(ob), "visible", me);
	if (!sizeof(inv)) {
		write((ob==me)? "目前你身上没有任何东西。\n"
			: ob->name() + "身上没有携带任何东西。\n");
		return 1;
	}

	str = sprintf("%s身上带着%s件东西(负重 %:2f%%)：\n",
		(ob==me)? "你": ob->name(),
		chinese_number(sizeof(inv)),
		ob->query_encumbrance() * 100.0 / ob->query_max_encumbrance()
	);

	first = filter_array(inv,
		(: $1->query("base_unit") && !$1->query("equipped") && !$1->query("embedded") :)
	);
	first += filter_array(inv,
		(: $1->query("equipped") :)
	);
	first += filter_array(inv,
		(: $1->query("embedded") :)
	);

	foreach(object item in first) {
		unit = sprintf("%s",
			(item->query("equipped")? HIC"□"NOR :item->query("embedded")? HIR"☆"NOR: "  ")+
			item->short()
		);
		if (wiz) {
			w = item->weight();
			unit += sprintf("%*s", 79-strwidth(strip(unit)), w/500+"斤"+(w%500)/50+"两"+(w%50)/5+"钱");
		}

		str += unit + "\n";
	}

	inv -= first;
	i = sizeof(inv);
	if (i < 1) {
		write(str);
		if (wiz) write("总重量：" + ob->query_encumbrance() + "\n");
		return 1;
	}

	first = ([]);
	foreach(object item in inv) {
		unit = base_name(item) + item->name();
		if( !mapp(first[unit]) ) {
			first[unit] = ([
				"total" : 1,
				"unit" : item->query("unit"),
				"short" : item->short(),
			]);
			if ( wiz ) first[unit]["weight"] = item->weight();
		} else first[unit]["total"]++;
	}

	key = keys(first);
	if (!sizeof(key)) {
		write(str);
		return 1;
	}

	foreach(string item in key) {
		unit = sprintf("  %s", chinese_number(first[item]["total"]) + first[item]["unit"] + first[item]["short"]);
		if( wiz ){
			w = first[item]["weight"] * first[item]["total"];
			unit += sprintf("%*s", 79-strwidth(strip(unit)), w/500+"斤"+(w%500)/50+"两"+(w%50)/5+"钱");
		}
		str += unit + "\n";
	}
	write(str);
	if ( wiz ) write("总重量：" + ob->query_encumbrance() + "\n");
	return 1;
}

int help ()
{
	write(@HELP
指令格式: inventory [玩家ID]

可列出你(你)目前身上所携带的所有物品。
玩家ID这个参数只有巫师可以使用！
注 : 此指令可以 " i " 代替。

HELP
);
	return 1;
}
