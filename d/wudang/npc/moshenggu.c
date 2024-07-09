// moshenggu.c
// by ydh

inherit NPC;

int ask_me_1();

void create()
{
        set_name("莫声谷", ({ "mo shenggu", "mo" }));
        set("nickname", "武当七侠");
        set("long", 
                "他就是张三丰的七弟子莫声谷。\n"
                "他年纪最轻，但身材魁梧，精明能干。\n");
        set("gender", "男性");
        set("age", 22);
        set("attitude", "peaceful");
        set("shen_type", 1);
        set("str", 26);
        set("int", 30);
        set("con", 26);
        set("dex", 28);
        
        set("max_qi", 1200);
        set("max_jing", 800);
        set("neili", 1500);
        set("max_neili", 1500);
        set("jiali", 50);
        set("combat_exp", 100000);
        set("score", 50000);

        set_skill("force", 80);
        set_skill("yinyun-ziqi", 60);
        set_skill("dodge", 80);
        set_skill("tiyunzong", 60);
        set_skill("cuff", 80);
        set_skill("taiji-quan", 60);
        set_skill("parry", 80);
        set_skill("sword", 80);
        set_skill("taiji-jian", 70);
        set_skill("taoism", 80);
        set_skill("literate", 80);

        map_skill("force", "yinyun-ziqi");
        map_skill("dodge", "tiyunzong");
        map_skill("cuff", "taiji-quan");
        map_skill("parry", "taiji-jian");
        map_skill("sword", "taiji-jian");
        prepare_skill("cuff", "taiji-quan");
        create_family("武当派", 2, "弟子");
//        set("inquiry", ([
//                "下山" : (: ask_me_1 :),
//        ]));

        setup();
        carry_object("/clone/weapon/changjian")->wield();
}
/*
string ask_me_1()
{
	object me;

  me=this_player();
 if ( (int)me->query_skill("yinyun-ziqi", 1) < 20)
   {
    command("pat "+(string)me->query("id"));
    return "你武艺未成，还是别下山的好！\n";
   }
  if((int)me->query("wudang_given")) 
    { command("kick "+me->query("id"));
      return "修道之人淡薄名利，你好好反省吧！\n";
    }
    command("nod "+(string)me->query("id"));
    return "你下山后要多行侠仗义，要是为非作歹，我必清理门户！\n";
}
*/
