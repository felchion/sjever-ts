// kid.c

inherit NPC;

void create()
{
	set_name("女孩",({ "girl" }) );
        set("gender", "女性" );
        set("age", 12);
        set("long", 
"这是个美貌少女，虽然只有十二、三岁。\n");
        set_temp("apply/defense", 5);
	set("combat_exp", 70);
//        set("shen", 100);
	set("shen_type", 1);
        set("str", 14);
        set("dex", 15);
        set("con", 16);
        set("int", 15);
        set("attitude", "friendly");
        setup();
        carry_object(__DIR__"obj/cloth")->wear();
	add_money("coin", 10 + random(4));
}

