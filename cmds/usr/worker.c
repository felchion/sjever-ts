#include <ansi.h>
inherit F_CLEAN_UP;

static mapping get_chinese = ([
	"axe":"斧",
	"brush":"笔",
	"fork":"叉",
	"hook":"钩",
	"spear":"枪",	
	"whip":"鞭",
	"stick":"棒",
	"sword":"剑",
	"xiao":"萧",
	"blade":"刀",
	"dagger":"匕首",
	"club":"棍",
	"staff":"杖",
	"hammer":"法轮",
	//"throwing":"暗器",
	"cap":"头盔",
	"glove":"手套",
	"boot":"靴",
	"belt":"腰带",
	"mantle":"披风",
	"coat":"彩衣",
	"armor":"甲胄",
	//"cloth":"衣服",
	//"finger":"戒指",
	//"neck":"项链",	
	//"wrists":"护胸",	
	//"hands":"指套",
	//"mineral":"矿石",
	//"jade":"玉",
	//"silk":"丝",
	//"cloths":"布",
]);

int sort_skill2(string sk) 
{
	return (SKILL_D(sk)->type() == "knowledge" && SKILL_D(sk)->type2() == "worker");
}
int main(object me,string arg)
{
        object ob;
        mapping job;
	mapping skl, lrn, map;
        
	string *sname, *mapped, *allskills, str = "", str2 = "";
	int i, j, k;
	seteuid(getuid());

	if(!arg)
		ob = me;
	else{
		ob = present(arg, environment(me));
		if (!ob) ob = find_player(arg);
		if (!ob) ob = find_living(arg);
		if (!ob) ob = LOGIN_D->find_body(arg);
		if (!ob || !me->visible(ob)) return notify_fail("你要察看谁的技能？\n");
	}	
	skl = ob->query_skills();
	map = ob->query_skill_map();
	if( mapp(map) ) mapped = values(map);
	if( !mapped ) mapped = ({});
	
	lrn = ob->query_learned();
	if( !mapp(lrn) ) lrn = ([]);
	
	allskills  = sort_array( keys(skl), (: strcmp :) );	
	
	sname  = filter_array( allskills, (: sort_skill2 :) );
	allskills -= sname;
	j = sizeof(sname);

	if( j > 0){
		str = HIW"【";
		str += (ob==me?"你":ob->name(1))+"的打造技能表】"NOR"：\n";	
		str += "┌────────────────────────────────────────────┐\n";
		str += sprintf("%-90s", "│总共"+chinese_number(j)+"项技能");
		for(i=0; i < j; i++) {
			if (i % 2 == 0) str += "│\n│";
			else str += "  ";
			str += sprintf("%s%-8s%-14s"HIC"→"NOR"%s %3d/%6d",
				(lrn[sname[i]] >= (skl[sname[i]]+1) * (skl[sname[i]]+1)) ? HIM : "",
				to_chinese(sname[i]), "(" + sname[i] + ")",
				"/cmds/skill/skills.c"->skill_level(SKILL_D(sname[i])->type(), skl[sname[i]]),
				skl[sname[i]], (int)lrn[sname[i]],);
			if ( i > j-2 && i % 2 == 0)
				str +="                                             ";
		}
		str += "│\n└────────────────────────────────────────────┘\n";
	}
	else
		str += HIW+" "+(ob==me ? "你" : ob->name(1)) + "目前并没有学会任何打造技能。\n"NOR;
	//
	write (str);		       
        job = ob->query("forging");
        if(!mapp(job)) job=([]);
        
        if(!arrayp(keys(job))) return 1;	
	k = sizeof(keys(job));	
	if (k<=0) str2 = HIW+" "+(ob==me ? "你" : ob->name(1))+"目前尚不会打造任何东西。\n"+NOR;
	else {

		
		while(k--){
			str2 = sprintf(" %-14s",get_chinese[keys(job)[k]]+"("+capitalize(keys(job)[k])+")")+str2;
			if (k % 4 == 0) str2 = "\n┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉\n" + str2;
		}
		str2 = HIW+"【"+(ob==me ? "你" : ob->name(1))+"已经学会的打造项目有】"+NOR
		 + str2
		 +"\n┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉\n"; 
	}
	
	write (str2);	
        return 1;
}

int help(object me)
{
        string *p;
        string str = "\n";
        int i = sizeof(get_chinese);
	p= sort_array(keys(get_chinese), 1);
	while(i--){
			str = sprintf("%-14s",get_chinese[p[i]]+"("+capitalize(p[i])+")")+str;
			if (i % 4 == 0) str = "\n┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉\n" + str;
	}
write(@HELP
指令格式 : worker
这个指令可以让你显示目前的打造技能等级和已经学会的打造项目。
目前开放的打造项目有：
HELP);
		write(str);
		write("┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉\n");

    return 1;
}