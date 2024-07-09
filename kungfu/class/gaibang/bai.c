// bai.c 白世镜
// By Looklove@SJ 2000/9/27
// Looklove modified 2001.5.25 肚饿中
// tangfeng modified 2004.8 For tlbb
#include <ansi.h>
inherit NPC;
inherit F_MASTER;
#include "begger.h";
string ask_dao();
int ask_xiaofeng();

#define QUESTDIR "quest/天龙八部/"
#define QUESTDIR2 "quest/天龙八部/天龙营救篇/"
#define QUESTDIR3 "quest/天龙八部/萧峰身世篇/"

void create()
{
        set_name("白世镜", ({ "bai shijing", "bai"}));
        set("title", HIW"丐帮执法长老"NOR);
        set("gb/bags",9);
        set("gender", "男性");
        set("age", 47);
        set("long", "这人是丐帮中的执法长老，向来铁面无私。\n"
        	    "帮中大小人等，纵然并不违犯帮规刑条，见到他也是惧怕三分。\n");
        set("attitude", "peaceful");
        set("unique", 1);
        set("str", 25);
        set("int", 20);
        set("con", 20);
        set("dex", 20);

        set("qi", 2400);
        set("max_qi", 2400);
        set("jing", 1600);
        set("max_jing", 1600);
	set("eff_jingli", 1600);
        set("neili", 3400);
        set("max_neili", 3400);
        set("jiali", 100);
        set("combat_exp", 1500000);

        set_skill("force", 150);
        set_skill("huntian-qigong", 150);
        set_skill("dodge", 150);
        set_skill("xiaoyaoyou", 150);
        set_skill("parry", 130);
        set_skill("strike", 150);
        set_skill("lianhua-zhang",150);
        set_skill("hand", 120);
        set_skill("suohou-shou", 100);
        set_skill("begging", 90);

        map_skill("force", "huntian-qigong");
        map_skill("parry","lianhua-zhang");
        map_skill("strike","lianhua-zhang");
        map_skill("dodge", "xiaoyaoyou");
        map_skill("hand", "suohou-shou");
        prepare_skill("strike", "lianhua-zhang");
    	set("chat_chance_combat", 90);
    	set("chat_msg_combat", ({
        	(: perform_action, "strike.cailian" :),
        	(: perform_action, "strike.bingdi" :),
        	(: exert_function, "huntian" :),
        }));

        create_family("丐帮", 18, "九袋长老");

        set("inquiry", ([
                "丐帮" : "我们丐帮是天下第一大帮！\n",
                "乔峰" : "他不就是萧峰，我们的前任帮主。\n",
		            "萧峰" : (: ask_xiaofeng :),
		            "竹林惊变" : (: ask_xiaofeng :),
		            "丐帮内乱" : (: ask_xiaofeng :),
		            "竹林剧变" : (: ask_xiaofeng :),
	        "法刀" : (: ask_dao :),
        ]));
        setup();
        carry_object(ARMOR_D("bainayi"))->wear();
        carry_object(ARMOR_D("shoes"))->wear();
        carry_object(__DIR__"obj/gb_budai9")->wear();
}

void attempt_apprentice(object ob)
{
        if (ob->query("family/family_name")
        && ob->query("family/family_name") != "丐帮") {
                command("say 你还是去别的门派看看吧。");
                return;
        }
        if ((int)ob->query("gb/bags") >= 2
         && ob->query("family/family_name") != "丐帮" ) {
                command("say 丐帮可容不下你这种背信弃义的小人！");
                command("kill "+ob->query("id"));
                return;
        }
        if ((int)ob->query("gb/bags") < 3 ) {
                command("say 你在帮中的资历尚浅，过些日子再说吧！");
                return;
        }
        if ( ob->query("shen") <0 || ob->query("shen") < ob->query("combat_exp")/2) {
                command("say 想入我刑堂，再多做侠义的事吧。");
                return;
        }
        command("say 好吧，希望" + RANK_D->query_respect(ob) +
        "能好好学习本门武功，将来在江湖中闯出一番作为。");
        command("recruit " + ob->query("id"));
        ob->set("title",sprintf(HIR"丐帮刑堂%s袋弟子"NOR,
                chinese_number(ob->query("gb/bags"))) );
        ob->set("gb/fenduo","刑堂");
        ob->set("class","beggar");
}

string ask_dao()
{
        object me;
        object *list;

        me = this_player();

        if ( me->query("family/family_name") != "丐帮")
                return RANK_D->query_respect(me) +
                "非我丐帮好汉，不知此话从何谈起？";

        if ( me->query("family/master_name")!="白世镜")
                return RANK_D->query_respect(me) +
                "不是我刑堂弟子，何出此言？";

        if ( me->query_temp("fadao/asked"))
        	return "刚才你不是问过了么？";

        if ( present("fa dao", me))
                return RANK_D->query_respect(me) +
                "法刀你不是拿着呢吗？";

        list = filter_array(objects(), (: $1->query("id")=="fa dao":));
        if ( sizeof(list) > 8) {
        	me->set_temp("fadao/asked",1);
        	return "其他兄弟正在执法，等会吧";
        }

        new(BINGQI_D("blade/fadao"))->move(me);
	me->set_temp("fadao/asked",1);
        message_vision("$N得到一柄法刀。\n",me);
        return "好，你代我去各地巡视，发现不守帮规的弟子，就地处置。";
}
int ask_xiaofeng()
{
	object me = this_player();
	if(me->query(QUESTDIR2+"over") && (!me->query(QUESTDIR3+"start")||!me->query_temp(QUESTDIR3+"ask_bai")))
	{
		if(!me->query_temp(QUESTDIR3+"ask"))
		{
			command("sigh");
	    message_vision(HIC"$N沉默了一会了，道：“当时在竹林那时，丐帮其他长老都在场，你可以找其他人问问吧。”\n"NOR,this_object());
	    me->set_temp(QUESTDIR3+"ask",1);
			return 1;
		}
		if(!me->query_temp(QUESTDIR3+"ask_chen"))
		{
			command("sigh");
	    message_vision(HIC"$N沉默了一会了，道：“当时在竹林那时，陈长老也在，你可以找他问问吧。”\n"NOR,this_object());
			return 1;
		}
		if(!me->query_temp(QUESTDIR3+"ask_song"))
		{
			command("sigh");
	    message_vision(HIC"$N沉默了一会了，道：“当时在竹林那时，宋长老都在场，你可以找他问问吧。”\n"NOR,this_object());
			return 1;
		}
		if(!me->query_temp(QUESTDIR3+"ask_liang"))
		{
			command("sigh");
	    message_vision(HIC"$N沉默了一会了，道：“当时在竹林那时，梁长老都在场，你可以找他问问吧。”\n"NOR,this_object());
			return 1;
		}
		command("heng");
    me->set_temp(QUESTDIR3+"ask_bai",1);
    message_vision(HIC"$N道：“想必真相已经大白，萧峰确实不是汉人，当时可以作证的除了丐帮长老之外，还有谭公谭婆、赵钱孙、单老判官。”\n"NOR,this_object());
    message_vision(HIC"$N顿了一顿道：“可恨那厮为了掩盖事实，竟然杀死养父、害死师傅、烧毁单家庄，可谓是人面兽心，果然是契丹的种子！”\n"NOR,this_object());
    me->set_temp(QUESTDIR3+"ask_bai",1);
		return 1;
	}	
	if(!me->query(QUESTDIR3+"start")&& me->query_temp(QUESTDIR3+"ask_bai")&&!me->query(QUESTDIR3+"over"))
	{
		message_vision(HIC"$N脸上露出一幅咬牙切齿的样子。\n"NOR,this_object());
    command("say 萧峰是契丹狗种，还是堂堂汉人，此时还未分明。倘若他真是契丹胡虏，我白某第一个跟他拚了。。"NOR);
		return 1;
	}	
	if(me->query(QUESTDIR3+"start")&&!me->query(QUESTDIR3+"over"))
	{
		message_vision(HIC"$N脸上露出一幅咬牙切齿的样子。\n"NOR,this_object());
    command("say 萧峰是契丹狗种，还是堂堂汉人，此时还未分明。倘若他真是契丹胡虏，我白某第一个跟他拚了。。"NOR);
		return 1;
	}	
	if(me->query(QUESTDIR3+"over"))
	{
		command("sigh");
		command("say 那萧峰要是还是我们帮主该多好......"NOR);
		return 1;
	}	
	message_vision(HIC"$N沉默了一会了，道：“萧峰正是我丐帮中人。”\n"NOR,this_object());
	return 1;
}
