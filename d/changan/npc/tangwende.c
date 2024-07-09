// Write By Yanyang@SJ 2001.9.27
// NPC : /d/changan/npc/tangwende.c

inherit NPC;
inherit F_VENDOR;
#include <ansi.h>
void create()
{
        set_name("唐文德", ({ "tang wende", "tang", "wende"}) );
        set("title",CYN"才高八斗"NOR);
        set("gender", "男性" );
        set("age", 47);
        set("per", 20);
        set("unique", 1);
        set("long","据说这位才高八斗的先生曾教过哪位当朝大官的书。\n");
        set("combat_exp", 3000);
        set("vendor_goods", ({
                (["name": BOOK_D("beauty_book.c"),"number":5]),
                (["name": BOOK_D("xu-book.c"),"number":5]),
        }));
        setup();
        carry_object(ARMOR_D("cloth"))->wear();
        carry_object(ARMOR_D("shoes"))->wear();
}

void init()
{
        add_action("do_buy", "buy");
        add_action("do_list", "list");
}