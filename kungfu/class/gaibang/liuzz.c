// liuzz.c 刘竹庄
// Lklv 2001.9.28 update for open 大智分舵

inherit NPC;

int action_1();

void create()
{
        set_name("刘竹庄", ({"liu zhuzhuang", "liu" }));
        set("title", "丐帮大智分舵五袋弟子");
        set("gb/bags", 5);
        set("gender", "男性");
        set("age", 28);
        set("long", "他是丐帮五袋弟子，大智分舵属下帮众。\n");
        set("attitude", "peaceful");
        set("shen_type", 1);
        set("str", 25);
        set("int", 20);
        set("con", 25);
        set("dex", 25);

        set("qi", 800);
        set("max_qi", 800);
        set("jing", 600);
        set("max_jing", 600);
        set("eff_jingli", 750);
        set("neili", 1000);
        set("max_neili", 1000);
        set("jiali", 50);

        set("combat_exp", 200000);

        set_skill("force", 80); // 基本内功
        set_skill("huntian-qigong", 80); // 混天气功
        set_skill("strike", 80); // 基本拳脚
        set_skill("dodge", 80); // 基本躲闪
        set_skill("xiaoyaoyou", 60); // 千里行乞
        set_skill("parry", 80); // 基本招架
        set_skill("lianhua-zhang", 80);

        map_skill("force", "huntian-qigong");
        map_skill("dodge", "xiaoyaoyou");
	map_skill("strike", "lianhua-zhang");
	map_skill("parry", "lianhua-zhang");
	prepare_skill("strike", "lianhua-zhang");

        create_family("丐帮", 20, "五袋弟子");

        set("inquiry", ([
                "丐帮" : "我们丐帮是天下第一大帮！\n",
                "乔峰" : "乔……他……，不关我的事！\n",
                "全冠清" : (: action_1 :),
        ]));
        setup();

        carry_object(ARMOR_D("bainayi"))->wear();
        carry_object(ARMOR_D("caoxie"))->wear();
        carry_object(__DIR__"obj/gb_budai5")->wear();
}

int action_1()
{
        command("fear");
        return 1;
}
