// fang.c 方舵主

#include <ansi.h>
inherit NPC;
inherit F_MASTER;

#include "begger.h";
string ask_book();
string ask_book2();

void create()
{
	set_name("方舵主", ({"fang duozhu", "fang", "duozhu"}));
	set("title", YEL"丐帮大仁分舵舵主"NOR);
	set("gb/bags", 8);
	set("gender", "男性");
	set("age", 30);
	set("long", "他是丐帮大仁分舵的舵主，生性忠厚，爱打抱不平。\n");
	set("attitude", "peaceful");
	set("str", 20);
	set("int", 30);
	set("con", 22);
	set("dex", 20);

	set("max_qi", 2420);
	set("max_jing", 1330);
	set("eff_jingli", 2180);
	set("neili", 2780);
	set("max_neili", 2780);
	set("jiali", 42);

	set("combat_exp", 720000);

	set_skill("force", 80);
	set_skill("huntian-qigong", 80);
	set_skill("strike", 80);
	set_skill("xianglong-zhang", 80);
	set_skill("dodge", 70);
	set_skill("xiaoyaoyou", 80);
	set_skill("parry", 80);
	set_skill("dagou-bang", 80);
	set_skill("stick", 80);
	set_skill("begging", 80);
	set_skill("lianhua-zhang", 90);

	map_skill("force", "huntian-qigong");
	map_skill("strike", "xianglong-zhang");
	map_skill("dodge", "xiaoyaoyou");
	map_skill("stick", "dagou-bang");
	prepare_skill("strike", "xianglong-zhang");

    	set("chat_chance_combat", 90);
    	set("chat_msg_combat", ({
        	(: perform_action, "strike.xiao" :),
        	(: perform_action, "strike.paiyun" :),
        	(: exert_function, "huntian" :),
        }));

        set("chat_chance", 2);
        set("chat_msg", ({
                "方舵主喃喃自语道：“降龙掌虽然厉害，可是学起来很难……”\n",
                "方舵主喃喃自语道：“莲花掌就比较好学，而且练起来容易些……”\n",
        }) );

	set("inquiry", ([
                "丐帮" : "我们丐帮是天下第一大帮！\n",
                "乔帮主" : "不管怎样，他永远是我的乔帮主。\n",
		"乔峰" : "不管怎样，他永远是我的乔帮主。\n",
		"萧峰" : "不管怎样，他永远是我的乔帮主。\n",
		"莲花掌" : "你要学我的莲花掌嘛？须得拜我为师才行。\n",
		"降龙十八掌" : "哈哈，厉害是厉害，不过我不能教你。\n",
		"rumors" : "传闻我们丐帮有本书，上面记载有练习掌法的窍门。\n",
		"掌法" : "掌法要想厉害，必须勤学苦练。\n",
		"洪七公" : "提那个老头干嘛？我没兴趣！\n",
		"练掌法" : (: ask_book :),
		"掌法图谱" : (: ask_book2 :),
	]));

	create_family("丐帮", 19, "弟子");

	setup();

        carry_object(ARMOR_D("bainayi"))->wear();
        carry_object(ARMOR_D("caoxie"))->wear();
	carry_object(__DIR__"obj/gb_budai8")->wear();
}
void attempt_apprentice(object ob)
{

	if( ob->query("shen") < 0 ) return;
        if ((int)ob->query("gb/bags") >= 2
        && ob->query("family/family_name") != "丐帮" ) {
                command("say 丐帮可容不下你这种背信弃义的小人！");
                command("kill "+ob->query("id"));
                return;
        }

        if((int)ob->query_con() < 20 ){
		command("say 你的体质较弱，不太适合学我的功夫。\n");
		return;
	}
	command("say 好，有你这样一等一的人才做我徒弟，我欢喜还来不及呢！");
        command("recruit " + ob->query("id"));

        if (!ob->query("gb/bags")) ob->set("gb/bags", 1);
        ob->set("title",sprintf("丐帮大仁分舵%s袋弟子",
                chinese_number(ob->query("gb/bags"))) );
        ob->set("gb/fenduo","大仁分舵");
        ob->set("class","beggar");
}

string ask_book()
{
	object me;
	me = this_player();

	if ( me->query("family/family_name") != "丐帮" )
		return RANK_D->query_respect(me)+ "与本派素无来往，不知此话从何谈起？";

	me->set_temp("ask_zftp",1);
	return "有本关于掌法的图谱，是从前帮主给我的……";
}

string ask_book2()
{
	object ob, me;
	me = this_player();

	if ( me->query("family/family_name") != "丐帮" )
		return RANK_D->query_respect(me)+ "与本派素无来往，不知此话从何谈起？";

	if (!me->query_temp("ask_zftp"))
		return "这个……这个……。";

        if (me->query_temp("zftpasked"))
        	return "我不是给你了么？";

        ob = unew("/clone/book/strike_book.c");
        if ( !clonep(ob) ) return "你来晚了，那本书我已经给人了。";
        if ( clonep(ob) && ob->violate_unique() ){
		destruct(ob);
		return "你来晚了，那本书已经被别人要走了。";
	}
	ob->move(me);
	me->delete_temp("ask_zftp");
	me->set_temp("zftpasked",1);
	return "既然你这么有心，这本书你拿回去好好看看。";
}