// sanhua-zhang perform 四季散花(春夏秋冬)
// by beyond
// rewrite by snowman@SJ 05/12/1999
//felchion 2008/3/6
inherit F_SSERVER;

#include <ansi.h>

int query_ap() {return 5;}

string perform_name() { return HIG"四季散花"NOR; }

int perform(object me, object target)
{
	int lvl,i;
	string msg;
	
	if( !target ) target = offensive_target(me);

	if( !target || !me->is_fighting(target) )
	return notify_fail("「四季散花」只能在战斗中对对手使用。\n");

	if(me->query_temp("weapon"))
	return notify_fail("你必须空手使用「四季散花」！\n");

	if( (lvl = (int)me->query_skill("sanhua-zhang",1)) < 180 )
	return notify_fail("你的散花掌不够娴熟，不能使用「四季散花」！\n");

	if( (int)me->query_skill("yijin-jing",1) < 180 )
	return notify_fail("你的易筋经内功等级不够，不能使用「四季散花」！\n");

	if( (int)me->query("max_neili") < 2000 )
	return notify_fail("你的内力太弱，不能使用「四季散花」！\n");

	if( (int)me->query("neili") < 1000 )
	return notify_fail("你的内力太少了，无法使用出「四季散花」！\n");

	if( me->query_temp("fumo"))
	return notify_fail("你正在使用大金刚拳的特殊攻击「金刚伏魔」！\n");

	if (me->query_skill_prepared("strike") != "sanhua-zhang"
	|| me->query_skill_mapped("strike") != "sanhua-zhang")
	return notify_fail("你现在无法使用「四季散花」进行攻击。\n");

	if (me->query_skill_mapped("force") != "yijin-jing")
	return notify_fail("你目前的内功无法使用「四季散花」进行攻击。\n");

	i = random(3);
	switch (i) {
		case 3: {
			msg = HIW"\n突然$N大吼一声，顿时把$n瞎了一个激灵，之间$N在眉宇之间布满了一层薄霜，双目冷冷的瞪着$n，接着僧袍一卷，一朵朵绽开的"HIM"「寒梅」"HIW"向$n满面扑来";
			break;
		}
		case 2: {
			msg = HIC"\n突然$N双手莫念经文，$n忽然觉得眼前人影如山，忽而排山倒海，忽而忽而高山流水，觉得一切都在离$n远去，跟着一股醉人的"HIY"「雏菊」"HIC"清香扑鼻而来";
			break;
		}
		case 1: {
			msg = MAG"\n只听$N“哎呀”一声，身行象前跌去，突然一道蓝光闪现，破空之声中几朵"WHT"「幽兰」"MAG"向$n的胸前飞去";
			break;
		}
		case 0:
		msg = CYN"\n$N突然面带微笑，手掌微张，掌中涌出一股劲风，恍忽是一片片嫩绿的"HIG"「芳草」"CYN"，直向$n的面颊飞去";
	}

	me->set_temp("apply/action/pfm", msg);
	me->receive_damage("neili", 500+lvl);
	me->add_temp("apply/damage",200+lvl);
	me->add_temp("apply/pfm_damage",200+lvl/5);
	me->add_temp("apply/pfm_attack",180+lvl/10);
	if (COMBAT_D->do_attack(me, target,0,random(2)?0:3)
	&& target
	&& me->is_fighting(target)
	&& target->query("qi")>0 ) {
		switch (i) {
			case 3: {
				target->add_condition("no_perform", 1+random(2));
				break;
			}
			case 2: {
				target->add_condition("no_exert", 2);
				break;
			}
			case 1: {
				target->add("neili", target->query("neili")/2);
				break;
			}
			case 0:
			target->add_busy(3);
		}
	}
	me->add_temp("apply/damage",-200-lvl);
	me->add_temp("apply/pfm_damage",-200-lvl/5);
	me->add_temp("apply/pfm_attack",-180-lvl/10);	      
	me->delete_temp("apply/action/");
	me->start_busy(random(2));
	return 1;
}

int help(object me)
{
        write(HIY"\n「四季散花」："NOR"\n");
        write(@HELP
        
        四季散花曼妙无比，以浑厚的内力携掌风伤敌，并造成对
        方状态异常或消耗大量内力。

        要求：  最大内力 2000 以上；
                当前内力 1000 以上；
                散花掌等级 180 以上；
                易筋经等级 180 以上；
                激发散花掌为掌法；
                
HELP
        );
        return 1;
}