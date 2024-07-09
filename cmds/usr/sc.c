#include <ansi.h>
#include <combat.h>
#include <mudlib.h>

inherit F_CLEAN_UP;

#include <viplvl.h>

string display_attr(int gift, int value);

void create() { seteuid(getuid()); }

string *age_color = ({
	CYN, YEL, HIY, HIG, HIC, HIR
});


int DB_last_query;

int main(object me, string arg)
{
	object ob;
	mapping my;
	string line ,temp ,skill_type, title,temp1;
  int user_weapon,damage_bonus;
	object weapon;
	mixed ret; 
	int attack_points, dodge_points, parry_points,i;

	if (!arg) ob = me;
	else if( !(ob=present(arg, environment(me))) ) {
		if (wizardp(me)) {
			if (!ob) ob = LOGIN_D->find_body(arg);
			if (!ob) ob = find_living(arg);
			if (!ob || !me->visible(ob)) return notify_fail("你要察看谁的状态？\n");
		}
		else return notify_fail("你要察看谁的状态？\n");
	}

	if(!wizardp(me) && ob!=me && !me->query("player_debug") ) {
		if(userp(ob) || !me->query_condition("true_view")) {
		return notify_fail("你无法查看"+ob->name()+"的状态。\n");
    }
  }


  	
  //add for get bbs credit by felchion 2008.2.12
	if (userp(ob) && time()-ob->query("last_dbquery")>10 && time()-DB_last_query>3){
		ob->set("last_dbquery",time());
		DB_last_query = time();

		dbquery("UPDATE members set extcredits1 = "+sprintf("%d",ob->query("max_pot")-100)+" WHERE username='"
		+ ob->query("id")+"@"+INTERMUD_MUD_NAME+"'"); 
			
		ret = dbquery("SELECT extcredits2,extcredits1,extcredits3,extcredits4 FROM members WHERE username='"
		+ ob->query("id")+"@"+INTERMUD_MUD_NAME+"'");
		
		if(ob->query("family/family_name"))	
		dbquery("update Users set party ='"+ob->query("family/family_name")+"' WHERE U_Username='"
		+ ob->query("id") +"'");
		
		if (sizeof(ret)==1) {
			ob->set("BBS_Credit",ret[0][0]);
			ob->set("BBS_honor",ret[0][1]);
			if(ret[0][3]>300 || (ret[0][3]>0 && !ob->check_day_flag("BBS_exchange")) ) {
				if(dbquery("UPDATE members set extcredits4=0 WHERE username='"+ob->query("id")+"@"+INTERMUD_MUD_NAME+"'")) {
					ob->add("SJ_Credit",ret[0][3]);
					log_db(ob,0,ret[0][3],ob->query("SJ_Credit"),"社区通宝");
					ob->set_day_flag("BBS_exchange");
				}
			}
			if (ret[0][1]*ret[0][0] < 0 || (ret[0][1]<0 && ret[0][0]==0))
			dbquery("UPDATE members set extcredits1 = -1*extcredits1 WHERE username='"
			+ ob->query("id")+"@"+INTERMUD_MUD_NAME+"'"); 
		}
		if (!ob->query("BBS_UID")) {
			ret = dbquery("SELECT cast(uid as CHAR) FROM discuz6.uc_members WHERE username='"
			+ ob->query("id")+"@"+INTERMUD_MUD_NAME+"'");
		  if (sizeof(ret)==1) {
		    sscanf(ret[0][0],"%d",i);
		    ob->set("BBS_UID",i);
		  }		
		}
	}
	
	my = ob->query_entire_dbase();
// add for temp title display
	my["title"] ? title = my["title"]: title = "无";
	
// 显示武器未打造的机会次数
        if (my["combat_exp"] < 500000) user_weapon = 0;
        else if (my["combat_exp"]  < 1000000 && !my["user_weapon_imbued"]) user_weapon = 1;
        else if ((my["combat_exp"]/1000000 - (int)my["user_weapon_imbued"])<0) user_weapon = 0;
        else if ((my["combat_exp"]/1000000 - (int)my["user_weapon_imbued"])==0) user_weapon = 1;
        else user_weapon = (my["combat_exp"]/1000000 - (int)my["user_weapon_imbued"])+1;
        user_weapon += my["imbue_reward"];
        //imbue_reward
                
	if (stringp(ob->query_temp("title")))
		title = ob->query_temp("title");

	line = "\n┏━━"HIW"【书剑个人资料卡】"NOR
		+(ob->query_temp("big5")?"繁":"简")+"体中文━┳━━━━━━━━━━━━━━━━━━━━┓\n";

	line += sprintf("┃称    谓：%-22s┃ 体  重：「%-29s┃\n",
		RANK_D->query_rank(ob),
		chinese_number(ob->query_weight()/500)+"斤」"
	);

	line += sprintf("┃头    衔：%-22s┃ %-20s%-19s┃\n", title,
		"臂  力：「"+display_attr(my["str"], ob->query_str(2))+"」",
		"根  骨：「"+display_attr(my["con"], ob->query_con(2))+"」"
	);

	line += sprintf("┃姓    名：%-22s┃ %-20s%-19s┃\n",
		(my["color"]?my["color"]:"")+my["name"]+NOR"("+capitalize(my["id"])+")",
		"身  法：「"+display_attr(my["dex"], ob->query_dex(2))+"」",
		"悟  性：「"+display_attr(my["int"], ob->query_int(2))+"」"
	);

	line += sprintf("┃外    号：%-22s┗━━━━━━━━━━━━━━━━━━━━┫\n",
		my["nickname"]?my["nickname"]+NOR:"无"
	);

	line += sprintf("┃年    龄：%-24s%-40s┃\n",
		age_color[my["day"]/5]+chinese_number(my["age"]-my["age_reyoung"])+"岁"+(my["month"]?"又"+chinese_number(my["month"])+"个月":"")+NOR,
		""
	);

	if (objectp(weapon = ob->query_temp("weapon")) ) skill_type = weapon->query("skill_type");
	else {
		mapping prepare = ob->query_skill_prepare();

		if (!sizeof(prepare)) skill_type = "unarmed";
		else skill_type = keys(prepare)[0];
	}
	attack_points = COMBAT_D->skill_power(ob, skill_type, SKILL_USAGE_ATTACK);
	parry_points = COMBAT_D->skill_power(ob, "parry", SKILL_USAGE_DEFENSE);
	dodge_points = COMBAT_D->skill_power(ob, "dodge", SKILL_USAGE_DEFENSE);
	line += sprintf("┃性    别：%-24s攻：%-9s躲：%-9s架：%8s  ┃\n",
		my["gender"]+(ob->is_robot()?HIG"→机器人"NOR:""),
		"/cmds/std/look"->tough_level(attack_points, 0, 1),
		"/cmds/std/look"->tough_level(dodge_points, 0, 1),
		"/cmds/std/look"->tough_level(parry_points, 0, 1)
	);

	if (my["divorce"]) temp1 = "婚    姻：离异"+chinese_number(my["divorce"])+"次";
	else temp1 = "婚    姻：未婚";
	if (mapp(my["marry"])) {
		if (ob->query("marry/name"))
			temp1 = (my["gender"]=="女性"?"夫    君："HIR:"娇    妻："HIG)
				+my["marry"]["name"]+"("+capitalize(my["marry"]["id"])+")" NOR;
	};
	
	temp = ob->query("family/family_name")?"【"+my["family"]["family_name"]+"】":"【普通百姓】";
	temp += ob->query("family/master_name")?"【"+my["family"]["master_name"]+"】":"";
	if(userp(ob))line += sprintf("┃%-34s师    承：%-30s┃\n",
		temp1,
		HIW+temp+NOR
	);

//  if (my["registered"]> sizeof(vip_type)-1 )
//  vip = HIY+vip_type[sizeof(vip_type)-1]+NOR;
//  else 
//	vip = HIY+vip_type[my["registered"]]+NOR;
	if(userp(ob)) line += sprintf("┃注    册：%-24s钱庄存款：%-30s┃\n",
  Vip_Class(ob->query("registered")),
  my["balance"]/10000?HIY BRED+(chinese_number(my["balance"]/10000>200000?my["balance"]/100000*10:my["balance"]/10000))+"锭黄金"NOR:"很少",
//		my["SJ_Credit"]?HIG BGRN+chinese_number(my["SJ_Credit"])+"个"NOR:"无",
//	  my["BBS_Credit"]?HIY BGRN+chinese_number(my["BBS_Credit"])+"个"NOR:"无",
	);
	if(userp(ob)) line += sprintf("┃上次遇害：%-64s┃\n",
			(ob->query("last_death")?ob->query("last_death"):"还未遇害")
			+(ob->query("last_killer")?"「"+ob->query("last_killer")+"」":"")
		);
	if(userp(ob)) line += "┠─────────────────────────────────────┨\n"; 


	if(userp(ob)) line += sprintf("┃江湖声望：%-18d%-6s%-18d"NOR"会员经验：%-12d┃\n",
		my["honor_point"],
		my["shen"] < 0?"戾气："HIR:"正气："HIC,
		(my["shen"] < 0?-1:1)*my["shen"],
		my["BBS_Credit_Used"]/10+my["SJ_TC_Used"]*2
	);
	

	
	
	if (base_name(ob) == "/clone/user/user") {
                line += sprintf("┃书剑通宝：%-18d书剑元宝：%-14d永恒币：%-14d┃\n",
                        my["SJ_Credit"],
                        my["BBS_Credit"],
                        my["SJ_TC"]
                );

		                line += sprintf("┃当前等级：%-18d解迷机会：%-14d打造机会：%-12d┃\n",
                        my["max_pot"]-100,
                        ob->query("quest/chance"),                       
                        user_weapon                       
                );
      line += sprintf("┃死亡次数：%-18d杀死玩家：%-14d正当防卫：%-12d┃\n",
			my["death_count"], 
			my["PKS"]+my["FKS"],
			my["FKS"]
		);

	}
  line += "┠─────────────────────────────────────┨\n"; 
		damage_bonus = ob->query_temp("apply/wp_damage");					

		if( my["jiali"] > 0 ) {
				if ( ob->query("is_head") 
				|| (ob->query_temp("weapon") 	&& (ob->query_temp("weapon"))->query("skill_type")!="throwing" ) ) 					
					damage_bonus += my["jiali"]/6;				
				else
					damage_bonus += my["jiali"]/2;
		}	
		line += sprintf("┃基础伤害：%-6d  伤害：%-8s命中：%-8s躲避：%-8s招架：%-8s┃\n",
			damage_bonus,(sprintf("+%d%%",ob->query_temp("apply/pfm_damage")/2)),	
			sprintf("+%d%%",ob->query_temp("apply/pfm_attack")/6),
			sprintf("+%d%%",ob->query_temp("apply/pfm_dodge")/6),
			sprintf("+%d%%",ob->query_temp("apply/pfm_parry")/6),
		);
		line += sprintf("┃    暴击：%-8s必杀：%-8s合气：%-8d防御：%-8s%-14s┃\n",
			(ob->query_temp("apply/rush")>0?"+":"")+sprintf("%d%%",ob->query_temp("apply/rush")),
			(ob->query_temp("apply/kill")>0?"+":"")+sprintf("%d%%",ob->query_temp("apply/kill")),			
			(ob->query("int")*3+ob->query_temp("apply/ap")),
			(ob->query_temp("apply/eq_armor")/2>0?"+":"")+sprintf("%d",ob->query_temp("apply/eq_armor")/2),
			
			""
		);
		
//		line += sprintf("┃控制抗性：%-6s  合气：%-8s防御：%-8s控制：%-8s致命：%-8s┃\n",
//			sprintf("%d",ob->query_temp("apply/anti_ctrl")),
//			sprintf("%d",ob->query_temp("apply/ap")+ob->query_int(2)*2),
//			(ob->query_temp("apply/eq_armor")/2>0?"+":"")+sprintf("%d",ob->query_temp("apply/eq_armor")/2),
//			(ob->query_temp("apply/ctrl")>0?"+":"")+sprintf("%d",ob->query_temp("apply/ctrl")),
//			(ob->query_temp("apply/fatal")>0?"+":"")+sprintf("%d",ob->query_temp("apply/fatal")
//			)
//		);
//	  line += sprintf("┃致命抗性：%-6s  幸运：%-8d                            理相：%2s%-4s  ┃\n",
//			sprintf("%d",ob->query_temp("apply/anti_fatal")),
//			(ob->query_temp("apply/luck")),
//			userp(ob)?ob->query("luck/phase"):"--",
//			userp(ob)?(ob->query("luck/offset")>0?sprintf("("HIC"%d"NOR")",ob->query("luck/offset")):sprintf("("+HIB+"%d"+NOR+")",-ob->query("luck/offset"))):""
//		);
	
	line += "┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛\n";

   if (wiz_level(me)>3) {
		line += sprintf(" 容貌[%s]",
			display_attr(my["per"], ob->query_per())),
		line += sprintf(" 攻击%d(+%d) 躲闪%d 防御%d(+%d)\n",
			attack_points/100, ob->query_temp("apply/damage"),
			dodge_points/100,
			parry_points/100, ob->query_temp("apply/armor"));
		}
		if( base_name(ob) == "/clone/user/user" ){
			if (my["mud_time"] >= 600) {
				line += sprintf(" 经验：%d 上周：%d 本周：%d 平均成长率：%d \n",
					my["combat_exp"], 
					ob->query("week_rate/last_week_exp"),
					ob->query("week_rate/this_week_exp"),
					to_int(my["combat_exp"]/(my["mud_time"]/60.0)*60)
				// to_int(my["combat_exp"]/(my["mud_time"]/60.0)*60/(1+(my["int"]-20)*0.03))
				);
			}
		}
  if(MUD_SITE_ID == "tj" || MUD_SITE_ID == "test")
	line += sprintf(" 炼金时间：%-22s \n",CHINESE_D->chinese_time(ob->query("time_mining")/600*600));
	if (my["mud_time"] >= 86400 && my["mud_time"]/100>0) {
		line += sprintf(" 在线时间：%-26s 有效在线时间：%s\n 上周有效在线: %-21s　本周有效在线：%s\n 在线有效比：百分之%-18s",
			CHINESE_D->chinese_time(my["mud_time"]/3600*3600),
			CHINESE_D->chinese_time(my["eff_mud_time"]/3600*3600),
			CHINESE_D->chinese_time(ob->query("week_rate/last_week_online")/600*600),
			CHINESE_D->chinese_time(ob->query("week_rate/this_week_online")/600*600),
			CHINESE_D->chinese_number((my["eff_mud_time"]/60000*100)/(my["mud_time"]/60000)));
		line += sprintf(" 平均每天在线：%s\n",
			CHINESE_D->chinese_time(to_int(my["mud_time"]*400.0/(time()-my["birthday"]+1)*216)>86400?86400:to_int(my["mud_time"]*400.0/(time()-my["birthday"]+1)*216) )
		);
	  
	}
	write(line);
	return 1;
}

string display_attr(int gift, int value)
{
	string str;
	if (value > gift) str = HIY;
	else if (value < gift) str = CYN;
	return str + value + NOR + "/" + gift;
}

int help(object me)
{
write(@HELP
指令格式 : score
	   score <对象名称>            (巫师专用)

这个指令可以显示你或指定对象(含NPC)的基本资料。

see also : hp
HELP
);
	return 1;
}
