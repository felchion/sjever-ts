// deng.c 嵩山派第五代弟子 丁勉 2004.12.15 add ask gold
// Lklv 2001.10.18 Modified

#include <ansi.h>
inherit NPC;
inherit F_MASTER;

int ask_gold(object who);

void create()
{
	set_name("丁勉", ({ "ding mian", "ding", "mian" }));
	set("nickname", HIR"托塔手"NOR);
	set("long", "这是位肥胖的老者，他身材魁伟，是嵩山派掌门人的二师弟托塔手丁勉。\n");
	set("gender", "男性");
	set("age", 51);
	set("per", 18);
	set("attitude", "heroism");
	set("shen_type", -1);
	set("str", 33);
	set("int", 23);
	set("con", 25);
	set("dex", 25);
	set("unique", 1);
	set("env/wimpy", 70);

	set("max_qi", 3000);
	set("max_jing", 2000);
	set("eff_jingli", 2000);
	set("max_neili", 3500);
	set("jiali", 50);
	set("combat_exp", 1500000);
	set("shen", -640000);

	set_skill("literate", 100);
	set_skill("force", 160);
	set_skill("parry", 165);
	set_skill("sword", 150);
	set_skill("dodge", 150);
	set_skill("hand", 160);
	set_skill("zhongyuefeng", 160);
	set_skill("songyang-shou", 160);
	set_skill("hanbing-zhenqi", 160);
	set_skill("songshan-jian", 160);

	map_skill("force", "hanbing-zhenqi");
	map_skill("dodge", "zhongyuefeng");
	map_skill("hand", "songyang-shou");
	map_skill("parry", "songshan-shou");
	map_skill("sword", "songshan-jian");

	prepare_skill("hand", "songyang-shou");
	set("chat_chance_combat", 10);
	set("chat_msg_combat", ({
		(: perform_action, "hand.junji" :),
		(: exert_function, "juhan" :),
	}));

	create_family("嵩山派", 3, "弟子");

	set("inquiry", ([
		"五岳并派" : (: ask_gold :),
		"绝技": "这绝招有个名字叫：「峻极神掌(junji)」，取自此峻极峰。",
		"大嵩阳手": "嘿嘿，想当年我两掌震伤恒山定逸老尼，用的就是这套手法的绝技。",
		"嵩山派" : "我们嵩山派是五岳剑派之首！\n",
		"五岳剑派" : "我们五岳剑派在武林之中势力巨大，可与少林、武当抗衡。\n",
		"左冷禅" : "他就是我们的掌门人，功夫深不可测。\n",
	]) );

	setup();
	carry_object(ARMOR_D("changpao1"))->wear();
	add_money("gold", 1);
}

int ask_gold(object who)
{
	object me = this_player();
	int gold = 0;

	if(me->query("shizhe_newbie")) {
		command("hmm "+(string)me->query("id"));
		command("say 找BUG呀，扣钱，扣钱！\n");
		return 1;
	}
	
	if( me->query("family/family_name") != "嵩山派" ) {
		command("say 我们嵩山派的事，" + RANK_D->query_respect(me) + "最好是不要插手！");
		return 1;
	}

	if( (int)me->query_skill("hanbing-zhenqi", 1) < 50 ) {
		command("say " + RANK_D->query_respect(me) + "不好好学习我嵩山派武功，整天问东问西，告诉你又能如何？");
		return 1;
	}

	if( me->query("songshan_given") >= 300 ) {
		command("say " + RANK_D->query_respect(me) + "武功高强，既然有心出力，那就烦请多联系些江湖高手来我派助阵吧！");
		return 1;
	}

	if( (int)me->query("songshan_given") + 10 < (int)me->query_skill("hanbing-zhenqi", 1) ) {
		if( (int)me->query_skill("hanbing-zhenqi", 1) > 300 ) 
			gold = 300 - (int)me->query("songshan_given");
		else gold = (int)me->query_skill("hanbing-zhenqi", 1) - (int)me->query("songshan_given");
		gold *= 10000;

		if( (int)me->query_skill("literate", 1) < 100 ) gold *= 5; 
		else if( (int)me->query_skill("literate", 1) >= 150 ) gold *= 3; 
		else gold *= 4; 

		command("interest");
		me->add("balance", gold);
		me->set("songshan_given", (int)me->query_skill("hanbing-zhenqi", 1));
		command("say 多日不见，" + RANK_D->query_respect(me) + "的武学似乎增进不少啊！");
		command("say 既然" + RANK_D->query_respect(me) + "有心，我这就将" + MONEY_D->money_str(gold) + "存入你的钱庄，你定要好好招揽群雄助我派夺得盟主之位啊！");
	} else command("say " + RANK_D->query_respect(me) + "，我嵩山派以武功称霸武林，在武学方面马虎不得啊！");
	return 1;
}

#include "app3.h";