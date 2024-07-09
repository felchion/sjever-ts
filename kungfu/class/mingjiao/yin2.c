// yin2.c 殷野王
// Modify By River@sj 99.06
// Modify By Looklove@sj 2000/9/17
#include <ansi.h>
inherit NPC;
inherit F_MASTER;
string ask_xunluo();
string ask_yin();
void create()
{
        set_name("殷野王",({"yin yewang", "yin", "yewang"}));
        set("title","明教天鹰分舵天微堂堂主");
        set("long", "他手摇折扇，行路足下生沙不起，便如是在水面上飘浮一般。\n"+
                    "这人绿的左襟上绣着一只小小黑鹰，双翅展开。他四十来岁\n"+
                    "年纪，但一双眼睛犹如冷电，精光四射，气势慑人。就是明\n"+
                    "教天鹰分舵天微堂堂主。当代明教教主的亲舅父殷野王。\n");
        set("age", 41);
        set("no_bark",1);
        set("env/wimpy", 40);
        set("attitude", "peaceful");
        set("shen", -8000);
        set("str", 25);
        set("int", 22);
        set("con", 25);
        set("dex", 25);
        set("per", 24);
        set("unique", 1);        
        set("max_qi", 1500);
        set("max_jing", 1300);
        set("jiali", 50);
        set("combat_exp", 800000);        

	set_skill("hand",110); 
	set_skill("dodge",110);
	set_skill("force", 110);
	set_skill("literate",110);
	set_skill("blade", 110);
	set_skill("sword", 110);
	set_skill("piaoyi-shenfa", 110);
	set_skill("shenghuo-shengong", 110);
	set_skill("yingzhua-shou",110);
	set_skill("liehuo-jian",110);
	set_skill("lieyan-dao",110);
	set_skill("parry", 110);

	map_skill("blade", "lieyan-dao");
	map_skill("sword", "liehuo-jian");
	map_skill("force", "shenghuo-shengong");
	map_skill("dodge", "piaoyi-shenfa");
	map_skill("hand", "yingzhua-shou");
	map_skill("parry", "yingzhua-shou");
	prepare_skill("hand","yingzhua-shou");

	set("inquiry", ([
		"殷无禄" : (: ask_yin :),
		"yin wulu" : (: ask_yin :),
		"画印"  : (: ask_xunluo :),
	]));
	set("xl", 8);
	set("xl2", "yin2");
	create_family("明教",37,"弟子");
	setup();
	carry_object("/d/mingjiao/obj/green-cloth")->wear();       
}

void init()
{
	::init();
        add_action("do_kill", "kill");
        add_action("do_kill", "tk");
        add_action("do_kill", "teamkill");
        add_action("do_kill", "hit");
}

void attempt_apprentice(object ob)
{        
        object me  = this_object();
        mapping fam = ob->query("family");
        mapping my_fam = me->query("family");
        if(!fam){
                command("hmm");
                command("say "+RANK_D->query_respect(ob)+"还是先去跟殷家三兄弟学点基本功再来吧。"); 
                return;
        }
        if ( fam && (fam["family_name"] == "武当" || fam["family_name"] == "峨嵋派"
            || fam["family_name"] == "少林派" || fam["family_name"] == "丐帮"
            || fam["family_name"] == "昆仑派" || fam["family_name"] == "华山派")){
                command("say 你们自命名门正派，还来这里做什？");
                return;
        }
        if(fam["master_name"] == "张无忌"){
                command("haha");
                command("say "+RANK_D->query_respect(ob)+"已是教主亲传弟子，"+RANK_D->query_self(me)+"怎敢再收你为徒？"); 
                return;
        }
        if (fam["generation"] <= my_fam["generation"]){
                command("shake");
                command("say "+RANK_D->query_respect(ob)+"已有明师，"+RANK_D->query_respect(me)+"怎敢再收你为徒？");
                return;
        }        
        if ((int)ob->query_skill("shenghuo-shengong", 1) < 50) {
                command("say 要学更为高深的武功，必须有高深的内功修为。"); 
                command("say " + RANK_D->query_respect(ob)+"是否还应该在圣火神功上多下点功夫？");
                return;
        }     
        command("look " + ob->query("id"));
        command("say 嗯，看你还是个学武的料，我就收下你吧！");
        command("say 多谢明尊敝佑，让我明教又得一良材，为康扶正义，把我明教发扬光大吧。");
        command("recruit " + ob->query("id"));
        ob->set("title","明教天微堂主座下香主");
}


string ask_yin()
{
	object me, ob, where;
	me=this_player();
	ob = find_living("yin wulu");
	if (!ob) return "刚有名弟子上来报告，我殷无禄好象被人给杀了。\n";
	where = environment(ob);
	if (!where) return "殷无禄好象巡逻去了，我也不知道他现在在哪里。\n";
		return "嗯，殷无禄好象在"+where->query("short")+CYN"一带巡逻。"NOR;
}

int accept_object(object who, object ob,object me)
{
	me=this_object();
	if ( me->query("family/family_name") != "明教"||ob->query("name")!="守卫书"){
		command("? "+who->query("id"));
		command("say 你给我东西有什么企图？！");
		return 0;
	}
	if(!who->query_temp("shouwei")){
		command("say 你是哪来的，没得到常遇春的允许不许守卫！");
		return 0;
	}
	if(ob->query("name")=="守卫书"){
		command("say 好吧，你既然有守卫书，就在这和我在这里一起守卫吧。");
		who->apply_condition("mj_shouwei",random(4)+7);
		who->set_temp("shouweied",1);
		return 1;
	}
	return 1;
}

int do_kill(string arg)
{
	object ob,ob2; 
        object me = this_player();
        ob2 = this_object();

        if (!arg) return 0;
        ob = present(arg, environment(me));
        if (ob == this_object() || ob == me) return 0;
        if (!ob || !ob->is_character()) return 0;

        
        if (userp(ob) && ob->query("family/family_name") == "明教") {
                if (me->query("family/family_name") == "明教") { 
			message_vision(ob2->name()+"对$N喝道：身为明教弟子，竟然杀自己的同门？我宰了你！\n", me);
		}
		else {
			message_vision(ob2->name()+"对$N喝道：大胆狂徒，居然敢来明教撒野！\n", me);
			kill_ob(me);
		}
		me->set("vendetta/明教", 1);
		return 1;
	}
        return 0;
}

#include "xunluo.h";
