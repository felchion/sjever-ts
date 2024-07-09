// mmr.h for hs job

int do_clone(object me, object ob);

int count = 0;
void init()
{
	object ling, name1;
	object ob = this_player();
	name1 = ob->query("id");
	ling = present("ling pai");

	::init();
	if (interactive(ob) 
	&& !environment(ob)->query("no_fight")
	&& ling
	&& ling->query_temp("mmr") == name1
	&& ling->query_temp("hsjob") == name1
	&& query_temp("mmr") == name1
	&& ob->query_temp("hs/jobji")
	&& ob->query_temp("hs/have")){
		if (count++) {
			if (count == 10) {
				remove_call_out("kill");
				call_out("dest", 1);
			}
		}
		else {
			set_leader(ob);
			ob->start_busy(2);
			this_object()->start_busy(2);
			if (!query_temp("skillset")) do_clone(this_object(), ob);
			call_out("kill", 1, ob); 
		}
	}
}

void kill(object ob)
{
	command("say 想跑，大爷今天就超渡你！！！");
	kill_ob(ob);
}

void copy_name(object ob)
{
	mapping name;
	int i;

	i = random(3)+ 1;

	name = RNAME_D->get_random_name(i);

	if (!ob) return;
		
	set_name(name["name"], name["id"]);		
	/*
	if ( ob->query_temp("hs/power")) {		
		message_vision(CYN"\n$N伸手拉掉面罩，大喝一声：老子坐不改名，行不改姓，"+name["name"]+"是也。\n"NOR, this_object());
	}
	*/	
}	

void kill_ob(object ob)
{
	if (ob->query("id") != query_temp("mmr")) {
		ob->remove_enemy(this_object());
		ob->remove_killer(this_object());
		this_object()->remove_enemy(ob );
		this_object()->remove_killer(ob );
		tell_object(ob, "这不是抢走你令牌的人。\n");
		return;
	}
	::kill_ob(ob);
}

//ob->set_temp("done",done);
int do_clone(object me, object ob)
{
	mapping killerskill1;
	int number,k_qi,k_neili,k_jing;
	string t_force,t_dodge;	
	int done;
		
	done = (int)query_temp("done",1);
	
	set("max_cure",5);
	
	number = (int)ob->query("max_pot");
	number = number-100;
	k_qi = (int)ob->query("max_qi")-ob->query_temp("adjust/qi");
	k_jing = (int)ob->query("max_jing")-ob->query_temp("adjust/jing");
	k_neili = (int)ob->query("max_neili");

	killerskill1 = skill2[random(sizeof(skill2))];

	me->set("dex", 10+random(21));
	me->set("str", 10+random(21));
	me->set("con", 10+random(21));
	me->set("int", 10+random(21));
	
	if(done==0){
		delete("max_cure");//不恢复
		me->delete("chat_msg");//不移动
		me->set("max_qi", k_qi);
		me->set("max_jing", k_jing);
		me->set("max_neili", k_neili);
		number = number-5-random(10);
		//难度降低
	}
	
	
	t_force = k_force[random(sizeof(k_force))];
	t_dodge = k_dodge[random(sizeof(k_dodge))];       
	
	copy_name(me);
	set_name(query("name"),parse_command_id_list()+({ ob->query("id")+"'s mengmian ren"  }) );
	
	if( random(5)==1 && done>=1 ) add_money("gold",random(3)+1)->move(me);
	
	//for robot

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

	me->set("max_qi",k_qi*7/5);
	me->set("eff_qi",k_qi*7/5);
	me->set("qi",k_qi*7/5);
	me->set("max_jing",k_jing);
	me->set("eff_jing",k_jing);
	me->set("jing",k_jing);
	me->set("eff_jingli", ob->query("eff_jingli")*3/2);
	me->set("jingli", ob->query("eff_jingli")*3/2);
	me->set("max_neili",k_neili);
	me->set("neili", k_neili);
	me->set("jiali", number/(3+random(4)));
	me->set("combat_exp",ob->query("combat_exp"));
  

	if(done>=1){
		me->add_temp("apply/attack",  number / 10 + random(number / 10));
		me->set("neili", k_neili*3/2);
	}
		
	me->set_temp("skillset",1);
	remove_call_out("dest");
	call_out("dest",300);
	return 1;
}

void dest()
{
	if (!living(this_object())) {
		call_out("dest", 10);
		return;
	}
	if (count >= 5) command("say 大爷没时间陪你溜达！");
	else command("say 他奶奶的，这次算你命大，下回看老子还饶得了你！");
	say(name()+"匆匆离开。\n");
	destruct(this_object());
}

void unconcious()
{
	object me,ob;

	me = this_object();
	ob = find_player(query_temp("mmr"));
	if (!objectp(ob) || !me) return;
	if ( me->query("cure") < me->query("max_cure")-2 
	 && me->query("combat_exp") > 2500000
	 && !me->query_temp("must_die") ){
		me->receive_curing("qi", me->query("max_qi")/5);
		me->add("qi", me->query("max_qi")/5);
		ob->kill_ob(me);
		me->add("cure", 1);
		return;
	}
	::unconcious();
}

void die()
{
	object me,ob;

	me = this_object();
	ob = find_player(query_temp("mmr"));

	if (!objectp(ob) || !me ) return;
	if ( me->query("cure") < me->query("max_cure")-2 
	 && me->query("combat_exp") > 2500000
	 && !me->query_temp("must_die") ){
	  me->receive_curing("qi", me->query("max_qi")/5);
		me->add("qi", me->query("max_qi")/5);
		ob->kill_ob(me);
		me->add("cure", 1);
		return;
	}
	::die();
}
