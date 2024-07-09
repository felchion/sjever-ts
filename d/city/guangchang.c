// Room: /d/city/guangchang.c

inherit ROOM;

int do_zhai(string arg);
int do_shake(string arg);
int do_strike(string arg);
int do_sling(string arg);
int do_climb(string arg);

void create()
{
	set("short", "中央广场");
	set("long", @LONG
这里是城市的正中心，一个很宽阔的广场，铺着青石地面。一些游手好闲
的人在这里溜溜达达，经常有艺人在这里表演。你可以看到东边和南边是几家
大的店铺，来自全国各地的行人来来往往，西面是扬州戌守兵营，而北边则是
扬州巡抚衙门的所在。广场中央有一棵老桃树(tree)，显得十分的粗壮，树上
的树叶(leaf)非常茂密，有几只不知名的小鸟在其中窜来窜去。大树旁边有一
块新立的牌子(sign)。
LONG
	);
        set("no_sleep_room", 1);
        set("outdoors", "扬州");

	set("exits", ([
		"east" : __DIR__"guangchangdong",
		"south" : __DIR__"guangchangnan",
		"west" : __DIR__"guangchangxi",
		"north" : __DIR__"guangchangbei",
	]));

	set("item_desc", ([
		"tree" : "一棵大树，枝叶茂密，看不清上面的情况。枝头挂满了桃子，已经熟地要掉下来了。\n",
		"leaf" : "树叶松松地挂在树上，看上去很想把它摘下来。\n",
		"sign" : "请爱护花草树木，攀爬损毁者重罚！    --扬州府告示\n",
	]) );
	set("objects", ([
		__DIR__"npc/xunbu" : 1,
		__DIR__"npc/zhucong" : 1,
		__DIR__"npc/gongzi" : 1,
		__DIR__"npc/yiren" : 1,
		__DIR__"npc/parrot" : 1,
	]));
	set("stick_count", 5);
	set("peach_count", 30);

	set("incity",1);
	setup();
}

void init()
{
	object room;

	add_action("do_sling", "sling");
	add_action("do_sling", "tan");
	add_action("do_sling", "shoot");
	add_action("do_sling", "she");
	add_action("do_shake", "shake");
	add_action("do_shake", "wave");
	add_action("do_shake", "yao");
	add_action("do_shake", "huang");
	add_action("do_strike", "strike");
	add_action("do_strike", "press");
	add_action("do_strike", "hit");
	add_action("do_strike", "pai");
	add_action("do_strike", "ji");
	add_action("do_climb", "climb");
	add_action("do_climb", "pa");
	add_action("do_zhai", "zhai");
	if (!wizardp(this_player()) && room = find_object(__DIR__"shushang"))
		tell_room(room, "透过树叶的缝隙，你看见" + this_player()->name() + "来到了树下。\n");
}

int do_zhai(string arg)
{
	object me = this_player();

	if (arg != "leaf" && arg != "shuye" && arg != "ye")
		return notify_fail("你要摘什么？\n");

	message_vision("$N伸手去够树上的叶子，发现手臂需要再长几十尺才行！\n", me);
	return 1;
}

int do_sling(string arg)
{
	object weapon, room, xunbu, money_ob, *ob;
	object me = this_player();
	int i, qi, level, jinglicost, topay, temp;
	string weapon_name, money_str = "";

	if (arg != "leaf" && arg != "shuye" && arg != "ye")
		return notify_fail("你要射什么？\n");

	if (me->is_busy() || me->is_fighting())
		return notify_fail("你正忙着哪！\n");

	weapon = me->query_temp("weapon");
	if (!weapon) {
		message_vision("$N对着树上的叶子运气，又蹦又跳试图把它摘下来，却始终没法够到。\n", me);
		return 1;
	}
	weapon_name = weapon->query("name");
	if (weapon->query("skill_type") != "throwing") {
		message_vision("$N用手中的" + weapon_name + "去够树上的叶子，却不够长。\n", me);
		return 1;
	}
	weapon_name = weapon->query("base_unit") + weapon->query("name");
	xunbu = present("xunbu", this_object());
	if (xunbu && !userp(xunbu)) {
		message_vision("$N掏出一" + weapon_name + "，正准备对着树叶弹去，巡捕忽然飞起一脚，将$N踢了一个跟头。\n", me);
		message_vision("巡捕对着$N喝道：“牌子上斗大的字让你爱护花草树木，你如何还敢再犯？”\n", me);
		me->add_temp("damage_yz_tree", 1);
		me->start_busy(1);
		topay = me->query_temp("damage_yz_tree");
		if (topay < 3) {
			message_vision("巡捕对着$N喝道：“下次被我抓到，定不轻饶！”\n", me);
			return 1;
		}
		else {
			message_vision("巡捕对着$N喝道：“罚款" + CHINESE_D->chinese_number(topay * 5) + "文铜钱！”\n", me);
			switch (MONEY_D->player_pay(me, topay * 5)) {
				case 0:
					message_vision("$N苦着脸道：“大人，小民身上钱不够，如何是好？”\n", me);
					topay = 0;
					if (money_ob = present("gold_money", me)) {
						money_str = CHINESE_D->chinese_number(money_ob->query_amount()) + "两黄金";
						money_ob->set_amount(0);
						topay = 1;
					}
					if (money_ob = present("silver_money", me)) {
						if (topay)
							money_str = money_str + "、" + CHINESE_D->chinese_number(money_ob->query_amount()) + "两白银";
						else
							money_str = CHINESE_D->chinese_number(money_ob->query_amount()) + "两白银";
						money_ob->set_amount(0);
						topay = 1;
					}
					if (money_ob = present("coin_money", me)) {
						if (topay)
							money_str = money_str + "、" + CHINESE_D->chinese_number(money_ob->query_amount()) + "文铜钱";
						else
							money_str = CHINESE_D->chinese_number(money_ob->query_amount()) + "文铜钱";
						money_ob->set_amount(0);
						topay = 1;
					}
					if (topay)
						message_vision("巡捕大怒，抓住$N的两脚倒提起来，使劲摇了半天，从$N身上掉下来" + money_str + "。\n巡捕捡起钱，悻悻地说：“这次饶了你，下次你可给我小心点！”\n", me);
					else
						message_vision("巡捕大怒，抓住$N的两脚倒提起来，使劲摇了半天，可是$N身上连一个铜板都没有掉下来。\n巡捕瞪着眼道：“没带钱？！找死呀你！这次饶了你，下次你可给我小心点！”\n", me);
					return 1;
				case 2:
					message_vision("$N掏出一张银票，小心翼翼地问道：“大人，小民只有银票在身上，您老能否通融一些？”\n", me);
					message_vision("巡捕一撇嘴，伸手将银票拿了过去，道：“在我面前扮大款？！多出来的就当你请我喝酒好了。”\n", me);
					money_ob = present("thousand-cash", me);
					money_ob->set_amount(money_ob->query_amount() - 1);
					return 1;
				default:
					message_vision("$N付了罚款，一脸的不高兴。\n", me);
					return 1;
			}
		}
	}
	if (room = find_object(__DIR__"shushang")) {
//		room = load_object(__DIR__"shushang");
		temp = 18 - me->query("str") / 5;
		if (temp < 1) temp = 1;
		if (!random(temp)) {
			ob = all_inventory(room);
			for (i = 0; i < sizeof(ob); i++) {
				if (userp(ob[i]) && !wizardp(ob[i])) {
					tell_object(ob[i], me->name() + "掏出一" + weapon_name + "，对着树上弹来，却正好击中你，你脚下一个不稳，掉了下去。\n");
					tell_room(room, me->name() + "掏出一" + weapon_name + "，对着树上弹来，却正好击中" + ob[i]->name() + "，" + ob[i]->name() + "脚下一个不稳，掉了下去。\n", ({ob[i]}));
					write("你掏出一" + weapon_name + "，对准树叶弹去，只听“啊”地一声惨叫，一个人从树上掉了下来，仔细一看，却是" + ob[i]->name() + "。\n");
					tell_room(this_object(), me->name() + "掏出一" + weapon_name + "，对准树叶弹去，只听“啊”地一声惨叫，一个人从树上掉了下来，仔细一看，却是" + ob[i]->name() + "。\n", ({me}));
					ob[i]->move(this_object());
					qi = ob[i]->query("qi");
					if (qi > 200) ob[i]->receive_damage("qi", qi / 2);
					else if (qi >= 100) ob[i]->receive_damage("qi", 100);
					else if (qi > 0) ob[i]->receive_damage("qi", qi);
					return 1;
				}
			}
		}
	}
	level = me->query_skill("throwing", 1);
	jinglicost = level / 3 + random(level / 3);
        if (me->query("jingli") <= jinglicost) {
		write("你太累了，先歇歇吧！\n");
		return 1;
	}
	if (level < 30) {
		write("你掏出一" + weapon_name + "，对准树叶弹去，却因为指劲太弱而没有弹到。\n");
		WEAPON_D->throw_weapon(me, me, weapon, 0);
		return 1;
	}
	me->receive_damage("jingli", jinglicost);
	if (level <= 100) {
		write("你掏出一" + weapon_name + "，对准树叶弹去，只听“嗤”地声响过后，一片树叶应声而落。\n");
		WEAPON_D->throw_weapon(me, me, weapon, 0);
		me->improve_skill("throwing", me->query("int"));
	}
	else {
		if (query("stick_count")) {
			message_vision("$N掏出一" + weapon_name + "，对准树叶弹去，结果用劲过大，连树枝都敲了下来，暗器功夫上却没有新进展。\n", me);
			add("stick_count", -1);
			new(__DIR__"obj/stick.c")->move(this_object());
		}
		else
			write("你掏出一" + weapon_name + "，对准树叶弹去，发现这样练习太轻松了，暗器功夫上已经无法取得新进展。\n");
		WEAPON_D->throw_weapon(me, me, weapon, 0);
	}
	return 1;
}

int do_shake(string arg)
{
	object room, peachroom, xunbu, money_ob, *ob;
	object me = this_player();
	int i, qi, level, jinglicost, topay, temp;
	string money_str = "";

	if (arg != "tree" && arg != "shu" && arg != "dashu")
		return notify_fail("你要摇什么？\n");

	if (me->is_busy() || me->is_fighting())
		return notify_fail("你正忙着哪！\n");

	xunbu = present("xunbu", this_object());
	if (xunbu && !userp(xunbu)) {
		message_vision("$N抱着树干，正准备摇，巡捕忽然飞起一脚，将$N踢了一个跟头。\n", me);
		message_vision("巡捕对着$N喝道：“牌子上斗大的字让你爱护花草树木，你如何还敢再犯？”\n", me);
		me->add_temp("damage_yz_tree", 1);
		me->start_busy(1);
		topay = me->query_temp("damage_yz_tree");
		if (topay < 3) {
			message_vision("巡捕对着$N喝道：“下次被我抓到，定不轻饶！”\n", me);
			return 1;
		}
		else {
			message_vision("巡捕对着$N喝道：“罚款" + CHINESE_D->chinese_number(topay * 5) + "文铜钱！”\n", me);
			switch (MONEY_D->player_pay(me, topay * 5)) {
				case 0:
					message_vision("$N苦着脸道：“大人，小民身上钱不够，如何是好？”\n", me);
					topay = 0;
					if (money_ob = present("gold_money", me)) {
						money_str = CHINESE_D->chinese_number(money_ob->query_amount()) + "两黄金";
						money_ob->set_amount(0);
						topay = 1;
					}
					if (money_ob = present("silver_money", me)) {
						if (topay)
							money_str = money_str + "、" + CHINESE_D->chinese_number(money_ob->query_amount()) + "两白银";
						else
							money_str = CHINESE_D->chinese_number(money_ob->query_amount()) + "两白银";
						money_ob->set_amount(0);
						topay = 1;
					}
					if (money_ob = present("coin_money", me)) {
						if (topay)
							money_str = money_str + "、" + CHINESE_D->chinese_number(money_ob->query_amount()) + "文铜钱";
						else
							money_str = CHINESE_D->chinese_number(money_ob->query_amount()) + "文铜钱";
						money_ob->set_amount(0);
						topay = 1;
					}
					if (topay)
						message_vision("巡捕大怒，抓住$N的两脚倒提起来，使劲摇了半天，从$N身上掉下来" + money_str + "。\n巡捕捡起钱，悻悻地说：“这次饶了你，下次你可给我小心点！”\n", me);
					else
						message_vision("巡捕大怒，抓住$N的两脚倒提起来，使劲摇了半天，可是$N身上连一个铜板都没有掉下来。\n巡捕瞪着眼道：“没带钱？！找死呀你！这次饶了你，下次你可给我小心点！”\n", me);
					return 1;
				case 2:
					message_vision("$N掏出一张银票，小心翼翼地问道：“大人，小民只有银票在身上，您老能否通融一些？”\n", me);
					message_vision("巡捕一撇嘴，伸手将银票拿了过去，道：“在我面前扮大款？！多出来的就当你请我喝酒好了。”\n", me);
					money_ob = present("thousand-cash", me);
					money_ob->set_amount(money_ob->query_amount() - 1);
					return 1;
				default:
					message_vision("$N付了罚款，一脸的不高兴。\n", me);
					return 1;
			}
		}
	}
	if (room = find_object(__DIR__"shushang")) {
//		room = load_object(__DIR__"shushang");
		temp = 15 - me->query("str") / 5;
		if (temp < 1) temp = 1;
		if (!random(temp)) {
			ob = all_inventory(room);
			for (i = 0; i < sizeof(ob); i++) {
				if (userp(ob[i]) && !wizardp(ob[i])) {
					tell_object(ob[i], me->name() + "使劲地摇晃着大树，你脚下一个不稳，掉了下去。\n");
					tell_room(room, me->name() + "使劲地摇晃着大树，" + ob[i]->name() + "脚下一个不稳，掉了下去。\n", ({ob[i]}));
					write("你使劲地摇晃着大树，忽听“哗啦”一声，一个人从树上掉了下来，仔细一看，却是" + ob[i]->name() + "。\n");
					tell_room(this_object(), me->name() + "使劲地摇晃着大树，忽听“哗啦”一声，一个人从树上掉了下来，仔细一看，却是" + ob[i]->name() + "。\n", ({me}));
					ob[i]->move(this_object());
					if (!random(3)) {
						qi = ob[i]->query("qi");
						if (qi > 200) ob[i]->receive_damage("qi", qi / 2);
						else if (qi >= 100) ob[i]->receive_damage("qi", 100);
						else if (qi > 0) ob[i]->receive_damage("qi", qi);
					}
					return 1;
				}
			}
		}
	}
	level = me->query_skill("parry", 1);
	jinglicost = level / 3 + random(level / 3);
        if (me->query("jingli") <= jinglicost) {
		write("你太累了，先歇歇吧！\n");
		return 1;
	}
	if (level < 30) {
		write("你使劲地摇晃着大树，大树却纹丝不动，你不禁想到那句“蚍蜉撼树”的成语来。\n");
		return 1;
	}
	if (level > 100) {
		write("你使劲地摇晃着大树，把树上的桃子都摇得落到几丈开外去了。\n");
		if (!random(8) && query("peach_count")) {
			add("peach_count", -1);
			switch (random(4)) {
				case 0:
					if (!(peachroom = find_object(__DIR__"guangchangbei")))
						peachroom = load_object(__DIR__"guangchangbei");
					break;

				case 1:
					if (!(peachroom = find_object(__DIR__"guangchangxi")))
						peachroom = load_object(__DIR__"guangchangxi");
					break;

				case 2:
					if (!(peachroom = find_object(__DIR__"guangchangnan")))
						peachroom = load_object(__DIR__"guangchangnan");
					break;

				case 3:
					if (!(peachroom = find_object(__DIR__"guangchangdong")))
						peachroom = load_object(__DIR__"guangchangdong");
					break;
			}
			new(__DIR__"obj/peach")->move(peachroom);
			tell_room(peachroom, "忽然从广场中心那边飞过来一个桃子，差点砸到你！\n", ({}));
		}
		return 1;
	}
	me->receive_damage("jingli", jinglicost);
	if (!random(5))
		message_vision("$N使劲地摇着大树，从树上掉下几个桃子来，砸向$N的头脸，$N赶忙伸臂架开。\n", me);
	else
		write("你使劲地摇着大树，从树上掉下几个桃子来，砸向你的头脸，你赶忙伸臂架开。\n");
	me->improve_skill("parry", me->query("int"));
	return 1;
}

int do_strike(string arg)
{
	object room, peachroom, xunbu, money_ob, *ob;
	object me = this_player();
	int i, qi, level, jinglicost, topay, temp;
	string money_str = "";

	if (arg != "tree" && arg != "shu" && arg != "dashu")
		return notify_fail("你要拍什么？\n");

	if (me->is_busy() || me->is_fighting())
		return notify_fail("你正忙着哪！\n");

	xunbu = present("xunbu", this_object());
	if (xunbu && !userp(xunbu)) {
		message_vision("$N伸掌拍向大树，巡捕忽然飞起一脚，将$N踢了一个跟头。\n", me);
		message_vision("巡捕对着$N喝道：“牌子上斗大的字让你爱护花草树木，你如何还敢再犯？”\n", me);
		me->add_temp("damage_yz_tree", 1);
		me->start_busy(1);
		topay = me->query_temp("damage_yz_tree");
		if (topay < 3) {
			message_vision("巡捕对着$N喝道：“下次被我抓到，定不轻饶！”\n", me);
			return 1;
		}
		else {
			message_vision("巡捕对着$N喝道：“罚款" + CHINESE_D->chinese_number(topay * 5) + "文铜钱！”\n", me);
			switch (MONEY_D->player_pay(me, topay * 5)) {
				case 0:
					message_vision("$N苦着脸道：“大人，小民身上钱不够，如何是好？”\n", me);
					topay = 0;
					if (money_ob = present("gold_money", me)) {
						money_str = CHINESE_D->chinese_number(money_ob->query_amount()) + "两黄金";
						money_ob->set_amount(0);
						topay = 1;
					}
					if (money_ob = present("silver_money", me)) {
						if (topay)
							money_str = money_str + "、" + CHINESE_D->chinese_number(money_ob->query_amount()) + "两白银";
						else
							money_str = CHINESE_D->chinese_number(money_ob->query_amount()) + "两白银";
						money_ob->set_amount(0);
						topay = 1;
					}
					if (money_ob = present("coin_money", me)) {
						if (topay)
							money_str = money_str + "、" + CHINESE_D->chinese_number(money_ob->query_amount()) + "文铜钱";
						else
							money_str = CHINESE_D->chinese_number(money_ob->query_amount()) + "文铜钱";
						money_ob->set_amount(0);
						topay = 1;
					}
					if (topay)
						message_vision("巡捕大怒，抓住$N的两脚倒提起来，使劲摇了半天，从$N身上掉下来" + money_str + "。\n巡捕捡起钱，悻悻地说：“这次饶了你，下次你可给我小心点！”\n", me);
					else
						message_vision("巡捕大怒，抓住$N的两脚倒提起来，使劲摇了半天，可是$N身上连一个铜板都没有掉下来。\n巡捕瞪着眼道：“没带钱？！找死呀你！这次饶了你，下次你可给我小心点！”\n", me);
					return 1;
				case 2:
					message_vision("$N掏出一张银票，小心翼翼地问道：“大人，小民只有银票在身上，您老能否通融一些？”\n", me);
					message_vision("巡捕一撇嘴，伸手将银票拿了过去，道：“在我面前扮大款？！多出来的就当你请我喝酒好了。”\n", me);
					money_ob = present("thousand-cash", me);
					money_ob->set_amount(money_ob->query_amount() - 1);
					return 1;
				default:
					message_vision("$N付了罚款，一脸的不高兴。\n", me);
					return 1;
			}
		}
	}
	if (room = find_object(__DIR__"shushang")) {
//		room = load_object(__DIR__"shushang");
		temp = 16 - me->query("str") / 5;
		if (temp < 1) temp = 1;
		if (!random(temp)) {
			ob = all_inventory(room);
			for (i = 0; i < sizeof(ob); i++) {
				if (userp(ob[i]) && !wizardp(ob[i])) {
					tell_object(ob[i], me->name() + "一掌拍向大树，你脚下一个不稳，掉了下去。\n");
					tell_room(room, me->name() + "一掌拍向大树，" + ob[i]->name() + "脚下一个不稳，掉了下去。\n", ({ob[i]}));
					write("你一掌拍向大树，忽听“哗啦”一声，一个人从树上掉了下来，仔细一看，却是" + ob[i]->name() + "。\n");
					tell_room(this_object(), me->name() + "一掌拍向大树，忽听“哗啦”一声，一个人从树上掉了下来，仔细一看，却是" + ob[i]->name() + "。\n", ({me}));
					ob[i]->move(this_object());
					if (!random(3)) {
						qi = ob[i]->query("qi");
						if (qi > 200) ob[i]->receive_damage("qi", qi / 2);
						else if (qi >= 100) ob[i]->receive_damage("qi", 100);
						else if (qi > 0) ob[i]->receive_damage("qi", qi);
					}
					return 1;
				}
			}
		}
	}
	level = me->query_skill("strike", 1);
	jinglicost = level / 3 + random(level / 3);
        if (me->query("jingli") <= jinglicost) {
		write("你太累了，先歇歇吧！\n");
		return 1;
	}
	if (level < 30) {
		write("你一掌拍向大树，大树却纹丝不动，你感到十分沮丧。\n");
		return 1;
	}
	if (level > 100) {
		write("你一掌拍向大树，把树上的桃子都震得落到几丈开外去了。\n");
		if (!random(8) && query("peach_count")) {
			add("peach_count", -1);
			switch (random(4)) {
				case 0:
					if (!(peachroom = find_object(__DIR__"guangchangbei")))
						peachroom = load_object(__DIR__"guangchangbei");
					break;

				case 1:
					if (!(peachroom = find_object(__DIR__"guangchangxi")))
						peachroom = load_object(__DIR__"guangchangxi");
					break;

				case 2:
					if (!(peachroom = find_object(__DIR__"guangchangnan")))
						peachroom = load_object(__DIR__"guangchangnan");
					break;

				case 3:
					if (!(peachroom = find_object(__DIR__"guangchangdong")))
						peachroom = load_object(__DIR__"guangchangdong");
					break;
			}
			new(__DIR__"obj/peach")->move(peachroom);
			tell_room(peachroom, "忽然从广场中心那边飞过来一个桃子，差点砸到你！\n", ({}));
		}
		return 1;
	}
	me->receive_damage("jingli", jinglicost);
	if (!random(5))
		message_vision("$N一掌拍向大树，大树晃了几晃，$N对于自己的掌法更有自信了。\n", me);
	else
		write("你一掌拍向大树，大树晃了几晃，你对于自己的掌法更有自信了。\n");
	me->improve_skill("strike", me->query("int"));
	return 1;
}

int do_climb(string arg)
{
	object xunbu, money_ob, me = this_player();
	int topay;
	string money_str = "";

	if (arg != "tree" && arg != "shu" && arg != "dashu" && arg != "up")
		return notify_fail("你要爬什么？\n");
	if (me->is_busy() || me->is_fighting())
		return notify_fail("你正忙着哪！\n");
	xunbu = present("xunbu", this_object());
	if (xunbu && !userp(xunbu)) {
		message_vision("$N往手心里啐了两口唾沫，运了运气，往树上爬去。刚爬了几下，巡捕忽然伸手将$N从树干上拽了下来。\n", me);
		message_vision("巡捕对着$N喝道：“牌子上斗大的字让你爱护花草树木，你如何还敢再犯？”\n", me);
		me->add_temp("damage_yz_tree", 1);
		me->start_busy(1);
		topay = me->query_temp("damage_yz_tree");
		if (topay < 3) {
			message_vision("巡捕对着$N喝道：“下次被我抓到，定不轻饶！”\n", me);
			return 1;
		}
		else {
			message_vision("巡捕对着$N喝道：“罚款" + CHINESE_D->chinese_number(topay * 5) + "文铜钱！”\n", me);
			switch (MONEY_D->player_pay(me, topay * 5)) {
				case 0:
					message_vision("$N苦着脸道：“大人，小民身上钱不够，如何是好？”\n", me);
					topay = 0;
					if (money_ob = present("gold_money", me)) {
						money_str = CHINESE_D->chinese_number(money_ob->query_amount()) + "两黄金";
						money_ob->set_amount(0);
						topay = 1;
					}
					if (money_ob = present("silver_money", me)) {
						if (topay)
							money_str = money_str + "、" + CHINESE_D->chinese_number(money_ob->query_amount()) + "两白银";
						else
							money_str = CHINESE_D->chinese_number(money_ob->query_amount()) + "两白银";
						money_ob->set_amount(0);
						topay = 1;
					}
					if (money_ob = present("coin_money", me)) {
						if (topay)
							money_str = money_str + "、" + CHINESE_D->chinese_number(money_ob->query_amount()) + "文铜钱";
						else
							money_str = CHINESE_D->chinese_number(money_ob->query_amount()) + "文铜钱";
						money_ob->set_amount(0);
						topay = 1;
					}
					if (topay)
						message_vision("巡捕大怒，抓住$N的两脚倒提起来，使劲摇了半天，从$N身上掉下来" + money_str + "。\n巡捕捡起钱，悻悻地说：“这次饶了你，下次你可给我小心点！”\n", me);
					else
						message_vision("巡捕大怒，抓住$N的两脚倒提起来，使劲摇了半天，可是$N身上连一个铜板都没有掉下来。\n巡捕瞪着眼道：“没带钱？！找死呀你！这次饶了你，下次你可给我小心点！”\n", me);
					return 1;
				case 2:
					message_vision("$N掏出一张银票，小心翼翼地问道：“大人，小民只有银票在身上，您老能否通融一些？”\n", me);
					message_vision("巡捕一撇嘴，伸手将银票拿了过去，道：“在我面前扮大款？！多出来的就当你请我喝酒好了。”\n", me);
					money_ob = present("thousand-cash", me);
					money_ob->set_amount(money_ob->query_amount() - 1);
					return 1;
				default:
					message_vision("$N付了罚款，一脸的不高兴。\n", me);
					return 1;
			}
		}
	}
	message_vision("$N往手心里啐了两口唾沫，运了运气，顺着树干爬了上去。\n", me);
	me->move(__DIR__"shushang", 1);
	message("vision", me->name() + "从树下爬了上来。\n", __DIR__"shushang", ({me}));
	return 1;
}

int valid_leave(object me, string dir)
{
	object room;
	int valid;

	valid = ::valid_leave(me, dir);
	if (valid && !wizardp(me) && room = find_object(__DIR__"shushang"))
		tell_room(room, "透过树叶的缝隙，你看见" + me->name() + "离开了树下。\n");
	return valid;
}
/*
void reset()
{
mixed *ob_list;
int i;

ob_list=objects();
for(i=0;i<sizeof(ob_list);i++)
ob_list[i]->delete_temp("vendor_goods");
::reset();
return;
}
*/
