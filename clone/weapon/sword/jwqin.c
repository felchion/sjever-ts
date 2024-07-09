//jiaowei qin 焦尾琴

#include <weapon.h>
#include <ansi.h>

inherit SWORD;

void create()
{
	set_name(MAG"焦尾琴"NOR,({"jiaowei qin","qin", "jiaowei"}));
	set_weight(10000);
	if(clonep())
		set_default_object(__FILE__);
	else{
		set("unit","张");
		set("value",3000);
		set("material","steel");
		set("unique", 1);
		set("rigidity", 5);
		set("treasure",1);
		set("long","相传这便是当年蔡文姬从火中所救出的焦木做成的美琴，琴的尾端仍可见焦黑色。\n");
		set("wield_msg",HIY"$N伸手轻轻一挥，一张$n"HIY"便已出现在$N手中。\n"NOR);
		set("unwield_msg",HIY"$N十指一拂，琴声曳然而止。$N收起$n"HIY"，放进背着的包袱里。\n"NOR);
	}
	init_sword(60);
	setup();
}

void init()
{
	if (environment() == this_player())
		add_action("do_ba", "ba");
}

int flag = 0;

int do_ba(string arg)
{
	object me = this_player();
	object ob;

	if (arg != "sword" && arg != "jian")
		return 0;
	if (query("equipped"))
		return notify_fail("你必须先把琴放下来。\n");
	if (flag || !(ob  = unew(BINGQI_D("sword/qz-jian")))) {
		flag = 1;
		return notify_fail("琴中已空无一物。\n");
	}
	ob->move(me);
	message_vision("$N放下背上的"+name()+"，从琴底抽出一把宝剑，但见青光闪闪，寒气逼人。\n",me);
	destruct(this_object());
	return 1;
}
