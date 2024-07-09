// xihuazi.c (kunlun)
// lane 2004.12.24 add 福利

inherit NPC;
inherit F_MASTER;
int ask_gold();

void create()
{
	set_name("西华子", ({ "xi huazi", "xi" }));
	set("long",
		"他是昆仑派第四代弟子，满腮虬髯，性子十分暴躁。\n");
	set("gender", "男性");
	set("age", 26);
	set("attitude", "peaceful");
	set("shen_type", -1);
	set("str", 36);
	set("int", 23);
	set("con", 25);
	set("dex", 28);

	set("max_qi", 800);
	set("max_qi", 800);
	set("max_jing", 600);
	set("neili", 1500);
	set("max_neili", 1500);
	set("jiali", 30);
	set("combat_exp", 400000);
	set("score", 1000);
	set("inquiry", ([
		"代掌门" : (: ask_gold :),
	]) );

	set_skill("force", 95);
	set_skill("xuantian-wuji", 105);
	set_skill("dodge", 80);
	set_skill("taxue-wuhen", 100);
	set_skill("parry", 100);
	set_skill("sword", 101);
	set_skill("xunlei-jian", 105);
	set_skill("cuff", 100);
	set_skill("zhentian-quan", 100);
	set_skill("throwing", 80);
	set_skill("mantian-huayu", 95);

	map_skill("force", "xuantian-wuji");
	map_skill("dodge", "taxue-wuhen");
	map_skill("sword", "xunlei-jian");
	map_skill("cuff", "zhentian-quan");
	map_skill("throwing", "mantian-huayu");
	prepare_skill("cuff", "zhentian-quan");

        create_family("昆仑派", 4, "弟子");
	setup();
	carry_object(BINGQI_D("changjian"))->wield();
	carry_object("/d/kunlun/obj/pao3")->wear();
}

void attempt_apprentice(object ob)
{
	if(ob->query("gender")=="女性"){
		command("fear");
		command("say "+RANK_D->query_respect(ob)+"师傅不准我收女弟子，你去找我师姊去吧。");
		return;
	}
	if(ob->query_skill("xuantian-wuji",1) < 60){
		command("hmm");
		command("say "+RANK_D->query_respect(ob)+"你的玄天无极功太差，我可不想收你。");
		return;
	}
	if(ob->query("appren_hezudao", 1) == 1) {
		command("say 前辈怎敢开这等玩笑，真是折杀做晚辈的了。");
		return;
	}
	command("say 既然"+RANK_D->query_respect(ob)+"这么努力，我就收下你吧。");
	command("recruit " + ob->query("id"));
}

int ask_gold()
{
	object me;
	int skill, gold;
	mapping myfam;
	me = this_player();
	skill = me->query_skill("xuantian-wuji", 1);
	myfam = (mapping)me->query("family");

	if(me->query("shizhe_newbie")) {
		command("hmm "+(string)me->query("id"));
		command("say 找BUG呀，扣钱，扣钱！\n");
		return 1;
	}
	
	if( !myfam || myfam["family_name"] != "昆仑派" ) {
		command("why2 "+(string)me->query("id"));
		return 1;
	}

	if( (int)me->query("kl_given") >= (int)me->query("age") ) {
		command("hmm "+(string)me->query("id"));
		command("say 不要多罗嗦了，心里记得我就可以了！");
		return 1;
	}

	if( skill < 40 ) {
		command("mimi2 "+(string)me->query("id"));
		command("say 你是那个师兄派来的啊，想套我口风？\n");
		return 1;
	}

	gold = 200000 + skill * 5000;
	if( 100 >= skill ) gold += gold/2;
	me->add("balance", gold);
	me->set("kl_given", (int)me->query("age"));
	command("say "+RANK_D->query_respect(me)+"真是有眼光啊，你怎么知道我一定会成为代掌门啊？不过...");
	command("say 我将这" + MONEY_D->money_str(gold) + "存入你的钱庄，以后还请多多支持啊！");
	return 1;
}
