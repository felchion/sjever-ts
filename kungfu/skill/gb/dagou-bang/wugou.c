//By lsxk@hsbbs 2007/7/3

#include <ansi.h>

inherit F_SSERVER;

int query_ap() {return 10;}

string get_msg(int i) {
	string msg;
	switch(i) {
		case 5: msg = HIR"$N「天下无狗」第六变，披天盖地般打来。只见$w"HIR"招招不离$n的脑门"; break;
		case 4: msg = HIB"只见慢天棒影一停，$N将「天下无狗」第五变使出，脚在$w"HIB"一端一踢，$w"HIB"突然飞向$n的小腹"; break;
		case 3: msg = HIG"$N使出「天下无狗」第四变，将$w"HIG"抡圆了，突然直捅$n的眼睛"; break;
		case 2: msg = HIW"$N闪身后跳，使出「天下无狗」第三变，手中$w"HIW"猛扫向$n的脚踝"; break;
		case 1: msg = CYN"忽然$N急走数步，回身$w"CYN"斜劈，使出「天下无狗」第二变，戳向$n的肩头";break;
		case 0: msg = HIC"$N棒走偏峰，将$w"HIC"舞成满天棒影，竟然使的是「天下无狗」第一变"; break;
		default: msg = HIC"$N棒走偏峰，将$w舞成满天棒影，竟然使的是「天下无狗」第一变"; break;
	}
	return msg;
}

string perform_name(){ return HIW"天下无狗"NOR; }

int perform(object me,object target)
{
	object weapon;
	int lvl,i,j;

	if( !target ) target = offensive_target(me);

	if( !objectp (target) || !me->is_fighting(target) || !living(target)
	|| environment(target)!=environment(me))
		return notify_fail("天下无狗只能对战斗中的对手使用。\n");

	if(me->query_skill_mapped("force") != "huntian-qigong")
		return notify_fail("你的内功不是混天气功，无法使用天下无狗！\n");
	if( (int)me->query_skill("huntian-qigong", 1) < 250 )
		return notify_fail("你的内功还未练成，不能使用天下无狗！\n");
	if( (int)me->query_skill("dagou-bang", 1) < 250 )
		return notify_fail("你的打狗棒法还未练成，不能使用天下无狗！\n");
	if((int)me->query_skill("stick", 1) < 250 )
		return notify_fail("你的基本棒法不够娴熟，不能在使用天下无狗。\n");
	if((int)me->query_skill("bangjue", 1) < 180 )
		return notify_fail("你的打狗棒诀不够娴熟，不能在使用天下无狗。\n");
	if(!me->query("dgb/wugou"))
		return notify_fail("你只听说过天下无狗这一招，却不知道怎么使用。\n");
	if((int)me->query_con(1) < 45 )
		return notify_fail("你的身体不够强壮，不能使用天下无狗。\n");
	if (!objectp(weapon = me->query_temp("weapon")) || weapon->query("skill_type") != "stick"
	|| me->query_skill_mapped("stick") != "dagou-bang"
	|| me->query_skill_mapped("parry") != "dagou-bang")
		return notify_fail("你现在使得了天下无狗么？\n");

	if((int)me->query("max_neili") < 4500 )
		return notify_fail("你的内力修为不够，不能使用天下无狗！\n");
	if((int)me->query("neili") < 2000 )
		return notify_fail("你现在真气不足，不能使用天下无狗！\n");

	message_vision(HIW"\n$N竹棒抖动，将一路“打狗棒法”使得变化万方，但见棒去如神龙夭矫，棒来又似灵蛇盘舞，奥妙招式,信手拈来！\n",me);
	message_vision(HIG"这正是打狗棒法的绝招「天下无狗」,据说共六种变化，一招使出,劲力所至,四面八方全是棒影，\n",me);
	message_vision(HIG"此时纵是有几十条恶犬也可一并打死了，所谓“天下无狗”便是此意，棒法之精妙绝伦，已臻武学中的绝诣!\n"NOR,me);


	lvl = (int)me->query_skill("dagou-bang",1);
	me->add("neili", -500-lvl);

	me->add_temp("apply/damage",600+lvl);
	me->add_temp("apply/pfm_damage",300+lvl/3);
	me->add_temp("apply/pfm_attack",240+lvl/10);
	
	for( i=0; i < 6; i++ ){
		for(j=0;j<4;j++)
		if(!target || target->query("qi") < 0) target = offensive_target(me);
		if (target && me->is_fighting(target) && target->query("qi")>0)
		me->set_temp("apply/action/pfm",get_msg(i));
		COMBAT_D->do_attack(me, target,weapon,1);
	}

	me->add_temp("apply/damage",-600-lvl);
	me->add_temp("apply/pfm_damage",-300-lvl/3);
	me->add_temp("apply/pfm_attack",-240-lvl/10);
	me->delete_temp("apply/action");
		
	me->start_busy(1);

	return 1;
}

int help(object me)
{
   write(HIW"\n打狗棒「天下无狗」："NOR"\n");
   write(@HELP
   此招共有六变，为打狗棒最终极的绝招，虽江湖上盛传丐帮
   弟子中，年轻有为者，已然学会该绝技，但只仅仅是招式而
   已，若能有幸得丐帮帮主洪老爷子亲口传授该绝技，领会该
   「天下无狗」之妙处，则使将出来乃威力无穷！

   要求：  最大内力 4500 以上；
           内力 2000 以上；
           打狗棒等级 250 以上；
           混天气功等级 250 以上；
           后天根骨 45 以上；
           打狗棒诀等级 180 以上；
           激发招架为打狗棒；
           解QUEST以后威力无穷。
HELP
   );
   return 1;
}
