// lianhuan.c 龙爪功连环八式
// By Spiderii 变态修改。。。。设定为高攻击，高伤害！代价是降低自身防御
/*
    空性突然间大喝一声，纵身而上，双手犹如狂风骤雨，「捕风式」、「捉影式」、「抚琴式」、「鼓瑟式」、「批亢式」、「捣虚式」、「抱残式」、「守缺式」，八式连环，疾攻而至。
    张无忌神定气闲，依式而为，捕风捉影、抚琴鼓瑟、批亢捣虚、抱残守缺，接连八招，招招后发而先至。
    空性神僧这八式连环的龙爪手绵绵不绝，便如是一招中的八个变化一般，快捷无比，
*/
//felchion 2008/3/6

inherit F_SSERVER;
 
#include <ansi.h>
#include <combat.h> 

int query_ap() {return 12;}
string perform_name(){ return RED"连环八式"NOR; }

void remove_effect(object me,object target,int lv);

int perform(object me, object target)
{
	int lv, i, j,hit;
	string lzg_msg;

	if( !target ) target = offensive_target(me);

	if( !objectp(target) || !me->is_fighting(target) )
	return notify_fail("「连环八式」只能在战斗中使用。\n");

	if( me->query_temp("lzg_lianhuan"))
	return notify_fail("你正在使用「连环八式」！\n");

	if( me->query_temp("weapon"))
	return notify_fail("你拿着武器怎么能使用「连环八式」！\n");

	if( (int)me->query_skill("longzhua-gong", 1) < 300 )
	return notify_fail("你的龙爪功还不够娴熟，使不出「连环八式」绝技。\n");

	if (me->query_skill_prepared("claw") != "longzhua-gong"
	|| me->query_skill_mapped("claw") != "longzhua-gong")
	return notify_fail("你现在无法使用「连环八式」进行攻击。\n");

	if( (int)me->query_skill("yijin-jing", 1) < 300 )
	return notify_fail("你的易筋经等级还不够，使不出「连环八式」绝技。\n");

	if( me->query("max_neili") < 3000 )
	return notify_fail("你现在内力太弱，使不出「连环八式」。\n");

	if( (int)me->query("neili") < 1500 )
	return notify_fail("你现在真气太弱，使不出「连环八式」。\n");

	lv = me->query_skill("longzhua-gong",1);
	hit = 8;

	me->add_temp("apply/pfm_attack", 180+lv/10);
	me->add_temp("apply/damage", 500+lv/2);
	me->add_temp("apply/pfm_damage", 180+lv/5);

	lzg_msg = RED"\n$N突然间大喝一声，纵身而上，双手犹如狂风骤雨。 \n";

	lzg_msg += "「捕风式」「捉影式」";
	lzg_msg += "「抚琴式」「鼓瑟式」";
	lzg_msg += "「批亢式」「捣虚式」";
	lzg_msg += "「抱残式」「守缺式」";

	lzg_msg += "八式连环，疾攻而至！\n"NOR;

	message_vision(lzg_msg, me, target);


	for( i=0; i < hit; i++ ){
		for(j=0;j<4;j++)
		if(!target || target->query("qi") < 0) target = offensive_target(me);
		if (target && me->is_fighting(target) && target->query("qi")>0)
		COMBAT_D->do_attack(me, target);
	}


	message_vision(HIY"\n这"+chinese_number(hit)+"式连环的龙爪手绵绵不绝，便如是一招中的"+chinese_number(hit)+"个变化一般，快捷无比！\n"NOR, me);
	me->add("neili", -hit*50);
	me->add("jingli", -hit*10);
	me->set_temp("lzg_lianhuan",1);
	hit = hit/2;
	me->start_perform(hit,"「连环八式」");
	me->start_busy(random(2));
	call_out("remove_effect", 14,me, target,lv);
	return 1;
}

void remove_effect(object me, object target, int lv)
{
	if (me) {
	  me->add_temp("apply/pfm_attack", -180-lv/10);
	  me->add_temp("apply/damage", -500-lv/2);
	  me->add_temp("apply/pfm_damage", -180-lv/5);
		me->delete_temp("lzg_lianhuan");
	}

	if (target && me && me->is_fighting(target))
	message_vision(HIW"$N气血翻腾，攻势渐渐慢了下来。\n"NOR,me,target);

}

int help(object me)
{
        write(RED"\n「连环八式」："NOR"\n");
        write(@HELP
        
        龙爪功为少林七十二绝技之一，乃是练习擒拿所需指力的重要功夫，其又称擒拿手。
        为软硬相兼之功，刚柔并济之劲，阴阳相合之力。练至能气随意注，力随指行方为
        大成。此「连环八式」一经使出，犹如狂风骤雨，连环抓出，凌厉狠辣，威猛无比。
        此式虽有违佛家宗旨，不过颇受武僧喜爱，修炼者甚多。

        要求：  最大内力 3000 以上；      
                当前内力 1500  以上；  
                龙爪功等级 300 以上；
                易筋经等级 300 以上；
                激发爪法为龙爪功；
                激发招架为龙爪功；
                且手无兵器；

                
HELP
        );
        return 1;
}


