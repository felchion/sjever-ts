// due.c

inherit NPC;
inherit F_MASTER;
#include "bai-du.h"
string ask_me();

void create()
{
        object ob;
        set_name("渡厄", ({ "du e", "du", "e" }) );
        set("title", "少林长老");
        set("gender", "男性");
        set("class", "bonze");
        set("age", 90);
        set("long", "这是一个面颊深陷，瘦骨零丁的老僧，他脸色枯黄，如同一段枯木。\n");
        set("attitude", "peaceful");
        set("combat_exp",2200000);
        set("shen", 50000);
        set("str", 30);
        set("int", 25);
        set("dex", 28);
        set("con", 28);
        set("qi", 6000);
        set("max_qi",6000);
        set("max_jing", 3000);
        set("eff_jingli", 2500);
        set("neili",6000);
        set("max_neili",6000);
        set("jiali", 70);
        set("no_get", 1);
        set("unique", 1);

        create_family("少林派", 35, "弟子");
        assign_apprentice("弟子", 0);
        set_skill("force", 180);
        set_skill("whip", 180);
        set_skill("parry", 180);
        set_skill("dodge", 180);
        set_skill("buddhism", 180);
        set_skill("literate", 150);
        set_skill("yijin-jing", 180);
        set_skill("riyue-bian", 180);
        set_skill("shaolin-shenfa", 180);
        set_skill("hand",180);
        set_skill("fumoquan-zhen",180);
        set_skill("leg",180);
        set_skill("blade", 180);
        set_skill("xiuluo-dao", 180);
        set_skill("qianye-shou",180);
        set_skill("ruying-suixingtui",180);
        map_skill("force", "yijin-jing");
        map_skill("blade", "xiuluo-dao");
        map_skill("whip",  "riyue-bian");
        map_skill("parry", "xiuluo-dao");
        map_skill("dodge", "shaolin-shenfa");
        map_skill("hand","qianye-shou");
        map_skill("leg","ruying-suixingtui");
        prepare_skill("hand","qianye-shou");
        prepare_skill("leg","ruying-suixingtui");
        set("inquiry", ([
                "伏魔金铙"     : (: ask_me :),
        ]));
        set("chat_chance_combat", 60);
        set("chat_msg_combat", ({
                (: perform_action, "whip.chanrao" :),
                (: perform_action, "whip.fumoquan" :),
                (: perform_action, "hand.qianshou" :),
                (: perform_action, "leg.ruying" :),
        })); 
        setup();
        if (clonep()) {
                ob=unew(BINGQI_D("whip/heisuo"));
                if (!ob) ob = unew(BINGQI_D("changbian"));
                ob->move(this_object());
                ob->wield();
                carry_object("/d/shaolin/obj/du-cloth")->wear();
        }       
}

void init()
{
        int i, j;
        object me, ob;
        mapping fam;
        me = this_player();
        ob = this_object();
        ::init();
        if(interactive(me)){
                if (mapp(fam = me->query("family")) && fam["family_name"] == "少林派" 
                && fam["generation"] <= 36 ) {
                        me->set_temp("fighter", 1);
                        return;
		}
                if (mapp(fam = me->query("family")) && fam["family_name"] == "少林派" 
                && fam["generation"] > 36               
                && me->query("qi") < 50 ) {
                        me->move("/d/shaolin/qyping");
                        me->unconcious();
                        return;
		}
                if(base_name(environment(ob))!="/d/shaolin/fumoquan") return;
                command("say 阿弥陀佛！");
                i = me->query("combat_exp");
                j = me->query("potential");
                COMBAT_D->do_attack(ob, me, query_temp("weapon"), 2);
                COMBAT_D->do_attack(ob, me, query_temp("weapon"), 2);
                me->set("combat_exp", i);
                me->set("potential", j);
                me->set_temp("fighter", 1);
                remove_call_out("halt");
                call_out("halt", 0, me);
                return;
        }
        return;
}

void halt(object me)
{
	if(me->is_ghost()) return;
	if(!living(me) && environment(me) == environment()){
		message_vision("$N被渡厄用长鞭一卷，扔出了金刚伏魔圈！\n", me);
		me->move("/d/shaolin/qyping");  
		message_vision("$N被人丛金刚伏魔圈给扔了出来！\n", me);
	}
	else command("say 好吧，大家住手！ 看这家伙有什么话要说！");
}

string ask_me()
{
        object whip;
        object ob,me;
        me = this_player();
        
	if ( !this_player()->query_temp("fighter") 
	 ||  this_player()->query("combat_exp") < 5000 ){
		command("say 大胆鼠辈，乳臭未干，竟敢偷入金刚伏魔圈，且让老衲来超度与你！");
		this_object()->kill_ob(this_player());
		this_player()->kill_ob(this_object());
		return 0;
        }

        if ( present("huangjin nao", this_player()) || present("fumo nao", this_player()))
                return RANK_D->query_respect(this_player())+ "金铙只有一对，而且就在你身上，真是贪得无餍！";

        if ( present("huangjin nao", environment()) || present("fumo nao", environment()))
                return RANK_D->query_respect(this_player()) + 
                "金铙只有一对，而且就在这里任你取走，真是贪得无餍！";

        if(present("zhanyao nao", environment()) || present("zhanyao nao",this_player()))
                return RANK_D->query_respect(this_player())+ "你有了斩妖金铙，就不能再拿伏魔金铙，莫要贪得无餍！";

        if ( present("jingang zhao", this_player()) || present("jingang zhao", environment()) )
		return RANK_D->query_respect(this_player())+ "取了金刚罩，就不能再拿黄金铙，莫要贪得无餍！";

        if(me->query_temp("fmnao")) return "刚才伏魔金铙不是刚给你吗？"; 

	if( clonep()){
		ob= unew(BINGQI_D("blade/jinnao"));
		if(ob){
			destruct(ob);
			ob=unew(BINGQI_D("blade/jinnao1"));
		}
		if(ob) ob->move(this_object());
	}
	if ( present("fumo nao", this_object())){
		message_vision("\n渡厄长笑一声，点点头，将手中的伏魔金铙用布包好。\n\n", this_player());
		command("give fumo nao to "+this_player()->query("id")); 
		me->add_busy(2);
		me->set_temp("fmnao",1);
		whip = unew(BINGQI_D("whip/heisuo")); 
		if(clonep(whip)){
			whip->move(this_object());
			command("wield bian"); 
		}    
		else {
			if (!(whip = present("changbian", this_object()))) 
				whip = unew(BINGQI_D("changbian")); 
			whip->move(this_object());
			command("wield bian"); 
		}
		return "你既然能闯进金刚伏魔圈来，必然是当今武林中的非凡人物，这只铙是适逢其主了！";
	}
	return "抱歉，你来晚了，伏魔金铙已经给人取走了。";
}
