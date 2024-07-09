inherit NPC;
inherit F_MASTER;

void create()
{
	set_name("枯荣禅师", ({ "kurong chanshi", "ku", "rong","chanshi","master" }) );
	set("nickname", "天龙寺老僧");
	set("gender", "男性" );
        set("class","bonze");   
	set("age", 74);
	set("str", 30);
	set("cor", 27);
	set("cps", 32);
	set("int", 19);
        set("max_jing",3000);
        set("eff_jingli",3000);
        set("max_qi",4000);
        set("max_neili",4000); 
	set("max_force", 1500);
	set("force", 1500);
	set("force_factor", 70);
	set("rank_info/respect", "高僧");
	set("long", 
	"一个半面红润若童子，半面干枯若枯骨的老僧，他身材高大，精神矍烁。\n");
	create_family("天龙寺", 12, "弟子");
	set("combat_exp", 2000000);
	set("score", 200000);
	set("chat_chance_combat", 60);
	set("chat_msg_combat", ({
		(: exert_function, "powerup" :),
		(: exert_function, "recover" :),
	(: perform_action, "sword.qian" :)
	}) );
    set_skill("parry", 200);
    set_skill("dodge", 200);
    set_skill("force", 200);
    set_skill("finger", 200);
    set_skill("buddhism", 200);
    set_skill("literate", 200);
    set_skill("tianlong-shengong", 200);
    set_skill("yiyang-zhi", 200);
    set_skill("lingbo-weibu", 200);
    map_skill("dodge", "lingbo-weibu");
    map_skill("parry", "yiyang-zhi");
    map_skill("finger", "yiyang-zhi");
    map_skill("force", "tianlong-shengong");
    prepare_skill("finger","yiyang-zhi");
	set("inquiry", ([
	"name": "贫僧就是枯荣",
	]) );

	setup();
//	carry_object(__DIR__ "obj/cloth")->wear();
}
void attempt_apprentice(object ob)
{
  string old_name,new_name;
  if((int)ob->query_skill("buddhism",1)<120)
       {
         command("say 我段家武功与佛学息息相关，我看"+RANK_D->query_respect(ob)+"的禅宗修为似乎不够？");
          return;
        }
       if((int)ob->query_skill("tianlong-shengong",1)<150)
        {
          command("say 修练一阳指内功最为重要，我看"+RANK_D->query_respect(ob)+"似乎应在天龙神功上多下点功夫。");
          return;
        }
       command("smile");
        old_name = ob->query("name");
        new_name="本"+old_name[1..1];

       if ((string)ob->query("class")!="bonze")
       {       
        command("say 贫僧只收出家弟子，今日老僧帮你剃度。");
        //command("chat  + old_name+ 今日在天龙寺出家，老僧给他剃度。"\n);
        message_vision("枯荣禅师伸出一只皮包骨头的瘦手在"+ob->query("name")+"的头上一按，顿时头发尽落。\n",ob);
	}
        ob->set("name",new_name);
        ob->set("class","bonze");
        command("say 入我门来法名"+new_name);
        command("recruit " + ob->query("id") );
        command("chat 哈哈哈哈!");
        command("chat 老衲终于找到一个可传衣钵之人,真是可喜可贺!");
        return;
}

void recruit_apprentice(object ob)
{
	if( ::recruit_apprentice(ob) )
      	ob->set("class", "bonze");
}
