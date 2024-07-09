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
	string line ,temp ,skill_type, title,vip;
  int user_weapon;
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

	if( ob!=me && !wizardp(me) ) {
		if( userp(ob)
		  || !me->query("group/id")
		  || me->query("group/id") != ob->query("group/id")
		  || me->query("group/class") >= ob->query("group/class") )
			return notify_fail("你只能查看同帮派，且等级比你低的NPC的状态。\n");
	}

  	
  //add for get bbs credit by felchion 2008.2.12
	if (userp(ob) && time()-me->query("last_dbquery")>10 && time()-DB_last_query>3 ){
		me->set("last_dbquery",time());
		DB_last_query = time();		

		if(ob->query("family/family_name"))	
		dbquery("update Users set party ='"+ob->query("family/family_name")+"' WHERE U_Username='"
		+ ob->query("id") +"'",1);
		
		ret = dbquery("SELECT extcredits2,extcredits1,extcredits3 FROM members WHERE username='"
		+ ob->query("id")+"@"+INTERMUD_MUD_NAME+"'");
		
		if (sizeof(ret)==1) {
			ob->set("BBS_Credit",ret[0][0]);
			if (ret[0][1]*ret[0][0] < 0 || (ret[0][1]<0 && ret[0][0]==0))
				dbquery("UPDATE members set extcredits1 = -1*extcredits1 WHERE username='"
				+ ob->query("id")+"@"+INTERMUD_MUD_NAME+"'",1); 
		  dbquery("UPDATE members set extcredits1 = "+(ob->query("max_pot")-100)+" WHERE username='"
		      + ob->query("id")+"@"+INTERMUD_MUD_NAME+"'",1);
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
		+(ob->query_temp("big5")?"繁":"简")+"体中文━━┳━━━━━━━━━━━━━━━━━━━┓\n";

	line += sprintf("┃称    谓：%-24s┃体  重：「%-28s┃\n",
		RANK_D->query_rank(ob),
		chinese_number(ob->query_weight()/500)+"斤」"
	);

	line += sprintf("┃头    衔：%-24s┃%-19s%-19s┃\n", title,
		"膂  力：「"+display_attr(my["str"], ob->query_str())+"」",
		"根  骨：「"+display_attr(my["con"], ob->query_con())+"」"
	);

	line += sprintf("┃姓    名：%-24s┃%-19s%-19s┃\n",
		(my["color"]?my["color"]:"")+my["name"]+NOR"("+capitalize(my["id"])+")",
		"身  法：「"+display_attr(my["dex"], ob->query_dex())+"」",
		"悟  性：「"+display_attr(my["int"], ob->query_int())+"」"
	);

	line += sprintf("┃外    号：%-24s┗━━━━━━━━━━━━━━━━━━━┫\n",
		my["nickname"]?my["nickname"]+NOR:"无"
	);

	line += sprintf("┃年    龄：%-26s%-38s┃\n",
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
	line += sprintf("┃性    别：%-26s攻：%-9s躲：%-9s架：%8s┃\n",
		my["gender"]+(ob->is_robot()?HIG"→机器人"NOR:""),
		"/cmds/std/look"->tough_level(attack_points, 0, 1),
		"/cmds/std/look"->tough_level(dodge_points, 0, 1),
		"/cmds/std/look"->tough_level(parry_points, 0, 1)
	);

	temp = ob->query("family/family_name")?"【"+my["family"]["family_name"]+"】":"【普通百姓】";
	temp += ob->query("family/master_name")?"【"+my["family"]["master_name"]+"】":"";
	line += sprintf("┃钱庄存款：%-26s师    承：%-28s┃\n",
		my["balance"]/10000?HIY BRED+chinese_number(my["balance"]/10000)+"锭黄金"NOR:"很少",
		HIW+temp+NOR
	);
	line += sprintf("┃书剑通宝：%-26s社区元宝：%-28s┃\n",
		my["SJ_Credit"]?HIG BGRN+chinese_number(my["SJ_Credit"])+"个"NOR:"无",
	  my["BBS_Credit"]?HIY BGRN+chinese_number(my["BBS_Credit"])+"个"NOR:"无",
	);
	line += "┠─────────────────────────────────────┨\n"; 

	if (my["divorce"]) temp = "婚姻：离异"+chinese_number(my["divorce"])+"次";
	else temp = "婚姻：未婚";
	if (mapp(my["marry"])) {
		if (ob->query("marry/name"))
			temp = (my["gender"]=="女性"?"夫君："HIR:"娇妻："HIG)
				+my["marry"]["name"]+"("+capitalize(my["marry"]["id"])+")" NOR;
	};
	line += sprintf("┃%-28s%-6s%-18d"NOR"综合评价：%-12d┃\n",
		temp,
		my["shen"] < 0?"戾气："HIR:"正气："HIC,
		(my["shen"] < 0?-1:1)*my["shen"],
		my["score"]
	);
	
	vip = Vip_Class(my["registered"]);
	

	if (base_name(ob) == "/clone/user/user") {
                line += sprintf("┃注册：%-8s%-14s叛师：%-18s解迷机会：%-12s┃\n",
                        vip,
                        my["no_pk"]?"→已金盆洗手"NOR:""NOR,
                        my["betrayer"]?HIY+chinese_number(my["betrayer"])+"次"NOR:"无",
                        ob->query("quest/chance")?HIR+chinese_number(ob->query("quest/chance"))+"次"NOR:"无",
                );

		line += sprintf("┃江湖声望：%-18d杀死玩家：%-14d正当防卫：%-12d┃\n",
			my["honor_point"],
			my["PKS"]+my["FKS"],
			my["FKS"]
		);

		//因为空着，所以加上打造兵器.:)
                line += sprintf("┃当前等级：%-18s死亡：%-18s打造机会：%-12s┃\n",
                        HIM+chinese_number(my["max_pot"]-100)+"级"+NOR,
                        my["death_count"]?HIM+chinese_number(my["death_count"])+"次"NOR:"无",                       
                        user_weapon?HIM+chinese_number(user_weapon)+"次"NOR:"无",
                        //HIM + user_weapon + NOR
                );
		line += sprintf("┃上次遇害：%-64s┃\n",
			(ob->query("last_death")?ob->query("last_death"):"还未遇害")
			+(ob->query("last_killer")?"「"+ob->query("last_killer")+"」":"")
		);
	}

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

	if (my["mud_time"] >= 86400 && my["mud_time"]/100>0) {
		line += sprintf(" 在线时间：%-26s 有效在线时间：%s\n 上周有效在线: %-21s　本周有效在线：%s\n 在线有效比：百分之%-18s",
			CHINESE_D->chinese_time(my["mud_time"]/600*600),
			CHINESE_D->chinese_time(my["eff_mud_time"]/600*600),
			CHINESE_D->chinese_time(me->query("week_rate/last_week_online")/600*600),
			CHINESE_D->chinese_time(me->query("week_rate/this_week_online")/600*600),
			CHINESE_D->chinese_number(my["eff_mud_time"]/(my["mud_time"]/100)));
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
