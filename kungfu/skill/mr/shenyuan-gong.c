#include <ansi.h>
inherit FORCE;

int valid_enable(string usage)
{
        return usage == "force";
}

#include "/kungfu/skill/force.h"

int valid_learn(object me)
{
        if ((int)me->query_skill("force", 1) < 10)
                return notify_fail("你的基本内功火候还不够，无法领会神元功。\n");

	if ( me->query("gender") == "无性")
		return notify_fail("你无根无性，阴阳不调，难以领会高深的神元功。\n");

        if ((int)me->query_skill("shenyuan-gong", 1) > me->query_skill("force", 1) + 10
          && me->query_skill("shenyuan-gong", 1) >= 200 )
               return notify_fail("你的基本功火候未到，必须先打好基础才能继续提高。\n");

        return valid_public(me);
}

int practice_skill(object me)
{
       if((int)me->query_skill("shenyuan-gong", 1) >= 200 )
       {
           if( (int)me->query("potential", 1) > 5 && (int)me->query("neili", 1) > 200)
           {
                   me->improve_skill("shenyuan-gong", (int)me->query_int());
                   me->add("neili", -100); 
                   me->add("potential", -1*(1+random(3)));
                   return 1;
           }
           else return notify_fail("你现在的修为不足以提高神元功了。\n");
       }
        else return notify_fail("你现在的神元功修为只能用学(learn)的来增加熟练度。\n");

              
}

string exert_function_file(string func)
{
        return __DIR__"mr/shenyuan-gong/" + func;
}
mapping exercise_msg(object me)
{
	return ([
		"status_msg" : HIB + me->name()+"眼神凌厉，神态极为凝重。" NOR,
		"start_my_msg" : "你随意一站，双手缓缓抬起，深吸一口气，真气开始在体内运转。\n",
		"start_other_msg" : me->name()+"随意在那里一站，双手缓缓抬起，眼神开始变的凌厉起来。\n",
		"halt_msg" : "$N眉头一皱，急速运气，把手放了下来。\n",
		"end_my_msg" : "你将真气在体内沿脉络运行了一圈，缓缓纳入丹田，放下手，长吐了一口气。\n",
		"end_other_msg" : "只见"+me->name()+"的眼睛慢慢闭上，立刻睁开，把手放下，长吐了一口气。\n"
	]);
}
