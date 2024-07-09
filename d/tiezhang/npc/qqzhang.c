// qqzhang.c 裘千丈
// Modify by Lklv 2001.10.18
// lane 2004.12.24 add 福利

#include <ansi.h>
inherit NPC;
inherit F_MASTER;
int ask_qiubai();
int ask_naogui();
int ask_gold();

void create()
{
	set_name("裘千丈", ({ "qiu qianzhang", "qiu", "qianzhang" }));
	set("long",
	"他是一个白须老头，身穿黄葛短衫，足穿麻鞋，右手挥着一把大蒲扇。\n"
	"他满脸的油滑无赖的表情。\n");
	set("gender", "男性");
	set("age", 60);
	set("attitude", "friendly");
	set("unique", 1);
	set("shen_type", -1);
       	set("str", 23);
	set("int", 30);
	set("con", 26);
	set("dex", 25);
	set("env/wimpy", 70);

	set("max_qi", 1000);
	set("max_jing", 1000);
	set("neili", 1000);
	set("max_neili", 1000);
	set("jiali", 80);
	set("combat_exp", 200000);
	set("score", 40000);

	set_skill("force", 60);
	set_skill("dodge", 60);
	set_skill("strike", 60);
	set_skill("parry", 60);
	set_skill("literate", 60);
	set_skill("tiezhang-zhangfa", 60);
	set_skill("guiyuan-tunafa", 60);
	set_skill("shuishangpiao", 60);
	set_skill("tiezhang-xinfa",60);

	map_skill("force", "guiyuan-tunafa");
	map_skill("strike", "tiezhang-zhangfa");
	map_skill("parry", "tiezhang-zhangfa");
	map_skill("dodge", "shuishangpiao");
	prepare_skill("strike", "tiezhang-zhangfa");

	create_family("铁掌帮", 15, "弟子");

	set("chat_chance_combat", 3);
	set("chat_chance", 3);
	set("chat_msg", ({
		"裘千丈大大咧咧的说道：“铁掌帮上上下下的事情，没有爷爷不知道的！”\n",
		"裘千丈神秘的说道：“无名峰上有一座坟墓，听说那里经常闹鬼！”\n",
		"裘千丈神秘的说道：“一旦内功基础有成，就可以从花草树木中采气来提高了！”\n",
		"裘千丈说道：“中指峰第二指节处，有一个神秘的所在，听说是已故帮主的埋骨之处！”\n",
		(: random_move :)
	}));
	set("chat_msg_combat", ({
		YEL"裘千仞正色道：你身上已受内伤，快回去密室中休养七七四十九日，不可见风，否则小命不保！\n\n"NOR,
		YEL"裘千仞急道：你再不住手，爷爷可就要不客气了！！！\n\n"NOR,
		(: random_move :)
	}));
	set("inquiry", ([
		"name" : "先父曾给我起过一个名字，叫甚么‘千丈’。我念着不好听，也就难得用它。\n",
		"here" : "这里就是铁掌帮的总坛所在，你如没事，不要四处乱走，免招杀身之祸！！！\n",
		"上官剑南" : (: ask_qiubai :),
		"闹鬼" : (: ask_naogui :),
		"逍遥馆" : (: ask_gold :),
	]));

	setup();
	carry_object(__DIR__"obj/huiyi")->wear();
}

void attempt_apprentice(object ob)
{
	object me  = this_object();
	mapping fam = ob->query("family");
	mapping my_fam = me->query("family");

	if(ob->query("gender")=="无性"){
		command ("say 嘿嘿，铁掌帮可不收你这种不男不女的人啊！");
		return;
	}
	if (fam && fam["family_name"] != "铁掌帮"
	 && (int)ob->query("is/tiezhang") == 1 ) {
		message_vision(BLU "$N！你这个被逐出本帮的叛徒还回来干什么 ？？？\n"NOR, ob );
		command("slap "+ob->query("id"));
		return;
	}
	if ((int)ob->query("shen") >= 0) {
		command("hehe");
		command("say 他妈的！爷爷我最恨你这样心慈手软的小王八蛋！。");
		return;
	}
	if( fam && fam["generation"] == my_fam["generation"] ){
		command("haha");
		command("say "+RANK_D->query_respect(ob)+"已是麻衣长老的弟子了，拜我为师干嘛呀？");
		return;
	}
	if( fam && fam["master_name"] == "裘千仞"){
		command("haha");
		command("say "+RANK_D->query_respect(ob)+"已是我弟弟的弟子了，"+RANK_D->query_self(me)+"怎好意思和我弟弟抢徒弟？");
		return;
	}
	if( fam && fam["master_name"] == "上官剑南"){
		command("ah");
		command("say 上官帮主不是早已故世多年了嘛？");
		command("say "+RANK_D->query_respect(ob)+"原来是老帮主关门弟子啊，失敬。"+RANK_D->query_self(me)+"怎敢再收你为徒？");
		return;
	}
	command("haha");
	command("say 爷爷我武功盖世，你这娃娃好好跟着我学，准没错！");
	command("recruit " + ob->query("id"));
	ob->set("is/tiezhang",1);
	ob->set("class","brigand");
}

int ask_qiubai()
{
	say("他是铁掌帮的第十三代帮主。听说十年前遭了暗算，死在湖北，不知是真是假！\n");
	return 1;
}

int ask_naogui()
{
	say("听一些帮众说，经常听见无名峰上的坟墓中，传出响声！嘿嘿！一定有什么蹊跷在里面！\n");
	this_player()->set_temp("marks/闹1", 1);
	return 1;
}

int accept_fight(object me)
{
	command("say " + RANK_D->query_respect(me) +
		"怕是活腻了，来来来，让爷爷教训教训你！！\n");
	command("say 啊哟，糟糕，糟糕，这会儿当真不凑巧！你等一会，我肚子痛，要出恭！\n");
	return 0;
}

int ask_gold()
{
	object me;
	int skill, gold;
	mapping myfam;
	me = this_player();
	skill = me->query_skill("tiezhang-zhangfa", 1);
	myfam = (mapping)me->query("family");

	if(me->query("shizhe_newbie")) {
		command("hmm "+(string)me->query("id"));
		command("say 找BUG呀，扣钱，扣钱！\n");
		return 1;
	}
	
	if( !myfam || myfam["family_name"] != "铁掌帮" ) {
		command("nod2");
		command("say 逍遥馆就是爷爷我开的，江湖上上下下的事情，没有爷爷不知道的！");
		return 1;
	}

	if( (int)me->query("tz_given") >= (int)me->query("age") ) {
		command("hmm "+(string)me->query("id"));
		command("say 我今年不是已经给过你钱拉？还不乖乖去打探消息啊！");
		return 1;
	}

	if( (int)me->query("age") < 18 || skill < 50 ) {
		command("yawn");
		command("say 就你这小细胳膊小细腿，能有什么用啊！\n");
		return 1;
	}

	gold = skill * 15000;
	if( 100 >= skill ) gold += gold/4;
	me->add("balance", gold);
	me->set("tz_given", (int)me->query("age"));
	command("say 爷爷我就是‘逍遥馆’的主人，‘逍遥馆’在江湖上赫赫有名！");
	command("say 这些钱你拿去，以后多帮我打探些消息吧，" + MONEY_D->money_str(gold) + "已经存入你的钱庄。");
	return 1;
}
