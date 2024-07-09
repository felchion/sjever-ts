//flowray 2005.6.16
#include <ansi.h>
#include <level.h>


inherit SPEC; 


void create()
{
	set_name (HIY"神眼"NOR, ({ "shenyan","eye" }));
	set("long","这是一颗"HIY"神眼"NOR"，透过它你可以查看(chk)其它玩家的技能等级。\n");
	set("unit","颗");
	set_weight(100);
	set("value",3000000);
	
	set("no_get",1);
	set("no_drop",1);
	set("no_get_from",1);
	
	set("degree",3);
	set("flag","spec/shenyan");
	set("desc","查看其它玩家的技能表。");
	set("credit",2);
	setup();
}

void init()
{
	add_action("do_check","chk");    
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

int sort_skill(string sk)
{
	return (SKILL_D(sk)->type() != "martial" && SKILL_D(sk)->type() != "array");
}

int sort_basic(string sk)
{
	return member_array(sk, valid_type)!=-1;
}

int do_check(string arg)
{
        object me=this_player(),ob;
        mapping skl, lrn, map;
        string *sname, *mapped, *allskills, str = "";
        int i, j;
        int last_on = strlen(arg);
        
  	if (!restrict()) {return notify_fail("本周你已经不能使用"+this_object()->query("name")+"了。\n");}
	    
	if(!arg)
                return notify_fail("你想查看谁。\n");           
	else{
		while (last_on--) if (arg[last_on] < 'a' || arg[last_on] > 'z')
			{return notify_fail(arg+"是什么东东？\n");break;}
        }

	if(!objectp(ob = LOGIN_D->find_body(arg))) {
		ob = new(USER_OB);
	    	seteuid(arg);
	   		export_uid(ob);
	      	ob->set_name("", ({ arg }));
		if (!ob->restore())
		{ destruct(ob); return notify_fail("没有这个人。\n");}
	}
		
		
	if (wiz_level(me) < wiz_level(ob))
		return notify_fail(HIW"神眼突然发出耀眼的白光照得你两眼都睁不开。\n"NOR);
		
	if (ob == me)
		return notify_fail("你还是去照镜子吧！\n");
        
	skl = ob->query_skills();
	
	if(!j = sizeof(skl)) {
		write(HIY"冥冥中你看到"+ob->name(1)+"目前没有学会任何技能。\n"NOR);
		return 1;
        }
        
	allskills  = sort_array( keys(skl), (: strcmp :) );
        
	str = HIY"冥冥中你看到";
        str += ob->name(1)+"总共有"+chinese_number(j)+"项技能"NOR;
        str += "\n┌────────────────────────────────────────────┐\n";
	map = ob->query_skill_map();
        if( mapp(map) ) mapped = values(map);
        if( !mapped ) mapped = ({});

        lrn = ob->query_learned();
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
                        	"cmds/skill/skills.c"->skill_level(SKILL_D(sname[i])->type(), skl[sname[i]]),
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
                        	"cmds/skill/skills.c"->skill_level(SKILL_D(sname[i])->type(), skl[sname[i]]),
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
                        "cmds/skill/skills.c"->skill_level(SKILL_D(sname[i])->type(), skl[sname[i]]),
                        skl[sname[i]], (int)lrn[sname[i]],"│");
        	}
        	str += "└────────────────────────────────────────────┘\n";
        }
        write(str);
        if (!find_player(arg)) destruct(ob);
        degree(); 
        return 1;
}
