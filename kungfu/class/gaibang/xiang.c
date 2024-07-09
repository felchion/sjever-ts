// xiang.c 项长老
// Modify By Looklove 2000/10/21

#include <ansi.h>
inherit NPC;
inherit F_MASTER;
#include "begger.h";

string ask_mantou();

void create()
{
	set_name("项长老", ({"xiang zhanglao", "xiang" ,"zhanglao"}));
	set("title", HIW"丐帮传功长老"NOR);
	set("gb/bags",9);
	set("mantou_count",30);
	set("gender", "男性");
	set("age", 70);
	set("long", "他看起来不怒自威。\n");
	set("attitude", "peaceful");
	set("shen_type", 1);
	set("str", 30);
	set("int", 35);
	set("con", 20);
	set("dex", 30);

	set("qi", 3500);
	set("max_qi", 3500);
	set("jing", 2900);
	set("max_jing", 2900);
	set("eff_jing", 3200);
	set("neili", 4600);
	set("max_neili", 4600);
	set("jiali", 100);

	set("combat_exp", 2600000);

	set_skill("force", 240);
	set_skill("huntian-qigong", 240);
	set_skill("strike", 240);
	set_skill("dodge", 240);
        set_skill("xiaoyaoyou", 240);
	set_skill("parry", 240);
        set_skill("blade",240);
        set_skill("liuhe-daofa",220);
        set_skill("hand",220);
        set_skill("begging", 120);
        set_skill("suohou-shou",220);

	map_skill("force", "huntian-qigong");
	map_skill("dodge", "xiaoyaoyou");
        map_skill("blade","liuhe-daofa");
        map_skill("hand","suohou-shou");
	map_skill("parry", "suohou-shou");
	prepare_skill("hand", "suohou-shou");

    	set("chat_chance_combat", 100);
    	set("chat_msg_combat", ({
        	(: perform_action, "blade.sancai" :),
        	(: exert_function, "huntian" :),
        }));

	create_family("丐帮", 18, "传功长老");

	set("inquiry", ([
		"丐帮" : "我们丐帮是天下第一大帮！\n",
		"乔峰" : "真是一位了不起的大英雄，可惜非我族类。\n",
		"给我一个馒头吃" : (: ask_mantou :),
	]));
	setup();

        carry_object(BINGQI_D("blade"))->wield();
        carry_object(ARMOR_D("bainayi"))->wear();
        carry_object(ARMOR_D("caoxie"))->wear();
	carry_object(__DIR__"obj/gb_budai9")->wear();
}

void attempt_apprentice(object ob)
{
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

        if((int)ob->query_skill("huntian-qigong",1) < 100){
		command("say 混天气功是丐帮祖上留传的功夫，你对此钻研不够啊。\n");
		return;
	}
        command("say 好吧，希望" + RANK_D->query_respect(ob) +
        "能好好学习本门武功，将来在江湖中闯出一番作为。");

        command("recruit " + ob->query("id"));
        if (!ob->query("gb/bags")) ob->set("gb/bags", 1);
        ob->set("title",sprintf("丐帮总舵%s袋弟子",
                chinese_number(ob->query("gb/bags"))) );
        ob->set("gb/fenduo","总舵");
        ob->set("class","beggar");
}

string ask_mantou()
{
        object me;
        me = this_player();

        if (me->query("family/family_name") != "丐帮")
                return RANK_D->query_respect(me) +
                "非我丐帮好汉，说这种话不觉得羞耻嘛？";

        if (me->query("gb/bags") > 8)
                return RANK_D->query_respect(me) +
                "这种身份，怎么能和我要东西吃呢？";

        if (present("mantou", me))
                return RANK_D->query_respect(me) +
                "你自己手里不是有吗？去死吧！！！饭桶！";

        if (query("mantou_count") < 1) return "我今天也没有讨到馒头，自己去讨吧。";

        new(FOOD_D("mantou"))->move(me);
	add("mantou_count", -1);
        message_vision("$N得到一个冷馒头。\n",me);
        return "拿去吃吧，别噎到。";
}	