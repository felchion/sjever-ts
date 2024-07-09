// bihai-chaosheng.c
#include <ansi.h>

inherit FORCE;

int valid_enable(string usage) { return usage == "force"; }

#include "../force.h"

int valid_learn(object me)
{
	if ( me->query("gender") == "无性")
		return notify_fail("你无根无性，阴阳不调，难以领会高深的碧海潮生神功。\n");

	if ((int)me->query_skill("bihai-chaosheng", 1) > me->query_skill("force", 1) + 10
	 && me->query_skill("bihai-chaosheng", 1) >= 200 )
		return notify_fail("你的基本功火候未到，必须先打好基础才能继续提高。\n");

	return valid_public(me);
}

int practice_skill(object me)
{
	if((int)me->query_skill("bihai-chaosheng", 1) >= 200 ){
		if( (int)me->query("potential", 1) > 5 && (int)me->query("neili", 1) > 200){
			me->improve_skill("bihai-chaosheng", (int)me->query_int());
			me->add("neili", -100); 
			me->add("potential", -1*(1+random(3)));
			return 1;
		}
		else return notify_fail("你现在的修为不足以提高碧海潮生功了。\n");
	}
	else return notify_fail("你现在的碧海潮生功修为只能用学(learn)的来增加熟练度。\n");
}

string exert_function_file(string func)
{
	return SKILL_D("thd/bihai-chaosheng/") + func;
}

mapping exercise_msg(object me)
{
	return ([
		"status_msg" : HIC + me->name() + "面容肃穆，静坐如渊岳，沐轻风而不惊。" NOR,
		"start_my_msg" : "你盘腿坐下，双目微闭，双手掌心相向成虚握太极，天人合一，练气入虚。\n",
		"start_other_msg" : me->name() + "盘腿坐下，双目微闭，双手掌心相向成虚握太极。轻风吹过，" + me->name() + "身上的衣袍竟然不为所动。\n",
		"end_my_msg" : "你将内息又运了一个小周天，缓缓导入丹田，双臂一震，站了起来。\n",
		"end_other_msg" : me->name() + "的脸上红光乍现，又立刻隐去。随即双臂一震，站了起来。\n",
		"heal_msg" : HIC"$N凝神静气，内息随悠扬箫声在全身游走，恰似碧海浪涛般冲击受损被封经脉。\n"NOR,
		"heal_finish_msg" : HIC"$N脸色渐渐变得舒缓，箫声亦随之急转直下，渐不可闻。\n"NOR,
		"heal_unfinish_msg" : HIR"$N内息微弱，难以为济，箫声募然高亢渐而消失，人随之站起，猛地吐出一口鲜血。\n"NOR,
		"heal_halt_msg" : HIC"$N突感周身不适，内息全然不受乐音指引，急忙停止吹奏，箫声骤然竭止。\n"NOR,
	]);
}
