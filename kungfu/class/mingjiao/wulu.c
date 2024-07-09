// wufu.c 殷无禄
// Modify By River@sj 99.06
#include <ansi.h>
inherit NPC;
inherit F_MASTER;
string ask_me();
string ask_me1();
string ask_me2();
int do_give(object ob, object me, object who);
void destroy(object ob);
void create()
{
	set_name("殷无禄", ({ "yin wulu", "yin", "wulu" }));
	set("title","明教天鹰分舵属下");
	set("age", 45);
	set("long","他是殷家的朴人。\n");
	set("str", 23);
	set("int", 20);
	set("con", 23);
	set("dex", 23);
	set("per", 10);
	set("unique", 1);
	set("no_get", "殷无禄对你来说太重了。\n");
	set("combat_exp", 350000);
	set("attitude", "friendly");
	set_skill("dodge",90);
	set_skill("force", 90);
	set_skill("piaoyi-shenfa", 90);
	set_skill("shenghuo-shengong", 90);
	set_skill("yingzhua-shou",90);
	set_skill("hand",90);
	set_skill("parry", 90);
	map_skill("force", "shenghuo-shengong");
	map_skill("dodge", "piaoyi-shenfa");
	map_skill("hand", "yingzhua-shou");
	map_skill("parry", "yingzhua-shou");
	prepare_skill("hand","yingzhua-shou");
	create_family("明教",38,"弟子");
	set("job_npc", 1);
	set("chat_chance", 15);
	set("chat_msg", ({
		(: random_move :)
	}) );
	set("inquiry", ([
		 "巡逻" : (: ask_me :),
		 "完成" : (: ask_me1 :),
		 "放弃" : (: ask_me2 :),
	]));
	setup();
	carry_object("/d/mingjiao/obj/green-cloth")->wear();
}

void heart_beat()
{
	::heart_beat();

	clear_conditions_by_type("poison");
	if( !is_fighting() && living(this_object()))
		reincarnate();
}

void attempt_apprentice(object ob)
{

	if ( ob->query("int") < 20 ){
		command("shake");
		command("say 要能达到炉火纯青之境，体质什么的倒是无关紧要，悟性却是半点也马虎不得。");
		return;
	}
	command("look " + ob->query("id"));
	command("say 嗯，看你还是个学武的料，我就收下你吧！");
	command("say 多谢明尊敝佑，让我明教又得一良材，为康扶正义，把我明教发扬光大吧。");
	command("recruit " + ob->query("id"));
	ob->set("title","明教天微堂教众");
}

string ask_me()
{
	mapping fam;
	object ob, me, obj;
	object* dest;
	int i, j;

	j = 0;
	me = this_player();
	ob = this_object();

	if(!(fam = me->query("family")) || fam["family_name"] != "明教")
		return RANK_D->query_respect(me)+"与本派素无来往，不知此话从何谈起？";

	if(me->query("combat_exp") < 20000)
		return me->name()+"你武功低微，我怎么放心让你来巡逻呢？\n";

	if( me->query_condition("job_busy"))
		return "大胆!你竟然敢同时做别的任务！";

//	if(me->query("mingjiao_job") > 2000)
//		return "你为明教做出了很多贡献，巡逻了这么多次，最近还是歇歇吧。";

	if(me->query_temp("mj/xunluo"))
		return "你不是在巡逻吗，怎么还呆在这里？";

	if(me->query_temp("mj/over"))
		return "你上次巡逻结束还没交差呢。";

	if(me->query("job_name") =="明教巡逻" && me->query("combat_exp") > 100000)
		return "你上次明教巡逻任务刚做完，还是先休息一下吧。";

	if(me->query("jiuyang/read") && me->query("combat_exp") > 2000000)
		return "你还是为明教干些别的事吧，巡逻的事情就让别的明教弟子去干吧";

	dest = users();
	i = sizeof(dest);
	while(i--) {
		if (userp(dest[i]) && !wizardp(dest[i]) && dest[i]->query_temp("mj/xunluo")
		&& dest[i]->query_condition("mj_xunluo"))
			j++;
	}
	if(j > 7)
		return "嗯，已经有足够的人在帮我巡逻了，你还是去忙点别的什么吧。";

	if(me->query_condition("mj_xunluo")){
		if(!me->query_temp("mj/fail"))
			return "嗯，你刚刚巡逻完毕，还是先去休息休息吧。";
		else
			return "你刚刚放弃了，还不去休息一会，待会再来吧。";
	}
	me->set_temp("mj/xunluo", 1);
	message_vision(CYN"\n$N自言自语道：现在共有"+chinese_number(j)+"名明尊弟子在光明顶巡逻。\n"NOR, ob);
	message_vision(CYN"\n$N对"+me->name()+"哈哈一笑，说道：既然这样，你就在光明顶和小沙丘之间好好巡逻。\n"NOR, ob, me);
	me->apply_condition("mj_xunluo", 200);
	me->apply_condition("job_busy", 9);
	me->set("job_name","明教巡逻");
	me->delete_temp("mj/fail");
	log_file("job/xunshan",sprintf("%8s%-10s开始做明教巡山任务，现在经验是%d。\n",
		me->name(1),"("+me->query("id")+")",me->query("combat_exp")), me);

	obj = new("/d/mingjiao/obj/lingpai");
	obj->set("owner", me->query("id"));
	obj->move(me);
	return "记着要请五位旗使，殷少主，张散人和我兄弟殷无福给你画印(ask about 画印)。\n"+
		"巡逻完成以后，回来告诉我(ask yin wulu about 完成)。\n";
}

string ask_me1()
{
	mapping fam;
	object ob, me;
	me = this_player();
	ob = this_object();

	if(!(fam = me->query("family")) || fam["family_name"] != "明教")
		return RANK_D->query_respect(me)+"你在说些什么啊？";
	if(!me->query_condition("mj_xunluo")){
		me->delete_temp("mj/xunluo");
		return "唉，你怎么久才巡逻完，真是没用！";
	}
	if(!me->query_temp("mj/xunluo"))
		return "完成？完成什么？";
	if(me->query_temp("mj/xunluo") < 8)
		return "你还没有巡逻完吧？怎么就回来了？";
	command("nod "+me->query("id"));
	me->delete_temp("mj/xunluo");
	me->set_temp("mj_task1", 1);
	me->set_temp("mj/over", 1);
	me->delete_temp("mj_task1");
	if(me->query_condition("mj_xunluo") > 10)
	  me->apply_condition("mj_xunluo", 10);
	message_vision("\n$N在巡逻令上写了写东西。\n",ob);
	return " 好！"+me->name()+"你完成得很好，快把这令牌拿去交给韦法王吧。";
}

string ask_me2()
{
	object me = this_player();
	mapping fam;

	if(!(fam = me->query("family")) || fam["family_name"] != "明教")
	    return RANK_D->query_respect(me)+"你在说些什么啊？";

	if(! me->query_temp("mj/xunluo")
	 && ! me->query_condition("mj_xunluo"))
	    return "放弃？你根本就没有做，放弃什么啊？";

	if(me->query_temp("mj/fail"))
	    return "你刚刚放弃，现在又要放弃什么啊？";

	command("disapp "+ me->query("id"));
	me->delete_temp("mj/xunluo");
	me->apply_condition("mj_xunluo", 6);
	me->set_temp("mj/fail", 1);
	    return "你就先休息一会，待会再来试试巡逻吧。";
}

int accept_object(object who, object ob,object me)
{
	me=this_object();
	
	command("say 你拿这东西给我是什么意思？");
	command("doubt "+me->query("id"));
	return 0;
	
}

int do_give(object ob, object me, object who)
{
       message_vision("\n$N在巡逻令上写了写东西。\n",me);
       if(!objectp(present(who, environment(me)))){
	 message_vision("$N说道：耶？那家伙怎么不见了？\n", me);
	 command("drop "+ob->query("id"));
	 me->delete("ling");
	 return 1;
       }
       if(!living(who)){
	 command("sigh "+who->query("id"));
	 ob->move(who);
	 me->delete("ling");
	 message_vision("$N将$n放进"+who->name()+"衣服里。\n", me, ob);
	 return 1;
       }
       else {
	 me->delete("ling");
	 command("give "+ob->query("id")+" to "+who->query("id"));
	 return 1;
       }
}

void destroy(object ob)
{
	destruct(ob);
	return;
}
