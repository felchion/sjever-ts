// user.c

#include <ansi.h>
#include <origin.h>
#include <user.h>
#include <mudlib.h>
#include <cp.h>


inherit CHARACTER;
inherit F_AUTOLOAD;
inherit F_SAVE;

void create()
{
	::create();
	set_name("使用者物件", ({ "user object", "user", "object" }) );
}

void look_room(){command("look");}

varargs void close_cp(mixed info){
	object lob = query_temp("link_ob"); 
	object me = this_object();
	if(IN_MAIN) return;
	if(stringp(info)) CP_D->set_room(me,info);
	tell_object(me,CP_CLOSE);
	if(objectp(lob)) destruct(lob);
	destruct(me);
}

varargs void enter_cp(string room){
	object tob,me=this_object();
	
	if(!IN_MAIN) return;
	
	if(objectp(tob=me->query_temp("telnet_ob"))) 
	destruct(tob);
	
	tob = new("/shadow/telnet");
	if (tob->do_shadow(me, 1) != me)
	{
		write("系统错误，副本生成失败。\n");
		if(stringp(room)) move(room);
		return;
	}
	me->set_temp("telnet_ob",tob);
	me->set("cp_startroom",room);
	me->save();
	
	tob->connect_to(me);
}

void terminal_type(string term_type)
{
	set_temp("terminal_type", term_type);
}

void window_size(int x, int y)
{
	set_temp("window_size/x", x);
	set_temp("window_size/y", y);
}

void telnet_suboption(string opt)
{
	set_temp("telnet_suboption", opt);
}

void reset()
{
	if (!query("jingli"))
		set("jingli", 10);
	if (query("potential") < query("max_pot")/2)
		add("potential", 1);
	if (query("thief") > 0)
		add("thief", -1);
	delete_temp("saved");
}

// This is used by F_SAVE to determine the filename to save our data.
string query_save_file()
{
	string id;

	id = geteuid();
	if (!id) id = getuid(); 
	if (!stringp(id)) return 0;
	return sprintf(DATA_DIR "user/%c/%s", id[0], id);
}

int save()
{
	int res;

	save_autoload();
	res = ::save();
	clean_up_autoload();		// To save memory
	return res;
}

static int last_age_set = uptime();

// This function updates player's age, called by heart_beat()



void update_age()
{
	object me = this_object(),ob;
	int same,minute, age = 14, month, modify = query("age_modify");
	int i,gap = uptime() - last_age_set;  
  int this_week=time()/86400/7;
  int lt,j;
  object *uob;
  string *ip;
  
  if (stringp(query_ip_number(me))) {
    ip=explode(lower_case(query_ip_number(me)), ".");	
  
		delete("age_modify");
	  add("mud_time", gap);
		set("kar",20);
		set("pur",10);
			
		uob = filter_array(users(),
			(: query_ip_number($1) == $(query_ip_number(me)) 
			&& $1->query("combat_exp") > 10000 
			:)
		);
		
		if ((same = sizeof(uob))>1) {
			for(i=0;i<sizeof(uob);i++) {
				if (uob[i]!=me ) {
					add("mip/"+ip[0]+"."+ip[1]+"/"+uob[i]->query("id"),gap);
					add("mlogin/" + uob[i]->query("id"),gap);
				}
				if(uob[i]->query("env/time_mining")
				&&uob[i]->query("combat_exp") > 1000000) j++;
			}
		}
		else j=1;
    
		if(query("combat_exp") > 1000000
		&& query("env/time_mining")
		&& (MUD_SITE_ID == "tj" || MUD_SITE_ID == "test")
		) 
 		  add("time_mining",gap/j);

		if (query("registered") == 2) same -= 1;
		if (query("registered") > 2) same = 1;
		if (same < 1) same = 1;		
		if (query("combat_exp")<10000) same = 1;
		if (!random(50) && same>1)
		  tell_object(me,BLINK+HIR+"系统提示：有多个玩家在你所在IP登录！\n"NOR);
		
		if (same<9) {
			add("eff_mud_time",to_int(gap/same));						
			if ( query("week_rate/week") != this_week  ) {
				if (this_week - query("week_rate/week") == 1 ) {
					set("week_rate/last_week_online",query("week_rate/this_week_online"));
					set("week_rate/last_week_exp",query("week_rate/this_week_exp"));
					set("week_rate/last_week_cmd",query("week_rate/cmd"));
			  }
				delete("week_rate/cmd");
				set("week_rate/this_week_online",0);
				set("week_rate/this_week_exp",0);
				set("week_rate/week",this_week);
			}
			add("week_rate/this_week_online",to_int(gap/same));			
    }
  }
	set("eff_online_rate",(query("eff_mud_time")+600)/(query("mud_time")/100+1));

  if(query("mud_time")-query("luck/drug/last_time") > 86400*2) delete("luck/drug");
  
  if (query("luck/drug/sharp")) lt = add("luck/trend",gap*2);  
  else if(query("luck/drug/smooth")) lt = add("luck/trend",gap/2);  
  else lt = add("luck/trend",gap);

  if ( lt > 3600 * 24) {
  	lt = 0;
  	set("luck/trend",0);
  	set("luck/phase",HIG"生"NOR);
    set("luck/affect", 0);
  }
  else if(lt < 3600 * 5) {
    set("luck/phase",HIG"生"NOR);
    set("luck/affect", lt*100/(3600*5));
  }	
  else if(lt > 3600 * 5 && lt < 3600*8) {
    set("luck/phase",HIR"旺"NOR);
    set("luck/affect", 100);
  }	
  else if(lt > 3600 * 8 && lt < 3600*13) {
    set("luck/phase",CYN"败"NOR);
    set("luck/affect", 100-(lt-3600*8)*100/(3600*5));
  }	
  else if(lt > 3600 * 13 && lt < 3600*18) {
    set("luck/phase","平");
    set("luck/affect", 0);
  }	
  else if(lt > 3600 * 18 && lt < 3600*22) {
    set("luck/phase",YEL"衰"NOR);
    set("luck/affect", -100*(lt-3600*18)/(3600*4));
  }
  else {
    set("luck/phase",HIB"死"NOR);
    set("luck/affect", -100);
  }	
  
  if(GIFT_D->query("NY_luck")) {
	  set("luck/affect", 100);
	  set("luck/phase",HIR"旺"NOR);
  }
  
  if (query("luck/affect") > 0) 
  set("luck/offset", query("luck/affect")*query_con(2)/200 );
	else 
	set("luck/offset", query("luck/affect")/4-query("luck/affect")*((query_con(2)>90?90:query_con(2))+10)/400 );
  
	  
  if (query("mud_age") / 6 > query("combat_exp")) add("mud_age", -gap/3);
	minute = add("mud_age", gap/3) / 60;
	if (minute <=8640) {
		age += minute / 1440;
		month = (minute % 1440) / 120;
	} else {
		minute -= 8640;
		age = 20;
		while ((minute -= age*240 - 3120) >= 0) age++;
		month = (minute + age*240 - 3120) * 12 / (age*240 - 3120);
	}
	if (modify > 0)
		while (modify--) {
			add("mud_age", 86400);
			if (age > 19) {
				add("mud_age", (age-19)*14400);
				add("mud_age", 1200*month);
			}
			age++;
		}
	else
		while (modify++) {
			add("mud_age", -86400);
			if (age > 20) {
				add("mud_age", (20-age)*14400);
				add("mud_age", -1200*month);
			}
			age--;
		}
	last_age_set += gap;
	if (query("age") != age) {
		set("age", age);
		tell_object(me, "时间过得真快，不知不觉你已经" + chinese_number(age) + "岁了。\n");
	  if(me->query("age_reyoung"))
	  tell_object(me, "但你的身体仍然和" + chinese_number(age-me->query("age_reyoung")) + "岁一样年轻。\n");
	  if (age == 16) {
	  	set("per",random(21)+10);
	  	if (query("ugly"))  set("per",random(5)+10);
	  	if (query("beauty"))  set("per",random(5)+26);	  	 
	  	set("per_reset",1);
	  	tell_object(me, "你终于长大成人了。\n");
	  }
	}
	if (query("month") != month) {
		set("month", month);
	}

	UPDATE_D->check_vip(me);
	
	add_temp("online_time", gap);
	add("online_total", gap);
	if (!query_temp("online_time")) {
		int t = time() - query("check_point");

		if ((t -= 3600) < 0) t = 0;
		if ((t *= 3) < 0) t =0;
		if (add("online_total", -t) < 0) delete("online_total");
	}
	set("check_point", time());
}


int query_aexp(){
	int *cp,ap,i;
  if (query("skill/attack")) {
		cp = values(query("skill/attack"));
		for(i=0;i<sizeof(cp);i++) ap+=cp[i];
		return ap;
  }
  else 
  return 0;
}

int query_dexp(){
	int *cp,ap,i;
  if (query("skill/defense")) {
		cp = values(query("skill/defense"));
		for(i=0;i<sizeof(cp);i++) ap+=cp[i];
		return ap;
  }
  else 
  return 0;
}
   
void setup()
{
	// We want set age first before new player got initialized with
	// random age.
	update_age();

	::setup();
	restore_autoload();
}

private void user_dump(int type)
{
	object me = this_object();

	switch(type) {
		case DUMP_NET_DEAD:
			tell_room( environment(), query("name") + "断线超过"
				+ chinese_number(NET_DEAD_TIMEOUT/60)
				+ "分钟，自动退出这个世界。\n");
			"/cmds/usr/quit"->main(me);
			break;
		case DUMP_IDLE:
			if (wiz_level(me) > 3) break;
			tell_object(me, "对不起，您已经发呆超过 "
				+ IDLE_TIMEOUT/60 + " 分钟了，请下次再来。\n");
			tell_room(environment(), "一阵风吹来，将发呆中的" + query("name")
				+ "化为一堆飞灰，消失了。\n", me);
			"/cmds/usr/quit"->main(me);
	}
}

// net_dead: called by the gamedriver when an interactive player loses
// hir network connection to the mud.
private void net_dead()
{
	object me = this_object();
	object link_ob;

//	if (!query_condition("killer"))
//		set_heart_beat(0);
	if (objectp(link_ob = query_temp("link_ob"))) {
		link_ob->set("last_on", time());
		link_ob->set("last_from", link_ob->query_temp("last_from"));
		link_ob->save();
		destruct(link_ob);
	}

// adding by Yu Jue 1997-8-17
	if (!environment()) {
		destruct(me);
		return;
	}

	// Stop fighting, we'll be back soon.
	remove_all_enemy();

	set_temp("netdead", uptime());
	if (userp(me)) {
		call_out("user_dump", wiz_level(me)?1:NET_DEAD_TIMEOUT, DUMP_NET_DEAD);
		tell_room(environment(), name() + "断线了。\n", me);
		CHANNEL_D->do_channel(load_object(LOGIN_D), wiz_level(me)?"wiz":"sys", short(1) + NOR " 断线了。");
	}
}

// reconnect: called by the LOGIN_D when a netdead player reconnects.
void reconnect()
{
	set_heart_beat(1);
	delete_temp("netdead");
	remove_call_out("user_dump");
	if(IN_MAIN) tell_object(this_object(), "重新连线完毕。\n");
}

varargs int check_week_flag(string flag_name) {
	if (query("time_flag/week/last_week")!=time()/86400/7){
		delete("time_flag/week");
		set("time_flag/week/last_week",time()/86400/7);
		return 0;
	}
	if(!flag_name) return 0;
	return query("time_flag/week/"+flag_name);
}

varargs int check_day_flag(string flag_name) {
	if (query("time_flag/day/last_day")!=time()/86400)	{
		delete("time_flag/day/");
		set("time_flag/day/last_day",time()/86400);
		return 0;
	}	
	if(!flag_name) return 0;
	return query("time_flag/day/"+flag_name);
}

varargs int check_hour_flag(string flag_name) {
	if (query("time_flag/hour/last_hour")!=time()/3600)	{
		delete("time_flag/hour");
		set("time_flag/hour/last_hour",time()/3600);
		return 0;
	}	
	if(!flag_name) return 0;
	return query("time_flag/hour/"+flag_name);
}

int set_week_flag(string flag_name) {	
	if (query("time_flag/week/last_week")!=time()/86400/7){
		delete("time_flag/week");
		set("time_flag/week/last_week",time()/86400/7);
	}
	return add("time_flag/week/"+flag_name,1);
}

int set_day_flag(string flag_name) {	
	if (query("time_flag/day/last_day")!=time()/86400){
		delete("time_flag/day");
		set("time_flag/day/last_day",time()/86400);
	}
	return add("time_flag/day/"+flag_name,1);
}

int set_hour_flag(string flag_name) {	
	if (query("time_flag/hour/last_hour")!=time()/3600){
		delete("time_flag/hour");
		set("time_flag/hour/last_hour",time()/3600);
	}
	return add("time_flag/hour/"+flag_name,1);
}
