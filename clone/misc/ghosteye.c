#include <ansi.h>

inherit ITEM;


void create()
{
       seteuid(getuid());
	set_name (HIB"鬼眼"NOR, ({ "ghosteye","eye" }));
	set("long",@LONG
这是一颗鬼眼，透过它你可以窥视(peek)其它玩家的状态。
LONG
);

	set("unit","颗");
	set("no_get", 1);
	set("no_drop", 1);
	set_weight(-100000);
	setup();
}

void init()
{
    seteuid(geteuid());
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
	object me=this_player(),ob,body;
	int lmt, i;
	mapping my;
       int last_on = strlen(arg);
	string dinfo;        
	seteuid(ROOT_UID);

       if(!arg)
                return notify_fail("你想窥视哪个玩家。\n");           
       else{
         while (last_on--) if (arg[last_on] < 'a' || arg[last_on] > 'z')
		    {return notify_fail(arg+"是什么东东？\n");break;}
       }

	if(!objectp(ob = LOGIN_D->find_body(arg))) {
	   body = new(LOGIN_OB);
	   body->set("id", arg);
          dinfo=geteuid();
          if( !body->restore() ) {destruct(body);return notify_fail(dinfo+"没有这个玩家。\n");}
	   ob = LOGIN_D->make_body(body);
	   if( !ob->restore() ) {destruct(ob); destruct(body); return notify_fail("没有这个人。\n");}
	}



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
       if (!find_player(arg)) destruct(body);
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