// 太极拳PFM
// by snowman@SJ
// Modify by snowman@SJ 27/02/2000

#include <ansi.h>

int query_ap() {return 2;}

string exert_name(){ return HIG"太极功"NOR; }

int check_fight(object me);
int exert(object me)
{
	object weapon;
	int skill=me->query_skill("yinyun-ziqi", 1); 

	if( !me->is_fighting() || !living(me))
		return notify_fail("「太极」只能在战斗中的使用。\n");
	if( me->query_temp("tj/太极"))
		return notify_fail("「太极」之意绵绵不绝，你已经在发挥它的威力了。\n");
	if( (int)me->query("max_neili", 1) < 1200 )
		return notify_fail("你的内力修为太弱，发挥不出「太极」的优势。\n");
	if( (int)me->query("neili", 1) < 500 )
		return notify_fail("你现在真气不够，发挥不出「太极」的威力。\n");
	if( userp(me) && (int)me->query_skill("yinyun-ziqi", 1) < 100 )
		return notify_fail("你的氤氲紫气等级不够高，发挥不出「太极」的威力。\n");
	if( userp(me) && (int)me->query_skill("taoism", 1) < 100 )
		return notify_fail("你对道学心法的领悟不够，发挥不出「太极」那圆转不断的真意。\n");
	if( userp(me) && me->query_skill_mapped("force") != "yinyun-ziqi")
		return notify_fail("你的内功不对，根本不能发挥出「太极」的优势。\n");
	
	if( userp(me)
	&&( me->query_skill_mapped("parry") != "taiji-quan"
	&& me->query_skill_mapped("parry") != "taiji-jian") )
		return notify_fail("「太极」奥意必须在用太极拳或太极剑激发招架的情况下才能使用。\n");
	
	if( me->query_skill_prepared("cuff") != "taiji-quan"
	|| me->query_skill_mapped("cuff") != "taiji-quan")
		return notify_fail("你必须懂得太极拳，才能施展「太极」。\n");
	
	if( me->query_skill_mapped("sword") != "taiji-jian" )
		return notify_fail("你必须懂得太极剑法，才能施展「太极」。\n");

	if( objectp(weapon = me->query_temp("weapon")) ){
		if( weapon->query("skill_type") != "sword" )
			return notify_fail("你必须使用剑才能运用「太极」！\n");
		if( (int)me->query_skill("taiji-jian", 1) < 150 )
			return notify_fail("你的太极剑法不够娴熟，还不能领会「太极」之诀窍。\n");
		if( (int)me->query_skill("taiji-jian", 1) > 350 )
			return notify_fail("你已经参透了太极剑法，不用再使用「太极」诀窍了。\n");

		if( me->query("jiali") && me->query_skill("taiji-jian", 1) < 300)
			return notify_fail("「太极」讲究用意不用力，太极圆转，无使断绝。你如此用力，是无法发挥出它的威力的。\n");
		if( me->query("jiali") > me->query_skill("taiji-jian", 1) / 3)
			return notify_fail("为了使太极用意不用力之意完全发挥，你不能如此加力。\n");
		
		message_vision("\n$N缓缓移动，一手持剑，一手画了个剑指，两足分开平行，接着两臂慢慢提起至胸前，但见剑出剑回，每一招都含着太极式的阴阳变化，精微奥妙！\n",me);
		tell_object(me, CYN"\n你默运神功，将自己和剑与万物合一，将太极剑法"RED"「剑意」"CYN"发挥得淋漓尽致！\n\n"NOR);
	}

	else {
		if( (int)me->query_skill("taiji-quan", 1) < 100 )
			return notify_fail("你的太极拳不够娴熟，还不能领会「太极」之诀窍。\n");

		if( (int)me->query_skill("taiji-quan", 1) > 350 )
			return notify_fail("你已经参透了太极拳理，不用再使用「太极」诀窍了。");

		if( me->query("jiali") && me->query_skill("taiji-quan", 1) < 200)
			return notify_fail("「太极」讲究用意不用力，太极圆转，无使断绝。你如此用力，是无法发挥出它的威力的。\n");

		if( me->query("jiali") > me->query_skill("taiji-quan", 1) / 5)
			return notify_fail("为了使太极用意不用力之意完全发挥，你不能如此加力。\n");

		message_vision("\n$N缓缓移动，双手下垂，手背向外，手指微舒，两足分开平行，接着两臂慢慢提起至胸前，左臂半环，掌与面对成阴掌，右掌翻过成阳掌，但见$P双手圆转，每一招都含着太极式的阴阳变化，精微奥妙！\n",me);
		tell_object(me, CYN"\n你默运神功，双手自然挥动，与万物合一，将太极拳"RED"「拳意」"CYN"发挥得淋漓尽致！\n\n"NOR);
	}

	me->add("neili", -skill);
	me->add_temp("apply/pfm_attack", 60);
	me->add_temp("apply/pfm_damage", 40);
	me->add_temp("apply/pfm_parry", 120);
  me->set_temp("tj/太极",1);
	me->set_temp("taiji", 1);
	call_out("remove_effect", 10+skill/10, me, skill);
	me->start_exert(1, "太极");
	return 1;
	
}
   
void remove_effect(object me, int skill)
{
	if (!me) return;
	  me->delete_temp("taiji");
	  me->add_temp("apply/pfm_attack", -60);
	  me->add_temp("apply/pfm_damage", -40);
	  me->add_temp("apply/pfm_parry", -120);
	  me->delete_temp("tj/太极",1);
	  
	if (me->is_fighting())
		tell_object(me, HIY"你心浮气燥，手上攻势渐缓。\n"NOR);	
}

int help(object me)
{
	write(HIM"\n氤氲紫气「太极」："NOR"\n");
	write(@HELP
	太极之意，虽变化万端，而理为一贯。由招熟而渐悟懂劲，由懂劲而阶及神
	明。然非用力日久，不能豁然贯通焉。基本上所有太极拳剑绝招都须以此来
	配合。每见数年纯功，不能运化者，率皆自为人制，双重之病未悟而；循序
	加力须等级三百以后，阴阳相济，方为懂劲。懂劲后，方能愈练愈精。
	
	要求：	太极拳等级 100 以上；
		(用剑必须太极剑法等级 150 以上)
		氤氲紫气等级 100 以上；
		道学心法等级 100 以上；
		最大内力 1200 以上；
		内力 500 以上。
HELP
	);
	return 1;
}
