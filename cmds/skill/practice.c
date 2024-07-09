// practice.c

#include <ansi.h>

inherit F_CLEAN_UP;

int main(object me, string arg)
{
	object where = environment(me);
	string skillname, sname;
	int skill, skill_basic, cnt = 0;
	int improve;

	seteuid(getuid());

	if (where->query("pigging"))
		return notify_fail("你还是专心拱猪吧！\n");
	if ( me->is_busy() )
		return notify_fail("你现在正忙着呢。\n");
	if ( me->is_exert() || me->is_perform() )
		return notify_fail("你正使用着某种特殊技能呢，怎能分心两用？\n");
	if( me->is_fighting() )
		return notify_fail("你已经在战斗中了，学一点实战经验吧。\n");

	if( !arg ) 
		return (__DIR__"enable")->main(me, arg);

	if (where->query("sleep_room"))
		return notify_fail("卧室不能练功，会影响别人休息。\n");

	sscanf(arg, "%s %d", arg, cnt);
	//      if (!random(2)) return notify_fail("使用 lian|practice skill 次数 的格式会方便一些。\n");
	if (arg == "parry")
		return notify_fail("你不能通过基本招架来练习。\n");
	if( !stringp(skillname = me->query_skill_mapped(arg)) )
		return notify_fail("你只能练习用 enable 指定的特殊技能。\n");

	skill_basic = me->query_skill(arg, 1);
	skill = me->query_skill(skillname, 1);

	if( skill < 1 )
		return notify_fail("你好像还没有学过这项技能吧？最好先去请教别人。\n");
	if( skill_basic < 1 )
		return notify_fail("你对这方面的技能还是一窍不通，最好从先从基本学起。\n");
	if (!(skill_basic*11 > skill*10))
		return notify_fail("你的基本功火候未到，必须先打好基础才能继续提高。\n");

	notify_fail("你现在不能练习这项技能。\n");
	if( !SKILL_D(skillname)->valid_learn(me) ) return 0;

	notify_fail("你必须通过学习才能练新的一招。\n");
	if( (improve = SKILL_D(skillname)->practice_skill(me)) ) {
		if(improve == 1) improve = me->query_skill("literate")/5 +1;
		//工匠学武控制
		improve = to_int(improve*me->query("int")/15.0);
	//	if(me->worker_skill("highest")>350) improve = improve*7/10;
	//	else if(me->worker_skill("highest")>250) improve = improve*8/10;
	//	if(me->worker_skill("limit")>600) improve = improve*8/10;
		////
		if (!random(3)) tell_room(environment(me),
			me->name() +
			"正在专心练习" +
			to_chinese(skillname) + "。\n", ({ me }));
			write ("你" + ((cnt == 1)?"试着":"反复") + "练习" + to_chinese(skillname));
			if (stringp(sname = SKILL_D(skillname)->query_skill_name(random(me->query_skill(skillname,1)))))
			write ("中「" + sname + "」这一招");
			write( "，获得了" + ((cnt == 1)?"少许":"不少") + "进步！\n");
			me->improve_skill(skillname,
			improve,
			skill_basic > skill? 0: 1);
		while (--cnt) {
			if (SKILL_D(skillname)->practice_skill(me)) {
				me->improve_skill(skillname,improve,skill_basic*11 > skill*10 ? 0: 1);
			}
			else
			return 1;
		}
		return 1;
	}
	return 0;
}

int help(object me)
{
        write(@TEXT
指令格式：practice|lian <技能种类> <次数>

这个指令让你练习某个种类的技能，这个技能必须是经过 enable 的专业技能。

如果你对这方面的基本技能够高，可以经由练习直接升级，而且升级的上限只跟
你基本技能的等级有关，换句话说，勤加练习是使你的所学「青出于蓝胜于蓝」
的唯一途径，当然，在这之前你必须从实际运用中获得足够的经验以提升你的基
本技能。
如果不指定次数，表示练到不能练为止。
TEXT
        );
        return 1;
}
