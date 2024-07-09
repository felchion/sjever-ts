// zhu.c 朱元璋
// Modify By River@sj 99.06
#include <ansi.h>
inherit NPC;

string ask_gold();
void create()
{
	set_name("朱元璋", ({ "zhu yuanzhang","zhu","yuanzhang" }) );
	set("title", "明教洪水旗弟子");
	set("gender", "男性");
	create_family("明教",38,"弟子");
	set("age",36);
	set("long","他是后来明朝的皇帝，依靠明教的影响才取得天下。\n");
	set("str", 24);
	set("int", 30);
	set("con", 23);
	set("dex", 20);
	set("per", 10);
	set("shen_type",1);
	set("combat_exp", 90000);
	set("unique", 1);
	set("inquiry", ([
		"消息"  : (: ask_gold :),
	]));
	set_skill("cuff",60);
	set_skill("literate",180);
	set_skill("force",60);
	set_skill("dodge",60);
	set_skill("parry",60);	
	set_skill("blade",60);	
	set("max_qi",600);
	set("max_jing",400);
	set("neili",400);
	set("max_neili",400);
	set("jiali",20);
	set("eff_jingli",200);
	set_skill("taizu-quan",60);
	map_skill("parry","taizu-quan");
	map_skill("cuff","taizu-quan");
	prepare_skill("cuff","taizu-quan"); 
	setup();
	carry_object("/d/mingjiao/obj/black-cloth")->wear();
	carry_object(BINGQI_D("blade"))->wield();
}

void attempt_apprentice(object ob)
{     
	command("say 呵呵，我武功低微，可不敢收徒啊。");
	command("say 这位"+RANK_D->query_respect(ob)+"还是请回吧。");
	return;
}

string ask_gold()
{
	mapping fam;

	if( !(fam = this_player()->query("family")) || fam["family_name"] != "明教" )
		return RANK_D->query_respect(this_player())+"与本派素无来往，不知此话从何谈起？";

	if( this_player()->query_temp("mingjiao_gold") != 1 )
		return RANK_D->query_respect(this_player())+"想知道什么消息啊？";

	this_player()->set_temp("mingjiao_gold", 2);
	return RANK_D->query_respect(this_player())+"所说的是否是宝鼎的下落啊？据我打听宝鼎是被几名教中弟子偷出，不过现我已将其寻回，请阁下放心吧！\n";
}
