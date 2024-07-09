inherit FIGHTER;
#include <ansi.h>
      
void create()
{
	set_name("莽汉", ({ "mang han", "han"}));
	set("long", HIY"他体格也算十分魁梧，只是眼神有些呆滞、似乎总是在寻找什么。\n"NOR);
	set("long_base", HIY"他体格也算十分魁梧，只是眼神有些呆滞、似乎总是在寻找什么。\n"NOR);
	set("gender", "男性");
	set("age", 33);
	set("attitude", "peaceful");
	set("shen", -100);

	set("str", 28);
	set("int", 20);
	set("con", 25);
	set("dex", 20);

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
	set("chat_chance", 1);
	set("chat_msg", ({
		"莽汉贼头贼脑地看了看周围没人，晃着脑袋流里流气地哼哼着：路边的野花，你不要采啊...不采白不采呀！\n",
			"莽汉满脸痴呆地傻笑着：花，我要花花。。。\n",
			"莽汉用上嘴唇亲了亲下嘴唇。\n",
		"莽汉看了看别人的嘴唇，吞了口唾液。\n",
		"莽汉摇摇头，叹口气道：“疯了，都疯了...”\n",
	}));
	set("inquiry", ([
		"比武招亲" : "看那小妞多俊俏，要不是我身手不成，也想上去试试……\n",
		"招亲" : "看那小妞多俊俏，要不是我身手不成，也想上去试试……\n",
	]));
	setup();
	carry_object(ARMOR_D("caoxie"))->wear();
	carry_object(ARMOR_D("cloth"))->wear();
}
void init()
{
    object me,ob; 
	object *obj;
	int i;
    mapping skill;
    string *skl;
    string good_skills;
    ::init();

    ob = this_object();
    me = this_player();
	obj = all_inventory(ob);
	i = sizeof(obj);
	while (i--) {
		if(!userp(obj[i]) && obj[i]->query("weapon_prop"))
			destruct(obj[i]);
	}	
	skill = ob->query_skills();
	skl = keys(skill);
	i = sizeof(skl);
	while (i--) {
		ob->delete_skill(skl[i]);
	}
   	if(!random(4)) command("chicken "+ob->query("id"));
	else if(!random(4)) command("@@ "+ob->query("id"));
	i=me->query("max_pot");
	if(i<350) i=350;
	i=i-100;		  
   	ob->set("party",str_menpai[random(sizeof(str_menpai))]);  	  
	ob->copy_menpai(({ob->query("party")}),1,random(2),30+random(50));    //复制npc的门派武功，                                                   
	ob->copy_state();				//根据门派更新npc 的一些状态
	ob->set_skills_level(i);
	ob->set("max_neili",me->query("max_neili"));
	ob->set("max_jingli",me->query("max_jingli"));
	ob->set("max_qi",me->query("max_qi"));
	ob->set("max_jing",me->query("max_jing"));
	ob->set("neili",query("max_neili")*3/2);
	ob->set("jingli",query("max_jingli")*3/2);
	ob->set("eff_jingli",query("max_jingli")*3/2);
	ob->set("qi",query("max_qi"));
	ob->set("eff_qi",query("max_qi"));
	ob->set("jing",query("max_jing")*3/2);
	ob->set("eff_jing",query("max_jing")*3/2);
    
	if (ob->query("f_skill2")) 
		good_skills=to_chinese(ob->query("f_skill"))+"和"+to_chinese(ob->query("f_skill2"));
	else	
		good_skills=to_chinese(ob->query("f_skill"));

   	ob->set("long",ob->query("long_base")+
       	 	"看架势和"HIW+ob->query("family/family_name")+NOR"好像有关系，而且还似乎学过几招"HIC+good_skills+NOR"。\n");
    ob->set("jiali",me->query("jiali"));
	ob->set("combat_exp",me->query("combat_exp"));
	ob->set_skill("literate",ob->query("int")*10); 	 											//full literate
	ob->set_skill("wuxing-zhen",ob->query("int")*10); 	
	obj = all_inventory(ob);
	i = sizeof(obj);
	while (i--) {
		if(!userp(obj[i]) && obj[i]->query("weapon_prop"))
		{
			if(obj[i]->query("skill_type")!="hammer")
				message_vision(HIG"$N嗬嗬傻笑几声，竟然拿出一"+obj[i]->query("unit")+NOR+obj[i]->query("name")+HIG"。\n"NOR, ob);
			else message_vision(HIG"$N嗬嗬傻笑几声，竟然拿出五"+obj[i]->query("unit")+NOR+obj[i]->query("name")+HIG"。\n"NOR, ob);
		}
	}	
}