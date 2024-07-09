#include <weapon.h>
#include <ansi.h>

inherit BLADE;
inherit F_UNIQUE;
void create()
{
	set_name(HIY"黄金双铙"NOR,({ "huangjin nao", "nao" }) );
	set_weight(30000);
	if( clonep() )
	set_default_object(__FILE__);
	else {
		set("treasure", 1);
		set("unit", "副");
		set("value", 1000);
		set("unique", 1);
		set("rigidity", 4);
		set("sharpness", 4);
		set("weapon_prop/parry", 4);
		set("material", "gold");
		set("wield_neili", 900);
		set("wield_maxneili", 1400);
		set("wield_str", 25);
		set("weapon_prop/ap", 80);
		set("wield_msg", HIY"只听见「眶」地一声，伏魔斩妖两只金铙对击，闪出道道金光，声震天地！\n"NOR);
		set("long", HIY"\n这是一副用纯金打造的金铙，直径二尺有三，分为伏魔、斩妖两半，\n"+
		"好象可以分(fen)开，上面分别刻满佛门伏魔真言和斩妖法诀。\n"NOR);
		set("unwield_msg", HIY "黄金双铙向半空飞出，合二为一，又飞回了$N的手中。\n" NOR);
	}
	init_blade(140);
	setup();
}

void init()
{
	add_action("do_sub",({"fen", "分"}));
}

int do_sub()
{
	object me, obj;
	me = this_player();
	obj = this_object();
	if((int)me->query("neili") < 500)
	return notify_fail("你感觉全身气血翻腾，原来你真气不够，分不开黄金双铙！\n");
	if((int)me->query_str() < 28)
	return notify_fail("你感觉全身气血翻腾，原来你力量不够，分不开黄金双铙！\n");

	message_vision(HIY"\n只听霹雳一声巨响，$N将黄金双铙向半空掷出，金铙在空中一分为二，化为两道金光又飞回了$N的手中！\n"NOR, me);
	me->add("neili", -300);
	new(BINGQI_D("blade/jinnao1"))->move(me, 1);
	new(BINGQI_D("blade/jinnao2"))->move(me, 1);
	destruct(obj);
	return 1;
}
