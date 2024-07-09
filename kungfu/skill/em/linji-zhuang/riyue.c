// riyun.c
// by snowman@SJ 27/01/2000
// Modified by Lane@SJ 2005/4/14


#include <ansi.h>

int query_ap() {return 5;}

int check_fight(object me);

int exert(object me)
{       
	int neili;
       
	if( !me->is_fighting() )
		return notify_fail("「日月」字决必须在战斗中使用。\n");

	if( (int)me->query_skill("linji-zhuang", 1) < 150 )
		return notify_fail("你的临济十二庄修为还不够。\n");

	if( me->query("class") != "bonze" && me->query("family/generation") > 3)
		return notify_fail("你不是出家人，无法领会临济十二庄的「日月」字决！\n");

	if( (int)me->query("neili") < 1000 )
		return notify_fail("你的真气不够。\n");

	if( me->query_temp("ljz/riyue") )
		return notify_fail("你正在运用临济十二庄的「日月」字决！\n");

	tell_object(me,MAG"\n你五心向天，潜运日月二庄，益气升阳，益阴潜阳，升降反正，天地二气交泰于身，顿觉自己内力源源不止了。\n"NOR);
	message_vision(HIW"\n只见$N脸上红光时隐时现，一下变得神采弈弈。\n\n"NOR, me);

	if( me->query_temp("weapon") )
		me->set_temp("ljz/weapon", 1);
	else me->set_temp("ljz/weapon", 0);

	neili = (int)me->query("neili");
	me->set_temp("ljz/riyue", neili);
	check_fight(me);
	return 1;

}

int check_fight(object me)
{
	int i;
	if( !me || !(int)me->query_temp("ljz/riyue") ) return 0;

	if( me->is_ghost() || !living(me) )
		me->delete_temp("ljz/riyue");

	if( me->query_temp("weapon") )
		i = 1;
	else
		i = 0;

	if( !me->is_fighting()
	|| me->query("neili") <= 10
	|| me->query_temp("ljz/weapon") != i
	|| (userp(me)
	&& me->query_skill_mapped("force") != "linji-zhuang") )
		call_out("remove_effect", 1, me);
	else {
		if( me->query("neili") < (int)me->query_temp("ljz/riyue") ) {
			tell_object(me, "你体内日月之力运转不息，使你感觉不到任何内力的消耗！\n");
			me->set("neili", me->query_temp("ljz/riyue"));
		}
		call_out("check_fight", 1, me);
	}       
	return 1;
}       

void remove_effect(object me)
{
	if( !me || !me->query_temp("ljz/riyue") ) return;
	me->delete_temp("ljz/weapon");
	me->delete_temp("ljz/riyue");
	message_vision(HIR"$N所聚天地之精气已散回日月之间，$N又恢复了原有的气力消耗。\n\n"NOR, me);
}

string exert_name(){ return HIY"日月诀"NOR; }

int help(object me)
{
        write(MAG"\n临济十二庄「日月诀」："NOR"\n\n");
        write(@HELP
        要求：  当前内力 1000 以上；
                临济十二庄等级 150 以上；
                出家或俗家三代弟子；
HELP
        );
        return 1;
}
