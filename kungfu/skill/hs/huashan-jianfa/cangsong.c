// cangsong.c 苍松迎客 Created by LSXK@hsbbs 2007/7/25
// 在《笑傲江湖》里，此招为令狐冲绝技，因此设定Quest,需要向令狐冲请教后，增加威力。
// 降低解quest前的命中率，解完quest后，才恢复教好的命中。  By lsxk@hsbbs 2007/7/31
/* 他侧头再看第二组图形时，见使剑的所使是本门一招‘苍松迎
   客’，登时精神一振，这一招他当年足足花了一个月时光才练
   得纯熟，已成为他临敌时的绝招之一。
*/

#include <ansi.h>
inherit F_SSERVER;

int query_ap() {return 5;}
string perform_name() {return HIG"苍松迎客"NOR;}
int perform(object me, object target)
{
	object weapon;
	int lvl;

	if( !target ) target = offensive_target(me);

	if( !objectp(target)
	||  !target->is_character()
	||  !me->is_fighting(target)
	||  !living(target))
	return notify_fail("「苍松迎客」只能对战斗中的对手使用。\n");

	if( (lvl = (int)me->query_skill("huashan-jianfa", 1)) < 150 )
	return notify_fail("你的华山剑法功力太浅，使不出华山剑法之「苍松迎客」的功夫!\n");

	if( (int)me->query_skill("sword", 1) < 150 )
	return notify_fail("你的基本剑法太差,使不出「苍松迎客」的功夫!\n");

	if( (int)me->query_skill("huashan-qigong", 1) < 150
	&&(int)me->query_skill("zixia-gong", 1) < 150)
	return notify_fail("你的特殊内功太差,使不出「苍松迎客」的功夫!\n");

	if(me->query_skill_mapped("force") != "huashan-qigong"
	&&me->query_skill_mapped("force") != "zixia-gong" )
	return notify_fail("你的特殊内功不对，无法使用「苍松迎客」！\n");

	if( (int)me->query("max_neili") < 150 )
	return notify_fail("你的内力修为太浅，使不出「苍松迎客」。\n");

	if( (int)me->query("neili") < 1000 )
	return notify_fail("你的真气不够，使不出「苍松迎客」功夫。\n");

	if (!objectp(weapon = me->query_temp("weapon")) || weapon->query("skill_type") != "sword"
	|| me->query_skill_mapped("sword") != "huashan-jianfa"
	|| me->query_skill_mapped("parry") != "huashan-jianfa")
	return notify_fail("你不使用华山剑法,如何使得了「苍松迎客」?\n");

	me->set_temp("hsjf/cangsong", 1);
	message_vision(HIG"\n$N手中的"+weapon->query("name")+HIG"突然一晃，满场游走，一身青衫化成了一片青影，一招「"+GRN+"苍"+
	"松迎客"+HIG+"」直刺$n。\n"NOR,me,target);
	me->receive_damage("neili", 500+lvl);
	me->add_temp("apply/pfm_attack",150+lvl/10);
		
	me->add_temp("apply/pfm_hurt",200+lvl/2);
	me->set_temp("apply/action/pfm", HIG"\n只见$N将手中"+weapon->query("name")+HIB+"缓缓刺出，剑身竟丝毫不晃动，可见这招内力运用已入化境"NOR);
	COMBAT_D->do_attack(me, target,weapon,random(3)?0:3,2);
	me->add_temp("apply/pfm_hurt",-200-lvl/2);

	me->set_temp("apply/action/pfm", HIG"\n$N竟不给$n有丝毫喘息机会，紧跟着"+weapon->query("name")+HIB"再一次挥出，剑出如龙吟，竟携带无比凌厉的剑气刺向$n"NOR);
	me->add_temp("apply/damage",600+lvl);
	me->add_temp("apply/pfm_damage",200+lvl/5);
	COMBAT_D->do_attack(me, target,weapon,random(2)?0:3);
	me->add_temp("apply/damage",-600-lvl);
	me->add_temp("apply/pfm_damage",-200-lvl/5);
	
	me->add_temp("apply/pfm_attack",-150-lvl/10);	
	me->delete_temp("apply/action/");
	me->delete_temp("hsjf/cangsong");

	me->start_perform(4, "「苍松迎客」");
	return 1;
}

int help(object me)
{
   write(WHT"\n华山剑法「"HIG"苍松迎客"WHT"」："NOR"\n");
   write(@HELP

    要求：  当前内力 1000 以上；
            最大内力 1500 以上； 
            华山剑法 150 以上；
            基本剑法 150 以上；
            紫霞神功 150 以上。

HELP
   );
   return 1;
}
