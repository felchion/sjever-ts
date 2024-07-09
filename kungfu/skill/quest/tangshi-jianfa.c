#include <ansi.h>
#include <combat.h>
inherit SKILL;
#include <combat_msg.h>

mapping *action = ({
([	"action" : "$N向前跨上一步，同时口中大喊一声，一式「"+HIG"哥翁喊上来"NOR+"」直刺$n的$l",
	"force" :random(30)+ 60,
	"dodge" : 5,
	"damage" : 20,
        "lvl" : 0,
	"damage_type" : "刺伤"
]),
([      "action" : "$N向前轻跳，一招「"+HIB"马命风小小"NOR+"」，剑光渺茫莫测，$w向前轻刺$n的$l",
        "force" :random(30)+ 80,
        "dodge" : 15,
        "damage" : 25,
        "lvl" : 10,
        "damage_type" : "刺伤",
]),
([      "action" : "$N左手耳朵一招，仿佛在倾听什么，一招「"+HIW"忽听喷惊风"NOR+"」，右手中的$w向前刺向$n的$l",
        "force" :random(30)+ 100,
        "dodge" : 5,
        "damage" : 35,
        "lvl" : 20,
        "damage_type" : "刺伤",
]),
([      "action" : "$N手中$w急转，一式「"+HIY"连山布若逃"NOR+"」，手中$w的剑势像一疋布一样砍向$n的$l",
        "force" :random(30)+ 120,
        "dodge" : 10,
        "damage" : 40,
        "lvl" : 30,
        "damage_type" : "割伤",
]),
([      "action" : "$N剑招忽变，$w胸前一横，歪歪斜斜地使出一式「"+HIR"是横不敢过"NOR+"」，$w横削$n",
        "force" :random(30)+ 140,
        "dodge" : -5,
        "damage" : 45,
        "lvl" : 40,
        "damage_type" : "割伤",
]),
([  	"action" : "$N剑尖斜指向地，向后一闪，一招「"+HIC"落泥招大姐"NOR+"」，招手让$n过来，$w随即刺出",
    	"force" :random(30)+ 160,
    	"dodge" : 15,
    	"damage" : 50,
    	"lvl" : 50,
    	"damage_type" : "刺伤",
]),
});

int valid_enable(string usage) { return usage == "sword" || usage == "parry"; }

int valid_learn(object me)
{
	return 1;
}

mapping query_action(object me, object weapon)
{
	int i, level;
	level   = (int) me->query_skill("tangshi-jianfa",1);
	if ( me->query_temp("tangshi/erguang")){
			return ([
				"action": random(2)?HIW"$N左手捏个剑诀，右手长剑陡然递出，猛地里剑交左手，右手反过来拍的一声耳光"NOR:HIY"$N右手捏个剑诀，左手长剑陡然递出，猛地里剑交右手，左手反过来拍的一声耳光"NOR,
                "damage": 150,
                "dodge" : 100,
                "damage_type": "内伤"
			]);
                
	}
	if ( me->query_temp("tangshi/qujian")){
			return ([
				"action": HIY"$N手中$w"HIY"一抖，向$n刺来，$n侧身闪过，见$N$w"HIY"停滞不前，当即反守为攻。\n"HIC"突然$w"HIC"如毒蛇暴起，剑招奇变横生，$w"HIC"向前一探，刺向$p。"NOR,
                "damage": 80,
                "dodge" : 100,
                "damage_type": "刺伤"
			]);                
	}
	if ( me->query_temp("tangshi/cijian")){
			return ([
				"action": HIY"$N长啸一声，招数猛然加快，$n挡得几招，$N突然间长剑抖动，卟的一声轻响刺向$n。"NOR,
                "damage": 80,
                "dodge" : 100,
                "damage_type": "刺伤"
			]);                
	}
	for(i = sizeof(action); i > 0; i--)
                if(level > action[i-1]["lvl"])
                        return action[NewRandom(i, 20, level/5)];
}

int practice_skill(object me)
{
	object weapon;

	if (!objectp(weapon = me->query_temp("weapon"))
	|| (string)weapon->query("skill_type") != "sword")
		return notify_fail("你使用的武器不对。\n");
	if ((int)me->query("jingli") < 30)
		return notify_fail("你的精力不够练躺尸剑法。\n");
	me->receive_damage("jingli", 20);
	return 1;
}
string perform_action_file(string action)
{
	return __DIR__"quest/tangshi-jianfa/" + action;
}

mixed hit_ob(object me, object victim, int damage_bonus)
{
	object weapon, weapon1; 
	weapon = me->query_temp("weapon");
	weapon1= victim->query_temp("weapon");
	if( !objectp(weapon) ) return;
	if(!me->query_temp("tangshi")) return;
    
	if(me->query_temp("tangshi/erguang") && random(2))
	{
		tell_object(victim, HIR"你不禁大骇，这一招太是突然，你一怔之间，"+me->query("name")+"已在面前。\n"NOR);        
		victim->add_busy(1);
	}
   if(me->query_temp("tangshi/qujian") && weapon1)
   {
	   if(random(2))
		   message_vision(HIC"跟着，$N翻过"+weapon->name()+HIC"，平靠"+weapon1->name()+HIC"，$N的"+weapon->name()+HIR"连转几个圈子。\n"NOR, me,victim);
	   else 
		   message_vision(HIY"跟着，$N"+weapon->name()+HIY"一伸，搭住"+weapon1->name()+HIY"，"+weapon->name()+HIY"如有胶水，将$n劲力全引到了相反的方向。\n"NOR, me,victim);

	   if(random(me->query("str")) > victim->query("str")/3){           
           		me->add("neili", -50);
           		message_vision(HIR"$N招式渐见涩滞，"+weapon1->name()+HIR"拿捏不住，脱手飞出！\n"NOR, victim);
           		weapon1->unequip();
           		weapon1->move(environment(victim));
           	}
      		else 
           		message_vision(HIR"$N招式渐见涩滞，"+weapon1->name()+HIR"差点拿捏不住！\n"NOR, victim);
   }
}

