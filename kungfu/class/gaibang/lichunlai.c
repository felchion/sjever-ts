// lichunlai.c 李春来
// Lklv 2001.9.28 update for open 大智分舵

inherit NPC;
#include "begger.h";

void create()
{
        set_name("李春来", ({"li chunlai", "li", "chunlai" }));
        set("title", "丐帮大智分舵四袋弟子");
        set("gb/bags", 4);
        set("gender", "男性");
        set("age", 32);
        set("long", "他是丐帮四袋弟子，大智分舵属下帮众。\n");
        set("attitude", "peaceful");
        set("shen_type", 1);
        set("str", 28);
        set("int", 22);
        set("con", 20);
        set("dex", 21);

        set("qi", 2100);
        set("max_qi", 2100);
        set("jing", 1400);
        set("max_jing", 1400);
        set("eff_jingli", 2400);
        set("neili", 1600);
        set("max_neili", 1400);
        set("jiali", 50);

        set("combat_exp", 800000);

        set_skill("force", 120); // 基本内功
        set_skill("huntian-qigong", 120); // 混天气功
        set_skill("strike", 120); // 基本拳脚
        set_skill("dodge", 120); // 基本躲闪
        set_skill("xiaoyaoyou", 120); // 千里行乞
        set_skill("parry", 120); // 基本招架
        set_skill("begging", 120);
        set_skill("lianhua-zhang", 120);

        map_skill("force", "huntian-qigong");
        map_skill("dodge", "xiaoyaoyou");
	map_skill("strike", "lianhua-zhang");
	map_skill("parry", "lianhua-zhang");
	prepare_skill("strike", "lianhua-zhang");

        create_family("丐帮", 20, "四袋弟子");

        set("inquiry", ([
                "丐帮" : "我们丐帮是天下第一大帮！\n",
                "乔峰" : "乔……他……，不关我的事！\n",
                "全冠清" : "我们舵主，难道你不知道嘛？\n",
        ]));
        setup();

        carry_object(ARMOR_D("bainayi"))->wear();
        carry_object(ARMOR_D("caoxie"))->wear();
        carry_object(__DIR__"obj/gb_budai4")->wear();
}
