// 回风拂柳剑
// Modified by Lane@SJ 2005/4/15

inherit SKILL;
#include <ansi.h>
#include <skill_class.h>
#include <combat.h>

string query_class() {return "柔";}

mapping *action = ({
([
  "action" : "$N一式「清风袭月」，剑锋平指，剑势如风，一气呵成猛攻$n的$l",
  "lvl" : 0,
  "damage_type" : "刺伤"
]),
([
  "action" : "$N向前跃上一步，左手剑诀，右手$w使出一式「飘雪穿云」直刺$n的$l",
  "lvl" : 20,
  "damage_type" : "刺伤"
]),
([
  "action" : "$N手中$w斜指苍天，一式「千峰竞秀」，化出无数个剑尖，裹向$n的$l",
  "lvl" : 40,
  "damage_type" : "刺伤"
]),
([
  "action" : "$N一式「万流归宗」，$w自上而下缓缓划出一个大弧，突然从中向$n刺出",
  "lvl" : 60,
  "damage_type" : "刺伤"
]),
([
  "action" : "$N回身拧腰，一式「乌龙搅柱」，$w随身平划，点向$n的$l要穴",
  "lvl" : 80,
  "damage_type" : "刺伤"
]),
([
  "action" : "$N一式「大雁啼沙」，全身贴地平飞，身剑合一，闪电般刺向$n的$l",
  "lvl" : 100,
  "damage_type" : "刺伤"
]),
([
  "action" : "$N施出「进退龙游」，$w脱手飞出，如游龙般翱翔青宇，破空射向$n的$l",
  "lvl" : 120,
  "damage_type" : "刺伤"
]),
([
  "action" : "$N长啸一声，一式「天地鹤翔」，腾空飞起，全身迸出无数道雪亮剑芒，照耀天际",
  "lvl" : 150,
  "damage_type" : "刺伤"
]),
});

int valid_enable(string usage) 
{ 
  return usage == "sword" || usage == "parry"; 
}

int valid_learn(object me)
{
  object weapon;
  if (!objectp(weapon = me->query_temp("weapon"))
   || (string)weapon->query("skill_type") != "sword")
    return notify_fail("你使用的武器不对。\n");
  if ( me->query("max_neili") < 100)
    return notify_fail("你的内力不够。\n");

  if ( me->query_skill("linji-zhuang", 1) < 20)
    return notify_fail("你的临济十二庄火候太浅。\n");

  if ( me->query_skill("parry", 1) < 10)
    return notify_fail("你的基本招架火候太浅。\n");
  return 1;
}

mapping query_action(object me,object target)
{
  int i, j, level, con;

  level = (int)me->query_skill("huifeng-jian",1);
  con = (int)me->query_con();
  for(i = sizeof(action); i > 0; i--) {
    if(level > action[i-1]["lvl"]) {
      j = NewRandom(i, 20, level/5);
      return ([  
        "action": action[j]["action"],
        "lvl": action[j]["lvl"],
        "damage_type" : action[j]["damage_type"],
        "skill_class"   : ROU,
        "skill_damage"  : -20,
        "skill_attack"  : 30,
        "skill_parry"   : -20,
        "skill_dodge"   : 20,
        "skill_kill"    : 20,
        "skill_rush"    : 20,
        "skill_hurt"    : 0,
        "skill_GP"      : 2
      ]);
    }
  }
}

int practice_skill(object me)
{
  int lvl = me->query_skill("huifeng-jian", 1);
  int i = sizeof(action);

  while (i--) if (lvl == action[i]["lvl"]) return 0;
  if ((int)me->query("jingli") < 50)
    return notify_fail("你的体力不够练回风拂柳剑。\n");
  if ((int)me->query("neili") < 20)
    return notify_fail("你的内力不够练回风拂柳剑。\n");
  me->receive_damage("jingli", 40);
  me->add("neili", -15);
  return 1;
}

int ob_hit(object ob, object me, int damage)
{
  object weapon, weapon1;
  string msg;
  int skill, i;

  if( ob->query("env/invisibility") ) return 0;

  weapon1 = me->query_temp("weapon");

  if( me->query_skill("huifeng-jian", 1) < 150
   || me->query_skill_mapped("sword") != "huifeng-jian"
   || me->query_skill_mapped("parry") != "huifeng-jian" // 招架必须是回风拂柳剑
   || !weapon1
   || weapon1->query("skill_type") != "sword" ) return 0;

//  原来后发先至的判断，去除了反复判断
  if(me->is_busy() || damage < 100 ) return 0;

  skill = me->query_skill("huifeng-jian", 1);
  weapon = ob->query_temp("weapon");

  if ( me->query("neili") < ob->query("neili")/5 ) return 0;

  if(random(me->query_skill("parry")) > ob->query_skill("force")/2
  || !random(3) ) {
    msg = HIW"$N摸透了$n的武功套路，"+weapon1->name()+HIW"微侧，便即抢攻，竟不挡格$n的来招！！"NOR;
    message_vision(msg, me, ob);
    i = ob->query("qi");
    me->set_temp("jianjue", 1);
    me->add_temp("apply/pfm_attack",  80+skill/20);
    me->add_temp("apply/pfm_damage",  100+skill/10);
    me->add_temp("apply/damage",  180+skill/5);
    COMBAT_D->do_attack(me, ob, me->query_temp("weapon"), random(2)?1:3);
    me->add_temp("apply/damage",  -180-skill/5);
    me->add_temp("apply/pfm_damage",  -100-skill/10);
    me->add_temp("apply/pfm_attack", -80-skill/20);
    me->delete_temp("jianjue");
    if( i > ob->query("qi") ) return -damage/2;
    else return -damage;
  }
  return 0;
}

string perform_action_file(string action)
{
  return __DIR__"em/huifeng-jian/" + action;
}
