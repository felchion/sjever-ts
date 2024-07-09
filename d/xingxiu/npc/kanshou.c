//caiji@SJ 8/24/2000
#include <ansi.h>
inherit NPC;
inherit F_UNIQUE;

void create()
{
	set_name("看守", ({ "kan shou", "kanshou", "shouwei" }) );
	set("gender", "男性" );
	set("title", "毒虫谷守卫");
	set("age", 35);
	set("location", 1);
	set("unique", 1);
	set("job_npc",1);
	set("long", "星宿老仙派他老守卫毒虫谷，一个很忠心的弟子。\n");
	set("str", 20);
	set("dex", 20);
	set("con", 20);
	set("int", 20);
	set("per", 30);
	set("shen", -7500);

	set_skill("force", 105);
	set_skill("huagong-dafa", 115);
	set_skill("dodge", 110);
	set_skill("zhaixingshu", 110);
	set_skill("strike", 110);
	set_skill("chousui-zhang", 110);
	set_skill("poison", 110);
	set_skill("parry", 110);
	set_skill("staff", 110);
	set_skill("tianshan-zhang", 110);
	set_skill("literate", 70);
	set_skill("claw", 110);
	set_skill("sanyin-zhua", 110);
	map_skill("claw", "sanyin-zhua");
	prepare_skill("claw", "sanyin-zhua");
	map_skill("force", "huagong-dafa");
	map_skill("dodge", "zhaixingshu");
	map_skill("strike", "chousui-zhang");
	map_skill("parry", "chousui-zhang");
	map_skill("staff", "tianshan-zhang");
	prepare_skill("strike", "chousui-zhang");

	set("max_qi", 2000);
	set("max_jing", 2000);
	set("eff_jingli", 1800);
	set("score", -14000);
	set("combat_exp", 750000);
	set("chat_chance_combat", 5);
	set("chat_msg_combat", ({
		(: exert_function, "huagong" :),
		(: perform_action, "strike.biyan" :),
	}));
	create_family("星宿派", 2, "弟子");
	set("attitude", "peaceful");
	set("inquiry", ([
		"丁春秋" : "丁春秋是你叫的吗？没大没小的。以后叫老仙！",
		"caiji" : "那可是位了不起的仙人！",
		"彩集" : "彩集是你叫的吗？没大没小的。以后叫风仙！",
	]));	
	setup();
	carry_object(BINGQI_D("gangzhang"))->wield();
	carry_object(ARMOR_D("changpao"))->wear();
	add_money("silver", 10);
}

int do_xian(string arg)
{
	object me=this_object();
	object who, obj;
	who = this_player();

	if(who->is_busy() || who->is_fighting()) 
		return notify_fail("你正忙着呢。\n");
	if(me->is_busy() || me->is_fighting()) 
		return notify_fail("没看见看守正忙着吗？\n");
	if(!arg || !objectp(obj = present(arg, who))) 
		return notify_fail("你要献给看守什么东西？\n");
   
	if(!who || environment(who) != environment()) return 0;
	if(userp(obj)){
		command("consider "+(string)who->query("id"));
		command("say 你拿个快死的人来骗我？");
		return 1;
	}
	message_vision(HIY"\n$n毕恭毕敬地将"+obj->query("name")+HIY"拿出，双手捧上，想要献给$N。\n\n"NOR, me, who);     
	if(me->is_busy()) {
		command("say 我正忙着呢，你等等。");
		return notify_fail("老仙让你等等。\n");
	}
	if((obj->query("id") == "zhizhu" 
	|| obj->query("id") == "wugong" 
	|| obj->query("id") == "xiezi" 
	|| obj->query("id") == "shachong") 
	&& obj->query("cook/owner") == who){
		command("nod "+(string)who->query("id"));
		command("say 好样的！\n");
		message_vision(HIY"$N将$n"HIY"放入旁边一个大坛子里，并拿出几个铜钱。\n"NOR,me, obj);
		remove_call_out("destroying");
		call_out("destroying", 0, obj, me,who);		       
		me->add_busy(2);
		who->add_busy(2);
		return 1;
	}
	if(obj->query("money_id")) {
		command("pat " + who->query("id"));
		command("say 钱我有的是，你以后还是留给自己吧。");
		return 1;
	}
	if(obj->query("cook/allow") && obj->query("cook/owner") != who->query("id")){
 //fixed by caiji，change who to who->query("id")
		command("hehe "+(string)who->query("id"));
		command("say 不错啊，但这虫子不是你抓的。");
		return 1;
	}
	else return 1;       
}

void destroying(object obj, object ob, object me)
{   
	int gain;
	object n_money;

	if(obj){
		gain=obj->query("cook/value")+1+random(4);
		destruct(obj);
	}
	if(!ob) return;
	if(!me || environment(me) != environment()){
		command("say 咦，人呢？");
		return;
	}
	command("nod "+me->query("id"));
		message_vision(HIY"$N对$n说道：这正是我们星宿派需要的，谢谢这位"+RANK_D->query_respect(me)+"的合作。！\n"NOR,ob, me);
	n_money = new("/clone/money/coin");
	n_money->move(me); 
	n_money->add_amount(random(50));
	if((!me->query_temp("xx_zuji")) && me->query("combat_exp") > 100000)
		gain /= 4;

	if ( me->query("combat_exp") < 2000000){
		me->add("combat_exp", gain);
		me->add("potential", gain/5);
		if ( me->query("potential", 1) > me->query("max_pot", 1) )
			me->set("potential" , me->query("max_pot", 1));
		tell_object(me, YEL "你的经验和潜能各增加了"+chinese_number(gain)+"点！\n" NOR);
		log_file("job/chong",sprintf("%8s%-10s第%5d次星宿抓虫，得到经验:%3d，潜能：%2d，现经验：%d。\n",
			me->name(), "("+getuid(me)+")", me->query("job_time/星宿熬膏"),gain,gain/5,me->query("combat_exp")),me);
	}
	me->add("job_time/星宿熬膏",1);
	message_vision(HIY"$N对$n说道：一点小意思，不成敬意，下次请再帮忙啊！\n$N给了$n几文铜钱。\n"NOR,ob,me);
	return;      
}

void init()
{       
	object me;
	me = this_player();
	::init();
	add_action("do_order", "order");
	add_action("do_xian", "xian");
}

int do_order(string arg)
{
	object env,me;

	me = this_player();
	env = environment(this_object());

	if(arg=="remove" && (me->query_temp("chonggu_allow") || (int)me->query("family/generation")<=2) &&
	 (string)env->query("short")=="虫谷入口" && (string)me->query("family/family_name")=="星宿派" &&
	 (!env->query("exits/north"))){
		me->delete_temp("chonggu_allow");
		message_vision("$n在$N耳边悄声说了几句话。\n", me,this_object());
		message_vision("$N跟你说道：既然已得到老仙应允，我自当放你进入毒虫谷。\n",this_object());
		message_vision("只见$N伸手在旁边一个石头上按了一按。\n", this_object());
		env->open_enter1();
		//call_out("open-enter",2);
		//env->start_call_out( (: call_other, env, "open-enter":), 2);
		message_vision("$N想回来时，喊（yell）一声就可以了。\n",me);
		return 1;
	}
	else if(arg=="defense" && (me->query_temp("chonggu_allow") || (int)me->query("family/generation")<=2) &&
	 (string)env->query("short")=="虫谷入口" && (string)me->query("family/family_name")=="星宿派"&&
	 env->query("exits/north")){
		me->delete_temp("chonggu_allow");
		message_vision("$N在$n耳边悄声说了几句话。\n", me,this_object());
		message_vision("$N跟你说道：既然如此，我自当关上谷门，防止外人进去。\n",this_object());
		message_vision("只见$N伸手在旁边一个石头上按了一按。\n", this_object());
		env->close_enter1();
		return 1;
	}
	else
		return 0;
}
