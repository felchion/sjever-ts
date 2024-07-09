// by cool 98.6.10
// Modify By River@SJ 8/30/2001

void init()
{
	object me, ob;
	//object obj;
	string t_name;
	
	::init();

	ob = this_player();
	me = this_object();
	if (ob != query("target")) return;
	t_name = ob->query("name");       

	if (interactive(ob)
	 && (ob->query_temp("songxin_ok")||!(int)ob->query_condition("dali_songxin"))
	 && me->query_temp("zuji_target")==t_name) {
		call_out("dest",1,ob);
		return;
	}

	if (environment(ob)->query("no_fight")
	 && me->query_temp("zuji_target")==t_name
	 && ob->query_temp("songxin_zuji")) {
		remove_call_out("dest");
		call_out("dest2",1,ob);
		return;
	}
  
	if (interactive(ob) 
	 && !environment(ob)->query("no_fight")
	 && ob->query_temp("songxin_zuji")
	 && (!me->query_temp("songxin_zuji1")||!(me->query_temp("zuji_target")!=t_name))){
		set("no_quest", 1);   
		me->set_temp("songxin_zuji1",  1);
		if (!me->query_temp("zuji_target")) me->set_temp("zuji_target",t_name);
		if (!me->query_temp("skill_set")) {
			command("say 小子，乖乖把密函交出来吧！！！\n");
			/*
			if (!ob->query_temp("letter_steal")) {
				if (ob->query("combat_exp",1) < 300000){
					obj=new(__DIR__"dali_letter");
					obj->move(me);
					ob->set_temp("letter_steal",1);
				}
				else {
					if (random(10) > 5){
						ob->set_temp("letter_steal",1);
						obj=new(__DIR__"dali_letter");
						obj->move(me);
					}
				}
			}*/
			remove_call_out("dest");
			call_out("dest",300,ob);
			remove_call_out("do_clone");
			call_out("do_clone", 0, me, ob);
		}
		else {
			command("say 想跑，还不给我交出来！！！\n");
			me->set_leader(ob);
			remove_call_out("dest");
			call_out("dest",300,ob);
			remove_call_out("kill_ob");
			call_out("kill_ob", 0, ob); 
		}
	}
}

void heart_beat()
{
	object target = query("target");
	object env;

	::heart_beat();
	if (target && !present(target) && (env = environment(target))
	&& !is_fighting() && !is_busy()
	&& !target->is_ghost() && living(this_object()))
		move(env);
}

int do_clone(object me, object ob)
{
	mapping killerskill1;
	int number,k_qi,k_neili,k_jing;
	string t_force,t_dodge;

	number = (int)ob->query("max_pot");
	number = number-100;
	k_qi = (int)ob->query("max_qi")-ob->query_temp("adjust/qi");
	k_jing = (int)ob->query("max_jing")-ob->query_temp("adjust/jing");
	k_neili = (int)ob->query("max_neili");

/*set killer's level*/

	if(ob->query("combat_exp") >= 2000000){
		killerskill1=skill2[random(sizeof(skill2))];
	}
	else if( ob->query("combat_exp") < 2000000
	 && ob->query("combat_exp") >= 1500000){
		killerskill1 = skill2[random(sizeof(skill2))];
	}
	else if( ob->query("combat_exp") < 1500000
	 && ob->query("combat_exp") >= 1000000){
		killerskill1 = skill2[random(sizeof(skill2))];
	}
	else if( ob->query("combat_exp") < 1000000
	 && ob->query("combat_exp") >= 500000){
		killerskill1 = skill1[random(sizeof(skill1))];
	}
	else if( ob->query("combat_exp") < 500000
	 && ob->query("combat_exp") >= 300000){
		killerskill1 = skill1[random(sizeof(skill1))];
	}
	else if( ob->query("combat_exp") < 300000){
		killerskill1 = skill1[random(sizeof(skill1))];
	}
  
  number -= random(10); 
     
/*set killer's skill*/

	t_force = k_force[random(sizeof(k_force))];
	t_dodge = k_dodge[random(sizeof(k_dodge))];       

	me->set_skill("force",number);
	me->set_skill("dodge",number);
	me->set_skill("parry",number);
	me->set_skill(t_force,number);
	me->set_skill(t_dodge,number);
	me->set_skill(killerskill1["t_skill"],number);
	me->set_skill(killerskill1["b_skill"],number);

/* set killer's  skill maps */

	me->map_skill("force",t_force);
	me->map_skill("dodge",t_dodge);
	me->map_skill("parry",killerskill1["t_skill"]);
	me->map_skill(killerskill1["b_skill"],killerskill1["t_skill"]);
	
/* set killer's skill prepares */

	me->prepare_skill(killerskill1["b_skill"],killerskill1["t_skill"]);

/* copy entire dbase values */

	me->set("max_qi",k_qi);
	me->set("eff_qi",k_qi);
	me->set("qi",k_qi);
	me->set("max_jing",k_jing*6/5);
	me->set("eff_jing",k_jing*6/5);
	me->set("eff_jingli", k_jing);
	me->set("jingli", k_jing);
	me->set("jing",k_jing);
	me->set("max_neili",k_neili);
	me->set("neili",k_neili);
	me->set("jiali", 50+random(40));
	me->set("combat_exp",ob->query("combat_exp"));
	
	  // 减少NPC加气
	me->set_temp("skill_set",1);
	ob->set_temp("zuji",1);
	me->set_leader(ob);
	
	//For Newbie Gold
	if (!random(2) && ob->query("combat_exp")<1000000 )
		add_money("silver", random(30)+30);
	//By Ciwei
		
	remove_call_out("kill_ob");
	call_out("kill_ob", 0, ob); 
	return 1;
}

void dest(object ob)
{
	object me;
	me = this_object();

	if (ob) {
		tell_object(ob,me->query("name")+"道：这次算你命大！\n");
		tell_object(ob,me->query("name")+"匆匆离开。\n");
	}
	destruct(this_object());
}

void kill_ob(object ob)
{
	object me = this_object();

	if (query("target")!=ob) {
		remove_killer(ob);
		ob->remove_killer(me);
		ob->add_busy(1);
		return;
	}
	::kill_ob(ob);
}

void dest2(object ob)
{
	if (ob) {
		tell_object(ob,query("name")+"道：你躲得过初一躲不过十五，我们还会来的！\n");
		tell_object(ob,query("name")+"匆匆离开。\n");
		ob->delete_temp("songxin_zuji");
	}
	destruct(this_object());
}

void dest3(object ob)
{
	if (ob) {
		tell_object(ob,query("name")+"大笑道：完成任务了！\n");
		tell_object(ob,query("name")+"匆匆离开。\n");
	}
	destruct(this_object());
}
