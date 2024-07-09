// zhihe.c 纸鹤
// By River@SJ 
inherit COMBINED_ITEM;
#include <ansi.h> 

void create()
{
	set_name(HIW"纸鹤"NOR, ({ "zhihe", "he" }));
	if (clonep())
		set_default_object(__FILE__);
	else {
		set("long", "一只代表思念的纸鹤，非常精致，据说收集到一千只可以带来好运。\n");
		set("base_value", 500);
		set("value", 500);
		set("base_unit", "只");
		set("unit", "些");
		set("base_weight", 5);
		set("no_drop", 1);
		set("no_give", 1);
		set("no_get", 1);
		set("material", "paper");
	}
	set_amount(1);
}

void init()
{
	add_action("do_give", "song");
	add_action("do_tear", ({"si", "tear"}));
}

int do_give(string arg)
{
	string target, item;
	object me, obj, who;

	me = this_player();
	obj = this_object();

	if (!arg) return 0;

	if( sscanf(arg, "%s to %s", item, target)==2
	 || sscanf(arg, "%s %s", target, item)==2);
	else return notify_fail("指令格式 : song <物品名称> to <某人> 或 : song <某人> <物品名称>\n");
	
	if ( item != "zhihe")
		return notify_fail("指令格式 : song <物品名称> to <某人> 或 : song <某人> <物品名称>\n");

	if (!objectp(who = present(target, environment(me))) || !living(who))
		return notify_fail("这里没有这个人。\n");

	if ( who == me) return notify_fail("给自己？\n");

	if ( me->is_busy()) return notify_fail("你正忙着呢。\n");

	if ( who->query("env/no_accept"))
		return notify_fail("对方现在不接受任何东西。\n");

	if ( who->query_temp("make_zhihe/get"))
		return notify_fail(who->query("name")+"已经有纸鹤了。\n");

	if ( query_amount() < 1000)
		return notify_fail("你还没有凑齐一千只纸鹤呢。\n");
	
	message_vision("$N给了$n一些"+obj->query("name")+"。\n", me, who);
	me->delete_temp("make_zhihe");
	who->set_temp("make_zhihe/get", 1);
	obj->move(who);
	return 1;
}

int do_tear(string arg)
{
	object me = this_player();
	
	if(!id(arg)) return 0;
	
	message_vision(HIW"$N呆呆地站着，将手中的纸鹤看了又看，突然一咬牙，几下将纸鹤撕成碎片，\n"+
			"看着片片碎鹤在风中飞舞，终至泪水夺眶而出。\n"NOR, me);
	EMOTE_D->do_emote(me,"cry", me->query("id"));
	me->delete_temp("make_zhihe");
	me->delete_temp("zhihe");
	call_out("dest", 1, this_object());
	return 1;
}

void dest(object obj)
{
	if (!obj) return;
	destruct(obj);
}
