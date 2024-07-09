#include <ansi.h>

inherit NPC;

string ask_for(string verb);

void create()
{
	set_name("成高道长", ({ "chenggao daozhang", "chenggao" }));
	set("long",
	"他就是武当旁支的弟子，不精武学，偏好机巧。\n"
	"身穿一件锈了一个八卦玲珑结的的白色道袍。\n"
	"江湖传闻：成高道人武功平平，却精于机巧之术。\n");

	set("no_ansuan",1);

	set("nickname",RED"千 "HIG"巧 "HIM"道 "HIY"人"NOR);

	set("gender", "男性");
	set("age", 40+random(10));
	set("attitude", "peaceful");
	set("shen_type", 1);
	set("per",10);
	set("str", 25);
	set("int", 55);
	set("con", 25);
	set("dex", 25);

	set("max_qi", 1500);
	set("max_jing", 800);
	set("neili", 2000);
	set("max_neili", 2000);
	set("jiali", 100);
	set("combat_exp", 2000000);
	set("score", 8000);

	set_skill("force", 280);
	set_skill("yinyun-ziqi", 280);
	set_skill("dodge", 280);
	set_skill("tiyunzong", 280);
	set_skill("xuanxu-daofa", 280);
	set_skill("blade", 280);
	set_skill("wudang-quan", 280);
	set_skill("parry", 280);
	set_skill("sword", 280);
	set_skill("taoism", 100);
	set_skill("literate", 300);

	set_skill("zhizao", 300);
	set_skill("nongsang", 300);
	set_skill("caikuang", 300);
	set_skill("duanzao", 300);

	map_skill("force", "yinyun-ziqi");
	map_skill("dodge", "tiyunzong");
	map_skill("cuff", "wudang-quan");
	map_skill("parry", "xuanxu-daofa");
	map_skill("blade", "xuanxu-daofa");
	prepare_skill("cuff", "wudang-quan");

	create_family("武当派", 4, "弟子");
	set("class", "taoist");

	set("chat_chance", 50);
	set("chat_msg", ({
		CYN"成高道人叹息一声：「当年曾遇到一位世外高人淳于蓝老先生，也不知此生还有无机缘再次与他切磋技艺……」\n"NOR,
		CYN"成高道人说道：「韦兰那老儿的锻造之术已臻化境，可惜年纪大咯，嘿嘿……」\n"NOR,
		(: random_move :),
		CYN"成高道人说道：「玄铁，天蚕丝，风雷玉？这等劳拾子也能算极品？当年……」\n"NOR,
	}) );

	set("inquiry",
	([
		"nongsang" : (: ask_for, "nongsang" :),
		"农桑" : (: ask_for, "nongsang" :),
		"zhizao" : (: ask_for, "zhizao" :),
		"织造" : (: ask_for, "zhizao" :),
		"duanzao" : (: ask_for, "duanzao" :),
		"锻造" : (: ask_for, "duanzao" :),
		"caikuang" : (: ask_for, "caikuang" :),
		"采矿" : (: ask_for, "caikuang" :),
	]));
	setup();
	carry_object("/d/wudang/obj/white-robe")->wear();
}

void unconcious()
{
	reincarnate();
	clear_condition(0);
	message_vision("$N深深的吸了口气脸色看起来好多了。\n",this_object());
}

void die()
{
	unconcious();	
}

string ask_for(string verb)
{
	object me = this_player();
	int money = 20000;
	int improve;
	if(!verb) return 0;
	
	if(is_busy()) return 0;
		
	if(me->query_skill(verb,1) < 220 || me->query_skill(verb,1) > 400  )
	{
		return 0;
	}
	
	if(me->query("potential") < 5 )
	{
		command("en "+me->query("id"));
		return 0;
	}
	
	if(me->query("registered")>3 )
	  money  = money / 2;
	if(me->query("registered") == 3 )
	  money  = money / 10 * 7;
	  
	switch (MONEY_D->player_pay(me, money)) {
		case 0:
		case 2:
		command("beg2 "+me->query("id"));
		return 0;				
	}
	
	improve = random(me->query_int() * 3 / 4) + random(me->query_skill(verb,1) / 10) + me->query_int() / 4;
	
	improve *= 3;
	
	if(me->query_skill(verb,1) < 280) improve = improve*2;	
							
	me->add("potential",-5);	
	message_vision(CYN"$N兴奋的搓着双手说道：「贫道正有兴趣与人探讨探讨这个问题呢……」\n"NOR,this_object());
	tell_object(me,CYN"你和成高道长一起交流着「"HIY + to_chinese(verb) + CYN"」的心得。\n"NOR);
	if(WORKER_D->check_impove(me,verb,improve,3) <= 0 )
	{
		message_vision(CYN"$N突然叹息道：「你真是一方顽石，顽石不可化也。我看你是很难开窍了……」\n"NOR,this_object() );
	}
	//start_busy(2+random(2));
	return "";
}