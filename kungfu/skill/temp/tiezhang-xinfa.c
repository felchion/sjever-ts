// tiezhang-xinfa.c?铁掌心法
inherit SKILL;

void create() { seteuid(getuid()); }

string type() { return "knowledge"; }

int valid_learn(object me) 
{
        if( (int)me->query_skill("tiezhang-xinfa", 1) >= 200 )
             return notify_fail("你不能再学习铁掌心法了。\n");
        return 1;
}
int practice_skill(object me)
{
        return notify_fail("铁掌心法只能靠学(learn)来提高。\n");
}


