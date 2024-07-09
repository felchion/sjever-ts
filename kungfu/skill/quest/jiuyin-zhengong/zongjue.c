#include <ansi.h>

int query_ap() {return 3;}

int exert(object me)
{
	int i;

	if(me->query_temp("jiuyin/powerup"))
		return notify_fail("你正在运使九阴总诀。\n");

	if((i=(int)me->query_skill("jiuyin-zhengong", 1) )< 200)
		return notify_fail("你的九阴真功修为还不够。\n");

	if ((int)me->query_skill("daode-jing", 1) < 200)
		return notify_fail("你的道德经修为还不够。\n");

	if( (int)me->query("neili") < 1500 )
		return notify_fail("你的真气不够。\n");

	me->add("neili", -500);
	me->add_temp("apply/damage", 500);
	me->add_temp("apply/pfm_damage", 150+i/5);
	me->add_temp("apply/pfm_attack", 180);
	me->add_temp("apply/pfm_dodge",  180);
	me->add_temp("apply/pfm_parry",  180);
	me->set_temp("jiuyin/powerup", 1);
	message_vision(HIY"$N运起九阴总诀，功夫威力陡然大增！\n"NOR, me);

	if ( me->is_fighting())
		me->start_busy(random(2));
	call_out("check_fight", 1, me, i,180);
	return 1;
}


void remove_effect(object me,int );
void check_fight(object me, int lvl, int count){
	if (!me) return;
	if( me->query_skill_mapped("force") != "jiuyin-zhengong"
	|| !me->is_fighting()
	|| count < 1 ){
		remove_effect(me, lvl);
		return;
	}
	call_out("check_fight", 1, me, lvl, count-1);
}


void remove_effect(object me,int lv)
{
	if (objectp(me)) 
	{
		me->delete_temp("jiuyin/powerup");
		me->add_temp("apply/damage", -500);
		me->add_temp("apply/pfm_damage", -150-lv/5);
		me->add_temp("apply/pfm_attack", -180);
		me->add_temp("apply/pfm_dodge",  -180);
		me->add_temp("apply/pfm_parry",  -180);
		message_vision(HIY"$N的九阴总诀运使完毕，吁了一口气。\n"NOR, me);
	}
}

string exert_name(){ return HIY"九阴总诀"NOR; }

int help(object me)
{
        write(HIY"\n九阴真功之「九阴总诀」："NOR"\n\n");
        write(@HELP
        要求：  当前内力 1500 以上；
        	道德经等级 200 以上；
                九阴真功等级 200 以上。

HELP
        );
        return 1;
}
