// wuwo.c
// Modified by Lane@SJ 2005/4/14

#include <ansi.h>
int query_ap() {return 3;}
int check(object me);
int exert(object me)
{

	int nh = me->query_skill("linji-zhuang", 1);

	if( !me->is_fighting() )
	return notify_fail("你只能在战斗中，运用「无我」字决！\n");

	if( me->query_skill("linji-zhuang", 1) < 160 )
	return notify_fail("你的临济十二庄修为还不够。\n");

	if( me->query("class") != "bonze" && me->query("family/generation") > 3 )
	return notify_fail("你不是出家人，无法领会临济十二庄的「无我」字决！\n");

	if( me->query_skill("medicine", 1) < 80 )
	return notify_fail("你的本草术理等级不够。\n");

	if( me->query("neili") < 500 )
	return notify_fail("你的真气不够。\n");

	if( me->query("jingli") < 300 )	// 增加精力要求
	return notify_fail("你的精力不够。\n");

	if( me->query_temp("ljz/wuwo") )
	return notify_fail("你正在运用临济十二庄的「无我」字决！\n");

	message_vision(HIW"只见$N深深吸了一口气，脸上露出慈祥的笑容。\n"NOR, me);
	me->set_temp("ljz/wuwo", nh);
	me->add("neili", - 200);
	check(me);
	me->start_exert(1,"「无我」字决");
	me->start_busy(random(2));
	me->start_call_out( (: call_other, __FILE__, "remove_effect", me :), nh/3);
	return 1;
}

int check(object me)
{
	if( !me || !(int)me->query_temp("ljz/wuwo") ) return 0;

	if( me->is_ghost() || !living(me) )
	me->delete_temp("ljz/wuwo");

	if( me->query("jingli") < 100
	|| me->query("neili") < 120
	|| me->query("qi") < 1		// 避免 BUG，气（-1）还恢复
	|| !me->is_fighting()
	|| (userp(me) && me->query_skill_mapped("force") != "linji-zhuang") ) {
		message_vision(HIC"$N暂缓呼吸，停止了内息的流动。\n"NOR, me);
		me->start_call_out( (: call_other, __FILE__, "remove_effect", me :), 1);
	}
	else{
		if( me->query("qi") < (int)me->query("max_qi") && ( !me->is_busy() || random(2) ) ) {
			message_vision(HIY"$N脸色一沈又回复正常，想是正在调节内息。\n"NOR,me);
			me->add("neili", -50 );
			me->add("eff_qi", me->query_skill("linji-zhuang", 1)); // 增加上限恢复
			if( me->query("eff_qi") > me->query("max_qi") )
			me->set("eff_qi", me->query("max_qi"));
		}
		call_out("check", 2+random(2), me);	// 缩短运行时间，增加效?
	}
	return 1;
}

void remove_effect(object me)
{
	int nh;
	if( !me || !me->query_temp("ljz/wuwo") ) return;
	nh = me->query_temp("ljz/wuwo");
	me->start_exert(0);
	me->delete_temp("ljz/wuwo");
	message_vision(HIR"$N散去了「无我」心法，又恢复了原来的$N。\n\n"NOR,me);
}

string exert_name(){ return HIW"无我诀"NOR; }

int help(object me)
{
        write(HIW"\n临济十二庄「无我诀」："NOR"\n\n");
        write(@HELP
        要求：  当前内力 500 以上；
                当前精力 300 以上；
                临济十二庄等级 160 以上；
                本草术理 80 以上；
                出家或俗家三代弟子；
HELP
        );
        return 1;
}
