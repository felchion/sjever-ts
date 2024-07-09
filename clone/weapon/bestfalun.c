//bestfalun.c 风火轮
//by iceland
#include <ansi.h>
#include <weapon.h>
inherit HAMMER;
inherit F_UNIQUE;
void create()
{
	set_name( HIR"风火轮"NOR,({ "fenghuo lun" ,"lun" ,"falun","fenghuolun"}));
	set_weight(75000);
	if (clonep())
	set_default_object(__FILE__);
	else {
		set("unit", "只");
		set("long",HIR"这只庞大的法轮上面刻着很多象形图案，转动之间隐隐显出火焰飞腾，这是传说中的兵器--风火轮。\n"
		"它是由数只法轮组合而成，如果力量足够，应该可以分开(fenkai)。\n"NOR);
		set("value", 1500);
		set("unique", 1);
		set("rigidity", 10);
		set("sharpness", 8);
		set("treasure",1);
		set("weapon_prop/dodge", 2);
		set("weapon_prop/parry", 10);
		set("weapon_prop/ap", 200);
		set("wield_neili", 800);
		set("wield_maxneili", 1800);
		set("wield_str", 40);
		set("material", "gsbil");
		set("wield_msg", HIR"$N从背后取出一只$n"HIR"握在手中，瞬间天昏地暗，狂风大做，四下里火光窜动。\n"NOR);
		set("unwield_msg",HIR"$N低头看着$n"HIR"，赞许的叹了口气，恋恋不舍的背在了背后。\n"NOR);
	}
	init_hammer(150);
	setup();
}
void init()
{
	add_action("do_sub",({"fenkai", "分开"}));
}

int do_sub()
{
	object me,ob;
	object ob1, ob2, ob3, ob4, ob5;;
	me = this_player();
	if((int)me->query_skill("longxiang-boruo",1)<200)
	return notify_fail("你的龙象般若功力修为尚未突破第十层天人绝境，无法分开此轮。\n");
	ob = this_object();
	ob1 = unew(BINGQI_D("gold-falun"));
	ob2 = unew(BINGQI_D("silver-falun"));
	ob3 = unew(BINGQI_D("bronze-falun"));
	ob4 = unew(BINGQI_D("iron-falun"));
	ob5 = unew(BINGQI_D("leaden-falun"));
	if (clonep(ob1) && clonep(ob2) && clonep(ob3) && clonep(ob4) && clonep(ob5)) {
		ob1->move(me);
		ob2->move(me);
		ob3->move(me);
		ob4->move(me);
		ob5->move(me);
		message_vision(HIY"$N将风火轮一转，只听叮叮几声轻响，风火轮散成了五只法轮。\n"NOR, me);
		destruct(ob);
		return 1;
	}
	if (ob1) destruct(ob1);
	if (ob2) destruct(ob2);
	if (ob3) destruct(ob3);
	if (ob4) destruct(ob4);
	if (ob5) destruct(ob5);
	return notify_fail("你试图将风火轮分成五个法轮，却发现五轮环环相嵌，实在无法分开。\n");
}
