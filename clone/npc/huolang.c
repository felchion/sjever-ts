// huolang.c 货郎
// By River@SJ

inherit NPC;

inherit F_VENDOR;
#include <ansi.h>
string ask_he();

void create()
{
	set_name("货郎", ({ "huo lang", "lang", "huo" }) );
	set("gender", "男性" );
	set("age", 50+random(15));
	set("long",  "一个白发苍苍的老翁，挑着货架走街串巷，专门卖些有情人的物什，\n"+
		"有着一副让人一看就感到亲近的和善的笑容。\n");
	set("combat_exp", 30000);
	set("no_quest", 1);
	set("per", 20);
	set("unique", 1);
	set("attitude", "friendly");
	set("move_seller", 1);
	set("location", 1);

	set("inquiry", ([
		"纸鹤" : (: ask_he :),
	]));

	set("chat_chance", 10);
	set("chat_msg", ({
		(: random_move :),
	}));

	set("vendor_goods", ({
		(["name": MISC_D("fengling"), "number": 5]),
		(["name": MISC_D("fengche"),"number" : 5]),
		(["name": MISC_D("paper"), "number" : 50]),
	}));

	setup();
	carry_object(ARMOR_D("cloth"))->wear();
}

void init()
{
	add_action("do_buy", "buy");
	add_action("do_list", "list");
}

string ask_he()
{
	object me = this_player();
	if (objectp(present("jin zhihe", me))) {
		command("pat "+ me->query("id"));
		return "仔细多看看金纸鹤的折叠方法吧。";
	}
	if ( me->query_temp("make_zhihe/done")) {
		command("congra "+ me->query("id"));
		return "这位"+RANK_D->query_respect(me)+"，可真有毅力，已经有一千只纸鹤了。";
	}
	message_vision(CYN"货郎神秘地笑道：「这位"+RANK_D->query_respect(me)+"可是要给自己的人儿叠鹤么。」\n"NOR,me);
	me->set_temp("make_zhihe/ask", 1);
	return "不过，这个这个.....";
}

int accept_object(object me, object obj)
{            
	object he;
	if(obj->query("money_id")){
		if( me->query_temp("make_zhihe/ask")){
			if( obj->value() >= 10000 ) {
				command("haha " + me->query("id"));
				command("say 这位"+RANK_D->query_respect(me)+"可真是有心人,既是如此,这叠鹤的方儿我就传了你吧。");
				he=new(MISC_D("jinhe"));
				he->set("owner", me->query("id"));
				he->move(me);
				message_vision("货郎给了$N一只"HIY"金纸鹤"NOR"。\n", me);
				me->delete_temp("make_zhihe/ask");
				return 1;
			}
			else {
				command("say 才这么点钱能干什么？");
				return 1;
			}
		}
		else {
			command("wah");
			command("say 那就多谢了！");
			return 1;
		}
	}
	command("? " + me->query("id"));
	command("say 我对"+obj->name()+"不稀罕。");
	return 0;
}

void unconcious()
{
	say("\n货郎伸手在怀里摸出一粒药丸来塞在嘴里，片刻之间就又恢复了精神。\n");
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

void kill_ob(object ob)
{
	object me = this_object();

	command("!!!");
	me->remove_enemy(ob);
	ob->remove_killer(me);
	return;
}

void accept_fight(object ob)
{
	command("peace "+ ob->query("id"));
	return;
}
