#include <ansi.h>
inherit F_SSERVER;

int query_ap() {return 3;}

int perform(object me, object target)
{
	string type,msg;
	object weapon;
	int i,lv,flag;

	if( !target ) target = offensive_target(me);

	if( !target
	||  !me->is_fighting(target) )
	return notify_fail("「破」只能对战斗中的对手使用。\n");

	if (!objectp(weapon = me->query_temp("weapon"))
	|| (string)weapon->query("skill_type") != "sword")
	return notify_fail("你使用的武器不对。\n");
	
	weapon = target->query_temp("weapon");	
	
	type = me->query("env/po");
	
	if(type && weapon ) {
		if ((type == "刀" && weapon->query("skill_type") == "blade")
		|| (type == "剑" && weapon->query("skill_type") == "sword")
		|| (type == "索" && weapon->query("skill_type") == "whip")
		|| (type == "箭" && weapon->query("skill_type") == "throwing")
		|| ((type == "枪") &&
				( weapon->query("skill_type") == "spear"
				|| weapon->query("skill_type") == "hook"
				|| weapon->query("skill_type") == "stick"
				|| weapon->query("skill_type") == "staff"
				|| weapon->query("skill_type") == "club")
			 )
		|| ((type == "鞭") &&
				( weapon->query("skill_type") == "dagger"				
				|| weapon->query("skill_type") == "hammer"
				|| weapon->query("skill_type") == "brush"
				|| weapon->query("skill_type") == "axe")
			  )
		)
		  flag = 1;
  }
  else if (!weapon && type == "掌")
		flag =1;
		
	if((int)me->query_skill("dugu-jiujian",1) < 150 )
	return notify_fail("你的独孤九剑还不够火候。\n");

	if((string)me->query_skill_mapped("sword") != "dugu-jiujian" )
	return notify_fail("你现在无法使用「破」！\n");


	if((int)me->query("neili")< 1000)
	return notify_fail("你的精力不够！\n");
	
	msg = HIW"\n$N剑上完全不出内力，剑随意行，毫无章法！\n"NOR;
	lv = me->query_skill("dugu-jiujian",1);

	me->add("neili", -500 -lv/2); 
	weapon = me->query_temp("weapon");
	if (!type || type=="气") {
		if( COMBAT_D->do_attack(me, target, weapon, 0, 1) 
		|| COMBAT_D->do_attack(me, target, weapon, 0, 1)
		|| COMBAT_D->do_attack(me, target, weapon, 0, 1) ){
			msg += HIW"$N运起「破气式」, $n顿时觉得眼前一花，气海穴上微微一痛，全身真气狂泄而出！\n" NOR;
			target->apply_condition("no_perform", 1+ random(2));
			target->set("neili", target->query("neili")/4);
			target->add_busy(2);
		}
		else
		{
			msg += "可是$n看破了$N的企图，立刻守紧门户，几经艰难终于还招挡开。\n"NOR;
		}
		message_vision(msg, me, target);
	
		return 1;
	}
	
	
	if(flag) {		
		msg+= HIW"\n$n只觉得$N的「破"+type+"式」将自己的武功克制的毫无还手之余！\n"NOR;
		message_vision(msg,me,target);				
		me->add_temp("apply/damage",500+lv/2);
	  me->add_temp("apply/pfm_damage",200+lv/5);
	  me->add_temp("apply/pfm_attack",60+lv/10);
	  if (!target->query_temp("apply/ddgj_po")) {
		  target->set_temp("apply/ddgj_po",1);
		  target->add_temp("apply/pfm_attack",-120);
		  target->add_temp("apply/pfm_damage",-80);
		  target->add_temp("apply/pfm_parry",-120);
		  target->add_temp("apply/pfm_dodge",-120);
		  call_out("check_fight", 1, me,target, 20);
	  }
	  me->add_temp("apply/action/color",HIW);		
		
		for(i = 0; i < 3; i++)
	    if (objectp(target) && me->is_fighting(target))
	      COMBAT_D->do_attack(me, target, weapon, random(3)?1:3);
	      
	  me->add_temp("apply/damage",-500-lv/2);
	  me->add_temp("apply/pfm_damage",-200-lv/5);
	  me->add_temp("apply/pfm_attack",-60-lv/10);
	  me->delete_temp("apply/action/color");		
	} 
	else {
		msg+= HIG"\n但是$N剑法一滞，无法连贯，$n的压力登时减轻了。\n"NOR;
		message_vision(msg,me,target);
		for(i = 0; i < 3; i++)
	    if (objectp(target) && me->is_fighting(target))
	      COMBAT_D->do_attack(me, target, weapon, 1);	  
  }
	return 1;
}

void remove_effect(object target);

void check_fight(object me, object target, int count){
	if (!target) return;
	if( !me 
	|| !me->query_temp("weapon")
	|| me->query_skill_prepared("sword") != "dugu-jiujian"
  || me->query_skill_mapped("sword") != "dugu-jiujian"
  || me->query_skill_mapped("sword") != "dugu-jiujian"
	|| !me->is_fighting()
	|| count < 1 ){		
			remove_effect(target);
		return;
	}
	call_out("check_fight", 1, me, count-1);
}

void remove_effect(object target)
{
	if (objectp(target)) 
	{
	  target->delete_temp("apply/ddgj_po");
	  target->add_temp("apply/pfm_attack",120);
	  target->add_temp("apply/pfm_damage",80);
	  target->add_temp("apply/pfm_parry",120);
	  target->add_temp("apply/pfm_dodge",120);
	}
}

string perform_name(){ return HIW"破"NOR; }

int help(object me)
{
	write(HIY"\n独孤九剑之「破」："NOR"\n\n");
	write(@HELP	
	破剑式
	    破解普天下各门各派的剑法。
	破刀式
	　　破解单刀、双刀、柳叶刀、鬼头刀、大砍刀、斩马刀种种刀法。讲究以轻御重，以快制慢。
	破枪式
	　　包括破解长枪，大戟、蛇矛、齐眉棍、狼牙棒、白蜡杆、禅杖、方便铲种种长兵刃之法。
	破鞭式
	　　破解解钢鞭、点穴橛、拐子、峨眉刺、匕首、斧、铁牌、八角槌、铁椎等等短兵刃
	破索式
	　　破解长索、短鞭、三节棍、炼子枪、铁链、渔网、飞锤流星等等软兵刃。
	破掌式
	　　破解拳脚指掌上功夫，长拳短打、擒拿点穴、鹰爪虎爪、铁沙神掌诸般拳脚功夫。
	破箭式
	　　破解诸般暗器，须得先学听风辨器之术。
	破气式
	　　对付身具上乘内功的敌人而用，神而明之，存乎一心。
	
	要求：	
		当前内力 1000 以上；
		独孤九剑等级 150 以上；
		激发剑法为独孤九剑；
		激发招架为独孤九剑；
		且手中持剑。

	注：使用命令　set po 气｜剑｜刀｜枪｜鞭｜索｜掌｜箭
	    可选择不同招式针对不同的敌人   
HELP
	);
	return 1;
}


