//奖励系统之经验潜能内力精力篇

#include <ansi.h>

inherit ITEM;

string long();

void create()
{


	set_name(WHT "丹药" NOR, ({ "dan yao","yao" }));
	if (clonep())
	set_default_object(__FILE__);
	else {

		set_weight(300);
		set("long", (: long :));
		set("unit", "颗");
		set("no_sell", "这么好的东西你也舍得卖？");
		//set("no_drop", 1);
		//set("no_give", 1);
		set("cloned",0);
		set("value", 2000);
	}
	setup();
}

void init()
{
	object me = this_player();
	int i, size;
	string name, id, id1;
	string *first_name=({HIR+"赤",YEL+"橙",HIY+"黄",HIG+"绿",HIC+"青",HIB+"蓝",MAG+"紫",HIY+"金",HIW+"银",WHT+"玉",HIM+"仙",CYN+"飞"});
	string *first_id=({"chi","cheng","huang","lv","qing","lan","zi","jin","yin","yu","xian","fei"});
	string *second_name=({"水","竹","石","肌","砂","林","龙","虎","豹","凤"});
	string *second_id=({"shui","zhu","shi","ji","sha","lin","long","hu","bao","feng"});
	string *third_name=({"丸"+NOR,"丹"+NOR});
	string *third_id=({"wan", "dan"});
	string str;
	if (!query("cloned"))
	{

		switch(random(11)) {
			case 0: str = "max_neili";      break;
			case 1: str = "combat_exp";     break;
			case 2: str = "max_jingli";     break;
			case 3: str = "potential";      break;
			case 4: str = "max_neili";      break;
			case 5: str = "max_neili";     break;
			case 6: str = "max_jingli";     break;
			case 7: str = "potential";      break;
			case 8: str = "max_neili";     break;
			case 9: str = "max_jingli";     break;
			case 10: str = "potential";      break;
		}
		set("bonus", str);
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

		if ( query("secret_obj") )
		set_name(HIY + "极品" + name, ({id+" "+id1,id,id1}));
		else
		set_name(name, ({id+" "+id1,id,id1}));
		
		set("gift_target",getuid(me));

		if((me->query("gender") == "无性" && random(100) == 1) || random(500) == 1)
		{
			set_name("密宗龙阳散",({"mizong longyangsan","san"}));
			set("bonus","recover_jj");
			delete("secret_obj");
			set("gift_target",getuid(me));
		}

		if ( environment() == me ) {
			add_action("do_eat", "fu");
			add_action("do_eat", "eat");
		}
		set("cloned",1);
	}
	add_action("do_eat", "fu");
	add_action("do_eat", "eat");

	/*if ( ! query("secret_obj")) return;

	if ( query("gift_target") == getuid(me) )
	return;

	ob = find_player(query("gift_target"));

	if ( ! ob) return;

	if ( ob->query_temp("die_by_from") == me){
	ob->delete_temp("die_by_from");
	CHANNEL_D->do_channel( this_object(), "rumor",sprintf("%s弄到了一颗%s！", me->name(1), query("name")));
	set("gift_target", getuid(me));
	set("eat_time", uptime());
	}
	return;
	*/


}

string long()
{
	string str;

	str = query("name")+"("+capitalize(query("id"))+")\n";
	str += "一颗"+query("name")+"，服用(fu)以后，有";
	if ( query("secret_obj") )
	str += "大量";
	switch(query("bonus")) {
		case "combat_exp":      str +="增加实战经验";   break;
		case "max_jingli":      str +="提高精力";       break;
		case "max_neili":       str +="加大内力";       break;
		case "potential":       str +="增加潜能";       break;
		case "recover_jj":      str +="恢复真男之身";     break;
		default:                str +="一定";           break;
	}
	str += "的功效。\n";
	if (query("bonus")=="recover_jj")
	str += HIG"\n此乃西域奇药，据说服用之后，如果服用者有修炼辟邪剑法，那么将被放弃，不能恢复，甚之甚之！\n"NOR;

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

	if ( query("gift_target") != getuid(me) && query("bonus")=="combat_exp")
	  return notify_fail("你从什么地方得到的？\n");

	str = HIW"$N吃下一颗"+query("name")+HIW"，顿时觉得浑身为之一振，增加了";
	switch(query("bonus")) {
		case "combat_exp":
		if ( query("secret_obj") )
		i = 4000 + random(2000);
		else
		i = 500 + random(80);
		me->add("combat_exp", i);
		log_file("job/PRIZE",
		sprintf("%8s%-10s食用%6s，得到%4d点经验。",
		ctime(time())[4..19],
		me->name(1)+"("+getuid(me)+")",
		query("name"),
		i
		), me
		);
		str += chinese_number(i)+"点实战经验。\n"NOR;
		break;
		case "max_jingli":
		if ( query("secret_obj") )
		i = random(10) + 10;
		else
		i = random(3) + 2;
		me->add("max_jingli", i);
		me->add("eff_jingli", i);
		str += chinese_number(i)+"点最大精力。\n"NOR;
		break;
		case "max_neili":
		if ( query("secret_obj") )
		i = random(10) + 10;
		else
		i = random(3) + 2;
		max = (me->query_skill("force")-me->query_temp("apply/force")) * 8 + me->query("combat_exp") / 1000;
		if ( (me->query("max_neili") - 100 ) < max )
		me->add("max_neili", i);
		str += chinese_number(i)+"点最大内力。\n"NOR;
		break;
		case "potential":
		if ( query("secret_obj") )
		i = 500 + random(100);
		else
		i = random(50) + 300;
		me->add("potential", i);
		if ( me->query("potential") > me->query("max_pot"))
		me->set("potential", me->query("max_pot"));
		str += chinese_number(i)+"点潜能。\n"NOR;
		break;
		case "recover_jj":
		if (me->query("gender")!="无性" )
		{
			message_vision("$N偷偷的摸出一个药丸似的东西添了一下。\n",me);
			tell_object(me,HIR"你觉得体内燥热似火，全身欲爆的感觉！！\n"NOR);
			me->unconcious();
			return 1;//不dest
		}
		me->set("gender","男性");
		if (me->query("class") == "eunuch")
		me->delete("class");
		if (me->query_skill("pixie-jian",1)){
			tell_object(me,HIR"你心中回忆起当年练辟邪剑法的点点滴滴，终于决定要放弃。\n"NOR);
			log_file("static/ABANDON", sprintf("%s(%s) 吃药放弃了全部 %d 级 %s 在 %s\n",
			me->name(1), me->query("id"), me->query_skill("pixie-jian",1), to_chinese("pixie-jian") , ctime(time()) ));
			me->delete_skill("pixie-jian");
		}
		str = HIW"$N吃下一颗"+query("name")+HIW"，顿时觉得浑身为之一振，";
		str += "恢复了真男之身。\n";
		log_file("job/PRIZE",
		sprintf("%8s%-10s食用%6s，恢复了JJ。",
		ctime(time())[4..19],
		me->name(1)+"("+getuid(me)+")",
		query("name"),
		), me
		);
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