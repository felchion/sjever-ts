// hhd.c

#include <ansi.h>

inherit ITEM;

string long();

void create()
{
	int i, size;
	string name, id, id1;

	string *first_name=({HIR+"赤",YEL+"橙",HIY+"黄",HIG+"绿",HIC+"青",HIB+"蓝",MAG+"紫",HIY+"金",HIW+"银",WHT+"玉",HIM+"仙",CYN+"飞"});
	string *first_id=({"chi","cheng","huang","lv","qing","lan","zi","jin","yin","yu","xian","fei"});
	string *second_name=({"水","竹","石","肌","砂","林","龙","虎","豹","凤"});
	string *second_id=({"shui","zhu","shi","ji","sha","lin","long","hu","bao","feng"});
	string *third_name=({"丸"+NOR,"丹"+NOR});
	string *third_id=({"wan", "dan"});

	size = sizeof(first_name);
	i = random(size);
	name = first_name[i];
	id = first_id[i];

	size = sizeof(second_name);
	i = random(size);
	name += second_name[i];
	id += second_id[i];

	size = sizeof(third_name);
	i = random(size);
	name += third_name[i];
	id1 = third_id[i];

	set_name(name, ({id+" "+id1,id,id1}));
	set_weight(500);
	if (clonep())
		set_default_object(__FILE__);
	else {
		set("long", (: long :));
		set("unit", "颗");
		set("no_drop", 1);
		set("no_give", 1);
		set("sjsz_item", 1);
		set("value", 2000);
	}
	setup();
}

void init()
{
	object me = this_player(), ob;

	if ( environment() == me ) {
		add_action("do_eat", "fu");
		add_action("do_eat", "eat");
	}

	if ( ! query("secret_obj")) return;

	if ( query("owner") == getuid(me) )
		return;

	ob = find_player(query("owner"));

	if ( ! ob) return;

	if ( ob->query_temp("die_by_from") == me){
		ob->delete_temp("die_by_from");
		CHANNEL_D->do_channel( this_object(), "rumor",sprintf("%s弄到了一颗%s！", me->name(1), query("name")));
		set("owner", getuid(me));
		set("eat_time", uptime());
	}
	return;

}

string long()
{
	string str;

	str = query("name")+"("+capitalize(query("id"))+")\n";
	str += "一颗"+query("name")+"，服用(fu)以后，有";
	switch(query("bonus")) {
		case "combat_exp":	str +="增加实战经验";	break;
		case "max_jingli":	str +="提高精力";	break;
		case "max_neili":	str +="加大内力";	break;
		case "potential":	str +="增加潜能";	break;
		default:		str +="一定";		break;
	}
	str += "的功效。\n";
	return str;
}

int do_eat(string arg)
{
	object me = this_player();
	int i, max;
	string str;

	if (!id(arg)) return 0;

	if ( me->is_busy() || me->is_fighting()) 
		return notify_fail("你正忙着呢。\n");

	if ( query("owner") != getuid(me))
		return notify_fail("你从什么地方得到的？\n");

	if ( query("secret_obj") && query("eat_time") + 900 > uptime())
		return notify_fail("时候未到，你现在还不适合吃。\n");

	str = HIW"$N吃下一颗"+query("name")+HIW"，顿时觉得浑身为之一振，增加了";
	switch(query("bonus")) {
		case "combat_exp":
			if ( query("secret_obj") )
				i = 4000 + random(2000);
			else
				i = 110 + random(80);
			me->add("combat_exp", i);
			log_file("job/shijian", 
				sprintf("%8s%-10s食用%6s，得到%4d点经验。",
					me->name(1),
					"("+getuid(me)+")",
					query("name"),
					i
				), me
			);
			str += chinese_number(i)+"点实战经验。\n"NOR;
			break;
		case "max_jingli":
			i = random(2) + 1;
			me->add("max_jingli", i);
			me->add("eff_jingli", i);
			str += chinese_number(i)+"点最大精力。\n"NOR;
			break;
		case "max_neili":
			i = random(2) + 1;
			max = (me->query_skill("force")-me->query_temp("apply/force")) * 8 + me->query("combat_exp") / 1000;
			if ( (me->query("max_neili") - 100 ) < max )
				me->add("max_neili", i);
			str += chinese_number(i)+"点最大内力。\n"NOR;
			break;
		case "potential":
			i = random(16) + 22;
			me->add("potential", i);
			if ( me->query("potential") > me->query("max_pot"))
				me->set("potential", me->query("max_pot"));
			str += chinese_number(i)+"点潜能。\n"NOR;
			break;
		default :
			return notify_fail("有问题，请向管理员汇报。\n");
	}
	message_vision(str, me);
	"/adm/daemons/emoted"->do_emote(me,"taste");
	if ( query("secret_obj"))
		CHANNEL_D->do_channel( this_object(), "rumor",sprintf("有人看到%s服下了一颗%s！", me->name(1), query("name")));
	destruct(this_object());
	return 1;
}
