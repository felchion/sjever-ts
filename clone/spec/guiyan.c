//flowray 2005.6.16
#include <ansi.h>


inherit SPEC;

void create()
{
	set_name (HIB"鬼眼"NOR, ({ "guiyan","eye" }));
	set("long","这是一颗"HIB"鬼眼"NOR"，透过它你可以窥视(peek)其它玩家的状态。\n");
	set("unit","颗");
	set_weight(100);
	set("value",3000000);
		
	set("no_get",1);
	set("no_drop",1);
	set("no_get_from",1);
	
	set("degree",3);
	set("flag","spec/guiyan");
	set("desc","查看任意玩家的气血状态。");
	set("credit",2);
	setup();
}

void init()
{
	add_action("do_peek","peek");    
}

string status_color(int current, int max);

string conv(int i)
{
	if (i < 1000) return ""+i;
		return conv(i/1000)+sprintf(",%03d", i%1000);
}

int do_peek(string arg)
{
	object me=this_player(),ob;
	int lmt, i;
	mapping my;
	int last_on = strlen(arg);  
	
	if (!restrict()) {return notify_fail("本周你已经不能使用"+this_object()->query("name")+"了。\n");}
       
	if(!arg)
		return notify_fail("你想窥视哪个玩家。\n");           
	else{
        	while (last_on--) if (arg[last_on] < 'a' || arg[last_on] > 'z')
			{return notify_fail(arg+"是什么东东？\n");break;}
	}

	if(!objectp(ob = LOGIN_D->find_body(arg))) {
		ob = new(USER_OB);
		seteuid(arg);
		export_uid(ob);
		ob->set_name("", ({ arg }));
		if (!ob->restore())
			{ destruct(ob); return notify_fail("没有这个玩家。\n");}
	}
	
	if (wiz_level(me) < wiz_level(ob))
		return notify_fail(HIR"鬼眼突然发出耀眼的红光照得你两眼都睁不开。\n"NOR);

	if (ob == me)
		return notify_fail("你还是去照镜子吧！\n");

	if (ob->is_corpse() || !ob->is_character())
		return notify_fail("那不是活物耶！\n");

	my = ob->query_entire_dbase();

	if (userp(ob)){
		lmt = my["combat_exp"];
		for (i = to_int(pow(10.0 * lmt, 0.3333333));to_int(i*i/10.0*i)<=lmt;i++);
		my["max_pot"] = i+100;
	}

	if (!my["max_jing"]) my["max_jing"] = 1;
	if (!my["max_qi"]) my["max_qi"] = 1;
	write(HIY"冥冥中你看到了"+ob->name(1)+"的状态"NOR); 
	write(sprintf("\n·精血·%s%5d / %5d %s(%3d%%)" NOR "  ·精力·%s%5d / %5d%s(%d)\n" NOR,
		status_color(my["jing"], my["eff_jing"]+ob->query_temp("apply/jing")), my["jing"], my["eff_jing"]+ob->query_temp("apply/jing"),
		status_color(my["eff_jing"]+ob->query_temp("apply/jing"), my["max_jing"]), ((my["eff_jing"]+ob->query_temp("apply/jing")) * 100 / my["max_jing"]),
		status_color(my["jingli"], my["eff_jingli"]+ob->query_temp("apply/jingli")), my["jingli"], my["eff_jingli"]+ob->query_temp("apply/jingli"),
		status_color(my["jingli"], my["eff_jingli"]+ob->query_temp("apply/jingli")), my["eff_jingli"]-my["max_jingli"]+ob->query_skill("force")*8 ));
	write(sprintf("·气血·%s%5d / %5d %s(%3d%%)" NOR "  ·内力·%s%5d / %5d(+%d)\n" NOR,
		status_color(my["qi"], my["eff_qi"]+ob->query_temp("apply/qi")), my["qi"], my["eff_qi"]+ob->query_temp("apply/qi"),
		status_color(my["eff_qi"]+ob->query_temp("apply/qi"), my["max_qi"]), ((my["eff_qi"]+ob->query_temp("apply/qi")) * 100 / my["max_qi"]),
		status_color(my["neili"], my["max_neili"]+ob->query_temp("apply/neili")), my["neili"], my["max_neili"]+ob->query_temp("apply/neili"),
		my["jiali"] ));

	write(sprintf("·%s气·%s %-13s   " NOR " ·内力上限·"HIC"%5d / %5d\n" NOR,
		my["shen"] < 0?"戾":"正",
		my["shen"] < 0?HIR:HIC,
		conv((my["shen"] < 0?-1:1) * my["shen"]),
		(ob->query_skill("force")-ob->query_temp("apply/force"))*8 + my["combat_exp"]/1000 + ob->query_temp("apply/neili"),
		(ob->query_skill("force")-ob->query_temp("apply/force"))*10 + my["age"]*20 + my["combat_exp"]/1000 + ob->query_temp("apply/neili")));

	i = ob->max_food_capacity();
	if (!i) i = 1;
	write(sprintf("·食物·%s%7.2f%%	 " NOR "     ·潜能·"HIY" %4d / %4d\n" NOR,
		status_color(my["food"], i),
		my["food"]*100.0/i, my["potential"], my["max_pot"] ));
	i = ob->max_water_capacity();
	if (!i) i = 1;
	write(sprintf("·饮水·%s%7.2f%%	 " NOR "     ·经验·"HIM" %s\n"NOR,
		status_color(my["water"], i),
		my["water"]*100.0/i,
		conv(ob->query("id") == "zhong shentong"?my["combat_exp"]/3*2:my["combat_exp"])));
	if (!find_player(arg)) destruct(ob);
	degree(); 
	return 1;
}

string status_color(int current, int max)
{
	int percent;

	if( max ) percent = current * 100 / max;
	else percent = 0;
	if( percent > 100 ) return HIC;
	if( percent >= 90 ) return HIG;
	if( percent >= 60 ) return HIY;
	if( percent >= 30 ) return YEL;
	if( percent >= 10 ) return HIR;
	return RED;
}