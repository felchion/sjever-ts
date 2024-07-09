// yuan.c 鸳刀
// zly 99.6.22

#include <weapon.h>
#include <ansi.h>

inherit BLADE;
inherit F_UNIQUE;

void create()
{
	set_name(HIC"鸳刀"NOR, ({"yuan dao","yung","dao","blade"}));
	set_weight(10000);
	if( clonep() )
	set_default_object(__FILE__);
	else{
		set("treasure", 1);
		set("unit", "把");
		set("long", HIC"这是鸳鸯刀中的鸳刀，但见精光耀眼，刀刃上刻著「仁者」二字。\n" NOR);
		set("value",4000);
		set("material","steel");
		set("weapon_prop/parry",2);
		set("unique", 1);
		set("rigidity", 3);
		set("sharpness", 3);
		set("wield_neili", 500);
		set("wield_maxneili", 900);
		set("weapon_prop/ap", 40);
		set("wield_str", 22);
		set("wield_msg", HIY "$N抽出一把青光闪耀，寒气逼人的短刀握在手中。顿时觉得心中升起一股\n"+
		"浩然正气,天地之间无所畏惧。 \n" NOR);
		set("unwield_msg", HIY "$N轻轻将手中的鸳刀插入腰间的刀鞘。\n" NOR);
	};
	init_blade(80);
	setup();
}

void init()
{
	add_action("do_add",({"he", "合"}));
}

int do_add()
{
	object me,ob, obj;
	me = this_player();
	ob = this_object();
	obj = present("yang dao", me);
	if(!objectp(obj))
	return notify_fail("你手中只有一把鸳刀，先找到另一把再说吧。\n");
	if((int)me->query("neili") < 1000)
	return notify_fail("你感觉全身气血翻腾，头晕目眩，原来你真气不够，无法将双刀合并！\n");
	if((int)me->query_str() < 28)
	return notify_fail("你感觉使出了全身的力气，仍无法将双刀合并！\n");
	me->add("neili", -300);
	message_vision(HIC"\n只听一声轻响，你双手把刀一并，卡嗒一声合二为一，拿在$N的手里。\n"NOR, me);
	destruct(obj);
	new(BINGQI_D("blade/yuanyang-dao"))->move(me, 1);
	destruct(ob);
	return 1;
}
