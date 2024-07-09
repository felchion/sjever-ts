// NPC: quan.c 全冠清
// Modify By Looklove 2000/10/18
// Lklv 2001.9.28 update for open 大智分舵

#include <ansi.h>
inherit NPC;
inherit F_MASTER;
#include "begger.h";

void create()
{
	set_name("全冠清", ({"quan guanqing", "quan" }));
	set("title", YEL"丐帮大智分舵舵主"NOR);
	set("gb/bags",8);
	set("nickname", "十方秀才");
	set("gender", "男性");
	set("age", 35);
	set("long", "他是八袋舵主，掌管大智分舵，为人足智多谋，武功高强。\n");
	set("attitude", "peaceful");
	set("shen_type", -1);
	set("str", 25);
	set("int", 35);
	set("con", 20);
	set("dex", 25);

	set("qi", 1300);
	set("max_qi", 1300);
	set("jing", 900);
	set("max_jing", 900);
	set("eff_jingli", 1100);
	set("neili", 1600);
	set("max_neili", 1600);
	set("jiali", 20);

	set("combat_exp", 600000);

	set_skill("force", 130); // 基本内功
	set_skill("huntian-qigong", 130); // 混天气功
	set_skill("strike", 130); // 基本拳脚
	set_skill("dodge", 130); // 基本躲闪
        set_skill("xiaoyaoyou", 120); // 千里行乞
	set_skill("parry", 120); // 基本招架
        set_skill("lianhua-zhang", 130);
        set_skill("blade", 140);
        set_skill("liuhe-daofa", 140);
        set_skill("begging", 140);

	map_skill("force", "huntian-qigong");
	map_skill("dodge", "xiaoyaoyou");
	map_skill("blade", "liuhe-daofa");
	map_skill("strike", "lianhua-zhang");
	map_skill("parry", "lianhua-zhang");
	prepare_skill("strike", "lianhua-zhang");

	create_family("丐帮", 19, "八袋弟子");
	set("chat_chance", 2);
        set("chat_msg", ({
		"全冠清哼了一声：刘竹庄这小子不知跑哪去了，干点小事情，这么久了还没回来。\n",
		"全冠清背着双手踱来踱去，似乎在想什么坏念头。\n",
		"全冠清自言自语地对自己说：“生存还是灭亡，这是一个问题！”\n",
	}) );

	set("inquiry", ([
		"丐帮" : "我们丐帮是天下第一大帮！\n",
		"乔峰" : "嘿嘿，他早给我们废了。\n",
		"刘竹庄" : "嘿嘿……嘿嘿……\n",
		"小事情" : "嘿嘿，我们准备废了……，让他不能……\n",
		"李春来" : "嘿嘿……嘿嘿……\n",
		"契丹人" : "嘿嘿……嘿嘿……\n",
		"契丹" : "嘿嘿……嘿嘿……\n",
		"帮主" : "嘿嘿……我……嘿嘿……\n",
		"rumors" : "听说……峨嵋山……雷动……\n",
	]));
	setup();
	carry_object(__DIR__"obj/gb_budai8")->wear();
        carry_object(ARMOR_D("bainayi"))->wear();
        carry_object(ARMOR_D("caoxie"))->wear();
        carry_object(BINGQI_D("blade"))->wield();
}

void attempt_apprentice(object ob)
{
	if ((int)ob->query_int() <=20	) {
		command("say 要想跟我必须够机灵" +
		RANK_D->query_respect(ob) +
		"，悟性太低，我可不想要个笨蛋。\n");
		return;
	}
        if ((int)ob->query("gb/bags") >= 2
        && ob->query("family/family_name") != "丐帮" ) {
                command("say 我是想收你，就怕乔帮主不肯。");
                return;
        }
        command("say 好吧，希望" + RANK_D->query_respect(ob) +
        "能好好学习本门武功，将来在江湖中闯出一番作为。");
        command("recruit " + ob->query("id"));

	if (!ob->query("gb/bags")) ob->set("gb/bags", 1);
        ob->set("title",sprintf("丐帮大智分舵%s袋弟子",
                chinese_number(ob->query("gb/bags"))) );
        ob->set("gb/fenduo","大智分舵");
	ob->set("class","beggar");
}
