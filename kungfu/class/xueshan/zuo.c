// zuo.c 左冷禅
// Update by looklove at 2000/8/25
// Modify by Looklove for add log_file at 2000/9/1
// Looklove@SJ 2000/10/22 fixed fangqi's bug
// Modified by darken@SJ
// Lklv 2001.10.19 modified

#include <ansi.h>
#include "zuo.h"

inherit NPC;

void create()
{
	object ob;

	set_name("左冷禅", ({ "zuo lengchan", "zuo", "lengchan" }));
	set("nickname", HIR"五岳盟主"NOR);
	set("long", "嵩山掌门左冷禅更是当今武林中了不起的人物，武功固然出神入化，
		为人尤富机智，机变百出，江湖上一提到“左盟主”三字，无不惕然。\n");
	set("gender", "男性");
	set("age", 52);
	set("per", 20);
	set("attitude", "heroism");
	set("class", "songshan");
	set("shen_type", -1);
	set("str", 29);
	set("int", 28);
	set("con", 28);
	set("dex", 28);
	set("unique", 1);
	set("no_bark",1);

	set("max_qi", 9500);
	set("max_jing", 5800);
	set("eff_jingli", 8000);
	set("jiali", 100);
	set("combat_exp", 15000000);
	set("shen", -18000);
	set("max_neili", 16000);
	set("neili", 16000);

	set_skill("literate", 220);
	set_skill("force", 500);
	set_skill("dodge", 500);
	set_skill("hand", 500);
	set_skill("strike", 500);
	set_skill("parry", 500);
	set_skill("sword", 500);
	set_skill("songshan-jian", 500);
	set_skill("songyang-shou", 500);
	set_skill("zhongyuefeng", 500);
	set_skill("hanbing-shenzhang", 500);
	set_skill("hanbing-zhenqi", 500);

	map_skill("force", "hanbing-zhenqi");
	map_skill("dodge", "zhongyuefeng");
	map_skill("hand", "songyang-shou");
	map_skill("parry", "songshan-jian");
	map_skill("sword", "songshan-jian");
	map_skill("strike", "hanbing-shenzhang");
	prepare_skill("strike", "hanbing-shenzhang");
	set("inquiry", ([
		"job": (: do_ask() :),
		"任务": (: do_ask() :),
		"cancel": (: do_cancel() :),
		"放弃": (: do_cancel() :),
		"这个": (: do_ask_hb() :),
		"次数": (:do_ask_times :),
		"功劳": (:do_ask_times :),
		"观日剑": (:do_ask_grj() :),
	]));
	create_family("嵩山派", 3, "掌门人");
	set("chat_chance_combat", 20);
	set("chat_msg_combat", ({
		(: perform_action, "sword.feiwu" :),
		(: perform_action, "hand.hanbing" :),
		(: exert_function, "juhan" :),
		(: exert_function, "hanbing" :),
	}));
	setup();
	if (clonep()) {
		ob = unew(BINGQI_D("sword/guanri-jian"));
		if (!ob) ob = unew(BINGQI_D("gangjian"));
		ob->move(this_object());
		ob->wield();
		carry_object(ARMOR_D("changpao1"))->wear();
		carry_object(MEDICINE_D("tianwang"));
	}
	add_money("gold", 1);
}

void attempt_apprentice(object ob)
{
	if(ob->query("shen") > 0){
		command("say 我最瞧不起岳不群那样的伪君子，还是把你那点假道学收敛一下吧。");
		return;
	}
	if(ob->query_skill("hanbing-zhenqi",1) < 150 ){
		command("say 你的武功还不够娴熟，再回去练练吧。");
		return;
	}
	if(ob->query("job_time/嵩山") < 300) {
		command("say 你自从入我派以来，偷懒耍滑，不务正业，还想拜我为师？");
		return;
	}
	command("say 好，我正想找些优秀弟子来亲自传授武功，看来你还不错嘛。");
	command("recruit " + ob->query("id"));
	ob->set("title", HIC"五岳盟主亲传弟子"NOR);
}

void init()
{
	object ob;
	ob = this_player();

	::init();
	if( interactive(ob = this_player()) && !is_fighting() && !is_busy()
	   && ob->query_temp("quest/family") == "嵩山派"
	   && ob->query_temp("quest/type") == "请") {
		remove_call_out("greeting");
		call_out("greeting", 2, ob);
	}
}

void greeting(object me)
{
	object target, ob;
	mapping quest;

	if( !me || environment(me) != environment() || !living(me)) return;

	if (!mapp(quest = me->query_temp("quest")) || quest["type"] != "请"
	|| !stringp(quest["id"])) return;

	command("nod "+me->query("id"));

	ob = this_object();

	if(objectp(target=present(quest["id"], environment(me)))
	  && target->query_temp("songshan_job") == me
	  && living(target)){
		me->add_busy(2);
		target->set_leader(0);
		command("haha");
		command("touch "+target->query("id"));
		command("say "+target->name()+"能来，我嵩山派又多了一份力量啦！哈哈！");
		call_out("done", 3, target, me, quest, 1);
		return;
	}
}

void done(object target, object me, mapping quest, int i)
{
       int count,exp,pot;

       if(me && i >= 0){
		if(target){
			message_vision("\n$N点头道："+me->name()+"这娃娃不赖，让我指点他几招，就当是迟来的见面礼吧！\n", target, me);
			tell_room( environment(me), target->name() + "在" + me->name()+ "耳边小声地说了些话。\n", ({ me, target }) );
			// give rewards.
			TASK_D->give_reward(me, me->query_temp("quest"), -1, ABS(me->query("job_time/嵩山")/100) , this_object(), "嵩山" );
			//me->add("job_time/嵩山", 1);
			//GIFT_D->check_count(me,this_object(),"嵩山");
			
			if ( me->query("family/family_name") == "嵩山派"
			&& me->query_skill("hanbing-zhenqi"))
				me->improve_skill("hanbing-zhenqi",me->query("int")*2);
							
			count = me->query("job_time/嵩山");
			if (count<1) count=1;
			pot= me->query_temp("quest/pot_reward");
			exp= me->query_temp("quest/exp_reward");
			log_file("job/songshan",sprintf("%8s%-10s第%4d次嵩山请人任务获得%3d点经验，%2d点潜能，经验：%d\n",
				me->name(),"("+me->query("id")+")",count,exp,pot,me->query("combat_exp")),me);
			me->delete_temp("quest");
			me->delete_temp("songshan_job");
			me->clear_condition("job_busy");
			me->set("job_name", "嵩山并派");
			tell_room(environment(target), target->query("name")+"大笑两声，在知客弟子带领下到厢房休息去了。\n", ({target}));
			if(target->return_home()){
				target->delete_temp("songshan_job");
				target->delete_temp("last_lost_to");
				target->clear_condition("songshan_busy");
				target->set("chat_chance", target->query("chat_c"));
				if(!target->query_temp("location"))
				target->delete("location");
				target->set_leader(0);
				target->reincarnate();
			}
			else destruct(target);
		}
	}
	else if(target){
		tell_room(environment(target), target->query("name")+"大笑两声，在知客弟子带领下到厢房休息去了。\n", ({target}));
		destruct(target);
	}
}
