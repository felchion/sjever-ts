// skills.c

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

string skill_level(string, int);

int main(object me, string arg)
{
        object ob;
        mapping skl, lrn, map;
        string *sname, *mapped;
        int i, j;

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

        if( ob!=me && !wizardp(me) ){
        	if(environment(ob)!=environment(me))
        		return notify_fail("你要察看谁的技能？\n");
        	if(!ob->is_apprentice_of(me)
        	&& !me->is_apprentice_of(ob) 
        	&& ob->query("winner") != me->query("id")
        	&& me->query("marry/id") != ob->query("id"))
                	return notify_fail("只有巫师或有师徒关系的人能察看他人的技能。\n");
	}
	
        skl = ob->query_skills();
        if(!j = sizeof(skl)) {
                write( (ob==me ? "你" : ob->name(1)) + "目前并没有学会任何技能。\n");
                return 1;
        }
        write( (ob==me ? "你" : ob->name(1)) +"目前学过"+chinese_number(j)+"种技能"+(j > 31?"，但所学技能太杂，将严重影响了以后的进步：":"：\n")+"\n");
        sname  = sort_array( keys(skl), (: strcmp :) );
        
        map = ob->query_skill_map();
        if( mapp(map) ) mapped = values(map);
        if( !mapped ) mapped = ({});

        lrn = ob->query_learned();
        if( !mapp(lrn) ) lrn = ([]);
        
        for(i=0; i < j; i++) 
        	write(sprintf("%s%s%-12s%-28s" NOR " - %s %3d/%6d\n",
                        (lrn[sname[i]] >= (skl[sname[i]]+1) * (skl[sname[i]]+1)) ? HIM : "",
                        (member_array(sname[i], mapped)==-1? "  ": "□"),
                        to_chinese(sname[i]), " (" + sname[i] + ")",
                        skill_level(SKILL_D(sname[i])->type(), skl[sname[i]]),
                        skl[sname[i]], (int)lrn[sname[i]],
        	));
        
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
        string str = "\n";
        int i = sizeof(skill_level_desc);

        while(i--) {
                str = "    " + skill_level_desc[i] + str;
                if (i % 5 == 0) str = "\n" + str;
        }
        write(@HELP
指令格式 : cha [<某人>]

这个指令可以让你查询所学过的技能。
你也可以指定一个和你有师徒关系的对象，用 skills 可以查知对方的技能状况。
巫师可以查询任何人或 NPC 的技能状况。
技能的描述由低到高依次为：
HELP);
        write(str);
    return 1;
}
