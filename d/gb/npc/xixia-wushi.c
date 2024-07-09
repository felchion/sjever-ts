// Modify By LOOKLOVE@SJ 2000/1/8

#include <ansi.h>

inherit FIGHTER;

inherit F_UNIQUE;
inherit F_SAVE;

void create()
{
	mapping name;

	set("title", HIW+"西夏一品堂 武士"+NOR);
	
	name = RNAME_D->get_random_name(1);	
	
	set_name(name["name"], name["id"] + ({ "wu shi" }) );
	
	set("init",0);

	set("gender", "男性");
	set("age", 20 + random(60));
	set("long", "这是西夏一品堂从江湖上重金招聘的一名武林高手。\n" );
	if( random(20) < 1 )
		set("double_attack",1);

	set("attitude", "peaceful");
	set("str", 20);
	set("int", 20);
	set("con", 20);
	set("dex", 20);

	set("qi", 10000);
	set("max_qi", 10000);
	set("jing", 10000);
	set("max_jing", 10000);
	set("eff_jingli", 10000);
	set("combat_exp", 2000000);
	set("score", 20000);
	set("jiali",10);
	set("no_get",1);
	set("no_quest", 1);
	set_temp("apply/damage",200);
	set_temp("apply/pfm_damage",80);	
	set_temp("apply/pfm_attack",50);
	set_temp("apply/hurt", 100);
  setup();
}

void init()
{
	object me,ob,cloth;
	int lvl,times;
	//string s_skill,s_force,s_perform,s_base;
	int index;
	object *team;
	mapping name;

	::init();
	me = this_player();
	ob = this_object();

	if( query("init") == 0
	&& me->query_temp("smy_job")
	&& userp(me) ) {

		set("init", 1);

		times = me->query_temp("smy_job/finish");
		
		if( me->query_temp("smy_job/lvl")){
			lvl = me->query_temp("smy_job/lvl");
		}

		else {
			lvl = me->query("max_pot");
			team = me->query_team();
			if (sizeof(team)) {				
				for(index=0;index<sizeof(team);index++){
					if( team[index]->query("max_pot") > lvl )
					lvl = team[index]->query("max_pot");
				} 
			}
			lvl = lvl - 100 + random(5);
		}

		if (times>5)
		  lvl += (times-5)*20;
 
		if(times>15){
			set_temp("apply/wp_damage",200*(times-15));
			set_temp("apply/pfm_damage",100*(times-15));	
			set_temp("apply/pfm_attack",120*(times-15));
			set_temp("apply/pfm_dodge",120*(times-15));
			set_temp("apply/pfm_parry",120*(times-15));
			set_temp("apply/hurt", 50*(times-15));
		}
		
		if (!lvl) { 
		  lvl = me->query("max_pot") - 100;
		}
		
		setparty(times,lvl,lvl*lvl*lvl/10,times);

		set("ciwei_level",times);

		set("max_neili",query_skill("force")*8 + query("combat_exp")/1000);
		set("max_jingli",query_skill("force")*8);
		set("eff_jingli",query_skill("force")*8);
		copy_state();
		setup();
		reincarnate();

	//	mapping name;
		name = RNAME_D->get_random_name(1);
		set_name(name["name"], name["id"] + ({ "wu shi" }) );
		set("gender", "男性");

		cloth = new(ARMOR_D("cloth"));
		cloth->set("name",HIC"西夏长袍"NOR);
		cloth->move(ob);
		cloth->wear();

		set("neili",query("max_neili")* 2);

		call_out("check",1,ob,me);
		if( me->query_temp("smy_job/enter") || me->query("smy_job/join") )
		{
			me->add_temp("smy_job/killed",1);
			ob->kill_ob(me);
		}
	}
}

void check(object ob, object me)
{
	int i;
	object *team;
	if( !ob ) return;

	checking(ob,me);


	if( me && me->is_ghost()){
		log_file("job/smy", sprintf("%8s%-10s抗敌颂摩崖任务，不幸死亡，经验：%d。",
		me->query("name"), "("+me->query("id")+")", me->query("combat_exp")), me);
	}

	if( !me
	|| !me->query_temp("smy_job")
	|| me->is_ghost()
	|| environment(me)!=environment(ob)
	|| ob->query_temp("combat_time")>300){
		tell_room(environment(ob), "西夏武士道：蛮子已有准备，快撤。说完转身逃去了\n");
		me->delete_temp("smy_job");
		me->apply_condition("job_busy",20+random(20));
		me->apply_condition("smy_job",0);
		
		if (team=me->query_team() && sizeof(team)>2) {
			for(i=0;i<sizeof(team);i++) {
				team[i]->delete_temp("smy_job");
			}
		}
		
		destruct(ob);
		return;
	}
	call_out("check",1,ob,me);
}

void unconcious()
{
	object ob = this_object(),me;
	me = ob->query_temp("last_damage_from");
	if(!me || !objectp(me)){
		::unconcious();
		return;
	}
	if( userp(me)
	 && !me->query_temp("smy_job")
	 && !me->query_team()
	 && me->query("combat_exp") > ob->query("cobmat_exp")/2 ){
	 	
	 	me->add_condition("killer", 5);
	 	
		reincarnate();
		set("eff_qi", query("max_qi"));
		set("qi", query("max_qi")*6/5);
		set("eff_jing", query("max_jing"));
		set("jing", query("max_jing"));
		set("jingli", query("eff_jingli"));
		set("neili",query("max_neili")*3/2);
		return;
	}
	::unconcious();
	return;
}

void die()
{
	object me, ob;

	ob = this_object();
	me = ob->query_temp("last_damage_from");

	if(!objectp(me)){
		::die();
		return;
	}
	if( userp(me)
	 && !me->query_temp("smy_job/enter")
	 && !me->query_temp("smy_job/join")
	 && me->query("combat_exp") > ob->query("combat_exp")/2 ){
	 	me->add_condition("killer", 5);
		new(__DIR__"xixia-wushi")->move(environment(me));
	}
	::die();
}
