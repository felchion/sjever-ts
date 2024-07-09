// hezudao.c (kunlun)

inherit NPC;
#include <ansi.h>
string ask_qin();

void create()
{
	object ob;
	set_name("何足道", ({ "he zudao", "he" }));
	set("title", "昆仑派开山祖师");
	set("nickname", "昆仑三圣");
	set("long",
		"他就是昆仑派开山祖师，号称琴剑棋三绝的“昆仑三圣”。\n"
		"长脸深目，瘦骨棱棱，约莫三十岁左右年纪。\n");
	set("gender", "男性");
	set("age", 35);
	set("attitude", "friendly");
	set("shen_type", 0);
	set("per", 27);
	set("str", 27);
	set("int", 60);
	set("con", 40);
	set("dex", 28);

	set("max_qi", 10000);
	set("max_jing", 6500);
	set("neili", 18000);
	set("eff_jingli", 7000);
	set("max_neili", 17000);
	set("jiali", 150);
	set("combat_exp", 15000000);
	set("unique", 1);
	set("score", 40000);

	set("chat_chance_combat", 85);
	set("chat_msg_combat", ({
  		(: perform_action, "sword.xunlei" :),
  		(: perform_action, "sword.hundun" :),
	}));

	set("inquiry", ([
		"琴" : (: ask_qin :),
		"焦尾琴" : (: ask_qin :),
	]));

	set_skill("art", 500);
	set_skill("literate", 250);
	set_skill("force", 500);
	set_skill("xuantian-wuji", 500);
	set_skill("dodge", 500);
	set_skill("taxue-wuhen", 500);
	set_skill("xunlei-jian", 500);
	set_skill("strike", 500);
	set_skill("kunlun-zhang", 500);
	set_skill("parry", 500);
	set_skill("sword", 500);
	set_skill("liangyi-jian", 500);
	set_skill("throwing", 500);

	map_skill("force", "xuantian-wuji");
	map_skill("dodge", "taxue-wuhen");
	map_skill("strike", "kunlun-zhang");
	map_skill("parry", "liangyi-jian");
	map_skill("sword", "liangyi-jian");
	prepare_skill("strike", "kunlun-zhang");

	set_temp("apply/sword", 100);
	set_temp("apply/parry", 100);
	set_temp("apply/force", 100);
	set_temp("apply/dodge", 100);
	set_temp("apply/strike", 100);
	set_temp("apply/damage", 75);

	create_family("昆仑派", 1, "开山祖师");
	setup();
	if (clonep()) {
		ob = unew(BINGQI_D("sword/jwqin"));
		if (!ob) ob = new(BINGQI_D("guzheng"));
		ob->move(this_object());
		ob->wield();
		carry_object("/d/kunlun/obj/pao1")->wear();
	}
}

void attempt_apprentice(object ob)
{
	if(ob->query_int() < 32 ) {
		command("say 弹琴下棋需要灵性极高的人才可以领悟其真谛，不致与武功背道而驰。");
		command("say "+RANK_D->query_respect(ob)+"还是先回去多读点书吧。");
		return;
	}
	if ((int)ob->query_skill("xuantian-wuji", 1) < 130) {
		command("say 琴棋书画虽为小技，但也需内力融入其中。");
		command("say " + RANK_D->query_respect(ob)+"是否还应该先在玄天无极功上多下点功夫？");
		return;
	}
	if ((int)ob->query_skill("art", 1) < 120) {
		command("say 我昆仑三圣以琴棋剑著称，剑法倒也罢了。");
		command("say " + RANK_D->query_respect(ob)+
			"是否琴棋书画的修为浅了些呢？");
		return;
	}
	if ((int)ob->query("max_neili", 1) < 1000) {
		command("say 内力不够，一心二用则极容易走火入魔。");
		command("say " + RANK_D->query_respect(ob) +
			"是否还应该先好好修炼内力？");
		return;
	}
	if ( ob->query("gender") == "男性" && ob->query_skill("zhentian-quan", 1) < 80 ){
		command("say "+ RANK_D->query_respect(ob) +"是否还应该先在震天拳上多下点功夫？");
		return;
	}
	if ( ob->query("gender") == "女性" && ob->query_skill("chuanyun-tui", 1) < 80 ){
		command("say "+ RANK_D->query_respect(ob) +"是否还应该先在穿云腿上多下点功夫？");
		return;
	}

	command("say 看来你也是性情中人，我就教你一些琴剑棋的知识吧。");
	command("recruit "+ob->query("id"));
	ob->set("appren_hezudao", 1);
	ob->set("title",HIW"昆仑三圣座前弟子"NOR);
}

void reset()
{
	object ob;

	if (clonep() && (ob = unew(BINGQI_D("sword/jwqin")))) {
		ob->move(this_object());
		command("wield jiaowei qin");
	}
}

string ask_qin()
{
	object me = this_player();

	if (me->query("family/master_name") != "何足道")
		return "这焦尾琴是我昆仑派的宝物，你并非我的传人，不能给你！";

	if (present("jiaowei qin",this_object())) {
		command("unwield jiaowei qin");
		command("give jiaowei qin to " + me->query("id"));
		return "这焦尾琴就给你了，你可要妥善保管好！";
	}
	return "你来晚了，焦尾琴我已经送与他人了。";
}
