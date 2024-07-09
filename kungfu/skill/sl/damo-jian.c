// damo-jian.c 达摩剑

inherit SKILL;

mapping *action = ({
([
	"action":"$N使一式「万事随缘往」，手中$w嗡嗡微振，幻成一条疾光刺向$n的$l",
	"lvl" : 0,
	"skill_name" : "万事随缘往",
	"damage_type":  "刺伤"
]),
([
	"action":"$N错步上前，使出「来去若梦行」，剑意若有若无，$w淡淡地向$n的$l挥去",
	"lvl" : 9,
	"skill_name" : "来去若梦行",
	"damage_type":  "割伤"
]),
([
	"action":"$N一式「浮世沧桑远」，纵身飘开数尺，运发剑气，手中$w遥摇指向$n的$l",
	"lvl" : 18,
	"skill_name" : "浮世沧桑远",
	"damage_type":  "割伤"
]),
([
	"action":"$N纵身轻轻跃起，一式「轮回法舟轻」，剑光如轮疾转，霍霍斩向$n的$l",
	"lvl" : 27,
	"skill_name" : "轮回法舟轻",
	"damage_type":  "割伤"
]),
([
	"action":"$N手中$w中宫直进，一式「水月通禅寂」，无声无息地对准$n的$l刺出一剑",
	"lvl" : 36,
	"skill_name" : "水月通禅寂",
	"damage_type":  "刺伤"
]),
([
	"action":"$N手中$w斜指苍天，剑芒吞吐，一式「鱼龙听梵音」，对准$n的$l斜斜击出",
	"lvl" : 44,
	"skill_name" : "鱼龙听梵音",
	"damage_type":  "刺伤"
]),
([
	"action":"$N左指凌空虚点，右手$w逼出丈许雪亮剑芒，一式「千里一苇去」刺向$n的$l",
	"lvl" : 52,
	"skill_name" : "千里一苇去",
	"damage_type":  "刺伤"
]),
([
	"action":"$N合掌跌坐，一式「禅心顿自明」，$w自怀中跃出，如疾电般射向$n的胸口",
	"lvl" : 60,
	"skill_name" : "禅心顿自明",
	"damage_type":  "刺伤"
]),
});

int valid_enable(string usage) { return usage == "sword" || usage == "parry"; }

int valid_learn(object me)
{
	object weapon;

	if (!objectp(weapon = me->query_temp("weapon"))
	|| (string)weapon->query("skill_type") != "sword")
		return notify_fail("你使用的武器不对。\n");

	if ((int)me->query("max_neili") < 100)
		return notify_fail("你的内力不够。\n");
	if ((int)me->query_skill("yijin-jing", 1) < 20)
		return notify_fail("你的易筋经内功功火候太浅。\n");
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
	int i, level, j;
	level   = (int) me->query_skill("damo-jian",1);
	for(i = sizeof(action); i > 0; i--) {
		if(level > action[i-1]["lvl"]) {
			j = NewRandom(i, 20, level/5);
			return ([
				"action": action[j]["action"],
				"damage_type": action[j]["damage_type"],
				"lvl": action[j]["lvl"],
				"force": 100 + random(250),
				"dodge": random(20)-10,
				"damage": 40 + random(60),
				"parry": random(10),
			]);
		}
	}
}

int practice_skill(object me)
{
	int lvl = me->query_skill("damo-jian", 1);
	int i = sizeof(action);

	while (i--) if (lvl == action[i]["lvl"]) return 0;

	if ((int)me->query("jingli") < 30 || me->query("neili") < 10)
		return notify_fail("你的精力不够练达摩剑。\n");
	me->receive_damage("jingli", 20);
	me->add("neili", -5);
	return 1;
}

string perform_action_file(string action)
{
	return __DIR__"sl/damo-jian/" + action;
}
