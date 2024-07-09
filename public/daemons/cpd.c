#include <cp.h>
inherit F_SAVE;

string query_save_file() {return "/data/cpd";}

mapping reward_list;
mapping die_list;
mapping room_list;

void create(){ 
	if(IN_MAIN) restore();
	if(!mapp(room_list)) room_list=([]);
	if(!mapp(die_list)) die_list=([]);
	if(!mapp(reward_list)) reward_list=([]);
	if(!IN_MAIN) save();
}

void set_reward(object me,mapping reward,mapping task){
	mapping r=([]);
	if(IN_MAIN) return;
	r["reward"] = reward;
	r["task"] = task;
	r["flag"] = time();	
	reward_list[me->query("id")] = r;
	save();
}

void set_die(object me,string reason,string killer){
	mapping r=([]);
	if(IN_MAIN) return;
	r["reason"] = reason;
	r["flag"] = time();
	r["killer"] = killer;
	die_list[me->query("id")] = r;
	save();
}

void set_room(object me,string room){
	mapping r=([]);
	if(IN_MAIN) return;
	r["room"] = room;
	r["flag"] = time();
	room_list[me->query("id")] = r;
	save();
}

void cp_update(object me){
	mapping r;
	if(!IN_MAIN) return;
	restore();
	if(mapp(r=room_list[me->query("id")])){
		if(me->query("CP_RM")!=r["flag"]){
			me->set("CP_RM",r["flag"]);			
			me->move(r["room"]);
		}
	}
	if(mapp(r=reward_list[me->query("id")])){
		if(me->query("CP_GOT")!=r["flag"]){
			me->set("CP_GOT",r["flag"]);
			TASK_D->event_reward(me,r["reward"],r["task"],1);
		}
	}
	if(mapp(r=die_list[me->query("id")])){
		object killer;
		if(me->query("CP_DIE")!=r["flag"]) {
			me->set("CP_DIE",r["flag"]);
			if(stringp(r["killer"]) && objectp(killer=find_player(r["killer"])))
			me->set_temp("last_damage_from",killer);
			else me->set_temp("last_damage_from",r["reason"]);
			me->die();
		}
	}
}

int invalid_move(object me,object ob){
	if(!IN_MAIN && userp(me) && ob->query("cpid")!=me->query("id") 
		&& base_name(ob)[0..9]!="/d/huashan" 
		&& base_name(ob)[0..4]!="/d/vi" 
		&& base_name(ob)[0..15]!="/d/xingxiu/silk3" ) 
	return 1;
}

mapping entrance_list = ([
	"/d/xiangyang/shanxiroad2" : (["north" : "/d/village/hsroad3"]),
	"/d//d/group/entry/caeroad3" : (["southeast" : "/d/village/shilu5"])
]);

mapping exit_list = ([
	"/d/village/shilu5" : (["northwest" : "/d/group/entry/caeroad3"]),
	"/d/village/hsroad3" : (["south" : "/d/xiangyang/shanxiroad2"])
]);


int enter(object me,object env,string dir){
	if(!IN_MAIN) return 0;	
	if(mapp(entrance_list[base_name(env)]) && entrance_list[base_name(env)][dir]){
		me->enter_cp(entrance_list[base_name(env)][dir]);
		return 1;
	}
	return 0;	
}

int exit(object me,object env,string dir){
	if(IN_MAIN) return 0;
	if(mapp(exit_list[base_name(env)]) && exit_list[base_name(env)][dir]) {
		if(stringp(me->query_temp("cp_no_leave"))){
			tell_object(me,me->query_temp("cp_no_leave"));
			return 1;
		}
		me->close_cp(exit_list[base_name(env)][dir]);
		return 1;
	}
	return 0;
}