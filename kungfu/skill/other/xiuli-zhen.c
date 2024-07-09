// 秀丽娥女针
// Created by snowman@SJ 28/02/2001

#include <ansi.h>
inherit SKILL;

mapping *action = ({
([      "action" : "$N一摆手中$w，出一式「苍松翠绿」，去势中庸、平指刺向$n，只第一招便将娥嵋武功中的“稳”发挥得淋漓尽致",
        "force" : 160,
        "dodge" : 20,
        "parry" : 5,
        "damage" : 25,
        "lvl" : 0,
        "skill_name" : "苍松翠绿",
        "damage_type" : "刺伤"
]),
([      "action" : "$N身姿优美，将$w随手晃出，但见一式「青山起舞」，如同娥嵋山上的连绵青松，青光点点扫向$n",
        "force" : 220,
        "dodge" : 15,
        "parry" : 15,
        "damage" : 30,
        "lvl" : 20,
        "skill_name" : "青山起舞",
        "damage_type" : "刺伤"
]),
([      "action" : "$N将手中$w斜指苍天，轻笑中一式「千峰竞秀」，化出无数个青光，配合着优美的身法，直向$n点去",
        "force" : 240,
        "dodge" : 15,
        "parry" : 15,
        "damage" : 40,
        "lvl" : 40,
        "skill_name" : "千峰竞秀",
        "damage_type" : "刺伤"
]),
([      "action" : "$N推后一步，一招「浮云彩霞」，将$w上下挥舞，剑影化出了一片片美丽的霞光，向$n洒出",
        "force" : 260,
        "dodge" : 10,
        "parry" : 15,
        "damage" : 50,
        "lvl" : 60,
        "skill_name" : "浮云彩霞",
        "damage_type" : "刺伤"
]),
([      "action" : "$N回身拧腰，$w随身而走，从上至下一式「飞瀑流泉」，霞光洒出，宛如瀑水直落，划向$n的要穴",
        "force" : 280,
        "parry" : 15,
        "dodge" : 10,
        "damage" : 60,
        "lvl" : 80,
        "skill_name" : "飞瀑流泉",
        "damage_type" : "刺伤"
]),
([      "action" : "$N将$w一甩，尖点颤动，在$n面前划出一朵朵美丽的剑花，宛如杜鹃盛开，这是娥眉秀丽针中的「山花怒放」",
        "force" : 300,
        "dodge" : 10,
        "parry" : 20,
        "damage" : 75,
        "lvl" : 100,
        "skill_name" : "山花怒放",
        "damage_type" : "刺伤"
]),
([      "action" : "$N施展开独门「绿水欢歌」身法，身形似隐似现，犹如翩翩起舞的彩蝶，又犹如天仙下凡，手中$w飞舞如彩带",
        "force" : 320,
        "dodge" : 5,
        "parry" : 15,
    	"damage" : 90,
        "lvl" : 120,
        "skill_name" : "绿水欢歌",
        "damage_type" : "刺伤"
]),
([      "action" : "$N长啸一声，一式「秀丽娥眉」腾空飞起，在半空回眸，一道月弧、雪亮的光芒飞出，照耀天际！",
        "force" : 340,
        "dodge" : 10,
        "parry" : 15,
    	"damage" : 120,
        "lvl" : 150,
        "skill_name" : "秀丽娥眉",
        "damage_type" : "刺伤"
]),
});


int valid_enable(string usage) { return (usage == "dagger") || (usage == "parry"); }
int valid_learn(object me)
{
        if ((int)me->query("max_neili") < 100)
                return notify_fail("你的内力不够。\n");
                
        if ((int)me->query_skill("qingdai-shengang", 1) < 20)
                return notify_fail("你的清黛神罡火候太浅。\n");
        if ((int)me->query_skill("parry", 1) < 10)
                return notify_fail("你的基本招架火候太浅。\n");
        return 1;
}


mapping query_action(object me,object target)
{
        int i, level;
        level   = (int) me->query_skill("xiuli-zhen",1);
        for(i = sizeof(action); i > 0; i--)
                if(level > action[i-1]["lvl"])
                        return action[NewRandom(i, 20, level/5)];
}
int practice_skill(object me)
{
        object weapon;

        if (!objectp(weapon = me->query_temp("weapon"))
        || (string)weapon->query("skill_type") != "dagger")
                return notify_fail("你使用的武器不对。\n");

       	if ((int)me->query("jingli") < 40)
                return notify_fail("你的体力不够练秀丽娥女针。\n");
        me->receive_damage("jingli", 30);
        if ((int)me->query("neili") < 20)
                return notify_fail("你的内力不够练秀丽娥女针。\n");
        me->receive_damage("jingli", 5);
        
        return 1;
}
           

string perform_action_file(string action)
{        
	return __DIR__"xiuli-zhen/" + action;
}
