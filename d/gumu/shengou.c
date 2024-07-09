// Modify By River 98/08
// Modify By YUJ@SJ 2001-0609 限制通缉犯
#include <ansi.h>
inherit ROOM;

void create()
{
	set("short", HIG"深沟"NOR);
	set("long", @LONG
匆匆地穿过树林，眼前出现一道深沟(gou)，不知是深谷还是山溪的所在，
此外已无去路。低头一看，地下立着一块石碑(shibei)。
LONG
     );

	set("exits", ([
	    "west" : __DIR__"shulin2",
	]));

	set("objects", ([
		CLASS_D("gumu") + "/sun" : 1,
	]));
	set("item_desc", ([
		"gou" : HIC"深沟里烟雾弥漫，隐约中寒气凛人，间或似乎传来飘渺人声，真是世外高人超\n"+
			"然凡尘的好所在，你悠然神往，不自禁产生一种跃身跳(tiao)下的冲动。\n"NOR,
		"shibei" : HIR"\t\t终南山活死人墓，外人莫入！\n"NOR
	]));

	set("outdoors", "古墓");

	setup();
}

void init()
{
	add_action("do_jump", "tiao");
}

int can_betray(object me)
{
	mapping skills = me->query_skills();

	if (sizeof(skills) < 1)
		return 1;
	foreach (string skill in keys(skills))
		if (SKILL_D(skill)->valid_enable("force"))
			return 0;
	return 1;
}

int do_jump(string arg)
{
	mapping fam;
	object me = this_player();
	object *inv;

	fam = me->query("family");
	if ( arg =="gou") {
		if (me->is_busy())
			return notify_fail("你正忙着呢。\n");
		if ( fam && fam["family_name"] == "全真派") {
			write(HIR"祖师遗训你忘了吗？！\n"NOR);
			me->unconcious();
			return 1;
		}
		if( me->query("gender") == "无性") {
			write(HIR"你这种不男不女的东西还想进古墓？\n"NOR);
			me->unconcious();
			return 1;
		}
		if (me->query_condition("killer"))
			return notify_fail("你还是了结完江湖恩怨再说吧。\n");
		if ( fam && fam["family_name"] != "古墓派" && !can_betray(me))
			return notify_fail(HIR"\t\t古墓重地，闲人莫入！\n"NOR);
		if (!fam || fam["family_name"] != "古墓派") {
			if (me->query("expell"))
				return notify_fail(HIR"\t\t古墓重地，闲人莫入！\n"NOR);
//			if (me->query_int() < 22)
//				return notify_fail(HIR"你悟性欠佳, 恐怕今生难圆双剑合壁之美梦。\n"NOR);
			if (me->query_skill("literate", 1) < 20)
				return notify_fail(HIR"你的读书写字太低，入古墓无法领悟。\n"NOR);
//			if (me->query_dex() < 15 && me->query("gender") == "男性")
//				return notify_fail(HIR"你身法太低，可能学不了古墓飘零的功夫。\n"NOR);
//			if (me->query_dex() < 18 && me->query("gender") == "女性")
//				return notify_fail(HIR"你身法太低，可能学不了古墓飘零的功夫。\n"NOR);
			if (me->query("per") < 20)
				return notify_fail(HIR"你天生容貌欠佳, 恐怕今生难圆双剑合壁之美梦。\n"NOR);
		}
		inv = filter_array(deep_inventory(me), (: userp :));
		if (sizeof(inv)) {
			write(HIR"你负重太多，脚下一个绊拴，摔倒在地。\n"NOR);
			me->unconcious();
			return 1;
		}
		tell_room(this_object(), HIM+me->name() + "微一提气，凭空纵高丈余，衣诀飘飘，身姿潇洒地落向山谷中。\n", me);
		me->move(__DIR__"rukou");
		message_vision(HIY"$N纵身跳下深沟，拿捏恰到好处，正好落在古墓入口前。\n"NOR, me);
		return 1;
	}
	return notify_fail("你往哪里跳？\n");
}
