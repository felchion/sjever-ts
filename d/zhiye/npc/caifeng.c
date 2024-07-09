// caifeng
// by augx@sj  10/9/2001

// Modified by mxzhao 2004/03/06 caijian => zhizao

#include <ansi.h>
#include <job_mul.h>
inherit NPC;
inherit F_VENDOR;
#include "/d/zhiye/npc/forbook.c"

#define RESET_TIME	15

string ask_job();
string ask_fail();
string ask_reward();
string ask_info();
string ask_list();

void create()
{
	set_name("老裁缝",({ "lao caifeng","caifeng" }) );
	set("gender", "男性" );
	set("age", 65);
	set("long",
"这是位带着波斯老花眼镜的老裁缝，他的手艺无人不知，达官贵人们莫不
以拥有他亲手织造的一件衣服为荣。不过现在他年岁大了，自己不常动手，
主要精力都放到做生意和教授学生上来了。");

	set("str", 35);
	set("dex", 55);
	set("con", 25);
	set("int", 35);
	set("shen_type", 1);
	set("move_seller", 1);
	set("shen", 100);
	set("attitude", "peaceful");
	set("combat_exp", 1000000);
	set_max_encumbrance(10000000);

	set_skill("dodge", 300);
	set_skill("force", 300);
	set_skill("parry", 300);
	set_skill("unarmed", 300);
	set_skill("literate", 150);
	set_skill("zhizao", 300);

	set("max_qi", 99999);
	set("max_neili", 99999);
	set("max_jing", 99999);
	set("max_jingli", 99999);
	set("eff_jingli", 99999);

	set_temp("apply/dodge",888);
	set_temp("apply/parry",888);
	set_temp("apply/force",888);
	set_temp("apply/attack",888);
	set_temp("apply/damage",333);
	set_temp("apply/armor",888);

	set("chat_chance", 1);
 	set("chat_msg", ({
		CYN"老裁缝沾沾自喜的说道：「最近可是赚了不少钱。」\n"NOR,
 		CYN"老裁缝说道：「我现在不大自己动手织造衣服了。」\n"NOR,
 		CYN"老裁缝欣慰的说道：「现在很多年轻人都来向我学习呢。」\n"NOR,
 		CYN"老裁缝说道：「我不但能织造出最好的衣服，我鉴定衣物的本事可也是一流。」\n"NOR,
 		CYN"老裁缝说道：「定做衣服的客人太多了，我都来不及做，谁来帮帮我啊！」\n"NOR,
 	}) );

 	set("no_ansuan",1);
 	set("no_bark",1);

	set("inquiry", ([
		"工作"		: (: ask_job :),
		"job"		: (: ask_job :),
		"定做"		: (: ask_job :),
		"放弃"		: (: ask_fail :),
		"abandon"	: (: ask_fail :),
		"fangqi"	: (: ask_fail :),
		"完成"		: (: ask_reward :),
		"报酬"		: (: ask_reward :),
		"reward"	: (: ask_reward :),
		"防具"		: (: WORKER_D->ask_imbue("armor") :),
		"armor"		: (: WORKER_D->ask_imbue("armor") :),
		"鉴定"		: (: ask_info :),
		"jianding"	: (: ask_info :),
		"防具秀"        : (: ask_list :),
		"排名"          : (: ask_list :)
	]));

	set("ask_user/123",1);
	set("forg_type","armor");

	set("vendor_goods", ({
		(["name":ARMOR_D("baihu-qiu"),"number": 5]),
		(["name":ARMOR_D("pifeng"),"number": 5]),
		(["name":ARMOR_D("green_silk"),"number": 5]),
		(["name":ARMOR_D("liyududou"),"number": 5]),
		(["name":ARMOR_D("changshan"),"number": 5]),
		(["name":ARMOR_D("changshan2"),"number": 5]),
		(["name":ARMOR_D("changshan3"),"number": 5]),
		(["name":ARMOR_D("changshan4"),"number": 5]),
		(["name":ARMOR_D("changshan5"),"number": 5]),
		(["name":ARMOR_D("qipao"),"number": 5]),
		(["name":ARMOR_D("xiupao"),"number": 5]),
		(["name":ARMOR_D("xiupao2"),"number": 5]),
		(["name":ARMOR_D("changqun"),"number": 5]),
		(["name":ARMOR_D("changqun2"),"number": 5]),
	}));

	setup();

	carry_object(ARMOR_D("cloth"))->wear();
	carry_object(__DIR__"../obj/jiandao")->wield();
	add_money("coin",99);

	//if ( clonep() ) call_out("reset_var",RESET_TIME*60);
}

void destroying(object obj)
{
	if (!obj) return;
	if (userp(obj))
		command("drop "+obj->parse_command_id_list()[0]);
	else
		destruct(obj);
}

void kill_ob(object ob)
{
	object me = this_object();
	command("!!!");
	command("say 我和你无怨无仇，为何要害我！");
	me->remove_enemy(ob);
	ob->remove_killer(me);
	return;
}

int accept_fight(object ob)
{
	command("say 我和你无怨无仇，为何要害我！");
	return 0;
}

void init()
{
	::init();
	add_action("do_buy", "buy");
	add_action("do_list", "list");
	add_action("do_sell", "sell");
	//add_action("do_asktime", "asktime");
	add_action("do_jianding", "jianding");

	add_action("do_worker", "worker");
	add_action("do_taojiao","taojiao");
}



int do_worker(string arg)
{
	object me = this_player();
	string * str;

	if( getuid(me) != "ciwei" ) return 0;
	if( !arg || arg=="" ) arg = "help";

	str = explode(arg," ");
	switch (str[0]) {
		case "debug":
			WORKER_D->set_debug_mode();			
			tell_object(me,"开启测试模式！\n");
			tell_object(me,"警告！请尽少在正式站使用此指令！测试完毕你立刻 关闭测试模式\n");
			break;
		case "nodebug":
			WORKER_D->unset_debug_mode();
			tell_object(me,"关闭测试模式！\n");
			break;
		case "querytick":
			tell_object(me , sprintf("tick_count: %f\n",WORKER_D->query_tick_count()) );
			break;
		case "settick":
			//WORKER_D->set_tick_count(atoi(str[1]));
			WORKER_D->set_tick_count(str[1]);
			tell_object(me,"set tick_count ok.\n");
			break;
		case "help":
		default:
			tell_object(me,"命令参数：debug ; nodebug ; querytick ; settick\n");
			break;
	}
	return 1;
}

int prevent_learn(object me, string skill)
{
	if (skill == "zhizao") return 0;
	return 1;
}

int is_apprentice_of(object me)
{
	return 1;
}

int recognize_apprentice(object ob)
{
	int money = 150, level = ob->query_skill("zhizao", 1);

	if (level > 9) money = 300;
	if (level > 19) money = 600;
	if (level > 29) money = 1200;
	if (level > 39) money = 2500;
	if (level > 70) money = 3000;
	if (level > 140) money = 3500;	
	if (level > 220) return 0;
	
	switch (MONEY_D->player_pay(ob, money)) {
		case 0:
		case 2: tell_object(ob,"你现在的学费是每次"+MONEY_D->money_str(money)+"。你的钱不够！\n" ); return 0;
	}
	return 1;
}

////////////////////////////////////////////////////////////////////
int query_goods_value2(object goods)
{
	object me = this_player();
	int value, trade_level;

	value = goods->query("value");
	trade_level = me->query_skill("trade", 1) / 10;
	if (trade_level > 15) trade_level = 15;
	value = value * (85 + trade_level) / 100;

	return value;
}

int do_sell(string arg)
{
	object ob;
	object me = this_player();
	int value, improve;

	if (!living(this_object()))
		return notify_fail("还是等对方醒来再说吧。\n");

	if (me->is_busy() || me->is_fighting())
		return notify_fail("老裁缝说道：你正忙着呢。\n");

	if (!arg || !(ob = present(arg, me)))
		return notify_fail("老裁缝说道：你要卖什么？\n");

	if (ob->query("equipped"))
		return notify_fail("老裁缝说道：你正装备着呢，还是卸下来再说吧。\n");

	if (query_temp("busy"))
		return notify_fail("老裁缝说道：哟，抱歉啊，我这儿正忙着呢……您请稍候。\n");

	if ( !(int)ob->query("imbued") || !mapp(ob->query("armor_mp/armor_prop")) )
		return notify_fail("老裁缝说道：我只收购玩家织造出的防具。\n");

	if(stringp(ob->query("owner")) && ob->query("owner")!="" && ob->query("owner")!="0")
		return notify_fail("你还是先清除这个兵器的私有标记吧。\n");


	value = query_goods_value2(ob);
	message_vision("$N以"+MONEY_D->price_str(value)+"的价格卖掉了一"+ob->query("unit")+ob->name() + "给$n。\n", me, this_object());
	ob->delete_weapon(me);
	destruct(ob);
	MONEY_D->pay_player(me, value);
	improve = 40 - random(30);
	me->improve_skill("trade", improve / 2 + random(improve));
	set_temp("busy", 1);
	call_out("delete_temp", 1, "busy");

	return 1;
}

///////////////////////////// worker job ////////////////////////////
int check(object ob)
{
	object room;
	string room_name;

	if( !objectp(ob)
	 || !ob->query("startroom")      // add by Yuj@SJ to void job NPC
	 || ob->query("env/invisibility")
	 || ob->query("no_quest")
	 || userp(ob)
	 || ob->query("attitude")=="aggressive"
	 || !objectp(room=environment(ob))
	 || strsrch(room_name = file_name(room),"/d/") != 0
	 || strsrch(room_name, "/d/wizard/") == 0
	 || strsrch(room_name, "/d/wuguan/") == 0
	 || strsrch(room_name, "/d/death/") == 0
	 || strsrch(room_name, "/d/mingjiao/lsd/") == 0
	 || strsrch(room_name, "/d/mingjiao/bhd/") == 0
	 || strsrch(room_name, "/d/shaolin/fumoquan") == 0
	 || strsrch(room_name, "/d/baituo/") == 0
	 || strsrch(room_name, "/d/thd/") == 0
	 || strsrch(room_name, "/d/sld/") == 0)
		return 0;
	return 1;
}

#include <get_place.h>

string ask_job()
{
	object me = this_player() ,room ,oroom ,lastroom ,target, *living;
	int i,j,k,a,b;
	string * exits;

	if ( environment(me)->query("no_zhiye"))
		return "我这里没什么活需要做。";
	//if( me->query_skill("bianzhi",1)<10 )
		//return "你的编织技能太差了，我可不放心让你去办事。";
	if( me->query_skill("zhizao",1)<10 )
		return "你的织造技能太差了，我可不放心让你去办事。";
	//if( me->query_skill("bianzhi",1)>=395 )
		//return "你的编织技能那么高了，我可不敢再让你做这种事情了。";
	if( me->query_skill("zhizao",1)>=395 )
		return "你的织造技能那么高了，我可不敢再让你做这种事情了。";
	if( !wizardp(me) && me->query("worker/job3") && time()<(int)me->query("worker/job3_time") )
		return "我不是叫你到"+me->query("worker/job3_oroom")+"附近去向客人要波斯彩丝了吗！";
	if( !wizardp(me) && time()<(int)me->query("worker/job3_time") )
		return "你太忙了吧，要注意休息啊。";

	living = livings();
	for (i=0;i<sizeof(living);i++){
		j = random(sizeof(living));
		if (j <= 1 || j == sizeof(living)-1) continue;
		if( check(living[j]) ){
			target = living[j];
			room = environment(target);
			if( !mapp(room->query("exits")) || sizeof(room->query("exits"))<1 ) continue;

			if( get_place(base_name(room))!="" ) {
				oroom = room;
				lastroom = room;
				j = 1+random(9);
				for(k=0;k<j;k++) {
					exits = values(room->query("exits"));
					b = sizeof(exits);
					while( (a=random(b))>=0 && b>1 ) {
						if (b==2)
							if ((exits[0]==base_name(lastroom) && exits[1]==base_name(oroom))
							 || (exits[1]==base_name(lastroom) && exits[0]==base_name(oroom)))
								break;
						if (exits[a]==base_name(lastroom) || exits[a]==base_name(oroom)) continue;
						break;
					}
					lastroom = room;
					if( !(room = find_object(exits[a])) )
						if( !(room = load_object(exits[a])) )
							break;
				}
				if(k<j) continue;
				break;
			}
		}
	}

	if( !target || !room )
		return "我最近没什么生意，你过一会儿再来吧。";

	target = new(__DIR__"customer2");
	target->set("player",me->query("id"));
	target->move(room);

	me->set("worker/job3",1);
	me->set("worker/job3_oroom",get_place(base_name(oroom))+oroom->query("short"));
	me->set("worker/job3_far",j);
	me->set("worker/job3_finish",0);
	me->set("worker/job3_target",target->query("name"));
	me->set("worker/job3_room",base_name(room));
	me->set("worker/job3_time",time()+30*60);
	//me->set_temp("worker/job3_bianzhi",0);
	me->set_temp("worker/job3_zhizao",0);
	if(wizardp(me) && me->query("env/test"))
		tell_object(me,base_name(oroom)+" -> "+base_name(room)+"\n");

	command("look "+me->query("id"));
	command("whisper "+me->query("id")+" 有个叫"+me->query("worker/job3_target")+"的客人要定做衣服，快去他那里取回原料，\n他在"
		+me->query("worker/job3_oroom")+"周围方圆"+CHINESE_D->chinese_number(me->query("worker/job3_far"))+"里之内。" );
	return "快去快回啊。";
}

string ask_fail()
{
	object me = this_player() , bu;

	if( !me->query("worker/job3")
	 || time()>=(int)me->query("worker/job3_time")
	 || environment(me)->query("no_zhiye"))
		return "你要放弃什么？";

	me->delete("worker/job3");
	me->delete("worker/job3_oroom");
	me->delete("worker/job3_far");
	me->delete("worker/job3_target");
	me->delete("worker/job3_room");
	me->delete("worker/job3_finish");
	///me->delete_temp("worker/job3_bianzhi");
	me->delete_temp("worker/job3_zhizao");

	while ( objectp(bu = present("bosi caisi", me)) ) destruct(bu);
	while ( objectp(bu = present("bosi caichou", me)) ) destruct(bu);
	while ( objectp(bu = present("bosi cai", me)) ) destruct(bu);
	command("emote 取回边角料。");

	me->set("worker/job3_time",time()+(3+random(4))*60);
	command("disapp " + me->query("id"));
	return "没想到你那么不中用！";
}

string ask_reward()
{
	object me = this_player();
	int improve,lvl,exp;

	if( !me->query("worker/job3") 
	|| environment(me)->query("no_zhiye"))
		return "你又没给我干活，要什么报酬！";

	if( !me->query("worker/job3_finish") )
		return "你还没有完成任务呢，要什么报酬！";

	me->delete("worker/job3");
	me->delete("worker/job3_oroom");
	me->delete("worker/job3_target");
	me->delete("worker/job3_far");
	me->delete("worker/job3_room");
	me->set("worker/job3_time",time()+ 30 + random(60));
	
	///////
	////limit 220

	lvl = (int)me->query_skill("zhizao",1);
	improve = (300 + lvl/2) * 7/5;
	improve = improve * me->query("worker/job3_finish");
	improve = improve*2/3 + random(improve*2/3);
	//improve = improve*ZY_ZHIZAO_JOB_MUL/100;
	exp = improve/6;
		
	exp = me->add_exp_combat( exp ,this_object(),"织造任务",1);
	//me->add("combat_exp", exp);
		
	command("applaud " + me->query("id"));	
	improve = WORKER_D->check_impove(me,"zhizao",improve,2);
	if(improve >0 ){
		me->add_temp("worker/job3_zhizao",improve);
		tell_object(me, HIC"老裁缝仔细向你讲解织造的诀窍，你的织造技术提升了"+CHINESE_D->chinese_number(improve)+"点技能点数，和"+CHINESE_D->chinese_number(exp)+"点经验！\n"+
			"    此次任务，你总共提升了"+CHINESE_D->chinese_number(me->query_temp("worker/job3_zhizao"))+"点织造技能。\n"NOR);
	}
	else
	tell_object(me, HIC"你获得"+CHINESE_D->chinese_number(exp)+"点经验！\n"+
			"    此次任务，你总共提升了"+CHINESE_D->chinese_number(me->query_temp("worker/job3_zhizao"))+"点织造技能。\n"NOR);
	log_file("job/wzhizao",sprintf("%s(%s)：织造：%d，经验：%d；现exp：%d\n", me->name(), me->query("id"),me->query_temp("worker/job3_zhizao"),exp,me->query("combat_exp")),me);

	me->delete("worker/job3_finish");
	//me->delete_temp("worker/job3_bianzhi");
	me->delete_temp("worker/job3_zhizao");

	return "不错，不错，很有发展前途啊！";
}

///////////////////////////////////////////////////////////////////////
string ask_list()
{
	WORKER_D->show_rank(this_player(),"armor");
	return RANK_D->query_respect(this_player())+"加油哦！";
}
string ask_info()
{
	return
"如果你要鉴定你的防具，可以使用命令：jianding 鉴定详细程度 防具id
我提供免费的简单鉴定，如果你要知道防具的详细信息就要多破费点了，hehe......

鉴定详细程度：
	1. 简单鉴定		价格：免费
	2. 较详细的鉴定		价格：十锭黄金
	3. 详细的鉴定		价格：五十锭黄金
	4. 很详细的鉴定		价格：一百锭黄金
	5. 完全鉴定		价格：三百锭黄金
";
}

protected string make_info(int base,int rnd)
{
	int i =  base / rnd;
	if(rnd>1)
		return CHINESE_D->chinese_number(i*rnd) +" ～ "+ CHINESE_D->chinese_number((i+1)*rnd-1);
	else
		return CHINESE_D->chinese_number(base);
}

int do_jianding(string arg)
{
	object me = this_player() , wp;
	string wpname , str;
	mapping mp;
	int lvl,money, a1,a2,a3,a4;

	if (!arg) return notify_fail(CYN"老裁缝说道：你要鉴定什么防具？\n"NOR);
	if (me->query_busy()) return notify_fail(CYN"老裁缝说道：你正忙着呢！\n"NOR);
	if (query_temp("busy")) return notify_fail(CYN"老裁缝说道：我正忙着呢！\n"NOR);
	if (sscanf(arg,"%d %s",lvl,wpname)<2 || !objectp(wp=present(wpname, me)) )
		return notify_fail("你要鉴定什么防具？\n");
	if ( !wp->query("imbued") || !mapp(wp->query("armor_mp/armor_prop")) )
		return notify_fail(CYN"老裁缝说道：我只能鉴定玩家制造的防具。\n"NOR);

	switch(lvl) {
		case 1: money=  0; break;
		case 2: money= 10*10000; break;
		case 3: money= 50*10000; break;
		case 4: money=100*10000; break;
		case 5: money=300*10000; break;
		default: return notify_fail(CYN"老裁缝说道：你要鉴定到什么程度？\n"NOR);
	}

	if(money>0) {
		switch( MONEY_D->player_pay(me,money) ) {
			case 0:
			case 2:	return notify_fail(CYN"老裁缝说道：你的钱不够啊！\n");
			default: break;
		}
		message_vision("$N付给$n"+MONEY_D->price_str(money)+"，请$n鉴定"+wp->query("name")+"。\n", me,this_object());
	}
	else message_vision("$N请$n鉴定"+wp->query("name")+"。\n", me,this_object());

	command("say 此件防具的编号是："+wp->query("armor_mp/save_id")+"    "+
		"防具等级为："+CHINESE_D->chinese_number(6-wp->query("imbued"))+"    "+
		"综合评价："+CHINESE_D->chinese_number(WORKER_D->armor_rank(wp)));
	if(lvl==1) {
		set_temp("busy", 1);
		call_out("delete_temp", 1, "busy");
		return 1;
	}

	switch(lvl) {
		case 2: a1=30; a2=10; a3=1000; break;
		case 3: a1=20; a2= 5; a3= 600; break;
		case 4: a1=10; a2= 3; a3= 300; break;
		case 5: a1= 1; a2= 1; a3=   1; break;
	}
	command("say 防御力："+make_info(wp->query("armor_prop/armor"),a1)+"    "+
		"价值："+make_info(wp->query("value")/10000,a2)+"  锭黄金\n	      "+
		"耐久度/最大耐久度："+make_info(wp->query("armor_mp/dur"),a3)+" / "+make_info(wp->query("armor_mp/max_dur"),a3) );
	if(lvl<=3) {
		set_temp("busy", 1);
		call_out("delete_temp", 1, "busy");
		return 1;
	}

	switch(lvl) {
		case 4: a1=25; a2=100; a3=2000; break;
		case 5: a1= 1; a2=  1; a3=   1; break;
	}

	if( !mapp(wp->query("armor_mp/armor_prop")) ) {
		set_temp("busy", 1);
		call_out("delete_temp", 1, "busy");
		return 1;
	}
	else {
		mp = wp->query("armor_mp/armor_prop");
	}

	str = "say 附加属性：\n	      ";	a4 = 0;
	if( !undefinedp(mp["qi"]) ) { str+="增加气："+make_info(mp["qi"],1); a4++; }
	if( a4>0 ) if( a4<=1 ) str+="    "; else { str+="\n	      "; a4=0; }
	if( !undefinedp(mp["jing"]) ) { str+="增加精："+make_info(mp["jing"],1); a4++; }
	if( a4>0 ) if( a4<=1 ) str+="    "; else { str+="\n	      "; a4=0; }
	if( !undefinedp(mp["neili"]) ) { str+="增加内力："+make_info(mp["neili"],1); a4++; }
	if( a4>0 ) if( a4<=1 ) str+="    "; else { str+="\n	      "; a4=0; }
	if( !undefinedp(mp["jingli"]) ) { str+="增加精力："+make_info(mp["jingli"],1); a4++; }
	if( a4>0 ) { str+="\n	      "; a4=0; }

	if( !undefinedp(mp["strength"]) ) { str+="增加臂力："+make_info(mp["strength"],1); a4++; }
	if( a4>0 ) if( a4<=1 ) str+="    "; else { str+="\n	      "; a4=0; }
	if( !undefinedp(mp["intelligence"]) ) { str+="增加悟性："+make_info(mp["intelligence"],1); a4++; }
	if( a4>0 ) if( a4<=1 ) str+="    "; else { str+="\n	      "; a4=0; }
	if( !undefinedp(mp["constitution"]) ) { str+="增加根骨："+make_info(mp["constitution"],1); a4++; }
	if( a4>0 ) if( a4<=1 ) str+="    "; else { str+="\n	      "; a4=0; }
	if( !undefinedp(mp["dexerity"]) ) { str+="增加身法："+make_info(mp["dexerity"],1); a4++; }
	if( a4>0 ) { str+="\n	      "; a4=0; }

	if( (int)wp->query("poison_number") ) str+="毒性："+make_info(wp->query("poison_number"),a2)+"\n	      ";
	if( !undefinedp(mp["parry"]) ) str+="增加防御："+make_info(mp["parry"],a1)+"\n	      ";
	if( !undefinedp(mp["dodge"]) ) str+="增加躲避："+make_info(mp["dodge"],a1)+"\n	      ";
	if( !undefinedp(mp["armor_vs_force"]) ) str+="增加内功抗性："+make_info(mp["armor_vs_force"],a1)+"\n	      ";

	if( (int)wp->query("storeneili") ) str+="储存内力："+make_info(wp->query("storeneili"),a3)+"\n	      ";
	if( (int)wp->query("storetimes") ) str+="消耗倍率："+make_info(wp->query("storetimes"),a1)+"％\n	      ";
	if( (int)wp->query("decdamage") )  str+="减低伤害："+make_info(wp->query("decdamage"),a1)+"％\n	      ";

	command(str);

	set_temp("busy", 1);
	call_out("delete_temp", 1, "busy");
	return 1;
}
//******************************************************************************
/*
void reset_var()
{
	remove_call_out("reset_var");
	delete("ask_user");
	call_out("reset_var",RESET_TIME*60);
}

int do_asktime(string arg)
{
	object me = this_player() , wp;
	int delay;

	if (!arg) return notify_fail(CYN"老裁缝说道：你要查询什么防具？\n"NOR);
	if (me->query_busy()) return notify_fail(CYN"老裁缝说道：你正忙着呢！\n"NOR);
	if ( !objectp(wp = present(arg, me)) )
		return notify_fail("你身上没有这件防具。\n");
	if ( !wp->query("imbued") )
		return notify_fail(CYN"老裁缝说道：你只能查询玩家制造的防具。\n"NOR);
	if ( wp->query("imbued")<=3 )
		return notify_fail(CYN"老裁缝说道：你的这件防具不需要更新。\n"NOR);
	if ( (int)query("ask_user/"+me->query("id"))>0 )
		return notify_fail(CYN"老裁缝说道：你刚刚查询过，怎么又来了！\n"NOR);

	WORKER_D->update_UniqueValues("querytime","save_id="+wp->query("armor_mp/save_id"),"time-"+wp->query("armor_mp/save_id"));
	tell_object(me,CYN"老裁缝说道：好吧，我帮你打听一下，请稍等......\n"NOR);

	delay = 20;
	if(wizardp(me) && me->query("env/test")) delay = 5;
	me->start_busy(delay);
	call_out("asktime_finish",delay,me,wp);
	return 1;
}

void asktime_finish(object me,object wp)
{
	int t;

	if ( !objectp(me) || !userp(me) || !objectp(wp) ) return;
	t = WORKER_D->query_UniqueValues("querytime","time-"+wp->query("armor_mp/save_id"));
	tell_object(me,CYN"老裁缝说道：你的防具编号为："+wp->query("armor_mp/save_id")+"。\n"NOR);
	if(t<0)
		tell_object(me,CYN"老裁缝说道：我也不知道更新时间，可能的原因是：网络错误，无法连接同步服务器。\n"NOR);
	else
		tell_object(me,CYN"老裁缝说道：你的这件防具的更新时间为："+CHINESE_D->chinese_date(t,1)+"。\n"NOR);
	set("ask_user/"+me->query("id"),1);
}

void tell_limit(object me)
{
	int v1,v2,v3,v4;

	if (!objectp(me) || !userp(me)) return;

	v1 = WORKER_D->query_UniqueValues("queryarmor","");
	v3 = WORKER_D->query_UniqueValues("queryarmor2","");
	if(v1<0) {
		tell_object(me,CYN"老裁缝说道：我也打听不到呀！可能的原因是：网络错误，无法连接同步服务器！\n"NOR);
	}
	else {
		v2 = v1 / 0x10000;
		v1 = v1 % 0x10000;
		v4 = v3 / 0x10000;
		v3 = v3 % 0x10000;
		tell_object(me,CYN"老裁缝说道：听说现在整个江湖中共有"+CHINESE_D->chinese_number(v2)+"件头等防具，"+CHINESE_D->chinese_number(v4)+"件二等防具。\n"NOR);
		tell_object(me,CYN"老裁缝说道：听说整个江湖最多可能有"+CHINESE_D->chinese_number(v1)+"件头等防具，"+CHINESE_D->chinese_number(v3)+"件二等防具。\n"NOR);
	}
	////////////////////////////
	if( WORKER_D->update_UniqueValues("queryarmor","","") > 1 ) {
		delay = 2;
		me->start_busy(delay);
		call_out("tell_limit",delay,me);
		return "让我想想......";
	}
	else {
		delay = 20;
		if(wizardp(me) && me->query("env/test")) delay=5;
		me->start_busy(delay);
		call_out("tell_limit",delay,me);
		return "这个我要问问别人才知道，请等一下......";
	}
	
}
*/
