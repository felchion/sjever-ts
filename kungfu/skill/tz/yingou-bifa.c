//yingou-bifa.c 银钩笔法
// BY BOBO@SJ 
inherit SKILL;
#include <skill_class.h>
string query_class() {return "慢";}

string  *msg = ({
"待到月涌中天，$N长啸一声，$w直划下来，当真是星剑光芒，如矢应机，霆不暇发，电不及飞，这一直乃是「锋」字的最后一笔",
"$N顺手使出「天」字的一捺，$w向$n的$l扫出",
"$N转身吐气，$w送出，却是「下」字诀的一直，向$n后心击出",
"$N一个左拗步，抢到了西首，$w自左向右平平横扫，打向$n的太阳穴，这正是「不」字决中的一横",
"$N手中$w自右上角斜挥左下角，扫向$n的腰肋。这一下是「不」字诀的一撇捺",
"$N左手直钩，右手中的$w砰的一点，写成「不」字最后一笔，一点点向$n胸中膻中穴",
"$N伸左掌在$n肩上一按，一借力，轻轻巧巧的在空中一个翻身，落在远处，这一招乃是「武」字诀中的一「戈」，正所谓：「差池燕起，振迅鸿飞，临危制节，中险腾机」，当千钧一发之际，转危为安",
"$N手中$w下掠，反手使出一记「刀」字诀，左掌带住$n的一记攻势，$w的一撇在$n$l一点，身子借势窜起",
"$N神定气足，挥洒自如，蓦地里右手倏出，使个「龙」字诀中的一钩，钩向$n的$l",
"$N一招「争」字诀中的一下直钩，将$w在半空中疾挥下来，击向$n的太阳穴",
});

int valid_enable(string usage)
{
    	return usage == "brush" || usage == "parry";
}

mapping query_action(object me, object weapon)
{
	return ([
		"action":msg[random(sizeof(msg))],
		"damage_type" : "刺伤",
		"skill_class"   : MAN,
		"skill_damage"  : 0,
		"skill_attack"  : 30,
		"skill_parry"   : 0,
		"skill_dodge"   : 0,
		"skill_kill"    : 0,
		"skill_rush"    : 0,
		"skill_hurt"    : 50,
		"skill_GP"      : 2
	]);
}

int practice_skill(object me)
{    
        object weapon;

        if (!objectp(weapon = me->query_temp("weapon"))
        || (string)weapon->query("skill_type") != "brush")
                return notify_fail("你使用的武器不对。\n");
       	if ((int)me->query("jingli") < 50)
               return notify_fail("你的体力不够练判官笔法。\n");         
       	if ((int)me->query("neili") < 30)
               return notify_fail("你的内力不够练判官笔法。\n");
       	me->receive_damage("jingli", 35);
       	me->add("neili", -10);
       	return 1;
}

int valid_learn(object me)
{
       int skill = me->query_skill("yingou-bifa", 1);
       if ((int)me->query("max_neili") < 1500 )
               return notify_fail("你的内力不够。\n");
       if ((int)me->query_skill("guiyuan-tunafa", 1) < (skill-60))
               return notify_fail("你的归元吐纳法火候太浅。\n");
       if ((int)me->query_skill("literate", 1) < 150 )
               return notify_fail("你的读书写字级别太低，无法领会书法的意念。\n");      
       return 1;
}

string perform_action_file(string action)
{
      	return __DIR__"tz/yingou-bifa/" + action;
}
