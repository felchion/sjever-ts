// bashuxian.c (kunlun)

inherit NPC;
inherit F_MASTER;
string ask_me();

void create()
{
	set_name("班淑娴", ({ "ban shuxian", "ban" }));
	set("title", "昆仑派掌门夫人");
	set("long",
		"她就是昆仑派掌门、名扬江湖的铁琴先生何太冲的夫人。\n"
		"她是一个身材高大的半老女子，头发花白，双目含威，眉心间聚有煞气。\n");
	set("gender", "女性");
	set("age", 44);
	set("attitude", "heroism");
	set("shen_type", -1);
	set("str", 26);
	set("int", 24);
	set("con", 28);
	set("dex", 28);

	set("max_qi", 3500);
	set("max_jing", 2000);
	set("neili", 3200);
	set("max_neili", 3200);
	set("jiali", 50);
	set("eff_jingli",2000);
	set("combat_exp", 15000000);
	set("score", 40000);

        set_skill("literate", 120);
	set_skill("force", 500);
	set_skill("xuantian-wuji", 500);
	set_skill("dodge", 500);
	set_skill("taxue-wuhen", 500);
	set_skill("parry", 500);
	set_skill("sword", 500);
	set_skill("leg", 500);
	set_skill("chuanyun-tui", 500);
	set_skill("xunlei-jian", 500);
	set_skill("mantian-huayu", 500);
	set_skill("throwing", 500);

	map_skill("throwing", "mantian-huayu");
	map_skill("force", "xuantian-wuji");
	map_skill("dodge", "taxue-wuhen");
	map_skill("leg", "chuanyun-tui");
	map_skill("parry", "xunlei-jian");
	map_skill("sword", "xunlei-jian");

	prepare_skill("leg", "chuanyun-tui");

        create_family("昆仑派", 3, "掌门夫人");
	set("class", "taoist");
	set("inquiry",([
		"秘籍" : (: ask_me :),
	]));

	setup();
	carry_object(BINGQI_D("changjian"))->wield();
	carry_object("/d/kunlun/obj/pao2.c")->wear();
}

void attempt_apprentice(object ob)
{
	if(ob->query("gender")=="男性"){
		command("hmm");
		command("say "+RANK_D->query_respect(ob)+"你们这些臭男人我见了就烦，快给我走开！");
		return;
	}
	if(ob->query("appren_hezudao", 1) == 1) {
		command("say "+RANK_D->query_respect(ob)+"怎敢开这等玩笑，真是折杀我了。");
		return;
	}  
	if(ob->query_skill("xuantian-wuji",1) < 100){
		command("hmm");
		command("say "+RANK_D->query_respect(ob)+"你的玄天无极功太差，我可不想收你。");
		return;
	}
	command("say 好吧，以后你就跟我学武功吧，你须得安心学艺，不可乱来。");
	command("recruit " + ob->query("id"));
}

string ask_me()
{
	mapping fam;
	object ob;

	if (!(fam = this_player()->query("family"))
	 || fam["family_name"] != "昆仑派")
		return RANK_D->query_respect(this_player())+"与本派素无来往，不知此话从何谈起？";

	if( this_player()->query("gender") == "男性")
		return "给我走开，我看见你们这些臭男人就烦。";

	ob = unew("/d/kunlun/obj/lyj-book.c");

	if(!clonep(ob)) return "你来晚了，本派真经已被你师兄拿走了。";

	if(clonep(ob) && ob->violate_unique()){
		destruct(ob);
		return "你来晚了，本派真经已被你师兄拿走了。";
	} 
	ob->move(this_player());
	return "先师曾传下来这本「两仪剑心得」，你拿回去好好钻研。";
}
