// dujie.c
// Modify by Looklove for ask mmjs busy

inherit NPC;
inherit F_MASTER;
#include "bai-du.h"
string ask_me();

void create()
{
        object ob;
        set_name("渡劫", ({ "du jie", "du", "jie" }) );
        set("title", "少林长老");
        set("gender", "男性");
        set("age", 90);
        set("class", "bonze");
        set("long","这是一个面颊深陷，瘦骨零丁的老僧，他脸色惨白，象一张纸一样。\n");
        set("attitude", "peaceful");
        set("combat_exp",2000000);
        set("shen", 50000);
        set("str", 30);
        set("int", 25);
        set("dex", 28);
        set("con", 28);
        set("qi", 8500);
        set("max_qi",8500);
        set("max_jing", 3000);
        set("eff_jingli", 2500);
        set("neili",8500);
        set("max_neili",5500);
        set("jiali", 70);
        set("no_get", 1);
        set("unique", 1);

        create_family("少林派", 35, "弟子");
        assign_apprentice("弟子", 0);
        set_skill("force", 180);
        set_skill("whip", 180);
        set_skill("parry", 180);
        set_skill("dodge", 180);
        set_skill("claw",180);
        set_skill("finger", 180);
        set_skill("yijin-jing", 180);
        set_skill("riyue-bian", 180);
        set_skill("shaolin-shenfa", 180);
        set_skill("longzhua-gong",180);
        set_skill("fumoquan-zhen",180);
        set_skill("yizhi-chan",180);
        set_skill("buddhism", 180);
        set_skill("literate", 150);
        map_skill("force", "yijin-jing");
        map_skill("whip",  "riyue-bian");
        map_skill("parry", "riyue-bian");
        map_skill("dodge", "shaolin-shenfa");
        map_skill("finger","yizhi-chan");
        map_skill("claw","longzhua-gong");
        prepare_skill("claw", "longzhua-gong");
        prepare_skill("finger", "yizhi-chan");
        set("chat_chance_combat", 60);
        set("chat_msg_combat", ({
                (: perform_action, "whip.chanrao" :),
                (: perform_action, "whip.fumoquan" :),
                (: perform_action, "claw.canyun" :),
                (: perform_action, "finger.qiankun" :),
                (: perform_action, "finger.wofo" :),
        }));
        set("inquiry", ([
                "木棉袈裟"     : (: ask_me :),
        ]));
        setup();
        if (clonep()) {
                ob = unew(BINGQI_D("whip/heisuo"));
                if (!ob) ob = unew(BINGQI_D("changbian"));
                ob->move(this_object());
                ob->wield();
                carry_object("/d/shaolin/obj/du-cloth")->wear();
        }
}

void init()
{
        int i,j;
        object me, ob;
        mapping fam;
        me = this_player();
        ob = this_object();
        ::init();
        if( interactive(me) )
        {
                if ( mapp(fam = me->query("family")) && fam["family_name"] == "少林派"
                && fam["generation"] <= 36 ) {
                        me->set_temp("fighter", 1);
                        return;
                        }
                if ( mapp(fam = me->query("family")) && fam["family_name"] == "少林派"
                && fam["generation"] > 36
                && me->query("qi") < 50 ) {
                        me->move("/d/shaolin/qyping");
                        me->unconcious();
                        return;
                }
                if(base_name(environment(ob))!="/d/shaolin/fumoquan") return;
                command("say 我佛慈悲！");
                i = me->query("combat_exp");
                j = me->query("potential");
                COMBAT_D->do_attack(ob, me, query_temp("weapon"), 2 );
                COMBAT_D->do_attack(ob, me, query_temp("weapon"), 2 );
                me->set("combat_exp", i);
                me->set("potential", j);
                me->set_temp("fighter", 1);
                return;
        }
        return;
}

string ask_me()
{
        object ob;
        if ( !this_player()->query_temp("fighter")
        ||    this_player()->query("combat_exp") < 5000 ){
                command("say 大胆鼠辈，乳臭未干，竟敢偷入金刚伏魔圈，且让老衲来超度与你！");
                this_object()->kill_ob(this_player());
                this_player()->kill_ob(this_object());
                return 0;
        }
        if ( this_player()->query_temp("mmjs"))
        	return "我不是给过你木棉袈裟了嘛？";
        if ( present("mumian jiasha", this_player()) )
                return RANK_D->query_respect(this_player()) +
                "木棉袈裟只有一件，而且就在你身上，真是贪得无餍！";
        if ( present("mumian jiasha", environment()) )
                return RANK_D->query_respect(this_player()) +
                "木棉袈裟只有一件，而且就在这里任你取走，真是贪得无餍！";
        if ( present("huangjin nao", this_player()) || present("huangjin nao", environment()) )
                return RANK_D->query_respect(this_player()) +
                "取了黄金铙，就不能再拿木棉袈裟，莫要贪得无餍！";
        ob = unew(ARMOR_D("armor/jingang-zhao"));
        if(!clonep(ob)) return "抱歉，你来晚了，木棉袈裟已经给人取走了。";

        if(clonep(ob) && ob->violate_unique()){
           	destruct(ob);
           	return "抱歉，你来晚了，木棉袈裟已经给人取走了。";
        }
        ob->move(this_player());
	this_player()->set_temp("mmjs",1);
        message_vision("\n渡劫一声不吭地瞧了$N半饷，扭过身，从树洞里取出木棉袈裟递给$N。\n\n", this_player());
        return "你能挨我们三个老不死的三招日月神鞭不死，确实是命大，这件木棉袈裟就是你的了！";
}
