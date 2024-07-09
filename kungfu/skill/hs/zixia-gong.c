// zixia-gong.c 紫霞神功

#include <ansi.h>
inherit FORCE;

int valid_enable(string usage) { return usage == "force"; }

#include "../force.h"

int valid_learn(object me)
{
        
        int lvl, i;
        lvl = (int)me->query_skill("zixia-gong", 1);
        i = (int)me->query("shen", 1);

        if ( me->query("gender") == "无性" && lvl > 49)
                return notify_fail("你无根无性，阴阳不调，难以领会高深的紫霞神功神功。\n");

        if ( me->query("class") == "bonze" )
                return notify_fail("紫霞神功讲究阴阳调合，有违佛家六根清净之意，"
                        +RANK_D->query_respect(me)+"欲修此功，已是罪过。\n");

        if ((int)me->query_skill("force", 1) < 15)
                return notify_fail("你的基本内功火候还不够。\n");

        if ((lvl > 10 && lvl < 100 && i < lvl*lvl*lvl / 20 )
            || ( lvl >=100 && i < 10000))
                return notify_fail("你的侠义正气太低了，无法学习高深的紫霞神功。\n");

        return valid_public(me);
}

int practice_skill(object me)
{
       if((int)me->query_skill("zixia-gong", 1) >= 200 )
       {
           if( (int)me->query("potential", 1) > 5 && (int)me->query("neili", 1) > 200)
           {
                   me->improve_skill("zixia-gong", (int)me->query("int"));
                   me->add("potential", -1*(1+random(3)));
                   me->add("neili", -100); 
                   return 1;
           }
           else return notify_fail("你现在的修为不足以提高紫霞功了。\n");
       }
        else return notify_fail("紫霞功只能用学(learn)的来增加熟练度。\n");
}

string exert_function_file(string func)
{
        return __DIR__"hs/zixia-gong/" + func;
}

mapping exercise_msg(object me)
{
	return ([
		"status_msg" : MAG + me->name()+"双手捏个剑诀，脸上紫气浮现。"NOR,
		"start_my_msg" : MAG"你屏息静气，坐了下来，左手搭在右手之上，在胸前捏了个剑诀，引导内息游走各处经脉。\n"NOR,
		"start_other_msg" : MAG + me->name() +"坐了下来，少顷脸上隐隐笼罩了一层紫气。\n"NOR,
		"halt_msg" : MAG"$N心神一动，将内息压回丹田，双臂一振站了起来。\n"NOR,
		"end_my_msg" : MAG"你将内息走满一个周天，只感到全身通泰，丹田中暖烘烘的，双手一分，缓缓站了起来。\n"NOR,
		"end_other_msg" : MAG""+me->name()+"脸上紫气退了下去，站了起来。\n"NOR
	]);
}
