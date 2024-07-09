// tong.c 
// Modified by Numa 19991019

#include <ansi.h>
inherit NPC;
#include "hmy_def.c"

int ask_dongfang();

void create()

{
        set_name("童百熊", ({ "tong baixiong", "tong",}));
        set("long", 
"一名须法俱白的老者，身材魁梧，双目炯炯
有神，他掌管着日月教风雷堂，负责着教中所有对外的事务。\n");
        set("title",HIY"日月神教 "HIC"风雷堂主"NOR);
        set("gender", "男性");
        set("age", 50);
        set("attitude", "peaceful");
        set("shen_type", -1);
        set("str", 28);
        set("int", 28);
        set("con", 28);
        set("dex", 38);

        set("max_qi", 1700);
        set("max_qi", 1700);
        set("max_jing", 1500);
        set("jing", 1500);
        set("neili", 1500);
        set("max_neili", 1500);
        set("eff_jingli", 1000);
        set("jingli", 1000);
        set("jiali", 50);
        set("combat_exp", 800000);
        set("shen", -1);

        set_skill("parry", 180);
        set_skill("dodge", 180);
        set_skill("force",180);
        set_skill("blade", 180);
        set_skill("ding-dodge",180);
        set_skill("tianmo-dao", 180);
        set_skill("tianmogong",180);

        map_skill("dodge", "ding-dodge");
        map_skill("force", "tianmogong");
        map_skill("blade", "tianmo-dao");
        map_skill("parry", "tianmo-dao");
        set("inquiry", ([
                "东方不败" : (: ask_dongfang :),
        ]));
        
        setup();
 
        carry_object("/clone/weapon/blade")->wield();
        carry_object("/clone/misc/cloth")->wear();        
}

int ask_dongfang()
{
        object ob = this_player();
        
        if (!ob->query_temp("hostage/job_1_1"))
        {
                command("? " + ob->query("id"));
                return 1;
        }
        command("say 当年我和东方兄弟出死入生，共历患难，" + RANK_D->query_rude(ob) + "你也想找他，先过了我这关再说！");
        command("say 来吧，让我试试你功夫如何！");
        ob->set_temp("hostage/pass_x",1);
        return 1;
}

int accept_fight()
{
        object me = this_object();
        object ob = this_player();
        
        if ((int)ob->query_temp("hostage/passed_x"))
        {
                command("say 咱们不是比试过了吗？");
                return 0;
        }

        if (me->query("eff_qi") < me->query("max_qi")
        || me->query("qi") < me->query("eff_qi")
        || me->query("neili") < me->query("max_neili"))
        {
                message_vision(CYN"$N对$n说：我还没有休息好，你等一会儿吧。\n"NOR, me, ob);
                return 0;
        }
        if (ob->query_temp("hostage/pass_x"))
        {
                message_vision(CYN"$N对$n说：好吧，那我就同你切搓几招吧，点到为止。\n"NOR, me, ob);
                remove_call_out("checking");
                call_out("checking", 1, me, ob);
                return 1;
        }
        else return 0;
}

int checking(object me, object ob)
{
        int my_max_qi,his_max_qi;
        my_max_qi  = (int)me->query("max_qi");
        his_max_qi = (int)ob->query("max_qi");
        if (me->is_fighting())
        {
                call_out("checking",1, me, ob);
                return 1;
        }
        if ( !present(ob, environment()) )
                return 1; 
        if (( (int)me->query("qi")*100 / my_max_qi) <= 50 )
        {
                message_vision(CYN"$N对$n不住地赞叹："+RANK_D->query_respect(ob)+"果然好功夫，你给我带路去盈盈那里吧。\n"NOR, me, ob);
                ob->delete_temp("hostage/pass_x");
                ob->set_temp("hostage/passed_x", 1);
                if (me->query("block_msg/all")>0)
                {
                        me->remove_call_out("revive");
                        me->revive();
                        me->reincarnate();
                }
                me->set_leader(ob);
                message_vision("$N决定跟随$n一起行动。\n",me,ob);
                return 1;
        }
        if (( (int)ob->query("qi")*100 / his_max_qi) < 50 )
        {
                message_vision(CYN"$N看了看$n，说道：你的功夫稀松平常，还想见我们教主？\n"NOR, me,ob);
                message_vision("$N急急忙忙离开了。\n",me);
                ob->set_temp("hostage/passno_x", 1);
                me->move(__HMY__"midao1");
                message_vision("$N急急忙忙走了过来。\n", me);
                return 1;
        }
        return 1;
}
