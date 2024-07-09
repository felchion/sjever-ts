// jiuyin-baiguzhua.c 九阴白骨爪

#include <ansi.h>
#include <skill_class.h>
inherit SKILL;

string query_class() {return "险";}

mapping *action = ({
([
	"action" : BLU"$N势若疯虎，形若邪魔，双爪如钢抓铁钩，左手手腕翻处，右手疾伸，五根手指抓向$n$l"NOR,
	"lvl" : 0,
   	"damage_type" : "抓伤"
]),
([
	"action" : BLU"突然人影闪动，$N迫到$n身后，袖中伸出手，五根手指向$n$l插了下去"NOR,
	"weapon" : "五指",
	"lvl" : 10,
	"damage_type" : "刺伤"
]),
([
	"action" : BLU"$N手臂关节喀喇一响，手臂斗然长了半尺，一掌按在$n$l，五指即要插进"NOR,
	"weapon" : "手指",
   	"lvl" : 20,
	"damage_type" : "刺伤"
]),
([
	"action" : BLU"$N双手微张，十根尖尖的指甲映出灰白光忙，突然翻腕出爪，五指猛地插落"NOR,
	"weapon" : "手爪",
	"lvl" : 30,
   	"damage_type" : "刺伤"
]),
([
	"action" : BLU"$N左手突然在$n眼前上围下钩，左旋右转，连变了七八般，蓦地里右手一伸，五根手指直插$n$l"NOR,
	"weapon" : "五根手指",
	"lvl" : 40,
	"damage_type" : "刺伤"
]),
([
	"action" : BLU "$N左爪虚晃，右爪蓄力，一招" HIR "「勾魂夺魄」" BLU "直插向$n的$l" NOR,
	"lvl"   : 100,
	"skill_name" : "勾魂夺魄" ,
	"damage_type" : "抓伤"
]),
([
	"action" : BLU "$N双手连环成爪，爪爪钩向$n，" HIR "「九子连环」" BLU "已向$n的$l抓出" NOR,
	"lvl"   : 120,
	"skill_name" : "九子连环" ,
	"damage_type" : "抓伤"
]),
([
	"action" : BLU "$N身形围$n一转，使出" HIR "「天罗地网」" BLU "，$n的$l已完全笼罩在爪影下" NOR,
	"lvl"   : 140,
	"skill_name" : "天罗地网" ,
	"damage_type" : "抓伤"
]),
([
	"action" : BLU "$N使一招" HIR "「风卷残云」" BLU "，双爪幻出满天爪影抓向$n全身" NOR,
	"lvl"   : 160,
	"skill_name" : "风卷残云" ,
	"damage_type" : "抓伤"
]),
([
	"action" : BLU "$N吐气扬声，一招" HIR "「唯我独尊」" BLU "双爪奋力向$n天灵戳下" NOR,
	"lvl"   : 180,
	"skill_name" : "唯我独尊" ,
	"damage_type" : "抓伤"
]),
});

int valid_enable(string usage)
{
	return usage=="claw" || usage=="parry" ;
}

int valid_learn(object me)
{
	if( !me->query("quest/jiuyin2/pass") && !me->query("quest/jiuyin1/pass") && !me->query("quest/jiuyin1/em_pass"))
		return notify_fail("你不可练习九阴白骨爪。\n");
	if( (int)me->query_skill("jiuyin-baiguzhua", 1 ) < 150 )
		return notify_fail("你现在不能练习九阴白骨爪。\n");
	if( (int)me->query_skill("jiuyin-shenzhua", 1 ) >= 1 )
		return notify_fail("会九阴神抓了，还想学九阴白骨爪？\n");
	if (me->query_temp("weapon") || me->query_temp("secondary_weapon"))
		return notify_fail("练九阴白骨爪必须空手。\n");
	if ((int)me->query_skill("claw", 1) < 41)
		return notify_fail("你的基本爪功火候不够，无法学习。\n");
	if ((int)me->query("max_neili") < 1000)
		return notify_fail("你的内力太弱，无法练九阴白骨爪。\n");
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
	level   = (int) me->query_skill("jiuyin-baiguzhua",1);
	for(i = sizeof(action); i > 0; i--) {
		if(level >= action[i-1]["lvl"]) {
			j = NewRandom(i, 20, level/5);
			return ([
				"action"				:	action[j]["action"],
				"lvl"						:	action[j]["lvl"],
				"damage_type"		:	action[j]["damage_type"],
				"skill_class" 	: XIAN,
				"skill_damage" 	: 50,
				"skill_attack" 	: 30,
				"skill_parry" 	: 0,
				"skill_dodge"		: 0,
				"skill_kill" 		: 10,
				"skill_rush" 		: 10,
				"skill_hurt" 	  : 100,			
				"skill_GP" 			: 2		
			]);
		}
	}
}

int practice_skill(object me)
{
	if ((int)me->query("jingli") < 50)
		return notify_fail("你的体力太低了。\n");
	if ((int)me->query("neili") < 20)
		return notify_fail("你的内力不够练九阴白骨爪。\n");
	me->receive_damage("jingli", 40);
	me->add("neili", -15);
	return 1;
}

string perform_action_file(string action)
{
	return __DIR__"quest/jiuyin-baiguzhua/" + action;
}

void skill_improved(object me)
{
	tell_object(me, "你练着练着忽然从心底生出一股邪念！\n");
	me->add("shen", -(me->query_skill("jiuyin-baiguzhua", 1)*100));
}
