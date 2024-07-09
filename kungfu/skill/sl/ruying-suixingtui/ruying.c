// ruying 如影随形
// by snowman@SJ 
// Modify by snowman@SJ 22/02/2000

#include <ansi.h>
#include <combat.h>

inherit F_SSERVER;
int query_ap() {return 3;}
string perform_name(){ return HIY"如影随形"NOR; }

int perform(object me, object target)
{
	mapping pre;
	string *skill, a, b;
	int i,lv;

	if( !target ) target = offensive_target(me);

	if( !target || !me->is_fighting(target) )
		return notify_fail("「如影随形」只能在战斗中对对手使用。\n");

	if( objectp(me->query_temp("weapon")) )
		return notify_fail("使用「如影随形」时双手必须空着！\n");
	if( me->query_temp("fumo"))
		 return notify_fail("你正在使用大金刚拳的特殊攻击「金刚伏魔」！\n");
	if( (int)me->query_skill("ruying-suixingtui", 1) < 120 )
		return notify_fail("你的如影随形腿不够娴熟，不会使用「如影随形」。\n");

	if( (int)me->query_skill("yijin-jing", 1) < 120 )
		return notify_fail("你的易筋经等级不够，不能使用「如影随形」。\n");

	if (me->query_skill_mapped("force") != "yijin-jing")
		return notify_fail("你目前的内功无法支持你使用「如影随形」进行攻击。\n");  

	if (me->query_skill_prepared("leg") != "ruying-suixingtui"
	|| me->query_skill_mapped("leg") != "ruying-suixingtui")
		return notify_fail("你现在无法使用「如影随形」进行攻击。\n");  
											       
	if( (int)me->query("max_neili") < 1800 )
		return notify_fail("你现在内力修为太弱，不能使用「如影随形」。\n");
	if( (int)me->query("neili") < 1000 )
		return notify_fail("你目前内力太少，不能使用「如影随形」。\n");

	if( (int)me->query("jingli") < 500 )
		return notify_fail("你现在精力太少，不能使用「如影随形」。\n");

	if( me->query_temp("sl_ruying") )
		return notify_fail("你刚使完「如影随形」，目前气血翻涌，无法再次运用！\n");

	tell_object(me,YEL"\n你猛吸一口真气，体内劲力瞬时爆发！\n" NOR,me, target);
	pre = me->query_skill_prepare();
	skill = keys(pre);     
	for (i=0; i<sizeof(skill); i++){
		if(skill[i]=="leg") continue; 
		a = skill[i];
		b = pre[skill[i]];
		me->prepare_skill(a);
	}

	lv = (int)me->query_skill("ruying-suixingtui", 1);	
	me->set_temp("sl_rysxt", 1);
	me->add("neili", -500 -lv/2);
	me->add_temp("apply/force",250+lv/2);
	me->add_temp("apply/pfm_force",120+lv/5);
	me->add_temp("apply/pfm_attack",120+lv/10);

	me->set_temp("apply/action/pfm",HIY"$N忽然跃起，左脚一勾一弹，霎时之间踢出一招「如」字诀的穿心腿，直袭$n前胸");
	COMBAT_D->do_attack(me, target,0, TYPE_RIPOSTE);
	if(me->is_fighting(target)){
		me->set_temp("apply/action/pfm",HIY"紧接着$N左腿勾回，将腰身一扭，那右腿的一招「影」字诀便紧随而至，飞向$n");
		COMBAT_D->do_attack(me, target, 0, TYPE_RIPOSTE);
	}
	if(me->is_fighting(target) && me->query_skill("ruying-suixingtui", 1) >= 150){
		me->set_temp("apply/action/pfm",HIY"只见$N右脚劲力未消，便凌空一转，左腿顺势扫出一招「随」字诀，如影而至");
		COMBAT_D->do_attack(me, target, 0, TYPE_RIPOSTE);
	}
	if(me->is_fighting(target) && me->query_skill("ruying-suixingtui", 1) >= 200){
		me->set_temp("apply/action/pfm",HIY"半空中$N脚未后撤，已经运起「形」字诀，内劲直透脚尖，在$n胸腹处连点了数十下");
		COMBAT_D->do_attack(me, target, 0, TYPE_RIPOSTE);
	}
	
	// 5th kick is type_super. cannot be done in normal fight.	
	if(me->is_fighting(target) && me->is_killing(target->query("id"))
	   && random(me->query_skill("ruying-suixingtui", 1)) >= 150) {
		me->set_temp("apply/action/pfm",HIR"这时$N双臂展动，带起一股强烈的旋风，双腿霎时齐并，「如影随形」一记重击直蹬在$n胸膛之上");
		COMBAT_D->do_attack(me, target, 0, TYPE_SUPER);
	}
	
	message_vision(YEL "\n$N连环飞腿使完，全身一转，稳稳落在地上。\n" NOR,me, target);
	me->prepare_skill(a,b);
	me->reset_action();
	me->delete_temp("sl_rysxt");
	me->add_temp("apply/force",-250-lv/2);
	me->add_temp("apply/pfm_force",-120-lv/5);
	me->add_temp("apply/pfm_attack",-120-lv/10);
	me->delete_temp("apply/action");
	me->start_busy(1);

	return 1;
}


int help(object me)
{
        write(HIY"\n「如影随形」："NOR"\n");
        write(@HELP
        
        连环飞腿如影随形，随等级提高攻击次数增加
        最高五次，威力也有相应提升。
        
        要求：  最大内力 1800 以上；
                当前内力 1000 以上；
                如影随形腿等级 120 以上；
                易筋经等级 120 以上；
                激发如影随形腿为脚法；
                
HELP
        );
        return 1;
}