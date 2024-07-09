//段家剑法
//cool@SJ,990528
#include <ansi.h>
inherit SKILL;
#include <skill_class.h>

string query_class() {return "快";}

mapping *action = ({
([
  "action" : "$N左手捏了个剑诀，右手$w平平递出，一式「其利断金」直刺$n的前胸",
  "damage_type" : "刺伤",
  "lvl": 0,
]),
([
  "action" : "$N纵身向前，$w从一个想不到的角度突然弹出，一式「碧鸡报晓」直刺$n的左肋",
  "damage_type" : "刺伤",
  "lvl": 20,
]),
([
  "action" : "$N一招「金马腾空」，身形跃在半空，$w横削而出，直向$n的头部",
  "damage_type" : "刺伤",
  "lvl": 40,
]),
([
  "action" : "$N剑身斜挥，剑尖指向$n的腹部，一招「碧渊腾蛟」，自下而上剖向$n",
  "damage_type" : "刺伤",
  "lvl": 60,
]),
([
  "action" : "$N双脚离地，斜飞而出，$w射向$n的$l，好一招「笑指天南」",
  "damage_type" : "刺伤",
  "lvl": 80,
]),
([ 
  "action" : "$N长啸一声，横剑轻挥隐隐透出王道之意，正是段家不传之秘「逐鹿中原」，剑风罩住$n的全身",
  "damage_type" : "刺伤",
  "lvl": 100,
]),
});

int valid_enable(string usage) { return (usage == "sword") || (usage == "parry"); }

int valid_learn(object me)
{
  object weapon;
  if (!objectp(weapon = me->query_temp("weapon"))
   || (string)weapon->query("skill_type") != "sword")
    return notify_fail("你使用的武器不对。\n");
  if (me->query("max_neili") < 100)
    return notify_fail("你的内力不够。\n");
  if ( ! me->query_skill("kurong-changong", 1))
  if (me->query_skill("qiantian-yiyang", 1) < 20)
    return notify_fail("你的乾天一阳火候太浅。\n");
  return 1;
}

mapping query_action(object me, object weapon)
{
  int i, level;
  string msg;
  level   = (int) me->query_skill("duanjia-jianfa",1);

  if(me->query_temp("fenglei")){
    switch(me->query_temp("fenglei")){
      case 4: msg = HIW"$N施展出「天龙翔」，一大步跃起在空中，身体围绕着$n不停旋转，连续由四个方位向$n刺出四剑，快捷无比" NOR;break;
      case 3: msg = HIW"$N借着$n的劲风在空中一荡，双手握剑，一剑刺出，犹如鹰击长空，＂嗡＂的一声，剑光象一道闪电般刺向$n" NOR;break;
      case 2: msg = HIW"$N左掌或拍或劈，吸引$n的注意，右手剑转无形，划出数道剑气，紧接着一剑横拍，无数剑风袭向$n的全身" NOR; break;
      case 1: msg = HIW"$N身体下沉，突然左手一掌向地拍出，头下脚上，身子借着掌力向空中一飘，又是快速无比的刺出两剑，直向$n"NOR;break;
      default: msg = HIW"$N身体下沉，突然左手一掌向地拍出，头下脚上，身子借着掌力向空中一飘，又是快速无比的刺出两剑，直向$n" NOR; break;
    }
    me->add_temp("fenglei", -1);
    return ([
      "action"        : msg,
      "damage"        : 130+random(140), 
      "damage_type"   : "刺伤",
      "force"         : 250+random(300),
      "skill_name"    : "柳絮剑法",
      "skill_class"   : KUAI,
      "skill_damage"  : 10,
      "skill_attack"  : 30,
      "skill_parry"   : 0,
      "skill_dodge"   : 0,
      "skill_kill"    : 0,
      "skill_rush"    : 20,
      "skill_hurt"    : 20,
      "skill_GP"      : 3
    ]);
  }
  for(i = sizeof(action); i > 0; i--) {
    if(level > action[i-1]["lvl"]) {
      return ([
        "action"        : action[NewRandom(i, 20, level/5)]["action"],
        "damage"        : 80+random(140),
        "damage_type"   : action[NewRandom(i, 20, level/5)]["damage_type"],
        "force"         : 200+random(350),
        "skill_class"   : KUAI,
        "skill_damage"  : -10,
        "skill_attack"  : 30,
        "skill_parry"   : -10,
        "skill_dodge"   : 20,
        "skill_kill"    : 0,
        "skill_rush"    : 20,
        "skill_hurt"    : 0,
        "skill_GP"      : 2
      ]);
    }
  }
}

int practice_skill(object me)
{
  int lvl = me->query_skill("duanjia-jianfa", 1);
  int i = sizeof(action);

  while (i--) if (lvl == action[i]["lvl"]) return 0;

  if ((int)me->query("jingli") < 50)
    return notify_fail("你的体力不够练段家剑法。\n");
  if((int)me->query("neili") < 20)
    return notify_fail("你的内力不够练段家剑法。\n");
  if ( me->query("class") == "bonze") {
    me->receive_damage("jingli", 30);
    me->add("neili", -10);
  }
  else {
    me->receive_damage("jingli", 40);
    me->add("neili", -15);
  }
  return 1;
}

string perform_action_file(string action)
{
  return __DIR__"tls/duanjia-jianfa/" + action;
}
