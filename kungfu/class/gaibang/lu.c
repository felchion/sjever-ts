// NPC: lu.c 鲁有脚
// Lklv 2001.9.28 update

#include <job_mul.h>
#include <ansi.h>
inherit NPC;
inherit F_MASTER;

string ask_me();
string ask_job();
int ask_num();

#include "gb_lujob.h"

void create()
{
	set_name("鲁有脚", ({"lu youjiao", "lu", "youjiao"}));
	set("title", HIW"丐帮九袋长老"NOR);
	set("nickname", "掌棒龙头");
	set("gender", "男性");
	set("age", 45);
	set("long", "他在丐帮中有仁有义，行事光明磊落，深得洪七公的器重。\n");

	set("attitude", "friendly");
	set("str", 25);
	set("int", 12);
	set("con", 25);
	set("dex", 20);
	set("unique", 1);
	set("no_bark",1);
	set("job_npc",1);
	set("no_get","鲁有脚对你来说太重了。\n");

	set("qi", 3500);
	set("eff_jingli", 2000);
	set("max_qi", 3500);
	set("jing", 2000);
	set("max_jing", 2000);
	set("eff_jing", 3200);
	set("neili", 4200);
	set("max_neili", 4200);
	set("jiali", 60);

	set("combat_exp", 1200000);
	set("score", 20000);

	set_skill("force", 190); // 基本内功
	set_skill("huntian-qigong", 190); // 混天气功
	set_skill("strike",195); // 基本拳脚
	set_skill("xianglong-zhang", 190); // 降龙十八掌
	set_skill("dodge", 190); // 基本躲闪
	set_skill("xiaoyaoyou", 190); // 逍遥游
	set_skill("parry", 190); // 基本招?	set_skill("literate", 100);
	set_skill("stick", 185); // 基本棍杖
	set_skill("begging", 160);

	map_skill("force", "huntian-qigong");
	map_skill("strike", "xianglong-zhang");
	map_skill("parry","xianglong-zhang");
	map_skill("dodge", "xiaoyaoyou");

	prepare_skill("strike", "xianglong-zhang");

	set("chat_msg_combat", ({
		(: perform_action, "strike.xiao" :),
		(: perform_action, "strike.paiyun" :),
		(: exert_function, "huntian" :),
	}));

	create_family("丐帮", 18, "九袋长老");

	set("inquiry", ([
		"棒法图解" : (:ask_me:),
		"报效国家" : (:ask_job:),
		"功劳": (: ask_num :),
		"洪七公": "我们平常都叫他“帮主”，亲近一点的叫他“七公”。\n",
	]));
	set("count",1);

	setup();
	carry_object(ARMOR_D("bainayi"))->wear();
	carry_object(ARMOR_D("caoxie"))->wear();
	carry_object(__DIR__"obj/gb_budai9")->wear();
}

void heart_beat()
{
	::heart_beat();

	clear_conditions_by_type("poison");
	if( !is_fighting() && living(this_object()))
	reincarnate();
}

void init()
{
	object me = this_player();
	int exp,pot,shen;

	::init();
	if( me->query_temp("gb_job2")
	&& me->query_temp("gb_job2_finish")>=5 ){
		me->delete_temp("gb_job2");
		me->delete_temp("gb_job2_finish");
		exp = 400 + random(300);

		exp = me->add_exp_combat(exp,this_object(),"草料场");

		pot = exp/5+random(exp/10);
		shen = exp*3;
		command("thumb "+me->query("id"));

		me->add("job_time/报效国家", 1);
		write( sprintf(HIW"\n好，任务完成了，你得到了"
		+ CHINESE_D->chinese_number( exp )
		+ "点实战经验，"
		+ CHINESE_D->chinese_number( pot )
		+ "点潜能和"
		+ CHINESE_D->chinese_number( shen )
		+ "点正神。\n" NOR));

		me->add( "potential", pot );
		if( me->query("potential") > me->query("max_pot") )
		me->set("potential", me->query("max_pot") );
		me->add( "shen", shen );
		me->set("job_name","火烧草料场");

		log_file("job/caoliao", sprintf("%8s%-10s火烧草料场任务，经验：%3d，潜能：%3d，目前经验：%d。\n",
		me->query("name"), "("+me->query("id")+")", exp, pot,me->query("combat_exp")),me );
	}
	add_action("qingjiao", "qingjiao");
}

string ask_me()
{
	object me = this_player();

	if ( me->query("family/family_name") != "丐帮" ){
		command("laugh " + me->query("id"));
		return RANK_D->query_respect(me) + "与本派素无来往，不知此话从何谈起？";
	}
	if (random(4)) {
		command("hehe");
		return "我不知道。";
	}
	if (query("count")){
		new(__DIR__"obj/stick-book")->move(me);
		add("count", -1);
		command(":(");
		message_vision("$N给$n一张纸片。\n", this_object(), me);
		return "好吧，这本图解你拿去研究，不懂的来问我。";
	}
	command("sigh " + me->query("id"));
	return "你来迟了一步，图解已经被人拿走了。";
}

string ask_job()
{
	object me = this_player();
	int exp = me->query("combat_exp");

	//if( (int)me->query("shen") < 0 )
	//      return "我看你獐头鼠目，不象个好人，如何能放心把军机大事托付给你。\n";

	if( exp < 3000 )
	return RANK_D->query_respect(me) + "纵有此心，奈何武功修为太差。\n";

	if( !wizardp(me) && me->query_condition("job_busy"))
	return "我这里现在没有什么任务，你等会再来吧。\n";

	if(me->query_temp("quest/busy")) //added by tangfeng 与quest冲突
	return "现在我这里没有给你的任务，你还是先处理好你其他事情再说吧。";


	if( me->query("menggu") )
	return "哼，你这汉奸，还想要任务？\n";

	if( exp < 80000 ){
		return gb_job1();
	}
	else if( exp < 500000 ){
		return gb_job2();
	}
	else if(exp < 1000000 ){
		return gb_job3();
	}
	else {
		return smy_job();
	}
	return "我这里没有什么可以给你做的任务了。\n";
}

int ask_num()
{
	object me = this_player();
	//old
	int num = me->query("job_time/报效国家");
	//old end
	//int num = me->query("job_time/告急") + me->query("job_time/火烧") + me->query("job_time/刺杀") + me->query("job_time/颂摩崖");

	command( "say "
	+ RANK_D->query_respect(me) + "已经为国家做了"
	+ CHINESE_D->chinese_number( num )
	+ "次贡献。" );
	command("addoil " + me->query("id"));
	return 1;
}

int qingjiao(string arg)
{
	object me = this_player();
	int lvl = me->query_skill("stick", 1);

	if (!living(me)) return 0;
	if (arg != "stick" || !present("stick book", me)) return notify_fail("你要请教什么？\n");
	if (!living(this_object())) return notify_fail("你先把他弄醒再说吧！\n");
	if (this_object()->is_busy() || this_object()->is_fighting())
	return notify_fail(this_object()->query("name")+"正忙着呢。\n");
	if (lvl < 30) return notify_fail("你的基本棒法修为不够，对鲁有脚的讲解似懂非懂。\n");
	if (lvl < 101) {
		write("你拿着棒法图解向鲁有脚请教。\n");
		me->receive_damage("jing", 30);
		write("你的「基本棒法」进步了。\n");
		me->improve_skill("stick", me->query("int"));
		return 1;
	}
	command("blush " + me->query("id"));
	return notify_fail("");
}