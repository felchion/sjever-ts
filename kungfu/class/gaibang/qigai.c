// qigai.c 乞丐

inherit NPC;
#include "begger.h";

void create()
{
	set_name("中年乞丐", ({ "qi gai", "qigai", "beggar" }) );
	set("title", "丐帮帮众");
	set("gb/bags", 1);
	set("long","一个一脸菜色的乞丐，看起来好象还饿着肚子。\n");
	set("gender", "男性");
	set("age", 30+random(10));
	set("attitude", "peaceful");
	set("shen_type", 0+random(10));

	set("str", 20+random(10));
	set("int", 16+random(10));
	set("con", 22);
	set("dex", 24+random(10));

	set("max_qi", 300+random(800));
	set("max_jing", 300+random(400));
	set("eff_jingli", 300+random(400));
	set("max_neili", 300+random(800));
	set("jiali", 10+random(30));
	set("combat_exp", 43000+random(10000));

	set_skill("force", 30+random(30));
	set_skill("dodge", 30+random(30));
	set_skill("xiaoyaoyou", 30+random(30));
	set_skill("parry", 30+random(30));
	set_skill("cuff", 30+random(30));
	set_skill("taizu-quan", 30+random(30));
	set_skill("begging", 120);

	map_skill("cuff", "taizu-quan");
	map_skill("parry", "taizu-quan");
	map_skill("dodge", "xiaoyaoyou");
	prepare_skill("cuff", "taizu-quan");

	set("chat_chance", 5);
	set("chat_msg", ({
		query("name")+"伸了个懒腰，活动了下筋骨。\n",
		query("name")+"无聊地东张西望，似乎想要寻找什么。\n",
		(: random_move :)
	}) );

	create_family("丐帮", 20, "一袋弟子");
	setup();

	carry_object(ARMOR_D("caoxie"))->wear();
	carry_object(ARMOR_D("bainayi"))->wear();
	carry_object(CLASS_D("gaibang/obj/gb_budai1"))->wear();
}
