// lanhua-shou.c 兰花拂穴手
// Modified by Numa 19990929
// Modified by Darken@SJ 20000415

#include <ansi.h>
#include <skill_class.h>

inherit SKILL;

string query_class() {return "柔";}

string *xue_name1 = ({ 
        "大锥穴",
        "百会穴",
        "志室穴",
        "肩井穴",
        "劳宫穴",
        "内关穴",
        "外关穴",
        "会宗穴",
});

string *xue_name2 = ({
        "曲池穴",
        "冲阳穴",
        "臂儒穴",
        "白海穴",
        "灵台穴",
        "风府穴",
        "神风穴",
        "玉书穴",
}); 

mapping *action = ({
([
	"action" : "$N左手忽做兰花之形，五指轻拢慢抹，拂向$n的任脉诸穴",
	"lvl" : 0,
	"skill_name" : "兰花手左手式",
	"damage_type" : "擦伤"
]),
([
	"action" : "$N右手轻摇，颤抖不定，便如兰花浴风般划向$n",
	"lvl" : 30,
	"skill_name" : "兰花手右手式",
	"damage_type" : "擦伤"
]),
([
	"action" : "$N抢到$n身后，双手成圈，左掌自外向右，右掌自上而下，一抄一带，双手拂向$n要穴所在。",
	"lvl" : 60,
	"skill_name" : "擒拿式",
	"damage_type" : "瘀伤"
]),
([
	"action" : "$N右掌顺拍，掌出即收，左掌随到，双手如两只玉蝶，在眼前上下翻飞拂向$n",
	"lvl" : 90,
	"skill_name" : "舞蝶式",
	"damage_type" : "瘀伤"
]),
([
	"action" : "$N左掌忽起，如一柄长剑般横削而来，掌风虎虎的拂向$n",
	"lvl" : 120,
	"skill_name" : "落叶式",
	"damage_type" : "瘀伤"
]),
});

int valid_enable(string usage) { return usage=="hand" || usage=="parry"; }

int valid_learn(object me)
{
	object nvxu;

	if (me->query_temp("weapon") || me->query_temp("secondary_weapon"))
		return notify_fail("学兰花拂穴手必须空手。\n");

	if ((int)me->query_skill("bihai-chaosheng", 1) < 100)
		return notify_fail("你的碧海潮生功火候不够，无法学兰花拂穴手。\n");

	if (me->query("thd/finish") == 1) {
		return 1;
	}
	else if (me->query("gender") == "男性") {
		if (!(nvxu = find_object("/clone/npc/huang-son-in-law")))
			nvxu = load_object("/clone/npc/huang-son-in-law");
		if (nvxu->query("winner") != me->query("id") || nvxu->query("death_count") < me->query("death_count")) 
			return notify_fail("兰花拂穴手乃黄岛主家传绝学，不传外姓。\n");
	}
	else if (me->query("gender") == "女性") {
		if (me->query("class") != "bonze" || me->query("family/family_name") != "峨嵋派" || me->query("family/generation") > 3)
			return notify_fail("兰花拂穴手乃峨嵋派祖师郭襄秘学，普通弟子是学不到的。\n");
	}
	else
		return notify_fail("兰花拂穴手乃黄岛主家传绝学，不传外姓。\n");
	if ((int)me->query("max_neili") < 300)
		return notify_fail("你的内力太弱，无法使用兰花拂穴手。\n");
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
	int i, level, j,bonus=0;
	level   = (int) me->query_skill("lanhua-shou",1);
	if (me->query("gender")=="男性") bonus=20;
	if ( 40-bonus > random(160)
	&& random(level) > 100 
	&& me->query_skill("force") >= 150 
	&& me->query("neili") >= 200 ) {
		me->add("neili", -50);
		me->set_temp("autopfm_lhs", 1);
		return ([
			"action"				: HIC"$N双手交叠，使出「"HIR"兰花手双手式"HIC"」，形状便如兰花新蕾初放，于轻风中微微摇摆。\n"CYN"$n看得有些发呆，却蓦然发现$N的十指已拂到面前" NOR,
			"damage_type" 	: "瘀伤",
			"skill_class" 	: ROU,
			"skill_damage" 	: 20+bonus,
			"skill_attack" 	: 50-bonus,
			"skill_parry" 	: -20,
			"skill_dodge"		: -20,
			"skill_kill" 		: 50-bonus,
			"skill_rush" 		: 20,
			"skill_hurt" 	  : 0,			
			"skill_GP" 			: 2			
		]);
	}
	me->delete_temp("autopfm_lhs");
	for(i = sizeof(action); i > 0; i--) {
		if(level > action[i-1]["lvl"]) {
			j = NewRandom(i, 20, level/5);
			return ([
				"action": action[j]["action"],
				"lvl": action[j]["lvl"],
				"damage_type": action[j]["damage_type"],
				"skill_class" 	: ROU,
				"skill_damage" 	: -10+bonus,
				"skill_attack" 	: 20-bonus,
				"skill_parry" 	: 10,
				"skill_dodge"		: 20,
				"skill_kill" 		: 30-bonus,
				"skill_rush" 		: 0,
				"skill_hurt" 	  : -20,
				"skill_GP" 			: 1	
			]);
		}
	}
}


int practice_skill(object me)
{
	int lvl = me->query_skill("lanhua-shou", 1);
	int i = sizeof(action);
	mapping learned = me->query_learned();

	if (!mapp(learned)) learned = ([]);
	if (!learned["lanhua-shou"])
		while (i--) 
			if (lvl == action[i]["lvl"])
				if (me->query("gender") == "女性")
					return notify_fail("你将已经学会的所有招式练习了一遍，发现如果不去找师父学新的招式就无法获得进步了。\n");
				else
					return notify_fail("你将已经学会的所有招式练习了一遍，发现如果不去找岳父学新的招式就无法获得进步了。\n");

	if ((int)me->query("jingli") < 40)
		return notify_fail("你的体力太低了。\n");
	if ((int)me->query("neili") < 20)
		return notify_fail("你的内力不够练兰花拂穴手。\n");
	me->receive_damage("jingli", 30);
	me->add("neili", -10);
	return 1;
}

mixed hit_ob(object me, object victim, int damage_bonus, int factor)
{
	int hurt, lvl = me->query_skill("lanhua-shou",1);
	int i =1;
	string name1,name2;
	object weapon = victim->query_temp("weapon");
	name1 = xue_name1[random(sizeof(xue_name1))];
	name2 = xue_name2[random(sizeof(xue_name2))];        
  
  if (lvl < 300) return 0;
  if ( me->query_temp("thd/feiying") ) return 0;
  if (victim->query_temp("lhs/auto")) 
  	me->delete_temp("autopfm_lhs");		
  if (userp(me)) i=2;
  if ( me->query_temp("thd/fuxue") ) i *= 3;     
	if ( me->query_temp("autopfm_lhs") ) {
		hurt = (victim->query("neili") < lvl ? victim->query("neili") : lvl );
		victim->add("neili", -hurt);
		victim->receive_wound("qi", -lvl*4, me);
		victim->receive_hurt("qi", -lvl*2, me);
		me->delete_temp("autopfm_lhs");
		return CYN "$n被双手式同时拂中了「"RED + name1 + CYN"」和「" RED + name2 + CYN"」，只觉得全身巨痛，真气在经脉里四处乱窜,一时竟然无法控制！\n" NOR;
	} 
	else
  if ( (me->query("gender")=="男性" && !random(24/i)) 
		||(me->query("gender")=="女性" && !random(12/i)) ) {
		switch (random(3)) {
			case 0:
				victim->add("neili", -lvl*20);
				if (victim->query("neili")<0) victim->set("neili", 0);
				if (weapon){
					weapon->unwield();
					weapon->move(environment(victim));
					victim->add_busy(2+random(2));
					return CYN "$n被落叶式拂中了「"HIR+name2+CYN"」，手一麻，"+weapon->query("name")+CYN"把持不住，掉在地上。\n" NOR; 
				} 
				else {
					victim->add_busy(4+random(2));
					return CYN "$n被落叶式拂中了「"HIR+name2+CYN"」，手一麻，丹田也受到了振荡！\n" NOR; 				
				}
				break;
			case 1: 
				victim->add_temp("apply/pfm_damage",-50-lvl/5);
				victim->add_temp("apply/armor",-50-lvl/5);
				victim->add_temp("apply/pfm_dodge",-100-lvl/20);
				victim->add_temp("apply/pfm_parry",-100-lvl/20);
				victim->set_temp("lhs/auto", 1); 
        call_out("remove_effect", 30, victim, lvl);
				return CYN "$n被舞蝶式连续拂中了任脉诸穴，战力大减！\n" NOR;
				break;
			case 2: 
				victim->add_busy(1 + random(2));
				victim->set_temp("lhs/auto", 1); 
				if (weapon) {
					victim->map_skill(weapon->query("skill_type"));
					victim->set_temp("no_enable",1);
				} 
				else {
					victim->prepare_skill("leg");
					victim->prepare_skill("hand");
					victim->prepare_skill("strike");
					victim->prepare_skill("cuff");
					victim->prepare_skill("claw");
					victim->prepare_skill("finger");
					victim->set_temp("no_prepare", 1);
				}
				call_out("remove_special", 30, victim);
				return CYN "$n被擒拿式拂中了「"HIR+name1+CYN"」，武功的特殊威力无法施展出来。\n" NOR;
				break;
		}
	}
}

string perform_action_file(string action)
{        
	return __DIR__"thd/lanhua-shou/" + action;
}

void remove_effect(object victim, int lvl)
{
	if (!victim) return;
	victim->delete_temp("lhs/auto");
	victim->add_temp("apply/pfm_damage",50+lvl/5);
	victim->add_temp("apply/armor",50+lvl/5);
	victim->add_temp("apply/pfm_dodge",100+lvl/20);
	victim->add_temp("apply/pfm_parry",100+lvl/20);
	message_vision(HIW"$N的任脉诸穴血脉行开了。\n"NOR, victim);
}

void remove_special(object victim)
{
	if (!victim) return;
	victim->delete_temp("lhs/auto");
	victim->delete_temp("no_enable");
	victim->delete_temp("no_prepare");
}
