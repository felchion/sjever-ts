// Modify By River@SJ

inherit FIGHTER;
#include <ansi.h>

void do_kill(object ob);

string *color_title = ({"天杀门", "地煞门"});

void create()
{
        int i;

	i = random(4);
	set_name("无名氏",({"wuming shi"}));
		
	set("gender", i?"男性":"女性");
	set("long", "这人身穿黑色紧装，面上蒙着一块黑布，只露出一双眼睛，精光四射。\n");
	set("title", color_title[random(sizeof(color_title))]+(i?"":"女")+"杀手");
	set("age", 20 + random(50));
	
	set("combat_exp", 8000000);
	set("meitude", "peaceful");
	set("str",18+random(5));
	set("int",18+random(5));
	set("con",18+random(5));
	set("dex",18+random(5));
	set("max_qi", 20000);
	set("max_jing", 20000);
	set("max_neili", 5000);
	set("neili", 5000);
	set("no_heal",1);
	set("no_ansuan",1);
  set("no_bark", 1);

	setup();

	carry_object("/d/dali/obj/blackcloth")->wear();
}
void init()
{
	object me, ob;
	string t_name;
	
	::init();

	ob = this_player();
	me = this_object();
	if (ob != query("target")) return;
	t_name = ob->query("name");       
	
	if (interactive(ob)
	 && ob->query_temp("songxin_ok")
	 && me->query_temp("zuji_target")==t_name) {
		call_out("dest",1, ob);
		return;
	}

	if (environment(ob)->query("no_fight")
	 && me->query_temp("zuji_target")==t_name
	 && ob->query_temp("songxin_zuji")) {
		remove_call_out("dest");
		call_out("dest2",1, ob);
		return;
	}

	if (interactive(ob) 
	 && !environment(ob)->query("no_fight")
	 && ob->query_temp("songxin_zuji")
	 && (!(me->query_temp("songxin_zuji1"))||!(me->query_temp("zuji_target")!=t_name))){
		me->set_temp("apply/pfm_attack",  30+random(30));
		me->set_temp("apply/pfm_dodge",  30+random(30));
		me->set_temp("apply/damage", 30 + random(60));
		me->set_temp("songxin_zuji1",  1);
		if (!me->query_temp("zuji_target")) me->set_temp("zuji_target",t_name);
		if (!me->query_temp("skill_set")) {
			command("say 哈哈，碰到我手里，你肯定完了！！！");
			me->set_temp("skill_set",1);
			ob->set_temp("zuji",1);
//                        me->set_leader(ob);
			remove_call_out("dest");
			call_out("dest",300,ob);
			remove_call_out("do_kill");
			call_out("do_kill", 0, ob); 
		}
		else {
                        command("say Kao，还敢追来，再练一百年吧！！！！");
//                        me->set_leader(ob);                     
			remove_call_out("dest");
			call_out("dest",300,ob);
			remove_call_out("do_kill");
			call_out("do_kill", 0, ob); 
		}
	}
}
int checking(object me, object ob)
{
	int ret =  ::checking(me,ob);
	if(!ret) return 0;
	remove_call_out("checking");
	call_out("checking", 1, me, ob);
	if(!living(ob)){
		remove_call_out("checking");
		ob->set("qi",100);																			//防止玩家意外死亡
		ob->set("jing",100);
		ob->set("jingli",100);
		tell_room(environment(me), "\n"+me->query("name")+"大喊一声：老子信也拿到手了，今天就不杀你了！说完转身几个起落就不见了。\n");
		destruct(me);
		 return 1;
	}
}
/*void heart_beat()
{
	object target = query("target");
	object env;

	::heart_beat();
	if (target && !present(target) && (env = environment(target))
	&& !is_fighting() && !is_busy()
	&& !target->is_ghost() && living(this_object()))
		move(env);
}*/


void dest(object ob)
{
	if (ob) {
		tell_object(ob, query("name")+"道：这次算你命大！\n");
		tell_object(ob, query("name")+"匆匆离开。\n");
	}
	destruct(this_object());
}

void dest2(object ob)
{
	if (ob) {
		tell_object(ob, query("name")+"道：你躲得过初一躲不过十五，我们还会来的！\n");
		tell_object(ob, query("name")+"匆匆离开。\n");
		ob->delete_temp("songxin_zuji");
	}
	destruct(this_object());
}
void do_kill(object ob)
{
	object me = this_object();

	if (query("target")!=ob) {
		remove_killer(ob);
		ob->remove_killer(me);
		ob->add_busy(1);
		return;
	}
	remove_call_out("checking");
	call_out("checking", 2, me, ob);
	::do_kill(ob);
}
