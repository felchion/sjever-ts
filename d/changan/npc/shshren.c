// Write By Yanyang@SJ 2001.9.27
// NPC: /d/changan/npc/shshren.c

inherit NPC;

void create()
{
        set_name("说书人", ({ "shuoshu ren", "ren"}) );
        set("gender", "男性" );
        set("age", 45);
        set("long", "他是一个讲述三国志，水浒传，大明英烈传等等英雄故事的说书人。\n");
        set("combat_exp", 1000);
        set("attitude", "friendly");
        set("per", 18);

        set("chat_chance", 5);
        set("chat_msg", ({
"说书人端起茶碗喝了几口茶水，又开始接着说书。\n",
"说书人讲得眉飞色舞。\n",
"说书人绘声绘色、滔滔不绝地说着书。\n",
        }) );
        setup();
        carry_object(ARMOR_D("cloth.c"))->wear();
}