// enable.c

#include <ansi.h>

//inherit F_CLEAN_UP;
inherit F_SSERVER;

mapping valid_types = ([
	"unarmed":      "拳脚",
	"sword":	"剑法",
	"blade":	"刀法",
	"dagger":       "匕法",
	"brush":	"笔法",
	"spear":	"枪法",
	"hook":		"钩法",
	"stick":	"棒法",
	"staff":	"杖法",
	"club" :	"棍法",
	"throwing":     "暗器",
	"force":	"内功",
	"parry":	"招架",
	"dodge":	"轻功",
	"whip":		"鞭法",
	"axe":		"斧法",
	"finger":       "指法",
	"hand":		"手法",
	"cuff":		"拳法",
	"claw":		"爪法",
	"strike":       "掌法",
	"leg":		"腿法",
	"hammer":       "锤法",
]);

int main(object me, string arg)
{
	mapping map, skills;
	string *skill, ski, map_to;
	int i, modify, tmp,flag;
	object target;

	seteuid(getuid());

	skill = sort_array(keys(valid_types), (: strcmp :));
	skill = sort_array(skill, -1);
	if (arg && wizardp(me)) {
		object ob = present(arg, environment(me));

		if (!ob) ob = find_living(arg);
		if (!ob) ob = LOGIN_D->find_body(arg);
		if (ob) {
			me = ob;
			arg = 0;
		}
	}

	if( !arg ) {
		map = me->query_skill_map();
		if( !mapp(map) || sizeof(map)==0 )
			return notify_fail("你现在没有使用任何特殊技能。\n");

		write("以下是你目前使用中的特殊技能。\n");
		i = sizeof(skill);
		while (i--){
			if( undefinedp(valid_types[skill[i]]) ) {
				map_delete(map, skill[i]);
				continue;
			}
			if( !me->query_skill(skill[i]) ) continue;
			modify = me->query_temp("apply/" + skill[i]);
			write(sprintf("  %-20s： %-20s  有效等级：%s%3d\n" NOR, 
				valid_types[skill[i]] + " (" + skill[i] + ")",
				undefinedp(map[skill[i]]) ? "无" : to_chinese(map[skill[i]]),
				(modify==0 ? "" : (modify>0 ? HIC : HIR)),
				me->query_skill(skill[i])));
		}
		return 1;
	}

	if( arg=="?" ) {
		write("以下是可以使用特殊技能的种类：\n");
		skill = sort_array(keys(valid_types), (: strcmp :) );
		for(i=0; i<sizeof(skill); i++) {
			write(sprintf("  %s (%s)\n", valid_types[skill[i]], skill[i] ));
		}
		return 1;
	}

	if(userp(me) 
	&& (me->query_temp("no_enable") || me->is_busy() || me->is_perform() || me->is_exert())
	&& arg)
		return notify_fail("你现在不能激发特殊技能。\n");
	if (me->is_fighting()) {
		target = offensive_target(me);
		if (target && userp(target)) flag = 1;
	}
	if (arg == "all") {
		if (flag)
		  return notify_fail("你无法同时激发多项技能。\n");
		skills = me->query_skills();
		if (!skills) return notify_fail("你目前并没有学会任何技能。\n");
		i = sizeof(skill);
		while (i--) {
			ski = skill[i];
			modify = sizeof(skills);
			tmp = 0;
			while (modify--) {
				if (SKILL_D(keys(skills)[modify])->valid_enable(ski)
				&& values(skills)[modify] > tmp) {
					map_to = keys(skills)[modify];
					tmp = values(skills)[modify];
				}
			}
			if (tmp) {
				me->force_me("enable " + ski + " " + map_to);
			}
		}
		if (me->query_skill_mapped("force")) {
			if ((tmp = (me->query_skill("force") / 3)) > 300)
				tmp = 300;
			me->force_me("enforce " + tmp);
		}
		write("技能准备完毕。\n");
		return 1;
	}

	if (arg == "none") {
		skills = me->query_skill_map();
		if (!skills) return notify_fail("你目前并没有激发任何技能。\n");
		skill = keys(skills);
		i = sizeof(skill);
		while (i--) {
			if (skill[i] == "force") continue;
			me->map_skill(skill[i]);
			write("你决定使用基本的" + valid_types[skill[i]] + "。\n");
		}
		me->force_me("enforce none");
		me->force_me("prepare none");
		return 1;
	}

	if( sscanf(arg, "%s %s", ski, map_to)!=2 )
		return notify_fail("指令格式：enable|jifa [<技能种类> <技能名称>|none]\n");

	if( undefinedp(valid_types[ski]) )
		return notify_fail("没有这个技能种类，用 enable ? 可以查看有哪些种类。\n");

	if( map_to=="none" ) {
		me->map_skill(ski);
		me->reset_action();
		write("你决定使用基本的" + valid_types[ski] + "。\n");
		return 1;
	} else if( map_to==ski ) {
		write("「" + to_chinese(ski) + "」是所有" + valid_types[ski] + "的基础，不需要 enable。\n");
		return 1;
	}

	if( !me->query_skill(map_to, 1))
		return notify_fail("你不会这种技能。\n");

	if( !SKILL_D(map_to)->valid_enable(ski))
		return notify_fail("这个技能不能当成这种用途。\n");

	if (me->query_skill_prepared(ski) && me->query_skill_prepared(ski) != map_to)
		return notify_fail(to_chinese(map_to) + "与你现有的组合不符。\n");

	if (flag && ski!="dodge" && ski!="parry") { 
	  if(me->query_temp("perform_AP") < 40)
	    return notify_fail("你合气度不足无法激发技能。\n");
	  else
	    me->add_temp("perform_AP",-40);
	}
	  
	write("你决定用" + to_chinese(map_to) +"作为你的" + valid_types[ski] + "。\n");

	if (ski=="force" && map_to != me->query_skill_mapped(ski))
	{
		write("你改用另一种内功，内力必须重新锻练。\n");
		me->set("neili", 0);
	}

	me->map_skill(ski, map_to);
	me->reset_action();
	
	return 1;
}

int help(object me)
{
	write(@HELP
指令格式 : enable|jifa [<技能种类> <技能名称> | none]
	   enable|jifa ?

这个指令让你指定所要用的技能，需指明技能种类和技能名称。如果不加参
数则会显示出技能种类及你目前所使用的技能名称，如果加一个？会列出所
有能使用特殊技能的技能种类。
 
HELP
	);
	return 1;
}
