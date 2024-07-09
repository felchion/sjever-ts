// daode-jing.c

inherit SKILL;

void create() { seteuid(getuid()); }

string type() { return "knowledge"; }

int valid_learn(object me) 
{
        int lvl;
        
        lvl = (int)me->query_skill("daode-jing", 1);
               
        if(!me->query("jiuyin/passed"))
                return notify_fail("你无法体会道德经的高深奥妙，学来也无用。\n");

        if(me->query("gender") == "无性" && lvl > 39)
                return notify_fail("你刚听一会儿，便觉心智迷糊，原来你没有阳刚之气，无法领会里面的阴阳变化之道。\n");

        return 1;
}

int practice_skill(object me)
{       
        return notify_fail("道德经只能靠学习来提高。\n");
}

