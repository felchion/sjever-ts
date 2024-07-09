// guifei 贵妃回眸
// 重新写了哈哈---by daidai
/*
     洪夫人身子微曲，纤腰轻扭，右足反踢，向教主小腹踹去。教主后缩相避，
洪夫人顺势反过身来，左手搂住教主的头颈，右手竟已握住了匕首，剑尖对准了
教主后心，笑道：“这是第一招，叫做‘贵妃回眸’，你记住了。”

*/
#include <ansi.h>
#include <combat.h>
inherit F_SSERVER;
#include <combat_msg.h>

int query_ap() {return 5;}
string perform_name(){ return HIM"贵妃回眸"NOR; }

int perform2(object me, object target);

int perform(object me, object target)
{
	object weapon;
	int lvl;

	if(!target) target = offensive_target(me);

	if(!target
	|| !target->is_character()
	|| !me->is_fighting(target) )
	return notify_fail("「贵妃回眸」只能在战斗时使用！\n");

	if(userp(me) && me->query("family/master_id")!="hong antong")
	return notify_fail("「贵妃回眸」只有拜了教主后才能用！\n");

	if(!objectp(weapon=me->query_temp("weapon"))
	|| weapon->query("skill_type") != "dagger")
	return notify_fail("你手上没有匕首，如何使用「贵妃回眸」。\n");

	if((int)(me->query("dex")+me->query_skill("dodge",1)/10) < 30 )
	return notify_fail("你身法不够，使用「贵妃回眸」，恐怕会伤到自己！\n");

	if((lvl = (int)me->query_skill("tenglong-bifa", 1)) < 150 )
	return notify_fail("你的腾龙匕法还不够熟练，不能使用「贵妃回眸」！\n");
	if((int)me->query_skill("shenlong-tuifa",1) < 150)
	return notify_fail("你的神龙腿法不够熟练，不能使用「贵妃回眸」！\n");

	if(me->query_skill_mapped("dagger") != "tenglong-bifa")
	return notify_fail("只有使用腾龙匕法时才能使用「贵妃回眸」！\n");

	if(me->query_skill_mapped("leg") != "shenlong-tuifa"
	|| me->query_skill_prepared("leg") != "shenlong-tuifa")
	return notify_fail("你现在没有做好准备，无法使用「贵妃回眸」！\n");

	if( (int)me->query_skill("dulong-dafa", 1) < 150 )
	return notify_fail("你的毒龙大法功力不足，不能用「贵妃回眸」。\n");

	if((int)me->query("jingli", 1) < 800)
	return notify_fail("你现在精力不足，不能使用「贵妃回眸」！\n");

	if((int)me->query("neili", 1) < 1500)
	return notify_fail("你现在内力不足，不能使用「贵妃回眸」！\n");

	if( me->query_skill_mapped("force") != "dulong-dafa")
	return notify_fail("你现在所使用的内功同毒龙大法相互抵触，不能使用「贵妃回眸」。\n");

	message_vision( HIR"\n$N乘$n稍稍走神之机，拼尽全力使出苏夫人不传之绝技－－「贵妃回眸」！"NOR,me,target);

	me->receive_damage("neili",lvl/2+300);
	me->receive_damage("jingli",150);
	
	me->set_temp("apply/action/pfm", MAG"\n$N身子微曲，腰身轻扭，右足反踢，向$n小腹踹去"NOR);
  me->set_temp("apply/action/hit", HIY"\n$n闪避不及，被$N一脚踢中，面色立刻变的惨白！\n"NOR);       
	target->set_temp("apply/action/parry", HIW"$p连退数步，闪身避让！\n"NOR);
	target->set_temp("apply/action/dodge", HIW"$p连退数步，闪身避让！\n"NOR);		
	
	me->receive_damage("neili", 500+lvl);
	me->add_temp("apply/pfm_hurt",200+lvl/2);
	me->add_temp("apply/pfm_attack",120+lvl/10);
	weapon->unequip();
	COMBAT_D->do_attack(me, target,0,random(3)?0:3,2);
	weapon->wield();
	me->add_temp("apply/pfm_hurt",-200-lvl/2);
	me->add_temp("apply/pfm_attack",-120-lvl/10);	    
	
	perform2(me, target);
	me->start_busy(1);
	
	return 1;
}


int perform2(object me, object target)
{
	object weapon;
	int lvl;
	
	if(!target) target = offensive_target(me);

	if(!target
	|| !target->is_character()
	|| !me->is_fighting(target) )
		return 0;
	
	if((int)me->query("jingli", 1) < 200)
		return 0;
	
	if((int)me->query("neili", 1) < 1000)
		return 0;
	
	weapon=me->query_temp("weapon");
	
	if(!weapon) return 0;
	
	lvl = me->query_skill("tenglong-bifa",1);
	me->set_temp("apply/action/pfm", HIY"\n$N再顺势反过身来，咯咯轻笑数声，左手向着$n的头顶轻轻拂去"NOR);
  me->set_temp("apply/action/hit", MAG"$n被$N搂住头顶，只觉得心中一荡，神情迷糊之间却没发觉到$P右手竟已握住了"+weapon->name()+MAG"直刺后心！\n"NOR);       
	target->set_temp("apply/action/parry", HIC"$n虽然缩头躲过，却也心下大震，数招之间手忙脚乱，不成章法！\n"NOR);
	target->set_temp("apply/action/dodge", HIC"$n虽然缩头躲过，却也心下大震，数招之间手忙脚乱，不成章法！\n"NOR);	

	me->start_perform(3,"「贵妃回眸」");
	me->receive_damage("neili", 500+lvl/2);
	me->add_temp("apply/damage",600+lvl);
	me->add_temp("apply/pfm_damage",200+lvl/5);
	me->add_temp("apply/pfm_attack",150+lvl/10);
	COMBAT_D->do_attack(me, target,weapon,random(2)?0:3);
	me->add_temp("apply/damage",-600-lvl);
	me->add_temp("apply/pfm_damage",-200-lvl/5);
	me->add_temp("apply/pfm_attack",-150-lvl/10);	      
	me->delete_temp("apply/action/");
	target->delete_temp("apply/action/");
	return 1;
}

int help(object me){   
  write(WHT"\n腾龙匕法「"HIM"贵妃回眸"WHT"」："NOR"\n");
  write(@HELP

     要求：当前内力 1500 以上；
           当前精力 800 以上；
           腾龙匕法等级 150 以上；
           毒龙大法等级 150 以上；
           后天身法 30 以上。
HELP
    );
    return 1;
}

