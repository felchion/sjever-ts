// buddhism.c

inherit SKILL;

void create() { seteuid(getuid()); }

string type() { return "knowledge"; }

int valid_learn(object me) 
{
	if( (int)me->query("guilty") > 0 )
		return notify_fail("你屡犯僧家数戒，尘俗之心太重，无法修炼禅宗心法。\n");
        if( (int)me->query_skill("buddhism", 1) >= 200 )
        return notify_fail("你不能再学习禅宗心法了。\n");
	return 1;
}

int practice_skill(object me)
{	
	return notify_fail("禅宗心法只能靠学(learn)来提高。\n");
}
