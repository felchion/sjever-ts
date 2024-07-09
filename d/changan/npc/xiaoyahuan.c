// Write By Yanyang@SJ 2001.9.26
// NPC: /d/changan/npc/xiaoyahuan.c

inherit NPC;

void create()
{
        set_name("小丫环", ({ "xiao yahuan", "yahuan"}) );
        set("gender", "女性" );
        set("age", 16);
        set("unique", 1);        
        set("long", "这位一个天真无邪的小丫环。\n");
        set("combat_exp", 100);
        set("attitude", "friendly");
        set("per", 30);

        set("chat_chance", 5);
        set("chat_msg", ({
"小丫环说道：“龙池在左边，凤池在右边。”\n",
        }) );
        setup();
        carry_object(ARMOR_D("skirt1.c"))->wear();
}

int accept_object(object who, object ob)
{
        
        return notify_fail("最近大老板外出，你下次在来吧。");
        if (ob->query("money_id") && ob->value() >= 10000) 
        {
                tell_object(who, "小丫环说道：“快进去洗洗身上的风尘吧！”\n");
                who->set_temp("pay_wash",1);
                return 1;
        }
        return 0;
}
