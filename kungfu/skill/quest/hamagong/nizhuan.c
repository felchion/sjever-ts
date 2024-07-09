// nizhuan.c 逆转九阴
// by snowman@SJ
// Modify by server 2007.07 完善逆转九阴最后Quest 威力
// Modify by server 2007.08 完善逆转九阴Quest 等级描述

#include <ansi.h>

inherit F_CLEAN_UP;

int query_ap() {return 4;}

string exert_name()
{
	return HIW"经脉逆转"NOR;
}

int exert(object me, object target)
{
	int lv;

	if( !me->is_fighting(target))
	return notify_fail("经脉逆转只能对战斗中使用。\n");

	if(!(int)me->query_skill("hamagong", 1) )
	return 0;
	
	if((int)me->query("neili") < 1000  )
	return notify_fail("你的内力不够。\n");

	if((int)me->query_temp("hmg_nizhuan") )
	return notify_fail("你已经逆转经脉。\n");

	if (me->query_skill("hamagong", 1) < 200)
	return notify_fail("你蛤蟆功修为太低无法逆转经脉。\n");
	

  lv = (me->query_skill("force",1) + me->query_skill("hamagong", 1))/2;


	me->add("neili", -lv);

	message_vision(HIW "\n$N双手撑地，口中发出一连窜低喉声，一股股内劲勃发使$N双足离地，呈头下足上之势。\n" NOR, me);
	tell_object(me, HIW "你感觉体内蛤蟆功随经脉逆转，好像有股巨大的力量要崩发出来一样。\n" NOR);


	me->add_temp("apply/damage", 500+lv);
	me->add_temp("apply/pfm_damage", 250+lv/5);
	me->add_temp("apply/pfm_attack", 600);
	me->add_temp("apply/pfm_parry",  600);
	me->add_temp("apply/pfm_dodge",  600);
	me->set_temp("apply/action/color",HBBLU);
	me->set_temp("hmg_nizhuan",1);

	call_out("check_fight", 1, me, lv, 10+lv/10);

	return 1;
}

void remove_effect(object me,int lv);
void check_fight(object me, int lvl, int count){
	if (!me) return;
	if( me->query_skill_mapped("force") != "hamagong"
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
		me->delete_temp("hmg_nizhuan");
		me->delete_temp("apply/action/color");
	  me->add_temp("apply/damage", -500-lv);
	  me->add_temp("apply/pfm_damage", -250-lv/5);
	  me->add_temp("apply/pfm_attack", -600);
	  me->add_temp("apply/pfm_parry",  -600);
	  me->add_temp("apply/pfm_dodge",  -600);
		tell_object(me, HIW"你逆转九阴运功过久，经脉又渐渐复原。\n"NOR);
	}
}

int help(object me)
{
write(WHT"\n蛤蟆功「逆转九阴」："NOR"\n");
write(@HELP
欧阳锋的独门绝技蛤蟆功乃是天下武学中的绝顶功夫。自从他逆练九阴成疯
而又夺得天下第一的称号后，这蛤蟆功更大逆寻常武功，更加怪异无伦。这
逆转九阴之法，便是当年他传与杨过的绝技！

      要求：当前内力 1000 以上；
            蛤蟆功等级 200级以上；
　　　　　　激发内功为蛤蟆功；
HELP
);
return 1;
}
