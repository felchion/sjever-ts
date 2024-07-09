//  No Copyright Message (6.17.1997)

#include <ansi.h>
#include <level.h>

inherit ITEM;


void create()
{
    seteuid(getuid());
	set_name (HIY"神眼"NOR, ({ "godeye" }));
	set("long",@LONG
这是一颗神眼，透过它你可以窥视(peek)其它玩家的技能等级。
LONG
);

	set("unit","颗");
	set("no_get", 1);
	set("no_drop", 1);
	set_weight(-100000);
	setup();
}

void init()
{
    seteuid(geteuid());
    add_action("do_peek","peek");    
}

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
"whip",  "axe",    "hammer",
});

string skill_level(string, int);
int sort_skill(string sk)
{
	return (SKILL_D(sk)->type() != "martial" && SKILL_D(sk)->type() != "array");
}

int sort_basic(string sk)
{
	return member_array(sk, valid_type)!=-1;
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
                	grade = level / 7;
                        if( grade >= sizeof(skill_level_desc) )
                                grade = sizeof(skill_level_desc)-1;
                        return skill_level_desc[grade];
        }
}

int do_peek(string arg)
{
        object me,ob,body;
        mapping skl, lrn, map;
        string dinfo,*sname, *mapped, *allskills, str = "";
        int i, j;
        int last_on = strlen(arg);
        me=this_player();
        dinfo=(geteuid(this_object()));

        if(!arg)
                return notify_fail(dinfo+"你想窥视哪个玩家。\n");           
        else{
          while (last_on--) if (arg[last_on] < 'a' || arg[last_on] > 'z')
		    {return notify_fail(arg+"是什么东东？\n");break;}
        }

	    if(!objectp(body = LOGIN_D->find_body(arg))) {
	       body = new(USER_OB);
	       seteuid("aloni");
	       dinfo=geteuid(this_object());
	       export_uid(body);
	       body->set_name("", ({ arg }));
	       //dinfo = body->query_save_file();
	       if (!body->restore())
             { destruct(body); return notify_fail(dinfo+"没有这个玩家。\n");}
	       //msg = user->name();
	       body->create();
		   //if( !body->restore() ) {destruct(ob); destruct(body); return notify_fail("没有这个人。\n");}
	    }

        skl = body->query_skills();
        if(!j = sizeof(skl)) {
                write( (body==me ? "你" : body->name(1)) + "目前并没有学会任何技能。\n");
                return 1;
        }
        
        allskills  = sort_array( keys(skl), (: strcmp :) );
        
        str = HIW"【";
        str += (body==me?"你":body->name(1))+"的技能表】"NOR"：总共"+chinese_number(j)+"项技能";
        if(j > 31) str+= "，已经严重影响了以后的进步！";
        str += "\n┌────────────────────────────────────────────┐\n";
	    map = body->query_skill_map();
        if( mapp(map) ) mapped = values(map);
        if( !mapped ) mapped = ({});

        lrn = body->query_learned();
        if( !mapp(lrn) ) lrn = ([]);
        
        sname  = filter_array( allskills, (: sort_skill :) );
        allskills -= sname;
        
        j = sizeof(sname);
        if( j > 0){
        	str += "│"HIY"『杂项 "+chinese_number(j)+" 种』"NOR"：\t\t\t\t\t\t\t\t\t  ";
        	for(i=0; i < j; i++) {
        		if (i % 2 == 0) str += "│\n│";
        		str += sprintf("%s%-8s%-13s"HIC"→"NOR"%s %3d/%6d  ",
                        	(lrn[sname[i]] >= (skl[sname[i]]+1) * (skl[sname[i]]+1)) ? HIM : "",
                        	to_chinese(sname[i]), "(" + sname[i] + ")",
                        	skill_level(SKILL_D(sname[i])->type(), skl[sname[i]]),
                        	skl[sname[i]], (int)lrn[sname[i]],);
                        if ( i > j-2 && i % 2 == 0)
                        	str +="\t\t\t\t\t\t  ";
        	}
        	str += "│\n├────────────────────────────────────────────┤\n";
        }
        
        sname  = filter_array( allskills, (: sort_basic :) );
        allskills -= sname;
        
        j = sizeof(sname); 
        if(j > 0){
        	str += "│"HIY"『基本 "+chinese_number(j)+" 种』"NOR"：\t\t\t\t\t\t\t\t\t  ";
        	for(i=0; i < j; i++) {
        		if (i % 2 == 0) str += "│\n│";
        		str += sprintf("%s%-8s%-13s"HIC"→"NOR"%s %3d/%6d  ",
                        	(lrn[sname[i]] >= (skl[sname[i]]+1) * (skl[sname[i]]+1)) ? HIM : "",
                        	to_chinese(sname[i]), "(" + sname[i] + ")",
                        	skill_level(SKILL_D(sname[i])->type(), skl[sname[i]]),
                        	skl[sname[i]], (int)lrn[sname[i]]);
                        if ( i > j-2 &&i % 2 == 0)
                        	str +="\t\t\t\t\t\t  ";
        	}
        	str += "│\n├────────────────────────────────────────────┤\n";
        }
        
        sname  = allskills;
        
        j = sizeof(sname);
        if(j > 0){
        	str += "│"HIY"『武功 "+chinese_number(j)+" 种』"NOR"：\t\t\t\t\t\t\t\t\t  │\n";
        	for(i=0; i < j; i++) {
        		str += sprintf("│  %s%s%-12s%-28s" HIC "→ "NOR"%s %3d/%7d %22s\n",
                        (lrn[sname[i]] >= (skl[sname[i]]+1) * (skl[sname[i]]+1)) ? HIM : "",
                        (member_array(sname[i], mapped)==-1? "  ": "□"),
                        to_chinese(sname[i]), " (" + sname[i] + ")",
                        skill_level(SKILL_D(sname[i])->type(), skl[sname[i]]),
                        skl[sname[i]], (int)lrn[sname[i]],"│");
        	}
        	str += "└────────────────────────────────────────────┘\n";
        }
        
        write(str);
        if (!find_player(arg)) destruct(body); 
        return 1;
}