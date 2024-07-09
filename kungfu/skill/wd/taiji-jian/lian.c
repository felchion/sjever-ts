#include <ansi.h>
inherit F_SSERVER;

int query_ap() {return 10;}

int perform(object me, object target)
{

	object weapon = me->query_temp("weapon");
	int lvl,i;

	if( !target ) target = offensive_target(me);

	if( !objectp(target) || !me->is_fighting(target) )
		return notify_fail("「连」字诀只能在战斗中对对手使用。\n");

	if( !living(target) )
		return notify_fail("「连」字诀只能在战斗中对对手使用。\n");

	if( !weapon || weapon->query("skill_type") != "sword" )
		return notify_fail("你手中无剑，如何使得「连」字诀？\n");

	if( me->query_skill_mapped("sword") != "taiji-jian" )
		return notify_fail("你所备不是太极剑法，无法使用「连」字诀。\n");

	if( me->query_skill_mapped("parry") != "taiji-jian" )
		return notify_fail("你必须以太极剑法为招架才能使用「连」字诀。\n");  

	if( me->query_temp("tjj/chan") )
		return notify_fail("你正在使用「缠」字诀。\n");

	if( (int)me->query_temp("tjj/lian") )
		return notify_fail("你现在正在使用「连」字诀。\n");

	if( (int)me->query_skill("taiji-jian", 1) <120 )
		return notify_fail("你的太极剑法修为尚不纯熟,无法使用「连」字诀！\n");             

	if( (int)me->query_skill("yinyun-ziqi",1) < 120 )
		return notify_fail("你的氤氲紫气修为尚浅，不能使用「连」字诀！\n");

	if( (int)me->query_skill("sword",1) < 120 )
		return notify_fail("你基本剑法修为尚不纯熟,不能使用「连」字诀！\n");  

	if( (int)me->query("max_neili") < 1500 )
		return notify_fail("你的内力修为太弱，不能使用「连」字诀！\n");

	if( (int)me->query("neili") < 800 )
		return notify_fail("你的真气太少了，不能使用「连」字诀！\n");  

	me->set_temp("tjj/lian",1);

	lvl = me->query_skill("taiji-jian", 1);

	message_vision(CYN"\n$N微微一笑，使出太极剑中"+RED+"「连」"+CYN+"字诀，当下身子缓缓右转,左手持剑向上提起，剑身横于胸前，\n"
			"左右掌心相对，如抱圆球，手中"+weapon->name()+CYN"未出，已然蓄势无穷。"NOR,me,target);
	
	me->add("neili", -100-lvl/4);
	me->receive_damage("neili", 100);
	me->receive_damage("jingli", 100);
	me->add_temp("apply/pfm_attack", 240+lvl/10);
	me->add_temp("apply/pfm_damage", 230+lvl/4);
	me->add_temp("apply/damage", 500+lvl);

	COMBAT_D->do_attack(me, target, me->query_temp("weapon"), 1);

  i = 9;

	call_out("check_fight", 2, me, i, lvl);
	return 1;
}
  

void remove_effect(object me, int i)
{
	if(!me) return;

	me->delete_temp("tjj/lian");
	me->add_temp("apply/pfm_attack", -240-i/10);
	me->add_temp("apply/pfm_damage", -230-i/4);
	me->add_temp("apply/damage", -500-i);
	me->delete_temp("apply/action");
	
	if (living(me)){
		message_vision( HIY"\n$N收剑而立，笼罩在全身上下的大小光圈随即不见踪影。\n\n"NOR, me);
		if (me->is_fighting())
			me->add_busy(1 + random(2));
	}
}

void check_fight(object me, int i, int j)
{
	object weapon, target;
	string msg;

	if (!me ) return;

	target = offensive_target(me);

	if(!i
	|| !living(me)
	|| !target
	|| !me->is_fighting(target)
	|| environment(target)!=environment(me)
	|| !me->query_temp("tjj/lian")
	|| me->query("neili") < 200
	|| me->query("jingli") < 200)
		return remove_effect(me, j);

	if(me->query_skill_mapped("sword") != "taiji-jian") {
		tell_object(me, HIR"你临时改换剑法，决定不再使用「连字诀」绝技。\n");
		return remove_effect(me, j);
	} 
	else if(me->query_skill_mapped("force") != "yinyun-ziqi") {
		tell_object(me, HIR"你临时改换内功，决定不再使用「连字诀」绝技。\n");
		return remove_effect(me, j);
	} 
	else if(!objectp(weapon = me->query_temp("weapon"))
		|| weapon->query("skill_type") != "sword"){
		tell_object(me, HIR"你临时改换武器，决定不再使用「连字诀」绝技。\n");
		return remove_effect(me, j);
	} 
	else if (me->is_busy())
		call_out("check_fight", 1, me, i, j);
	else {
		me->receive_damage("neili", 100);
		me->receive_damage("jingli", 100);
		switch(i%3) {
			case 0:
				msg = CYN"\n$N左手"+weapon->name()+CYN"缓缓向前划出，成一弧形，一股森森寒气，直逼向$n，突然之间，$N剑交右手，寒光一闪，向$n划去";
				break;
			case 1:
				msg =CYN"\n$N又将剑交左手，在身前划了两个圆圈，剑劲连绵护住全身，竟无半分空隙，右手捏着剑诀，\n"+
					"左手"+weapon->name()+CYN"不住的抖动，罩向$n上盘七大要穴，剑尖急颤，看不出攻向何处";     
		    break;
	    case 2:	
				msg = CYN"\n$N剑上所幻的"+RED+"光圈"+CYN+"越来越多，过不多时，全身已隐在无数"+RED+"光圈"+CYN+"之中，"+RED+"光圈"+CYN+"光圈一个未消，另一个再生，\n"
					"手中"+weapon->name()+CYN"虽使得极快，却丝毫听不到金刃劈风之声，足见剑劲之柔韧已达于化境";
				break;
		}
		me->set_temp("apply/action/pfm",msg);
		COMBAT_D->do_attack(me, target, me->query_temp("weapon"), random(2)?1:3);
		call_out("check_fight", 2, me, --i, j);
	}
}


string perform_name(){ return CYN"连字诀"NOR; }

int help(object me)
{
        write(CYN"\n太极剑法之「连字诀」："NOR"\n\n");
        write(@HELP
        要求：  当前内力 800 以上;
                最大内力 1500 以上；
                太极剑法等级 120 以上；
                氤氲紫气等级 120 以上；
                基本剑法等级 120 以上；
                激发剑法为太极剑法；
                激发招架为太极剑法；
                激发内功为氤氲紫气。

HELP
        );
        return 1;
}
