// /d/dzd/obj/qimen.c

#include <ansi.h>
inherit ITEM;

void create()
{
	set_name(HIY"奇门八卦阵"NOR,({"qimenbagua zhenxing"}));
	set_weight(10);
	if(clonep())
		set_default_object(__FILE__);
	else {
		set("unit","个");
		set("long","这是一个简单的奇门八卦阵。\n");
		set("no_get",1);
	}
	setup();
}

void init()
{
	int dmg;
	object ob = this_object();
	object me = this_player();
	//if(me->query_temp("buzhen")) return;
	if(interactive(me) && ob->query_temp("zhen_master") != me->query("id")) {
		if(me->query_skill("qimen-bagua",1) >= 145)
			return notify_fail("$N对阵法早已烂熟于胸，信步走出了奇门八卦阵！\n");
		if(random(4)) {
			message_vision(HIW"$N一时不察，登时陷入阵中，不禁立时大乱！\n"NOR,me);
			me->start_busy(6);
		}
		else if(random(3)) {
			message_vision(HIW"$N一时不察，登时陷入阵中，一个不慎，已被阵法所伤！\n"NOR,me);
			dmg = me->query("eff_qi")/3;
			me->receive_damage("qi",dmg);
			me->receive_wound("qi",dmg);
		}
		else {
			message_vision(HIW"$N一时不察，登时陷入阵中，但微一思考已得其要领！\n"NOR,me);
			dmg = me->query_skill("force",1);
			dmg -= me->query_skill("qimen-bagua",1);
			dmg /= 4;
			me->start_busy(1);
			//me->add_temp("apply/dodge",-dmg);
			//me->add_temp("apply/parry",-dmg);
			//me->add_temp("apply/attack",-dmg);
			//call_out("recover_status",random(5)+10,me,dmg);
		}
	}
}

void recover_status(object me,int dmg)
{
	if(!me) return;
	me->add_temp("apply/dodge",dmg);
	me->add_temp("apply/parry",dmg);
	me->add_temp("apply/attack",dmg);
}