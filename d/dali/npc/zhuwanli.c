//by cool 98.3.25
// 不能和wd,xs job一起做。River@SJ 2000.7.19

inherit NPC;
#include <ansi.h>
#include <get_place.h>
#include <mudlib.h>

int check(object ob);
string ask_job();
string ask_fangqi();

void create()
{
	set_name("褚万里", ({ "zhu wanli","zhu","wanli" }));
	set("title", "大理国镇南王府家将");
	set("gender", "男性");
	set("age", 33);
	set("str", 27);
	set("dex", 26);
	set("long", "这人身穿军服，腰上插着一根软索，英气逼人。\n");
	set("combat_exp", 400000);
	set("shen_type", 1);
	set("attitude", "peaceful");
	set("no_bark",1);
	set_skill("unarmed", 100);
	set_skill("force", 100);
	set_skill("whip", 100);
	set_skill("dodge", 100);
	set_skill("parry", 100);
	set_temp("apply/attack", 70);
	set_temp("apply/defense", 70);
	set_temp("apply/armor", 70);
	set_temp("apply/damage", 20);

	set("max_qi", 900);
	set("neili", 900);
	set("max_neili", 900);
	set("jiali", 20);
	set("inquiry", ([
		"送信" : (: ask_job :),
		"job" : (: ask_job :),
		"放弃" : (: ask_fangqi :),
		"fangqi" : (: ask_fangqi :),
	]));

	setup();
	carry_object(BINGQI_D("ruansuo"))->wield();
	carry_object(ARMOR_D("junfu"))->wear();
}

string ask_fangqi()
{
	object ob = this_player();
	object obj;
	int i;

	if (!ob->query_temp("dali_songxin"))
		return RANK_D->query_respect(ob) +"你并没有领了送信的任务，说什么放弃啊？";

	i = ob->query_temp("songxin/time") - ob->query_condition("dali_songxin");
	if ( i > 8 )	i = 8;
	ob->apply_condition("job_busy", 9-i);
	ob->delete_temp("dali_songxin");
	ob->clear_condition("dali_songxin");
	ob->delete_temp("letter_steal");
	ob->delete_temp("zuji");
	ob->delete_temp("songxin_zuji");
	ob->delete_temp("songxin");
	ob->clear_condition("job_busy");
	ob->clear_condition("dali_songxin");

	if(objectp(obj = present("secret letter", ob)))
		destruct(obj);

	ob->add("combat_exp", -(30+random(30)));		
  ob->add("job_time/送信",-1);
	return "既然"+ RANK_D->query_respect(ob) +"做不了，也就罢了。";
}

string ask_job()
{
	object me, *living, target, ob, obj;
	int time,i,j;
	string str;

	ob=this_player();
	me=this_object();

	if (!wizardp(ob) && !ob->query_temp("dali_join"))
		return RANK_D->query_respect(ob) +"，你不是本王府随从，此话从何说起？";

	if (ob->query_temp("dali_songxin"))
		return "你不是已经领了送信的任务吗？还不快去做。";

	if (ob->query("job_name") == "大理送信" && !wizardp(ob))
		return "你刚做完大理送信任务，还是去休息一会吧。";

	if (ob->query_condition("dali_songxin") > 0)
		return "现在暂时没有给你的任务，再等"+chinese_number((ob->query_condition("dali_songxin")+5)/6)+"个时辰吧。";

	if( ob->query_condition("job_busy"))
		return "你先去休息一会吧！";

	if (ob->query("combat_exp") < 100000)
		     return ("你的功夫还不够啊，送信可是很危险的事，我可不敢交给你这任务。");

	if(ob->query_temp("quest/busy")) //added by tangfeng 与quest冲突
		return "现在我这里没有给你的任务，你还是先处理好你其他事情再说吧。";


	living = livings();
	for (i=0;i < sizeof(living);i++){
		j = random(sizeof(living));
		if (j <= 1 || j == sizeof(living) -1) continue;
		if(check(living[j]) ){
			target = living[j];
			str = environment(target)->query("short");
			if( str == "武僧堂"
			 || str == HIR"心禅堂"NOR
			 || str =="地下黑拳市"
			 || str == "金刚伏魔圈" ) continue;
			if( get_place(base_name(environment(target)))!="" ) break;
		}
	}

	if(wizardp(ob)) target = find_living("xuannan dashi");
	
	if( !target )
		return "暂时没有信需要送，你过一会儿再来吧。";
	
	time = random(10)+ 25;
	ob->delete_temp("songxin_ok");
	ob->delete_temp("dali_songxin_late");
	ob->delete_temp("songxin_zuji");
	ob->set_temp("dali_songxin", 1);
	ob->set_temp("songxin/time", time);
	ob->set_temp("songxin/times", uptime());
	ob->apply_condition("job_busy", time);

	if ( random(3)) {
		ob->apply_condition("dali_songxin", time);
		ob->set_temp("songxin/target_name", target->query("name"));
		ob->set_temp("songxin/area", load_object(target->query("startroom"))->query("short"));
		ob->set_temp("songxin/areab", target->query("startroom"));
		ob->set_temp("songxin/id", target->query("id"));
		ob->set_temp("songxin/where", get_place(ob->query_temp("songxin/areab"))+ob->query_temp("songxin/area"));
		if ( random(3) ) ob->set_temp("songxin/other", 1);
	}
	else {
		ob->apply_condition("dali_songxin", time*9/10+2);
		obj = new("/d/dali/job/sxtarget");
		obj->set("startroom", target->query("startroom"));
		obj->move(environment(target));
		ob->set_temp("songxin/other", 1);
		ob->set_temp("songxin/target_name", obj->query("name"));
		ob->set_temp("songxin/area", load_object(target->query("startroom"))->query("short"));
		ob->set_temp("songxin/areab", target->query("startroom"));
		ob->set_temp("songxin/id", obj->query("id"));
		ob->set_temp("songxin/where", get_place(ob->query_temp("songxin/areab"))+ob->query_temp("songxin/area"));
	}

	switch(random(3)) {
		case 0:{
			command("nod "+ ob->query("id"));
			command("tell "+ ob->query("id")+" 你把这封密函火速送到「"+ob->query_temp("songxin/where")+GRN"」的「"+ob->query_temp("songxin/target_name")+GRN"」手上。");
			break;
		}
		case 1:{
			command("smile "+ ob->query("id"));
			command("tell "+ ob->query("id")+" 我这里正好有封信，你去把它送到「"+ob->query_temp("songxin/where")+GRN"」的「"+ob->query_temp("songxin/target_name")+GRN"」手上。");
			break;
		}
		case 2:{
			command("consider");
			command("tell "+ ob->query("id")+" 你赶紧把它送到「"+ob->query_temp("songxin/where")+GRN"」的「"+ob->query_temp("songxin/target_name")+GRN"」手上。");
			break;
		}
	}
	message_vision("$N交给$n一封密函。\n",me,ob);
	ob=new("/d/dali/job/dali_letter");
	ob->set("ever_owner",getuid(this_player()));
	ob->move(this_player());
	return "路上常有强人出没，务必小心。";
}

string *no_kill_list = ({
	 "mu ren", "jin ren", "yin ren", "zhong shentong", "zhu cong", "shi ren", "huo du","he shiwo"
});

int check(object ob)
{
	object room;
	string room_name, start_room;
//	|| ob->query("chat_chance")

	if (!objectp(ob)
	|| userp(ob)
	|| ob->query("env/invisibility")
	|| ob->query("no_quest")
	|| member_array(ob->query("id"), no_kill_list) != -1
	|| ob->query("race") !="人类"
	|| ob->query("attitude")=="aggressive"
	|| !ob->query("can_speak")
	|| ob->query("have_master")
	|| ob->query("combat_exp") < 20000
	|| !(start_room = ob->query("startroom"))	// add by Yuj@SJ to void job NPC
	|| !objectp(room=environment(ob))
	|| room->query("no_fight")
	|| room->query("quest")
	|| strsrch(room_name = base_name(room), "/d/") != 0
	|| ob->query("lastroom") == room_name && start_room != room_name
	|| start_room[0..5] != room_name[0..5]
	|| strsrch(room_name, "/d/wizard/") == 0
	|| strsrch(room_name, "/d/wuguan/") == 0
	|| strsrch(room_name, "/d/death/") == 0
	|| strsrch(room_name, "/d/mingjiao/bhd/") == 0
	|| strsrch(room_name, "/d/mingjiao/lsd/") == 0
	|| strsrch(room_name, "/d/dali/") == 0
	|| strsrch(room_name, "/d/tls/") == 0
	|| strsrch(room_name, "/d/shaolin/fumoquan") == 0
	|| strsrch(room_name, "/d/group/") == 0
	|| strsrch(room_name, "/d/baituo/") == 0
	|| strsrch(room_name, "/d/tianshan/") == 0
	|| strsrch(room_name, "/d/thd/") == 0 )
		return 0;
	return 1;
}
