// mohebasi.c 摩诃巴思
// by iceland

#include <ansi.h>

inherit NPC;
inherit F_MASTER;

string ask_write();

void create()
{
        set_name("摩诃巴思", ({ "mohe basi", "mohebasi" ,"mohe","basi" }));
        set("long",@LONG
他长得身材瘦高，脸色黑里透红。他原来到大轮寺只求佛法，来到以后
却迷恋上了武功，但因为练功太晚，所以成就非常有限。
LONG
        );
        set("title", HIY "大轮寺第十三代弟子" NOR);
        set("gender", "男性");
        set("age", 49);
        set("attitude", "friendly");
        set("shen_type", -1);
        set("str", 20);
        set("int", 15);
        set("con", 22);
        set("dex", 24);
        set("max_qi", 700);
        set("eff_jing", 400);
	set("max_jing",400);
        set("neili", 700);
        set("max_neili", 700);
        set("jiali", 30);
        set("combat_exp", 100000);
        set("score", 20000);
	set("unique", 1);

        set_skill("huanxi-chan", 50);
        set_skill("literate", 50);
        set_skill("force", 50);
        set_skill("longxiang-boruo", 50);
        set_skill("dodge", 50);
        set_skill("yuxue-dunxing", 50);
        set_skill("hand", 50);
        set_skill("dashou-yin", 50);
        set_skill("claw", 50);
        set_skill("tianwang-zhua", 50);
        set_skill("parry", 50 );

        map_skill("force", "longxiang-boruo");
        map_skill("dodge", "yuxue-dunxing");
        map_skill("hand", "dashou-yin");
        map_skill("parry", "tianwang-zhua");
        map_skill("claw", "tianwang-zhua");

        prepare_skill("hand","dashou-yin");
        prepare_skill("claw","tianwang-zhua");

        set("write_count", 3);
        set("inquiry", ([
                "写经" : (: ask_write :),
                "写经书" : (: ask_write :),
        ]));

        create_family("大轮寺", 13, "弟子");
        set("class", "huanxi");

        setup();
        carry_object("/d/xueshan/npc/obj/qinxiu-jiasha")->wear();

        add_money("silver",50);
}

void attempt_apprentice(object ob)
{
	mapping myfam;
	myfam = (mapping)ob->query("family");

	if (myfam && myfam["family_name"] == "大轮寺"){
		command("ah "+(string)ob->query("id"));
		command("say "+ RANK_D->query_respect(ob) +"不必谦虚，我们共同研习佛法如何？\n");
		return;
	}

	if( (string)ob->query("gender") == "无性" ){
		command ("say 施主身体残缺，本寺可不收留。");
		return;
	}
/*
	if( (int)ob->query("combat_exp") > 10000 ){
		command ("say 看你的功夫成就我已经没什么可教的了，拜我为师，如何敢当。");
                return;
	}
*/
	if( (int)ob->query("str") < 20 ){
		command ("say 看你手无缚鸡之力，能干什么？想白吃白喝享清福么？");
                return;
	}
	if( (int)ob->query("con") < 20 ){
		command ("say 大轮寺终年风雪不断，看你身体单薄，恐怕是吃不消呀。");
                return;
	}
	command("say 好吧，以后你就先留在寺中当个杂役，干些杂活吧。");
	command("recruit " + ob->query("id"));
	ob->set("join_age",(int)ob->query("age"));
}

 string ask_write()
{
	object ob,obj,obj1;
	ob = this_player();
	
       if ((string)ob->query("family/family_name") != "大轮寺") 
                return RANK_D->query_respect(ob) +
                        "如心慕本派佛祖，是否应该先弃恶向善，归依本寺呢？";
	if (present("mao bi", ob) && present("bo juan",ob))
		return RANK_D->query_respect(this_player()) + 
		"你现在身上写经所用器具齐全，怎麽又来要了？";

	if (query("write_count") < 1)
		return "摩诃巴思面带歉意的说：“可惜你来的不巧，这会我这里没有笔和帛绢了。”";

                obj=new(MISC_D("bojuan"));
        obj1=new(BINGQI_D("maobi"));
                obj->move(ob);
                obj1->move(ob);
                add("write_count", -1);

	message_vision("摩诃巴思将笔和帛绢交给了$N。\n",ob);

        return "摩诃巴思面带微笑道：“善哉！善哉！"+RANK_D->query_respect(ob)+"勤奋好学，真是令人佩服。”";
}
