// yaodian zhanggui
// by augx@sj  3/4/2002

// Modified by mxzhao 2004/03/06

#include <ansi.h>
inherit NPC;

string ask_job();
string ask_fail();
string ask_reward();
string ask_limit();
int ask_liaoshang();
static string yao_path = "/d/zhiye/obj/yao";

void add_yao(object yao)
{
	add("vendor_goods", ({
		([	"name"	: yao_path,
			"number": 99,
			"short"	: yao->short(),
			"value"	: yao->query("value")/2,
			"id"	: yao->query("id"),
		]),
	}));
}


void create()
{
	object yao,me;

	set_name("药店掌柜",({ "yaodian zhanggui","zhanggui" }) );
	set("gender", "男性" );
	set("age", 51);
	set("long","这是位和蔼可亲的老人家，在此买卖药品已经多年了。");

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
	set_skill("liandan", 220);

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
 		CYN"药店掌柜说道：「最近没有什么好的药物啊。」\n"NOR,
 		CYN"药店掌柜四顾着说道：「你们需不需要学习炼丹技术？可以向我学习啊，学费么.....嘿嘿」\n"NOR,
 		CYN"药店掌柜说道：「我现在太忙了走不开，谁能帮我把客人定制的药品所过去？」\n"NOR,
 	}) );

 	set("no_ansuan",1);
 	set("no_bark",1);
    set("inquiry", ([
		"疗伤" : (:ask_liaoshang:),//for newbie
    ]));
/*
	set("inquiry", ([
		"工作"		: (: ask_job :),
		"job"		: (: ask_job :),
		"运送"		: (: ask_job :),
		"放弃"		: (: ask_fail :),
		"abandon"	: (: ask_fail :),
		"完成"		: (: ask_reward :),
		"报酬"		: (: ask_reward :),
		"reward"	: (: ask_reward :),
		"丹药"		: (: ask_limit :),
		"drug"		: (: ask_limit :),
	]));
*/
	set("vendor_goods", ({ }));
	me = this_object();

	// add yao
	yao = new(yao_path);
	yao->set_type("yangjing dan",me->query_skill("liandan",1),5000,sprintf("zhanggui%dd",time()),me,0);
	add_yao(yao); destruct(yao);

	yao = new(yao_path);
	yao->set_type("buqi wan",me->query_skill("liandan",1),5000,sprintf("zhanggui%dd",time()),me,0);
	add_yao(yao); destruct(yao);

	yao = new(yao_path);
	yao->set_type("xujing dan",me->query_skill("liandan",1),5000,sprintf("zhanggui%dd",time()),me,0);
	add_yao(yao); destruct(yao);

	yao = new(yao_path);
	yao->set_type("neixi wan",me->query_skill("liandan",1),5000,sprintf("zhanggui%dd",time()),me,0);
	add_yao(yao); destruct(yao);

	yao = new(yao_path);
	yao->set_type("bushi dan",me->query_skill("liandan",1),5000,sprintf("zhanggui%dd",time()),me,0);
	add_yao(yao); destruct(yao);

	yao = new(yao_path);
	yao->set_type("bushui wan",me->query_skill("liandan",1),5000,sprintf("zhanggui%dd",time()),me,0);
	add_yao(yao); destruct(yao);

	yao = new(yao_path);
	yao->set_type("jinchuang yao",me->query_skill("liandan",1),5000,sprintf("zhanggui%dd",time()),me,0);
	add_yao(yao); destruct(yao);

	yao = new(yao_path);
	yao->set_type("liaojing dan",me->query_skill("liandan",1),5000,sprintf("zhanggui%dd",time()),me,0);
	add_yao(yao); destruct(yao);

	yao = new(yao_path);
	yao->set_type("zhengqi dan",me->query_skill("liandan",1),10000,sprintf("zhanggui%dd",time()),me,0);
	add_yao(yao); destruct(yao);

	yao = new(yao_path);
	yao->set_type("xieqi wan",me->query_skill("liandan",1),10000,sprintf("zhanggui%dd",time()),me,0);
	add_yao(yao); destruct(yao);

	yao = new(yao_path);
	yao->set_type("yannian dan",me->query_skill("liandan",1),20000,sprintf("zhanggui%dd",time()),me,0);
	add_yao(yao); destruct(yao);

	yao = new(yao_path);
	yao->set_type("fuling wan",me->query_skill("liandan",1),20000,sprintf("zhanggui%dd",time()),me,0);
	add_yao(yao); destruct(yao);

	yao = new(yao_path);
	yao->set_type("danggui dan",me->query_skill("liandan",1),20000,sprintf("zhanggui%dd",time()),me,0);
	add_yao(yao); destruct(yao);

	yao = new(yao_path);
	yao->set_type("chuanbei wan",me->query_skill("liandan",1),20000,sprintf("zhanggui%dd",time()),me,0);
	add_yao(yao); destruct(yao);

	yao = new(yao_path);
	yao->set_type("chantui yao",me->query_skill("liandan",1),20000,sprintf("zhanggui%dd",time()),me,0);
	add_yao(yao); destruct(yao);

	yao = new(yao_path);
	yao->set_type("huoxue dan",me->query_skill("liandan",1),20000,sprintf("zhanggui%dd",time()),me,0);
	add_yao(yao); destruct(yao);

	yao = new(yao_path);
	yao->set_type("jiedu wan",me->query_skill("liandan",1),20000,sprintf("zhanggui%dd",time()),me,0);
	add_yao(yao); destruct(yao);

	// test
	/*
	yao = new(yao_path);
	yao->set_type("huangqi dan",me->query_skill("liandan",1),20000,sprintf("zhanggui%dd",time()),me,0);
	add_yao(yao); destruct(yao);
	*/

	setup();
	carry_object(ARMOR_D("cloth"))->wear();
	carry_object(__DIR__"../obj/yaochui")->wield();
	add_money("coin",99);
}
//for newbie
int ask_liaoshang()
{
	object ob;
	ob=this_player();
	if(ob->query("combat_exp")<3000) 
	{
		command("? "+ob->query("id"));     
		command("say 你怎么从武馆出来了啊，这外面多危险啊！\n");
		command("poor "+ob->query("id"));
		return 1;
	}
	if(ob->query("combat_exp")>50000) 
	{
		command("? "+ob->query("id"));     
		command("say 疗伤？花钱买药啊。公平共价，童叟无欺！\n");
		command("knock "+ob->query("id"));
		return 1;
	}
	if (ob->query("eff_qi") >= ob->query("max_qi"))
	{
		command("? "+ob->query("id"));     
		command("say 调皮蛋，跑我这里添乱！\n");
		command("smile "+ob->query("id"));
		return 1;
	}
	else 
	{
		message_vision("$n对着$N嚷道：“老板叔叔，我受伤了，好痛啊......”\n", this_object(),ob);
		command("poor "+ob->query("id"));
		command("pat "+ob->query("id"));
		command("say 好吧，看你一个小孩子出来闯荡江湖，也可怜的紧。");
                message_vision("$N拿出一根银针轻轻捻入$n受伤部位附近的穴道，$n感觉舒服多了。\n", this_object(),ob);
		ob->start_busy(3);
		call_out("recover", 5, ob);
		return 1;
	}
}
void recover(object ob)
{
	if (!ob || !present(ob)) return;
	if (environment(ob) != environment(this_object())) return;
	ob->set("eff_qi", (int)ob->query("max_qi"));
	//ob->set("eff_jing", (int)ob->query("max_jing"));  
	message_vision("一柱香的工夫过去了，$N觉得伤势已经基本痊愈了。\n",ob);
	command("say 江湖险恶,不小心的话，下次你就没这么幸运了。\n");
	command("pat "+ob->query("id"));
}
void kill_ob(object ob)
{
	object me = this_object();
	command("!!!");
	command("say 我和你无怨无仇，为何要害我？");
	me->remove_enemy(ob);
	ob->remove_killer(me);
	return;
}

int accept_fight(object ob)
{
	command("say 我和你无怨无仇，为何要害我？");
	return 0;
}

void init()
{
	::init();
	add_action("do_buy", "buy");
	add_action("do_list", "list");
	add_action("do_sell", "sell");
}
/*
int prevent_learn(object me, string skill)
{
	if ( skill=="liandan" && me->query_skill("liandan",1)<=50 )
		return 0;
	else
		return 1;
}
*/
int is_apprentice_of(object ob)
{
	return 1;
}
/*
int recognize_apprentice(object ob)
{
	int money = 150, level = ob->query_skill("liandan", 1);

	if (level > 9) money = 300;
	if (level > 19) money = 600;
	if (level > 29) money = 1200;
	if (level > 39) money = 2400;

	switch (MONEY_D->player_pay(ob, money)) {
		case 0:
		case 2: tell_object(ob,"你现在的学费是每次"+MONEY_D->money_str(money)+"。你的钱不够！\n" ); return 0;
	}
	return 1;
}
*/

////////////////////////////////////////////////////////////////////
int query_goods_value(int value , int number , string sid)
{
	object me = this_player();
	int trade_level;
	int cur;

	trade_level = me->query_skill("trade", 1) / 10;
	if (trade_level > 15) trade_level = 15;
	value = value * (115 - trade_level) / 100;

	cur = query_temp("vendor_goods/"+sid);
	if( cur != number )
		value += value/6 * cur/number;

	return value;
}

string is_vendor_good(string arg)
{
	mapping *goods;
	int i;

	if (arrayp(goods = query("vendor_goods")))
		for (i = 0; i < sizeof(goods); i++)
			if( goods[i]["id"]==arg && goods[i]["number"]!=query_temp("vendor_goods/"+goods[i]["id"]) )
				return goods[i]["name"];
	return "";
}

void reset()
{
	this_object()->delete_temp("vendor_goods");
}

int do_list(string arg)
{
	mapping *goods;
	int i;

	if (!living(this_object()))
		return notify_fail("还是等对方醒来再说吧。\n");
	if (!(int)query("move_seller") && base_name(environment()) != query("startroom"))
		return notify_fail("这……我这次是出来游玩的，你下次来吧。\n");

	if (arrayp(goods = query("vendor_goods")))
	{
		write(query("name") + "目前出售以下货物：\n");
                write("┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓\n");
		write("┃  货  物                          价  格                          库存/总量 ┃\n");
		write("┣━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫\n");
		for (i = 0; i < sizeof(goods); i++)
			write(sprintf("┃%-30s%-36s%4d/%4d ┃\n", goods[i]["short"],
				MONEY_D->price_str(query_goods_value(goods[i]["value"],goods[i]["number"],goods[i]["id"])),
				goods[i]["number"]-query_temp("vendor_goods/"+goods[i]["id"]),
				goods[i]["number"])
			);
		write("┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛\n");
		return 1;
	}
}

int do_buy(string arg)
{
	int i,j,improve;
	string ob_file;
	object ob;
	object me = this_player();
	mapping goods;

	if (!living(this_object()))
		return notify_fail("还是等对方醒来再说吧。\n");
	if (me->is_busy() || me->is_fighting())
		return notify_fail("你正忙着呢。\n");
	if (query_temp("busy"))
		return notify_fail("哟，抱歉啊，我这儿正忙着呢……您请稍候。\n");
	if (!(int)query("move_seller") && base_name(environment()) != query("startroom"))
		return notify_fail("这……我这次是出来游玩的，你下次来吧。\n");
	if (!arg) return notify_fail("你想买什么？\n");
	if ((ob_file = is_vendor_good(arg)) == "")
		return notify_fail("你想买的东西我这里没有。\n");

	goods = query("vendor_goods");
	for(i=0;i<sizeof(goods);i++)
		if(goods[i]["id"]==arg) break;

	ob = new(ob_file);
	ob->set_type(goods[i]["id"],this_object()->query_skill("liandan",1),goods[i]["value"],sprintf("zhanggui%dd",time()),this_object(),0);

	switch(MONEY_D->player_pay(me,j = query_goods_value(goods[i]["value"],goods[i]["number"],goods[i]["id"]))) {
	case 0:
		destruct(ob);
		return notify_fail("穷光蛋，一边呆着去！\n");
	case 2:
		destruct(ob);
		return notify_fail("您的零钱不够了，银票又没人找得开。\n");
	default:
		if (!ob->move(me)) {
			destruct(ob);
			return 0;
		}
		message_vision("$N以"+MONEY_D->price_str(j)+"的价格从$n那里买下了一" + ob->query("unit") + ob->name() + "。\n", me, this_object());
		improve = 40 - random(30);
		me->improve_skill("trade", improve + random(improve));
		add_temp("vendor_goods/"+goods[i]["id"],1);
	}
	set_temp("busy", 1);
	call_out("delete_temp", 1, "busy");

	return 1;
}


int query_goods_value2(object goods)
{
	object me = this_player();
	int value, trade_level;

	value = goods->query("value");
	trade_level = me->query_skill("trade", 1) / 10;
	if (trade_level > 15) trade_level = 15;
	value = value * (80 + trade_level) / 100;

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
		return notify_fail("药店掌柜说道：你正忙着呢。\n");
	if (!arg || !(ob = present(arg, me)))
		return notify_fail("药店掌柜说道：你要卖什么？\n");
	if (query_temp("busy"))
		return notify_fail("药店掌柜说道：哟，抱歉啊，我这儿正忙着呢……您请稍候。\n");
	if ( !(int)ob->query("imbued") || !(int)ob->query("yao") || !(int)ob->query("user_make") )
		return notify_fail("药店掌柜说道：我只收购玩家炼制的丹药。\n");

	value = query_goods_value2(ob);
	message_vision("$N以"+MONEY_D->price_str(value)+"的价格卖掉了一"+ob->query("unit")+ob->name() + "给$n。\n", me, this_object());
	ob->delete_drug(me);
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
	object me = this_player() ,room ,oroom ,lastroom ,target, *living , obj;
	int lv1,lv2,i,j,k,a,b;
	string * exits;

	lv1 = (int)me->query_skill("tilian",1);
	lv2 = (int)me->query_skill("liandan",1);

	if ( environment(me)->query("no_zhiye"))
		return "我这里没什么活需要做。";
	if( lv1<10 || lv2 <10 )
		return "你的技能太差了，我可不放心让你去办事。";
	if( lv1>=395 || lv2>=395 )
		return "您的水平那么高了，我可不敢再差遣您了。";
	if( !wizardp(me) && me->query("worker/djob") && time()<(int)me->query("worker/djob_time") )
		return "我不是叫你把丹药送给"+me->query("worker/djob_oroom")+"附近的"+me->query("worker/djob")+"了吗！";
	if( !wizardp(me) && time()<(int)me->query("worker/djob_time") )
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
		target = new(__DIR__"customer3");
		target->set("player",me->query("id"));
		target->move(room);
	}

	obj = new("/d/zhiye/obj/danyao");
	obj->move(me);

	me->set("worker/djob",1);
	me->set("worker/djob_oroom",get_place(base_name(oroom))+oroom->query("short"));
	me->set("worker/djob_far",j);
	me->set("worker/djob_finish",0);
	me->set("worker/djob_target",target->query("name"));
	me->set("worker/djob_room",base_name(room));
	me->set("worker/djob_time",time()+20*60);
	if(wizardp(me) && me->query("env/test"))
		tell_object(me,base_name(oroom)+" -> "+base_name(room)+"\n");

	command("pat "+me->query("id"));
	if (j)
		command("whisper "+me->query("id")+" 快帮我把丹药送给"+me->query("worker/djob_target")+"，他在"
			+me->query("worker/djob_oroom")+"周围方圆"+CHINESE_D->chinese_number(me->query("worker/djob_far"))+"里之内。" );
	else
		command("whisper "+me->query("id")+" 快帮我把丹药送给"+me->query("worker/djob_oroom")+"的"
			+me->query("worker/djob_target")+"。");
	message_vision(CYN"药店掌柜给$N一颗刚刚炼制好的丹药。\n"NOR, me);
	return "好好干活，我不会亏待你的。";
}

string ask_fail()
{
	object me = this_player() , obj;

	if( !me->query("worker/djob") 
	|| time()>=(int)me->query("worker/djob_time")
	|| environment(me)->query("no_zhiye"))
		return "你要放弃什么？";

	me->delete("worker/djob");
	me->delete("worker/djob_oroom");
	me->delete("worker/djob_far");
	me->delete("worker/djob_target");
	me->delete("worker/djob_room");
	me->delete("worker/djob_finish");

	if ( objectp(obj = present("dan yao", me)) ) {
		message_vision(CYN"$N把丹药还给铸剑师。\n"NOR, me);
		destruct(obj);
		me->set("worker/djob_time",time()+(2+random(3))*60);
		command("disapp " + me->query("id"));
		return "没想到这么简单的事情你都做不了！";
	}
	else {
		command("slash " + me->query("id"));
		me->set("worker/djob_time",time()+(10+random(10))*60);
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
	int improve1,improve2,lv1,lv2,exp;

	if( !me->query("worker/djob")
	|| environment(me)->query("no_zhiye"))
		return "你又没给我干活，要什么报酬！";

	if( !me->query("worker/djob_finish") )
		return "你还没有完成任务呢，要什么报酬！";

	me->delete("worker/djob");
	me->delete("worker/djob_oroom");
	me->delete("worker/djob_target");
	me->delete("worker/djob_far");
	me->delete("worker/djob_room");
	me->delete("worker/djob_finish");
	me->set("worker/djob_time",time()+ 20 + random(30));

	lv1 = (int)me->query_skill("tilian",1) / 2;
	improve1 = (300 + lv1) * 15/10 /2;
	improve1 = improve1*2/3 + random(improve1*2/3);

	lv2 = (int)me->query_skill("liandan",1) / 2;
	improve2 = (300 + lv2) * 13/10 /2;
	improve2 = improve2*2/3 + random(improve2*2/3);

	exp = (improve1+improve2) / 30;
	me->improve_skill("tilian", improve1);
	me->improve_skill("liandan", improve2);
	me->add("combat_exp", exp);

	command("applaud " + me->query("id"));
	tell_object(me, HIC"药房掌柜仔细向你讲解提炼和炼丹的诀窍，你的提炼技术提升了"+CHINESE_D->chinese_number(improve1)
			+"点，你的炼丹技术提升了"+CHINESE_D->chinese_number(improve2)+"点，你获得了"+CHINESE_D->chinese_number(exp)+"点经验！\n"NOR);
	log_file("job/drugjob",sprintf("%s(%s)：提炼：%d，炼丹：%d，经验：%d；现exp：%d\n", me->name(), me->query("id"),improve1,improve2,exp,me->query("combat_exp")),me);

	return "好样的，干活真利索，以后有空还来我这里干活啊。";
}

/////////////////////////////////////////////////////////////////////

string ask_limit()
{
	int delay;
	object me = this_player();

	if( WORKER_D->update_UniqueValues("querydrug","","") > 1 ) {
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

void tell_limit(object me)
{
	int v1,v2,v3,v4;

	if (!objectp(me) || !userp(me)) return;

	v1 = WORKER_D->query_UniqueValues("drug","");
	v3 = WORKER_D->query_UniqueValues("drug2","");
	if(v1<0) {
		tell_object(me,CYN"药店掌柜说道：我也打听不到呀！可能的原因是：网络错误，无法连接同步服务器！\n"NOR);
	}
	else {
		v2 = v1 / 0x10000;
		v1 = v1 % 0x10000;
		v4 = v3 / 0x10000;
		v3 = v3 % 0x10000;
		tell_object(me,CYN"药店掌柜说道：听说这段时间内，整个江湖中已经炼制出"+CHINESE_D->chinese_number(v2)+"颗头等丹药，"+CHINESE_D->chinese_number(v4)+"颗二等丹药。\n"NOR);
		tell_object(me,CYN"药店掌柜说道：听说这段时间内，整个江湖最多可能炼制"+CHINESE_D->chinese_number(v1)+"颗头等丹药，"+CHINESE_D->chinese_number(v3)+"颗二等丹药。\n"NOR);
	}
}
