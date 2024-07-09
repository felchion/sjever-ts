// xianglong-zhang.c 降龙十八掌
// Rebuild By looklove 2001.4.6
// Modify by looklove 2001.5.2 for rumo
// Modify by Looklove 2001.5.6 for slbw
// Modify by Looklove 2001.7.20 for fz
// Modify at 2001.10.05 for bxsj

#include <ansi.h>
inherit SKILL;
#include <combat.h>
#include <skill_class.h>

string query_class() {return "刚";}

string *parry_msg = ({
"$n不待回身，半招「神龙摆尾」，做势反劈。$N见这招势凶狠，不敢硬接，纵身避开。\n",
"只见$n左掌圆劲，右掌直推，挡在身前。一招纯是防御，却似在双方之间布了一道坚壁。\n",
"$n当下不敢怠慢，双掌当胸平推，将自己全身笼罩在掌力之下。守得稳若渊停岳峙，毫无半点破绽。\n",
"$n只觉对方劲力之强无法闪避，无奈之下，$n双眼一闭，大喝一声，双掌上扬，硬接了$N这一招。\n",
"$n脚下一转，左掌架开了$N攻来的招式，右掌暗藏在腰间，伺机反击。\n",
"$n胸有成竹，辨清了$N的招式，双掌一拍，轻轻一笑间，已封住了对方的招式。\n",
});

string query_parry_msg(object weapon, object victim)
{
	if( victim->query_temp("weapon")
	 || victim->query_skill("xianglong-zhang", 1) < 100
	 || victim->query_skill_prepared("parry") != "xianglong-zhang" )
		return SKILL_D("parry")->query_parry_msg(weapon);
	else
		return parry_msg[random(sizeof(parry_msg))];
}

mapping *action = ({
([
	"action": "$N双腿微曲，双掌平平提到胸前，右臂划个圆圈，呼的一声，使出降龙十八掌的第一招「亢龙有悔」拍向$n",
	"lvl" : 0,
	"skill_name": "亢龙有悔",
	"damage_type": "瘀伤"
]),
([
	"action": "但见$N左掌护胸，身体一侧，后转背对$n，右掌紧跟着一摆，一招「神龙摆尾」拍向身后的$n",
	"skill_name": "神龙摆尾",
	"lvl" : 10,
	"damage_type": "瘀伤"
]),
([
	"action": "但见$N一招未完，一招又起，左掌回收护胸，右掌向右横推而出，这是一招「见龙在田」，迅捷无比地劈向$n的$l",
	"lvl" : 20,
	"skill_name": "见龙在田",
	"damage_type": "瘀伤"
]),
([
	"action": "$N凝神闭气，提起双掌齐胸平推，施出「鸿渐于陆」，这招外表看似平凡朴实，但掌力夹却带着隐隐的风声，缓缓压向$n",
	"lvl" : 30,
	"skill_name": "鸿渐于陆",
	"damage_type": "瘀伤"
]),
([
	"action": "$N右掌聚成拳状，一式炮锤格开$n的手臂，接着近身上前左掌迅速推出，这是一招「潜龙勿用」，掌力雄浑涌向$n",
	"lvl" : 40,
	"skill_name": "潜龙勿用",
	"damage_type": "内伤"
]),
([
	"action": "爆喝声中$N飞身上前，单掌凝气，凌空劈出一招「利涉大川」，这降龙掌掌风如刀如刃，迅猛无比地砍向$n的$l",
	"weapon" : "降龙掌掌风",
	"skill_name": "利涉大川",
	"lvl" : 50,
	"damage_type": "割伤"
]),
([
	"action": "$N双掌相并，身体突然前跃，使出「突如其来」这招，掌力排山倒海般向$n正面推出，避无可避",
	"skill_name": "突如其来",
	"lvl" : 60,
	"damage_type": "瘀伤"
]),
([
	"action": "突见$N猛然大吼一声，中宫空门大开，双掌一上一下劈出两道惊人的劲，不顾一切地斩击$n，这是一招「震惊百里」",
	"skill_name": "震惊百里",
	"lvl" : 70,
	"damage_type": "瘀伤"
]),
([
	"action": "$N跨前一步，一招「或跃在渊」，左右双掌向$n连连拍出，一浪接着一浪，掌风更带起地上无数沙石",
	"skill_name": "或跃在渊",
	"lvl" : 80,
	"damage_type": "瘀伤"
]),
([
	"action": "$N身形向前移动，双掌分使两式「双龙取水」，凌厉的掌风一左一右，但却不分先后，同时压向$n$l",
	"skill_name": "双龙取水",
	"lvl" : 90,
	"damage_type": "瘀伤"
]),
([
	"action": "$N左脚微曲，身形飞起，于半空中拍出「鱼跃于渊」，双掌立砍，斧一般的掌劲齐向$n的$l劈下",
	"weapon" : "手掌",
	"skill_name": "鱼跃于渊",
	"lvl" : 110,
	"damage_type": "割伤"
]),
([
	"action": "$N扎个马部，使出一招「时乘六龙」，双掌一正一反，一上一下，一前一后向$n周身连连续续猛烈砍了六下",
	"skill_name": "时乘六龙",
	"lvl" : 130,
	"damage_type": "瘀伤"
]),
([
	"action": "$N使出一招「密云不雨」，左掌将掌力连连拍出，带起旋风封住$n了的退路，右掌跟着斜斜地劈向$n的$l",
        "weapon" : "降龙掌掌力",
	"skill_name": "密云不雨",
	"lvl" : 140,
	"damage_type": "割伤"
]),
([
	"action": "$N弓步向前，使出一招「损则有孚」，双掌看似软绵绵地拍向$n，但劲力过处，地上的沙石连连飞舞，着实潜力不小",
	"skill_name": "损则有孚",
	"lvl" : 160,
	"damage_type": "内伤"
]),
([
	"action": "$N脚下一转，突然欺身到$n身前，右掌斜转护住全身，左掌运劲一招「龙战于野」，猛然砍向$n的$l",
	"weapon" : "掌缘",
	"skill_name": "龙战于野",
	"lvl" : 170,
	"damage_type": "割伤"
]),
([
	"action": "$N一声爆喝，胸前门户大开，双掌相并，齐向$n猛地推去，正是一招至刚的「履霜冰至」，至自己于死地而不顾",
	"skill_name": "履霜冰至",
	"lvl" : 180,
	"damage_type": "瘀伤"
]),
([
	"action": "$N左掌低垂，右掌抬高，使出一招「羝羊触蕃」，两股雄浑的掌力打出，一上一下同时向中间的$n挤压下去",
	"skill_name": "羝羊触蕃",
	"lvl" : 190,
	"damage_type": "瘀伤"
]),
([
	"action": "$N突然一跃飞起半空，身形急速旋转，居高临下，双掌使出一招「飞龙在天」，夹杂着汹涌的罡气直向身下的$n压去",
	"lvl" : 200,
	"skill_name": "飞龙在天",
	"damage_type": "瘀伤"
]),
});

int valid_enable(string usage) { return usage=="strike" ||  usage=="parry"; }

int valid_learn(object me)
{
	if( me->query_temp("weapon") || me->query_temp("secondary_weapon") )
		return notify_fail("练降龙十八掌必须空手。\n");
	if( (string)me->query("family/master_name") != "洪七公" )
		return notify_fail("降龙十八掌只能从洪七公处学到。\n");
	if( (int)me->query_skill("huntian-qigong", 1) < 100 )
		return notify_fail("你的混天气功火候不够，无法练降龙十八掌。\n");
	if( me->query("gender") == "无性" )
		return notify_fail("你一个不男不女之人怎能学习降龙掌？\n");
	if( (int)me->query("max_neili") < 1000 )
		return notify_fail("你的内力太弱，无法学降龙十八掌。\n");
	if( (int)me->query_str(1) < 27 )
		return notify_fail("你没有雄壮的臂力无法学降龙十八掌。\n");
	if( (int)me->query_con(1) < 26 )
		return notify_fail("你没有雄厚的体质无法学降龙十八掌。\n");
	return 1;
}

string query_skill_name(int level)
{
	int i = sizeof(action);
	while (i--)
		if(level > action[i]["lvl"])
			return action[i]["skill_name"];
}

mapping query_action(object me, object weapon)
{
	int i, j,level;
  
  level = (int)me->query_skill("xianglong-zhang", 1);
	for(i = sizeof(action); i > 0; i--)
		if(level >= action[i-1]["lvl"]) {
			j = NewRandom(i, 20, level/5);
			return ([
				"action" : action[j]["action"],
				"lvl" : action[j]["lvl"],
				"damage_type" : action[j]["damage_type"],
				"skill_class" 	: GANG,
				"skill_damage" 	: 30 ,
				"skill_attack" 	: 0 ,
				"skill_parry" 	: 30 ,
				"skill_dodge"		: -10 ,
				"skill_kill" 		: -100,
				"skill_rush" 		: -100,
				"skill_hurt" 	  : 100,
				"skill_GP" 			: 2
			]);
		}
}

int practice_skill(object me)
{
	int lvl = (int)me->query_skill("xianglong-zhang", 1);
	int i = sizeof(action);

	while (i--) if (lvl == action[i]["lvl"]) return 0;

	if( me->query("jingli") < 50 )
		return notify_fail("你的体力太低了。\n");
	if( me->query("neili") < 20 )
		return notify_fail("你的内力不够练降龙十八掌。\n");
	me->receive_damage("jingli", 40);
	me->receive_damage("neili", 15);
	return 1;
}

string perform_action_file(string action)
{
	return __DIR__"gb/xianglong-zhang/" + action;
}




int help(object me)
{
	write(HIW"\n降龙十八掌："NOR"\n");
	write(@HELP
	降龙十八掌，为丐帮世代相传震帮之宝。威力无匹，是外家刚猛一类的功夫。
	讲究攻击时的爆发力，全力一击，当者披靡。号称天下最霸道的空手。

	学习要求：
		混天气功等级 100 以上；
		内力 1000 以上；
		后天膂力 27 以上；
		后天根骨 26 以上；
		由洪七公亲传，或解谜后可得。
HELP
	);
	return 1;
}
