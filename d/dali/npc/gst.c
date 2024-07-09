// NPC :gaoshengtai.c 高升泰 
// Modify By River 98/12 
inherit NPC;
inherit F_MASTER;
string huanggong();
#include <ansi.h>
void create()
{
        set_name("高升泰", ({ "gao shengtai", "gao","shengtai" }));
        set("title", "大理国善阐侯府侯爷");
        set("gender", "男性");
        set("age", 40);
        set("str", 28);
        set("dex", 20);
        set("long", "这是一个宽袍大袖的中年男子，三绺长须，形貌高雅，
双手持着一枝铁笛,此人便是人称为“高君侯”的高升泰。\n");
        set("max_qi", 2000);
        set("max_jing", 1000);
        set("neili", 2000);
        set("max_neili", 2000);        
        set("eff_jingli",1000);
        set("jiali",30);
        set("combat_exp", 500000);
        set("shen_type", 1);
        set("attitude", "friendly");
        set("unique", 1);
        create_family("天龙寺", 14 , "俗家弟子");
         
        set_skill("force",105);
        set_skill("sword",110);
        set_skill("cuff",90);
        set_skill("duanjia-quan",90);
        set_skill("dodge",100);
        set_skill("tianlong-xiang",90);
        set_skill("parry",110);
        set_skill("qingyan-zhang", 100);
        set_skill("strike", 100);
        set_skill("qiantian-yiyang",110);
        set_skill("duanjia-jianfa",100);
        map_skill("parry", "duanjia-jianfa");
        map_skill("dodge", "tianlong-xiang");
        map_skill("cuff","duanjia-quan");
        map_skill("force", "qiantian-yiyang");
        map_skill("sword", "duanjia-jianfa");       
        prepare_skill("cuff","duanjia-quan");

        set("inquiry", ([
        "入皇宫" : (: huanggong :),
        ]));

        set_temp("apply/attack", 70);
        set_temp("apply/defense", 70);
        set_temp("apply/armor", 70);
        set_temp("apply/damage", 20);        

        setup();
       if (clonep(this_object())) 
    if (ob = carry_object("/d/dali/npc/obj/tiedi")) ob->wield();

 //       carry_object(__DIR__"obj/zipao")->wear();
}

void attempt_apprentice(object ob)
{
       object me  = this_object();
       mapping fam = ob->query("family");
       mapping my_fam = me->query("family");    
       if (ob->query("gender") == "女性"){
         command ("say 这位小姑娘，本王府不收女徒，你还是请回吧。");
         return;
         }
       if (ob->query("tls")){                     
         command ("say 你已经是天龙寺出家弟子了，我不能再收你了！");
         return;
         } 
       if(ob->query("int") < 25){
         command("say 学我大理段家武功必须胆大心细，依我看" + RANK_D->query_respect(ob) + "的资质似乎不宜？");
         return;
         }
       if ((int)ob->query("shen") < 0){
         command ("say  我大理段氏乃堂堂正派，依我看" + RANK_D->query_respect(ob) +"眼露凶光，不适合入我大理段氏。");
         return;
         }
      if ( fam && fam["generation"] == "段正淳"){
         command("shake");
         command("say "+RANK_D->query_respect(ob)+"已拜镇南王为师，"+RANK_D->query_self(me)+"怎敢再收你为徒？");
         return;
         }
       if( fam && fam["master_name"] == "段正明"){
         command("haha");
         command("say "+RANK_D->query_respect(ob)+"已是皇上身前的侍卫，"+RANK_D->query_self(me)+"怎敢再收你为徒？"); 
         return;
         }
         command("smile");
         command("say 很好，" + RANK_D->query_respect(ob) + "只要肯为我大理多做事，他日必定能有所成就。");
         command("recruit " + ob->query("id") );
         ob->set("title","大理国镇南王府家丁");
}

void recruit_apprentice(object ob)
{
      if( ::recruit_apprentice(ob) )
      ob->set("class", "officer");
}

string huanggong()
{
        object ob=this_player();     
        if (!ob->query_temp("dali_join"))
           return RANK_D->query_respect(ob) + "，你不是本王府随从，此话从何说起？";
        if (ob->query("dali_job")<200)
           return "我看你为镇南王府做事做的还不够啊，努力做吧！";
        ob->set_temp("huanggong",1);
           return RANK_D->query_respect(ob)+"只要说是镇南王府的，就可以入宫了。";
}

