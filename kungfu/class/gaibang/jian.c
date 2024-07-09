// NPC: jian.c 简长老
// Modify By Looklove 2000/10/18

#include <ansi.h>
inherit NPC;
inherit F_MASTER;
#include "begger.h";

void create()
{
	set_name("简长老", ({"jian zhanglao", "jian", "zhanglao"}));
	set("title", HIW"丐帮九袋长老"NOR);
    	set("gb/bags",9);
	set("gender", "男性");
    	set("age", 65);
	set("long", "他乃丐帮六大长老之一。\n");
	set("attitude", "peaceful");

	set("str", 28);
	set("int", 20);
	set("con", 25);
	set("dex", 28);

	set("qi", 2800);
	set("max_qi", 2800);
	set("jing", 1600);
	set("max_jing", 1600);
	set("eff_jing", 2600);
	set("neili", 3800);
	set("max_neili", 3800);
	set("jiali", 30);

    	set("combat_exp", 550000);

	set_skill("force", 140); // 基本内功
	set_skill("huntian-qigong", 140); // 混天气功
	set_skill("hand", 140); // 基本拳脚
        set_skill("suohou-shou", 140);
	set_skill("dodge", 135); // 基本躲闪
        set_skill("xiaoyaoyou", 140); // 千里行乞
	set_skill("parry", 140); // 基本招架
        set_skill("stick", 100);
        set_skill("dagou-bang", 100);
	set_skill("begging", 100);

	map_skill("force", "huntian-qigong");
        map_skill("hand", "suohou-shou");
	map_skill("dodge", "xiaoyaoyou");
	map_skill("parry", "suohou-shou");
	map_skill("stick", "dagou-bang");
	prepare_skill("hand", "suohou-shou");

	create_family("丐帮", 18, "九袋长老");

        set("chat_chance_combat", 80);
        set("chat_msg_combat", ({
		(: exert_function, "powerup" :),
	}));

	setup();
	carry_object(__DIR__"obj/gb_budai9")->wear();
        carry_object(ARMOR_D("bainayi"))->wear();
        carry_object(ARMOR_D("caoxie"))->wear();
        carry_object(BINGQI_D("zhubang"))->wield();
}

void attempt_apprentice(object ob)
{
	if ((int)ob->query_str() <= 25	) {
		command("say 我的功夫刚猛强劲" +
		RANK_D->query_respect(ob) +
		"，臂力太弱，似乎不适合跟我学武功。\n");
		return;
	}

        if (ob->query("family/family_name")
        && ob->query("family/family_name") != "丐帮") {
                command("say 你还是去别的门派看看吧。");
                return;
	}
        if ((int)ob->query("gb/bags") >= 2
        && ob->query("family/family_name") != "丐帮" ) {
                command("say 丐帮可容不下你这种背信弃义的小人！");
                command("kill "+ob->query("id"));
                return;
        }

	if( ob->query_skill("huntian-qigong", 1) < 50){
		command("say "+RANK_D->query_respect(ob) +"的混天气功是不是还不够？");
		return;
	}
        command("say 好吧，希望" + RANK_D->query_respect(ob) +
        "能好好学习本门武功，将来在江湖中闯出一番作为。");
        if (!ob->query("gb/bags")) ob->set("gb/bags", 1);
        command("recruit " + ob->query("id"));
        ob->set("title",sprintf("丐帮大仁分舵%s袋弟子",
                chinese_number(ob->query("gb/bags"))));
        ob->set("class","beggar");
}
