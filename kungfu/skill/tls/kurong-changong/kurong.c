// By Spiderii 合并同类项 kurong.c
// By Spiderii 鉴于空手门派，提高防御

#include <ansi.h>
void remove_effect(object me,int skill);

int query_ap() {return 3;}

string exert_name() {return HIG"亦枯亦荣"NOR;}

int exert(object me)
{
	int lv;
	lv = me->query_skill("kurong-changong",1);
	if ( !me->is_fighting())
		return notify_fail("你只能在战斗中使用「枯荣」！\n");

	if( lv < 150 )
		return notify_fail("你的枯荣禅功修为不够，无法使用「枯荣」！\n");

	if (me->query_skill_mapped("force") != "kurong-changong")
		return notify_fail("其它内功有「枯荣」吗？\n");

	if( (int)me->query("neili") < 2000 )
		return notify_fail("你目前的内力太少了，无法使用「枯荣」！\n");

	if( (int)me->query("max_neili") < 3000 )
		return notify_fail("你的内力修为这么差，还想使用「枯荣」？\n");

	if( me->query_temp("krcg_kurong"))
		return notify_fail("你正在运用「枯荣」！\n");

	// 亦枯亦荣
	if (lv > 450)
		message_vision(HIG "$N" HIG "运起枯荣禅功，全身皮肤竟变得一半犹"
		"如婴儿，另一半却似干皱的树皮。\n" NOR, me);
	// 全荣
	else if (lv > 350)
		message_vision(HIC "$N" HIC "暗自凝神，运起枯荣禅功，全身皮肤竟"
		"变得犹如婴儿般光滑亮泽。\n" NOR, me);
	// 全枯
	else if (lv > 250)
		message_vision(YEL "$N" YEL "暗自凝神，运起枯荣禅功，全身皮肤竟"
		"变得犹如树皮般干皱苍老。\n" NOR, me);
	// 半枯半荣
	else
		message_vision(HIY "$N" HIY "暗自凝神，运起枯荣禅功，真气顿时游"
		"遍全身。\n" NOR, me);


	me->add_temp("apply/eq_armor", 150+lv/10);
	me->add_temp("apply/pfm_parry", 150+lv/10);

	me->add("neili", -lv*2);
	me->set_temp("krcg_kurong",1);
	call_out("remove_effect", 1, me, lv);

	return 1;
}
void remove_effect(object me, int lv)
{

	if (objectp(me)) {
		if (me->is_fighting()) {
			call_out("remove_effect", 1, me, lv);
			return;
		}

		me->delete_temp("krcg_kurong");

	  me->add_temp("apply/eq_armor", -150-lv/10);
	  me->add_temp("apply/pfm_parry", -150-lv/10);
		message_vision(WHT"\n$N的枯荣禅功运行完毕，将内力收回丹田。\n"NOR, me);
		return;

	}
}


int help(object me)
{
        write(WHT"\n枯荣禅功「枯荣」："NOR"\n");
        write(@HELP
        枯荣禅功乃是大理国护国寺院--天龙寺主持枯荣长老的内功心法
        相传佛祖涅磐是在八棵树之间。这八棵树分东西南北两棵辆棵分
        列，它们分别是一枯一荣，东西南北分别是：常与无常，乐与无
        乐，我与无我，净与无净。 讲究心中无枯无荣方是大成。枯荣
        长老心中有物，是以终其一生，只参得半枯半荣。
        此枯荣一诀能使玩家大幅度提高自身的防御与招架，并且有一定
        几率使对方攻击无效！

        要求:   枯荣禅功 150 级以上；
                最大内力 3000 点；
                当前内力 2000 点；
                需激发内功为枯荣禅功。
              
HELP
        );
        return 1;
}


