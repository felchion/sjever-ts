// wang zhanggui
// by augx@sj  3/21/2002

inherit NPC;
inherit F_VENDOR;
#include <ansi.h>
#include <group.h>

void create()
{
	set_name("王掌柜", ({"wang zhanggui", "wang", "zhanggui"}));
	set("title", "材料行老板");
	set("gender", "男性");
	set("long", "这位王掌柜已经在此经验材料行多年，信誉很好，还有很多江湖帮会有来往。\n在这里买东西还可以使用银票。\n");
	set("age", 50);

	set("no_kill",1);
	set("no_ansuan",1);
	set("no_bark",1);
	set("no_get", 1);

	set("max_qi", 99999);
	set("qi", 99999);
	set("max_jing", 99999);
	set("jing", 99999);
	set("max_neili", 99999);
	set("neili", 99999);
	set("eff_jingli", 99999);
	set("jingli", 99999);

	set("combat_exp",5000000);
	set("chat_chance", 1);
	set("chat_msg", ({
		"王掌柜骄傲的说道：本行的建筑材料和都是货真价实的啊。\n",
		"王掌柜说道：我和很多帮会的大哥都有关系噢，别想在我的店里惹是生非。\n"
	}));

	set_skill("unarmed", 350);
	set_skill("dodge", 350);
	set_skill("parry", 400);
	set_temp("apply/attack", 300);
	set_temp("apply/defense", 300);
	set_temp("apply/damage", 30);

	set("vendor_goods", ({
		(["name": "/d/group/obj/biaozhi.c","number":5]),
		(["name": "/d/group/obj/qiangui.c","number":5]),
		(["name": "/d/group/obj/shiliao.c","number":20]),
		(["name": "/d/group/obj/muliao.c","number":20]),
		(["name": "/d/group/obj/shihui.c","number":30]),
		(["name": "/d/group/obj/door.c","number":5]),
		(["name": "/d/group/obj/board.c","number":5]),
		(["name": "/d/group/obj/guard.c","number":10]),
	}));

	setup();
	carry_object(ARMOR_D("cloth"))->wear();
	carry_object(ARMOR_D("shoes"))->wear();
}

void init()
{
	add_action("do_list", "list");
	add_action("do_buy", "buy");
}

int query_goods_value(string goods, int number)
{
	object me = this_player();
	int value, trade_level;
	int cur;

	value = ((object)goods)->query("value");
	trade_level = me->query_skill("trade", 1) / 10;
	if (trade_level > 20) trade_level = 20;
	value = value /100 * (120 - trade_level);

	cur = query_temp("vendor_goods/"+goods);
	if( cur!=number ) value += value/5 /number*cur;

	return value;
}

int do_buy(string arg)
{
	int i, improve, delay;
	string ob_file;
	object ob;
	object me = this_player();
	mapping goods;

	if (!living(this_object()))
		return notify_fail("还是等对方醒来再说吧。\n");
	if (me->is_busy() || me->is_fighting())
		return notify_fail("你正忙着呢。\n");
	if (!me->query("group/id"))
		return notify_fail("你又没有加入帮会，要买这些东西干什么？\n");
	if (query_temp("busy"))
		return notify_fail("哟，抱歉啊，我这儿正忙着呢……您请稍候。\n");
	if (!(int)query("move_seller") && base_name(environment()) != query("startroom"))
		return notify_fail("这……我这次是出来游玩的，你下次来吧。\n");
	if (!arg) return notify_fail("你想买什么？\n");
	if ((ob_file = is_vendor_good(arg)) == "")
		return notify_fail("你想买的东西我这里没有。\n");
	if( !GROUP_D->site_now(me->query("group/id")) )
		return notify_fail( GROUP_D->get_last_error() );

	goods=query("vendor_goods");
	for(i=0;i<sizeof(goods);i++)
		if(goods[i]["name"]==ob_file)
			break;

	ob = new(ob_file);
	if( ob->query("guard") && file_size("/d/group/"+me->query("group/id"))!=-2 ) {
		destruct(ob);
		return notify_fail("你的帮派还没有建起来呢，要买保镖干什么？\n");
	}

	i = query_goods_value(ob_file,goods[i]["number"]);

	if( me->query("group/id") && me->query("group/class")<=3
	    && (GROUP_D->query_money(me->query("group/id"))-LIMIT_GOLD)>(i/10000+1) ) {
		i = i/10000 + 1;
		if( GROUP_D->add_money(me->query("group/id"),-i) < -50000 ) {
			destruct(ob);
			return notify_fail("数据库操作失败，请报告巫师！\n");
		}
		i *= 10000;
	}
	else {
		if( MONEY_D->player_pay(me,i,1) == 0 ) {
			destruct(ob);
			return notify_fail("穷光蛋，一边呆着去！\n");
		}
	}

	if( ob->query("guard") ) {
		message_vision("$N想以"+MONEY_D->price_str(i)+"的价格从$n那里雇佣一位" + ob->name() + "。\n", me, this_object());
		ob_file = ob->query("guard_file");
		destruct(ob);

		ob_file = GROUP_D->create_group_npc(ob_file,me);
		if( strlen(ob_file)>3 && ob_file[0..2]!="/d/" )
			return notify_fail(ob_file);

		delay = 10;
		if( wizardp(me) && me->query("env/test") ) delay = 2;
		me->start_busy(delay);
		call_out("create_group_npc_finish",delay*2-1,me,ob_file);
	}
	else {
		if (!ob->move(me)) {
			destruct(ob);
			return 0;
		}
		message_vision("$N以"+MONEY_D->price_str(i)+"的价格从$n那里买下了一" + ob->query("unit") + ob->name() + "。\n", me, this_object());
	}
	improve = 40 - random(30);
	me->improve_skill("trade", improve + random(improve));
	add_temp("vendor_goods/"+ob_file,1);

	set_temp("busy", 1);
	call_out("delete_temp", 2, "busy");

	return 1;
}

void create_group_npc_finish(object me,string file)
{
	object ob;
	string * ids;

	if( !me ) return;
	if( !(ob=new(file)) ) {
		tell_object(me,"NPC生成失败，请报告巫师！\n");
		return;
	}

	ob->move( environment(me) );
	ob->set("title",GROUP_D->get_group_name(me->query("group/id"))+"保镖");
	ob->set("group",copy(me->query("group")));
	ob->set("group/class",5);
	ids = ob->parse_command_id_list();
	if( sizeof(ids) > 1 ) ob->set("id1",ids[1]); else ob->set("id1",ob->query("id"));
	if( sizeof(ids) > 2 ) ob->set("id2",ids[2]); else ob->set("id2",ob->query("id"));
	ob->save();

	tell_object(me,"你要雇佣的保镖"+ob->name()+"走了过来。\n");
	ob->set_leader(me);
	tell_object(me,ob->name()+"决定跟随"+me->name()+"一起行动。\n");
}
