// loseletter.c
//
// Created by mxzhao 2004/03/22

#include <ansi.h>
#include <job_mul.h>
#include <mudlib.h>
#include <get_place.h>

inherit ITEM;

#define BASE_EXP	400
#define BASE_POT	100

#define RAND_EXP	400
#define RAND_POT	100
#define RAND_MONEY	50

#define GET_EXP(exp)		(exp = (BASE_EXP + random(RAND_EXP)))
#define GET_POT(pot)		(pot = (BASE_POT + random(RAND_POT)))
#define GET_ITEM(obj)		(obj = new(bonus_items[random(sizeof(bonus_items))]))

string do_look();
void dest();
private int set_addressee();
private int do_send(string arg);
private int do_drop(string arg);
private int do_mark(string arg);
private int do_check(string arg);

string *bonus_items = ({
	"/d/zhiye/obj/jtp", 
});

string make_code()
{
	int i = 8, j;
	string id = "";
	string seed = "1AaBb2CcDd3EeFf4GgHh5IiJj6KkLl7MmNn8OoPp9QqRr0SsTtUuVvWwXxYyZz";
	
	while (i--) {
		j = random(62);
		id += seed[j..j];
	}
	return id;
}

void create()
{
	set_name(YEL"失落的信笺"NOR, ({ "letter", "xin", "lose letter" }));

	set_weight(10);

	if(clonep())
	{
		set_default_object(__FILE__);
	}
	else
	{
		set("unit", "封");
		set("value", 0);
		set("material", "paper");
		set("no_drop",1);
	}

	set("long", (: do_look :));
	

	setup();
}

void init()
{
	object me = this_player();
	if (environment() == me && !query("add_set")){
		set_addressee();
		set("add_set",1);
		if (!query("bearer")){
			message_vision("$N得到了一封" + name(1) + "。\n", me);
			set("bearer", me->query("id"));
		}
			
		if (me->query("id") == query("bearer")){
			if (me->query("id") == query("addressee"))
				set_addressee();
			if (query("addressee"))
				add_action("do_send", "sendto");
				add_action("do_mark", "mark");	
				add_action("do_check", "chakan");	
				if (me->query("ll/last_day")!=time()/86400)	{
					me->set("ll/last_day",time()/86400);
					me->set("ll/last_count",me->query("ll/count"));
					me->delete("ll/count");
					me->delete("ll/get");
				}
				me->add("ll/get",1);					
		}
	
		add_action("do_drop", "discard"); 
		
  }
}

int do_check(string arg) {
	
	if (arg != "letter")
	  return notify_fail("你要查看什么东西。\n");
	
	if (query("checked")) 
		return notify_fail("信封外层的火漆已经除去。\n");

	set("checked", 1);
	call_out("dest", random(5)*30 + 300);
	return notify_fail("你刮开信封外层的火漆。\n");
	
}

int do_mark(string arg) {
	object me=this_player();
	
	if (!query("addr_name")) 
		return notify_fail("你觉得有点无聊，就在信封上画了几个叉叉。\n");
	
	if ( query("addr_name") != arg ){
		add("err_times",1);
		if (query("err_times")>4) {
			me->add_busy(150);
			call_out("dest", 1);
			return notify_fail("想作弊？嘿嘿，慢慢发呆吧！\n"); 
		}
		else {
			me->add_busy(3);
			return notify_fail("你再看清楚一点。\n");
		}
	}
	else {
		set("mark_ok",1);
		return notify_fail("你在信卦上写上收信人的名字。\n");
	}
}

string delansi(string str)
{
   int is_ansi,ansi_len,i;
   //string ruslt;
   is_ansi=0;
   ansi_len=0;
   
   for (i=0;i<strlen(str);i++){     
     if (str[i]==27)
    	 is_ansi=1;
     if (str[i]==109 && is_ansi==1){ 
     	 str[i]=37;
     	 is_ansi=0;
     }
     if (is_ansi==1){
     	 ansi_len++;
       str[i]=37;
     }     
   }
	 return replace_string(str,"%","");	
}

string do_look()
{
	object ob;
	string place_name;
	string str = "这是一封大理国镇南王府送出的书信。\n"
		"信封纸色蜡黄，信舌用火漆封着。\n";
  
	string target = query("addressee");	
	
	if (!query("checked")) 
		return "信封被两层火漆给封着，要查看收信人请先查看信件(chakan letter)。\n";
		
	if (!target) 
		return str += "信封上的字迹模糊不清，不知何人遗落到此处。\n";


		
	ob = find_player(target);
	
	if (objectp(ob)) {
		place_name = get_place(file_name(environment(ob)));
	  if (this_player()->query_condition("true_view"))
	  place_name += environment(ob)->query("short");
	}		
	
	if (target && place_name)
	{
			str += "信封上写着：" + ob->short(1) 
				+ "\n看来是很重要的信笺，要抓紧时间把它送去(sendto)。\n"
				+ "好象收信人曾在" + HIY + place_name + NOR +"一带出现。\n";
	}
	else
	{
		str += "信封上的字迹模糊不清，不知何人遗落到此处。\n";
	}

	return str;
}

private int set_addressee()
{
		object *list;
		object ob;
		string str; 
		string code=make_code();		
		object me=this_player();
		
		if (!me || !userp(me)) {
			destruct(this_object());
			return 0;
		}
		
		list = filter_array(users(), 
		(: $1->query("combat_exp") > 100000
		&& !wizardp($1)
		&& $1->query("name")
		&& environment($1)
		&& query_ip_number($1) != $(query_ip_number(this_player()))
		&& $1->query_condition("job_busy") > 0
		&& time()-($1->query_temp("last_go")) < 3 
		&& !($1->query_condition("smy_job"))
		:)); 

	list -= ({ me });

	if(sizeof(list) == 0)
	{
		delete("addressee");
		return 0;
	} 

	ob = list[random(sizeof(list))];
	if (!ob)	return 0;
	set("addressee", ob->query("id"));
	set("addr_name", ob->query("name"));
  set("owner",me->query("id"));
  me->set_temp("ll_code",code);
  if (ob->query("title"))
  	str = delansi(ob->query("title"));
  else
    str = "普通百姓";
  str+= " "+ob->query("name");
  str+= ob->query("nickname")?"「"+delansi(ob->query("nickname"))+"」":"";

	return 1;
}


private int do_drop(string arg) {
		object me=this_player();		
		if ( !arg || arg!="letter") 
			return notify_fail("你要丢什么？\n");
		remove_call_out("dest");
		me->set("drop_loseletter",time());
		dest();	
		return 1;
}

private int do_send(string arg)
{
	int exp, pot;
	int obexp, obpot, obmoney;
	int i,to_rb,times;
	string itemp;

	object ob, obj;
	object me = this_player();

	if ( !arg || !objectp(ob = present(arg, environment(me)))) {
		return notify_fail("你要送给谁？\n");
	}	

	if ( me->is_busy() || me->is_fighting()){
		return notify_fail("你正忙着呢。\n");
	}	

	if ( query("addressee") != arg ){
		add("err_times",1);
		if (query("err_times")>4) {
			me->add_busy(150);
			return notify_fail("想作弊？嘿嘿，慢慢发呆吧！\n"); 
		}
		else {
			me->add_busy(3);
			return notify_fail("这封信不是送给这个人的。\n");
		}
	}
	
	if ( !living(ob) )
		return notify_fail("看清楚点，那是活人吗？！\n");

	if ( !userp(ob) )
		return notify_fail("嘿嘿，想作弊？！\n"); 
	
	if (ob->is_busy()) {
		return notify_fail(ob->name()+"正忙着呢。\n");
	}
	
	//if (base_name(environment(me))[0..11]=="/d/xiangyang") to_rb=1;//((base_name(environment(me))[0..6]=="/d/city") 	 
	if( ob->query("ll/last_count") < 5 ) to_rb = 1;
	
	me->add_busy(random(2));
	
	message_vision("$n将" + name() + "交给了$N。\n", ob, me);
	

	times = me->query("job_time/丢失的信") + 1;
	
	if (times%50) 
		i = 20;
	else 
		i = 2000;
	
	if (me->query("registered") == 3)
	  i = i*6/5;
	  
	if (me->query("registered") > 3)
		i = i*7/5;
		
	if (INTERMUD_MUD_NAME != "tj" && !random(10000/i)){
		message_vision("$N狐疑地看着$n：「这信怎么落到你的手上？」\n"
			"$N上下打量了$n两眼：「你当真没有偷拆来看过？" 
			+ RANK_D->query_self(ob) + "可是有隐私权的。」\n", ob, me);
		
		EMOTE_D->do_emote(ob, "liar");

		message_vision("$N越想越生气，暴打了$n一顿，打得$n遍体鳞伤。\n", ob, me);

		GET_EXP(exp);

		exp = random(exp/10);

		tell_object(me, "你被" + ob->name() + "打成重伤，经验下降了" 
			+ chinese_number(exp) + "点。\n");

		exp = -exp;

		me->add_exp_combat(exp, ob, "丢失的信");
		me->add("quest/chance",1);
		me->add("quest/chance_times",1);		
		EMOTE_D->do_emote(me, "wuwu", me->query("id"));
		CHANNEL_D->do_channel(me, "rumor", "据说"+me->name()+"被"+ob->name()+"打傻了!");
		log_file("quest/chance",
			sprintf("%s(%s)第%d次完成失落信笺任务时，被%s(%s)暴打得到解迷机会1次，现共有%d次。\n",
				me->query("name"),
				me->query("id"),
				me->query("job_time/丢失的信"),
				ob->query("name"),
				ob->query("id"),
				me->query("quest/chance")
		), 
		me );
	}
	else
	{
		EMOTE_D->do_emote(ob, "touch", me->query("id"));

		message_vision("过了一会儿，$N才说道：「我等侯段王爷的消息多时，"
			"正在踌躇为何音讯全无，真是多谢这位" 
			+ RANK_D->query_respect(me) + "。」\n", ob, me);
		
		switch (random(9)%5){
		case 4:
			message_vision("$N拆开信笺，皱眉读道：「您给大理国所投简历，"
				"已经我大理人事局第一次筛选而拒绝！\n"
				"您妄图行贿大理人事领导的行为已被彻查，贿物送回，"
				"我国保留追究法律责任权力……」\n", ob);
			
			EMOTE_D->do_emote(ob, "wait");


			message_vision("$N恶狠狠得瞪了$n一眼，气鼓鼓的把信笺内封的贿赂用品『" 
				+ HIW + "精铁魄" + NOR + "』扔在地上。\n", ob, me);

			GET_EXP(obexp);
			GET_POT(obpot);
			//ob->add("combat_exp", obexp);
			obexp = ob->add_exp_combat(obexp, me, "丢失的信", 1);
			ob->add("potential", obpot);

			tell_object(ob, "你行贿求职失败，卧薪尝胆，重练武学，获得了" 
				+ chinese_number(obexp) + "点经验，" + chinese_number(obpot) + "点潜能。\n");

			exp = random(50)+100;
			pot = random(30)+30;
			exp = me->add_exp_combat(exp, ob, "丢失的信");
			me->add("potential", pot);
			

			tell_object(me, "你因为自己拾「信」不昧，获得了" 
				+ chinese_number(exp) + "点经验，" + chinese_number(pot) + "点潜能。\n");			
				
			tell_object(me, "你捡到了" + ob->name() + "扔掉的『" + HIW + "精铁魄" + NOR + "』收进背囊。\n");

			me->add("collection/l1/jingtie po",1);
			break;
		case 3:
			message_vision("$N拆开信笺看了看，大喜道：「我因为送信勤恳，"
				"已经被评选为大理年度劳动模范了！」\n", ob);

			EMOTE_D->do_emote(ob, "touch");

			message_vision("$N说道：「多谢这位" + RANK_D->query_respect(me) 
				+ "送信给我。」\n", ob);

			GET_EXP(exp);
			GET_POT(pot);
			
			exp = me->add_exp_combat(exp, ob, "丢失的信");			
			me->add("potential", pot);            
		
			if(!to_rb) {
				GET_EXP(obexp);
				GET_POT(obpot);
				obexp = ob->add_exp_combat(obexp, me, "丢失的信", 1);
				ob->add("potential", obpot);
				tell_object(ob, "你被评选为大理年度劳动模范，获得了" 
					+ chinese_number(obexp) + "点经验，" + chinese_number(obpot) + "点潜能。\n");
			}
			
			tell_object(me, "你心想自己拾「信」不昧，在此基础上再加把劲，"
				"来年兴许也能评为光荣的劳动模范，不由精神大为振奋。\n"
				"你获得了" + chinese_number(exp) + "点经验，" 
				+ chinese_number(pot) + "点潜能。\n");
			break;
		case 2:
			if (to_rb) {
				obmoney = 10000;
				itemp = "五十";
			}
			else if (!random(1000)){
				itemp="特";
				obmoney = (1000+random(200))*10000;
				CHANNEL_D->do_channel(ob, "rumor", "据说"+ob->name()+"中了大理福利彩票"+HIR+"特等奖"+HIM+"!");
			}
			else if (!random(250)){
				itemp="一";
				obmoney = (500+random(100))*10000;
				CHANNEL_D->do_channel(ob, "rumor", "据说"+ob->name()+"中了大理福利彩票"+HIR+"一等奖"+HIM+"!");
			}
			else if(!random(100)){
				itemp="二";
				obmoney = (250+random(40))*10000;
				CHANNEL_D->do_channel(ob, "rumor", "据说"+ob->name()+"中了大理福利彩票"+HIR+"二等奖"+HIM+"!");
			}
			else if(!random(40)){
				obmoney = (100+random(20))*10000;
				itemp="三";
				CHANNEL_D->do_channel(ob, "rumor", "据说"+ob->name()+"中了大理福利彩票"+HIR+"三等奖"+HIM+"!");
			}
			else if(!random(16)){
				obmoney = (50+random(10))*10000;
				itemp="四";
			}
			else {
				obmoney = (10+random(10))*10000;
				itemp="五";
			}	

			message_vision("$N拆开信笺看了看，喜道：「我在大理国购买的福利彩票中了" 
				+ itemp + "等奖啦！！！」\n", ob, me);
			EMOTE_D->do_emote(ob, "yeah");


			ob->add("balance", obmoney);

			GET_EXP(exp);
			GET_POT(pot);				
					
			exp = me->add_exp_combat(exp, ob, "丢失的信");			
			me->add("potential", pot);
			
			tell_object(ob, "你获得了大理国福利彩票滚动奖金" 
				+ MONEY_D->money_str(obmoney) + "，奖金已汇入你的账户。\n");

			tell_object(me, "你因为自己拾「信」不昧，获得了" 
				+ chinese_number(exp) + "点经验，" + chinese_number(pot) + "点潜能。\n");
			break;
		case 1:
		case 0:
			message_vision("$N拆开信笺，皱眉读道：「您给大理国所投简历，"
				"已经我大理人事局第一次筛选而拒绝！\n"
				"您妄图行贿大理人事领导的行为已被彻查，贿物送回，"
				"我国保留追究法律责任权力……」\n", ob);
			
			EMOTE_D->do_emote(ob, "wait");

			if(!to_rb) {
				if(!random(300)){ 
					obj = new("/clone/gift/imbue");
				}
//				if(!random(200)){
//					obj = new("/d/zhiye/obj/book");
//					obj->set_level(random(2)+1);
//				}
//				else if(!random(100)){
//				  obj = new("/d/gumu/qqll/qqllyu");
//				}
				else
				  obj = new("/clone/gift/danwan");
				
				obj->move(ob);
				message_vision("$N恶狠狠得瞪了$n一眼，气鼓鼓的把信笺内封的贿赂用品『" 
											+ obj->name() + "』塞回怀里。\n", ob, me);
			}
			
			GET_EXP(exp);
			GET_POT(pot);
			
			exp = me->add_exp_combat(exp, ob, "丢失的信");
			me->add("potential", pot);
			

			tell_object(me, "你因为自己拾「信」不昧，获得了" 
				+ chinese_number(exp) + "点经验，" + chinese_number(pot) + "点潜能。\n");			
			break;
		}
	}
 
  me->set_day_flag("vip_ll_check");
  me->add("honor_point",2);
  tell_object(me, HIY"你获得了二点江湖声望。\n");	
	log_file("job/loseletter_s",
		strip(sprintf("%18s送信给%18s，"
		"奖励%4d点经验、%3d点潜能。(%d)\n",
		me->query("name")+"("+
		me->query("id")+")",
		ob->query("name")+"("+
		ob->query("id")+")",
		exp,
		pot, 
		me->query("job_time/丢失的信")
		)), 
		me );
	
	if (obexp>0 || obpot>0 || obmoney>0 || objectp(obj))
	{
		log_file("job/loseletter_r",
			strip(sprintf("%18s从%18s处收到"
			"%4d点经验、%3d点潜能、%4d锭黄金、物品：%s。\n",
			ob->query("name")+"("+
			ob->query("id")+")",
			me->query("name")+"("+
			me->query("id")+")",
			obexp,
			obpot, 
			obmoney/10000, 
			objectp(obj)?obj->name():"无"
			)), 
			me );
	}

	me->set("last_loseletter",time()); 
	me->delete_temp("ll_code");
	
	if (me->query("ll/last_day")!=time()/86400)	{
		me->set("ll/last_day",time()/86400);
		me->set("ll/last_count",me->query("ll/count"));
		me->delete("ll/count");
		me->delete("ll/get");
	}
	me->add("ll/count",1);
		
	destruct(this_object());
	return 1;
}

void dest()
{
	object ob1,ob = environment(this_object());
	string place_name;
	string target = query("addressee");	

		
	if(target && stringp(target)) {
	 ob1 = find_player(target);	
	
	 if (objectp(ob1))
		place_name = get_place(file_name(environment(ob1))); 
	}
	
	if (!target || !place_name)
	  this_player()->add("ll/get",-1);
	  
	if (userp(ob))
	{
		tell_object(ob, "你想来想去，事不关己，高高挂起，何必为他人苦苦奔波。\n"
			"你乘人不注意，偷偷把" + this_object()->name() + "扔进了路边的草丛。\n");
	}

	ob->delete_temp("ll_code");
	destruct(this_object());
}

