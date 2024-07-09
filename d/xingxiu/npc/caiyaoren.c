// npc: /d/xingxiu/npc/caiyaoren.c

inherit NPC;

void create()
{
        set_name("采药人", ({ "caiyao ren", "caiyao", "ren" }));
        set("gender", "男性");
        set("age", 30 + random(10));
        set("long", "一个辛苦工作的采药人。\n");
        set("attitude", "friendly");
        set("env/wimpy", 60);
        set("combat_exp", 1000);

        set_skill("unarmed", 20);
        set_skill("dodge", 20);
        set_skill("hammer", 20);
        set("apply/attack", 15);
        set("apply/defense", 15);
        set("apply/damage", 5);
        setup();
        add_money("coin", 50);
        carry_object(BINGQI_D("yaochu"))->wield();
        carry_object(ARMOR_D("yangao"))->wear();
}
