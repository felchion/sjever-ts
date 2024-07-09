inherit F_SAVE;

string query_save_file() {return "/data/cpd";}

mapping reward_list=([]); 

void set_reward(object me,mapping reward,mapping task){
	mapping r=([]);
	if(IN_MASTER) return;
	r["reward"] = reward;
	r["task"] = task;
	r["flag"] = time();	
	reward_list[me->query("id")] = r;
	save();
	tell_object(me,CP_REWARD,1);
}

void get_reward(object me){
	mapping r;
	if(!IN_MASTER) return;
	restore();
	if(mapp(r=reward_list[me->query("id")])){
		if(me->query("CP_GOT")==r["flag"]) return;
		me->set("CP_GOT",r["flag"]);
		TASK_D->event_reward(me,r["reward"],r["task"],1);
	}
}

mapping query_reward(){return reward_list;}