// /d/dzd/obj/qxht.c

#include <ansi.h>

inherit ITEM;

void create()
{
	set_name("七心海棠粉",({"haitang fen","fen"}));
	set_weight(50);
	if(clonep())
		set_default_object(__FILE__);
	else
	{
		set("long",HIB"这是一包由七心海棠炼制而成的粉末，剧毒无比！
下毒的时候可以弹(tan)哦。\n"NOR);
		set("value",200000);
		set("unit","包");
	}
	setup();		
}
void init()
{
	add_action("do_tan","tan");
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

int do_tan(string arg)
{
	object me =this_player();
	object target;
	
	if( environment(me)->query("no_fight"))
	 	return notify_fail("这里不准战斗。\n");
	if( !arg ) 
		target = offensive_target(me);
	else {
		target = present(arg, environment(me));
	}
	if(!target)
		return notify_fail("你想对谁使用七心海棠毒？\n");
	if(!target->is_character()) return notify_fail("您老搞笑啊？\n");
	
	if(me->is_busy())
		return notify_fail("你正忙着呢。\n");
	//if(!target || !target->is_character() || !me->is_fighting(target))
		//return notify_fail("毒粉只能对战斗中的对手使用！\n");
	if(me->query_temp("dzd_quest/fen_used"))
		return notify_fail("你已经对对手使用七心海棠毒了。\n");
	if(!living(target))
		return notify_fail("不用下毒了吧？\n");
	me->set_temp("dzd_quest/fen_used",1);
	call_out("out_fen",10,me);
	tell_object(me,BLU"你手掌一伸，指甲中已挑了七心海棠的药粉,右手食指的指甲一弹，一阵无色无味的薄雾向对方飘去。\n"NOR);
	if(random(3)) {
		target->apply_condition("haitang_poison",4 + target->query_condition("haitang_poison"));
		message_vision(HIY"$N一时大意，已经中了$n的暗算！\n"NOR,target,me);
	}
	else 
		message_vision(HIY"$N看你动作有异，心下早有准备，闭气凝神，连击数掌，把毒雾驱散！\n"NOR,target);
	me->start_busy(1);
	return 1;
}

void out_fen(object me)
{
	//remove_call_out("out_fen");
	if(!me) return;
	me->delete_temp("dzd_quest/fen_used");
}