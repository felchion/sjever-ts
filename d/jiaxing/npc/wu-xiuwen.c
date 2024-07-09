// wu-santong.c 武三通
// by shang 97/6

inherit NPC;

void create()
{
        set_name("武修文", ({ "wu xiuwen", "wu" }) );
	set("gender", "男性" );
        set("age", 11);
	set("long",
"武三通的小儿子?\n" "武学名家的儿子，却取了个斯文名字。\n");
	set("attitude", "friendly");
        set("str", 20);
        set("dex", 20);
        set("con", 20);
        set("int", 22);
        set("shen_type", 1);

        set_skill("unarmed", 35);
        set_skill("force", 20);
        set_skill("dodge", 20);
        set_skill("parry", 20);
        set("jiali", 10);

        set("combat_exp", 4000);

        set("max_qi", 250);
        set("max_jing", 100);
        set("neili", 100);
        set("max_neili", 100);
        set("max_jingli", 100);

        set("inquiry", ([
            "武三娘" : "是我娘。",
            "武敦儒" : "他是我哥哥。",
            "武三通" : "你怎么可以在儿子面前提老子的名字? ",
            "here" : "我迷了路, 也不知道在哪里。",
       ]) );
	setup();
        carry_object("/d/city/obj/cloth")->wear();
}
