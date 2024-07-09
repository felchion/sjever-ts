// shenhuo-shengong.c
#include <ansi.h>
inherit FORCE;

int valid_enable(string usage) { return usage == "force"; }

#include "/kungfu/skill/force.h"

int valid_learn(object me)
{
        if (me->query_skill("force", 1) < 10)
      	    return notify_fail("你的基本内功还需要进一步提高。\n");

	if ( me->query("gender") == "无性")
		return notify_fail("你无根无性，阴阳不调，难以领会高深的圣火神功。\n");

        if ((int)me->query_skill("shenghuo-shengong", 1) > me->query_skill("force", 1) + 10
          && me->query_skill("shenghuo-shengong", 1) >= 200 )
               return notify_fail("你的基本功火候未到，必须先打好基础才能继续提高。\n");

	return valid_public(me);
}

int practice_skill(object me)
{
       if((int)me->query_skill("shenghuo-shengong", 1) >= 200 )
       {
           if( (int)me->query("potential", 1) > 5 && (int)me->query("neili", 1) > 200)
           {
                   me->improve_skill("shenghuo-shengong", (int)me->query_int());
                   me->add("neili", -100); 
                   me->add("potential", -1*(1+random(3)));
                   return 1;
           }
           else return notify_fail("你现在的修为不足以提高圣火神功了。\n");       }
        else return notify_fail("你现在的圣火神功修为只能用学(learn)的来增加熟练度。\n");

}

string exert_function_file(string func)
{
	return SKILL_D("mj/shenghuo-shengong/") + func;
}
mapping exercise_msg(object me)
{
  return ([
    "status_msg" : HIR + me->name()+"神情严肃，周身宛如一团火焰。" NOR,
    "start_my_msg" : "你盘膝而坐，双手垂于胸前成火焰状，深吸口气，让经络中的真气化做一股灼流缓缓涌入丹田。\n",
    "start_other_msg" : me->name()+"盘膝而坐，双手在前胸搭住，神情严肃，周身仿如一团火焰般相仿。\n",
    "halt_msg" : "$N周身微微颤动，长出口气，站了起来。\n",
    "end_my_msg" : "你将周身内息贯通经脉，缓缓睁开眼睛，站了起来。\n",
    "end_other_msg" : "只见"+me->name()+"睁开眼睛，站了起来，头顶凝聚的白气慢慢飘散开。\n",
    "heal_msg" : HIW"$N盘膝而坐，双手十指张开，举在胸前，作火焰飞腾之状，运起"RED"圣火神功"HIW"开始疗伤。\n"NOR,
    "heal_finish_msg" : HIW"$N脸上流光浮现，一声：“焚我残躯，熊熊圣火，生亦何欢，死亦何苦？”，缓缓站起。\n"NOR,
    "heal_unfinish_msg" : "$N神态庄严，缓缓站起身来，但脸上血红，看来伤势还没有完全恢复。\n",
    "heal_halt_msg" : "$N脸颊胀红，头顶热气袅袅上升，猛地吸一口气，挣扎着爬了起来。\n",
    ]);
}
