// sf_ling.c
// Created by snowman@SJ 08/05/2001

#include <weapon.h>
#include <ansi.h>

inherit DAGGER;
inherit F_UNIQUE;
void create()
{
	set_name(WHT"赏善罚恶令"NOR,({ "shangshanfae ling", "shangshanfae", "ling", "dagger" }) );
	set_weight(8000);
	if( clonep() )
	set_default_object(__FILE__);
	else {
		set("treasure", 1);
		set("unit", "对");
		set("value", 1000000);
		set("unique", 1);
		set("rigidity", 3);
		set("weapon_prop/dodge", 3);
		set("weapon_prop/parry", 2);
		set("weapon_prop/ap", 80);
		set("dagger_count", 2);
		set("material", "bonze");
		set("wield_neili", 100);
		set("wield_maxneili", 1000);
		set("wield_str", 20);
		set("wield_msg", WHT"「铛」地一声，$N"WHT"摸出$n"WHT"相互一击，发出刺耳的声音！\n"NOR);
		set("long", WHT"\n两块闪闪发光的白铜牌子，约有巴掌大小，一块牌上刻有一张笑脸，和蔼慈祥，另一牌上刻的却是一张狰狞的煞神凶脸，显得十分诡异！
这两块令好像能 分开 似的。\n"NOR);
		set("unwield_msg", WHT"$N"WHT"将$n"WHT"收回到口袋里。\n" NOR);
	}
	init_dagger(160);
	setup();
}

void init()
{
	if (environment()==this_player())
	add_action("do_sub",({"fen", "分开"}));
}

int do_sub()
{
	object me, obj;
	me = this_player();
	obj = this_object();
	if((int)me->query("neili") < 200)
	return notify_fail("你感觉全身气血翻腾，原来你真气不够，分不开赏善罚恶令！\n");
	if((int)me->query_str() < 25)
	return notify_fail("你感觉全身气血翻腾，原来你力量不够，分不开赏善罚恶令！\n");

	message_vision(HIW"\n$N将赏善罚恶令相互一挫，令一下子一分为二，闪出耀眼的白光！\n"NOR, me);
	me->add("neili", -150);
	new(BINGQI_D("shangshan_ling"))->move(me, 1);
	new(BINGQI_D("fae_ling"))->move(me, 1);
	destruct(obj);
	return 1;

}

