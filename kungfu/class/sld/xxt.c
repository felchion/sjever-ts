// 许雪亭

inherit F_MASTER;
inherit NPC;
void create()
{
        set_name("许雪亭", ({ "xu xueting","xu" }));
	set("long", "他四十来岁年纪，是神龙教青龙门掌门使。\n");
      set("gender", "男性");
	set("title", "神龙教青龙门掌门使");	
      set("age", 42);       
	set("str", 20);
      set("int", 22);
      set("con", 22);
      set("dex", 22);
      set("per", 15);
      set("combat_exp", 800000);
      set("shen", -50000);
      set("attitude", "peaceful");
	set("max_qi",3500);
      set("max_jing",3000);
      set("neili",3000);
      set("max_neili",3000);
      set("jingli",2500);
      set("eff_jingli",2500);
	set("jiali",80);
	set("score", 20000);

	set_skill("force", 150);
	set_skill("dodge", 150);
	set_skill("parry",150);
	set_skill("strike",150);
	set_skill("throwing",150);
	set_skill("huagu-mianzhang", 150);
	set_skill("youlong-shenfa",150);
	set_skill("dulong-dafa",150);
	set_skill("hansha-sheying",150);
	set_skill("poison",150);
	set_skill("shenlong-yaoli",150);

	map_skill("force", "dulong-dafa");
	map_skill("dodge", "youlong-shenfa");
	map_skill("strike", "huagu-mianzhang");
	map_skill("parry","huagu-mianzhang");
	map_skill("throwing","hansha-sheying");
	prepare_skill("strike", "huagu-mianzhang");
	
	create_family("神龙教",2, "掌门使");
	setup();
	carry_object("/d/sld/npc/obj/gcp")->wear();
}

void attempt_apprentice(object ob)
{
       mapping myfam;

       myfam = (mapping)ob->query("family");
 	if(!myfam || myfam["family_name"] != "神龙教")
           command("say 你非本教弟子，还是先去向黑龙使求教吧.");
       else if(ob->query("shen",1) > -20000)
           command("say 你还不够心黑手辣，再去多杀几个白道中人吧");
       else if(ob->query_skill("dulong-dafa",1)<120 || ob->query_skill("huagu-mianzhang",1)<120)
                   command("shake "+ob->query("id"));
       else
            {
           command("say 好吧，我就收下你做我的弟子。");
	    command("recruit " + ob->query("id"));
       }
	return;	
}

int recognize_apprentice(object ob)
{
    mapping myfam;
    object hw;

    myfam = (mapping)ob->query("family");
if(!myfam || (myfam["family_name"] != "神龙教")) return 0;
        if(!ob->is_apprentice_of(this_object())) return 0;
    if(myfam && (myfam["family_name"] == "神龙教")&&(ob->query("shen",1) >0))
    {
    	command("say 哼！你竟然敢和白道中人搞在一起，是不是不想活了？！去黑屋好好反省反省吧！");
	if(!objectp(hw=find_object("/d/sld/npc/obj/heiwu")))
		hw=load_object("/d/sld/npc/obj/heiwu");
	ob->set("in_heiwu",time());
	ob->move(hw);        
        return 0;
    }
    return 1;
}
