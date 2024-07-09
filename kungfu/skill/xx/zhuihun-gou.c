// zhuihun-gou.c, 天山追魂钩
// by snowman@SJ 30/09/1999

#include <ansi.h>
inherit SKILL;

string *parry_msg = ({
"$n嘿嘿一笑，手中$w望前一架，将$N硬生生地挡了开去，钩尖还差点挂伤$P。\n",
"却见$n不闪不避，只将手中$w顺着手臂一放，便挡住了$N。\n",
"$n钩法突变，$w飞舞而出，后发先至，在$N还没攻出时就破了$p的招数。\n",
"但见$n举$w硬挡，乒的一声架开了来势汹汹的$N。\n",
});


string query_parry_msg(object weapon, object victim) 
{ 
	if ( victim->query_temp("weapon") && this_player()->query_temp("weapon"))
		return parry_msg[random(sizeof(parry_msg))];
		
	else	return SKILL_D("parry")->query_parry_msg();
}

mapping *action = ({
([  "action": "森森慑人的厉吼出自$N口中，$w蓦而急颤晃动，在眨眼的瞬息间，耀目的电光四射迸闪，飞掠周遭，迅捷无匹的击向$n",
    "dodge": 30,
    "force" : 130,
    "damage": 60,
    "skill_name" : "极电追魂",
    "lvl" : 0,
    "damage_type": "刺伤"
]),
([  "action": "随着$N手中$w发出的一片刺耳而沈闷的金属撞击声，半空中气流旋荡，地上的尘土亦四散溅射，一条乌光向着$n直挥而至",
    "dodge": 30,
    "force" : 200,
    "damage": 80,
    "skill_name" : "煞风追魂",
    "lvl" : 20,
    "damage_type": "割伤"
]),
([  "action": "$N再次冲上，$w围著$n的四周旋舞闪射，千百光影在黯衣中幻映出一幅美丽而眩目的图案，这图案美极了，似是一片闪亮的雪花晶体",
    "dodge": 40,
    "force" : 330,
    "damage": 150,
    "skill_name" : "寒星追魂",
    "lvl" : 40,
    "damage_type": "刺伤"
]),
([  "action": "$N附近地面蓦而滚滚飞扬，五丈方圆内起了一阵回旋的风暴，光华灿然中，隐隐有无数条金色狂龙飞舞而出，随着$w寒光一闪，直奔$n",
    "dodge": 30,
    "force" : 200,
    "damage": 100,
    "skill_name" : "狂沙追魂",
    "lvl" : 60,
    "damage_type": "割伤"
]),
([  "action": "$N神色沈凝，钩势刹那间又出，在七个角度里由$w的二十一钩幻成七片流光，骤出一刺有若九天飞瀑，瞬间已经到了$n眼前",
    "dodge": 20,
    "force" : 270,
    "damage": 110,
    "skill_name" : "飓涛追魂",
    "lvl" : 100,
    "damage_type": "拉伤"
]),
([  "action": "$N手中$w尖突然闪起一团强烈的寒光，「嗡」地一颤，弯成一个半弧，又蓦而弹直，如云似幻，锋利的边刃猝然斜偏，削向$n颈项",
    "dodge": 30,
    "force" : 300,
    "damage": 130,
    "skill_name" : "幻云追魂",
    "lvl" : 140,
    "damage_type": "割伤"
]),
([  "action": "$N的钩式趋向凌乱，$w划出溃无章法，锋利的刃身幌摇不定，嗡嗡作响，震得$n随著那阵阵狂飙之声跄踉後退，狼狈不堪",
    "dodge": 10,
    "force" : 300,
    "damage": 100,
    "skill_name" : "迅雷追魂",
    "lvl" : 180,
    "damage_type": "震伤"
]),
([  "action": "$N怀抱$w而立，真气宛如海啸山崩，呼天唤地，滚翻而至，宛如一阵鬼泣神号般的凄厉呼啸，连带着无数条细密的光影刮向$n全身上下",
    "dodge": 30,
    "force" : 320,
    "damage": 160,
    "skill_name" : "骤雨追魂",
    "lvl" : 0,
    "damage_type": "割伤"
]),
});

string query_skill_name(int level)
{
        int i = sizeof(action);

        while (i--)
                if(level > action[i]["lvl"])
                        return action[i]["skill_name"];
}


int valid_enable(string usage) { return (usage == "hook") || (usage == "parry"); }

int valid_learn(object me)
{
    if ((int)me->query("max_neili") < 2000)
        return notify_fail("天山追魂钩需要极高的内力修为才能运用，你还是先集中精力修练内力吧。\n");

    if ((int)me->query("max_jingli") < 1000)
        return notify_fail("天山追魂钩需要极高的精力修为才能运用，你还是先集中精力修练精力吧。\n");

    if (me->query_skill("huagong-dafa", 1) < 100)
               return notify_fail("你的化功大法火候太浅，修习不了天山追魂钩。\n");
               
        return 1;
}

int practice_skill(object me)
{
        object weapon;
       
        if (!objectp(weapon = me->query_temp("weapon"))
        || (string)weapon->query("skill_type") != "hook")
        	return notify_fail("你手上的武器不能用来练天山追魂钩。\n");
        if ((int)me->query("jingli") < 60)
        	return notify_fail("你先歇口气再练吧。\n");
        if ((int)me->query("neili") < 20)
        	return notify_fail("你先聚集点内力再练吧。\n");
        me->receive_damage("jingli", 50);  
        me->receive_damage("neili", 10);      
        return 1;
}

mapping query_action(object me, object weapon)
{
        int i, level;
        
        level=(int)me->query_skill("zhuihun-gou",1);
        
        for(i = sizeof(action); i > 0; i--)
                if(level > action[i-1]["lvl"])
                        return action[NewRandom(i, 20, level/5)];
}

string perform_action_file(string action)
{
        return __DIR__"xx/zhuihun-gou/" + action;
}

