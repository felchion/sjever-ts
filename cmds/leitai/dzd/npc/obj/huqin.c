// /d/dzd/obj/huqin.c

#include <ansi.h>
#include <weapon.h>

inherit SWORD;

void create()
{
	set_name("胡琴",({"huqin","jian","sword"}));
	set_weight(1000);
	if(clonep())
		set_default_object(__FILE__);
	else {
		set("unit","把");
		set("value","10000");
		set("material","steel");
		set("long","这是一把残旧的胡琴,看样子好像有些年月了，传说是当年莫大先生所用。
据说时间、地点不同，拉琴(playqin)的效果也不一样，不知道是不是真的！\n");
		set("wield_msg",HIC"忽然眼前青光一闪，$N从胡琴中抽出一柄细细的长剑。\n"NOR);
		set("unwield_msg",HIC"$N缓缓将长剑从胡琴底部插入，剑身尽没。\n"NOR);
	}
	init_sword(80);
	setup();
}

void init()
{
       	add_action("do_playqin","playqin");
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

int do_playqin(string arg)
{
	int dmg;
	object target;
	object me = this_player();
	if( !arg ) 
		target = offensive_target(me);
	else {	
		target = present(arg, environment(me));		
	}
	if(me->is_busy())
		return notify_fail("你正忙着呢。\n");
	if(me->query_temp("dzd_quest/play_huqin"))
		return notify_fail("你正在拉胡琴呢！\n");		
	if( !target ) {
		message_vision(BLU"忽然间伊伊呀呀的响起了胡琴之声,一曲“潇湘夜雨”自身边响起，渐渐远去，苍凉的胡琴声隐隐约约传来。\n"NOR,me);
		return 1;
	}
	//|| !me->is_fighting(target)
	if(!me->query_temp("dzd_quest/look_bei")
	 || !target	 
	 || !environment(me)->query("play_huqin") ) {
		message_vision(BLU"忽然间伊伊呀呀的响起了胡琴之声,一曲“潇湘夜雨”自身边响起，渐渐远去，苍凉的胡琴声隐隐约约传来。\n"NOR,me);
	}
	else {
		message_vision(BLU"忽然间耳中传入几下幽幽的胡琴声，琴声凄凉，似是叹息，又似哭泣，跟着琴声颤抖，
发出瑟瑟瑟断续之音，如是一滴滴小雨落上树叶。$N默念石碑上的诗句，将此情、此景水乳交融，胡琴声愈发凄苦。\n"NOR,me);
		me->set_temp("dzd_quest/play_huqin",1);
		remove_call_out("out_play");
		call_out("out_play",15,me);
			if(!random(2)) {
				message_vision(HIW"$N听得音色凄绝，如泣如诉。$N只觉灵台一片空明，竟不知自己身在何处,身是何物。只觉得世间一切都了无牵挂，悲极而恸，纵身撞向墓碑！\n"NOR,target);
				if(random(4)) { 
					message_vision(HIW"$N的头重重碰上墓碑，只觉天旋地转，眼前一黑，晕了过去。\n"NOR,target);
					target->unconcious();
				} else {
				message_vision(HIR"$N的头重重碰上墓碑，鲜血溅满了墓碑,神智略有清醒。\n"NOR,target);
				dmg = target->query("max_qi")/5;
				target->receive_damage("qi",dmg);
				target->receive_wound("qi",dmg);
				}
			} else 
				if(!random(3)) {
				message_vision(HIW"$N听的音色凄绝，如泣如诉，不觉精神恍惚，神志被夺，出招慢了下来。\n"NOR,target);
				target->start_busy(11);
				//remove_call_out("out_qinyin");
				call_out("out_qinyin",3+random(6),target);
			}
			else {
				message_vision(HIW"$N听的音色凄绝，如泣如诉，感觉不妙，马上意守丹田，凝神出招。\n"NOR,target);
				me->start_busy(1);	

		}
	}
	return 1;
}

void out_qinyin(object target)
{
	if( !target ) return;
	message_vision(HIW"$N猛地一惊，突然从琴音中惊醒过来，连忙紧守门户，凝神出招。\n"NOR,target);
	target->start_busy(-1);
	target->interrupt_me();
}

void out_play(object me)
{
	if(!me) return;
	me->delete_temp("dzd_quest/play_huqin");	
}