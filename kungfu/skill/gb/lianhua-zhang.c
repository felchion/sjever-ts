// lianhua-zhang.c 莲花掌
// By Looklove 2000/10/18
// Modify by Looklove 2001.4.4
// Modify by Looklove 2001.4.6
// update 2001.10.4

#include <ansi.h>
inherit SKILL;

#include <skill_class.h>
string query_class() {return "慢";}

mapping *action = ({
([
	"action" : "$N使出莲花掌起手势「小莲初绽」，左掌护胸，右掌缓缓拍向$n的$l",
	"lvl" : 0,
	"damage_type" : "瘀伤",
	"skill_name" : "小莲初绽"
]),
([
	"action" : "$N面露微笑，一招「叶底清荷」左掌一摆，右掌划出一道美妙的弧线，直切$n的$l",
	"lvl" : 10,
        "damage_type" : "劈伤",
	"skill_name" : "叶底清荷"
]),
([
	"action" : "$N双手微张缓缓前探，双臂伸直后突然变化为掌，一式「莲池泛舟」摆向$n的$l",
	"lvl" : 20,
	"damage_type" : "瘀伤",
	"skill_name" : "莲池泛舟"
]),
([
	"action" : "只见$N使一招「藕断丝连」，左掌晃动着平举，右掌摇摆不定中画个圆圈拍向$n",
	"lvl" : 30,
	"damage_type" : "内伤",
	"skill_name" : "藕断丝连"
]),
([
	"action" : "$N后退一步，右手护胸，左手一记「千叶莲花」劈出一道掌风，直奔$n的$l",
	"lvl" : 40,
	"damage_type" : "内伤",
	"skill_name" : "千叶莲花"
]),
([	"action" : "$N使出「双莲争艳」，双掌飞快舞动，将$n圈在掌风之内，在眼花缭乱间骤然拍出",
	"lvl" : 50,
        "damage_type" : "劈伤",
	"skill_name" : "双莲争艳"
]),
([
	"action" : "$N一式「雨打莲花」飞身跃起直扑$n，双掌幻化出数十个掌影，快如雨点击向$n",
	"lvl" : 60,
        "damage_type" : "瘀伤",
	"skill_name" : "雨打莲花"
]),
([
	"action" : "$N右掌护胸，左掌在面前一晃，突地从腋下穿出，一式「旧藕新莲」直拍$n胸膛",
	"lvl" : 80,
        "damage_type" : "劈伤",
	"skill_name" : "旧藕新莲"
]),
([	"action" : "$N绕着$n快速游走，使出「遍野荷香」手掌如穿花蝴蝶般上下飞舞，罩向$n的$l",
	"lvl" : 100,
        "damage_type" : "瘀伤",
	"skill_name" : "遍野荷香"
]),
([
	"action" : "刹那间$N反身错步，突然使出一招「雨后清莲」，猛一掌便向$n的$l劈去",
	"lvl" : 120,
        "damage_type" : "内伤",
	"skill_name" : "雨后清莲"
]),
});

int valid_enable(string usage) { return usage=="strike" ||  usage=="parry"; }

int valid_learn(object me)
{
	if( me->query_temp("weapon") || me->query_temp("secondary_weapon") )
		return notify_fail("练莲花掌必须空手。\n");
	if( (int)me->query_skill("huntian-qigong", 1) < 30 )
		return notify_fail("你的混天气功火候不够，无法学习莲花掌。\n");
	if( (int)me->query("max_neili") < 150 )
		return notify_fail("你的内力太弱，无法练莲花掌。\n");
	return 1;
}

string query_skill_name(int level)
{
        int i = sizeof(action);
        while (i--)
                if(level > action[i]["lvl"])
                        return action[i]["skill_name"];
}

mapping query_action(object me)
{
	int i, j,level;
  
  level = (int)me->query_skill("dagou-bang", 1);
	for(i = sizeof(action); i > 0; i--)
		if(level >= action[i-1]["lvl"]) {
			j = NewRandom(i, 20, level/5);
			return ([
				"action" : action[j]["action"],
				"lvl" : action[j]["lvl"],
				"damage_type" : action[j]["damage_type"],
				"skill_class" 	: MAN,
				"skill_damage" 	: 0 ,
				"skill_attack" 	: 30 ,
				"skill_parry" 	: -10 ,
				"skill_dodge"		: 30 ,
				"skill_kill" 		: -50,
				"skill_rush" 		: 100,
				"skill_hurt" 	  : -50,
				"skill_GP" 			: 2
			]);
		}
}


int practice_skill(object me)
{
	int lvl = me->query_skill("lianhua-zhang", 1);
	int i = sizeof(action);

	while (i--) if (lvl == action[i]["lvl"]) return 0;

	if ( me->query("jingli") < 40)
		return notify_fail("你的精力太低了。\n");
	if ( me->query("neili") < 20)
		return notify_fail("你的内力不够练莲花掌。\n");
	me->receive_damage("jingli", 30);
	me->receive_damage("neili", 10);
	return 1;
}

string perform_action_file(string action)
{
	return __DIR__"gb/lianhua-zhang/" + action;
}

mixed hit_ob(object me, object target)
{
	string msg;
	int j, p;
	object wp = me->query_temp("weapon");

	p = me->query_int()+me->query("dex");
	j = me->query_skill("lianhua-zhang", 1);

	if( random(p) > 40 && !target->is_busy() && random(j) > 100
	 && me->query("neili") > random(target->query("neili")/2)
	 && random(me->query("neili")) > 500
	 && me->query("max_neili") > 1500
	 && !objectp(wp)
	 && random(j+p) > random(target->query_skill("dodge",1) )
	 && !me->query_temp("lhz/bdlh")
	 && !me->query_temp("shuangjue/lhz") ) {
		switch(random(2)) {
			case 0 :
				msg = "紧跟着$N又使出「双莲争艳」，双掌车轮般挥出，将$n圈在掌风之内，";
				msg += "$n眼前一花，已失去了$N的踪影。\n";
				target->add_busy(3+random(2));
				break;
			case 1 :
				msg = "紧跟着$N又一式「遍野荷香」，两只手掌如穿花蝴蝶般上下飞舞。\n";
				msg += "$n只觉得眼花缭乱，分不清哪招是实哪招是虚。\n";
				target->add_busy(3+random(2));
				break;
		}
		message_vision(msg, me, target);
	}
}

int help(object me)
{
	write(HIM"\n莲花掌："NOR"\n");
	write(@HELP
	丐帮世代流传的一门空手入门功夫。初入门的弟子，多学莲花掌。
        随着级别的提高，威力会有所增强。

	学习要求：混天气功大于30，内力大于150 。
	自动攻击：雨后清莲。
	悟性大于24，混天气功，基本掌法，莲花掌全部大于80，
	内力大于 800 ，战斗中随机发出。
HELP
        );
        return 1;
}
