// 玉箫剑法
// thd 新年微调 by Lane@SJ 2005.2.1

#include <ansi.h>
#include <skill_class.h>
inherit SKILL;

string query_class() {return "妙";} 

mapping *action = ({
	([
		"action" : "$N一振手中的$w，笔直向前递出，剑尖乱颤，正是一招「棹歌中流」，径取中锋，直袭$n的$l",
		"skill_name" : "棹歌中流",
		"lvl" : 0,
		"damage_type" : "刺伤"
	]),
	([
		"action" : "$N向前跨上一步，手中$w蓦地向上一挑，一式「山外清音」径指$n的咽喉",
		"skill_name" : "山外清音",
		"lvl" : 20,
		"damage_type" : "刺伤"
	]),
	([
		"action" : "$N一招「金声玉振」，手中$w凭空虚刺了几下，剑身上寒光点点，刹时分袭$n的全身要害",
		"skill_name" : "金声玉振",
		"lvl" : 40,
		"damage_type" : "刺伤"
	]),
	([
		"action" : "$N使出「响隔楼台」，$w斜指向上，剑锋直出却又抑扬，忽缓忽急地刺向$n的$l",
		"skill_name" : "响隔楼台",
		"lvl" : 60,
		"damage_type" : "割伤"
	]),
	([
		"action" : "$N一招「斜阳音袅」，剑意绵绵，似有波澜，剑光幻化做水面残阳，$n只一呆间，已经被剑意笼罩",
		"skill_name" : "斜阳音袅",
		"lvl" : 80,
		"damage_type" : "刺伤"
	]),
	([
		"action" : "$N手中$w虚晃，倏然连劈了几下，自上而下分袭$n的正面及两侧，正是一招「月落曲惶」",
		"skill_name" : "月落曲惶",
		"lvl" : 100,
		"damage_type" : "割伤"
	]),
	([
		"action" : "$N左足一点，身形后撤，忽然又纵身向前，剑意如音缈九天，一招「清箫万里」，$w轻飘飘地直逼$n",
		"skill_name" : "清箫万里",
		"lvl" : 120,
		"damage_type" : "割伤"
	]),
	([
		"action" : "$N跃起丈余，手中$w画了个弧线，斜斜划下，剑光匝地，一招「狂歌九天」，罩向$n的$l",
		"skill_name" : "狂歌九天",
		"lvl" : 140,
		"damage_type" : "割伤"
	]),
	([
		"action" : "$N横剑就唇，使出「凤曲长鸣」，$w轻颤有声，呜咽作响，忽然快如闪电般划向$n的右耳",
		"skill_name" : "凤曲长鸣",
		"lvl" : 160,
		"damage_type" : "割伤"
	]),
	([
		"action" : "$N回身拧腰，右手双指捏了个指诀，左手挥$w向后反撩而上，一式「箫史乘龙」，剑势如匹练般撩向$n的$l",
		"skill_name" : "箫史乘龙",
		"lvl" : 180,
		"damage_type" : "刺伤"
	]),
});

int valid_enable(string usage) { return (usage == "sword") || (usage == "parry"); }

int valid_learn(object me)
{
  object weapon;

  if (!(weapon = me->query_temp("weapon")))
    return notify_fail("空手时无法练玉箫剑法。\n");
  if ((string)weapon->query("skill_type") != "sword" 
  && (string)weapon->query("skill_type") != "xiao" )
    return notify_fail("你使用的武器不对，无法练玉箫剑法。\n");
  if ( me->query("max_neili") < 60)
    return notify_fail("你的内力不够。\n");
  if ( me->query_skill("bihai-chaosheng", 1) < 12)
    return notify_fail("你的碧海潮生功火候太浅。\n");
  if (me->query_skill_mapped("force") != "bihai-chaosheng")
    return notify_fail("修习玉箫剑法必须有碧海潮生功配合。\n");
  return 1;
}

string query_skill_name(int level)
{
  int i;
  for(i = sizeof(action)-1; i >= 0; i--)
  if(level >= action[i]["lvl"])
    return action[i]["skill_name"];
}

 

mapping query_action(object me, object weapon)
{
  int i, j, level;


  level = (int) me->query_skill("yuxiao-jian",1);
  for(i = sizeof(action); i > 0; i--) {
    if(level > action[i-1]["lvl"]) {
      j = NewRandom(i, 20, level/5);
      return ([
      "action" : action[j]["action"],
      "damage_type" : action[j]["damage_type"],
      "lvl": action[j]["lvl"],
      "skill_class"   : MIAO,
      "skill_damage"  : -10,
      "skill_attack"  : 20,
      "skill_parry"   : -20,
      "skill_dodge"   : 20,
      "skill_kill"    : 0,
      "skill_rush"    : 20,
      "skill_hurt"    : 20,
      "skill_improve" : 2
      ]);
    }
  }
}

int practice_skill(object me)
{
  int lvl = me->query_skill("yuxiao-jian", 1);
  int i = sizeof(action);
  mapping learned = me->query_learned();

  if (!mapp(learned)) learned = ([]);
  if (!learned["yuxiao-jian"])
  while (i--)
  if (lvl == action[i]["lvl"])
    return notify_fail("你将已经学会的所有招式练习了一遍，发现如果不去找师父学新的招式就无法获得进步了。\n");;

  if ( me->query("jingli") < 50)
    return notify_fail("你的体力不够练玉箫剑法。\n");
  if( me->query("neili") < 20)
    return notify_fail("你的内力不够练玉箫剑法。\n");
  me->receive_damage("jingli", 40);
  me->add("neili", -15);
  return 1;
}

string perform_action_file(string action)
{
  return __DIR__"thd/yuxiao-jian/" + action;
}

void yuxiao(object me, object victim)
{
  string msg;

  if (!me || !victim || !me->is_fighting(victim))
  return;

  if ( !me->query_temp("weapon")) return;

  switch(random(3)){
    case 0 :
      msg = HIW"\n$N施展开奇门剑路，剑剑不依照常规！$n连连後退，全力招架，无力还招！"NOR;
      message_vision(msg, me, victim);
      me->set_temp("yuxiao_auto", 1);
      COMBAT_D->do_attack(me, victim, me->query_temp("weapon"), 2);
      me->delete_temp("yuxiao_auto");
      victim->add_busy(random(2));
      break;
    case 1 :
      msg = HIW"\n$N脚下踩著奇门步法，一下子刺出好几剑！$n根本分不清楚那一剑是虚，那一剑是实，只好停下攻势，严守门户！"NOR;
      message_vision(msg, me, victim);
      me->set_temp("yuxiao_auto", 1);
      COMBAT_D->do_attack(me, victim, me->query_temp("weapon"), 0);
      me->delete_temp("yuxiao_auto");
      victim->add_busy(random(2));
      break;
    case 2 :
      msg = HIW"\n$N风驰电掣的绕著$n快跑，忽东忽西，简直像是足不点地的飞行一般！$n随着$N转来转去，弄得眼花缭乱，摇摇欲坠！"NOR;
      message_vision(msg, me, victim);
      me->set_temp("yuxiao_auto", 1);
      COMBAT_D->do_attack(me, victim, me->query_temp("weapon"), 1);
      me->delete_temp("yuxiao_auto");
      victim->add_busy(random(2));
      break;
  }
}

mixed hit_ob(object me, object victim, int damage_bonus)
{
  int j, k;

  j = me->query_skill("yuxiao-jian", 1);
  k = me->query_skill("qimen-bagua",1);

  if( random(2) 
  && random(j) > 120
  && k > 150
  && me->query_temp("weapon")
  && j/3 > random(victim->query_skill("dodge"))
  && !me->query_temp("yuxiao_auto")
  && !me->query_temp("thd/qimen")
  && !me->query_temp("thd/feiying")){
    call_out((: yuxiao :), 1, me, victim);
  }
}
