// kusang_stick.c 哭丧棒
// by hunthu 99/7

#include <ansi.h>
#include <weapon.h>
inherit STICK;
inherit F_UNIQUE;
void create()
{
	set_name(BLU"哭丧棒"NOR, ({"kusang bang", "bang", "stick"}));
	set("weight", 10000);
	if (clonep())
	set_default_object(__FILE__);
	else {
		set("unit", "根");
		set("long", "一根乌黑的铁棒，沉甸甸的似乎有什么古怪。\n");
		set("value", 1000);
		set("unique", 1);
		set("material", "blacksteel");
		set("weapon_prop/ap", 40);
		set("rigidity", 6);
		set("wield_msg", "$N阴阴一笑从背后抽出一根黑棒子。\n");
		set("unwield_msg", "$N把手中的哭丧棒背回背后。\n");
		set("chanchu_poison",3);
	}
	init_stick(80);
	set("treasure",1);
	setup();
}


void init()
{
	add_action("do_throw", "she");
}
int do_throw(string arg)
{
	object me, ob;
	int i, damage;
	string msg;

	me = this_player();
	if (!arg || arg == "")   return 0;
	ob = present(arg, environment(me));
	if (!ob) return 0;
	if(! me->is_fighting(ob)) return notify_fail("你必须在战斗中才能使用.\n");
	i = ob->query_skill("dodge", 1) + ob->query_skill("parry", 1);
	i = random(i/2) - 10;
	damage = me->query_skill("bangjue", 1)+me->query_str()*2;
	if (!arg) return notify_fail("你要对谁下手？\n");
	if (!ob) return notify_fail("找不到这个生物。\n");
	if( environment(me)->query("no_fight") )
	return notify_fail("你不能在这里动手。\n");
	if(me->is_busy())
	return notify_fail("你正忙着呢。\n");
	if(me->query("neili") < 500)
	return notify_fail("你的内力不够用来动手。\n");
	if(this_object()->query("chanchu_poison")<1)
	return notify_fail("哭丧棒里面的蟾蜍毒粉已经用尽了，无法再喷射伤人了。\n");
	msg = HIW"\n$N一声阴笑，按动棒底机栝，一股碧绿的蟾蜍毒烟向$n急射而去！\n"NOR;
	me->start_busy(2);
	me->add("neili", -100);
	me->add("jingli", -20);
	if(me->query_skill("bangjue", 1) > i && random(ob->query("kar")) < 10 &&
	random(me->query("combat_exp")) > (int)ob->query("combat_exp")/2 ) {
		msg += HIR"$n烟雾之中，只见$n急忙后跃闪避，但是仍然吸入不少毒雾，摇摇欲坠！\n"NOR;
		ob->apply_condition("chanchu_poison", 70);
		ob->receive_wound("jing", damage/2);
		//          p = (int)ob->query("qi")*100/(int)ob->query("max_qi");
		//           msg += "( $n"+eff_status_msg(p)+" )\n";
		ob->set_temp("last_damage_from", "被"+me->name()+"杀");
	}
	else {
		msg +=HIY"毒雾来得虽快，$n飞身後跃之势却是更快，只听呼的一声,毒烟已被$n的劈空掌力荡开消失无迹。\n"NOR;
		ob->add("jingli", -10);
	}
	message_vision(msg, me, ob);
	if(!ob->is_killing(me->query("id"))) ob->kill_ob(me);
	this_object()->add("chanchu_poison",-1);
	return 1;
}
