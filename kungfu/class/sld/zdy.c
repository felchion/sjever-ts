// zdy.c 张淡月
// Modified by Lklv add ask xionghuangjiu

inherit F_MASTER;
inherit NPC;
string ask_jiu();

void create()
{
	set_name("张淡月", ({ "zhang danyue","zhang" }));
	set("long", "他四十来岁年纪，是神龙教黑龙门掌门使。\n");
	set("gender", "男性");
	set("title", "神龙教黑龙门掌门使");
	set("age", 42);
	set("str", 20);
	set("int", 22);
	set("con", 22);
	set("dex", 22);
	set("per", 15);
	set("combat_exp", 200000);
	set("shen", -50000);
	set("attitude", "peaceful");
	set("max_qi",1000);
	set("max_jing",1000);
	set("neili",1000);
	set("max_neili",1000);
	set("jingli",800);
	set("eff_jingli",800);
	set("jiali",30);
	set("score", 20000);

	set_skill("force", 50);
	set_skill("dodge", 50);
	set_skill("parry",50);
	set_skill("strike",50);
	set_skill("huagu-mianzhang", 50);
	set_skill("youlong-shenfa",50);
	set_skill("dulong-dafa",50);

	map_skill("force", "dulong-dafa");
	map_skill("dodge", "youlong-shenfa");
	map_skill("strike", "huagu-mianzhang");
	map_skill("parry","huagu-mianzhang");
	prepare_skill("strike", "huagu-mianzhang");

	create_family("神龙教",2, "掌门使");
        set("inquiry", ([
                "神龙教" : "哈哈！我们神龙教最是逍遥自在。\n",
                "雄黄酒" : (: ask_jiu :),
        ]));
	setup();
	carry_object("/d/sld/npc/obj/bcp")->wear();
}

void attempt_apprentice(object ob)
{
	mapping myfam;
	object ob1;

	myfam = (mapping)ob->query("family");
	if(!myfam || myfam["family_name"] != "神龙教"){
		if(ob->query("app/sld")!=1){
			command("say 好吧，我就收下你做我的弟子。");
			ob->set("app/sld",1);
			command("recruit " + ob->query("id"));
			tell_object(ob, "你把这碗雄黄酒喝了，就可以不怕蛇咬了。\n");
			ob1 = new("/d/sld/npc/obj/xionghuangjiu");
			if(!ob1->move(ob))
				ob1->move(environment(ob));
			tell_room(environment(ob), "张淡月给了"+ob->name()+"一碗雄黄酒。\n");
		}
		else{
		command("say 神龙教不收你这种叛教之人！你走吧!");
		}
	}
}

int recognize_apprentice(object ob)
{
	mapping myfam;
	object hw;

	myfam = (mapping)ob->query("family");
	if(!myfam || (myfam["family_name"] != "神龙教")) return 0;
	if(!ob->is_apprentice_of(this_object())) return 0;
	if(myfam && (myfam["family_name"] == "神龙教")&&(ob->query("shen",1) >0)){
		command("say 哼！你竟然敢和白道中人搞在一起，是不是不想活了？！去黑屋好好反省反省吧！");
		if(!objectp(hw=find_object("/d/sld/npc/obj/heiwu")))
			hw=load_object("/d/sld/npc/obj/heiwu");
		ob->set("in_heiwu",time());
		ob->move(hw);
		return 0;
	}
	return 1;
}

string ask_jiu()
{
        object ob, me = this_player();

	if (me->query("family/family_name") != "神龙教")
                return "去找别人吧，我可不告诉你！";
        if (me->query("xionghuangjiu"))
                return "你好象已经喝过雄黄酒了，再喝不怕喝死你？";

        if (present("xionghuang jiu", me))
                return "你身上还有雄黄酒吧。";

        ob = new("/d/sld/npc/obj/xionghuangjiu");
        ob->move(me);
        message_vision("张淡月递给$N一碗雄黄酒。\n", me);
        return "你把这碗雄黄酒喝了，就不怕蛇咬了。";
}