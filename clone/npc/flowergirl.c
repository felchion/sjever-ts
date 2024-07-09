// /clone/npc/flowergirl.c

inherit NPC;
inherit F_VENDOR;

void create()
{
        set_name("卖花姑娘", ({ "maihua guniang", "guniang", "girl" }) );
        set("gender", "女性" );
        set("age", 18);
        set("per", 28);
        set("unique", 1);
        set("long","这位姑娘正忙著招呼客人，还不时给她的花浇浇水。\n");
	set("combat_exp", 50000+random(50000));
        set("attitude", "friendly");
        set("rank_info/respect", "小美人");
        set("vendor_goods", ({
        	(["name":"/d/dali/obj/chahua","number":5]),
        	(["name":"/d/dali/obj/chahua2","number":5]),
        	(["name":"/d/dali/obj/chahua3","number":5]),
        	(["name":"/d/dali/obj/chahua4","number":5]),
        	(["name":"/d/dali/obj/chahua1","number":5]),
        	(["name":"/d/city/npc/obj/w-rose","number":5]),
        	(["name":"/d/city/npc/obj/r-rose","number":5]),
        	(["name":"/d/foshan/obj/hua1","number":5]),
		(["name":"/d/foshan/obj/hua2","number":5]),
		(["name":"/d/foshan/obj/hua3","number":5]),
        }));
        setup();
	carry_object(ARMOR_D("cloth"))->wear();
	carry_object(ARMOR_D("shoes"))->wear();
}

void init()
{
        object ob = this_player();

        ::init();
        if (base_name(environment()) != query("startroom")) return;
        if( interactive(ob) && !is_fighting() ) {
		if ( ob->query("family/family_name") == "丐帮" && ob->query("gb/bags") < 2 ){
                        remove_call_out("saying");
                        call_out("saying", 0, ob);
                }
                else {
                        remove_call_out("greeting");
                        call_out("greeting", 1, ob);
                }
        }
        add_action("do_buy", "buy");
        add_action("do_list", "list");
}

void greeting(object ob)
{
        if( !ob || environment(ob) != environment() ) return;
        switch( random(2) ) {
                case 0:
                        say( "卖花姑娘笑咪咪地说道：这位" + RANK_D->query_respect(ob)
                                + "，进来看看茶花吧。\n");
                        break;
                case 1:
                        say( "卖花姑娘笑盈盈地说道：这位" + RANK_D->query_respect(ob)
                                + "，请进请进。\n");
                        break;
        }
}

void saying(object ob)
{
        if (!ob || environment(ob) != environment()) return;

        say("\n卖花姑娘皱皱眉,说道:对不起,小店今日还没卖出几盆花,请去别处吧！\n\n");
}
