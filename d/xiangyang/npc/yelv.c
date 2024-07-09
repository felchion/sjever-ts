#include <ansi.h>
inherit NPC;
int ask_job();
void create()
{
        set_name("耶律齐", ({"yelv qi", "yelv", "qi"}));
        set("gender", "男性");
        set("age", 30);
        set("long", "他是丐帮的第二十代帮主，郭大侠的女婿。\n");
        set("attitude", "peaceful");
        set("str", 35);
        set("int", 25);
        set("con", 35);
        set("dex", 35);

        set("max_qi", 6000);
        set("max_jing", 6000);
        set("neili", 6000);
        set("max_neili", 6000);
        set("jiali", 100);     
        set("eff_jingli",6000);
        set("combat_exp", 1800000);
 
        set_skill("force", 200); 
        set_skill("huntian-qigong", 200);
        set_skill("strike", 200);
        set_skill("xianglong-zhang", 200);
        set_skill("dodge", 200);
        set_skill("xiaoyaoyou", 200);
        set_skill("parry", 200);

        map_skill("force", "huntian-qigong");
        map_skill("strike", "xianglong-zhang");
        map_skill("dodge", "xiaoyaoyou");
        map_skill("parry", "xiaoyaoyou");
        prepare_skill("strike", "xianglong-zhang");

        set("inquiry", ([
                "守城": (: ask_job :),
        ]) );

        set("chat_chance_combat", 100);
        set("chat_msg_combat", ({
                (: perform_action, "strike.xiao" :),
                (: exert_function, "powerup" :),
                (: perform_action, "strike.paiyun" :),
                (: perform_action, "strike.kanglong" :),
        }));
        create_family("丐帮", 20, "帮主");
        setup();
        
        if (clonep()) {
                carry_object("/d/thd/obj/grn-cloth.c")->wear();
        }
}

int ask_job()
{
       object me = this_player();
       if (!me->query_temp("xy/job"))
       {
               command("say 请先向我岳父请示。");
               return 1;
       }
      if ((me->query("family/family_name") == "峨嵋派")||
         (me->query("family/family_name") == "古墓派")||
         (me->query("family/family_name") == "桃花岛")||
         (me->query("family/family_name") == "丐帮"))
      {
               command("say 你快去指定的地点吧，看来蒙古兵就要攻城了！");
               return 1;
      }
      command("smile");
      command("say 这虽然没什么敌军，还是要小心有敌人偷袭。");
      me->set_temp("xy/job",2);
      return 1;
}
