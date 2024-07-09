// char.c
// Modified by snowman@SJ 07/04/10
// added query_temp(apply/xxx)

// Modified by mxzhao 2004/04/28 for add "env/wimpycmd"

//#pragma save_binary

#include <action.h>
#include <ansi.h>
#include <command.h>
#include <condition.h>
#include <dbase.h>
#include <move.h>
#include <name.h>
#include <skill.h>
#include <team.h>
#include <user.h>
#include <job_mul.h>
#include <mudlib.h>


inherit F_ACTION;
inherit F_ALIAS;
inherit F_APPRENTICE;
inherit F_ATTACK;
inherit F_ATTRIBUTE;
inherit F_COMMAND;
inherit F_CONDITION;
inherit F_DAMAGE;
inherit F_DBASE;
inherit F_EDIT;
inherit F_MESSAGE;
inherit F_MORE;
inherit F_MOVE;
inherit F_NAME;
inherit F_SKILL;
inherit F_TEAM;
inherit F_LINEUP;

// Use a tick with longer period than heart beat to save cpu's work
static int tick,atb,heal_tick,cnd_flag;

static int save = 15 + random(15);
// static int exp_tick = 1;
// static int exp = 0;

void create()
{
	seteuid(0); // so LOGIN_D can export uid to us
}

// Use this function to identify if an object is a character.
int is_character() { return 1; }

  //modified by felchion@sjever 2016.2.3
  //把战斗检查移出物件心跳，改用call_out机制来检查
  
void check_fighting()  {
  int wimpy_ratio;
  int sk_spd = query_temp("fight/speed");
  if(!sk_spd) sk_spd = 100;

	sk_spd += query_temp("apply/speed");
	if(sk_spd<0) sk_spd=0;
	if (is_fighting()) {
		atb += sk_spd*query_dex(2);
		while(atb>30000) {
			if ( intp(wimpy_ratio = (int)query("env/wimpy"))
			&& wimpy_ratio > 0
			&& (query("qi") * 100 / (query("max_qi")+query_temp("apply/qi")) <= wimpy_ratio
			|| query("jing") * 100 / (query("max_jing")+query_temp("apply/jing")) <= wimpy_ratio ))	{	// Modified by mxzhao 2004/04/28
				int success = 0;
				string wimpycmd = query("env/wimpycmd");

				if(!is_busy()){
					if (stringp(wimpycmd)){
						int count = 0;
						foreach (string cmd in explode(wimpycmd, "\\"))	{
							if (count++ > 4)	break;
							if (command(process_input(cmd)))	success++;
						}
					}
					if (success == 0) GO_CMD->do_flee(this_object());
				}
			}

			if(!query_temp("combat_yield")) attack();

			atb -=30000;
		}
	}
	call_out("check_fighting",0.2);
}

// setup: used to configure attributes that aren't known by this_object()
// at create() time such as living_name (and so can't be done in create()).
void setup()
{
	if (!clonep()) return;
	seteuid(getuid());

	set_heart_beat(1);
	tick = 9 + random(3);
	enable_player();
  
	CHAR_D->setup_char(this_object());
}

void set_hb(int i){
 if(i) set_heart_beat(1);
 else set_heart_beat(0);
}

void check_cnd() {  	
	if(query_heart_beat() && cnd_flag && query_entire_conditions()) {
		if(!is_ghost())update_condition();
		call_out_walltime("check_cnd",20);
	}
	else cnd_flag = 0;
}	

void heart_beat()
{
	mapping my;
	object ob,link_ob;

	if (!environment()) return;
	if ((link_ob = query_temp("link_ob")) && !link_ob->is_character() && --save == 0) {
		save = 285 + random(30);
		link_ob->save();
		this_object()->save();
		write(HIG "您的资料已经自动保存好了。\n" NOR);
	}

	ob = this_object();
	my = query_entire_dbase();
	
	if(query_entire_conditions()) {
		if(!cnd_flag){
		  cnd_flag = 1;
		  check_cnd();
		}
		update_f_condition();	
	}	

	// check user's client type
	if( userp(ob) ) CLIENT_D->CheckStateChange(ob);

	if (my["neili"] < 0) my["neili"] = 0;
	if (my["max_neili"] < 0) my["max_neili"] = 0;
	if (my["combat_exp"] < 0) my["combat_exp"] = 0;
	if (my["potential"] < 0) my["potential"] = 0;

	// here is the change.
	
	if(query_temp("in_cp")) CP_D->cp_update(ob);
	
	if(query_temp("apply/auto_heal")>100){
		if(my["eff_qi"] > my["max_qi"] * query_temp("apply/auto_heal")/100)
		my["eff_qi"] = my["max_qi"] * query_temp("apply/auto_heal")/100;
	}
	else if (my["eff_qi"] > my["max_qi"])
	my["eff_qi"] = my["max_qi"];
	
	if (my["qi"] > my["eff_qi"] + query_temp("apply/qi"))
	my["qi"] = my["eff_qi"] + query_temp("apply/qi");
	/*
	if (userp(ob) && --exp_tick == 0) {
	exp_tick = 300;
	if (exp > 0) {
	exp -= my["combat_exp"];
	if (exp < -800) message("wizard:yuj", sprintf("%s:%d\n", my["id"], -exp), users());
	}
	exp = my["combat_exp"];
	}
	*/
	// If we are dying because of mortal wounds?

	// and here
	if( (my["eff_qi"] + query_temp("apply/qi")) < 0
	|| (my["eff_jing"] + query_temp("apply/jing")) < 0 ) {
		remove_all_enemy();
		die();
		return;
	}

	// If we're dying or falling unconcious?
	if( my["qi"] < 0 || my["jing"] < 0 || my["jingli"] < 0) {
		remove_all_enemy();
		if( !living(ob) ) die();
		else unconcious();
		return;
	}

	continue_action();

	if (!is_fighting()) {
		if(query_temp("in_fight")) {
			atb=0;		
			delete_temp("combat_time");
			delete_temp("fight_time");
			add_temp("clean_AP",1);
			remove_call_out("check_fighting");
			if (query_temp("clean_AP") > 2 ) {
				delete_temp("perform_AP");
				delete_temp("combat_AP");
				delete_temp("clean_AP");
				delete_temp("last_ap_give");
				delete_temp("fight");
				delete_temp("in_fight");				
				delete_temp("performing");
				delete_temp("start_performing");
			  delete_temp("apply/action");    
			}
	  }
	}
  else {
  	if (find_call_out("check_fighting")==-1) 
  	check_fighting();  	
  	
	
  	delete_temp("clean_AP");
  	add_temp("fight_time",1);
  	set_temp("in_fight",1);
  }
  if (is_busy()) {
		// halt from dazuo, tune, heal ... etc.
		if (query_temp("pending") && is_fighting() && !userp(ob))
		interrupt_me();

		// We don't want heart beat be halt eventually, so return here.
		//	      return; // 这里如果返回会造成 condition 无法更新，所以屏蔽掉。
	}
	else
	{
		// Is it time to flee?

		// End

		// Do attack or clean up enemy if we have fleed.

	}
  
  if(heal_tick > 4) {
  	heal_up(); 
  	heal_tick=0;
  }
  else heal_tick++;

	
	if( !ob ) return;	
	TASK_D->heart_beat_event(ob);
	
	if( !userp(ob) ) {
		ob->chat();
		// chat() may do anything -- include destruct(this_object())
		if (!ob) return;
	}

	if( tick-- ) return;
	tick = 7 + random(5);
	if( userp(ob) ) UPDATE_D->check_inventory(ob);	
	if(!ob) return; 
//	BBS_D->user_sync(ob);
	
	//write(sprintf("%d\n",time()));
//	if (!is_ghost()) {
//		cnd_flag = update_condition();
//		if( !ob ) return;
//	}

	// heal_up() must be called prior to other two to make sure it is called
	// because the && operator is lazy :P
	//if( (cnd_flag & CND_NO_HEAL_UP) || !heal_up());

	if (!interactive(ob)) {
//		if (!query_condition("killer") && !is_fighting()
//		&& !sizeof(filter_array(all_inventory(environment()), (: interactive :))))
//		set_heart_beat(0);
		return;
	}

	// Make us a bit older. Only player's update_age is defined.
	// Note: update_age() is no need to be called every heart_beat, it
	//       remember how much time has passed since last call.
	set("last_on",time());
	
	if(objectp(link_ob)){
		//write("last on "+time()+"\n");
		link_ob->set("last_on",time());
	}
	
	ob->update_age();

	if (query_idle(ob) > IDLE_TIMEOUT)
	//if (query_idle(ob) > 10)
	{
		//ob->user_dump(DUMP_IDLE);
		call_out("eval_function", 1,(:call_other,ob,"user_dump",DUMP_IDLE:));
	}
}

int visible(object ob)
{
	int lvl, invis;

	if (geteuid() == "felchion"  && query("env/debug"))	{
		return 1;
	}

	lvl = wiz_level(this_object());		
	if( lvl > wiz_level(ob) ) return 1;
	
	invis = wizardp(ob)?(int)ob->query("env/invisibility"):!userp(ob)?(int)ob->query("env/invisibility"):0;
	if( intp(invis) && (invis > lvl) ) return 0;

	if ( ob->is_ghost() && !userp(ob) ) return 0;
	if(objectp(ob) && ob->hide(this_object())) return 0;
	return 1;
}

int is_vip()
{
	return query("registered")>=3;
}

int is_die_dummy()
{
	return query("honor_point")<-5000;
}

int shared_id(mixed ob)
{
	string id;	
	int st,sr;
	
	if(!ob) return 0;
	
	if(stringp(ob)) id = ob;
	
	if(objectp(ob) && userp(ob)) {
		id=ob->query("id");
	  if (query("ID_Bind") 
	  && (query("ID_Bind") == ob->query("ID_Bind"))) 
	    return 10;
	}
	
	if (st=query("mlogin/"+id)) {		
		sr = st*100/query("mud_time");		
		return sr/10;	
	}
	return 0;
}

int is_dummy()
{
	if (query("registered")>3) return 0;
	
	else if( query("registered")==3 
	&& query("week_rate/last_week_online") > 48*3600 
	&& query("week_rate/last_week_exp") > 150000 )
	return 0;
	
	else if( query("registered")==2 
	&& query("week_rate/last_week_online") > 96*3600 
	&& query("week_rate/last_week_exp") > 300000 )
	return 0;
	
	else 
	return 1;
}

int is_robot(int unset)
{
	if(unset)
	{
		delete("iamrobot");
		return 0;
	}
	else
	{
		return (int)query("iamrobot");
	}
}

int add_exp_combat(int iexp,mixed giver,string reason,int no_job,int gift_flag)
{
  return TASK_D->add_exp_combat(this_object(),iexp,giver,reason,no_job,gift_flag);
}

/*
this function get the party's status,and return 3 status
garrison 
*/

string party_status()
{
	string family;
	family = query("family/family_name");
	if (family=="武当派"||family=="古墓派"||family=="少林派"||family=="华山派"||family=="天龙寺"||family=="峨嵋派")
		return "正派";
	else if (family=="星宿派"||family=="大轮寺"||family=="神龙岛"||family=="嵩山派")	
		return "邪派";
	else
		return "中立派";	
}