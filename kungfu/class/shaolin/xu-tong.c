// Npc: /kungfu/class/shaolin/xu-tong.c

inherit NPC;

void create()
{
	set_name("虚通", ({
		"xu tong",
		"xu",
		"tong",
	}));
	set("long",
		"他是一位身穿黄布袈裟的青年僧人。脸上稚气未脱，身手却已相\n"
		"当矫捷，看来似乎学过一点武功。\n"
	);


	set("nickname", "知客僧");
	set("gender", "男性");
	set("attitude", "friendly");
	set("class", "bonze");

	set("age", 20);
	set("shen_type", 1);
	set("str", 20);
	set("int", 20);
	set("con", 20);
	set("dex", 20);
	set("max_qi", 500);
	set("max_jing", 300);
	set("neili", 500);
	set("max_neili", 500);
	set("jiali", 100);
	set("combat_exp", 2000);
	set("score", 100);

	set_skill("force", 20);
	set_skill("yijin-jing", 20);
	set_skill("dodge", 20);
	set_skill("shaolin-shenfa", 20);
	set_skill("strike", 20);
	set_skill("banruo-zhang", 20);
	set_skill("parry", 20);
	set_skill("sword", 20);
	set_skill("damo-jian", 20);

	map_skill("force", "yijin-jing");
	map_skill("dodge", "shaolin-shenfa");
	map_skill("strike", "banruo-zhang");
	map_skill("parry", "damo-jian");
	map_skill("sword", "damo-jian");

	prepare_skill("strike", "banruo-zhang");

	create_family("少林派", 41, "弟子");

	setup();

        carry_object("/d/shaolin/obj/xu-cloth")->wear();
}

int accept_object(object who, object ob,object me)
{
        mapping fam; 
        me=this_object();

        if (!(fam = this_player()->query("family"))|| fam["family_name"] != "少林派"){
                command("hehe "+who->query("id"));
                command("say 你给我东西有什么企图？！");
        	return 0;
        }
	if(!who->query_temp("lunzhi")){
		command("say 你的令牌是那来的，轮值必须得到慧修大师的允许！");
		return 0;
	}
	if(!userp(ob) && ob->query("name") == "轮值令"){
	       	command("say 好吧，你既然有慧修大师的令牌，你就在这和我在这里一起守卫吧。");
		who->apply_condition("sl_lunzhi",random(4)+7);
		who->set_temp("lunzhied",1);
		call_out("dest", 1, ob);
		return 1;
	}
	return 1;
}

void dest(object ob)
{
	if( !ob) return;
	destruct(ob);
}
