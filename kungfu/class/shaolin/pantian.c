// pantiangeng.c 潘天耕

inherit NPC;

void create()
{
	set_name("潘天耕", ({ "pan tiangeng","pan","tiangeng" }) );
	set("gender", "男性" );
	set("age", 64);
	set("long", "这个老者相貌平平无奇，只是脸色蜡黄，微带病容。听说一手
伏魔剑在少林无出其右。/n");
	set("attitude", "peaceful");
	set("nickname", "西域少林长老");
	set("class", "bonze");
	set("str", 35);
	set("con", 35);
	set("int", 25);
	set("dex", 35);


	set("max_qi", 3000);
	set("max_jing", 1500);
	set("neili", 300);
	set("max_neili", 3000);
	set("jiali", 100);
	set("shen_type", 1);
	set("startroom","/d/shaolin/shijie8");
	set("unique", 1);

	set("combat_exp", 1200000);

	set_skill("force", 150);
	set_skill("finger", 150);
	set_skill("sword", 160);
	set_skill("dodge", 150);
	set_skill("parry", 160);
	set_skill("shaolin-shenfa", 150);
	set_skill("literate", 150);
	set_skill("yizhi-chan", 140);
        set_skill("fumo-jian", 170);
	set_skill("yijin-jing", 150);
	set_skill("buddhism", 150);

	map_skill("force", "yijin-jing");
	map_skill("dodge", "shaolin-shenfa");
        map_skill("finger", "yizhi-chan");
        map_skill("parry", "fumo-jian");
        map_skill("sword", "fumo-jian");
        prepare_skill("finger", "yizhi-chan");

	create_family("西域少林", 19, "弟子");

	set("chat_chance", 20);
	set("chat_msg", ({
		"潘天耕说道，西域少林虽是少林分支，但外功的刚猛却有独到之处。\n",
		"潘天耕说道，出家人虽戒杀生，但锄恶降魔却也是我佛本意。\n",
                 (: random_move :)
	}) );

	setup();
         carry_object("/clone/weapon/changjian")->wield();
        carry_object("/d/shaolin/obj/xuan-cloth")->wear();
}

void kill_ob(object me)
{
        set("combat_exp", 1200000);
        set("max_qi", 3000);
        set("max_neili", 3000);
        set("neili", 3000);
        set("jiali", 100);
        set("chat_chance_combat", 50);
	set("chat_msg_combat", ({
		(: perform_action, "finger.wofo" :)
        }));
        ::kill_ob(me);
}
