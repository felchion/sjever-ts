//  mujiang.c

inherit NPC;
inherit F_VENDOR;

int do_order(string arg);
int my_sell(string arg);
int ask_price();

void create()
{
	set_name("木匠", ({ "mu jiang", "mujiang", "mu", "jiang" }));
	set("shen_type", 1);

	set("gender", "男性");
	set("age", 30);
	set("long", "这位木匠正在埋头工作，因为发现你在注视着他，就抬起头来微笑了一下。\n");
	set_skill("unarmed", 25);
	set_skill("dodge", 22);
	set_temp("apply/damage", 20);

	set("combat_exp", 1000);
	set("attitude", "friendly");
	set("vendor_goods", ({
            (["name": BINGQI_D("mujian"),"number" : 99]),
	}));
	set("inquiry", ([
		"name": "在下姓张，不是自吹，说得上是方圆百里内手艺最好的木匠了。",
		"rumors": "这里连年打仗，百姓许多都出走了。幸好有郭大侠在，不然早就沦陷了。",
		"here": "这里是襄阳城，蒙古兵一般喜欢秋天来攻打。",
		"木人": "怎么，你也想订做(order)木人吗？现在要的人多，价钱可是不低呀。",
		"郭大侠": "就是郭靖呀，这里的人都尊称他为大侠。",
		"郭靖": "这里的人都尊称他为大侠。",
		"价钱": (: ask_price :),
		"价格": (: ask_price :),
		"price": (: ask_price :),
	]) );
	setup();
        set("chat_chance", 5);
        set("chat_msg", ({
		"木匠对你说道：可别小看了本店，就连少林的师傅们都要来这里订做木人呢！\n",
        }) );
        carry_object(ARMOR_D("cloth"))->wear();
        carry_object(ARMOR_D("shoes"))->wear();
}

void init()
{
        add_action("do_list", "list");
        add_action("do_buy", "buy");
	add_action("do_order", "order");
	add_action("do_order", "ding");
	add_action("my_sell", "sell");
}

int ask_price()
{
	object ob = this_player();
	int value, temp;

	if ((temp = ob->query_temp("muren_num")) >= 8) {
		command("say 拜托，小店可不是给您一个人开的！");
		return 1;
	}
	value = (ob->query("combat_exp") / 100000 + 1) * 30;
	value = value + temp * value / 2;
	command("say 如果是客官您订做的话，就收" + chinese_number(value) + "两银子好了。");
	return 1;
}

void muren_done(object ob)
{
	object muren;

	delete_temp("busy");
	if (objectp(ob)) {
		if (present(ob, environment())) {
			muren = new(__DIR__"muren");
			muren->set("owner", ob);
			muren->set("gender", ob->query("gender"));
			muren->set("name", ob->query("name") + "订做的木人");
			if (muren->move(ob)) {
				message_vision("木匠把做好的木人交给了$N。\n", ob);
			}
			else {
				muren->move(environment());
				message_vision("木匠把做好的给$N的木人放到了地上。\n", ob);
			}
			ob->add_temp("muren_num", 1);
		}
		else {
			tell_object(ob, "因为木匠找不到你了，所以订做被取消了。\n");
		}
	}
}

int my_sell(string arg)
{
	object ob;
	object me = this_player();
    int value, improve;

	if (me->is_busy() || me->is_fighting())
		return notify_fail("你正忙着呢。\n");
	if (!arg || !(ob = present(arg, me)))
		return notify_fail("你要卖什么？\n");

	if (ob->query("money_id"))
		return notify_fail("你想卖「钱」？？\n");

	if (ob->query("no_drop"))
		return notify_fail("这样东西不能卖。\n");

        if (query_temp("busy"))
                return notify_fail("哟，抱歉啊，我这儿正忙着呢……您请稍候。\n");

	if (ob->query("material") != "wood")
                return notify_fail("哟，抱歉啊，我这儿只收购废旧木头和你自己订做的木人。\n");

	if (arg != "mutou" && !ob->id("mu ren"))
                return notify_fail("哟，抱歉啊，我这儿只收购废旧木头和你自己订做的木人。\n");

	if (ob->id("mu ren")) {
		if (ob->query("owner") != me)
                	return notify_fail("咦，这个好象不是你从我这里订做的木人呀？！\n");
		else {
			value = (me->query("combat_exp") / 100000 + 1) * 2000;
			me->add_temp("muren_num", -1);
		}
	}

	if (arg == "mutou") value = 1000;
	message_vision("$N卖掉了一" + ob->query("unit") +
		ob->query("name") + "给$n。\n", me, this_object());
	MONEY_D->pay_player(me, value);
	improve = 40 - random(30);
	me->improve_skill("trade", improve / 2 + random(improve));
	destruct(ob);
	return 1;
}

int do_order(string arg)
{
	object ob = this_player();
	int value, temp;

	if (arg == "muren" || arg == "木人") {
		message_vision("$N想跟木匠订做一个木人。\n", ob);
		if (query_temp("busy")) {
			message_vision("木匠赶忙说：“哟，抱歉啊，我这儿正忙着呢……您请稍候。”\n", ob);
			return 1;
		}
		if ((temp = ob->query_temp("muren_num")) >= 8) {
			command("say 拜托，小店可不是给您一个人开的！");
			return 1;
		}
		value = (ob->query("combat_exp") / 100000 + 1) * 3000;
		value = value + temp * value / 2;
		switch (MONEY_D->player_pay(ob, value)) {
			case 0:
				return notify_fail("订做木人要" + chinese_number(value / 100) + "两白银！\n");
			case 2:
				return notify_fail("您的零钱不够了，银票又没人找得开。\n");
			default:
		        	set_temp("busy", 1);
				message_vision("木匠点点头，说：“这就开始做。”然后就开始埋头钉木人。\n", ob);
			        remove_call_out("muren_done");
			        call_out("muren_done", 10 + random(10), ob);
		}
		return 1;
	}
	return 0;
}

void kill_ob(object ob)
{
	ob->remove_killer(this_object());
	remove_killer(ob);
	message_vision("$N正要冲过去给木匠来一家伙，没想到木匠抬起头来瞪了一眼，$N只好沮丧地退了回来。\n", ob);
}

void unconcious()
{
	say( "木匠伸手在怀里摸了一阵，摸出一粒药丸来塞在嘴里，片刻之间就又恢复了精神。\n");
	reincarnate();
	set("eff_qi", query("max_qi"));
	set("qi", query("max_qi"));
	set("eff_jing", query("max_jing"));
	set("jing", query("max_jing"));
	set("jingli", query("eff_jingli"));
}

void die()
{
	unconcious();
}

