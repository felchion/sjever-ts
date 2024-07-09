// NPC leng.c 冷谦
// Modify By River@sj 99.06
#include <ansi.h>
inherit NPC;
int waiting(object me);
int checking(object me);
int do_break(string arg);
void create()
{
        set_name("冷谦", ({ "leng qian","leng","qian" }) );
        set("nickname","冷面先生");
        set("title", "明教五散人");
        create_family("明教",36,"散人");
        set("gender", "男性");
        set("age",50);
        set("long", "他就是明教五散人之一，是明教的刑堂执法，\n"+
                    "他平时极少言语，但语出惊人。铁面无情。\n");
        set("str", 25);
        set("int", 25);
        set("con", 25);
        set("dex", 25);
        set("per", 15);
        set("combat_exp", 800000);
        set("unique", 1);

        set_skill("blade",105);
        set_skill("strike",105);     
        set_skill("force",105);
        set_skill("literate",80);
	set_skill("parry", 105);
        set_skill("dodge",105);
        set_skill("jinshe-zhangfa",105);
        set_skill("piaoyi-shenfa",105);
        set_skill("shenghuo-shengong",105);
        set_skill("lieyan-dao",105);
        map_skill("strike","jinshe-zhangfa");
        map_skill("force","shenghuo-shengong");
        map_skill("dodge","piaoyi-shenfa");
        map_skill("blade","lieyan-dao");
        map_skill("parry","lieyan-dao");
        prepare_skill("strike","jinshe-zhangfa");

        set("max_qi",2000);
        set("max_jing",1400);
        set("jiali",50);
        set("eff_jingli",1300);

        setup();
        carry_object("/d/mingjiao/obj/yel-cloth")->wear();    
}

#include "5san_app.h";

void init()
{
        object ob;
        ::init();
        add_action("do_break","break");
        if( interactive(ob = this_player()) && !is_fighting()) {
                remove_call_out("greeting");
                call_out("greeting", 1, ob);
        }
}
void greeting(object ob)
{
        object me;
        me=this_object();
        if (is_fighting()) return 0;
        if (wizardp(ob)) return 0;
        if (ob->query_temp("marks/狱1")&&!ob->query_temp("marks/狱2")){
                command("say 你，回，下次，死！");
                ob->set_temp("jianyu_name",ob->query("name"));
                remove_call_out("hiting_ob");
                call_out("hiting_ob",1,ob);
        }
        if (ob->query_temp("marks/狱2")&&ob->query_temp("marks/狱1")){
                command("stare "+ob->query("id"));
                call_out("do_kill",1,ob);
        }
}
int hiting_ob(object ob)
{
        object me;
        me=this_object();
        if(objectp(ob) && present(ob, environment(me)) && 
           !environment(me)->query("no_fight")){
               me->set_leader(ob);
               ob->kill_ob(me);
               me->fight_ob(ob);
               remove_call_out("moving_ob");
               call_out("moving_ob",1,ob);
               return 1;
        }        
        else me->move("/d/mingjiao/rukou");
        return 1;       
}

int moving_ob(object ob)
{
        object me;
        me=this_object();
        ob = this_player();
        if (!living(ob)){
                command("say 哼！");
                command("say 抓到，回。");
                ob->move("/d/mingjiao/jianyu");  
                ob->apply_condition("mj_jail", 120);
                ob->set_temp("marks/狱2",1);
                remove_call_out("do_back");
                call_out("do_back",1,me);
        }
        else {
          remove_call_out("hiting_ob");
          call_out("hiting_ob",1,ob);
        }
}

int do_kill(object ob)
{
        object me;
        me=this_object();        
        if( objectp(ob) && present(ob, environment(me))
                 && !environment(me)->query("no_fight")){
                me->set_leader(ob);
                me->kill_ob(ob);
                ob->fight_ob(me);
        }
}

int do_back(object me)
{
        me=this_object();
        message("vision", "冷谦急急忙忙的离开了。\n", environment(), me );
        me->move("/d/mingjiao/rukou");
        message("vision", "冷谦匆匆忙忙的回来了。\n",  environment(), me );
        me->set_leader(0);
        return 1;
}

int do_break(string arg)
{
        mapping fam;
        object me,ob;
        ob = this_player();
        me = this_object();
        if( !arg || arg=="" ) return 0;
        if(arg=="men"){
               if(!(fam = ob->query("family")) || !(fam["family_name"] == "明教") ) {
                       command("say 大胆，劫狱，死！\n");
                       me->kill_ob(ob);
                       return 1;
                       }
               else return notify_fail("内奸？\n");
       }        
}      
