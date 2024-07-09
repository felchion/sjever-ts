//  zhu.c

inherit NPC;
inherit F_VENDOR;

int ask_lost();
void create()
{
        set_name("朱老板", ({ "zhu laoban", "zhu", "laoban" }));
        set("shen_type", 1);

        set("gender", "男性");
        set("age", 44);
        set("long", "这位老板显然很有钱，手上戴满了金戒指。\n");
        set_skill("unarmed", 24);
        set_skill("dodge", 21);
        set_temp("apply/damage", 18);

        set("combat_exp", 7000);
        set("attitude", "friendly");
        set("vendor_goods", ({
           (["name":__DIR__"obj/beauty_book","number":3]),
           (["name":BINGQI_D("yuxiao"),"number":5]),
           (["name":__DIR__"obj/necklace","number":20]),
           (["name":__DIR__"obj/goldring","number":25])
        }));
        set("inquiry", ([
		"遗失" : (:ask_lost:),
		"lost" : (:ask_lost:),
		]));
        setup();
        carry_object("/clone/misc/cloth")->wear();
}

void init()
{
        add_action("do_list", "list");
        add_action("do_buy", "buy");
}

void kill_ob(object ob)
{
        ob->remove_killer(this_object());
        remove_killer(ob);
        message_vision("$N沮丧地发现朱掌柜躲在高高的柜台后面，够不到。\n", ob);
}

void unconcious()
{
        say( "朱老板在怀里摸了一阵，摸出一个盒子来。他打开盒子，捏出一个药丸来塞在嘴里。\n");
        say( "片刻之间，朱老板又恢复了精神，“嘿嘿”奸笑了两声。\n");
        reincarnate();
        set("eff_qi", query("max_qi"));
        set("qi", query("max_qi"));
        set("eff_jing", query("max_jing"));
        set("jing", query("max_jing"));
        set("jingli", query("eff_jingli"));
}

void die()
{
        unconcious();
}

int ask_lost()
{
	object me = this_player();
	
	if ( (mapp(me->query("token/ring"))) && (!present("ring", me)) )
	{
		me->set_temp("token_lost","ring");
		command("say 前两天我花了一百两白银收购了一个白金戒指,难道是你丢的？");
		return 1;
	}
	
	if ( (mapp(me->query("token/bangle"))) && (!present("bangle", me)) )
	{
		me->set_temp("token_lost","bangle");
		command("say 前两天我花了一百两白银收购了一只翡翠玉镯,难道是你丢的？");
		return 1;
	}
	
	if ( (mapp(me->query("token/jade"))) && (!present("jade", me)) )
	{
		me->set_temp("token_lost","jade");
		command("say 前两天我花了一百两白银收购了一块龙凤玉佩,难道是你丢的？");
		return 1;
	}
	
	if ( (mapp(me->query("token/necklace"))) && (!present("necklace", me)) )
	{
		me->set_temp("token_lost","necklace");
		command("say 前两天我花了一百两白银收购了一条紫水晶项链,难道是你丢的？");
		return 1;
	}
	
	if ( (mapp(me->query("token/headgear"))) && (!present("headgear", me)) )
	{
		me->set_temp("token_lost","headgear");
		command("say 前两天我花了一百两白银收购一个黄金头饰,难道是你丢的？");
		return 1;
	}
	
	command("say 难道你想冒领失物？");
	return 1;
}

int accept_object(object me, object ob)
{
        int nmoney=ob->value();
        string str;
        object item;

        

        if(!str=me->query_temp("token_lost"))
                return notify_fail("朱老板说道：这位" + RANK_D->query_respect(me) + "，不知看中本店的什么珠宝？\n");
        if( ob->query("money_id") && nmoney >= 10000 ) 
        {
                item=new("clone/spec/item/"+me->query_temp("token_lost")+".c");
                command("say 罢了,君子不夺人所爱,这"+item->query("name")+"就还给"+ RANK_D->query_respect(me) + "吧。\n");
                item->move(me);
                me->delete_temp("token_lost");
                return 1;
        }
        else
        {
                message_vision("朱老板笑道：我可是花了一百两白银呀。\n", me);
                  return 0;
        }
}