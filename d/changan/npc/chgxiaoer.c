// Write By Yanyang@SJ 2001.9.27
// NPC: /d/changan/npc/chgxiaoer.c

inherit NPC;
inherit F_VENDOR;
void create()
{
        set_name("茶馆小二", ({ "chaguan xiaoer", "xiaoer" ,"waiter"}) );
        set("gender", "男性" );
        set("unique", 1);
        set("age", 18);
        set("long", "他是长安城茶馆里的小二，正忙得热火朝天。\n");
        set("combat_exp", 1000);
        set("attitude", "friendly");
        set("per", 18);

        set("chat_chance", 5);
        set("chat_msg", ({
"茶馆小二百里抽闲抬头看了看你。\n",
"茶馆小二擦了擦头上的汗。\n",
"茶馆小二利落的收拾干净了一张桌子。\n",
        }) );
        
        set("vendor_goods", ({
        (["name":FOOD_D("peanut"),"number":20]),
        (["name":FOOD_D("chahu"),"number":20]),
}));

        setup();
        carry_object(ARMOR_D("cloth.c"))->wear();
}


void init()
{        
        add_action("do_list", "list");
        add_action("do_buy", "buy");
}
