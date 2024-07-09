inherit NPC;
inherit F_MASTER;
string ask_me();
int do_nod(string);
void create()
{
	object ob;
	set_name("渡难", ({ "du nan", "nan" }) );
	set("title", "少林长老");
	set("gender", "男性");
	set("class", "bonze");
	set("age", 90);
	set("long", "这是一个面颊深陷，瘦骨零丁的老僧，他脸色漆黑，象是生铁铸成。\n");
	set("attitude", "peaceful");
	set("combat_exp",1200000);
	 set("shen", 50000);
	set("str", 30);
	set("int", 25);
	set("dex", 28);
	set("con", 28);
	set("qi", 3500);
	set("max_qi",5500);
	set("max_jing", 3000);
	set("eff_jingli", 2500);
	set("neili",3000);
	set("max_neili",3000);
	set("jiali",0);
	set("no_get", 1);
	set("unique", 1);

	create_family("少林派", 35, "弟子");
	assign_apprentice("弟子", 0);
	set_skill("force", 180);
	set_skill("whip", 180);
	set_skill("parry", 180);
	set_skill("dodge", 180);
	set_skill("jingang-quan",180);
	set_skill("banruo-zhang", 180);
	set_skill("yijin-jing", 180);
	set_skill("riyue-bian", 180);
	set_skill("cuff",180);
	set_skill("strike", 180);
	set_skill("shaolin-shenfa", 180);
	set_skill("buddhism", 180);
	set_skill("fumoquan-zhen", 180);
	set_skill("literate", 150);
	map_skill("cuff","jingang-quan");
	map_skill("strike", "banruo-zhang");
	map_skill("force", "yijin-jing");
	map_skill("whip",  "riyue-bian");
	map_skill("parry", "riyue-bian");
	map_skill("dodge", "shaolin-shenfa");
	prepare_skill("cuff","jingang-quan");
	prepare_skill("strike", "banruo-zhang");
	set("chat_chance_combat", 60);
	set("chat_msg_combat", ({
		(: perform_action, "whip.chanrao" :),
		(: perform_action, "whip.fumoquan" :),
		(: perform_action,"cuff.fumo" :),
	}));
	set("inquiry", ([
		"菩提子"     : (: ask_me :),
	]));
	set("count", random(10)-8);
	setup();
	if (clonep()) {
		ob = unew(BINGQI_D("heisuo"));
		if (!ob) ob = unew(BINGQI_D("changbian"));
		ob->move(this_object());
		ob->wield();
		carry_object("/d/shaolin/obj/du-cloth")->wear();
	}
}

void init()
{
	int i;
	object me, ob;
	mapping fam;

	me = this_player();
	ob = this_object();
	::init();
	if (base_name(environment()) == "/d/shaolin/dmyuan") {
		add_action("do_nod", "nod");
		call_out("do_back", 120);
		return;
		}
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
		command("say 普渡众生！");
		i = me->query("combat_exp");
		COMBAT_D->do_attack(ob, me, query_temp("weapon"), 2 );
		COMBAT_D->do_attack(ob, me, query_temp("weapon"), 2 );
		me->set("combat_exp", i);
		me->set_temp("fighter", 1);
		return;
	}
	return;
}

string ask_me()
{
	object ob;
	if ( !this_player()->query_temp("fighter")
	    || this_player()->query("combat_exp") < 5000 ){
		command("say 大胆鼠辈，乳臭未干，竟敢偷入金刚伏魔圈，且让老衲来超度与你！");
		this_object()->kill_ob(this_player());
		this_player()->kill_ob(this_object());
		return 0;
	}

	if ( present("puti zi", this_player()) )
		return RANK_D->query_respect(this_player())+"菩提子只有一颗，而且就在你身上，真是贪得无餍！";

	if ( present("puti zi", environment()) )
		return RANK_D->query_respect(this_player())+"菩提子只有一颗，而且就在这里任你取走，真是贪得无餍！";

	if (query("count") < 1)
		return "菩提子乃天地之灵物，采集艰难无比，我这里现在可没有。";

	ob = unew(MEDICINE_D("neili/puti-zi"));
	if(!clonep(ob)) return "菩提子乃天地之灵物，采集艰难无比，我这里现在可没有。";
	if(clonep(ob) && ob->violate_unique()){
	   destruct(ob);
	   return "菩提子乃天地之灵物，采集艰难无比，我这里现在可没有。";
	}
	ob->set("owner", this_player()->query("id"));
	ob->move(this_player());
	add("count", -1);
	log_file("quest/neili",sprintf("%s %s(%s)得到%s。\n",
		ctime(time())[4..19],this_player()->name(1),capitalize(this_player()->query("id")),ob->name()));
	message_vision("\n渡难惨然一笑，接着长叹一声，从树洞里取出个白玉磁瓶，倒出菩提子递给$N。\n\n", this_player());
	return "我等昔年于佛祖座前发下宏愿，每十年遍访名山大川，历尽艰险，\n" +
	       "方采得数颗菩提子，君乃有缘人，能得此造化奇物，望好自为之！";
}

void attempt_apprentice(object ob)
{
	if (ob->query("passdu")) {
		command("recruit " + ob->query("id"));
		return;
	}
	if (base_name(environment()) != "/d/shaolin/dmyuan" || !ob->query_temp("pending/waitdu")){
		command("say 对不起，老衲现在不收徒。");
		return;
	}
	ob->delete_temp("pending/waitdu");
	ob->set_temp("wait_nod", 1);
	command("say 依照少林寺历代规矩，" + RANK_D->query_respect(ob) + "你必须接下我五招，我才能收你为徒。");
}

int do_nod(string arg)
{
	object ob = this_player();
	if (!id(arg) || !ob->query_temp("wait_nod")) return 0;
	ob->delete_temp("wait_nod");
	call_out("test", 1, ob);
	return 0;
}
void test(object ob)
{
	object me=this_object();

	COMBAT_D->do_attack(me, ob, query_temp("weapon"), 2);
	COMBAT_D->do_attack(me, ob, query_temp("weapon"), 2);
	COMBAT_D->do_attack(me, ob, query_temp("weapon"), 2);
	COMBAT_D->do_attack(me, ob, query_temp("weapon"), 2);
	COMBAT_D->do_attack(me, ob, query_temp("weapon"), 2);
	call_out("check", 1, ob);
}

void check(object ob)
{
	string new_name;
	object ob1;

	if (!living(ob) || environment(ob) != environment()) {
		command("sigh");
		call_out("do_back", 2);
		return;
	}
	command("buddhi du nan");
	command("recruit " + ob->query("id"));
	new_name=ob->query("name");
	new_name[0..0] = "玄";
	ob->set("name", new_name);
	command("say 从今以后你的法名叫做" + new_name + "，恭喜你荣升为少林派玄字辈圣僧之一！\n");
	command("chat* 哈哈大笑道：“老衲垂死之年，又收一可塑之才，真是可喜可贺！”");
	command("say 大师一番勤学苦练，终有今日，且让老衲给你一颗菩提子,以增强功力。");
	if (!ob->query("passdu")) {
		ob->set("passdu", 1);
		ob->add("combat_exp", 3000);
		ob1=new(MEDICINE_D("neili/puti-zi"));
		log_file("quest/neili",sprintf("%-18s升辈从渡难处得到%s。\n",
			ob->query("name")+"("+capitalize(ob->query("id"))+")", ob1->name()), ob);
		ob1->set("owner",ob->query("id"));
		ob1->move(ob);
	}
	call_out("do_back", 3);
}

void do_back()
{
	command("say 老衲先告辞了。");
	command("bye");
	say("渡难快步走了出去。\n");
	destruct(this_object());
}
