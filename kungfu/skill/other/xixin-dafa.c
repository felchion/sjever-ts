// xixin-dafa.c 吸心大法

inherit FORCE;

int valid_enable(string usage) { return usage == "force"; }

int valid_learn(object me)
{
        int i = (int)me->query_skill("xixin-dafa", 1);
        int t = 1, j;
        for (j = 1; j < i / 10; j++) t*= 2;
        if ((int)me->query_skill("force", 1) < 10)
                return 
           notify_fail("你的基本内功火候不足，不能学吸心大法。\n");
        if (i > 10 && (int)me->query("shen") > t * 50)
               return notify_fail("学吸心大法要杀人来调节自己的心神，多杀功夫高，现在你可杀的不多呀！害怕了？\n");
        if ( me->query_skill("buddhism",1) || me->query_skill("tao",1) )
                return notify_fail("你练了别的心法是不能学习吸心大法的，只有放弃学习别的心法。\n");
        if ( me->query_skill("hunyuan-yiqi",1) 
        || me->query_skill("huntian-qigong",1) 
        || me->query_skill("taiji-shengong",1) )
                return notify_fail("你不先放弃正派内功，怎能学我们日月教的吸心大法！\n");
        return 1;
 }

int practice_skill(object me)
{
        return notify_fail("吸心大法只能用学(xue)的增加熟练度。\n");
}

string exert_function_file(string func)
{
        return __DIR__"other/xixin-dafa/" + func;
}            
