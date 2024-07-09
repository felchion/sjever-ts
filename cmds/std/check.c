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
        object ob, man, *all;
        mapping fam, skl, lrn, map;
        string *sname, *mapped;
        int sp, dp;
        int i, cost;

        seteuid(getuid());

    	if (me->is_busy()) return notify_fail("你正忙着呢。\n");
    	
        all = all_inventory(environment(me));
        for(i=0; i<sizeof(all); i++) {
        	if( living(all[i]) && !userp(all[i]) && all[i]->query("can_speak") && !all[i]->query("mute")) 
        		man = all[i];
        }

        if ( (!(fam = me->query("family")) || fam["family_name"] != "丐帮") )
                return notify_fail("只有乞丐才能打探别人的技能！\n");

        if ( !objectp(man) )
                return notify_fail("周围没人能帮你去了解别人的技能！\n");

        if ( me->query_skill("checking",1) < 10 )
                return notify_fail("你的打探本领尚未纯熟，无法了解别人的技能！\n");

        if ( !arg )
                return notify_fail("你要打听谁的技能？\n");

        if ( present(arg, environment(me)) )
                return notify_fail("你要打听的人就在边上，在这里问不大妥当吧？\n");

        ob = find_player(arg);
        if (!ob) ob = find_living(arg);
        if (!ob || !me->visible(ob)) return notify_fail("你要打听谁的技能？\n");

        if ( ob == me )
                return notify_fail("你没这么笨吧，要别人告诉你自己的技能？\n");

        cost = me->query("max_jing")/(me->query_skill("checking",1)/10) - 10;

        if ( me->query("jing") <= cost )
                return notify_fail("现在你太累了，无法去打听别人的技能。\n");

        tell_object(me, "你走上前去，小心翼翼地向" + man->name(1) + "打听关于" + ob->name() + "的情况 ...\n\n");
        message("vision", "只见" + me->name() + "陪着笑脸跟" + man->name() + "说着话，好象在打听些什么。\n\n", 
                environment(me), ({ me, man }) );
        me->start_busy(1+random(2));

        sp = me->query_skill("checking")*10 + me->query("kar")*5 + me->query("jing") + man->query("jing");
        dp = ob->query("kar")*5 + ob->query("jing")*2;
        
        if (ob->query("no_quest") || random(sp) < random(dp))
                return notify_fail( man->name() + "摇了摇头，说道：这事我可不清楚，"
                        + RANK_D->query_respect(me) + "还是去向别人打听一下吧！\n");

        me->receive_damage("jing", cost );
	if (ob->query("env/check") == 314) ob->add("env/check_"+me->query("id"), 1);
        skl = ob->query_skills();
        if(!sizeof(skl)) {
                write( man->name() + "告诉你： " + ob->name(1) + "目前并没有学会任何技能。\n");
                return 1;
        }
        write( man->name() + "告诉你： " + ob->name(1) +"目前所学过的技能：\n\n");
        sname  = sort_array( keys(skl), (: strcmp :) );
        
        map = ob->query_skill_map();
        if( mapp(map) ) mapped = values(map);
        if( !mapped ) mapped = ({});

        lrn = ob->query_learned();
        if( !mapp(lrn) ) lrn = ([]);
        
        i = random(sizeof(skl));

        write(sprintf("%s%s%-12s%-28s" NOR " - %s %3d/%6d\n",
                (lrn[sname[i]] >= (skl[sname[i]]+1) * (skl[sname[i]]+1)) ? HIM : "",
                (member_array(sname[i], mapped)==-1? "  ": "□"),
                to_chinese(sname[i]), " (" + sname[i] + ")",
                skill_level(SKILL_D(sname[i])->type(), skl[sname[i]]),
                skl[sname[i]], (int)lrn[sname[i]],
        ));
        if (random((int)me->query_skill("checking",1))>100){
         	me->add("combat_exp", 1 + random(4));
         	me->add("potential", random(2));
        }
        
        write("\n");
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
			grade = level / 7;
                        if( grade >= sizeof(skill_level_desc) )
                                grade = sizeof(skill_level_desc)-1;
                        return skill_level_desc[grade];
        }
}

int help(object me)
{
        write(@HELP
指令格式 : check|dating [<某人>]

这个指令可以让你打听别人所学过的技能。

你也可以指定一个和你有师徒关系的对象，用 skills|cha 可以查知对方的技能状况。

此命令为丐帮弟子专用，并需学习相应的技能。

HELP
    );
    return 1;
}
