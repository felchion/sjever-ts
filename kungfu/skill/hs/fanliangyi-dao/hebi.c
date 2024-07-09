// 从闪电的代码扒的，fanliangyi-dao perform hebi

#include <ansi.h>

string perform_name() {return HIY"双人合壁"NOR;}

int check_fight(object me, object target, int amount);
private int remove_effect(object me, object target, int amount);

int perform(object me, object target)
{
        object *enemy;
        int i,j,k,amount;
        object wep1, wep2;
	wep1 = me->query_temp("weapon");

        if( !target || target == me) return notify_fail("你要和谁合璧？\n");

        if (me->query_temp("hebi")) return notify_fail("你已经在合璧了。\n");
        if (target->query_temp("hebi")) return notify_fail("对方已经在合璧了。\n");
        if (me->query("jingli") < 200) return notify_fail("你的精力不够了。\n");
        if (target->query("jingli") < 200) return notify_fail("对方的精力不够了。\n");
        if (!me->is_fighting()) return notify_fail("合璧只能在战斗中使用。\n");
        if (me->is_fighting(target)) return notify_fail("你正在和对方打架，合璧干什么？\n");

        if( (int)me->query_skill("fanliangyi-dao", 1) < 120 )
                return notify_fail("你的反两仪刀法不够娴熟，不会使用合璧。\n");
        if( (int)me->query_skill("zixia-gong", 1) < 120 )
                return notify_fail("你的紫霞神功未到火候，如何会有合璧的威力？\n");
        if (!wep1 || wep1->query("skill_type") != "blade"
        || me->query_skill_mapped("blade") != "fanliangyi-dao")
                return notify_fail("你现在无法合璧。\n");
        
        enemy = me->query_enemy();
        i = sizeof(enemy);
        while (i--) if (target->is_fighting(enemy[i])) break;
        if (i<0) return notify_fail(target->name()+"并没有和你的对手在交战。\n");

        if( (int)target->query_skill("fanliangyi-dao", 1) < 120 )
                return notify_fail("对方的反两仪刀法不够娴熟，不会使用合璧。\n");
        if( (int)target->query_skill("zixia-gong", 1) < 120 )
                return notify_fail("对方的紫霞神功不够纯熟，无法发挥合璧威力。\n");
        wep2 = target->query_temp("weapon");
        if (target->is_busy() || !wep2 || wep2->query("skill_type") != "blade"
        || target->query_skill_mapped("blade") != "fanliangyi-dao")
                return notify_fail("对方现在无法合璧。\n");
       
        message_vision(HIY "\n$N和$n互相使了个眼色，双刀一交，威力顿时陡增。但见飞沙走石，日月为之失色。\n" NOR, me, target);
        me->set_temp("hebi", target->query("id"));
        target->set_temp("hebi", me->query("id"));
        me->receive_damage("jingli", 100);
        target->receive_damage("jingli", 100);
        me->receive_damage("neili", 200);
        target->receive_damage("neili", 200);
        me->start_busy(1);
        target->start_busy(1);
          k = (int)me->query_skill("fanliangyi-dao", 1);
          j = (int)target->query_skill("fanliangyi-dao", 1);
          amount = (k + j)/10;
        me->add_temp("apply/dexerity", amount);
        me->add_temp("apply/strength", amount);
        target->add_temp("apply/dexerity", amount);
        target->add_temp("apply/strength", amount);
        if (me->query("marry/id") == target->query("id"))
 	  {
             message_vision(HIY "\n$N和$n两夫妻灵犀相通，手中两把钢刀犹如双飞彩凤，威力更增一层。\n" NOR, me, target);
             me->add_temp("apply/dexerity", amount/2);
             me->add_temp("apply/strength", amount/2);
             me->add_temp("apply/attack", amount);
             target->add_temp("apply/dexerity", amount/2);
             target->add_temp("apply/strength", amount/2);
	     target->add_temp("apply/attack", amount);
          }
	 check_fight(me, target, amount);
        return 1;
}

int check_fight(object me, object target, int amount)
{  
    object wep1, wep2;
    if(!me || !target) return remove_effect(me, target, amount);
    wep1 = me->query_temp("weapon");
    wep2 = target->query_temp("weapon");
    if(!me->is_fighting() || !living(me) || me->is_ghost() || !wep1 || 
	!target->is_fighting() || !living(target) || target->is_ghost() || !wep2)
        remove_effect(me, target, amount);
    else {
        call_out("check_fight", 1, me, target, amount);
    }
    return 1;
}

private int remove_effect(object me, object target, int amount)
{
	if(me && target && living(me) && !me->is_ghost() && living(target) && !target->is_ghost())
        message_vision(HIY "\n$N和$n一路反两仪刀法合璧用完，灵犀互通，相视一笑，各自收招。\n" NOR, me, target);

	if (me && me->query("marry/id") == me->query_temp("hebi"))
	{
		me->add_temp("apply/dexerity", -amount/2);
		me->add_temp("apply/strength", -amount/2);
		me->add_temp("apply/attack", -amount);
	}
	if (target && target->query("marry/id") == target->query_temp("hebi"))
	{
		target->add_temp("apply/dexerity", -amount/2);
		target->add_temp("apply/strength", -amount/2);
		target->add_temp("apply/attack", -amount);
	}

	if (me) {
		me->add_temp("apply/dexerity", -amount);
		me->add_temp("apply/strength", -amount);
		me->delete_temp("hebi");
	}
	if (target) {
		target->add_temp("apply/dexerity", -amount);
		target->add_temp("apply/strength", -amount);
		target->delete_temp("hebi");
	}
	return 0;
}
