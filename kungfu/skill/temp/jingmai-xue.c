// jingmai-xue.c 经脉学

#include <ansi.h>
inherit SKILL;
void create() { seteuid(getuid()); }
string type() { return "knowledge"; }

int valid_learn(object me)
{
	if((int)me->query_skill("jingmai-xue", 1) >= 200 )
		return notify_fail("你不能再学习经脉学了。\n");
	return notify_fail("经脉学只能通过研习医学书籍来提高。\n");
}

int practice_skill(object me)
{
	return notify_fail("经脉学只能靠研读(study)来提高。\n");
}

void skill_improved(object me)
{
	switch (me->query_skill("jingmai-xue",1)) {
	case 10:
		tell_object(me,HIY"你觉得对手太阴肺经完全了解了。\n"NOR);break;
	case 20:
		tell_object(me,HIB"你觉得对手阳明大肠经完全了解了。\n"NOR);break;
	case 30:
		tell_object(me,HIY"你觉得对足阳明胃经完全了解了。\n"NOR);break;
	case 40:
		tell_object(me,HIB"你觉得对足太阴脾经完全了解了。\n"NOR);break;
	case 50:
		tell_object(me,HIY"你觉得对手少阴心经完全了解了。\n"NOR);break;
	case 60:
		tell_object(me,HIB"你觉得对手太阳小肠经完全了解了。\n"NOR);break;
	case 70:
		tell_object(me,HIY"你觉得对足太阳膀胱经完全了解了。\n"NOR);break;
	case 80:
		tell_object(me,HIB"你觉得对足少阴肾经完全了解了。\n"NOR);break;
	case 90:
		tell_object(me,HIB"你觉得对手厥阴心包经完全了解了。\n"NOR);break;
	case 100:
		tell_object(me,HIY"你觉得对手少阳三焦经完全了解了。\n"NOR);break;
	case 110:
		tell_object(me,HIY"你觉得对足少阳胆经完全了解了。\n"NOR);break;
	case 120:
		tell_object(me,HIB"你觉得对足厥阴肝经完全了解了。\n"NOR);break;
	case 130:
		tell_object(me,HIG"你觉得对阳矫脉完全了解了。\n"NOR);break;
	case 140:
		tell_object(me,HIW"你觉得对阴矫脉完全了解了。\n"NOR);break;
	case 150:
		tell_object(me,HIG"你觉得对阳维脉完全了解了。\n"NOR);break;
	case 160:
		tell_object(me,HIW"你觉得对阴维脉完全了解了。\n"NOR);break;
	case 170:
		tell_object(me,HIG"你觉得对冲脉完全了解了。\n"NOR);break;
	case 180:
		tell_object(me,HIW"你觉得对督脉完全了解了。\n"NOR);break;
	case 190:
		tell_object(me,HIG"你觉得对带脉完全了解了。\n"NOR);break;
	case 200:
		tell_object(me,HIR"你觉得对任脉完全了解了。\n");
		if(! me->query("jingmai")){
			tell_object(me,"你对人全身的经脉了如指掌，内功不知不觉中已大进！\n"NOR);
			me->add("max_neili",50 + random(30));
			me->set("jingmai", 1);
		}
	break;
	}
}
