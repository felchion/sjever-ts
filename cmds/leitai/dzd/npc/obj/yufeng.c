// /d/dzd/obj/yufeng.c
#include <ansi.h>
inherit ITEM;

int do_zhao(string arg);

void create()
{
	set_name("玉蜂瓶",({"yufeng ping","ping"}));
	set_weight(500);
	if(clonep())
		set_default_object(__FILE__);
	else {
		set("long","这是当年小龙女养蜂用的玉蜂瓶，做的精致异常！
不知道现在还可不可以招(zhao)玉蜂？\n");
		set("value",200000);
		set("unit","个");
	}
	setup();
}

void init()
{
	add_action("do_zhao","zhao");
}

object offensive_target(object me)
{
	int sz;
	object *enemy;

	enemy = me->query_enemy();
	if( !enemy || !arrayp(enemy) ) return 0;

	sz = sizeof(enemy);
	if( sz > 4 ) sz = 4;

	if( sz > 0 ) return enemy[random(sz)];
	else return 0;
}

int do_zhao(string arg)
{
	object target;
	object me = this_player();
	
	if( environment(me)->query("no_fight"))
	 	return notify_fail("这里不准战斗。\n");	 	
	//if(!arg || arg!= "yufeng")
	//	return notify_fail("你想招什么？\n");	
	if( !arg ) 
		target = offensive_target(me);
	else {
		target = present(arg, environment(me));
	}
	if(!target) return notify_fail("你想对谁用？\n");
	if(!target->is_character()) return notify_fail("您老搞笑啊？\n");
	//if(!target || !target->is_character() || !me->is_fighting(target))
		//return notify_fail("玉蜂只能对战斗中的对手使用！\n");
	if(!living(target))
		return notify_fail("都这样了，你就不必费心机了。\n");	
	if(me->query_temp("dzd_quest/yufeng_used"))
		return notify_fail("你正在招玉蜂！\n");
	message_vision(HIW"$N从怀中取出玉瓶，右手伸掌握住，拔开瓶塞，潜运掌力，热气从掌心传入瓶中，
过不多时，一股芬芳馥郁的蜜香送了出去。！\n"NOR,me);
	me->set_temp("dzd_quest/yufeng_used",1);
	//remove_call_out("zhao_yf1");
	call_out("zhao_yf1",2,me,target);
	//remove_call_out("yufeng_busy");
	call_out("yufeng_busy",20,me);
	return 1;
}

void zhao_yf1(object me,object target)
{
	int dmg;
	//object target;
	//object me = this_player();
	if(!me || !living(me)) return;
	if( !target ) target = offensive_target(me);
	if(!target || !target->is_character() || !me->is_fighting(target)) return;
	if(!living(target)) return;
	message_vision(HIW"突然树林中灰影闪动，飞出一群玉蜂。$N小指指甲伸入玉瓶，挑了一点蜂蜜向$n弹去，
左手食指向$n左边一点，右边一点，口中呼啸吆喝，几千只玉蜂转身向$n冲去。\n"NOR,me,target);
	if(random(10) >= 3) {
		message_vision(HIW"$N大惊，挥动袍袖要将蜂子驱开。\n"NOR,target);
		if(random(3)) {
			message_vision(HIW"蜂群突分为二，一群正面扑来，另一群却从后攻至！$N更是心惊，不敢怠慢，
双袖飞舞，护住全身,一时无暇出招。\n"NOR,target);
			target->start_busy(5);
		} else {
			message_vision(HIW"$N只见无数白色蜂子在身周飞舞来去，耳中听到的尽是嗡嗡之声，
衣袖舞得微一缓慢，几只玉蜂猛地从空隙中飞了进去，在$N身上上各螫了一针。\n"NOR,target);
			target->add_condition("yufeng_poison",6);
			dmg = target->query("max_qi")/5;
			target->receive_damage("qi",dmg);
			target->receive_wound("qi",dmg);
		}
	} else {
		message_vision(HIW"$N大惊，忙潜运内力，挥动袍袖将玉蜂尽数驱开。\n"NOR,target);
		me->start_busy(1);
	}
}
	
void yufeng_busy(object me)
{
	if(!me) return;
	me->delete_temp("dzd_quest/yufeng_used");
	return;
}