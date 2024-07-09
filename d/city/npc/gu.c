// gu.c 顾炎武
// by pishou and zqb :)
// Updated by Jpei

#include <ansi.h>
inherit NPC;

string ask_jiannan();
string ask_degree();
string ask_prise();
void create()
{
	set_name("顾炎武", ({ "gu yanwu", "gu" }));
	set("long", "顾先生名炎武，字亭林，江苏昆山人士。
			顾先生是当世大儒，明亡之后，心伤国变，隐居不仕。
			他又高又瘦，面目黝黑。\n");
	set("gender", "男性");
	set("age", 55);
	set("per", 24);

	set("no_get","顾炎武对你来说太重了。\n");
	set_skill("literate", 2000);
	set_temp("apply/attack", 10);
	set_temp("apply/defense", 10);
	set_temp("apply/damage", 5);

	set("combat_exp", 500);
	set("shen_type", 1);
	setup();

	set("inquiry", ([
		"画" : "这是二瞻先生的丹青。",
		"二瞻" : "他乃是当今一位大画家。",
		"查士标" : "二瞻先生有位本家伊璜先生牵连在这场‘明史’大案中。",
		"伊璜" : "查伊璜, 也就是查继佐, 因大力将军所助, 免罪不究。",
		"大力" : "大力将军乃广东提督吴六奇将军。",
		"大力将军" : "大力将军乃广东提督吴六奇将军。",
		"吴六奇" : "吴六奇江湖上人称铁丐，真是个大英雄。",
		"天地会" : "天地会是台湾国姓爷延平郡王郑成功手下谋主陈永华所创。",
		"陈永华" : "陈先生暗中联络老兄弟，胸口所刺八字便是会里的口号。",
		"陈近南" : "就是天地会总舵主陈永华，江湖人言道：‘平生不识陈近南，便称英雄也枉然’。",
		"反清复明" : "你也要学一学当年岳武穆尽忠报国, 刺字以决你心? ",
		"岳武穆" : "岳武穆名飞字鹏举，乃抗金的名将，一生用兵如神。据说他死后留下了一部武穆遗书，其中记载着他一生戎马生涯的兵法要诀。",
		"武穆遗书" : "这部遗书最早收藏在皇宫之中，后听说铁掌帮帮主上官剑南率帮中高手，将此书偷走，不知是真是假。",
		"上官剑南" : (: ask_jiannan :),
		"学位" : (:ask_degree:),
		"科举" : (:ask_degree:),
		"学费" : (:ask_prise:),
	]) );
	set("chat_chance", 2);
	set("chat_msg", ({
		"顾炎武高声呤道：清风虽细难吹我，明月何尝不照人？ 真是绝唱！ \n",
		"顾炎武道：如此江山，沦于夷狄。我辈忍气吞声。偷生其间，实令人悲愤填膺。\n",
		"顾炎武道：终有一日驱除胡虏，还我大汉河山，比之徒抒悲愤，更加令人气壮。\n",
		"顾炎武道：当今之世，便真有桃源乐土，咱们也不能独善其身，去躲了起来。\n",
	}));
	carry_object(ARMOR_D("changshan"))->wear();
	carry_object(ARMOR_D("ruanxue"))->wear();
}

void init()
{
	add_action("do_yes","yes");
	add_action("do_no","no");
	::init();
}

string ask_jiannan()
{
	object me = this_object();
	object ob = this_player();

	if (present("zitie", ob)) return "顾炎武道：我不是已经告诉你了么，怎么还问？\n";
	command("say 这位上官帮主乃是一位侠义之士，铁掌帮自他接任后，力加整顿，多行侠
义之事，不过数年声势大振，在江湖上侵寻已可以与北方的丐帮分庭抗礼了。只是后来忽
然消声匿迹，不知所踪了。看来想找到武穆遗书，必先找到上官帮主才行！\n");
	command("look " + ob->query("id"));
	command("say 看来" + RANK_D->query_respect(ob) + "也是我辈中人，老朽这里有一幅字帖送给你，希望你
能不忘驱除胡虏，还我河山的志向！");
	new("/d/tiezhang/obj/zitie")->move(ob);
	message_vision("$N交给$n一幅字帖。\n", me, ob);
	return "";
}

string ask_prise()
{
	int money, level;
	int rate;
	/*
	玩家的literate等级低于80时，学习费用（GOLD）减少为原来的50%。
	玩家的literate等级高于80时，学习费用（GOLD）恢复正常计算公式。
	玩家的literate等级高于250时，学习费用（GOLD）增加为原来的150%。
	*/

	money = 50;
	level = this_player()->query_skill("literate", 1);

	rate=1;
	if(level > 100) rate=2;
	if(level > 400) rate=3;
	
	
	if (level > 29) money = 100*rate/2;
	if (level > 59) money = 200*rate/2;
	if (level > 89) money = 500*rate/2;
	if (level > 119) money = 1000*rate/2;
	if (level > 149) money = 2000*rate/2;

	return "你现在的学费是每次" + MONEY_D->money_str(money) + "，请备好存款。";
}

int is_apprentice_of(object ob)
{
	return 1;
}

int recognize_apprentice(object ob)
{
	int money = 50, level = ob->query_skill("literate", 1);

	if (level >= ob->query("max_pot")-100){
		message_vision(CYN"\n$N对着$n端详了一番道：“你因经验所制，暂时无法再进修更高深的学问了。”\n"NOR, this_object(), ob);
		return 0;
	}

	if (level > 29) money = 100;
	if (level > 59) money = 200;
	if (level > 89) money = 500;
	if (level > 119) money = 1000;
	if (level > 149) money = 2000;

	if(ob->query("balance")<money) {
			ob->delete_temp("mark/literate");
			return notify_fail("你现在学习一次所需要的费用是"+MONEY_D->money_str(money)+"，你存款不够了。\n");
	}
	ob->add("balance",-money);
	ob->set_temp("mark/literate", 1);
	return 1;
}

void kill_ob(object ob)
{
	if(!is_killing(ob->query("id")))
		command("chat* !!!");
	ob->start_busy(2);
	::kill_ob(ob);
}

int accept_object(object who, object ob)
{
	object me = this_object();


	if (!who || environment(who) != environment()) return 0;
	if (!objectp(ob) ) return 0;
	if (!present(ob, who)) return notify_fail("你没有这件东西。");

	if (userp(ob)){
		command("fear "+(string)who->query("id"));
		command("say 你这粗人，怎么做出这等事来？");
		return 0;
	}

	if (ob->query("id") == "tu juan") {
		command("ah " + who->query("id"));
		command("say 真是想不到啊！竟然能见到" + ob->query("name") + "真迹！\n");
		message_vision("$N拿起$n仔细地欣赏着。\n", me, ob);
		who->set_temp("gu_gold", 150);
		remove_call_out("destroying");
		call_out("destroying", 1, ob);
		remove_call_out("ask");
		call_out("ask", 4, me, ob, who);
		return 1;
	}
	else if (ob->query("id") == "xiang shi") {
		command("ah");
		command("say 东汉画像石！你是从哪里搞到的？\n");
		message_vision("$N拿起$n仔细地欣赏着。\n", me, ob);
		who->set_temp("gu_gold", 100);
		remove_call_out("ask");
		call_out("ask", 4, me, ob, who);
		return 1;
	}
	else if (ob->query("id") == "kuaixueshiqing tie") {
		command("fly");
		command("say 快雪时晴帖！王羲之的真迹！！\n");
		message_vision("$N拿起$n仔细地欣赏着。\n", me, ob);
		who->set_temp("gu_gold", 400);
		remove_call_out("ask");
		call_out("ask", 2, me, ob, who);
		return 1;
	}
	else if (ob->id("xi shan xing lv tu")) {
		command("jump " + who->query("id"));
		command("say 范宽的溪山行旅图！我找它好久了！！\n");
		message_vision("$N拿起$n仔细地欣赏着。\n", me, ob);
		message_vision("$N把玩了半天，叹道：可惜可惜，这是无价之宝，老夫已经买不起了。\n", me);
	}
	else if (ob->id("ou xie pu")) {
		command("nod " + who->query("id"));
		command("say 老夫也略通棋理，看来这是一张很高深的棋谱，你可以去找识货的人。\n");
	}
	else if (ob->id("guang ling san")) {
		command("nod " + who->query("id"));
		command("say 老夫也略通音律，不过这本琴谱高深莫测，你还是去找识货的人吧。\n");
	}
	return 0;
}

void ask(object ob, object obj, object me)
{
	if(!me || environment(me) != environment()){
		command("say 咦，人呢？");
		destruct(obj);
		return;
	}
	command("hmm "+me->query("id"));
	message_vision(HIY"$N说道：在下极其喜爱这" + obj->query("name") + HIY"，不知这位" + RANK_D->query_respect(me) + "能(yes)不能(no)忍痛割爱呢？\n"NOR, ob, me);
	me->set_temp("gu_answer", 1);
	destruct(obj);
}

int do_yes()
{
	object me = this_player();

	if (!me->query_temp("gu_answer")) return 0;
	message_vision(CYN"$N连忙对$n一鞠躬，答道：先生竟然喜爱，那就当晚辈送给先生的礼物吧。\n"NOR,me, this_object());
	me->set_temp("gu_yes", 1);
	me->delete_temp("gu_answer");
	remove_call_out("answer");
	call_out("answer", 3, me);
	return 1;
}

int do_no()
{
	object me = this_player();

	if (!me->query_temp("gu_answer")) return 0;
	message_vision(CYN"$N面有难色：这个…这个…\n"NOR, me);
	me->delete_temp("gu_answer");
	remove_call_out("answer");
	call_out("answer", 4, me);
	return 1;
}

int answer(object me)
{
	int amount;
	object n_money, ob = this_object();

	if (!me->query_temp("gu_yes")) {
		command("pat "+me->query("id"));
		command("say 哈哈，我知你也舍不得。这样吧，就当是我求你，我再给双倍价钱。");
		message_vision("说着$N不由$n分说，塞给$n几锭黄金，就拿进内屋里去了。\n", ob, me);
		n_money = new("/clone/money/gold");
		n_money->move(me);
		amount = me->query_temp("gu_gold");
		n_money->set_amount(amount);
		me->delete_temp("gu_gold");
	}
	else {
		command("haha");
		command("say 好，那我就多谢你了！");
		me->delete_temp("gu_yes", 1);
		me->delete_temp("gu_gold");
		return 1;
	}
}

void die()
{
	object killer;

	if (objectp(killer = query_temp("last_damage_from")) ){
		killer->add("shen", -5000);
		killer->add_condition("killer", 30);
		command("chat "+killer->name()+"奸诈险恶，竟然背叛我朝，投靠清廷...");
	}
	::die();
}


string ask_degree()
{
	object me = this_player();

	string newtitle;
	int level;

	level = me->query_skill("literate",1);
	if (!level || level < 1 )
		newtitle = "文盲";
	else if (level < 20)
		newtitle = "学童";
	else if (level < 40)
		newtitle = "童生";
	else if (level < 60)
		newtitle = "书生";
	else if (level < 80)
		newtitle = "秀才";
	else if (level < 100)
		newtitle = "举人";
	else if (level < 120)
		newtitle = "解元";
	else if (level < 140)
		newtitle = "贡士";
	else if (level < 160)
		newtitle = "会元";
	else if (level < 200)
		newtitle = "进士";
	else if (level < 240)
		newtitle = "庶吉士";
	else if (level < 260)
		newtitle = "传胪";
	else if (level < 280)
		newtitle = "探花";
	else if (level < 300)
		newtitle = "榜眼";
	else if (level < 320)
		newtitle = "状元";
	else if (level < 340)
		newtitle = "翰林";
	else if (level < 360)
		newtitle = "学士";
	else if (level < 400)
		newtitle = "内阁大学士";
	else if (level < 460)
		newtitle = "内阁首辅";
	else if (level < 500)
		newtitle = "文坛领袖";
	else if (level < 560)
		newtitle = "文学大宗师";
	else newtitle = "文圣";

	me->set_temp("title", newtitle);

	return "凭阁下的学识，依老夫看，你可以做一名"+newtitle+"。";
}
