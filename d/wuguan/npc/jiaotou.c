// jiaotou.c 武馆教头
inherit NPC;
#include <ansi.h>

void create()
{
	set_name("武馆教头", ({ "wuguan jiaotou", "jiaotou"}));
	set("long", "这是武馆聘请的教头，他所有的基本武技，你都可以向他学习。\n"+
		"查询教头技能指令为：cha jiaotou。\n"+
		"从教头处学习技能的指令为：xue jiaotou <技能名>。\n");
	set("gender", "男性");
	set("age", 41);
	set("unique", 1);
         

	set_skill("axe", 80);
	set_skill("blade", 80);
	set_skill("club", 80);
	set_skill("cuff", 80);
	set_skill("dodge", 80);
	set_skill("finger", 80);
	set_skill("force", 80);
	set_skill("hammer", 80);
	set_skill("hand", 80);
	set_skill("leg", 80);
	set_skill("parry", 80);
	set_skill("stick", 80);
	set_skill("staff", 80);
	set_skill("strike", 80);
	set_skill("sword", 80);
	set_skill("whip", 80);

	set("jiali", 50);
	set("apply/attack", 30);
	set("apply/defense", 30);
	set("apply/damage", 35);
	set("combat_exp", 200000);
	set("shen_type", 1);
          
	set("chat_chance", 3);
	set("chat_msg", ({
		CYN"武馆教头喊道：「大家好好练啊，有什么不懂可以问我。」\n"NOR,
		CYN"武馆教头东看西瞧：「你们可不要偷懒哦，大家加油啊。」\n"NOR,
		(: random_move :)
	}));

	setup();
	carry_object(ARMOR_D("beixin"))->wear();      
}

void init()
{
	object ob, me;
	me = this_object();
	::init();
	if (interactive(ob = this_player()) && ob->query_condition("killer")){ 
		command("say 你竟敢在武馆杀人! \n");
		me->set_leader(ob);
		remove_call_out("kill_ob");
		call_out("kill_ob", 1, ob);
	}
}

int is_apprentice_of(object ob)
{
	return 1;
}

int prevent_learn(object ob, string skill)
{
	return 0;
}

int recognize_apprentice(object ob)
{
	return 1;
}
              