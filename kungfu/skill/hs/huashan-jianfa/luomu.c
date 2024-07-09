// luomu.c 无边落木 Create By lsxk@hsbbs 2007/7/25
/* 记得去年腊月，师父见大雪飞舞，兴致甚高，聚集了一众弟子讲
   论剑法，最后施展了这招“无边落木”出来，但见他一剑快似一
   剑，每一剑都闪中了半空中飘下来的一朵雪花，连师娘都鼓掌喝
   彩，说道：“师哥，这一招我可服你了，华山派确该由你做掌门
   人。”
*/

#include <ansi.h>

inherit F_SSERVER;

int query_ap() {return 7;}

string perform_name() {return HIC"无边落木"NOR;}

int perform(object me,object target)
{
	object weapon;
	int i,lvl;

	if( !target ) target = offensive_target(me);

	if(!objectp(target) || !me->is_fighting(target) || !living(target)
	|| environment(target)!=environment(me))
	return notify_fail("「无边落木」只能对战斗中的对手使用。\n");

	if(me->query_skill_mapped("force") != "zixia-gong"
	&&me->query_skill_mapped("force") != "huashan-qigong")
	return notify_fail("你的内功有误，无法使用「无边落木」！\n");

	if( (int)me->query_skill("zixia-gong", 1) < 350
	&&(int)me->query_skill("huashan-qigong", 1) < 350 )
	return notify_fail("你的内功还未练成，不能使用「无边落木」！\n");

	if((int)me->query_skill("force", 1) < 350 )
	return notify_fail("你的基本内功不够娴熟，无法支持「无边落木」。\n");

	if( (lvl = (int)me->query_skill("huashan-jianfa", 1)) < 350 )
	return notify_fail("你的剑法还未练成，不能使用「无边落木」！\n");

	if( (int)me->query_skill("huashan-shenfa", 1) < 350 )
	return notify_fail("你的华山身法还未练成，不能使用「无边落木」！\n");

	if((int)me->query_skill("sword", 1) < 350 )
	return notify_fail("你的基本剑法不够娴熟，不能在剑招中使用「无边落木」。\n");

	if((int)me->query_skill("dodge", 1) < 350 )
	return notify_fail("你的基本轻功不够娴熟，不能使用「无边落木」。\n");

	if (!objectp(weapon = me->query_temp("weapon"))
	|| weapon->query("skill_type") != "sword"
	|| me->query_skill_mapped("sword") != "huashan-jianfa"
	|| me->query_skill_mapped("parry") != "huashan-jianfa")
	return notify_fail("你手里无剑，如何使用「无边落木」？\n");

	if((int)me->query("max_neili") < 8000 )
	return notify_fail("你的内力修为不够，不能使用「无边落木」！\n");

	if((int)me->query("neili") < 2000 )
	return notify_fail("你现在真气不足，不能使用「无边落木」！\n");

	if((int)me->query("jingli") < 1000 )
	return notify_fail("你现在太累了，不能使用「无边落木」！\n");

	message_vision(HIC"\n$N长啸一声，竟使出「无边落木」绝技！但见$N一剑快似一剑，每一剑都在半空中激起一道剑气，凌厉致极！\n"NOR, me,target);

	me->add("neili", -1500);
	me->add("jingli",-500);
	me->set_temp("hsjf/luomu",1);

	me->add_temp("apply/damage", 500+lvl/2);
  me->add_temp("apply/pfm_damage", 200+lvl/5);
  me->add_temp("apply/pfm_attack", 150+lvl/10);

	for(i = 0; i < 5; i++)
    if (objectp(target) && me->is_fighting(target))
      COMBAT_D->do_attack(me, target, weapon, random(3)?0:3);

	me->add_temp("apply/damage", -500-lvl/2);
  me->add_temp("apply/pfm_damage", -200-lvl/5);
  me->add_temp("apply/pfm_attack", -150-lvl/10);
  
	me->delete_temp("hsjf/luomu");
	me->start_busy(1);

	return 1;
}

int help(object me)
{
   write(WHT"\n华山剑法「"HIC"无边落木"WHT"」："NOR"\n");
   write(@HELP
    五岳剑派乃各有所长，华山剑法更以轻巧、灵活、多变而著称。这招
    「无边落木」乃是华山剑法绝诣，非精通华山剑法者无法使用，该招
    一旦使出，剑招乃一剑快似一剑，瞬间向对手击出数招威力巨大的剑
    招！若能有紫霞神功配合，则无论命中还是威力，均有难以想像的提
    高！
    指令：perform luomu

    要求：华山气功 或者 紫霞神功350级
          华山剑法350级，
          华山身法350级，
          基本内功350级，
          基本剑法350级，
          基本轻功350级，
          当前内力2000以上，
          当前精力1000以上，
          最大内力8000以上。

HELP
   );
   return 1;
}

