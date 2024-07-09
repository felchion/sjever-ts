// zhou.c 周颠
// Modify By River@sj 99.06
// Modify by Looklove@sj 2000/9/30



#include <ansi.h>
inherit NPC;
inherit F_MASTER;
string ask_blade();
void create()
{
        object weapon;
        set_name("周颠", ({ "zhou dian","zhou","dian" }) );
        set("title", "明教五散人");
        
        set("gender", "男性");
        set("age",52);
        set("long", "他就是明教五散人之一的周颠。\n");
        set("str", 25);
        set("int", 25);
        set("con", 25);
        set("dex", 25);
        set("shen_type",0);
        set("combat_exp", 600000);
        set("unique", 1);

        set_skill("blade",100);
        set_skill("force",100);
        set_skill("dodge",100);
        set_skill("hand", 100);
	set_skill("parry", 100);
        set_skill("piaoyi-shenfa",105);
        set_skill("shenghuo-shengong",105);
        set_skill("lieyan-dao",105);
        set_skill("suohou-shou",105);
        map_skill("hand","suohou-shou");
        map_skill("force","shenghuo-shengong");
        map_skill("dodge","piaoyi-shenfa");
        map_skill("blade","lieyan-dao");
        map_skill("parry","lieyan-dao");
        prepare_skill("hand","suohou-shou");

        create_family("明教",36,"散人");

        set("max_qi",1500);
        set("max_jing",1200);
        set("jiali",50);
        set("eff_jingli",1100);
        set("inquiry", ([
                "借刀"     : (: ask_blade :),
                "青焰刀" : "那是我的随身兵器，想用，可以借给你。\n",
                ]));
        setup();

        carry_object("/d/mingjiao/obj/yel-cloth")->wear();
        if (clonep()){
                weapon = unew(BINGQI_D("blade/qingyan-dao"));
                if(!weapon) weapon=new(BINGQI_D("blade"));
                weapon->move(this_object());
                weapon->wield();
        }
}

void init()
{
	::init();
        add_action("do_kill", "kill");
        add_action("do_kill", "tk");
        add_action("do_kill", "teamkill");
        add_action("do_kill", "hit");
}
string ask_blade()
{ 
        object blade,me,ob;
        me = this_player(); 
        ob = this_object();

        if ( me->query("combat_exp") < 100000 )
        return RANK_D->query_respect(me)+"的功夫太差，借给你，怕你也保不住宝刀。";
        if ( present("qingyan dao", me)|| me->query_temp("qydao"))
                return RANK_D->query_respect(this_player()) + 
                "刀只有一把，已经借给你啦！";
        if ( blade=present("qingyan dao", ob))
        command("give qingyan dao to "+me->query("id"));
        if(!objectp(blade))   blade = unew(BINGQI_D("blade/qingyan-dao"));
        if(!objectp(blade))   return "抱歉，这把青焰刀我已经借给别人了。"; 

        blade->move(me);
        blade = unew(BINGQI_D("blade"));
        blade->move(this_object());
        me->set_temp("qydao",1);
        command("wield dao"); 
        return "大家都是江湖同道，交个朋友,尽管拿去用。";
         
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


#include "5san_app.h";

