//add by tangfeng
//2005.3

inherit FIGHTER;
inherit F_UNIQUE;
#include <ansi.h>
#include <skills_pfm.h>
#include "/d/city/diemenglou_party.h";


string *valid_types =
({
	"claw",
	"cuff",
	"finger",
	"hand",
	"leg",
	"strike",
});

void create()
{
	mapping name;
	int sex = random(3);
	name = RNAME_D->get_random_name(random(3));
	set_name(name["name"], name["id"]);
	set_name(query("name"), ({ query("id"),"mengdie npc"}));
	set("gender", (sex?"男性":"女性"));
	set("long", "神秘人，你尽力地去去看清，却还是一片模糊。\n");
	set("age", 25+random(100));
	set("attitude", "peaceful");
	set("rank_info/rank","神秘人");
	set("rank_info/self_rude","神秘人");
	set("rank_info/self","小生");
	set("rank_info/rude","神秘人");
	set("rank_info/respect","神秘人");
	set("rank_info/color","HIW");

	set("str", 30);
	set("int", 30);
	set("con", 30);
	set("dex", 30);
	set("per", 30);
	set("max_qi", 500);
	set("max_jing", 500);
	set("eff_jingli",400);
	set("neili", 100);
	set("max_neili", 100);
	set("jiali", 20);
	set("combat_exp", 8000+random(500));
//一些武功绝技被设定因为quest取消，特意增加该变量
//目前设定不允许使用的是tanzhi-shentong的pfm qiankun
    set("quest_cancel",1);
	set("no_quest",1);//

	set_skill("force", 70);
	set_skill("dodge", 80);
	set_skill("unarmed", 70);
	set_skill("parry", 80);
	set_skill("sword", 80);
	set_skill("cuff", 80);
	set("inquiry", ([
		"name" : random(2)?"什么叫名字？":"呵呵，人生如梦，名字早就忘记了。",
		"名字" : random(2)?"什么叫名字？":"呵呵，人生如梦，名字早就忘记了。",
		"拜师" : "嗯....，小生注定一声漂泊，还是算了吧。",
    ]));
	set("chat_chance_combat", 100);
	setup();
	carry_object(ARMOR_D("shoes"))->wear();
	carry_object(ARMOR_D("changshan"))->wear();
}
init()
{
    object me,ob;  
	int i,p;
    mapping pmap,my_party;
    ::init();
    ob = this_object();
	if(!ob->query("kill_object")) return;
    me =ob->query("kill_object");
	if(!me) return;
	if(query("set_ok")) return;
	set("set_ok",1);
	pmap = ob->query_skill_prepare();
	if (!pmap)
	{
		i=sizeof(valid_types);
		p=0;
		while (i--)
		{
			if (!ob->query_skill(valid_types[i], 1)) continue;
			if (ob->query_skill_mapped(valid_types[i]))continue;
			if(p>=2) continue;
			p=p+1;
			ob->prepare_skill(valid_types[i], ob->query_skill_mapped(valid_types[i]));
		}
	}
	my_party = get_party_string(ob->query("party"));
	if(ob->query("teamleader"))
	{
		if(my_party["shen"]=="邪")
		{
			if(random(2)) command("nomatch");
			else command("u&me");
			message_vision(HIC"$N对$n哼了一声道：老子正是"+query("title")+query("name")+HIC"，特来会会。\n" NOR, ob,me);
			message_vision(HIC"$N狂笑道："+my_party["col"]+my_party["party-name"]+my_party["lineup-name"]+HIC"二十秒钟之内就要启动，$n还是早做准备的好。\n" NOR, ob,me);
		}
		else if(my_party["shen"]=="正")
		{
			if(random(2)) command("sorry2");
			else command("welcome "+ob->query("id"));
			message_vision(HIC"$N对$n点了点头道：在下正是"+query("title")+query("name")+HIC"，还请阁下手下留情。\n" NOR, ob,me);
			message_vision(HIC"$N微笑道："+my_party["col"]+my_party["party-name"]+my_party["lineup-name"]+HIC"二十秒钟之内就要启动，$n还是早做准备的好。\n" NOR, ob,me);
		}
		else
		{
			if(random(2)) command("sneer "+me->query("id"));
			else command("grpfight "+ob->query("id"));
			message_vision(HIC"$N瞥了$n一眼道：在下正是"+query("title")+query("name")+HIC"，还望多多包涵。\n" NOR, ob,me);
			message_vision(HIC"$N奸笑道："+my_party["col"]+my_party["party-name"]+my_party["lineup-name"]+HIC"二十秒钟之内就要启动，$n还是早做准备的好。\n" NOR, ob,me);
		}
	}
	if(my_party["party"]=="pxj" && objectp(ob->query_temp("weapon"))) 
	{
		ob->set_temp("double_attack",1);//pxj 兵器hubo
		ob->add_temp("apply/attack", ob->query_skill("force")/5);
		ob->add_temp("apply/damage", ob->query_skill("force")/5);	
	}
	if(my_party["party"]=="jsj") 
	{
		ob->set("double_attack",1);//jsj 空手hubo
		ob->add_temp("apply/attack", ob->query_skill("force")/5);
		ob->add_temp("apply/damage", ob->query_skill("force")/5);	
		ob->add_temp("apply/parry", ob->query_skill("force")/4);
	}
	if(!objectp(ob->query_temp("weapon")) && random(3))//空手增加伤害和防御
	{
		ob->add_temp("apply/damage", ob->query_skill("force")/6);
		ob->add_temp("apply/parry", ob->query_skill("force")/6);
	}
	if(!random(5)) //随机增加攻击
	{
		if(my_party["shen"]=="邪")
			command("killair "+me->query("id"));
		else if(my_party["shen"]=="正")
			command("buddha");
		else
			command("piggy");
		ob->add_temp("apply/attack", random(ob->query_skill("force")/5));
		ob->add_temp("apply/damage", random(ob->query_skill("force")/5));
	}
	ob->remove_all_killer();
    remove_call_out("kill");
    remove_call_out("check");    
    call_out("check",19,ob,me);
    call_out("kill",20,me);
	remove_call_out("dest");
	call_out("dest",360);
}
void check(object ob, object me) 
{
   mapping my_party;
   int ret =  ::checking(ob,me);

   if(!ret) return;
   if(!ob) return;
   my_party = get_party_string(ob->query("party"));

   command("lineup with mengdie npc 1");
   command("lineup with mengdie npc 2");
   command("lineup with mengdie npc 3");
   command("lineup with mengdie npc 4");
   command("lineup with mengdie npc 5");
   command("lineup with mengdie npc 6");
   command("lineup with mengdie npc 7");
   command("lineup with mengdie npc 8");
   command("lineup with mengdie npc 9");
   command("lineup with mengdie npc 10");

   command("lineup form "+my_party["lineup"]);  

   command("lineup with mengdie npc 1");
   command("lineup with mengdie npc 2");
   command("lineup with mengdie npc 3");
   command("lineup with mengdie npc 4");
   command("lineup with mengdie npc 5");
   command("lineup with mengdie npc 6");
   command("lineup with mengdie npc 7");
   command("lineup with mengdie npc 8");
   command("lineup with mengdie npc 9");
   command("lineup with mengdie npc 10"); 
   remove_call_out("check");
   call_out("check", 1, ob, me);
   return;
}

void kill(object me)
{
   if(!me) return;
   this_object()->kill_ob(me);
   me->kill_ob(this_object());
}

void die()
{
	object ob,me;
	object *obs;
	int i;
	ob=this_object();

    if(ob->query("kill_object") && ob->query("quest/level"))
	{
        me=ob->query("kill_object");
	}
	if(!me) return;
	message_vision("\n"NOR,ob);
	command("why");
	if(!random(3))	command("sue");
	else if(!random(3)) command("kill1");
	else if(!random(3)) command("nomatch");
	else if(random(2)) command("poem");
	else command("killair");
	if(random(2)) command("wait "+me->query("id"));
	else if(random(2)) message_vision(HIG"$N哼了一声，缓缓消失在角落里。\n" NOR, ob);
	else message_vision(HIG"$N慢慢地退了出去。\n" NOR, ob);
	obs = deep_inventory(ob);       
	obs = filter_array(obs,(:$1->query("imbued"):));
	i = sizeof(obs);
	while(i--)
	{
		obs[i]->move(environment(ob));
		message_vision(YEL"$N顺手丢下一"+obs[i]->query("unit")+obs[i]->name()+NOR+YEL"。\n"NOR,ob);
	}
	if(me && present(me,environment(ob)))
	{
		i=ob->query("quest/level");
		i=i*i;
		me->add_temp("quest/special/score",i);
		me->add("quest/special/score",i);
		tell_object(me,HIR"\n你成功的击退"+ob->short(1)+HIR"，获得奖励分数"+i+"。\n"NOR);
		log_file("quest/SPECIAL", sprintf("%s(%s)二楼比武得分：%d。经验：%d。\n", me->name(1),me->query("id"),i,me->query("combat_exp")) );
	}
	destruct(this_object());
}

void unconcious()
{
  die();
}

void dest()
{
	object ob,me;
	object *obs;
	int i;
	ob=this_object();
	obs = deep_inventory(ob);       
	obs = filter_array(obs,(:$1->query("imbued"):));
	i = sizeof(obs);
	if(ob->query("kill_object") && ob->query("quest/level"))
	{
		me=ob->query("kill_object");
	}
	while(i--)
	{
		if(me) 
		{
			tell_object(me,HIY"突然你觉得行囊里似乎多了点什么，低头一看竟然是刚才失去的那一"+obs[i]->query("unit")+obs[i]->name()+HIY"。\n"NOR);
			obs[i]->move(me);
		}
		else 
		{
			message_vision(YEL"$N丢下一"+obs[i]->query("unit")+obs[i]->name()+NOR+YEL"。\n"NOR,ob);
			obs[i]->move(environment(ob));
		}
	}
	message_vision(HIR"\n突然一阵风散，$N原来站着的地方竟然一点痕迹也没有留下。"HIM"刚才的一切如梦幻般，全然消逝在风中。\n"NOR,this_object());
	destruct(this_object());
}