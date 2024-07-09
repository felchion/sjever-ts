// guidao.c
// by cleansword
// 96-02-16 YZC disabled NPC killing, & "unknown" problem when revealed

#include <ansi.h>

inherit NPC;

string ask_me(object who);

void create()
{
	set_name("落魄公子", ({ "gong zi", "gong" }) );
	set("title", "嫖客");
	set("gender", "男性" );
	set("shen_type", -1);
	set("age", 25);
	set("str", 25);
	set("cor", 25);
	set("cps", 25);
	set("int", 25);
	set("long",
	"这是个眉清目秀的青年公子，但精神颓废，一身酒气熏人，想必是在酒色场中淘空了身子。\n" );

	set("combat_exp", 20000);
	set("attitude", "heroism");
	set("chat_chance_combat", 15);
	set("chat_msg_combat", ({
		"公子说道: 你当真吃了豹子胆啦 ? 你知道我是谁 ! \n",
		"公子忽然发出一剑，招数精奇，但剑到中途却又急忙收招。\n",
		"公子说道: 快离开我 ! 我不能打架了 !....\n"
	}) );

	set("inquiry", ([
		"张三丰" : "是武林的泰山北斗，谁人不识？" ,
	        "张无忌" : "淫贼，我终有一天要抽他的筋剥他的皮。",	
                "宋远桥" : "武当七侠之首，名杨天下。",
                "宋青书" : "玉面孟尝之名播于江湖。",
                "周芷若"   : (: ask_me :),
	]) );

	set_skill("sword", 50);
	set_skill("dodge", 40);
	set_skill("unarmed", 50);
	set_skill("parry", 50);
	
	setup();
	carry_object(BINGQI_D("changjian"))->wield();
	carry_object(ARMOR_D("cloth"))->wear();
}

string ask_me(object who)
{
	int i;
	object env = environment(who);
	object *ob;

	if( !env->query("no_fight") && query("revealed") ) {
		if( is_killing(who->query("id")) ) return "知道我秘密的人都得死！\n";
		else {
			kill_ob(who->query("id"));
			who->fight_ob(this_object());
			return "知道我秘密的人都得死，纳命来吧！\n";
		}
	}

	if( (env->query("no_fight") || random(10) < 5) || is_fighting() )
		return "这个......\n";

	set_temp("apply/attack",  50);
	set_temp("apply/defense", 50);
	set_temp("apply/damage",  20);

	message("vision",
		HIY "落魄公子眼中突然放出兽性的光芒，怒声说道，你竟敢提周姑娘的名字？！\n"
		"落魄公子大喝一声，叫道，天下除我宋青书之外，敢提周姑娘的人都得死！！\n" 
		"宋青书使开长剑，招数精奇，沉猛狠辣，太极神剑端的非同小可。\n" NOR,
		environment(), this_object() );
	set("title","玉面孟尝");
	set_name("宋青书",({ "song qingshu", "song", "qingshu" }));
	set("pursuer", 1);
	set("vendetta/authority", 1);
	set("chat_chance", 15);
	set("chat_msg", ({
		(: random_move :)
	}));
	set("bellicosity", 10000);
	set("combat_exp", 300000);
	set("max_qi",1400);
	set("qi", 1400);
	set("eff_qi", 1400);
	set("max_neili",1200);
	set("neili", 1200);
	set("eff_jingli",1000);  
	set("jingli", 1000);
	set("max_jing", 1000);
	set("eff_jing", 1000);
	set("jing", 1000);

	set_skill("force", 100);
	set_skill("yinyun-ziqi", 100);
	set_skill("dodge", 100);
	set_skill("tiyunzong", 100);
	set_skill("cuff", 100);
	set_skill("taiji-quan", 100);
	set_skill("parry", 100);
	set_skill("sword", 120);
	set_skill("taiji-jian", 120);
	set_skill("taoism", 30);
	set_skill("literate", 40);

        map_skill("force", "yinyun-ziqi");
	map_skill("dodge", "tiyunzong");
	map_skill("cuff", "taiji-quan");
	map_skill("parry", "taiji-jian");
	map_skill("sword", "taiji-jian");
	prepare_skill("cuff", "taiji-quan");
        ob = all_inventory(environment());
	for(i=sizeof(ob)-1; i>=0; i--) {
		if( !userp(ob[i]) ) continue;
                if((int)ob[i]->query("combat_exp",1)<50000) continue;
		kill_ob(ob[i]);
		ob[i]->fight(this_object());
	}
	add_money("gold", 5);
	set("revealed", 1);
	return "天下除我宋青书之外，敢提周姑娘的人都得死！\n";
}
