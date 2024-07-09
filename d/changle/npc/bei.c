// bei.c 贝海石
// By River 2003.2.26
#include <ansi.h>
#include <job_mul.h>
#include <get_place.h>
inherit NPC;

string ask_job();
string ask_times();
string ask_finish();
string ask_fail();
int check(object ob);

void create()
{
	set_name("贝海石",({"bei haishi", "bei", "haishi"}));
	set("title", "长乐帮总管");
	set("nickname",HIY"着手回春"NOR);
	set("long", "他就是长乐帮总管，人称着手回春的贝海石。\n"+
		"他是一个脸有病容的老者。\n");
	set("age", 65);
	set("attitude", "friendly");
	set("shen", 0);
	set("str", 26);
	set("int", 29);
	set("con", 27);
	set("dex", 30);
	set("no_get", 1);
	set("no_bark", 1);
	set("job_npc", 1);
	set("no_ansuan", 1);

	set("max_qi", 4500);
	set("max_jing", 3200);
	set("eff_jingli", 3500);
	set("jiali", 80);
	set("combat_exp", 2500000); 
	set("unique", 1);

	set_skill("dodge",220);
	set_skill("force", 220);
	set_skill("literate", 200);
	set_skill("strike", 220);
	set_skill("kunlun-shenfa", 220);
	set_skill("banruo-zhang", 220);
	set_skill("shenzhao-jing", 220);
	set_skill("parry", 220);
	set_skill("medicine", 160);

	map_skill("force", "shenzhao-jing");
	map_skill("dodge", "kunlun-shenfa");
	map_skill("parry", "banruo-zhang");
	map_skill("strike", "banruo-zhang");
	prepare_skill("strike", "banruo-zhang");

	set("inquiry", ([
		"任务"	:	(: ask_job :),
		"job"	:	(: ask_job :),
		"完成"	:	(: ask_finish :),
		"finish":	(: ask_finish :),
		"功劳"	:	(: ask_times :),
		"放弃"	:	(: ask_fail :),
		"fangqi":	(: ask_fail :),
		"帮主"	:	"帮主已经失踪多日，我们正在到处寻找中。",
		"侠客岛":	"这个要等帮主回来后，才能商议。",
		"长乐帮":	"长乐帮分为龙灵堂、狮威堂、熊敏堂、虎猛堂和豹捷堂五堂。",
	]));

	setup();
	carry_object(ARMOR_D("changshan"))->wear();
}

void heart_beat()
{
	::heart_beat();

	clear_conditions_by_type("poison");
	if( !is_fighting() && living(this_object()))
		reincarnate();
}

string *party = ({"龙灵堂","狮威堂","熊敏堂","虎猛堂","豹捷堂"});

string ask_job()
{
	object me, ob, *living, target;
	string str, str2, address;
	object objwhere;
	int i,j;

	me = this_player();
	str2 = party[random(sizeof(party))];

	if (me->query_temp("changle/changle_job") && !me->query_temp("changle/job_place"))
		return "啊……，对不起，地点有误，你放弃重新再来吧。";

	if (me->query_temp("changle/job_place1"))
		return "你不是已经知道了，杀害我帮帮众的仇人在"+me->query_temp("changle/job_place1")+CYN"一带出现过。";

	if ( me->query_temp("changle/done"))
		return "你不是已经进展到一定地步了，还是继续努力吧！";

	if (me->query_temp("changle/changle_job"))
		return "我不是告诉你了吗，我帮有人在"+me->query_temp("changle/job_place")+CYN"遇到袭击，你还不赶快前去救援！";

	if (me->query("job_name") =="长乐帮" && !wizardp(me))
		return "你刚做完长乐帮任务，还是先去休息一会吧。";

	if (me->query("combat_exp") < 100000 )
		return "这位" + RANK_D->query_respect(me) + "武功未成，我帮还不需要你的帮助。";

	if ( me->query_condition("job_busy"))
		return "我帮现在比较空闲，暂时还没有任务让你去做。";
	
	if(!wizardp(me) && me->query_condition("task_busy"))
		return "我帮现在比较空闲，暂时还没有任务让你去做。";
		
	if(me->query_temp("quest/busy")) //added by tangfeng 与quest冲突
		return "现在我这里没有给你的任务，你还是先处理好你其他事情再说吧。";

	living = livings();
	for (i=0;i < sizeof(living);i++){
		j = random(sizeof(living));
		if (!j || j == sizeof(living) -1) continue;
		if(check(living[j]) ){
			target = living[j];
			objwhere = environment(target);
			str = objwhere->query("short");
			if (!sizeof(objwhere->query("exits"))
			|| str == "武僧堂"
			|| str == "僧舍"
			|| str == HIR"心禅堂"NOR
			|| str == "金刚伏魔圈" ) continue;
			if( get_place(base_name(environment(target))) != "" ) break;
		}
	}

	if( !target )
		return "暂时没有任务需要做，你过一会儿再来吧。";

	ob = new(__DIR__"zhong");
	ob->set_temp("target", getuid(me));
	ob->set("title", HIR"长乐帮"+str2+"属下帮众"NOR);
	ob->move(objwhere);
	ob->set("env/invisibilty", 10);

	for ( j = 0; j < 5; j ++)
		ob->random_move();

	if ( environment(ob)->query("no_fight") || environment(ob)->query("quest") )
		ob->random_move();

	ob->delete("env/invisibilty");

	if (strsrch(file_name(environment(ob)), "/d/zhiye/") >= 0
	 || strsrch(file_name(environment(ob)), "/d/group/") >= 0) {
	 	if ( environment(ob)->query("outdoors"))
			me->set_temp("change/job_place", environment(ob)->query("outdoors")+environment(ob)->query("short"));
	 	else if ( environment(ob)->query("indoors"))
			me->set_temp("change/job_place", environment(ob)->query("indoors")+environment(ob)->query("short"));
		else me->set_temp("changle/job_place", environment(ob)->query("short"));
	}
	else me->set_temp("changle/job_place", get_place(base_name(environment(ob)))+environment(ob)->query("short"));

	if ( ! me->query_temp("changle/job_place"))
		return "暂时没有任务需要做，你就过一会儿再来吧。";

	address = strip(me->query_temp("changle/job_place"));
	me->set_temp("changle/changle_job",1);
	me->set_temp("changle/times", uptime());
	me->apply_condition("job_busy", 40 );
	me->apply_condition("task_busy", 8);
	me->set_temp("changle/npc_name", ob->name(1));
	command("bow "+ me->query("id"));
	command("whisper "+ me->query("id")+" 我接到飞鸽传书，"+str2+"属下帮众"+ob->query("name")+"在"+ address +"处遇到袭击，你赶快前去救援！");
	return "这位"+RANK_D->query_respect(me)+"肯为我帮出力，如能完成使命，必有重赏。";
}

string ask_finish()
{
	object ob, me = this_player();
	int pot, exp, time, i, gold,j;
	string msg;

	if (!me->query_temp("changle"))
		return "你根本就没有领任务，完成什么啊？";

	if ( !me->query_temp("changle/done"))
		return "你真的完成任务了么？";

	msg = me->query_temp("changle/target_family");
	j = me->query_condition("task_busy");
	switch(me->query_temp("changle/done")){
		case 1:
			exp = 1 + random(5);
			msg = "既然已经知道了是"+msg+"所为，哼，这个梁子算结上了。";
			me->add("job_time/长乐帮", -1);
			if (j>6) me->apply_condition("task_busy", j-6);
			else me->clear_condition("task_busy");
			break;
		case 2:
			exp = 120 + random(10);
			msg = "哎，可惜不能手刃此奸贼，但"+msg+"和我帮的梁子算结上了。";
			me->add("job_time/长乐帮", -1);
			if (j>4) me->apply_condition("task_busy", j-4);
			else me->clear_condition("task_busy");
			break;
		case 3:
			exp = 320 + random(100);
			time = uptime()- me->query_temp("changle/times");
			i = time * 85 / 36;
			if (exp > i)
				exp = i + random(exp-i);
			if ( exp < 250)
				exp = 200 +random(50);
			gold = 1 +random(3);
			MONEY_D->pay_player(me, gold * 10000);
			msg = "做的好！这些金子"+RANK_D->query_respect(me)+"先拿去零花，下次还需要您多为我帮出力。";			
			break;
		default:
			msg = "啊？出问题了，请报告开发人员检查。";
			me->add("job_time/长乐帮", -1);
			break;
	}
		
	exp = me->add_exp_combat(exp,this_object(),"长乐帮");
	if(me->query_temp("changle/done")==1)
		pot = 50 + random(30);
	else
		pot = exp/5 + random(exp/10);
	me->add("potential", pot);
	
	if ( me->query("potential", 1) > me->query("max_pot"))
		me->set("potential" , me->query("max_pot"));

	log_file("job/changle",
		sprintf("%8s%-10s完成第%5d次长乐帮任务(%d)，%d|%d|%d。\n",
			me->name(),
			"("+getuid(me)+")",
			me->query("job_time/长乐帮"),
			me->query_temp("changle/done"),
			exp,
			i,
			me->query("combat_exp")
		), me
	);

	me->delete_temp("changle");
	me->clear_condition("job_busy");
	me->set("job_name", "长乐帮");
	if ( objectp(ob= present("cu bu", me)))
		destruct(ob);

	tell_object(me,HIW"您被奖励了"+chinese_number(exp)+"点经验，"+chinese_number(pot)+"点潜能，您已经为长乐帮出力"+chinese_number(me->query("job_time/长乐帮"))+"次。\n"NOR);
	return msg;
}

string ask_fail()
{
	object ob, me = this_player();
	int i;

	if (!me->query_temp("changle"))
		return "你根本就没有领任务，完成什么啊？";

	i = 40 - me->query_condition("job_busy");
	if ( i > 8 ) i = 8;
		me->apply_condition("job_busy", 9-i );

	me->clear_condition("task_busy");
	if ( objectp(ob= present("cu bu", me)))
		destruct(ob);

	command("disapp "+ me->query("id"));
	me->delete_temp("changle");
	me->add("combat_exp", -(30+random(10)));
	return "既然你无法完成，那也只能如此了。";
}

string ask_times()
{
	object me = this_player();

	if ( !me->query("job_time/长乐帮"))
		return "这位"+RANK_D->query_respect(me)+"好像还未为本帮出力过。";

	command("addoil "+ me->query("id"));
	return "这位"+RANK_D->query_respect(me)+"，您已经为本帮出力"+chinese_number(me->query("job_time/长乐帮"))+"次了。";
}

int check(object ob)
{
	object room;
	string room_name;

	if(!objectp(ob)
	 || !ob->query("startroom")      // add by Yuj@SJ to void job NPC
	 || ob->query("env/invisibility")
	 || ob->query("no_quest")
	 || userp(ob)
	 || ob->query("attitude")=="aggressive"
	 || ob->query("have_master")
	 || !objectp(room=environment(ob))
	 || room->query("no_fight")
	 || room->query("outdoors") == "昆仑翠谷"
	 || strsrch(room_name = file_name(room),"/d/") != 0
	 || strsrch(room_name, "/d/wizard/") == 0
	 || strsrch(room_name, "/d/wuguan/") == 0
	 || strsrch(room_name, "/d/death/") == 0
	 || strsrch(room_name, "/d/group/") == 0
	 || strsrch(room_name, "/d/mingjiao/lsd/") == 0
	 || strsrch(room_name, "/d/mingjiao/bhd/") == 0
	 || strsrch(room_name, "/d/changle/") == 0
	 || strsrch(room_name, "/d/city/") == 0
	 || strsrch(room_name, "/d/shaolin/fumoquan") == 0
	 || strsrch(room_name, "/d/baituo/") == 0)
		return 0;
	return 1;
}

void unconcious()
{
	::die();
}
