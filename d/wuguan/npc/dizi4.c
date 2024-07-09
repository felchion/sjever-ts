//dizi4.c 孙均 by river

#include <ansi.h>
inherit NPC;
int ask_wuguan();
int ask_liguan();
int ask_xuexi1();

void create()
{
	set_name("孙均", ({ "sun jun","sun","jun", "man" }));
	set("title", "襄阳武馆四弟子");
	set("gender", "男性");
	set("age", 21);
	set("long", "他是万震山的四弟子，相貌和蔼，一天到晚笑呵呵的。\n");
	set("combat_exp", 5000);
	set("shen_type", 1);
	set("attitude", "friendly");
	set("unique", 1);

	set_skill("force", 50);
	set_skill("strike", 50);
	set_skill("sword", 50);
	set_skill("shenzhao-jing",50);
	map_skill("force", "shenzhao-jing");

	set("inquiry", ([
		"万震山" : "他老人家是我的恩师啊。",
		"离馆" : (:ask_liguan:),
		"武馆" : (:ask_wuguan:),
		"襄阳武馆" :(:ask_wuguan:),
		"学习" : (:ask_xuexi1:),
	]));
	setup();
	carry_object(ARMOR_D("cloth"))->wear();
}

void init()
{
	object ob;
	::init();
	if (interactive(ob = this_player()) && !is_fighting()) {
		remove_call_out("greeting");
		call_out("greeting", 0, ob);
	}
}

void greeting(object ob)
{
	if( !ob || environment(ob) != environment() ) return;
	if ( ob ->query("combat_exp") < 500 && ob->query_temp("jobask") == 1){
		command("bow "+ob->query("id"));
		command("say 这位" + RANK_D->query_respect(ob) +",你可以 "HIY HBCYN"ask sun about 学习"CYN" 来向我了解武馆的情况。"NOR);
		
	}
}

int ask_xuexi1()
{
	object ob = this_player();
	if( ob->query_temp("jobask") == 1 ) {
		command("say 你不妨四处看看"HIY HBCYN"look"CYN"。地上有什么东西你都可以捡起来"HIY HBCYN"get"CYN"收着。\n"+
		"你可以先查查"HIY HBCYN"i or inventory"CYN"，看你现在身上有些什么。你要不时地查查"HIY HBCYN"hp\n"+
		CYN"你的身体状态，要是饿了或渴了就去找些吃的喝的吧。你要是有什么问题可\n"+
		"以问我。"HIY HBCYN"ask sun about 襄阳武馆，ask sun about 离馆"CYN"。\n"+
		"至于各种指令的具体用法可以参看 "HIY HBCYN"help commands"CYN"。"NOR);
		ob->delete_temp("jobask");
		if(ob->query_temp("jobover") != 1) {
			ob->set_temp("jobover", 1);
			command("say 听说狄云兄弟有点事情找你，你最好去看一下。"NOR);
			if ( ob->query("combat_exp") < 100) {
				ob->add("potential", 20);
				ob->add("combat_exp",100);
				tell_object(ob,HIW"你听了孙均的讲解，增加了一百点实战经验和二十点潜能，对江湖又多了一分了解。\n"NOR);
			}
		}
		return 1;
	}
	if ( ob->query_temp("jobover") == 1) {
		command("say 我不是都和你说了么，狄云兄弟有事找你，快去看下吧。");
		return 1;
	}
}

int ask_wuguan()
{
	object ob = this_player();
	if (ob ->query("combat_exp") < 1000){
		command("say  襄阳武馆是由我师傅“五云手”万震山他老人家一手建立的，当初\n"+
		"创立的目的就在于给一些刚入江湖的新手一些练功和实践的机会。具体的你可以\n"+
		"看看 "HIY HBCYN"help wuguan"CYN" 和 "HIY HBCYN"help map_wuguan"CYN"。"NOR);
		return 1;
	}
	else{
		command("kick "+ ob->query("id"));
		command("say 在武馆这么长时间了，你在干吗啊？自己熟悉去吧！\n");
		return 1;
	}
}

int ask_liguan()
{
	object book, ob = this_player();
	if (!ob->query("enter_wuguan")){
		command("congra "+ob->query("id"));
		command("say 这位" + RANK_D->query_respect(ob)+"可以出去闯荡江湖了，江湖险恶，可千万小心啊。往西走\n"+
		"就到了驿馆，从那里可以乘车到各大门派，出武馆就是襄阳了。");
		ob->set_temp("marks/离馆", 1);
		if (!ob->query_temp("have_xymap") && ob->query("max_neili") < 100 ) {
			new("/d/xiangyang/obj/map_book")->move(ob);
			book = new(MEDICINE_D("neili/yujiwan"));
			book->set("owner", ob->query("id"));
			book->move(ob);
			ob->set_temp("have_xymap", 1);
			ob->add("balance",100000);
			ob->set("per",25);
			command("say 我二师哥周圻闯荡江湖多年，这里有颗玉肌丸和一本襄阳地图册或许对你有用，拿去吧。");
			command("say 初入江湖多有不便，馆主已经在钱庄给你存点银子，你可要好生使用。");
			message_vision("$N递给$n一颗"+HIW+"玉肌丸"+NOR+"和一本"+HIC+"襄阳地图册"+NOR+"。\n", this_object(), ob);
		}
		return 1;
	}
	else{
		command("hmm "+ob->query("id"));
		command("say 这位" + RANK_D->query_respect(ob)+"没有馆主的同意，我可不敢轻易让你离开武馆。");
		return 1;
	}
}
