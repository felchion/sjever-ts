// yinggu.c 瑛姑 鬼谷算数
// created by campsun 2003/12/2
//是否处于鬼谷算术状态的判断为query_temp("ggs/started") 在做相关修改的时候要特别注意。
//LinuX@SJ 2004.2.28


#include <ansi.h>
inherit NPC;

int ask_ggs();
int ask_ggso();

void create()
{
	seteuid( getuid() );
        set_name("瑛姑", ({ "ying gu","ying"}));
        set("nickname",HIW"鬼谷算术专家"NOR);
        set("title",HIR"神算子"NOR);
        set("gender", "女性");
        set("age", 35);
        set("long", "她是个看起来似乎有几分美貌的女子。但是确是一头白发如雪，两只眸子里都是幽怨和感伤。\n");
        set("attitude", "peaceful");
        set("shen_type", 0);
	set("no_get","瑛姑对你而言太重了。");
        set("str", 30);
        set("int", 60);
        set("con", 30);
        set("dex", 26);
	set("per", 28);
	
	set("jiali",50);
	set_skill("force", 200);
	set_skill("huntian-qigong", 200);
	set_skill("bihai-chaosheng", 200);
	set_skill("dodge", 200);
	set_skill("xiaoyaoyou", 200);
	set_skill("parry", 200);
	set_skill("strike", 200);
	set_skill("luoying-zhang", 200);
	set_skill("hand", 200);
	set_skill("lanhua-shou", 200);
	set_skill("finger", 200);
	set_skill("tanzhi-shentong", 200);
	set_skill("sword", 200);
	set_skill("yuxiao-jian", 200);
	set_skill("literate",500);
	set_skill("qimen-bagua", 500);
	set_skill("bangjue",200);
	set_skill("dagou-bang",200);
	set_skill("stick",200);

	map_skill("force", "huntian-qigong");
	map_skill("dodge", "xiaoyaoyou");
	map_skill("strike", "luoying-zhang");
	map_skill("hand", "lanhua-shou");
	map_skill("finger", "tanzhi-shentong");
	map_skill("parry", "dagou-bang");
	map_skill("sword", "yuxiao-jian");
	map_skill("stick", "dagou-bang");
	prepare_skill("hand", "lanhua-shou");

	set("chat_chance_combat", 100);
	set("chat_msg_combat", ({
		(: perform_action, "stick.chuo" :),
		(: perform_action, "stick.ban" :),
		(: perform_action, "stick.yin" :),
		(: perform_action, "stick.chuo" :),
		(: perform_action, "stick.chan" :),
		(: perform_action, "stick.wugou" :),
		(: exert_function, "huntian" :),
	}));

	set("combat_exp", 2500000);
	set("max_qi", 5000);
	set("max_jing", 4000);
	set("neili", 7500);
	set("max_jingli", 8500);
	set("eff_jingli", 8500);
	set("max_neili", 8500);


        set("inquiry", ([
                "rumors" : "这道题怎么那么难算啊？不知道上次来求医的小姑娘什么时候才能再来。",
                "周伯通" : "听说他被困在桃花岛上，我苦心研究鬼谷算数就是为了能去救他。",
                "四张机" : "四张机，鸳鸯织就欲双飞....",
                "鬼谷算术" : (: ask_ggs :),
                "开始" 	: (: ask_ggs :),
                "start" : (: ask_ggs :),
                "结束" 	: (: ask_ggso :),
                "休息" 	: (: ask_ggso :),
                "over" 	: (: ask_ggso :),
        ]));

        setup();
        carry_object(ARMOR_D("pink_cloth"))->wear();

}

int ask_ggs()
{
	object me,ob;
	int this_week = time()/86400/7;
	ob = this_object();
	me = this_player();
	
	
	if (me->query("registered")<3)
	{
		tell_object(me, "对不起啊，目前鬼谷算术只对贵宾VIP用户开放强化学习功能。\n");
		return 1;
	} 
	
	if(!me->query("ggs/last_week") || me->query("ggs/last_week")=="" || me->query("ggs/last_week")!=this_week)
	{
		me->set("ggs/last_week", this_week);
		me->set("ggs/started",0);
		me->set("ggs/start_time","");
		me->set("ggs/over_time","");
		me->set("ggs/left_time",10*3600);
	}

	//For all Bug By Ciwei@SJ
	while(me->query("ggs/left_time")>10*3600)
		me->add("ggs/left_time",-10*3600);

	if(me->query("ggs/left_time") < 1)
	{
		command("say 你虽然天资聪慧，但是贪多嚼不烂，不能再冒进了，请下周再来询问吧。");
		return 1;
	}
	if(me->query("ggs/started")==1)
	{
		if(me->query_temp("ggs/started")=="")
		{
			tell_object(me, "你先要问我休息，才能再开始使用鬼谷算术。\n");
		}
		else
		{
			tell_object(me, "你不是正在研习修炼鬼谷算术中么？还不快点抓紧时间……\n");
			command("kick " + me->query("id"));
		}
		return 1;
	}
	
	me->set("ggs/started",1); //使用状态标记
	me->set_temp("ggs/started",1); //使用状态标记
	me->set("ggs/start_time",time()); //使用状态标记
	
	me->apply_condition("guigushu",1); //这个condition 不会自动减少
	
	command("say 鬼谷算术，是我这些年精心研究的修心妙法，若能参悟，可以加倍提高学习领悟的能力。");
	command("say 不过鬼谷算术博大渊深，甚劳心神，每周最多也就应用十个小时吧。");
	command("look " + me->query("id"));
	command("say 现在是 "+ CHINESE_D->chinese_date(time(),1) + "。");
	command("say 你本周还可以使用鬼谷算术"+CHINESE_D->chinese_time(me->query("ggs/left_time"))+"，现在开始计时了。");
	log_file( "quest/ggs",sprintf("%s(%s)在第%d周开始鬼谷算术,剩:%s.\n",
			me->name(),
			me->query("id"),
			this_week,
			CHINESE_D->chinese_time(me->query("ggs/left_time"))
			));
	
return 1;
}

int ask_ggso()
{
	object me,ob;
	int user_time;
	//int an_hour = 60*60*60;
	int this_week = time()/86400/7;
	ob = this_object();
	me = this_player();
	
	if (me->query("registered")<3)
	{
		tell_object(me, "对不起啊，目前鬼谷算术只对贵宾VIP用户开放强化学习功能。\n");
		return 1;
	}
	if(me->query("ggs/started")!=1)
	{
		command("say 还没有开始就要休息……");
		command("pig " + me->query("id"));
		return 1;
	}
	if(me->query_temp("ggs/started")!=1)
	{
		command("say 你是不是没问休息就下线了？结束时间就按照你上次下线时间算吧。");
		me->set("ggs/over_time",FINGER_D->get_last_on(me->query("id")));		
	}
	else
	{	
		me->set("ggs/over_time",time());	
	}
	
	user_time=(int)me->query("ggs/over_time")-(int)me->query("ggs/start_time");
	command("say 是啊，劳逸结合，读书领悟久了，是该休息休息。");
	command("look " + me->query("id"));
	
	command("say 你的鬼谷算术开始时间是：" + CHINESE_D->chinese_date((int)me->query("ggs/start_time"),1) + "。");
	command("say             结束时间是：" + CHINESE_D->chinese_date((int)me->query("ggs/over_time"),1) + "。");
	command("say 这次你一共修炼了：" + CHINESE_D->chinese_time(user_time)+"。");
	me->set("ggs/left_time",me->query("ggs/left_time") - user_time);

	log_file( "quest/ggs",sprintf("%s(%s)在第%d周结束ggs,剩:%s,用:%s.\n",
			me->name(),
			me->query("id"),
			this_week,
			CHINESE_D->chinese_time(me->query("ggs/left_time")),
			CHINESE_D->chinese_time(user_time)
			));



	if (me->query("ggs/left_time") < 0 ) //防止bug
		me->set("ggs/left_time",0);
		
	if (me->query("ggs/left_time")) 	
		command("say 你本周还可以使用鬼谷算术"+CHINESE_D->chinese_time(me->query("ggs/left_time"))+"。");
	else
		command("say 你的鬼谷算术时间用完了，下周再来吧。");
	
	me->set_temp("ggs/started",0);
	me->set("ggs/started",0);
	me->set("ggs/start_time","");
	me->set("ggs/over_time","");	
	
	me->clear_condition("guigushu"); //清除了


return 1;	
}

//不死
int accept_fight(object ob){
        command("say 这位"+RANK_D->query_respect(ob)+"，武功是用来防身的，不是好斗滴！");
}
int hit_ob(object ob)
{
        command("say 这位"+RANK_D->query_respect(ob)+"，武功是用来防身的，不是好斗滴！");
}


void kill_ob(object ob)
{
        ob->remove_killer(this_object());
        remove_killer(ob);
        command("swear");
}

void unconcious()
{
	message_vision(YEL"$N突然运用鬼谷之术，全身发出异样色彩，有愈战愈勇之势。\n"NOR, this_object());
        reincarnate();
        add("str",10); //越来越勇，^_^
        set("eff_qi", query("max_qi"));
        set("qi", query("max_qi"));
        set("eff_jing", query("max_jing"));
        set("jing", query("max_jing"));
        set("jingli", query("eff_jingli"));
        clear_conditions_by_type("poison");
}

void die()
{
        unconcious();
}