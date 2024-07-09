// tuoboseng.c
// 2004.12.14 lane add ask gold

inherit NPC;

int ask_gold(object who);
void create()
{
	set_name("托钵僧", ({"tuobo seng","seng"}));
	set("title", "少林弟子");
	set("long", "他是一位未通世故的青年和尚，脸上挂着孩儿般的微笑。\n");

	set("gender", "男性");
	set("attitude", "heroism");
	set("class", "bonze");

	set("age", 20);
	set("shen_type", 1);
	set("str", 15);
	set("int", 30);
	set("con", 30);
	set("dex", 18);
	set("max_qi", 880);
	set("max_jing", 500);
	set("neili", 380);
	set("max_neili", 380);
	set("jiali", 7);
	set("combat_exp", 120000);
	set("shen_type", 1);
	set("chat_chance", 30);

	set("chat_msg", ({
		"托钵僧说道: 请这位施主化几两银子给贫僧，积点阴德，造福众生。\n",
		(: random_move :)
        }) );

//        set("inquiry", ([
//                "化缘" : (: ask_gold :),
//        ]));

	set_skill("force", 41);
	set_skill("cuff", 41);
	set_skill("dodge", 41);
	set_skill("parry", 41);
	set_skill("shaolin-shenfa", 42);
	set_skill("jingang-quan", 41);
	set_skill("yijin-jing", 41);

	map_skill("force", "yijin-jing");
	map_skill("dodge", "shaolin-shenfa");
	map_skill("cuff", "jingang-quan");
	map_skill("parry", "jingang-quan");
	prepare_skill("cuff","jingang-quan");

	create_family("少林派", 40, "弟子");
	setup();
        carry_object("/d/putian/npc/obj/cloth")->wear();
        carry_object("/d/putian/npc/obj/sengxie")->wear();

	setup();
}

int accept_object(object me, object ob)
{
	object ling;
	//add by look@SJ because 玩家私藏宝物。
	//add by caiji@SJ because 天龙寺出家弟子进不了少林。
	if( ob->query("money_id") && ob->value() >= 10000 && ((me->query("class") != "bonze"
	&& ( me->query("family/family_name") != "少林派"))
	|| me->query("gender")== "女性")) {
		command("smile");
		command("say 阿弭陀佛，多谢这位" + RANK_D->query_respect(me) + " ！");
		command("say " + RANK_D->query_respect(me) +
		"如此乐善好施，兼又身怀绝艺，本寺方丈正欲一晤天下英雄，不知可愿赴少林寺内一游？");
		ling=new("/d/shaolin/obj/tie-ling");
		ling->move(me);
		message_vision("托钵僧给$N一块铁铸令牌。\n", me);
		command("say 此牌限时一日，逾时失效，请于限时前离寺，否则多有不便，切记切记！");
	}
	else return notify_fail("托钵僧说道：" + RANK_D->query_respect(me) + "送贫僧这些东西，不知是何用意？\n");
	return 1;
}

int ask_gold(object who)
{
	object me = this_player();
	int gold = 0;

	if( me->query("family/family_name") != "少林派" ) {
		command("say 请这位施主化几两银子给贫僧，积点阴德，造福众生！");
		return 1;
	}

	if( me->query("shen") < 0 ) {
		command("say " + RANK_D->query_respect(me) + "身为少林弟子，却满脸戾气，实在大不应该啊！");
		return 1;
	}
	if( (int)me->query_skill("buddhism", 1) < 30 ) {
		command("say " + RANK_D->query_respect(me) + "身为少林弟子，佛法如此之差，如何能体会世人的疾苦！");
		return 1;
	}
	if( (int)me->query_skill("yijin-jing", 1) < 30 ) {
		command("say " + RANK_D->query_respect(me) + "身为少林弟子，内功如此不济，希望多多努力！");
		return 1;
	}
	if( (int)me->query_skill("yijin-jing", 1) > 200 ) {
		command("say " + "你已经是神僧了，还要化缘，岂不让人笑话！");
		return 1;
	}	
	if( (int)me->query("shaolin_given") + 10 < (int)me->query_skill("yijin-jing", 1) ) {
		gold = (int)me->query_skill("yijin-jing", 1) - (int)me->query("shaolin_given");
		if( (int)me->query_skill("yijin-jing", 1) >= 180 ) gold *= 3; 
		else if( (int)me->query_skill("yijin-jing", 1) >= 120 ) gold *= 2; 
		//gold *= 30;000
		gold *= 15000;
		if( random(2) ) gold = gold + random(gold/3);
		else gold = gold - random(gold/4);
		command("buddhi tuobo seng");
		me->add("balance", gold);
		me->set("shaolin_given", (int)me->query_skill("yijin-jing", 1));
		command("say 多日不见" + RANK_D->query_respect(me) + "的佛法修为又更进一层，老衲自叹不如啊！");
		command("say 老衲就将这些月来所化的银两托付给你，" + MONEY_D->money_str(gold) + "已经存入钱庄，望" + RANK_D->query_respect(me) + "将之带回，造福众生！");
		log_file("static/WEAL",
		sprintf("%s获得少林福利,易筋经等级:%d,黄金:%s",
		me->query("id"),
		(int)me->query_skill("yijin-jing", 1),
		MONEY_D->money_str(gold))
		);
	} else command("say 老衲在此化缘已久，望能以己之力，多积阴德，造福众生。");
	return 1;
}
