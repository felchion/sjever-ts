// Skill: Emei-ChangQuan.c 峨眉长拳
// Date : AHA 97/06/22

inherit SKILL;

mapping *action = ({
(["action" : "$N身形凝重，左拳化掌和右拳相接，同时从胸前正中推出，"
               "一招「起手式」击向$n的胸口",
  "force" : 80,
  "dodge" : 20,
  "skill_name" : "起手式",
  "damage_type" : "瘀伤"
]),
(["action" : "$N步法灵动，使出一招「千斤腿」，右脚脚尖勾起蹬踢$n的$l",
  "force" : 100,
  "dodge" : 15,
  "skill_name" : "千斤腿",
  "damage_type" : "瘀伤"
]),
(["action" : "$N左手握拳向左上挑格，右拳前冲，一招「当心锤」击向$n的胸口",
  "force" : 110,
  "dodge" : 10,
  "skill_name" : "当心锤",
  "damage_type" : "瘀伤"
]),
(["action" : "$N步法一变，双拳猛地从下往上抖打，一招「螳螂手」击向$n的$l",
  "force" : 120,
  "dodge" : 15,
  "skill_name" : "螳螂手",
  "damage_type" : "瘀伤"
]),
(["action" : "$N重心下沉，稳住弓步，使出「顺风拳」，左拳下压，右拳勾击$n的$l",
  "force" : 130,
  "dodge" : 20,
  "skill_name" : "顺风拳",
  "damage_type" : "瘀伤"
]),
(["action" : "$N左膝微屈，右脚弹起，一招「日月脚」闪电般地同时击向$n的$l",
  "force" : 140,
  "dodge" : 25,
  "skill_name" : "日月脚",
  "damage_type" : "瘀伤"
]),
(["action" : "$N一个错步闪开$n的攻击，接着一招「猴抢桃」，双拳分两路"
             "同时击向$n的$l",
  "force" : 150,
  "dodge" : 15,
  "skill_name" : "猴抢桃",
  "damage_type" : "瘀伤"
]),
(["action" : "$N身形一变，转到$n的身后，一招「霸王手」，右拳撩击$n的$l",
  "force" : 160,
  "dodge" : 25,
  "skill_name" : "霸王手",
  "damage_type" : "瘀伤"
]),
(["action" : "$N左脚蹬地跳起，一招「鸟分翅」，双拳化掌，分别击向$n的$l",
  "force" : 140,
  "dodge" : 15,
  "skill_name" : "鸟分翅",
  "damage_type" : "瘀伤"
]),
(["action" : "$N稳住身形，左手虚晃，一招「劈山拳」，右拳向前猛击$n的头部",
  "force" : 170,
  "dodge" : 10,
  "skill_name" : "劈山拳",
  "damage_type" : "瘀伤"
]),
(["action" : "$N猛地跳起，左脚落在$n右脚之前，一招「龙摆尾」，右拳甩起击向"
             "$n的$l",
  "force" : 160,
  "dodge" : 20,
  "skill_name" : "龙摆尾",
  "damage_type" : "瘀伤"
]),
(["action" : "$N马步微沉，力贯拳背，一招「连四拳」，双拳如风，从左右连续"
             "击向$n的$l",
  "force" : 180,
  "dodge" : 30,
  "skill_name" : "连四拳",
  "damage_type" : "瘀伤"
])
});

int valid_enable(string usage) { return usage=="cuff" || usage=="parry"; }

int valid_learn(object me)
{
  if(me->query_temp("weapon") || me->query_temp("secondary_weapon"))
     return notify_fail("练峨眉长拳必须空手。\n");
  return 1;
}

string query_skill_name(int level)
{
  return action[random(sizeof(action))]["skill_name"];
}

mapping query_action(object me, object weapon)
{
  int max_neili;
  float factor;
 
  mapping act = action[random(sizeof(action))];
  max_neili = me->query("max_neili");
  if(max_neili <= 100) return act;
  factor = sqrt(sqrt((float)max_neili/100.0));
  act["force"] = (int)((float)act["force"]*factor);
  act["dodge"] = (int)((float)act["dodge"]*factor);
  return act;
}

int practice_skill(object me)
{
  int level;
  if(me->query_temp("weapon") || me->query_temp("secondary_weapon"))
     return notify_fail("练峨眉长拳必须空手。\n");
                  
  if((int)me->query("qi") < 30)
     return notify_fail("你的体力太低了。\n");

  level = (int)me->query_skill("emei-changquan", 1)/10;
  if(level >= 3){
    if((int)me->query("neili") < 20)
       return notify_fail("你的内力不够练峨眉长拳。\n");
    me->add("neili", -8);
  }
  me->receive_damage("qi", 20);
  return 1;
}

string perform_action_file(string action)
{
  return __DIR__"emei-changquan/" + action;
}

