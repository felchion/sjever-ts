// Update by Lklv 2001.10.18

#include <ansi.h>
inherit F_MASTER;
inherit NPC;

void create()
{
	set_name("孤鸿子", ({"guhong zi", "zi", "guhong"}));
	set("long","他就是峨嵋派的前任掌门人孤鸿子。\n"
		"他虽然衣衫褴褛，但自有一番威严，只是苍老的脸上时时露出忧郁的神色。\n"
	);
	set("gender", "男性");
	set("attitude", "friendly");
	set("unique", 1);
	set("age", 75);
	set("shen", 20000);
	set("str", 32);
	set("int", 25);
	set("con", 30);
	set("dex", 28);
	set("max_qi", 5500);
	set("max_jingli", 4000);
	set("max_jing", 2500);
	set("neili", 8500);
	set("max_neili", 8500);
	set("jiali", 80);
	set("combat_exp", 15000000);

	set_skill("dacheng-fofa", 200);
	set_skill("linji-zhuang", 500);
	set_skill("jieshou-jiushi", 500);
	set_skill("huifeng-jian", 500);
	set_skill("sixiang-zhang", 500);
	set_skill("yanxing-daofa", 500);
	set_skill("hand", 500);
	set_skill("literate", 240);
	set_skill("strike", 500);
	set_skill("linji-jianzhen", 500);
	set_skill("sword", 500);
	set_skill("blade", 500);
	set_skill("parry", 500);
	set_skill("dodge", 500);
	set_skill("force", 500);
	set_skill("anying-fuxiang", 500);

	map_skill("force", "linji-zhuang");
	map_skill("sword", "huifeng-jian");
	map_skill("blade", "yanxing-daofa");
	map_skill("parry", "yanxing-daofa");
	map_skill("dodge", "anying-fuxiang");
	map_skill("strike", "sixiang-zhang");
	map_skill("hand", "jieshou-jiushi");

	prepare_skill("hand", "jieshou-jiushi");
	set("chat_chance_combat", 40);
	set("chat_msg_combat", ({
		(: perform_action, "blade.huanying" :),
		(: perform_action, "hand.foguang" :)
	}));
	create_family("峨嵋派", 2, "前掌门");
	setup();
	set_temp("apply/dodge", 40);
	set_temp("apply/attack", 40);
	set_temp("apply/armor", 40);
	carry_object(BINGQI_D("blade"))->wield();
	UPDATE_D->get_cloth(this_object());
}

void init()
{
	object me, ob;
	::init();
	ob = this_player();
	me = this_object();
	if (interactive(ob = this_player()) && !is_fighting()) {
		remove_call_out("greeting");
		call_out("greeting", 1, ob);
	}
}

void greeting(object me)
{
	mapping myfam;
//	mapping lrn;
	int shen;
//	int lvl;
	myfam = (mapping)me->query("family");
	if (!myfam) return;
	shen = me->query("shen");
	if (shen < -200000 && myfam["master_name"] == "孤鸿子"){
		command("say 你满眼瘴气，身带邪气，再不思悔改，我要将你开除师门！");
	}
/*
        if ( me->query("class") != "bonze" && myfam["family_name"] == "峨嵋派"){
		lvl = (int)me->query_skill("linji-zhuang", 1);
		lrn = me->query_learned();
		if( !mapp(lrn) ) return;
		if( (int)lvl/10 == 14 && (int)me->query("emsujia")<1 ) {
			command("say 临济庄需要有高深的佛法相辅助，否则就会前功尽弃。");
			command("say 我们俗家弟子世俗缠身，不可能像佛门弟子那样潜心向佛。");
			command("say 你的内功修为虽说已到了一定层次，但你最好需要到后山修炼一下。");
			tell_object(me, "你听了师傅的话,深感有理。\n");
		}
		if( (int)lvl/10 == 16 && (int)me->query("emsujia")<2 ) {
			command("say 临济庄需要有高深的佛法相辅助，否则就会前功尽弃。");
			command("say 我们俗家弟子世俗缠身，不可能像佛门弟子那样潜心向佛。");
			command("say 你的内功修为虽说已到了一定层次，但你最好需要到后山修炼一下。");
			tell_object(me, "你听了师傅的话,深感有理。\n");
		}
		if( (int)lvl/10 == 17 && (int)me->query("emsujia")<3 ) {
			command("say 徒儿，你的功夫已经快赶上老夫了。为师有一桩心愿，那就是在为师的有生之年能够再看一眼(show)倚天剑。");
		}
	}
*/
}

void attempt_apprentice(object ob)
{
        object me;
        mapping ob_fam, my_fam;
        string name;

	me = this_object();
	my_fam  = me->query("family");
	name = ob->query("name");

	if (!(ob_fam = ob->query("family")) || ob_fam["family_name"] != "峨嵋派") {
		command("say " + RANK_D->query_respect(ob) + "与本派素无来往，不知此话从何谈起？");
		return;
	}
	if ((int)ob->query_skill("linji-zhuang",1) < 75) {
		command("say " + RANK_D->query_respect(ob) + "，你的临济十二庄修为不够，不能拜我为师。");
		return;
	}
        if (!ob->query("luohan_winner") ) {
		command("say " + RANK_D->query_respect(ob) + "，你的武学修为不够，不能拜我为师。");
		return;
        }
	if (ob->query("class") == "bonze" ) {
		command("say " + RANK_D->query_respect(ob) + "，我只收俗家弟子。");
		return;
        }
	command("chat* haha");
	command("say 老夫垂死之年，终得到一个衣钵传人，真是可喜可贺 !");
	command("recruit " + ob->query("id"));
	return;
}

int accept_object(object who, object ob)
{
	mapping myfam = (mapping)who->query("family");
	if(myfam["family_name"] != "峨嵋派"){
		command("say " + RANK_D->query_respect(who) + "与本派素无来往，不知此话从何谈起？");
		return 0;
	}
	if (ob->id("yitian jian") && !this_object()->query("jian"))  {
		command("say 这。。这就是当年被魔教恶人夺走的倚天剑啊！");
		command("say 老夫有生之年能再看到这把剑，真是死也瞑目了。");
		command("say 来，来，不能辜负了你的心意。");
		message_vision(HBGRN"孤鸿子对$N教导了一番，$N感觉胸中升起一股正气。\n"NOR,who);
		who->set_temp("emei/jianjue",1);
        	who->add("shen",5000);
		call_out("dest", 1, ob);
        	set("jian",1);
		call_out("delete", 900, "jian");
        	return 1;
	}
        return 0;
}

void dest(object ob)
{
	if (ob) destruct(ob);
}
