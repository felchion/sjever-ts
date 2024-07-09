// /d/thd/npc/huangrong.c
// Modified by Numa 19990914

#include <ansi.h>
inherit NPC;
#include "nadir.c"
#include "thdjob.h"

int ask_husband();
int ask_shouhuan();
int ask_jiuyin();
int ask_job_s();
int ask_job();
int ask_job_time();
int ask_quest();
int ask_book();

void create()
{
	set_name("黄蓉", ({ "huang rong", "huang", "rong" }));
	set("long",
			"她方当韶龄，不过十五六岁年纪，肌肤胜雪，娇美无比，容色绝丽，不可逼视。\n"
			"她长发披肩，全身白衣，头发上束了一条金带，灿然生光。一身装束犹如仙女一般。\n");
	set("gender", "女性");
	set("rank_info/rude", "小妖女");
	set("age", 16);
	set("attitude","peaceful");
	set("str", 16);
	set("dex", 26);
	set("con", 18);
	set("int", 45);
	set("per", 50);
	set("shen_type", 0);

	set("tu", 1);
	set("shu", 1);
	set("shouhuan",1);

	set("jiali",50);
	set_skill("force", 200);
	set_skill("huntian-qigong", 200);
	set_skill("bihai-chaosheng", 200);
	set_skill("dodge", 200);
	set_skill("xiaoyaoyou", 200);
	set_skill("parry", 200);
	set_skill("strike", 200);
	set_skill("luoying-zhang", 200);
	set_skill("hand", 200);
	set_skill("lanhua-shou", 200);
	set_skill("finger", 200);
	set_skill("tanzhi-shentong", 200);
	set_skill("sword", 200);
	set_skill("yuxiao-jian", 200);
	set_skill("literate",500);
	set_skill("qimen-bagua", 500);
	set_skill("bangjue",200);
	set_skill("dagou-bang",200);
	set_skill("stick",200);

	map_skill("force", "huntian-qigong");
	map_skill("dodge", "xiaoyaoyou");
	map_skill("strike", "luoying-zhang");
	map_skill("hand", "lanhua-shou");
	map_skill("finger", "tanzhi-shentong");
	map_skill("parry", "dagou-bang");
	map_skill("sword", "yuxiao-jian");
	map_skill("stick", "dagou-bang");
	prepare_skill("hand", "lanhua-shou");

	set("chat_chance_combat", 100);
	set("chat_msg_combat", ({
		(: perform_action, "stick.chuo" :),
		(: perform_action, "stick.ban" :),
		(: perform_action, "stick.yin" :),
		(: perform_action, "stick.chuo" :),
		(: perform_action, "stick.chan" :),
		(: perform_action, "stick.wugou" :),
		(: exert_function, "huntian" :),
	}));

	set("combat_exp", 2500000);
	set("max_qi", 5000);
	set("max_jing", 4000);
	set("neili", 7500);
	set("max_jingli", 8500);
	set("eff_jingli", 8500);
	set("max_neili", 8500);

	set("inquiry", ([
		"name" : "在下黄蓉，是桃花岛主的女儿。",
		"rumors" : "我爹爹在这里布了一个八卦阵，必须精通八卦的卦象才能通过。",
		"here" : "这里就是桃花岛了。寒雨那个小坏蛋在这里加了无数的ｑｕｅｓｔ，看你能不能找全了。",
		"郭靖" : "靖哥哥虽然有点傻乎乎的，对我却是真心的。",
		"黄蓉" : "你找我有什么事吗？",
		"黄药师" : "我爹爹聪明绝顶，文才武学，书画琴棋，算数韬略，医卜星相，奇门五行，无一不精。",
		"爹爹" : "我爹爹聪明绝顶，文才武学，书画琴棋，算数韬略，医卜星相，奇门五行，无一不精。",
		"东邪" : "我爹爹聪明绝顶，文才武学，书画琴棋，算数韬略，医卜星相，奇门五行，无一不精。",
		"周伯通" : (: ask_jiuyin :),
		"八卦阵" : "是爹爹按八卦的卦象排的，不同的方向表示爻的变化与否，每三爻为一卦，八卦按顺序都走对就过阵了。",
		"爻" : "这也不知道？我没法帮你了。",
		"丈夫" : (: ask_husband :),
		"手环" : (: ask_shouhuan :),
		"夫君" : (: ask_husband :),
		"寒雨": "说起来也是他创造了我，不过这家伙是个造ｑｕｅｓｔ狂。",
		"jpei": "说起来也是他创造了我，不过这家伙是个造ｑｕｅｓｔ狂。",
// here add thd-job "ask huang" by Numa 19990911
		"师母": (: ask_job_s :),
		"守墓": (: ask_job :),
// ask job times by Numa 19991005
		"功劳": (: ask_job_time :),
// add for taohua quest
		"梅超风": (: ask_quest :),
		"陈玄风": (: ask_quest :),
// for hetu/luoshu
		"奇门八卦": (: ask_book :),
	]) );

//here set do job's number
	set("thd/havejob",1);

	setup();

	carry_object(__THD__"obj/gold_ribbon.c")->wear();
	carry_object("/d/city/obj/necklace.c")->wear();
	carry_object(__THD__"obj/white_cloth.c")->wear();
	carry_object(__THD__"obj/gold_belt.c")->wear();
	carry_object("/d/city/obj/goldring.c")->wear();
	carry_object("/clone/weapon/zhubang")->wield();
}

void reset()
{
	set("tu", 1);
	set("shu", 1);
	set("shouhuan",1);
}

int ask_jiuyin()
{
	object me= this_player();
	if (me->query_temp("jyquest")!=1) {
		command("say 不知道他现在怎么了。");
		command("consider");
		return 1;
	}

	command("say 他好像是被爹爹关在岛上，不知道为什么。");
	command("say 我天天都给他送饭，不过今天爹爹要我好好练些奇门算数。");
	command("angry");
	command("say 这样吧，你帮我送去可好？");
	message_vision(HIY"黄蓉给$N一个饭盒。\n"NOR,me);
	me->set_temp("jyquest",2);
	new(__THD__"obj/fanhe")->move(me);
	return 1;
}

int ask_shouhuan()
{
	object me= this_player();
	if (me->query("family/family_name")!= "桃花岛") {
		command("say 你又不是我桃花岛弟子，跑这里来瞎嚷嚷什么？");
		return 1;
	}
	if( present("yin shouhuan", me)) {
		command("angry");
		command("say 已经给你啦！");
	}
	if(query("shouhuan") < 1)
		return notify_fail("手环已经被人拿走了。\n");

	if(me->query("job_time/桃花岛") < 300) {
		command("say 你不多帮忙，我才不给你。");
		return 1;
	}
	command("say 好吧，看在你平常帮我不少忙，就给你了吧。\n");
	new("/d/thd/obj/shouhuan")->move(me);
	add("shouhuan",-1);
	return 1;
}

int ask_husband()
{
	object nvxu, user;
	object ob = this_player();
	string id;
	int user_load = 0;

	if (!(nvxu = find_object("/clone/npc/huang-son-in-law")))
		nvxu = load_object("/clone/npc/huang-son-in-law");
	id = nvxu->query("winner");
	if (!id)
		id = "";
	if (id == ob->query("id")) {
		if (nvxu->query("death_count") < ob->query("death_count"))
			say("黄蓉低下头，弄着衣角，什么话也不说。\n");
		else
			message_vision(CYN"黄蓉含情脉脉地看着$N，眼光娇羞无限。\n"NOR, ob);
	}
	else {
		if (!(user = find_player(id))) {
			user = new(LOADUSER_OB, id);
			if (!user->restore()) {
				say("黄蓉低下头，弄着衣角，什么话也不说。\n");
				destruct(user);
				return 1;
			}
			user_load = 1;
		}
		if (nvxu->query("death_count") < user->query("death_count"))
			say("黄蓉低下头，弄着衣角，什么话也不说。\n");
		else
			command("say 我的丈夫就是" + nvxu->query("name") + "！");
		if (user_load) destruct(user);
	}
	return 1;
}

int ask_job_time()
{
	object me = this_player();
	int i;

	if (me->query("family/family_name")!= "桃花岛") {
		command("say 你又不是我桃花岛弟子，跑这里来瞎嚷嚷什么？");
		return 1;
	}
	if(!intp(i = me->query("job_time/桃花岛")) || i<=1 ) {
		command("say 你好象从来没守过墓吧?");
		return 1;
	}
	message_vision("黄蓉点了点头对着$N说道：你已为我桃花岛成功守墓" + chinese_number(i) + "次。\n",me);
	command("addoil " + me->query("id"));
	return 1;
}

int ask_book()
{
	object me = this_player();
	object book;
	int i = random(20) + 1;

	if(me->query("potential") < i)
		return notify_fail("你的潜能不够了。\n");

	if(me->query("job_time/桃花岛") < 200) {
		command("say 你没帮忙我做过事吧？");
		command("hehe");
		return 1;
	}

	if((present("he tu", me)||present("luo shu", me)) && (me->query_skill("qimen-bagua",1) < 200)) {
		command("say 我就指点你一点奇门八卦的技巧吧。");
		message_vision(HIW"黄蓉指导了$N一点河图、洛书的奇门之学。\n"NOR,me);
		me->improve_skill("qimen-bagua", random(me->query_int())* i);
		me->add("potential", -i);
		if(me->query("job_time/桃花岛") < 0)
			me->set("job_time/桃花岛",0);
		return 1;
	}

	if (me->query("family/family_name")!= "桃花岛") {
		command("say 你又不是我桃花岛弟子，跑这里来瞎嚷嚷什么？");
		return 1;
	}

	if ((me->query("job_time/桃花岛")<200) && (me->query("thd/finish")<1)) {
		command("say 你帮我多做些事，我才要借你书。");
		command("xixi");
		return 1;
	}
	if((me->query_skill("qimen-bagua",1)<120) && (query("tu")>0)) {
		book = unew("/d/thd/obj/hetu");
		if (book) {
			command("say 好吧，这本河图你拿去好好研读吧。");
			book->move(me);
			add("tu",-1);
		} else {
			command("say 书已经不在我这了。");
			command("sorry");
		}
		return 1;
	}
	if( me->query_skill("qimen-bagua",1) > 100
	 && query("shu") > 0 ) {
		book = unew("/d/thd/obj/luoshu");
		if (book) {
			command("say 好吧，这本洛书你拿去好好研读吧。");
			book->move(me);
			add("shu",-1);
		} else {
			command("say 书已经不在我这了。");
			command("sorry");
		}
		return 1;
	}
	command("say 我现在没那种心情，别吵我啦。");
	command("yawn");
	return 1;
}

int ask_quest()
{
	object me = this_player();

	if (me->query("family/family_name")!= "桃花岛")	{
		command("say 你又不是我桃花岛弟子，跑这里来瞎嚷嚷什么？");
		return 1;
	}
	if (me->query_temp("thd/onquest")==0)	{
		command("say 他们本来是爹爹的徒弟......你还是自己去问爹吧。");
		command("consider");
		return 1;
	}
	if (me->query_temp("thd/onquest")==2)	{
		command("say 他们武功高强，你要小心了！");
		command("fear");
		return 1;
	}
	command("ah");
	command("say 我这儿有把匕首，你拿去防身....有危险时，就抽(chou)出来吧。");
	message_vision(HIY"$N小心翼翼地把匕首插在靴子。\n"NOR, me);
	command("say 这上面有靖哥哥的名字，可别弄掉了。");
	command("blush");
	me->set_temp("thd/onquest",2);
	return 1;
}
