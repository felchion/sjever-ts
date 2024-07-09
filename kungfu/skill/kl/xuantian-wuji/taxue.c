// 玄天无极-踏雪无痕
// 脚踏白雪身无痕
// modify by cmkj 2003.8.19
// update增加飞跃后效果
#include <ansi.h>
inherit F_CLEAN_UP;
int query_ap() {return 2;}

string exert_name(){ return HIW"踏雪无痕"NOR; }

int exert(object me, object target)
{
	int skill;

  if ( me->query_skill_mapped("force") != "xuantian-wuji")
		return notify_fail("你现在的情况，无法使用踏雪无痕。\n");
	if ( me->query_skill_mapped("dodge") != "taxue-wuhen")
		return notify_fail("你现在的情况，无法使用无极心法。\n");		
	if ((int)me->query_skill("xuantian-wuji", 1) < 100)
	  return notify_fail("你的玄天无极功修为还不够。\n");
	if((int)me->query("max_neili")<1000)
  	return notify_fail("你的内力修为太差，多多打坐吧。\n");
	if( (int)me->query("neili") < 800 )
  	return notify_fail("你的真气不够。\n");
	if (me->query_temp("kl/taxue"))
  	return notify_fail("你已经在使用踏雪无痕了。\n");


	skill = me->query_skill("xuantian-wuji",1) / 3;

	message_vision(CYN "$N双手虚按丹田，展开轻功，身子轻飘飘的，宛似凌空飞行！\n" NOR, me);

	me->add_temp("apply/pfm_dodge", 150);
	me->set_temp("kl/taxue", skill);
	me->add("neili", -150-skill/2);
	me->start_call_out( (: call_other, __FILE__, "remove_effect", me :), skill/2 );
	if( me->is_fighting() ) me->start_busy(1);

	return 1;
}

void remove_effect(object me)
{
	int amount;

	if ( me && (int)me->query_temp("kl/taxue") ) {
		amount = me->query_temp("kl/taxue");
		me->add_temp("apply/pfm_dodge", - 150);
		me->delete_temp("kl/taxue");
		message_vision(CYN "$N散去丹田聚集的内力，立定了脚步，身法回复了正常！\n" NOR, me);
	}
}

int help(object me)
{
        write(HIW"\n玄天无极功之「踏雪无痕」："NOR"\n\n");
        write(@HELP
        要求：  当前内力 800 以上；
                玄天无极功等级 100 以上；
                激发内功为玄天无极功。
                激发轻功为踏雪无痕。

HELP
        );
        return 1;
}