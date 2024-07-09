// Skill: Emei-JiuYang.c 峨眉九阳功
// Date : AHA 97/06/22

inherit FORCE;

int valid_enable(string usage) { return usage == "force"; }

int valid_learn(object me)
{
  int level = (int)me->query_skill("emei-jiuyang", 1);
  int changquan = (int)me->query_skill("emei-changquan", 1);
  
  int t = 1, j;
  for (j = 1; j < level / 10; j++) t *= 2;

  if ( me->query("gender") == "无性" )
     return notify_fail("峨眉九阳功练的是浩然正气，以公公无根无性之身，
                         如何修得！\n");

  if ((int)me->query_skill("force", 1) < 10)
     return notify_fail("你的基本内功火候还不够。\n");

  if ( changquan < 20 ) 
     return notify_fail("你的峨眉长拳修为太低，无法领悟峨眉九阳功。\n");  

  if (level > 10 && (int)me->query("shen") < t * 100) 
     return notify_fail("你的侠义正气太低了。\n");

  return 1;
}

int practice_skill(object me)
{
  return notify_fail("峨眉九阳功只能用学(learn)的来增加熟练度。\n");
}

string exert_function_file(string func)
{
  return __DIR__"emei-jiuyang/" + func;
}

