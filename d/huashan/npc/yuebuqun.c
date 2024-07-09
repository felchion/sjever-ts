// NPC: /d/huashan/npc/yuebuqun.c
// Date: Look 99/03/25
// Modify by Looklove 2000,12,15

inherit NPC;
inherit F_MASTER;

#include <job_mul.h>
#include <ansi.h>
string ask_zhengqi();
string ask_zixia();
string ask_me1();
string ask_me2();
string ask_pay();
string ask_fail();

void create()
{
	set_name("岳不群",({"yue buqun", "yue", "buqun" }));
	set("title","华山派第十三代掌门人");
	set("nickname","君子剑");
	set("long", "这就是在江湖中侠名远播，疾恶如仇，华山派掌门人。\n");
	set("age", 40);
	set("attitude", "friendly");
	set("no_bark",1);
	set("shen_type", 1);
	set("job_npc", 1);
	set("no_get", 1);

	set("per", 25);
	set("pur", 10);
	set("str", 30);
	set("int", 30);
	set("con", 30);
	set("dex", 30);

	set("max_qi",3800);
	set("qi", 3800);
	set("max_jing", 2400);
	set("jing",2400);
	set("eff_jingli", 2700);
	set("jingli", 2700);
	set("max_neili", 5500);
	set("neili", 5500);
	set("jiali", 50);
	set("combat_exp", 2400000);
	set("score", 30000);
	set("chat_chance_combat",100);
	set("chat_msg_combat", ({
		(: perform_action, "sword.yangwu" :),
		(: perform_action, "sword.feijian" :),
		(: perform_action, "sword.junzi" :),
		(: exert_function, "zixia" :),
		(: perform_action, "sword.kuaijian" :),
		(: perform_action, "sword.lianhuan" :),
		(: perform_action, "sword.sanqing" :),
		(: perform_action, "cuff.leidong" :),
		(: exert_function, "zixia" :),
	}));

	set_skill("poyu-quan", 180);
	set_skill("cuff", 180);
	set_skill("sword",  180);
	set_skill("dodge",  180);
	set_skill("force",  180);
	set_skill("parry",  180);
	set_skill("literate",100);
	set_skill("zixia-gong", 180);
	set_skill("huashan-jianfa",  180);
	set_skill("huashan-shenfa",  180);
	set_skill("zhengqi-jue", 180);

	map_skill("dodge", "huashan-shenfa");
	map_skill("force","zixia-gong");
	map_skill("sword","huashan-jianfa");
	map_skill("parry","huashan-jianfa");
	map_skill("cuff","poyu-quan");
	prepare_skill("cuff","poyu-quan");
	create_family("华山派",13,"掌门");

	set("inquiry", ([
		"紫霞秘籍"	: (: ask_zixia :),
		"zixia"		: (: ask_zixia :),
		"miji"		: (: ask_zixia :),
		"失败"		: (: ask_fail :),
		"fail"		: (: ask_fail :),
		"惩恶扬善"	: (: ask_me1 :),
		"job"		: (: ask_me1 :),
		"复命"		: (: ask_me2 :),
		"完成"		: (: ask_me2 :),
		"finish"	: (: ask_me2 :),
		"正气诀"	: (: ask_zhengqi :),
		"zhengqi-jue"	: (: ask_zhengqi :),
	]));

	setup();
	carry_object(BINGQI_D("sword"))->wield();
	carry_object(ARMOR_D("cloth"))->wear();
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
	object ob;
	::init();
	ob = this_player();
	if (interactive(ob = this_player()) && !is_fighting()) {
		remove_call_out("greeting");
		call_out("greeting", 1, ob);
	}
}

void greeting(object me)
{
	int shen;
	if(!me) return;

	if ( file_name(environment(this_object())) != this_object()->query("startroom")) return;

	shen = me->query("shen");
	if(shen <= -200000 && me->query("family/family_name") == "华山派"){
		command("disapp "+me->query("id"));
		command("say 你满眼瘴气，身带邪气，从今天起你不再是华山弟子了！");
		command("expell " + me->query("id"));
	}
	if(shen <0 && shen > -200000 && me->query("family/family_name") == "华山派"){
		command("sigh "+me->query("id"));
		command("say 我看你满眼瘴气，身带邪气，最近是不是在外面做了什么坏事？");
		command("say 如果被我查到你做过有辱我华山声名的事，我定不饶你！");
	}
}

void attempt_apprentice(object ob)
{
	mapping fam;
	if ( mapp(fam=ob->query("family"))
	 && (string)fam["family_name"] != "华山派"
	 && (int)ob->query("is/huashan")==1 ) {
		message_vision(CYN "$N！你这个华山派的叛徒还回来干什么 ？？？\n"NOR, ob );
		command("kick "+ob->query("id"));
		return;
	}
	if ( ob->query("quest/dgjj/fengset")) {
		command("say 你不是已经拜了剑宗的前辈为师了么？");
		return;
	}
	if ( ob->query("gender") != "男性" ){
		command("say 我只收男徒，" + RANK_D->query_respect(ob) + "请回吧。");
		return;
	}
	if ((int)ob->query("shen") < 0) {
		command("say 我华山派乃是堂堂名门正派，对弟子要求极严。");
		command("say 在德行方面，" + RANK_D->query_respect(ob) + "是否还做得不够？");
		return;
	}
	command("say 好吧，我就收下你了,你可要为我们华山派争口气。");
	command("recruit " + ob->query("id"));
}

string ask_me1()
{
	object ob, me, name1, ling;
	me=this_player();
	ob = this_object();
	ling = present("ling pai", me);
	name1 = me->query("id");

	if (me->query_temp("hs/jobji") || me->query_temp("hs/have"))
		return "你不能光说呀，倒是做出点成绩给我看看！\n";

	if (!wizardp(me) && (int)me->query("combat_exp") < 100000)
		return "你武功未成，这么危险的任务还是别做了！\n";

	if (me->query("job_name") == "惩恶扬善" && !wizardp(me))
		return "现在没有听说有恶人为害百姓，你自己去修习武功去吧！";

	if(!wizardp(me) && me->query_condition("job_busy"))
		return "你现在正忙着做其他任务呢！";

	if(!wizardp(me) && me->query_condition("task_busy"))
		return "你现在正忙着做其他任务呢！";
		
	if(me->query_temp("quest/busy")) //added by tangfeng 与quest冲突
		return "现在我这里没有给你的任务，你还是先处理好你其他事情再说吧。";


	ob = new(__DIR__"obj/ling");
	ob->set("ever_owner",getuid(me));
	me->set_temp("hs/jobji",1);
	command("nod "+me->query("id"));
	ob->move(me);
	ob->set_temp("hsjob", name1);
	me->apply_condition("hs_job", 30 + random(5));
	me->apply_condition("job_busy", 30);
	me->apply_condition("task_busy", 7);
	me->set_temp("hs_time", uptime());
	me->query_temp("cp_no_leave","你还有任务没有完成。\n");
	tell_object(me, "岳不群给了你一块令牌。\n");
	return "听说现在华山附近经常出现一些恶贼，骚扰百姓，你去巡视一下，遇到恶人不可手软。\n";
}

string ask_zhengqi()
{
	mapping fam;
	object me, ob;
	int shen, exp, i;
	me = this_player();
	ob = this_object();
	shen = (int)me->query("shen", 1);
	exp = (int)me->query("combat_exp", 1);
	i = (shen - exp);
	fam = me->query("family");

	if (!fam || fam["family_name"] !="华山派")
		return RANK_D->query_respect(me)+"与本派素无来往，不知此话从何谈起？\n";
	if( i < 1000 )
		return "你的侠义之气不够，如何叫我指点你的正气诀？\n";
	if( (int)me->query_skill("zhengqi-jue", 1) < 150 )
		return "你的正气诀未到火候，恐怕我无法指点你。\n";
	if( (int)me->query_skill("zhengqi-jue", 1) >= 200 )
		return "你的正气诀已到火候，恐怕我无法指点你。";
	if( (int)me->query("jing", 1) < 100 )
		return "你现在身体状况太差了，你无法领悟正气诀的绝妙之处。\n";
	if( (int)me->query("potential", 1) < 2 )
		return "你现在潜能不足，我无法指点你关于正气诀。\n";
	me->improve_skill("zhengqi-jue", 100 + random(100));
	me->add("shen", -1000);
	me->add("potential", -1);
	me->receive_damage("jing", random(100));
	tell_object(me, HIC"你感到心中激荡着侠义正气。\n"NOR);
	return  "好吧，既然你问起我正气诀，我就指点你一下。\n" ;
}

string ask_me2()
{
	return "你的令牌呢？";
}

int accept_object(object me, object ling)
{
	object name1;
	int exp,pot,getshen, total_job_times, time, done,i;
	name1 = me->query("id");

	if( ling->query("id") != "ling pai")
		return notify_fail("岳不群说道：惭愧呀，华山派居然出了你这样的骗子！\n");
	if( ling->query_temp("mmr") != name1 && !wizardp(me)){
		command("hmm "+ me->query("id"));
		me->add("shen", -(int)me->query("job_time/华山",1));
		me->delete_temp("hs/jobji");
		me->delete_temp("hs/have");
		me->delete_temp("hs/power");
		me->delete_temp("hs/robot");
		call_out("destroying", 1, ling);
		return notify_fail("岳不群说道：你怎么这么没用？没敢去找到恶贼？\n");
	}
	if( ling->query_temp("hsjob") != name1 && !wizardp(me)) {
		command("hmm "+ me->query("id"));
		me->add("shen", -(int)me->query("job_time/华山",1) * 5);
		me->delete_temp("hs/jobji");
		me->delete_temp("hs/have");
		me->delete_temp("hs/power");
		me->delete_temp("hs/robot");
		call_out("destroying", 1, ling);
		return notify_fail("岳不群说道：这是你的令牌么？\n");
	}
	done = (int)ling->query_temp("done", 1);
	if( done < 2 && !(done==1 && me->query_temp("hs/robot")) )
		return notify_fail("岳不群说道：你还没完成师傅的任务呢。\n");
	if ( !me->query_temp("hs/jobji") && !wizardp(me)){
		command("hmm "+ me->query("id"));
		me->add("shen", -(int)me->query("job_time/华山",1) * 5);
		me->delete_temp("hs/jobji");
		me->delete_temp("hs/have");
		me->delete_temp("hs/power");
		me->delete_temp("hs/robot");
		call_out("destroying", 1, ling);
		return notify_fail("岳不群说道：你的令牌从哪里偷来的？\n");
	}
	if ( !me->query_condition("hs_job") && !wizardp(me)){
		command("hmm "+ me->query("id"));
		me->add("shen", -(int)me->query("job_time/华山",1) * 5);
		me->delete_temp("hs/jobji");
		me->delete_temp("hs/have");
		me->delete_temp("hs/power");
		me->delete_temp("hs/robot");
		call_out("destroying", 1, ling);
		return notify_fail("岳不群说道：你怎么去了这么长时间？\n");
	}
	write("岳不群看了看你的令牌。\n", me);
	command("thumb "+ me->query("id"));
	command("say 还望" + RANK_D->query_respect(me) + "多加努力，日后必有重谢。");
	getshen = 100 + random((int)me->query("age",1)) + random((int)me->query("job_time/华山",1)/15);
	if( me->query("family") && me->query("family/family_name") =="桃花岛") getshen = 0;
	if (me->query("shen")<0) getshen=-getshen;
	me->add("shen", getshen );
		
	exp = 100+random(25);
	
	if (done==2)
		exp += 150+random(30);	
	if ( me->query_temp("hs/power"))
		exp += exp/5 + random(exp/5);	

	if (done==2) {
		exp = me->add_exp_combat(exp,this_object(),"华山",0,30);
		pot = 35 + random(10); 
	}
	else { 
		exp = me->add_exp_combat(exp,this_object(),"华山",0,-60);
		pot= 15 + random(10);
			//只做一轮job busy为1分钟
		i = me->query_condition("task_busy");
		if (i>4) me->apply_condition("task_busy", i-4);
		else me->clear_condition("task_busy");
	}
	
	
	me->delete_temp("hs_time");
	me->clear_condition("hs_job");
	me->clear_condition("job_busy");
	me->set("job_name","惩恶扬善");
	me->delete_temp("hs/jobji");
	me->delete_temp("hs/have");
	me->delete_temp("hs/power");
	me->delete_temp("hs/robot");
	call_out("destroying", 1, ling);
	
	if(!IN_MAIN){
		mapping tt = ([
			"cid" : "华山",
			"giver" : "岳不群"
		]);
		if(random(3)) TASK_D->event_reward(me,(["exp":exp*3/2,"pot":pot*2]),tt);
		else TASK_D->event_reward(me,(["exp":exp*3/2,"pot":pot*2,"jtp":1]),tt);
		me->delete_temp("hs/jobji");
		me->delete_temp("hs/have");
		me->delete_temp("hs/power");
		me->delete_temp("hs/robot");
		me->delete_temp("cp_no_leave");
		call_out("destroying", 1, ling);
		return 1;
	}
	
	if(( (int)me->query("potential", 1) + pot) > (int)me->query("max_pot", 1)){
		me->set("potential", (int)me->query("max_pot", 1));
	}
	if(( (int)me->query("potential", 1) + pot) <= (int)me->query("max_pot", 1)){
		me->add("potential", pot);
	}

	write("你获得了"+CHINESE_D->chinese_number(exp)+"点经验，"
		+CHINESE_D->chinese_number(pot)+"点潜能，"+CHINESE_D->chinese_number(getshen)+"点"+(getshen<0?"":"正")+"神。\n", me);

	total_job_times = me->query("job_time/华山") + me->query("job_time/送信") + me->query("job_time/丐帮");
	if (random(me->query("job_time/华山")) >400
		&& random(me->query("job_time/送信")) >400
		&&(random(me->query("job_time/丐帮")) >400 || random(me->query("combat_exp"))>2000000)
		&& !random(20)
		&& !me->query("quest/dgjj/fengset")
		&& !me->query("quest/dgjj/mb")
		&& (me->query("quest/dgjj/mbtimes") < me->query("combat_exp")/500000 + 1)
		&& me->query("family/family_name")=="华山派"
		&& total_job_times % 50 == 0 ){
		me->set("quest/dgjj/mb", 1);
		me->add("quest/dgjj/mbtimes", 1);
		log_file("quest/jiujian",sprintf("%8s%-10s第%4d次华山任务时，上思过崖面壁，送信：%4d，丐：%4d，次：%d。\n",
			me->name(1),"("+capitalize(me->query("id"))+")",me->query("job_time/华山"), me->query("job_time/送信"),
			me->query("job_time/丐帮"), me->query("quest/dgjj/mbtimes")),me);
		command("say "+me->query("name")+"你杀了不少恶人，未免杀气过重不如上思过崖面壁忏悔吧。\n");
	}

	return 1;
}

string ask_zixia()
{
	mapping fam;
	object me;
	me = this_player();
	fam = me->query("family");

	if (!fam || fam["family_name"] !="华山派")
		return RANK_D->query_respect(me)+"与本派素无来往，不知此话从何谈起？";
	if((int)me->query_skill("zhengqi-jue", 1) < 60 )
		return "你的正气诀未到火候，现在就来问紫霞秘籍，未免为时过早。";
	me->set_temp("hs/askzixia",1);
	return  "紫霞秘籍乃我华山至宝，前段时间不知道流落何处了。你去问问你二师兄吧。" ;
}

string ask_fail()
{
	object me,ling;
	me = this_player();
	ling = present("ling pai", me);
	if(! me->query_temp("hs/jobji"))
		return "你又没领过任务，何谓失败啊？";
	if( ling && ling->query_temp("hsjob") == me->query("id"))
		destruct(ling);
	me->delete_temp("hs/jobji");
	me->delete_temp("hs/have");
	me->delete_temp("hs/power");
	me->delete_temp("hs/robot");
	me->clear_condition("hs_job");
	me->clear_condition("task_busy");
	me->apply_condition("job_busy", 3+random(4));
	me->add("combat_exp", -(40+random(30)));
	me->delete_temp("cp_no_leave");
	return "既然如此，也不能勉强，你就先退下吧。";
}

void destroying(object ling)
{
	if (ling) destruct(ling);
}