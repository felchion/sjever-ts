// anran-zhang.c 黯然销魂掌
// By River 98.10.28
// Modified by snowman@SJ 20/06/2000

#include <ansi.h>
#include <combat.h>

inherit SKILL;

#include <combat_msg.h>
#include <skill_class.h>

string query_class() {return "慢";}

mapping *action = ({
([
	"action" :"$N身影陡转，如鬼魅般忽前忽后，瞬左瞬右，一式「徘徊空谷」掌风已至$n的$l",
	"lvl" : 0,
	"skill_name" : "徘徊空谷",
	"damage_type" : "瘀伤"
]),
([
	"action" :"$N长叹一声，身形凝重，一招「力不从心」，诱得$n全力进击之际，反手直拍$n的$l",
	"lvl" : 15,
	"skill_name" : "力不从心",
	"damage_type" : "瘀伤"
]),
([
	"action" :"$N面目僵然，四肢挺直，无弯无折，似「行尸走肉」般，刀臂剑腿，横斩直刺$n的$l",
	"lvl" : 30,
	"skill_name" : "行尸走肉",
	"damage_type" : "瘀伤"
]),
([
	"action" :"$N一式「庸人自扰」，左手锤胸，一边晃首不止，倏忽间绕敌身后，右掌暴击$n的$l",
	"lvl" : 45,
	"skill_name" : "庸人自扰",
	"damage_type" : "瘀伤"
]),
([
	"action" :"$N突然头下脚上，倒过身子，手代足行，只手一招「倒行逆施」闪电般拍向$n的$l",
	"lvl" : 60,
	"skill_name" : "倒行逆施",
	"damage_type" : "瘀伤"
]),
([
	"action" :"$N左手成掌，缓缓推出一招「废寝忘食」，$n只觉一股强劲无伦的掌管风横压而至",
	"lvl" : 75,
	"skill_name" : "废寝忘食",
	"damage_type" : "瘀伤"
]),
([
	"action" : "$N单臂负后，凝目远眺，一式「孤形只影」，$n略一迟疑，浑身已为$N掌力所绕",
	"lvl" : 90,
	"skill_name" : "孤形只影",
	"damage_type" : "瘀伤"
]),
([
	"action" : "$N左掌一招「饮恨吞声」将$n裹在掌风之中。$n犹如身陷洪涛巨浪，随着$N的掌风转动",
	"lvl" : 105,
	"skill_name" : "饮恨吞声",
	"damage_type" : "瘀伤"
]),
([
	"action" : "却见$N翻身坐倒，旋及旋向空中，凭空一式「六神不安」砸向$n，威力骇人，无法硬接",
	"lvl" : 120,
	"skill_name" : "六神不安",
	"damage_type" : "瘀伤"
]),
([
	"action" : "$N右手在$n脸前一拂，一招「穷途末路」击到$n胸前半尺之处，突然转弯，击在$n的$l",
	"lvl" : 135,
	"skill_name" : "穷途末路",
	"damage_type" : "瘀伤"
]),
([
	"action" : "$N脸上喜怒哀乐，怪状百出，乘$n心神难以自制之际，一招「面无人色」攻向$n的$l",
	"lvl" : 150,
	"skill_name" : "面无人色",
	"damage_type" : "瘀伤"
]),
([
	"action" : "$N忽然狂笑不止，忽又颠哭阵阵，$n正迷沉恍惚中，$N已一式「想入非非」攻向$n的$l",
	"lvl" : 165,
	"skill_name" : "想入非非",
	"damage_type" : "瘀伤"
]),
([
	"action" : "突见$N身形晃动，已攻至$n身畔，一式「呆若木鸡」，乘$n惊愕之时，疾打$n的$l。",
	"lvl" : 180,
	"skill_name" : "呆若木鸡",
	"damage_type" : "瘀伤"
]),
});

int valid_enable(string usage)
{
	return usage == "strike" || usage == "parry";
}

int valid_learn(object me)
{
	int skill = me->query_skill("anran-zhang", 1);

	if ( me->query("max_neili") < 1500)
		return notify_fail("你的内力不够。\n");

	if ( me->query_str(1) < 30 )
		return notify_fail("你没有强劲的膂力无法学黯然消魂掌。\n");

	if ( me->query_con(1) < 26 )
		return notify_fail("你没有雄厚的体质无法学黯然消魂掌。\n");

	if ((int)me->query_skill("yunu-xinjing", 1) < (skill-30)
	&& (int)me->query_skill("anran-zhang", 1) < 230)
		return notify_fail("你的玉女心经火候太浅。\n");
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
	level = (int) me->query_skill("anran-zhang",1);

	for(i = sizeof(action); i > 0; i--) {
		if(level > action[i-1]["lvl"]) {
			j = NewRandom(i, 20, level/5);
			return ([
				"action" : action[j]["action"],
				"lvl" : action[j]["lvl"],
				"damage_type" : action[j]["damage_type"],
        "skill_class"   : MAN,
        "skill_damage"  : 20,
        "skill_attack"  : -10,
        "skill_parry"   : 20,                           
        "skill_dodge"   : 0,
        "skill_kill"    : -20,
        "skill_rush"    : 0,
        "skill_hurt"    : 40,
        "skill_GP"      : 2, 
			]);
		}
	}
}

int practice_skill(object me)
{
	int lvl = me->query_skill("anran-zhang", 1);
	int i = sizeof(action);
	while (i--) if (lvl == action[i]["lvl"]) return 0;

	if ( me->query_temp("weapon") || (int)me->query_temp("secondary_weapon"))
		return notify_fail("练黯然销魂掌必须空手。\n");

	if ( me->query("jingli") < 50)
		return notify_fail("你的体力不够练黯然销魂掌。\n");

	if ( me->query("neili") < 20)
		return notify_fail("你的内力不够练黯然销魂掌。\n");

	me->receive_damage("jingli", 40);
	me->add("neili", -15 );
	return 1;
}


string perform_action_file(string action)
{
	return __DIR__"gm/anran-zhang/" + action;
}

int help(object me)
{
        write(HIB"\n黯然销魂掌："NOR"\n\n");
        write(@HELP
        杨过自和小龙女在绝情谷断肠崖前分手，不久便由神雕带着在海潮之中练功，
        数年之后，除了内功循序渐进之外，别的无可再练，心中整日价思念小龙女，
        渐渐的形销骨立，了无生趣。一日在海滨悄然良久，百无聊赖之中随意拳打脚
        踢，其时他内功火候已到，一出手竟具极大威力，轻轻一掌，将海滩上一只大
        海龟的背壳打得粉碎。他由此深思，创出了一套完整的掌法，出手与寻常武功
        大异，厉害之处，全在内力，一共是一十七招。他生平受过不少武学名家的指
        点，自全真教学得玄门正宗内功的口诀，自小龙女学得「玉女心经」，在古墓
        中见到「九阴真经」，欧阳锋以蛤蟆功和逆转经脉，洪七公与黄蓉授以打狗棒
        法，黄药师授以弹指神通和玉箫剑法，除了一阳指之外，东邪、西毒、北丐、
        中神通的武学无所不窥，而古墓派的武学又于五大高人之外别创蹊径，此时融
        会贯通，已是卓然成家。只因他单剩一臂，是以不在招数变化取胜，反而故意
        与武学通理相反。他将这套掌尘定名为「黯然销魂掌」，取的是江淹「别赋」
        中那一句「黯然销魂者，唯别而已矣」之意。

        要求：
               最大内力 1500;
               后天根骨 26;
               后天膂力 30;
               黯然销魂掌 230 级前大于玉女心经 30 级。

HELP
        );
        return 1;
}
