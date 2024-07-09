// guanbing for tdh job by Lklv

inherit NPC;
#include <ansi.h>
#include "skill.h"

//string *rtitle = ({HIY"御前侍卫"NOR, HIR"刑部铁捕"NOR, HIM"大内高手"NOR});

void create()
{
//	string str;
	mapping name;
	name = RNAME_D->get_random_name(1);
//	str = rtitle[random(sizeof(rtitle))];

	set_name(name["name"], name["id"]+({"shi wei"}) );
	set("gender", "男性");
	set("long", "这家伙膀大腰圆，气势汹汹，令人望而生畏。\n");
	set("shen", -5000);
	set("attitude", "friendly");
	set("age", random(20)+25);
//	set("title", str);
	set("title",HIY"御前侍卫"NOR);

	set("no_ansuan", 1);
	set("no_bark", 1);
	set("no_quest", 1);
	set("tdh_bing", 1);

	set("str", 20);
	set("int", 20);
	set("con", 20);
	set("dex", 20);

	set("max_qi",3500);
	set("max_jing", 1600);
	set("eff_jingli", 2000);
	set("max_neili", 4000);
	set("jiali", 40);
	set("combat_exp", 800000);

	set_skill("force", 200);
	set_skill("dodge", 200);
	set_skill("parry", 200);
	set_skill("cuff", 200);
	set_skill("taizuquan", 200);
	set_skill("taiji-shengong", 200);
	map_skill("force", "taiji-shengong");
	map_skill("parry", "taizuquan");
	map_skill("cuff", "taizuquan");
	prepare_skill("cuff", "taizuquan");
	if ( !random(5))
		add_money("gold", 1);
	setup();

	carry_object(ARMOR_D("cloth"))->wear();
	carry_object(ARMOR_D("shoes"))->wear();

	if (clonep())
		call_out("dest", 200, this_object());
}

void dest(object ob)
{
	object jober;

	if ( !ob ) return;
	if ( ob->is_fighting())
		tell_room(environment(ob), "\n"+ob->name()+"恶狠狠说道：好汉不吃眼前亏，老子先撤了。\n");
	else
		tell_room(environment(ob), "\n"+ob->name()+"转眼间走的无影无踪。\n");

	if ( stringp(query("天地会jober"))
	 && objectp(jober= find_player(query("天地会jober")))
	 && jober->query_temp("tdhjob")
	 && !jober->query_temp("天地会job/lost")){
		jober->add_temp("天地会job/meet", -1);
	}
	destruct(ob);
}

void heart_beat()
{
	object jober;

	::heart_beat();

	if ( stringp(query("天地会jober"))
	 && objectp(jober= find_player(query("天地会jober")))
	 && jober->query_temp("天地会job/lost")){
		dest(this_object());
	}
}

void kill_ob(object ob)
{
	object me = this_object();

	if( me->query("天地会jober") != ob->query("id") && userp(ob)){
		me->remove_enemy(ob);
		ob->remove_killer(me);
		return;
	}
	::kill_ob(ob);
}

void fight_ob(object ob)
{
	if( query("天地会jober") != ob->query("id") && userp(ob)){
		command("say "+ob->name()+"你快走远点，老子我正在执行公务！");
		ob->remove_killer(this_object());
	}
	else ::fight_ob(ob);
}

void die()
{
	object ob, jober;
	ob = this_object();

	if ( objectp( jober = ob->query_temp("last_damage_from") )
	&& query("天地会jober") == jober->query("id")
	&& !jober->query_temp("天地会job/lost")
	&& jober->query_temp("tdhjob")){
		jober->add_temp("天地会job/killer", 1);
	}
	if ( random(3)){
		dest(ob);
		return;
	}
	if (objectp(jober= find_player(query("天地会jober")))
	 && jober->query_temp("tdhjob")
	 && !jober->query_temp("天地会job/lost")){
		jober->add_temp("天地会job/meet", -1);
	}
	::die();
}

void unconcious()
{
	object ob = this_object();
	tell_room(environment(ob), "\n"+ob->name()+"大叫道：“反贼！你们是跑不了的！”\n");
	die();
}
