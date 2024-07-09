// /d/dzd/obj/yuwang.c

#include <ansi.h>

inherit ITEM;

int do_sa(string arg);

void create()
{
	set_name("渔网",({"yuwang"}));
	set_weight(50);
	if(clonep())
		set_default_object(__FILE__);
	else {
		set("long",HIW"这是一张以极细丝线结成的渔纲。丝线细如头发，质地透明，坚韧异常，遇物即缩。
不知道战斗的时候能不能撒(sa)别人呢？\n"NOR);
		set("value",200000);
		set("unit","张");
		set("material","silk");
	}
	setup();		
}
void init()
{
	add_action("do_sa","sa");
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

int do_sa(string arg)
{
	object target;
	object me = this_player();
	
	if( environment(me)->query("no_fight"))
	 	return notify_fail("这里不准战斗。\n");
	 
	if( !arg ) 
		target = offensive_target(me);
	else {
		target = present(arg, environment(me));
	}
	if(!target)
		return notify_fail("你想对谁使用渔网？\n");
		
	if(!target->is_character()) return notify_fail("您老搞笑啊？\n");
	
	//if(!target || !target->is_character() || !me->is_fighting(target))
		//return notify_fail("渔网只能对战斗中的对手使用！\n");
	if(!living(target))
		return notify_fail("网不能动的人？\n");
	if(target->is_busy())
		return notify_fail("对手正在自顾不暇，请放胆攻击吧！\n");
	if(me->query_temp("dzd_quest/yuwang_used"))
		return notify_fail("你正在使用渔网！\n");
	message_vision(HIW"$N手中突然多了件物事，似是一块透明的布疋，若有若无，不知是什么东西,向$n当头撒去！\n"NOR,me,target);
	me->set_temp("dzd_quest/yuwang_used",1);
	call_out("yuwang_busy",15,me);
	if(random(10) >= 4) {
		message_vision(HIW"$N突然间脚下一滑，扑地倒了，跟着身子便变成了一团。\n"NOR,target);
		if(random(3)) {
			message_vision(HIW"$N忙于挣开渔网，一时无暇出招！\n"NOR,target);
			target->start_busy(9);
			//remove_call_out("out_yuwang");
			call_out("out_yuwang",3+random(3),target);
		} else {
			message_vision(HIW"$N身入纲中，越是挣扎，渔纲缠得越紧，片刻之间，就成为一只大粽子般，给缠得难以动弹。\n"NOR,target);
			target->start_busy(10);
		}
	} else {
		message_vision(HIW"$N不慌不忙，轻轻闪了开去。\n"NOR,target);
		me->start_busy(1);
	}
	return 1;
}

void out_yuwang(object target)
{	
	if(!target) return;
	message_vision(HIW"$N忙了半天，终于将渔网解开。\n"NOR,target);
	target->start_busy(-1);
	target->interrupt_me();
	return;
}
	
int yuwang_busy(object me)
{
	if(!me) return 1;
	me->delete_temp("dzd_quest/yuwang_used");
	return 1;
}