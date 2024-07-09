// skills.c
// Modified by snowman@SJ 14/03/2000

#include <ansi.h>
#include <level.h>

inherit F_CLEAN_UP;

string *skill_level_desc = ({ LEVEL });

string *knowledge_level_desc = ({
	BLU "新学乍用" NOR,HIB "初窥门径" NOR,MAG "略知一二" NOR,HIM "半生不熟" NOR,
	CYN "马马虎虎" NOR,YEL "已有小成" NOR,YEL "融会贯通" NOR,HIY "心领神会" NOR,
	HIY "了然於胸" NOR,HIG "已有大成" NOR,HIG "非同凡响" NOR,HIC "举世无双" NOR,
	HIC "震古铄今" NOR,HIW "无与伦比" NOR,HIW "超凡入圣" NOR,HIR "空前绝后" NOR
});

string *valid_type = ({
"force", "parry",  "dodge", "unarmed","claw",
"cuff",  "finger", "strike","hand",   "leg",
"sword", "blade",  "dagger","brush",  "spear",
"hook",  "stick",  "staff", "club",   "throwing",
"whip",  "axe",    "hammer","bow",
});

string skill_level(string, int);

int sort_skill2(string sk) // add by augx@sj 8/27/2001
{
	return (SKILL_D(sk)->type() == "knowledge" && SKILL_D(sk)->type2() == "worker");
}

int sort_skill(string sk)
{
	return (SKILL_D(sk)->type() != "martial" && SKILL_D(sk)->type() != "array");
}

int sort_basic(string sk)
{
	return member_array(sk, valid_type)!=-1;
}

int main(object me, string arg)
{
	object ob;
	mapping skl, lrn, map;
	string *sname, *mapped, *allskills, str = "";
	int i, j;

	seteuid(getuid());

	if(!arg)
		ob = me;

	else if(arg == "?"){
		i = sizeof(skill_level_desc);
		while(i--) {
			str = "    " + skill_level_desc[i] + str;
			if (i % 5 == 0) str = "\n" + str;
		}
		str = "\n\n技能的描述由低到高依次为："+str;
		i = sizeof(knowledge_level_desc);
		while(i--) {
			str = "    " + knowledge_level_desc[i] + str;
			if (i % 4 == 0) str = "\n" + str;
		}
		return notify_fail("知识的描述由低到高依次为："+str+"\n");
	}

	else{
		ob = present(arg, environment(me));
		if (!ob) ob = find_player(arg);
		if (!ob) ob = find_living(arg);
		if (!ob) ob = LOGIN_D->find_body(arg);
		if (!ob || !me->visible(ob)) return notify_fail("你要察看谁的技能？\n");
	}

	if( ob!=me && !wizardp(me) ){
		if(environment(ob)!=environment(me))
			return notify_fail("你要察看谁的技能？\n");
		if( !ob->is_apprentice_of(me)
		 && !me->is_apprentice_of(ob)
		 && ob->query("winner") != me->query("id")
		 && me->query("marry/id") != ob->query("id")) {
		 	if( userp(ob)
		  	 || !me->query("group/id")
		  	 || me->query("group/id") != ob->query("group/id")
		  	 || me->query("group/class") >= ob->query("group/class") )
				return notify_fail("只有有师徒关系或同在一个帮派的人才能察看他人的技能。\n");
		}
	}

	skl = ob->query_skills();

	if(!j = sizeof(skl)) {
		write( (ob==me ? "你" : ob->name(1)) + "目前并没有学会任何技能。\n");
		return 1;
	}

	allskills  = sort_array( keys(skl), (: strcmp :) );

	str = HIW"【";
	str += (ob==me?"你":ob->name(1))+"的技能表】"NOR"：总共"+chinese_number(j)+"项技能";
	if(j > 31) str+= "，已经严重影响了以后的进步！";
	str += "\n┌──────────────────────────────────────────────┐\n";
	map = ob->query_skill_map();
	if( mapp(map) ) mapped = values(map);
	if( !mapped ) mapped = ({});

	lrn = ob->query_learned();
	if( !mapp(lrn) ) lrn = ([]);

	// add by augx@sj 8/27/2001
	sname  = filter_array( allskills, (: sort_skill2 :) );
	allskills -= sname;

	j = sizeof(sname);
	if( j > 0){
		str += sprintf("%-93s", "│"HIY"『职业技能 "+chinese_number(j)+" 种』"NOR"：");
		for(i=0; i < j; i++) {
			if (i % 2 == 0) str += "│\n│";
			else str += "  ";
			str += sprintf("%s%-8s%-14s"HIC"→"NOR"%s %4d/%7d",
				(lrn[sname[i]] >= (skl[sname[i]]+1) * (skl[sname[i]]+1)) ? HIM : "",
				to_chinese(sname[i]), "(" + sname[i] + ")",
				skill_level(SKILL_D(sname[i])->type(), skl[sname[i]]),
				skl[sname[i]], (int)lrn[sname[i]],);
			if ( i > j-2 && i % 2 == 0)
				str +="                                               ";
		}
		str += "│\n├──────────────────────────────────────────────┤\n";
	}
	//

	sname  = filter_array( allskills, (: sort_skill :) );
	allskills -= sname;

	j = sizeof(sname);
	if( j > 0){
		str += sprintf("%-93s", "│"HIY"『杂项技能 "+chinese_number(j)+" 种』"NOR"： ");
		for(i=0; i < j; i++) {
			if (i % 2 == 0) str += "│\n│";
			else str += "  ";
			str += sprintf("%s%-8s%-14s"HIC"→"NOR"%s %4d/%7d",
				(lrn[sname[i]] >= (skl[sname[i]]+1) * (skl[sname[i]]+1)) ? HIM : "",
				to_chinese(sname[i]), "(" + sname[i] + ")",
				skill_level(SKILL_D(sname[i])->type(), skl[sname[i]]),
				skl[sname[i]], (int)lrn[sname[i]],);
			if ( i > j-2 && i % 2 == 0)
				str +="                                               ";
		}
		str += "│\n├──────────────────────────────────────────────┤\n";
	}

	sname  = filter_array( allskills, (: sort_basic :) );
	allskills -= sname;

	j = sizeof(sname);
	if(j > 0){
		str += sprintf("%-93s", "│"HIY"『基本技能 "+chinese_number(j)+" 种』"NOR"：    ");
		for(i=0; i < j; i++) {
			if (i % 2 == 0) str += "│\n│";
			else str += "  ";
			str += sprintf("%s%-8s%-14s"HIC"→"NOR"%s %4d/%7d",
				(lrn[sname[i]] >= (skl[sname[i]]+1) * (skl[sname[i]]+1)) ? HIM : "",
				to_chinese(sname[i]), "(" + sname[i] + ")",
				skill_level(SKILL_D(sname[i])->type(), skl[sname[i]]),
				skl[sname[i]], (int)lrn[sname[i]]);
			if ( i > j-2 &&i % 2 == 0)
				str +="                                               ";
		}
		str += "│\n├──────────────────────────────────────────────┤\n";
	}

	sname  = allskills;

	j = sizeof(sname);
	if(j > 0){
		str += sprintf("%-93s│\n", "│"HIY"『特殊技能 "+chinese_number(j)+" 种』"NOR"：  ");
		for(i=0; i < j; i++) {
			str += sprintf("│  %s%s%-13s%-29s" HIC "→ "NOR"%s %4d/%8d %21s\n",
			(lrn[sname[i]] >= (skl[sname[i]]+1) * (skl[sname[i]]+1)) ? HIM : "",
			(member_array(sname[i], mapped)==-1? "  ": "□"),
			to_chinese(sname[i]), " (" + sname[i] + ")",
			skill_level(SKILL_D(sname[i])->type(), skl[sname[i]]),
			skl[sname[i]], (int)lrn[sname[i]],"│");
		}
		str += "└──────────────────────────────────────────────┘\n";
	}


	write(str);
	return 1;
}

string skill_level(string type, int level)
{
	int grade;

	switch(type) {
		case "knowledge":
			grade = level / 20;
			if( grade >= sizeof(knowledge_level_desc) )
				grade = sizeof(knowledge_level_desc)-1;
			return knowledge_level_desc[grade];
		default:
			//grade = level / 7; this is old SJ system
                		// but now system the player combat speed is very fast.
			grade = level / 11;
			if( grade >= sizeof(skill_level_desc) )
				grade = sizeof(skill_level_desc)-1;
			return skill_level_desc[grade];
	}
}

int help(object me)
{
	write("\n指令格式 : skills [<某人> | ?]\n");
	write(@HELP
这个指令可以让你查询所学过的技能。
你也可以查知和你有师徒关系和自己的拷贝的技能状况。
如果 skills ? 则会显示目前的技能的描述。

巫师可以查询任何人或 NPC 的技能状况。

	Copyright 1997-1999 ShuJian MUD Wizard Group.
	All rights reserved.

	Visit our Homepage at - www.mymud.com

HELP);

    return 1;
}
