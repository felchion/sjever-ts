// chen.c 陈近南

inherit NPC;
#include <ansi.h>

string ask_ningxue();

void create()
{
	set_name("陈近南", ({ "chen jinnan", "chen", "jinnan" }));
	set("gender", "男性");
	set("age", 45);
	set("shen_type", 1);
	set("天地会/title","总舵主");
	set("title", HIW"天地会总舵主"NOR);
	set("long", "他就是天地会总舵主陈近南，所谓平生不见陈近南，便称英雄也枉然。\n");

	set("str", 35);
	set("int", 35);
	set("con", 35);
	set("dex", 35);
	set("per", 28);

	set("max_qi", 12500);
	set("max_jing", 7500);
	set("neili", 18600);
	set("eff_jingli", 7500);
	set("max_neili", 18500);
	set("jiali", 150);
	set("combat_exp", 5850000);
	set("unique", 1);
	set("no_get", 1);
	set("no_bark", 1);

	set_skill("claw", 380);
	set_skill("dodge", 380);
	set_skill("parry", 380);
	set_skill("force", 380);
	set_skill("ningxue-shenzhua", 380);
	set_skill("literate", 300);
	set_skill("medicine", 150);
	set_skill("xiantian-gong", 380);
	set_skill("shaolin-shenfa", 380);

	map_skill("force", "xiantian-gong");
	map_skill("claw","ningxue-shenzhua");
	map_skill("dodge","shaolin-shenfa");
	map_skill("parry","ningxue-shenzhua");

	prepare_skill("claw", "ningxue-shenzhua");


	set("chat_chance", 50);
	set("chat_msg", ({
		(: random_move :),
	}));

	set("chat_chance_combat", 90);
	set("chat_msg_combat", ({
		(: perform_action, "claw.ningxue":),
	}));

	set("inquiry", ([
		"凝血神爪":	(: ask_ningxue :),
	]));
	setup();

	carry_object(ARMOR_D("cloth"))->wear();
	carry_object(ARMOR_D("shoes"))->wear();
	add_money("silver", 10 + random(20));
}

string ask_ningxue()
{
	object ob, me = this_player();
	int time, i, job = me->query("job_time/天地会");
	mapping quest;

	if ( job <= 1000 || ! me->query("quest/ningxue/meet"))
		return "凝血神爪？你从那里听说的？";

	time = time() - me->query("quest/ningxue/time");
	if ( me->query("quest/ningxue/time") && time < 86400 && !me->query("quest/ningxue/pass"))
		return "我不是和你说了，你暂时无法学会凝血神爪么？";

	me->set("quest/ningxue/time", time());

	if ( me->query("con") < 15 )
		return "凝血神爪对根骨有一定的要求，看来你并不合适。";

	if ( me->query("dex") < 15 )
		return "凝血神爪对身法有一定的要求，看来你并不合适。";

	quest = me->query("quest/ningxue");
	i = me->query("combat_exp", 1) / 1000000;

	if ( quest["fail"] >= i)
		return "现在你的修为还不够再次尝试，还是过段时间再来吧。";

	if ( quest["pass"]) {
		if ( present("ningxue shenzhuapu", me))
			return "「凝血神爪谱」不是在你身上么？还问我做什么？";

		if ( me->query_skill("ningxue-shenzhua", 1) < 150 ) {
			ob = new(BOOK_D("ningxue"));
			ob->set("owner", me);
			message_vision("$n给了$N一本"+ob->name()+"。\n", me, this_object());
			ob->move(me);
			return "这本「凝血神爪谱」你就拿去研究下吧。";
		}
		else
			return "你已经基本掌握了凝血神爪功，只需要勤加修炼就行了。";
	}

	if ( random(me->query_con(1)) > 35 
	&& random(me->query_dex(1)) > 35
	&& random(me->query("kar")) > 25 ) {
		command("look "+ me->query("id"));
		command("pat "+ me->query("id"));
		ob = new(BOOK_D("ningxue"));
		ob->set("owner", me);
		message_vision("$n给了$N一本"+ob->name()+"。\n", me, this_object());
		ob->move(me);
		log_file("quest/ningxue",
			sprintf(HIR"%-18s失败%-2d次后从陈近南处得到凝血神爪谱，根：%d，身：%d，福：%d。"NOR,
				me->name(1)+"("+capitalize(getuid(me))+")", 
				me->query("quest/ningxue/fail"),
				me->query_con(1), 
				me->query_dex(1), 
				me->query("kar")
			), me
		);
		me->set("quest/ningxue/pass", 1);
		return "嗯，不错，你资质不错是块学武的料，这本「凝血神爪谱」你就拿去研究下吧。";
	}
	else {
		command("pat "+ me->query("id"));
		me->add("quest/ningxue/fail", 1);
		log_file("quest/ningxue", 
			sprintf("%-18s第%-2d次没有能从陈近南处得到凝血神爪谱，根：%d，身：%d，福：%d。",
				me->name(1)+"("+capitalize(getuid(me))+")", 
				me->query("quest/ningxue/fail"),
				me->query_con(1), 
				me->query_dex(1), 
				me->query("kar")
			), me
		);
		return "这位"+RANK_D->query_respect(me)+"看来还需要努力一番才行，你现在状态不太适合学习凝血神爪。";
	}
}
                                                                                                                                                                                                                                                                                                         