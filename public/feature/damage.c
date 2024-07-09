// Modified by snowman@SJ 22/08/2000.
// 在YuJ增加无食水减jing,qi的基础上，屏蔽对newbie的影响。
// Modified by snowman@SJ 13/04/2001
// heal 中增加对apply/xxx 的支持。
// add apply/re_xx Ciwei@SJ
// 增加根骨影响 felchion 2008.03.13

#include <ansi.h>
#include <dbase.h>
#include <login.h>
#include <move.h>
#include <cp.h>

int ghost = 0;

static mapping extra_listener = ([]);
int is_ghost() { return ghost; }
void set_ghost(int i) { ghost=i; }
int max_food_capacity() { return query("str")*3+query("con")*7; }
int max_water_capacity() { return query("str")*3+query("con")*7; }

mapping query_extra_listener()
{
	return extra_listener;
}

void register_relay_listener(string type)
{
	object ob;

	ob = previous_object();
	if (!ob) return;	
	if (arrayp(extra_listener[type])) {
		if (member_array(ob, extra_listener[type]) >=0) return;
		extra_listener[type] += ({ ob });
	} else
		extra_listener[type] = ({ ob });
}

void remove_relay_listener(string type)
{
	object ob;

	ob = previous_object();
	if (!ob) return;
	if (arrayp(extra_listener[type])) {
		if (member_array(ob, extra_listener[type]) == -1) return;
		extra_listener[type] -= ({ ob });
	}
}

varargs int receive_damage(string type, int damage, mixed who)
{
	int lvl,val;
	object me = this_object();

	if( damage < 0 ) damage = 0;

	if( type!="jing" && type!="qi" && type!="jingli" && type!="neili")
	error("F_DAMAGE: 伤害种类错误。\n");

	//added by snowman for some shadow npcs' hit
	//added by felchion for prevention of murderer quit immediately
	
	if( objectp(who) ){
		if(type=="qi" && me->is_fighting(who)) {
			if( (lvl=QSKILL_D->quest_lvl(me,"bmyq"))
			&& QSKILL_D->quest_lvl(me,"bmsg")>5
			&& random(100) < 20 + lvl*5
			&& me->query("neili")>damage*2/lvl) {
				me->add("neili",-damage*2/lvl);
				message_vision(ansi_color(98)+"$N运起北冥神功，用内力化解了$n造成的伤害。\n"+ansi_color(0),me,who);
				return 0;				
			}
		}
		
		if(objectp(who->query("master_user/user")) )
		set_temp("last_damage_from", who->query("master_user/user"));
		add_temp("combat_been_damage/"+type,damage);
		//added by felchion for prevention of murderer quit immediately
		if (userp(this_player()) && userp(who))
		who->set_temp("hurt_player_time",time());
	}
  
  if (who) set_temp("last_damage_from", who);
	
	val = query(type) - damage;

	if( val < 0 ) val = -1;
	if(set(type, val)<0) call_out("heart_beat",0.4);
	if(type=="qi") set_temp("last_damage_time",time());
	set_heart_beat(1);

	return damage;
}

varargs int receive_wound(string type, int damage, mixed who)
{
	int lvl,val;
	object me=this_object();
	
	if (damage < 0) damage = 0;
	if (type!="jing" && type!="qi" && type!="neili")
		error("F_DAMAGE: 伤害种类错误。\n");

	//add constitution effect modify by felchion 2008.03.13
	if (userp(me) 
	&& (type=="qi" || type=="jing") 
	&& living(me)
	&& me->query("qi")>0 
	&& me->query("jing")>0 ) {
		if (type=="qi" && who && objectp(who) 
		&& who->is_character() && me->is_fighting(who)) {			
			if ((me->query_con(2)-who->query_str(2))>random(40)) return 0;			
			if( (lvl=QSKILL_D->quest_lvl(me,"bmyq")) 
			&& QSKILL_D->quest_lvl(me,"bmsg")==10
			&& me->query("neili")>damage*2/lvl) {
				me->add("neili",-damage*2/lvl);
				message_vision(ansi_color(98)+"$N运起北冥神功，用内力化解了$n造成的损伤。\n"+ansi_color(0),me,who);
				return 0;				
			}
			set_temp("last_hurt_time",time());
		}				
	}
	
	//added by snowman for some shadow npcs' hit
	if (who) set_temp("last_damage_from", who);
	if( objectp(who) ){
		if(objectp(who->query("master_user/user")) )
		set_temp("last_damage_from", who->query("master_user/user"));
		add_temp("combat_been_wound/"+type,damage);
		//added by felchion for prevention of murderer quit immediately
		if (userp(me) && userp(who)) 
		  who->set_temp("hurt_player_time",time());	
	}

	if (type == "neili") val = query("max_" + type) - damage; 
	else val = query("eff_" + type) - damage;

	val += query_temp("apply/" + type);
	if (val < 0) val = -1;
	if (val < 0 && type == "neili") val = 0;
	if (query(type) > val && type != "neili") set(type, val);
	val -= query_temp("apply/" + type);

	if (type == "neili") set("max_" + type, val);
	else if(set("eff_" + type, val)<0) call_out("heart_beat",0.1);

	set_heart_beat(1);

	return damage;
}

int receive_heal(string type, int heal)
{
	int val;

	if (heal < 0 ) heal = 0;
	if (type!="jing" && type!="qi" && type!="jingli")
		error("F_DAMAGE: 恢复种类错误。\n");

	val = query(type) + heal;
	if (val > query("eff_" + type) + query_temp("apply/" + type))
		val = query("eff_" + type) + query_temp("apply/" + type);

	set(type, val);

	return heal;
}

int receive_curing(string type, int heal)
{
	int max, val;

	if (heal < 0) heal = 0;
	if (type!="jing" && type!="qi")
		error("F_DAMAGE: 恢复种类错误。\n");

	val = query("eff_" + type) + heal;
	max = query("max_" + type);

	if (val > max)
		val = max;
	set( "eff_" + type, val);
	return heal;
}

void unconcious()
{
	object me = this_object(), defeater;
	int i;

	if (!living(me)) return;
	if (wizardp(me) && query("env/immortal")) return;

	if (objectp(defeater = query_temp("last_damage_from")))
		COMBAT_D->winner_reward(defeater, me);

	me->remove_all_enemy();
	set_temp("faint_by", defeater);

	tell_object(me, HIR "\n你只觉得头昏脑胀，眼前一黑，接着什么也不知道了……\n\n" NOR);
	command("hp");
	me->disable_player(" <昏迷不醒>");

	if ((i = query("jing")) < 0) i = 0;
	set("jing", i/2);
	if ((i = query("qi")) < 0) i = 0;
	set("qi", i/2);
	if ((i = query("jingli")) < 0) i = 0;
		
	set("jingli", i-i/4);
	//added by snowman
	if ((i = query("neili")) < 0) i = 0;
	set("neili", i/2);

	set_temp("block_msg/all", 1);
	COMBAT_D->announce(me, "unconcious");

	call_out("revive", 30 + random(60 - query("con")));
	// grin YUJ@SJ 2001-05-08 06-03
	if (userp(me) && previous_object()) {
		set("no_get", "你刚想动手，对方似乎动了一下。\n");
		set("no_get_from", 1);
	} else if (userp(me)) {
		set("no_get", "对方还没有完全昏迷，等等再背吧。\n");
		call_out("delete", 10, "no_get");
	}
}

varargs void revive(int quiet)
{
	object me = this_object();

	remove_call_out("revive");
					
	// grin YUJ@SJ 2001-06-03
	if (userp(me)) {
		delete("no_get");
		delete("no_get_from");
	}
	// changed environment(environment()) into environment()->is_character(), by snowman
	while( environment()->is_character() ) me->move(environment(environment()));
	me->enable_player();
	if (!quiet) {
		COMBAT_D->announce(me, "revive");
		delete_temp("block_msg/all");
		tell_object(me, HIY "\n一股暖流发自丹田流向全身，慢慢地你又恢复了知觉……\n\n" NOR);
		command("hp");
		if (query_temp("unconcious") && query_temp("unconcious") != environment()) move(environment(), 1);
		if (environment()) environment()->relay_revive(me);
		//When revive delete this temp by Ciwei@SJ
		delete_temp("faint_by");
	} else delete_temp("block_msg/all");
	
	delete_temp("unconcious");
}

void linkoff(object me,object ob){	
	if(ob) destruct(ob);
	if(me) destruct(me);
}

void die()
{
	object corpse;
	object me = this_object();
	mixed killer, npc_killer;
	int db_exp,true_view,ggss,tp_exp,smy;

	if (!living(me)) revive(1);
	else delete_temp("faint_by");

	if( is_ghost() ) return;
	
	if(!IN_MAIN && userp(me)){
		killer = query_temp("last_damage_from");
		if(userp(killer)) {
			CP_D->set_die(me,"被"+killer->name()+"杀死了。",killer);
			call_out("linkoff",1,me,me->query_temp("link_ob"));
			return;
		}
		if(objectp(killer))
		killer=sprintf("被%s杀死了。", killer->name());
		else if(!killer)
		killer = "莫名其妙地";
		
		CP_D->set_die(me,killer,0);
		call_out("linkoff",1,me,me->query_temp("link_ob"));
		return;
	}
	
	if (arrayp(extra_listener["die"])) 
	{
		int temp,ret=0;
		extra_listener["die"] -= ({ 0 });
		if (sizeof(extra_listener["die"])){
			for(int i=0;i<sizeof(extra_listener["die"]);i++)
			{
				temp = extra_listener["die"][i]->relay_listener(me, "die");
				ret = temp?temp:ret;
			}
		}
		if(ret) return;
	}//run the listener here 
	
	if( wizardp(me) && query("env/immortal") )
	{
		return;
	}

	COMBAT_D->announce(me, "dead");
	command("hp");

	if (!(killer = query_temp("last_damage_from")))
		killer = "莫名其妙地";

	npc_killer = query_temp("faint_by");

	if (!environment()->query("no_death") || !userp(me)) {
		if( objectp(corpse = CHAR_D->make_corpse(me, killer)) ) {
			corpse->move(environment()); 
			if( userp(me) && GROUP_D->is_group_fight(me)==1 )
				corpse->set("group_war",1);
		}	
		if (userp(me)) {
			int qt_exp;
			
			COMBAT_D->killer_reward(killer, me);
			

			
			if(me->query("Extra_Life")>0  && objectp(killer) && !userp(killer) && !killer->query("smart_fight") ) {
			  me->add("Extra_Life",-1);
			  message_vision(HIR"\n$N的尸体上突然跳起一簇火焰，火光中隐约可以看到一只金色大鸟的虚影。\n倾刻之间，尸体就化为一堆灰烬，$N在灰烬中又站了起来。\n\n"NOR,me);
			 	CHANNEL_D->do_channel(me, "rumor", HIR"据说"+query("name")+HIR"浴火重生了。\n"NOR);
			 	set("eff_jing", query("max_jing"));
      	set("eff_qi", query("max_qi"));
      	set("jingli", query("eff_jingli"));
      	set("jing", query("eff_jing"));
      	set("qi", query("eff_qi"));
      	set("neili", query("max_neili"));
      	set("food", max_food_capacity());
      	set("water", max_water_capacity());
			  set_temp("invulnerability", time()+10);
			  me->clear_conditions_by_type("poison");
	      me->clear_conditions_by_type("hurt");
			  me->clear_conditions_by_type("wound");
			  TASK_D->clean_hb_event(me,1);
			  destruct(corpse);
			  return;		
			}
			
			TASK_D->clean_hb_event(me);
			// Clear all the conditions by death.
			// and some condtiions can not be cleared
			
			db_exp = me->query_condition("db_exp");
			tp_exp = me->query_condition("tp_exp");
			qt_exp = me->query_condition("qt_exp");
			true_view = me->query_condition("true_view");
			ggss = me->query_condition("ggss");
			smy = me->query_condition("smy_job");			  
			me->clear_condition();		
			if (qt_exp > 0)	me->apply_condition("qt_exp",qt_exp);
			if (db_exp > 0)	me->apply_condition("db_exp",db_exp);
			if (true_view > 0)	me->apply_condition("true_view",true_view);
			if (ggss > 0)	me->apply_condition("ggss",ggss);
			if (tp_exp > 0)	me->apply_condition("tp_exp",tp_exp);
			if (smy > 0) me->apply_condition("smy_job",smy);			
			
			set_temp("last_damage_from", "莫名其妙地");
			me->remove_all_killer();
			all_inventory(environment())->remove_killer(me);
			me->start_call_out( (: call_other, me,"dismiss_team" :), 3 );
			//call_out me->dismiss_team();
			if (me->is_busy()) {
					me->start_busy(1, 2);
					me->interrupt_me();
			}
			set("jing", 1); set("eff_jing", 1);
			set("qi", 1); set("eff_qi", 1);
			ghost = 1;				
			me->move(DEATH_ROOM);
			me->save();
			DEATH_ROOM->start_death(me);
		}					
		else 
			destruct(me);		
	}
	else {
		tell_object(me,HIY"突然你感到一股神力充斥在空间之中，你似乎又恢复了一线生机。\n"NOR);
		me->set("eff_qi",1);
		me->set("eff_jing",1);
		me->clear_conditions_by_type("hurt");
		me->clear_conditions_by_type("wound");
		me->clear_conditions_by_type("poison");
		log_file("no_death", sprintf("%O %O\n", me, environment()));
	}			
}



void reincarnate()
{
	ghost = 0;
	
	//by felchion 2020.06.29
	if(query("max_jing")<100) set("max_jing",100);
	if(query("max_qi")<100) set("max_qi",100);
	
	set("eff_jing", query("max_jing"));
	set("eff_qi", query("max_qi"));
	set("jingli", query("eff_jingli"));
	set("jing", query("eff_jing"));
	set("qi", query("eff_qi"));
	set("neili", query("max_neili"));
	set("food", max_food_capacity());
	set("water", max_water_capacity());
}

int heal_up()
{
	int update_flag, i;
	object me = this_object();
	mapping my;

	if (ghost || environment()->query("no_update")) return 0;
	update_flag = 0;

	my = query_entire_dbase();
	if ( my["food"] > max_food_capacity()+50 )
		my["food"] = max_food_capacity()+50;
	if ( my["water"] > max_water_capacity()+50 )
		my["water"] = max_water_capacity()+50;

	if (my["water"] > 0 && random(my["con"] + 20) < 10)  { my["water"]--; update_flag++; }
	if (my["food"] > 0 && random(my["con"] + 20) < 10) { my["food"]--; update_flag++; }
	if (me->is_busy() ) return update_flag;
	if (userp(me) && !wizardp(me)) {
		if( my["water"] < 1 ) {
			message_vision("$N渴得眼冒金星，全身无力。\n", me);
			if (random(4)) me->start_busy(1);
			else if (me->query("age") > 16) {
				receive_wound("jing", my["max_jing"]/(3*my["con"]));
				me->set_temp("last_damage_from", "脱水时间太长渴");
			}
			return update_flag;
		}
		switch(my["water"]){
			case 50:
			case 40:
			case 30:
			case 20:
			case 10:
				message_vision(HIY"$N舔了舔干裂的嘴唇，看来是很久没有喝水了。\n"NOR, me);
		}
	}
	i = my["con"] / 3 + my["max_jingli"] / 10;
	if(me->query_temp("apply/re_jing")) i += my["max_jing"] * (me->query_temp("apply/re_jing"))/100;
	//if (me->is_fighting()) i /= 3;
	my["jing"] += i;
	if (my["jing"] >= my["eff_jing"] + me->query_temp("apply/jing")) {
		my["jing"] = my["eff_jing"] + me->query_temp("apply/jing");
		if( my["eff_jing"] < my["max_jing"] ) {
			my["eff_jing"]++;
			my["eff_jing"] += my["max_jing"] * me->query_temp("apply/auto_heal")/100;
			if( my["eff_jing"] > my["max_jing"] ) my["eff_jing"] =  my["max_jing"];
			update_flag++;
		}
	} else update_flag++;

	i = my["con"] / 3 + my["max_neili"] / 10;
	if(me->query_temp("apply/re_qi")) i += my["max_qi"] * (me->query_temp("apply/re_qi"))/100;
	//if (me->is_fighting()) i /= 3;
	
	if( my["eff_qi"] < my["max_qi"] ) {
		my["eff_qi"]++;
		my["eff_qi"] += my["max_qi"] * me->query_temp("apply/auto_heal")/100;
		update_flag++;
	}
	else if(me->query_temp("apply/auto_heal")>100){
		if(my["eff_qi"] < my["max_qi"] * me->query_temp("apply/auto_heal")/100){
			my["eff_qi"] += random(my["max_qi"] * (me->query_temp("apply/auto_heal")-100)/200);
			if(my["eff_qi"] > my["max_qi"] * me->query_temp("apply/auto_heal")/100)
			my["eff_qi"] = my["max_qi"] * me->query_temp("apply/auto_heal")/100;
			update_flag++;
		}
	}
	else if( my["eff_qi"] > my["max_qi"] ) my["eff_qi"] =  my["max_qi"];

	my["qi"] += i;
	if (my["qi"] >= my["eff_qi"] + me->query_temp("apply/qi")) {
		my["qi"] = my["eff_qi"] + me->query_temp("apply/qi");
		update_flag++;		
	} else update_flag++;

	if (userp(me) && !wizardp(me)) {
		if( my["food"] < 1 ) {
			message_vision("$N饿得头昏眼花，直冒冷汗。\n", me);
			if (random(4)) me->start_busy(1);
			else if( me->query("age") > 16) {
				receive_wound("qi", my["max_qi"]/(3*my["con"]));
				me->set_temp("last_damage_from", "太久没有进食饿");
			}
			return update_flag;
		}
		switch(my["food"]){
			case 50:
			case 40:
			case 30:
			case 20:
			case 10:
				message_vision(HIY"突然一阵“咕咕”声传来，原来是$N的肚子在叫了。\n"NOR, me);
		}
	}
	if (my["jingli"] < my["eff_jingli"] + me->query_temp("apply/jingli")) {
		i = my["con"] / 2 + me->query_skill("force", 1) / 3;
		if(me->query_temp("apply/re_jingli")) i += my["max_jingli"]  * (me->query_temp("apply/re_jingli"))/100;
		//if (me->is_fighting()) i /= 3;
		i += my["eff_jingli"]*me->query_temp("apply/auto_charge")/100;
		my["jingli"] += i;
		if (my["jingli"] > my["eff_jingli"] + me->query_temp("apply/jingli"))
			my["jingli"] = my["eff_jingli"] + me->query_temp("apply/jingli");
		update_flag++;
	}

	if (my["neili"] < my["max_neili"] + me->query_temp("apply/neili")) {
		i = me->query_skill("force", 1) / 2 + my["con"];
		if(me->query_temp("apply/re_neili")) i += my["max_neili"] * (me->query_temp("apply/re_neili"))/100;
		//if (me->is_fighting()) i /= 3;
		i += my["max_neili"]*me->query_temp("apply/auto_charge")/100;
		my["neili"] += i;
		if (my["neili"] > my["max_neili"] + me->query_temp("apply/neili"))
			my["neili"] = my["max_neili"] + me->query_temp("apply/neili");
		update_flag++;
	}
	else if(me->query_temp("apply/auto_charge")>100 
	&& my["neili"] < my["max_neili"]*me->query_temp("apply/auto_charge")/100) {
		my["neili"] += random(my["max_neili"]*(me->query_temp("apply/auto_charge")-100)/200);
		if(my["neili"] > my["max_neili"]*me->query_temp("apply/auto_charge")/100) 
		my["neili"] = my["max_neili"]*me->query_temp("apply/auto_charge")/100;
		update_flag++;
	}

	if (my["neili"] > (my["max_neili"]+me->query_temp("apply/neili"))*2) {
		if (!me->query_temp("pending/exercise"))
			my["neili"] = (my["max_neili"] + me->query_temp("apply/neili")) * 2;
		update_flag++;
	}

	return update_flag;
}
