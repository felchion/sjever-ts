
inherit FIGHTER;
#include <ansi.h>

string *str_menpai = ({ "wd","hs","ss","gm","gb","tz","dls","sld","mj","thd","kl"});       
#include "/d/city/npc/skills_pfm.h";
int ask_sj();
int ask_up();
void go_up(object me);
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
	set_name("老仆", ({ "laopu"}));
	set("long", "一个神秘的老人，看似白发苍苍，却是双目如电，炯炯有神。\n");
	set("long_base", HIG"一个神秘的老人，看似白发苍苍，却是双目如电，炯炯有神。\n"NOR);
	set("gender", "男性");
	set("age", 33);
	set("attitude", "peaceful");
	set("shen", -100);
	set("rank_info/rank","老翁");
	set("rank_info/self_rude","老翁");
	set("rank_info/self","老朽");
	set("rank_info/rude","老翁");
	set("rank_info/respect","老翁");
	set("rank_info/color","HIW");
	set("age",80+random(30));
	set("no_quest",1);//

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

	set_skill("force", 70);
	set_skill("dodge", 80);
	set_skill("unarmed", 70);
	set_skill("parry", 80);
	set_skill("sword", 80);
	set_skill("cuff", 80);
	set("inquiry", ([
		"name" : random(2)?"老朽已经一把年纪，名字早就忘记了。":"呵呵，行将就木，名字早就忘记了。",
		"名字" : random(2)?"老朽已经一把年纪，名字早就忘记了。":"呵呵，行将就木，名字早就忘记了。",
		"主人":  "我主人是谁？主人就是主人……，只有你们这些凡夫俗子才在意名号，\n你一定要打听，我也听过有人称他为逍遥子。",
		"蝶梦楼":  "蝴蝶是梦，梦是蝴蝶，真幻难辨，想上楼去见识见识么？",

		"书剑" : (: ask_sj :),
		"梦境" : (: ask_sj :),
		"梦幻" : (: ask_sj :),
		"上楼" : (: ask_up :),
		"拜师" : "嗯....，老朽注定一声漂泊，还是算了吧。",
    ]));

	setup();
	carry_object(ARMOR_D("shoes"))->wear();
	carry_object(ARMOR_D("changshan"))->wear();
}
void init()
{
    object me,ob; 
	object *obj;
	int i,p;
    mapping skill, pmap;
    string *skl;


    string force_name;
    ::init();
    ob = this_object();
    me = this_player();
    if(ob->is_fighting()) return;
	obj = all_inventory(ob);
	i = sizeof(obj);
	while (i--) {
		if(!userp(obj[i]) && obj[i]->query("weapon_prop"))
			destruct(obj[i]);
	}
	pmap = ob->query_skill_prepare();
	if (pmap)
	{
		skl = keys(pmap);
		foreach (string item in skl)
			ob->prepare_skill(item);
		ob->reset_action();
	}
	skill = ob->query_skills();
	skl = keys(skill);
	i = sizeof(skl);
	while (i--) {
		ob->delete_skill(skl[i]);
	}
	p=ob->query("max_skill");
	i=me->query("max_pot");
	i=i-100;
	if(i<p) i=p; 
	if(i<250) i=250;
	ob->set("max_skill",i);
   	ob->set("party",str_menpai[random(sizeof(str_menpai))]);  	  
	ob->copy_menpai(({ob->query("party")}),1,random(2),100);    //复制npc的门派武功，                                                   
	ob->copy_state();				//根据门派更新npc 的一些状态
	ob->set_skills_level(i);
	ob->set("max_neili",me->query("max_neili"));
	ob->set("max_jingli",me->query("max_jingli"));
	p=ob->query("max_qi");
	i=me->query("max_qi")*2;
	if(i<p) i=p;
	ob->set("max_qi",i);
	ob->set("max_jing",me->query("max_jing"));
	ob->set("neili",query("max_neili")*3/2);
	ob->set("jingli",query("max_jingli")*3/2);
	ob->set("eff_jingli",query("max_jingli")*3/2);
	ob->set("qi",query("max_qi"));
	ob->set("eff_qi",query("max_qi"));
	ob->set("jing",query("max_jing")*3/2);
	ob->set("eff_jing",query("max_jing")*3/2);
	if(!stringp(force_name = to_chinese(ob->query_skill_mapped("force"))))
		force_name = "无名神功";
	ob->set("long",ob->query("long_base")+
       	 	HIG"倒是一身武功"+HIW+force_name+HIG"早已返璞归真，似乎与"+HIW+ob->query("family/family_name")+HIG"有关。"NOR);
    ob->set("jiali",me->query("jiali"));
	ob->set("combat_exp",me->query("combat_exp"));
	ob->set_skill("literate",ob->query("int")*10); 	 											//full literate
	ob->set_skill("wuxing-zhen",ob->query("int")*10); 	
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
}
int ask_up()
{
    object me;
	object *obj;
	int x,p;
	mapping mp;
	me=this_player();
	if(me->query_condition("killer"))
	{
		command("sigh");
		command("say 人在江湖，怎能没有半点血腥染手。老夫倒是理解，只是官府这一关......");
		message_vision(HIY"$N木木然，似有难言之隐，低头长时间没有言语。\n"NOR,this_object());
		return 1;
	}
	if(me->query_condition("job_busy"))
	{
		command("sigh");
		command("say 刚做完"+me->query("job_name")+"任务，还是稍微休息一下吧！");
		message_vision(HIY"$N木木然，似有难言之隐，低头长时间没有言语。\n"NOR,this_object());
		return 1;
	}
	if(me->query_temp("天地会job")||me->query_temp("tdhjob"))
	{
		command("pat "+me->query("id"));
		command("say 天地会任务很忙吧！");
		message_vision(HIY"$N木木然，似有难言之隐，低头长时间没有言语。\n"NOR,this_object());
		return 1;
	}
	if(me->query("neili")<me->query("max_neili"))
	{
		command("pat "+me->query("id"));
		command("say 你内力不足，如何全力一战？");
		message_vision(HIY"$N木木然，似有难言之隐，低头长时间没有言语。\n"NOR,this_object());
		return 1;
	}
	if(me->query("eff_jing")<me->query("max_jing"))
	{
		command("pat "+me->query("id"));
		command("say 你精血不足，如何全力一战？");
		message_vision(HIY"$N木木然，似有难言之隐，低头长时间没有言语。\n"NOR,this_object());
		return 1;
	}
	if( mapp(mp = me->query_conditions_by_type("poison")) && sizeof(mp)>0 )
	{
		command("sigh");
		command("say "+me->query("name")+"还是先治好自己的病再说吧！");
		message_vision(HIY"$N木木然，似有难言之隐，低头长时间没有言语。\n"NOR,this_object());
		return 1;
	}
	if( mapp(mp = me->query_conditions_by_type("killer")) && sizeof(mp)>0 )
	{
		command("sigh");
		command("say "+me->query("name")+"还是先解决了自己的个人恩怨再来吧！");
		message_vision(HIY"$N木木然，似有难言之隐，低头长时间没有言语。\n"NOR,this_object());
		return 1;
	}
	if( mapp(mp = me->query_conditions_by_type("hurt")) && sizeof(mp)>0 )
	{
		command("sigh");
		command("say "+me->query("name")+"还是先治疗一下内伤再来吧！");
		message_vision(HIY"$N木木然，似有难言之隐，低头长时间没有言语。\n"NOR,this_object());
		return 1;
	}
	if(wizardp(me) && !me->query("env/mengdie_test")) 
	{
		command("sigh");
		command("say "+me->query("name")+"你老就别捣乱了，专心搞开发去吧！");
		message_vision(HIY"$N木木然，似有难言之隐，低头长时间没有言语。\n"NOR,this_object());
		return 1;
	}
	obj = deep_inventory(me);
	p = sizeof(obj);
	while (p--)
	{
		if (obj[p]->is_character() )
		{
			command("sigh");
			command("say "+me->query("name")+"，你这个"+obj[p]->query("name")+"是什么东东哦？");
			message_vision(HIY"$N木木然，似有难言之隐，低头长时间没有言语。\n"NOR,this_object());
			return 1;
		}
		if(obj[p]->query("unique"))
		{
			command("sigh");
			command("say "+me->query("name")+"，你这个"+obj[p]->query("name")+"可是宝物啊，上面太危险了！");
			message_vision(HIY"$N木木然，似有难言之隐，低头长时间没有言语。\n"NOR,this_object());
			return 1;
		}
	}
	if(!me->query_temp("quest/special/sj") )
	{
		command("sigh");
		message_vision(HIY"$N木木然，似有难言之隐，低头长时间没有言语。\n"NOR,this_object());
		return 1;
	}
	if(me->query_temp("quest/special/up"))
	{
		command("sigh");
		me->delete_temp("quest/special");
		message_vision(HIY"$N木木然，似有难言之隐，低头长时间没有言语。\n"NOR,this_object());
		return 1;
	}
	//只能一个人做
	obj = users();
	x = sizeof(obj);
	while(x--)
	{
		if ( obj[x]->query_temp("quest/special/up") && obj[x]!=me) 
		{
			message_vision(HIY"$N木木然，似有难言之隐，半响抬起头来。\n"NOR,this_object());
			command("say 已有"+obj[x]->query("name")+"刚已寻梦去了，"+me->query("name")+"还是罢了吧！");	
			return 1;
		}
	}
	message_vision(HIY"$N神秘得一笑，说道：「这梦蝶小楼，是家主人所筑，且在北溟之外，亦夫南溟之中……。」\n"NOR,this_object());
	if(!me->query_temp("quest/special/enter"))
	{
		command("hehe");
		return 1;
	}
	message_vision(HIY"$N咪咪笑着闪过身子，$n觉得神志恍惚……\n"NOR,this_object(),me);
	me->set_temp("quest/special/up",1);
	remove_call_out("go_up");
	call_out("go_up",4,me);
	return 1;
}
void go_up(object me)
{
	object ob,room;
	ob=this_object();
	if(!me) return;
	if(!me->query_temp("quest/special/sj")) return;
	if(!me->query_temp("quest/special/up")) 
	{
		message_vision(HIB"$N突然想起了什么似的，微微一招手，$n顿时清醒，发现自己竟然还在这"HIW"莲性寺白塔"HIB"。\n"NOR,this_object(),me);
		return;
	}
	if(environment(ob)!=environment(me))
	{
		me->delete_temp("quest/special");
		command("hmm");
		return;
	}
	if (!(room = find_object("/d/city/diemenglou1")))
		room = load_object("/d/city/diemenglou1");
	if(!room)
	{
		message_vision(HIY"\n如梦如醉之间，发现$n已经在瘦西湖边，$N已不知哪里去了……。\n"NOR,this_object(),me);
		me->delete_temp("quest/special");
		me->move("/d/city/hubian1");
		return;
	}
	command("smile");
	me->move(room);
	message_vision(HIY"\n如梦如醉之间，发现$n已经在一所亭台之中，$N已不知哪里去了……。\n"NOR,this_object(),me);
	return;        	
}
int ask_sj()
{
    object me;
    me=this_player();
    if(me->query_temp("quest/special/sj"))
    {
		message_vision(HIY"$N木木然，似有难言之隐，低头不再言语。\n"NOR,this_object());
		return 1;
	}
	message_vision(HIY"$N木木然吟道：「北冥……有鱼，其名为鲲。……。」\n"NOR,this_object());
	message_vision(HIY"$N木木然吟道：「鲲之大，不知其……几千里也。」\n"NOR,this_object());
	message_vision(HIY"$N木木然吟道：「化……而为……鸟，其名为……鹏。」\n"NOR,this_object());
	message_vision(HIY"$N喃喃自语道：「书剑……，凄离梦……，往乎……，化为蝶……。」\n"NOR,this_object());
	me->set_temp("quest/special/sj",1);
	return 1;
}
int accept_object(object who, object ob)
{
   if(userp(ob)) return 0;        
   if((ob->query("id") == "silver"||ob->query("id") == "gold") && ob->value() >= 5000)
   {
      message_vision(HIW"$N惊奇地叫了起来：「如此这般，怎么敢当……」\n"NOR,this_object());
	  command("say");
	  command("hoho");
	  who->set_temp("quest/special/enter",1);
	  return 1;
   }
   message_vision("$N缓缓地摇了摇头：「如此这般，怎么敢当……」\n",who);
   return 0;
}

