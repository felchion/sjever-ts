// yinyang.c
// Modified by Lane@SJ 2005/4/14

#include <ansi.h>

int query_ap() {return 3;}

int exert(object me)
{
	int improve = (int)me->query_skill("force",1) * 20;

	if( !me->is_fighting() )
	return notify_fail("你只能在战斗中，运用「阴阳诀」！\n");

	if( (int)me->query_skill("linji-zhuang", 1) < 120 )
		return notify_fail("你的临济十二庄修为还不够。\n");

	if( me->query("class") != "bonze" && me->query("family/generation") > 3)
		return notify_fail("你不是出家人，无法领会临济十二庄的「阴阳诀」！\n");

	if( (int)me->query("neili") < 1000 )
		return notify_fail("你的内力不够。\n");

	if( (int)me->query("max_neili") < 1500 )
		return notify_fail("你的真气不够。\n");

	if( me->query_temp("ljz/yinyang")   )
		return notify_fail("你正在运用临济十二庄的「阴阳诀」！\n");

	message_vision(HIW"只见$N吸取天地之间的阴阳两气，顿然气力大增！\n"NOR,me);

	me->add("max_qi", improve);
	me->add("eff_qi", improve);
	me->add("qi", improve);

//	修改以前的直接恢复气血

	me->add("max_jing", improve);
	me->add("eff_jing", improve);
	me->add("jing", improve);

//	增加精血

	me->set_temp("ljz/yinyang", improve);
	me->add("neili", - 500);
	call_out("check_fight", 1, me, 180);
	return 1;
}

void remove_effect(object me)
{      
	int improve;
	if( !me || !me->query_temp("ljz/yinyang") ) return;
	improve = me->query_temp("ljz/yinyang");
	me->delete_temp("ljz/yinyang");			
	
	me->add("max_qi", -improve);
	me->add("max_jing", -improve);
	
	if( me->is_ghost() ) return;

	// 修改原来恢复上限问题
	if( me->query("eff_qi") > me->query("max_qi") )
		me->set("eff_qi", me->query("max_qi") );
	if( me->query("qi") > me->query("max_qi") )
		me->set("qi", me->query("max_qi") );

	if( me->query("eff_jing") > me->query("max_jing") )
		me->set("eff_jing", me->query("max_jing") );
	if( me->query("jing") > me->query("max_jing") )
		me->set("jing", me->query("max_jing") );

	tell_object(me, HIR"你的临济十二庄「阴阳诀」运功完毕，气力回复了正常。\n"NOR);
}

void check_fight(object me,int count){
	if (!me) return;
	if(me->query_skill_mapped("force") != "linji-zhuang"
	|| !me->is_fighting()
	|| count < 1 ){
		remove_effect(me);
		return;
	}
	call_out("check_fight", 1, me, count-1);
}

string exert_name(){ return HIR"阴阳诀"NOR; }

int help(object me)
{
        write(HIW"\n临济十二庄「阴阳诀」："NOR"\n\n");
        write(@HELP
        要求：  当前内力 1000 以上；
                最大内力 1500 以上；
                临济十二庄等级 120 以上；
                出家或俗家三代弟子。

HELP
        );
        return 1;
}
