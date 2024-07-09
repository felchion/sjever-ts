// huagong-dafa.c 化功大法

#include <ansi.h>
inherit FORCE;

int valid_enable(string usage) { return usage == "force"; }
#include "../force.h"

int valid_learn(object me)
{
	int j, t, lvl, shen, mforce;
	lvl = (int)me->query_skill("huagong-dafa", 1);
	shen = (int)me->query("shen");
	mforce = me->query_skill("force", 1);

	t = 1;

	for ( j = 1; j < lvl / 10; j++) t *= 2;
	if ( t > 5000 ) t = 5000;

	if ( me->query_skill("buddhism",1) || me->query_skill("taoism",1) )
		return notify_fail("你练了别派心法，再学化功大法会走火入魔！\n");

	if ( mforce < 10)
		return notify_fail("你的基本内功还差点儿, 学不了化功大法。\n");

	if ( lvl > mforce + 10 && lvl >= 200 )
		return notify_fail("你的基本功火候未到，必须先打好基础才能继续提高。\n");

	if ( (lvl > 10 && lvl < 100 && shen > -(t * 200))
	|| ( lvl >=100 && shen > -50000))
		return notify_fail("你的邪气太少了。\n");

        return valid_public(me);
}

int practice_skill(object me)
{
	if( me->query_skill("huagong-dafa", 1) >= 200 ){
		if( me->query("potential", 1) > 5 && me->query("neili", 1) > 200) {
			me->improve_skill("huagong-dafa", me->query_int());
			me->add("neili", -100);
			me->add("potential", -1*(1+random(3)));
			return 1;
		}
		return notify_fail("你现在的修为不足以提高化功大法了。\n");
	}
	return notify_fail("你现在的化功大法修为只能用学(learn)的来增加熟练度。\n");
}

string exert_function_file(string func)
{
	return __DIR__"xx/huagong-dafa/" + func;
}

void skill_improved(object me)
{
	int skill, poison;
	skill = me->query_skill("huagong-dafa", 1);
	poison = me->query_skill("poison", 1);

	if(skill >= 100 && !me->query("hgdf") && poison > 100){
		tell_object(me, HIY"冥冥中你感觉到体内的化功大法和毒技相互相济，使你的内力修为有所提高！\n"NOR);
		me->add("max_neili", 50);
		me->set("hgdf", 1);
	}
	if(skill >= 150 && me->query("hgdf")==1 && poison > 150){
		tell_object(me, HIY"冥冥中你感觉到体内的化功大法和毒技相互相济，使你的精力修为有所提高！\n"NOR);
		me->add("max_jingli", 50);
		me->add("eff_jingli", 50);
		me->set("hgdf", 2);
	}
	if(skill >= 200 && me->query("hgdf")==2 && poison > 200){
		tell_object(me, HIY"冥冥中你感觉体内的化功大法和毒技相互相济，使你的武功修为有所提高！\n"NOR);
		me->add("combat_exp", 5000);
		me->set("hgdf", 3);
	}
}

mapping exercise_msg(object me)
{
	return ([
		"status_msg" : BLU + me->name()+"正坐在地上修练内息，身体被一股青烟环绕着。" NOR,
		"start_my_msg" : "你气运丹田，将体内毒素慢慢逼出，控制着它环绕你缓缓飘动。你感觉到内劲开始有所加强了。\n",
		"start_other_msg" : me->name()+"盘膝坐下运功，身体渐渐被一股青烟所包围。\n",
		"halt_msg" : "$N双眼一睁，眼中射出一道精光，接着阴阴一笑，站了起来。\n",
		"end_my_msg" : "你感觉毒素越转越快，就快要脱离你的控制了！你连忙收回毒素和内息，冷笑一声站了起来。\n",
		"end_other_msg" : me->name()+"阴阴一声冷笑，呼出一口青烟，站起身来藐视地扫了在场的人一眼。\n"
	]);
}
