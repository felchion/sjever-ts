// Skill: Emei-JianFa.c 峨眉剑法
// Date:  AHA 97/06/24

inherit SKILL;

mapping *action = ({
(["action" : "$N左手剑诀向后斜指，右手$w挽了个剑花，一招「虚式分金」，削向"
             "$n的$l",
  "force" : 200,
  "dodge" : 25,
  "skill_name" : "虚式分金",
  "lvl" : 0,
  "damage" : 60,
  "damage_type" : "刺伤"
]),
(["action" : "$N左膝微屈，右脚上前，剑身微颤，一招「顺水推舟」，削向$n的$l",
  "force" : 220,
  "dodge" : 30,
  "skill_name" : "顺水推舟",
  "lvl" : 9,
  "damage" : 70, 
  "damage_type" : "刺伤"
]),
(["action" : "$N左手剑指点在右手腕间，气贯剑身，一招「铁索横江」，推送而上，"
             "$w削向$n的$l",
  "force" : 240,
  "dodge" : 35,
  "skill_name" : "铁索横江",
  "lvl" : 19,
  "damage" : 80,
  "damage_type" : "刺伤"
]),
(["action" : "$N向前迈上一步，一招「黑沼灵狐」，$w化作数十柄$w，突然闪电般"
             "地刺向$n的$l",
  "force" : 270,
  "dodge" : 25,
  "skill_name" : "黑沼灵狐",
  "lvl" : 29,
  "damage" : 90, 
  "damage_type" : "刺伤"
]),
(["action" : "$N气凝剑尖，一招「月落西山」，$w自上而下划了个半弧，剑尖闪烁，"
             "直刺$n的$l",
  "force" : 300,
  "dodge" : 30,
  "skill_name" : "月落西山",
  "lvl" : 39,
  "damage" : 100, 
  "damage_type" : "刺伤"
]),
(["action" : "$N步法一变，转到$n的身后，手中$w自左腋下穿出，一招「烟斜雾横」"
             "，刺向$n的$l",
  "force" : 340,
  "dodge" : 40,
  "skill_name" : "烟斜雾横",
  "lvl" : 49,
  "damage": 120,
  "damage_type" : "刺伤"
]),
(["action" : "$N右脚蹬地跳起，手中$w在空中舞成一朵剑花，一招「沙草晨牧」，"
             "刺向$n的$l",
  "force" : 380,
  "dodge" : 50,
  "skill_name" : "沙草晨牧",
  "lvl" : 59,
  "damage": 140,
  "damage_type" : "刺伤"
]),
(["action" : "$N手中$w自左至右，又自右至左连晃九下，一招「轻罗小扇」，刺向"
             "$n的九处大穴",
  "force" : 420,
  "dodge" : 60,
  "skill_name" : "轻罗小扇",
  "lvl" : 69,
  "damage": 160,
  "damage_type" : "刺伤"
]),
(["action" : "$N将内力注入剑身，手中$w幻成满天剑影，一招「千峰竞秀」，以排"
             "山倒海之势，刺向$n的$l",
  "force" : 460,
  "dodge" : 50,
  "skill_name" : "千峰竞秀",
  "lvl" : 79,
  "damage" : 180,  
  "damage_type" : "刺伤"
]),
(["action" : "$N双手握剑，只听见$w在内力激荡下发出阵阵龙吟之声，一招「金顶"
             "佛光」，刺向$n的$l",
  "force" : 600,
  "dodge" : 45,
  "skill_name" : "金顶佛光",
  "lvl" : 99,
  "damage" : 210, 
  "damage_type" : "刺伤"
])
});

int valid_enable(string usage) 
  { return (usage == "sword") || (usage == "parry"); }

int valid_learn(object me)
{
  if ((int)me->query_skill("emei-jiuyang", 1) < 40)
     return notify_fail("你的峨眉九阳功火候太浅，无法领会高深的峨眉剑法。\n");
  if ((int)me->query("max_neili") < 300)
     return notify_fail("峨眉武学以内力为根本，你的内力太低了。\n");
  if ((int)me->query_skill("jinding-mianzhang", 1) < 50)
     return notify_fail("学习峨眉功夫，需要扎实的基本功。\n"
                        "你的金顶绵掌火候太浅，无法领会高深的峨眉剑法。\n");
  if ((int)me->query("shen") < 4000)
     return notify_fail("你的侠义正气太低，不能传授峨眉剑法。\n");
  if ((int)me->query("combat_exp") < 15000)
     return notify_fail("你的实战经验太差，无法领会高深的峨眉剑法。\n");
  return 1;
}

string query_skill_name(int level)
{
  int i = sizeof(action);

  while (i--)
    if(level >= action[i]["lvl"]) return action[i]["skill_name"];
}

mapping query_action(object me, object weapon)
{
  int i, level, max_neili;
  float factor;
  mapping act;

  level = (int) me->query_skill("emei-jianfa",1);

  for(i = sizeof(action); i > 0; i--)
     if(level > action[i-1]["lvl"]){
        act = action[NewRandom(i, 20, level/5)];
        max_neili = me->query("max_neili") - 400;
        if(max_neili > 100){ 
           factor = sqrt(sqrt(((float)max_neili)/100.0));
           act["force"] = (int)((float)act["force"]*factor);
           act["dodge"] = (int)((float)act["dodge"]*factor);
           act["damage"]= (int)((float)act["damage"]*factor);
        }
        return act;
     }
}

int practice_skill(object me)
{
  object weapon;

  if (!objectp(weapon = me->query_temp("weapon")) || 
       (string)weapon->query("skill_type") != "sword")
	  return notify_fail("你使用的武器不对。\n");
  if(me->query_skill("emei-jiuyang")*2 < me->query_skill("emei-jianfa")*3)
    return notify_fail("你的峨眉九阳功火候不足，无法领会峨眉剑法。\n");
  if((int)me->query("qi") < 100)
    return notify_fail("你的体力不够练峨眉剑法。\n");
  if((int)me->query("neili") < 100)
    return notify_fail("你的内力不足，不能练峨眉剑法。\n");
  if((int)me->query("jingli") < 50)
    return notify_fail("你的精力太差了，不能练习峨眉剑法。\n");
  me->receive_damage("qi", 50);
  me->receive_damage("jingli", 30); 
  me->add("neili", -20);
  return 1;
}

string perform_action_file(string action)
{
  return __DIR__"emei-jianfa/" + action;
}

