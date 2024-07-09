// tisha.c 踢沙
// Update By lsxk@hsbbs 2007/6/11 

#include <ansi.h>
inherit F_SSERVER;

int query_ap() {return 3;}

int perform(object me, object target)
{
	object weapon;
	int lv, flag;


	if( !target ) target = offensive_target(me);

	if( !objectp(target)
	|| !target->is_character()
	|| !me->is_fighting(target))
	return notify_fail("「圣火绝学」只能对战斗中的对手使用。\n");

	if( (lv = me->query_skill("shenghuo-lingfa", 1)) < 200 )
	return notify_fail("你的圣火令法功力太浅，不能使用。\n");

	if( me->query_skill("shenghuo-lingfa", 1) < 200 )
	return notify_fail("你的七伤拳法功力太浅，不能使用。\n");
	
	if(!me->query_skill("jiuyang-shengong", 1)||(int)me->query_skill("jiuyang-shengong",1)<200)
	if(me->query_skill("shenghuo-shengong", 1) < 200 )
	return notify_fail("你的特殊内功功力太浅，不能使用。\n");

	if(me->query_skill("dagger", 1) < 200 )
	return notify_fail("你的基本匕法功力太浅，不能使用。\n");

	if(me->query_skill("cuff", 1) < 200)
	return notify_fail("你的基本拳法功力太浅，不能使用。\n");

	if(me->query_skill("qiankun-danuoyi", 1) < 200 )
	return notify_fail("你的乾坤大挪移功力太浅，不能使用。\n");

	if(me->query("max_neili") < 3000 )
	return notify_fail("你的内力太浅，不能使用。\n");

	if(me->query("neili") < 1000 )
	return notify_fail("你的真气不够，不能使用。\n");

	if(me->query("jingli") < 1000)
	return notify_fail("你的精力不够，不能使用。\n");

	if( me->query_temp("qkdny/yin") )
	return notify_fail("你正在施展乾坤大挪移「引」字诀呢！\n");

	if( me->query_skill_mapped("parry") != "shenghuo-lingfa"
	&& me->query_skill_mapped("parry") != "qiankun-danuoyi" )
	return notify_fail("你需要激发招架为圣火令法，方能使用此绝技。\n");

	if (!objectp(weapon = me->query_temp("weapon"))
	|| weapon->query("skill_type") != "dagger"
	|| me->query_skill_prepared("cuff") != "qishang-quan"
	|| me->query_skill_mapped("dagger") != "shenghuo-lingfa"
	|| me->query_skill_mapped("cuff") != "qishang-quan"
	|| (me->query_skill_mapped("force") != "shenghuo-shengong" && me->query_skill_mapped("force") != "jiuyang-shengong") )
	return notify_fail("你不能使用这个绝招！\n");

	if((int)me->query_temp("ling")
	&& ((int)me->query_skill("jiuyang-shengong",1)<450 || me->query_skill_prepared("force") != "jiuyang-shengong"))
	return notify_fail("你正在使用「心魔」中，无暇使用「踢沙」绝技。\n");

	if ( !me->query_skill("jiuyang-shengong", 1)||(!userp(me) && userp(target)))
	  flag = 3;
	else
	  flag = 4;


	message_vision(YEL"\n$N突然横腿疾扫，卷起地下大片沙石，身子一分为二，前后夹击、连攻四招！\n" NOR, me);

	me->set_temp("tisha", 1);

	me->add("neili", -500 -lv/2);
	me->add_temp("apply/pfm_attack", 90+lv/10);
	me->add_temp("apply/damage", 350+lv/2);
	me->add_temp("apply/pfm_damage", 150+lv/5);
  me->set_temp("apply/action/skill","shenghuo-lingfa");
	me->set_temp("apply/action/pfm",CYN"$N直抢而前，脚下一踬，忽然一个筋斗摔了过去，双手已向$n胸口拍了过去");
	me->set_temp("apply/action/damage_type","瘀伤");
	COMBAT_D->do_attack(me, target);


	if (present(target,environment(me)) && me->is_fighting(target)) {
		me->set_temp("apply/action/damage_type","刺伤");
		me->set_temp("apply/action/pfm",CYN"$N坐倒在地，双拳猛捶自己胸膛，待$n靠近突然间脚踢飞沙，抽出$w"CYN"向$n的$l疾刺");
		COMBAT_D->do_attack(me, target, me->query_temp("weapon"), 1);
	}


	if (present(target,environment(me)) && me->is_fighting(target)) {
		me->set_temp("apply/action/damage_type","瘀伤");
		me->set_temp("apply/action/pfm",CYN"$n在霎时之间，但觉飞雪扑面，双眼不能见物，茫然不知所措，$N趁势用"YEL"肩部"CYN"从背后向$n撞去");
		COMBAT_D->do_attack(me, target, 0, (flag==4)?3:0);
	}

	if (present(target,environment(me)) && me->is_fighting(target) && flag >=4 ) {
		me->set_temp("apply/action/damage_type","刺伤");
		me->set_temp("apply/action/pfm",CYN"$N随着$n的去式，巧运"HIR"乾坤大挪移神功"CYN"，钻到$n的身前，将手中$w"CYN"向$n的$l插去");
		COMBAT_D->do_attack(me, target, me->query_temp("weapon"), (flag==4)?3:1);
	}
	
	me->add_temp("apply/pfm_attack", -90-lv/10);
	me->add_temp("apply/damage", -350-lv/2);
	me->add_temp("apply/pfm_damage", -150-lv/5);
	me->delete_temp("apply/action");
	
	me->delete_temp("tisha", 1);
	me->start_busy(random(2));
	return 1;
}

string perform_name(){ return YEL"踢沙"NOR; }

int help(object me)
{
	write(YEL"\n圣火令法之「踢沙」："NOR"\n\n");
	write(@HELP
	要求：	 当前内力 1000 以上；
           最大内力 3000 以上；
           当前精力 1000 以上；
           圣火令法等级 200 以上；
           乾坤大挪移等级 200 以上：
           圣火神功等级 200 以上 或者 明教九阳功等级 200 以上；
           基本令法等级 200 以上；
           基本拳法等级 200 以上；
           激发令法为圣火令法；
           激发拳法为圣火令法；
           激发招架为圣火令法；
           备拳法为圣火令法；
           且手持兵器。

HELP
        );
        return 1;
}
