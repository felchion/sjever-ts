// zhujian-shi
// by augx@sj  9/15/2001

#include <ansi.h>
#include <job_mul.h>
#include <get_place.h>

/*
/adm/daemons/workerd.c
*/

inherit NPC;
inherit F_VENDOR; 

#include <mudlib.h>

#define RESET_TIME	15

string ask_job();
string ask_fail();
string ask_reward();
string ask_info();
string ask_list();
string ask_weilan();
int do_answer(string arg);
#include "/d/zhiye/npc/forbook.c"

int do_answer(string arg)
{
	string id1,id2,cmds;
	int i,j,rate;
	object lu;
	object obj,npc;
	object me = this_player();

	//if(wizardp(me) && INTERMUD_MUD_NAME!="TEST" && INTERMUD_MUD_NAME!="SJ") return notify_fail("你老人家不是一直用SMASH的么？\n");
	if(!me->query_temp("zj_answer")) return 0;
	if(!arg) return notify_fail("你想说什么？\n");
	switch(arg)
	{
		case "sword":
		case "axe":
		case "blade":
		case "brush":
		case "club":
		case "dagger":
		case "fork":
		case "hammer":
		case "hook":
		case "spear":
		case "staff":
		case "stick":
		case "xiao":
		case "whip": break;
		default:
		return notify_fail("有这种武器么？\n");
	}

	me->delete_temp("zj_answer");

	if(objectp(query_temp("dazao_target")))
	return notify_fail("我正忙着呢！\n");

	//if ( !objectp(weapon=this_object()->query_temp("weapon")) || weapon->query("id")!="tiechui" )
	//return notify_fail("我的铁锤不见了，以后再说吧。\n");
	if( !environment(this_object()) || !(lu=present("huo lu",environment(this_object()))))
	{
		command("shout");
		return notify_fail("昨夜遇盗，火炉已经被小贼给偷了。\n");
	}


	if( !me->query("imbue_reward")){
		if( me->query("combat_exp") < 500000 )
		return notify_fail("依您目前的水平，我看是还是别找他，你以后再来吧。\n");

		if( me->query("combat_exp")/1000000 < (int)me->query("user_weapon_imbued") ){
			return notify_fail("韦兰铁匠现在很忙，你还是好好练功，过段时间再来吧。\n");
		}
		me->add("user_weapon_imbued", 1);
	}
	else me->add("imbue_reward",-1);


	if(!me->query("imbue_reward")) me->delete("imbue_reward");

	if( me->query("combat_exp") > 3000000 && !random(20) )
	i = 3;
	else if( me->query("combat_exp") > 1000000  && !random(10))
	i = 2;
	else
	i = 1;

  if (me->query("quest/forg_luck")>0) {
    command("say 哇，幸运女神对你撩裙子了啊？？？\n");
    command("admire");
		if(me->query("quest/forg_spec")){ 
			me->delete("quest/forg_spec");
			CHANNEL_D->do_channel(this_object(), "rumor", BLINK+HIY"据说"+get_place(file_name(environment(me)))+"将有神兵现世！"+NOR);
			i=5;
    }
		else if(random(1000) ) i = 4;
		else { 
			CHANNEL_D->do_channel(this_object(), "rumor", BLINK+HIY"据说"+get_place(file_name(environment(me)))+"将有神兵现世！"+NOR);
			i=5;
    }
    me->add("quest/forg_luck",-1);
  }
	 
	switch(i)
	{
		case 0:j = 2+random(2);break;
		case 1:j = 4+random(2);break;
		case 2:j = 6+random(2);break;
		case 3:j = 8;break;
		case 4:j = 9;break;
		case 5:j = 10;break;
		default:
		return notify_fail("我拷！你是神仙啊！\n");
	}

	command("nod");
	npc = new(NPC_D("weilan_tj"));
	npc->move(environment(this_object()));

	message_vision("$N快步走了过来。\n",npc);

	obj = new("/d/zhiye/obj/kuangshi");
	if(obj && obj->set_level(j))
	{
		obj->move(npc);
		id1= obj->query("id");
		message_vision("$N交给$n一"+obj->query("unit")+obj->name()+"。\n",this_object(),npc );
	}
	obj = new("/d/zhiye/obj/kuangshi");
	if(obj && obj->set_level(j))
	{
		obj->move(npc);
		//message_vision("$N从怀里掏出"+obj->name()+"。\n",this_object());
		message_vision("$N交给$n一"+obj->query("unit")+obj->name()+"。\n",this_object(),npc );
	}
	obj = new("/d/zhiye/obj/othermaterial");

	if(obj && obj->set_level(i+1,1))
	{
		obj->move(npc);
		id2= obj->query("id");
		message_vision("$N交给$n一"+obj->query("unit")+obj->name()+"。\n",this_object(),npc );
	}
	cmds=arg+" "+id1+" with "+id2;
	set_temp("dazao_target",me);

	//lu->duanzao_it(this_object(),cmds)
	if(!npc->go_dazao(lu,me,cmds)){
		log_file("career/worker",getuid(me)+" ask 打造失败。",this_object() );
		return notify_fail("出错了，快去通知Master。\n");
	}
	command("say 稍等片刻。\n");
	return 1;
}

string ask_weilan()
{
	object me;
	object lu;
	me = this_player();

	if(!userp(me)) 
		return 0;

//	if(wizardp(me)  && INTERMUD_MUD_NAME!="TEST" && INTERMUD_MUD_NAME!="SJ" ) 
//		return "你老人家不是一直用SMASH的么？\n";

	if(objectp(query_temp("dazao_target")))
		return "我正忙着呢！\n";

	//if ( !objectp(weapon=this_object()->query_temp("weapon")) || weapon->query("id")!="tiechui" )
	//return "我的铁锤不见了，以后再说吧。\n";
	if( !environment(this_object()) || !(lu=present("huo lu",environment(this_object())))){
		command("shout");
		return "昨夜遇盗，火炉已经被小贼给偷了。\n";
	}
	
	if (me->query("eff_mud_time")/(me->query("mud_time")/100) < 95)
	  return "你的在线有效比太低，无法打造。\n";
	
	if( !me->query("imbue_reward")){
		if( me->query("combat_exp") < 500000 ) 
			return "依您目前的水平，我看是还是别找他，你以后再来吧。\n";

		if( me->query("combat_exp")/1000000 < (int)me->query("user_weapon_imbued") ){
			return "韦兰铁匠现在很忙，你还是好好练功，过段时间再来吧。\n";
		}
	}
	command("say 哦，你想找韦兰铁匠打造武器？");
	command("say 韦兰铁匠现在接受韦爵爷雇用了，没那么多空闲时间了。");
	command("whisper "+getuid(me)+" 看在你练功那么努力的份上，我就叫韦兰铁匠来吧。");
	me->set_temp("zj_answer",1);
	return "你想要打造什么就跟我说(da)吧！\n";
}

void create()
{
	set_name("铸剑师",({ "zhujian shi","shi" }) );
	set("gender", "男性" );
	set("age", 51);
	set("long","这是位严肃不苟言笑的铸剑师，虽然没有造出过什么神兵利器，但打造的普通兵器质量还不错。
在出售自己制造的武器之外，他也做收购兵器的生意。");

	set("str", 55);
	set("dex", 55);
	set("con", 25);
	set("int", 25);
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
	set_skill("duanzao", 300);

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
 		CYN"铸剑师沾沾自喜的说道：「最近又买到不少好兵器。」\n"NOR,
 		CYN"铸剑师说道：「我打造的兵器质量可是有保证的哦。」\n"NOR,
 		CYN"铸剑师四顾着说道：「你们需不需要学习锻造技术？可以向我学习啊，学费么.....嘿嘿」\n"NOR,
 		CYN"铸剑师说道：「我现在太忙了走不开，谁能帮我把打好的兵器给用户送去啊？」\n"NOR,
 		CYN"铸剑师说道：「虽然我造不了神兵利器，但是我鉴定兵器的本事可是天下第一！」\n"NOR,
 	}) );

 	set("no_ansuan",1);
 	set("no_bark",1);

	set("inquiry", ([
		"工作"		: (: ask_job :),
		"job"		: (: ask_job :),
		"运送"		: (: ask_job :),
		"放弃"		: (: ask_fail :),
		"abandon"	: (: ask_fail :),
		"完成"		: (: ask_reward :),
		"报酬"		: (: ask_reward :),
		"reward"	: (: ask_reward :),
		"兵器"		: (: WORKER_D->ask_imbue("weapon") :),
		"weapon"	: (: WORKER_D->ask_imbue("weapon") :),
		"鉴定"		: (: ask_info :),
		"jianding"	: (: ask_info :),
		"排名"          : (: ask_list :),
		"兵器谱"          : (: ask_list :),
		"韦兰铁匠":(:ask_weilan:),
		"weilan":(:ask_weilan:),
		"韦兰":(:ask_weilan:),
	]));

	set("ask_user/123",1);
	set("forg_type","weapon");

	set("vendor_goods", ({
		(["name": BINGQI_D("sword/sword"),"number":10]),
		(["name": BINGQI_D("dagger/bishou"),"number":10]),
		(["name": BINGQI_D("blade/blade"),"number":10]),
		(["name": BINGQI_D("hammer/chui"),"number":10]),
		(["name": BINGQI_D("club/gun"),"number":10]),
		(["name": BINGQI_D("staff/gangzhang"),"number":10]),
		(["name": BINGQI_D("whip/whip"),"number":10]),
		(["name": BINGQI_D("stick"),"number":10]),
		(["name": BINGQI_D("axe"),"number":10]),
		(["name": BINGQI_D("brush"),"number":10]),
	}));

	setup();

	carry_object(ARMOR_D("cloth"))->wear();
	carry_object(__DIR__"../obj/tiechui")->wield();
	add_money("coin",99);

//	if ( clonep() ) call_out("reset_var",RESET_TIME*60);
}

string ask_list()
{
	WORKER_D->show_rank(this_player(),"weapon");
	return RANK_D->query_respect(this_player())+"加油哦！";
}

void kill_ob(object ob)
{
	object me = this_object();
	command("!!!");
	command("say 我是一个守法的铸剑师，为什么要杀我呢？");
	me->remove_enemy(ob);
	ob->remove_killer(me);
	return;
}

int accept_fight(object ob)
{
	command("say 我是一个守法的铸剑师，为什么要杀我呢？");
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
	add_action("do_taojiao","taojiao");///for tiejiang xx
	add_action("do_answer",({"da","answer"}));
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
	if (skill == "duanzao")
		return 0;
	return 1;
}

int is_apprentice_of(object me)
{
	return 1;
}

int recognize_apprentice(object ob)
{
	int money = 150, level = ob->query_skill("duanzao", 1);

	if (level > 9) money = 300;
	if (level > 19) money = 600;
	if (level > 29) money = 1200;
	if (level > 39) money = 2500;
	if (level > 70) money = 3000;
	if (level > 140) money = 3500;	
	if (level > 220) return 0;
	
//	if(ob->query("registered")>3 )
//	  money  = money / 2;
//	if(ob->query("registered") == 3 )
//	  money  = money / 10 * 7;

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
		return notify_fail("铸剑师说道：你正忙着呢。\n");

	if (!arg || !(ob = present(arg, me)))
		return notify_fail("铸剑师说道：你要卖什么？\n");

	if (query_temp("busy"))
		return notify_fail("铸剑师说道：哟，抱歉啊，我这儿正忙着呢……您请稍候。\n");

	if ( !(int)ob->query("imbued") || !mapp(ob->query("weapon_mp/weapon_prop")) )
		return notify_fail("铸剑师说道：我只收购玩家制造的兵器。\n");
	
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
	 || room->query("no_fight")
	 || room->query("quest")
	 || room->query("outdoors") == "昆仑翠谷"
	 || strsrch(room_name = file_name(room),"/d/") != 0
	 || strsrch(room_name, "/d/wizard/") == 0
	 || strsrch(room_name, "/d/wuguan/") == 0
	 || strsrch(room_name, "/d/death/") == 0
	 || strsrch(room_name, "/d/group/") == 0
	 || strsrch(room_name, "/d/mingjiao/lsd/") == 0
	 || strsrch(room_name, "/d/mingjiao/bhd/") == 0
	 || strsrch(room_name, "/d/shaolin/fumoquan") == 0
	 || strsrch(room_name, "/d/baituo/") == 0 )
		return 0;
	return 1;
}


string ask_job()
{
	object me = this_player() ,room ,oroom ,lastroom ,target, *living , obj;
	int lvl = (int)me->query_skill("duanzao",1) , i,j,k,a,b;
	string * exits;

	if ( environment(me)->query("no_zhiye"))
		return "我这里没什么活需要做。";
	if( lvl<10 )
		return "你的技能太差了，我可不放心让你去办事。";
	if( lvl>=395 )
		return "您的锻造技能那么高了，我可不敢再差遣您了。";
	if( !wizardp(me) && me->query("worker/job2") && time()<(int)me->query("worker/job2_time") )
		return "我不是叫你把宝剑送给"+me->query("worker/job2_oroom")+"附近的"+me->query("worker/job2_target")+"了吗！";
	if( !wizardp(me) && time()<(int)me->query("worker/job2_time") )
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
				if ( random(10)<3 && target->query("race")=="人类" ) {
					j = 0;
					break;
				}

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

	if(j>0) {
		target = new(__DIR__"customer");
		target->set("player",me->query("id"));
		target->move(room);
	}

	obj = new("/d/zhiye/obj/baojian");
	obj->move(me);

	me->set("worker/job2",1);
	me->set("worker/job2_oroom",get_place(base_name(oroom))+oroom->query("short"));
	me->set("worker/job2_far",j);
	me->set("worker/job2_finish",0);
	me->set("worker/job2_target",target->query("name"));
	me->set("worker/job2_room",base_name(room));
	me->set("worker/job2_time",time()+20*60);
	if(wizardp(me) && me->query("env/test"))
		tell_object(me,base_name(oroom)+" -> "+base_name(room)+"\n");

	command("pat "+me->query("id"));
	if (j)
		command("whisper "+me->query("id")+" 快帮我把宝剑送给"+me->query("worker/job2_target")+"，他在"
			+me->query("worker/job2_oroom")+"周围方圆"+CHINESE_D->chinese_number(me->query("worker/job2_far"))+"里之内。" );
	else
		command("whisper "+me->query("id")+" 快帮我把宝剑送给"+me->query("worker/job2_oroom")+"的"
			+me->query("worker/job2_target")+"。");
	message_vision(CYN"铁匠给$N一把刚刚打造好的定制宝剑。\n"NOR, me);
	return "好好干活，我不会亏待你的。";
}

string ask_fail()
{
	object me = this_player() , obj;

	if( !me->query("worker/job2")
	 || time()>=(int)me->query("worker/job2_time")
	 || environment(me)->query("no_zhiye"))
		return "你要放弃什么？";

	me->delete("worker/job2");
	me->delete("worker/job2_oroom");
	me->delete("worker/job2_far");
	me->delete("worker/job2_target");
	me->delete("worker/job2_room");
	me->delete("worker/job2_finish");

	if ( objectp(obj = present("dingzhi baojian", me)) ) {
		message_vision(CYN"$N把定制宝剑还给铸剑师。\n"NOR, me);
		destruct(obj);
		me->set("worker/job2_time",time()+(2+random(3))*60);
		command("disapp " + me->query("id"));
		return "没想到这么简单的事情你都做不了！";
	}
	else {
		command("slash " + me->query("id"));
		me->set("worker/job2_time",time()+(10+random(10))*60);
		return "货送不到也就算了，你怎么把货都弄丢了！";
	}
}

void destroying(object obj)
{
	if (!obj) return;
	if (userp(obj))
		command("drop "+obj->parse_command_id_list()[0]);
	else
		destruct(obj);
}

string ask_reward()
{
	object me = this_player();
	int improve,lvl,exp;

	if( !me->query("worker/job2")
	|| environment(me)->query("no_zhiye"))
		return "你又没给我干活，要什么报酬！";

	if( !me->query("worker/job2_finish") )
		return "你还没有完成任务呢，要什么报酬！";

	me->delete("worker/job2");
	me->delete("worker/job2_oroom");
	me->delete("worker/job2_target");
	me->delete("worker/job2_far");
	me->delete("worker/job2_room");
	me->delete("worker/job2_finish");
	me->set("worker/job2_time",time()+ 20 + random(30));

	lvl = (int)me->query_skill("duanzao",1) / 2;
	improve = (300 + lvl) * 11/10;
	improve = improve*2/3 + random(improve*2/3);
	//improve = improve*ZY_ZHUJIAN_JOB_MUL/100;
	exp = improve/20;
	
	exp = me->add_exp_combat( exp ,this_object(),"送剑任务",1);
	//me->add("combat_exp", exp);
		
	command("applaud " + me->query("id"));
	improve = WORKER_D->check_impove(me,"duanzao",improve,2);
	if(improve>0) tell_object(me, HIC"铸剑师仔细向你讲解锻造的诀窍，你的锻造技术提升了"+CHINESE_D->chinese_number(improve)+"点技能点数，和"+CHINESE_D->chinese_number(exp)+"点经验！\n"NOR);
	else tell_object(me, HIC"你获得"+CHINESE_D->chinese_number(exp)+"点经验！\n"NOR);
	/*
			
	if (me->query_skill("duanzao",1) <220){
		me->improve_skill("duanzao", improve);
		tell_object(me, HIC"铸剑师仔细向你讲解锻造的诀窍，你的锻造技术提升了"+CHINESE_D->chinese_number(improve)+"点技能点数，和"+CHINESE_D->chinese_number(exp)+"点经验！\n"NOR);
	}
	else
	{
		improve = 0;
		tell_object(me, HIC"你获得"+CHINESE_D->chinese_number(exp)+"点经验！\n"NOR);
	}
	*/
	log_file("job/wsongjian",sprintf("%s(%s)：技能点数：%d，经验：%d；现exp：%d\n", me->name(), me->query("id"),improve,exp,me->query("combat_exp")),me);

	return "好样的，干活真利索，以后有空还来我这里干活啊。";
}

string ask_info()
{
	return
"如果你要鉴定你的兵器，可以使用命令：jianding 鉴定详细程度 兵器id
我提供免费的简单鉴定，如果你要知道兵器的详细信息就要多破费点了，hehe......

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

	if (!arg) return notify_fail(CYN"铸剑师说道：你要鉴定什么兵器？\n"NOR);
	if (me->query_busy()) return notify_fail(CYN"铸剑师说道：你正忙着呢！\n"NOR);
	if (query_temp("busy")) return notify_fail(CYN"铸剑师说道：我正忙着呢！\n"NOR);
	if (sscanf(arg,"%d %s",lvl,wpname)<2 || !objectp(wp=present(wpname, me)) )
		return notify_fail("你要鉴定什么兵器？\n");
	if ( !wp->query("imbued") || !mapp(wp->query("weapon_mp/weapon_prop")) )
		return notify_fail(CYN"铸剑师说道：我只能鉴定玩家制造的兵器。\n"NOR);

	switch(lvl) {
		case 1: money=  0; break;
		case 2: money= 10*10000; break;
		case 3: money= 50*10000; break;
		case 4: money=100*10000; break;
		case 5: money=300*10000; break;
		default: return notify_fail(CYN"铸剑师说道：你要鉴定到什么程度？\n"NOR);
	}

	if(!wizardp(me) && money > 0) {
		switch( MONEY_D->player_pay(me,money) ) {
			case 0:
			case 2:	return notify_fail(CYN"铸剑师说道：你的钱不够啊！\n");
			default: break;
		}
		message_vision("$N付给$n"+MONEY_D->price_str(money)+"，请$n鉴定"+wp->query("name")+"。\n", me,this_object());
	}
	else message_vision("$N请$n鉴定"+wp->query("name")+"。\n", me,this_object());

	command("say 此件兵器的编号是："+wp->query("weapon_mp/save_id")+"    "+
		"兵器等级为："+CHINESE_D->chinese_number(6-wp->query("imbued"))+"    "+
		"综合评价："+CHINESE_D->chinese_number(WORKER_D->weapon_rank(wp)));
	if(lvl==1) {
		set_temp("busy", 1);
		call_out("delete_temp", 1, "busy");
		return 1;
	}

	switch(lvl) {
		case 2: a1=50; a2=10; a3=10; a4=1000; break;
		case 3: a1=30; a2= 5; a3= 5; a4= 600; break;
		case 4: a1=20; a2= 3; a3= 3; a4= 300; break;
		case 5: a1= 1; a2= 1; a3= 1; a4=   1; break;
	}
	command("say 威力："+make_info(wp->query("weapon_prop/damage"),a1)+"    "+
		"锋利度："+make_info(wp->query("sharpness"),a2)+"    "+
		"硬度："+make_info(wp->query("rigidity"),a2)+"\n	      "+
		"耐久度/最大耐久度："+make_info(wp->query("weapon_mp/dur"),a4)+" / "+make_info(wp->query("weapon_mp/max_dur"),a4)+"\n	      "+
		"价值："+make_info(wp->query("value")/10000,a3)+"  锭黄金");
	if(lvl<=3) {
		set_temp("busy", 1);
		call_out("delete_temp", 1, "busy");
		return 1;
	}

	switch(lvl) {
		case 4: a1=30; a2=25; a3=100; break;
		case 5: a1= 1; a2= 1; a3=  1; break;
	}

	if( !mapp(wp->query("weapon_mp/weapon_prop")) ) {
		set_temp("busy", 1);
		call_out("delete_temp", 1, "busy");
		return 1;
	}
	else {
		mp = wp->query("weapon_mp/weapon_prop");
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

	if( wp->query("poison_number") ) str+="毒性："+make_info(wp->query("poison_number"),a3)+"\n	      ";
	if( !undefinedp(mp["attack"]) ) str+="增加命中："+make_info(mp["attack"],a1)+"\n	      ";
	if( !undefinedp(mp["parry"]) ) str+="增加防御："+make_info(mp["parry"],a2)+"\n	      ";
	if( !undefinedp(mp["dodge"]) ) str+="增加躲避："+make_info(mp["dodge"],a2)+"\n	      ";
	if( !undefinedp(mp["armor_vs_force"]) ) str+="增加内功抗性："+make_info(mp["armor_vs_force"],a2)+"\n	      ";

	command(str);

	set_temp("busy", 1);
	call_out("delete_temp", 1, "busy");
	return 1;
}
//******************************************************************
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

	if (!arg) return notify_fail(CYN"铸剑师说道：你要查询什么兵器？\n"NOR);
	if (me->query_busy()) return notify_fail(CYN"铸剑师说道：你正忙着呢！\n"NOR);
	if ( !objectp(wp = present(arg, me)) )
		return notify_fail("你身上没有这件兵器。\n");
	if ( !wp->query("imbued") )
		return notify_fail(CYN"铸剑师说道：你只能查询玩家制造的兵器。\n"NOR);
	if ( wp->query("imbued")<=3 )
		return notify_fail(CYN"铸剑师说道：你的这件兵器不需要更新。\n"NOR);
	if ( (int)query("ask_user/"+me->query("id"))>0 )
		return notify_fail(CYN"铸剑师说道：你刚刚查询过，怎么又来了！\n"NOR);

	WORKER_D->update_UniqueValues("querytime","save_id="+wp->query("weapon_mp/save_id"),"time-"+wp->query("weapon_mp/save_id"));
	tell_object(me,CYN"铸剑师说道：好吧，我帮你打听一下，请稍等......\n"NOR);

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
	t = WORKER_D->query_UniqueValues("querytime","time-"+wp->query("weapon_mp/save_id"));
	tell_object(me,CYN"铸剑师说道：你的兵器编号为："+wp->query("weapon_mp/save_id")+"。\n"NOR);
	if(t<0)
		tell_object(me,CYN"铸剑师说道：我也不知道更新时间，可能的原因是：网络错误，无法连接同步服务器。\n"NOR);
	else
		tell_object(me,CYN"铸剑师说道：你的这件兵器的更新时间为："+CHINESE_D->chinese_date(t,1)+"。\n"NOR);
	set("ask_user/"+me->query("id"),1);
}

void tell_limit(object me)
{
	int v1,v2,v3,v4;	

	if (!objectp(me) || !userp(me)) return;

	v1 = WORKER_D->query_UniqueValues("queryweapon","");
	v3 = WORKER_D->query_UniqueValues("queryweapon2","");
	if(v1<0) {
		tell_object(me,CYN"铸剑师说道：我也打听不到呀！可能的原因是：网络错误，无法连接同步服务器！\n"NOR);
	}
	else {
		v2 = v1 / 0x10000;
		v1 = v1 % 0x10000;
		v4 = v3 / 0x10000;
		v3 = v3 % 0x10000;
		tell_object(me,CYN"铸剑师说道：听说现在整个江湖中共有"+CHINESE_D->chinese_number(v2)+"件头等兵器，"+CHINESE_D->chinese_number(v4)+"件二等兵器。\n"NOR);
		tell_object(me,CYN"铸剑师说道：听说整个江湖最多可能有"+CHINESE_D->chinese_number(v1)+"件头等兵器，"+CHINESE_D->chinese_number(v3)+"件二等兵器。\n"NOR);
	}
	///////////////////////////////////
	int delay;
	object me = this_player();

	if( WORKER_D->update_UniqueValues("queryweapon","","") > 1 ) {
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
