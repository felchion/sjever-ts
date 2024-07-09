// wufu.c 殷无寿
// Modify By River@sj 99.06
#include <ansi.h>
inherit NPC;
inherit F_MASTER;
string ask_yin();
void create()
{
        set_name("殷无寿", ({ "yin wushou", "yin", "wushou" }));
        set("title","明教天鹰分舵属下");
        set("age", 46);
				set("long","他是殷家的朴人。\n");
				set("str", 23);
        set("int", 20);
        set("con", 23);
        set("dex", 23);
        set("per", 10);
        set("unique", 1);
        set("env/wimpy", 80);
        set("combat_exp", 350000);
        set("attitude", "peaceful");
        set_skill("dodge",90);
        set_skill("force", 90);
        set_skill("piaoyi-shenfa", 90);
        set_skill("shenghuo-shengong", 90);
        set_skill("liehuo-jian",90);
        set_skill("yingzhua-shou",90);
        set_skill("hand",90);
        set_skill("parry", 90);
        map_skill("force", "shenghuo-shengong");
        map_skill("dodge", "piaoyi-shenfa");
        map_skill("hand", "yingzhua-shou");
        map_skill("parry", "yingzhua-shou");
        prepare_skill("hand","yingzhua-shou");
        set("inquiry", ([
            "殷无禄" : (: ask_yin :),
            "yin wulu" : (: ask_yin :),
        ]) );
        create_family("明教",38,"弟子");
        setup();
        carry_object("/d/mingjiao/obj/green-cloth")->wear();
}

void attempt_apprentice(object ob)
{
        if ( ob->query("int") < 20 ){
        	command("shake");
        	command("say 要能达到炉火纯青之境，体质什么的倒是无关紧要，悟性却是半点也马虎不得。");
        	return;
        }
        command("look " + ob->query("id"));
        command("say 嗯，看你还是个学武的料，我就收下你吧！");
        command("say 多谢明尊敝佑，让我明教又得一良材，为康扶正义，把我明教发扬光大吧。");
        command("recruit " + ob->query("id"));
        ob->set("title","明教天微堂教众");
}

string ask_yin()
{
        object me, ob, where;
        me=this_player();
        ob = find_living("yin wulu");
        if (!ob) return "刚有名弟子上来报告，我无禄兄好象被人给杀了。\n";
        where = environment(ob);
        if (!where) return "我无禄兄好象巡逻去了，我也不知道他现在在哪里。\n";
         return "嗯，无禄兄好象在"+where->query("short")+CYN"一带巡逻。"NOR;
}
