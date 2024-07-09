// lisheng.c 黎生
// Looklove 2000/10/21

#include <ansi.h>
inherit NPC;
inherit F_MASTER;
#include "begger.h";

void create()
{
	set_name("黎生", ({"li sheng", "li", "sheng"}));
	set("title", "丐帮八袋弟子");
	set("gb/bags",8);
	set("nickname", HIC"江东蛇王"NOR);
	set("gender", "男性");
	set("age", 58);
	set("long", "这是位生性刚直，嫉恶如仇的丐帮八袋弟子。\n");
	set("attitude", "peaceful");

	set("str", 32);
	set("int", 18);
	set("con", 20);
	set("dex", 23);

	set("qi", 3390);
	set("max_qi", 3390);
	set("jing", 2300);
	set("max_jing", 2300);
	set("eff_jingli", 2800);
	set("neili", 4800);
	set("max_neili", 3800);
	set("jiali", 60);

	set("combat_exp", 1200000);

	set_skill("force", 181);
	set_skill("huntian-qigong", 182);
	set_skill("strike", 185);
	set_skill("xianglong-zhang", 175);
	set_skill("dodge", 180);
	set_skill("xiaoyaoyou", 182);
	set_skill("parry", 182);
	set_skill("stick", 180);
	set_skill("begging", 120);

	map_skill("force", "huntian-qigong");
	map_skill("strike", "xianglong-zhang");
	map_skill("parry", "xianglong-zhang");
	map_skill("dodge", "xiaoyaoyou");
	prepare_skill("strike", "xianglong-zhang");

    	set("chat_chance_combat", 100);
    	set("chat_msg_combat", ({
        	(: perform_action, "strike.xiao" :),
        	(: exert_function, "huntian" :),
        }));

	create_family("丐帮", 19, "弟子");

	setup();

        carry_object(ARMOR_D("bainayi"))->wear();
        carry_object(ARMOR_D("caoxie"))->wear();
	carry_object(__DIR__"obj/gb_budai8")->wear();
}

void attempt_apprentice(object ob)
{
	if( ob->query("shen") < 0 ) return;
        if (ob->query("family/family_name")
        && ob->query("family/family_name") != "丐帮") {
                command("say 你还是去别的门派看看吧。");
                return;
	}
        if ((int)ob->query("gb/bags") >= 2
        && ob->query("family/family_name") != "丐帮" ) {
                command("say 丐帮可容不下你这种反复无常的小人！给我滚！");
                command("kill "+ob->query("id"));
                return;
        }

        if((int)ob->query_skill("xiaoyaoyou",1) < 80 ){
		command("say 逍遥游是丐帮祖传的功夫，你对此钻研不够啊。\n");
		return;
	}
	command("say 好，有你这样一等一的人才做我徒弟，我欢喜还来不及呢！");
        command("recruit " + ob->query("id"));

        if (!ob->query("gb/bags")) ob->set("gb/bags", 1);
        ob->set("title",sprintf("丐帮总舵%s袋弟子",
                chinese_number(ob->query("gb/bags"))) );
        ob->set("gb/fenduo","总舵");
        ob->set("class","beggar");
}