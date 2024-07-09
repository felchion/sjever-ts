// /u/jpei/thd/npc/ya-mujiang.c

#include <ansi.h>

inherit NPC;

object me;

void create()
{
	set_name("哑木匠", ({"ya mujiang", "ya", "mujiang", "jiang"}));
	set("long", "他是一位身材适中的木匠，不知为什么舌头被割去了。\n");
	set("mute", 1);

	set("gender", "男性");
	set("attitude", "friendly");

	set("age", 40);
	set("shen_type", 0);
	set("str", 20);
	set("int", 20);
	set("con", 20);
	set("dex", 20);
	set("max_qi", 400);
	set("max_jing", 300);
	set("neili", 450);
	set("max_neili", 450);
	set("jiali", 40);
	set("combat_exp", 100000);
	set("family/family_name", "桃花岛");

	set_skill("force", 50);
	set_skill("dodge", 100);
	set_skill("strike", 50);
	set_skill("parry", 60);

	setup();

        carry_object("/clone/misc/cloth")->wear();
}

void init()
{	
	object ob = this_player();

	::init();
	if (!query("in_repairing") && ob->id("machine bonze") && ob->query("damaged"))
	{
		remove_call_out("repairing_1");
		call_out("repairing_1", 3, ob, environment());
		set("in_repairing", 1);
	}
}

int repairing_1(object ob, object env)
{
	if (env != environment()) return 1; 
	if (!present(ob, env)) return 1; 

	command("follow none");
	remove_call_out("no_find");
	if (present(me, environment()) ) command("nod " + me->query("id"));
	tell_room(env, "哑木匠用手势比划着说他要开始修理了。\n", ({}));
	
	remove_call_out("repairing_2");
	call_out("repairing_2", 2 + random(3), ob, env);

	return 1;
}

int repairing_2(object ob, object env)
{
	if (env != environment()) {
		destruct(this_object());
		return 1; 
	}
	if (!present(ob, env)) {
		tell_room(env, GRN "\n哑木匠忽然发现机械和尚不见了，奇怪地挠了挠头。\n" NOR, ({}));
		tell_room(env, GRN "\n哑木匠走了出去。\n" NOR, ({}));
		destruct(this_object());
		return 1; 
	}

	if( ob->query("damaged") )
	{
		tell_room(env, GRN "\n哑木匠对着机械和尚瞧了一会，又试着扳动机械和尚的四肢和脑袋，然后歪着脑袋好象在想什么。\n\n" NOR, ({}));
		remove_call_out("repairing_3");
		call_out("repairing_3", 2 + random(2), ob, env);
	}
	else
	{
		if (present(me, environment())) {
			command("? " + me->query("id"));
			command("angry " + me->query("id"));
		}

		tell_room(env, GRN "\n哑木匠气鼓鼓地走了。\n" NOR, ({}));
		destruct(this_object());
	}

	return 1;
}
	
int repairing_3(object ob, object env)
{
	if (env != environment()) {
		destruct(this_object());
		return 1; 
	}
	if (!present(ob, env)) {
		tell_room(env, GRN "\n哑木匠忽然发现机械和尚不见了，奇怪地挠了挠头。\n" NOR, ({}));
		tell_room(env, GRN "\n哑木匠走了出去。\n" NOR, ({}));
		destruct(this_object());
		return 1; 
	}

	command("nod");
	tell_room(env, GRN "\n接着从怀里掏出一大堆工具来，打开机械和尚的身体，摆弄了几下。\n\n" NOR, ({}));

	remove_call_out("repairing_4");
	call_out("repairing_4", 3 + random(3), ob, env);

	return 1;
}

int repairing_4(object ob, object env)
{
	if (env != environment()) {
		destruct(this_object());
		return 1; 
	}
	if (!present(ob, env)) {
		tell_room(env, GRN "\n哑木匠忽然发现机械和尚不见了，奇怪地挠了挠头。\n" NOR, ({}));
		tell_room(env, GRN "\n哑木匠走了出去。\n" NOR, ({}));
		destruct(this_object());
		return 1; 
	}

	if (random(3)) 
	{		
		ob->delete("damaged");
		ob->set("fight_times", 0);	
		ob->enable_player();
		tell_room(env, GRN "\n机械和尚的身体吱吱地扭动了几下，看来已经恢复正常了。\n\n" NOR, ({}));
		command(":)");
		command("wave");
		tell_room(env, GRN "\n哑木匠走了出去。\n" NOR, ({}));
		destruct(this_object());
	}
	else {
		command("angry");
		tell_room(env, GRN "\n哑木匠大怒之下，用斧子将机械和尚劈了个稀烂。\n" NOR, ({}));
		destruct(ob);
		tell_room(env, GRN "\n哑木匠平静下来，好象觉得很过意不去，打手势表示说他去钉个新的来。\n" NOR, ({}));
		tell_room(env, GRN "\n哑木匠走了出去。\n" NOR, ({}));
		this_object()->move_to_ob("../mujiangfang");

		remove_call_out("do_back");
		call_out("do_back", 120 + random(60), env);
	}
	return 1;
}

int do_back(object env)
{
	this_object()->move(env);
	if (!present("machine bonze 4", env)) {
		tell_room(env, GRN "\n哑木匠扛着一个新的机械和尚走了过来。\n" NOR, ({}));
		tell_room(env, GRN "\n哑木匠把机械和尚放到地下。\n" NOR, ({}));
		new(__DIR__"machine-bonze.c")->move(environment());
	}
	else {
		tell_room(env, GRN "\n哑木匠空着双手回来了。\n" NOR, ({}));
		tell_room(env, GRN "\n哑木匠打手势表示没有制作成功。\n" NOR, ({}));
		command("shrug");
	}
	command("wave");
	tell_room(env, GRN "\n哑木匠走了出去。\n" NOR, ({}));
	destruct(this_object());
	return 1;
}

int no_find()
{
	message_vision(GRN "哑木匠感到很不耐烦了，用手势朝$N比划了几下就走了。\n" NOR, me);
	destruct(this_object());
}

void do_follow(object ob)
{
	command("nod");
	if (present(ob, environment(this_object()))) {
		command("follow " + ob->query("id"));
		set("busy", 1);
		me = ob;
		remove_call_out("no_find");
		call_out("no_find", 180);
	}
	else {
		tell_room(environment(this_object()), GRN "哑木匠四处望了望，没有看到" + ob->name() +"，疑惑地走了。\n" NOR, ({}));
		destruct(this_object());
	}
}
