// /d/dzd/obj/hssy.c

#include <ansi.h>

inherit ITEM;

void create()
{
	set_name("含沙射影",({"hansha sheying","hansha"}));
	set_weight(50);
	if(clonep())
		set_default_object(__FILE__);
	else {
		set("long","铁带上装着一只钢盒，盒盖上有许多小孔,机括装在胸前，发射时不必先取准头，只须身子对正敌人,伸手在腰旁一按，
一阵钢针就由强力弹簧激射而出,真是神不知，鬼不觉，何况钢针既细，为数又多，一枚沾身，便中剧毒,实是天下第一阴毒暗器!
这么厉害的暗器，要是随便一按(sheying)……嘿嘿。\n");
		set("value",200000);
		set("unit","只");
		set("material","steel");
	}
	setup();
}

void init()
{
	add_action("do_an","sheying");
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

int do_an(string arg)
{
	int dmg;
	object target;
	object me = this_player();
	
	if( environment(me)->query("no_fight"))
	 	return notify_fail("这里不准战斗。\n");
	if( me->is_busy()) return notify_fail("你现在正忙着呢。\n");			
	if( !arg ) 
		target = offensive_target(me);
	else {
		if(!objectp(target = present(arg, environment(me))))
		return notify_fail("你想对谁使用含沙射影？\n");	
	}
	if(!target)
		return notify_fail("你想对谁使用含沙射影？\n");	
	if(!target->is_character()) return notify_fail("您老搞笑啊？\n");
	//if(!target || !target->is_character() || !me->is_fighting(target))
		//return notify_fail("含沙射影只能对战斗中的对手使用！\n");
	
	if(!living(target))
		return notify_fail("都这样了，还有这必要？\n");;
	/*
	dzd_quest/hansha/used
	dzd_quest/hansha/num	
	*/
	if(me->query_temp("dzd_quest/hansha/used"))
		return notify_fail("你正在使用含沙射影！\n");
	message_vision(HIW"$N将身子对正敌人，伸手在腰旁一按，$n突然间眼前金光闪动,一阵钢针由$N腰间激射而出！\n"NOR,me,target);
	me->set_temp("dzd_quest/hansha/used",1);
	me->add_temp("dzd_quest/hansha/num",1);
	remove_call_out("hansha_busy");
	call_out("hansha_busy",15,me );
	if(random(10) >= 3) {
		message_vision(HIW"钢针既细，为数又多，$N大惊之下，不及躲闪，被打了个正着！\n"NOR,target);
		dmg = target->query("eff_qi")/2;
		target->receive_damage("qi",dmg);
		target->receive_wound("qi",dmg);
		target->add_condition("snake_poison",10);	
	} else {
		message_vision(HIW"$N大吃一惊，知道不妙，百忙中一飞冲天，跃起寻丈，只听得一阵细微的铮铮之声，数十枚暗器都打在地上。\n"NOR,target);
		me->start_busy(1);
	}
	if(me->query_temp("dzd_quest/hansha/num")>=5)
		destruct(this_object());
	return 1;
}

	
void hansha_busy(object me)
{
	if(!me) return;
	me->delete_temp("dzd_quest/hansha/used");	
}