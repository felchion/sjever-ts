// 玄阴剑法
#include <ansi.h>
inherit SKILL;

mapping *action = ({
([
	"action" : HIB"$N一举手中的$w"HIB"，向前直攻，正是一招「阴阳绝路」直指$n的$l"NOR,
	"skill_name" : "阴阳绝路",
	"lvl" : 0,
	"damage_type" : "刺伤"
]),
([
	"action" : HIM"$N使出「阴阳轮回」，$w"HIM"左右摇摆不定，摇摇晃晃得刺向$n的$l"NOR,
	"skill_name" : "阴阳轮回",
	"lvl" : 30,
	"damage_type" : "割伤"
]),
([
	"action" : HIW"$N一招「阴风袭体」，$n一呆，$l已经被$N击中"NOR,
	"skill_name" : "阴风袭体",
	"lvl" : 50,
	"damage_type" : "刺伤"
]),
([
	"action" : HIC"$N跃起丈余，手中$w"HIC"画了个弧线，斜斜划下，「阴徊九天」，击向$n的$l"NOR,
	"skill_name" : "阴徊九天",
	"lvl" : 70,
	"damage_type" : "割伤"
]),
([
	"action" : HIG"$N剑交左手，突然挥$w"HIG"反撩而上，一式「玄阴剑」，迅捷无比的攻向$n的$l"NOR,
	"skill_name" : "玄阴剑",
	"lvl" : 90,
	"damage_type" : "刺伤"
]),
});

int valid_enable(string usage) { return (usage == "sword") || (usage == "parry"); }

int valid_learn(object me)
{
	object weapon;
	if (!(weapon = me->query_temp("weapon")))
		return notify_fail("空手时无法练玄阴剑法。\n");
	if ((string)weapon->query("skill_type") != "sword")
		return notify_fail("你使用的武器不对，无法练玄阴剑法。\n");
	if ((int)me->query("max_neili") < 60)
		return notify_fail("你的内力不够。\n");
	if ( me->query_skill("xuanyin-jian", 1) < 150 )
		return notify_fail("你的玄阴剑法等级还不够练习。\n");
	if ((int)me->query_skill("jiuyin-zhengong", 1) < 50)
		return notify_fail("你的九阴真功火候太浅。\n");
	if (me->query_skill_mapped("force") != "jiuyin-zhengong")
		return notify_fail("修习玄阴剑法必须有九阴真气配合。\n");
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

	level   = (int) me->query_skill("xuanyin-jian",1);
	for(i = sizeof(action); i > 0; i--)
	if(level > action[i-1]["lvl"]) {
		j = NewRandom(i, 20, level/5);
		return ([
			"action":	action[j]["action"],
			"lvl":		action[j]["lvl"],
			"damage_type":	action[j]["damage_type"],
			"skill_damage" 	: 50,
			"skill_attack" 	: 50,
			"skill_parry" 	: 0,
			"skill_dodge"		: 0,
			"skill_kill" 		: 0,
			"skill_rush" 		: 0,
			"skill_hurt" 	  : 0,
			"skill_GP" 			: 2
		]);
	}
}
 
int practice_skill(object me)
{
	if ((int)me->query("jingli") < 60 || me->query("neili") < 30)
		return notify_fail("你的体力不够练玄阴剑法。\n");
	me->receive_damage("jingli", 50);
	me->add("neili", -20);
	return 1;
}

string perform_action_file(string action)
{
	return __DIR__"quest/xuanyin-jian/" + action;
}

int ob_hit(object ob, object me, int damage)
{
	object wp,wp1;
	string msg;
	int p,j,skill, neili, neili1, exp, exp1;
	wp = me->query_temp("weapon");
	wp1 = ob->query_temp("weapon");
	skill = me->query_skill("xuanyin-jian", 1);
	neili = me->query("neili");
	neili1 = ob->query("neili");
	exp = me->query("combat_exp");
	exp1 = ob->query("combat_exp");

	if( me->query("jiali") 
	&& skill+50 > random(ob->query_skill("parry", 1)/3)
	&& wp 
	&& skill > 100
	&& me->query_skill_mapped("parry") == "xuanyin-jian"
	&& living(ob)
	&& !me->is_busy()){
		if(wp1){
			msg = HIW"$N剑刃一封，运起九阴真功一震！\n"NOR;
			msg = replace_string(msg, "$w", wp1->name());
			if(neili >= random(neili1+damage)){
				if(random(me->query_str(1)) > ob->query_str(1)/3 && skill > 120){
					msg += HIW"$n手中"+wp1->name()+HIW"被$N一震，险些落手！\n"NOR;
					ob->add_busy(2+random(2));
					j = -(damage);
				}
				else{
					ob->receive_damage("qi", damage/10, me);
					ob->receive_wound("qi", damage/10, me);
					p = ob->query("qi")*100/ob->query("max_qi");
					msg += COMBAT_D->damage_msg(damage/20, "伤害");
					msg += "( $n"+COMBAT_D->eff_status_msg(p)+" )\n";
					j = -(damage/2+skill/2);
				}
			}
			else if(neili >= random(neili1)+damage){
				msg += "结果把$n的攻势阻了一阻。\n";              
				j = -(damage/2+skill/2);
			}
			else{
				j = damage/2+random(damage/2);
				if(j<damage/2) msg += "结果九阴真气把$n大半的力道消解了去。\n";
				else msg += "结果以九阴真气把$n大半的力道消解了去。\n";
				j = -j;
			}
			message_vision(msg, me, ob);
			return j;
		}
		else{
			msg = HIY"$N剑刃一转，向$n中宫直进！\n"NOR;
			if(neili >= neili1+random(neili1)+damage){
				ob->receive_damage("qi", damage/10, me);
				ob->receive_wound("qi", damage/10, me);
				p = ob->query("qi")*100/ob->query("max_qi");
				msg += COMBAT_D->damage_msg(damage, "内伤");
				msg += "( $n"+COMBAT_D->eff_status_msg(p)+" )\n";
				j = -(damage*2+skill);
			}
			else if(neili >= random(neili1)+damage){
				msg += "结果将$n的攻势阻了一阻。\n";              
				j = -(damage+skill);
			}
			else{
				j = damage/2+random(damage/2); 
				if(j<damage/2) msg += "结果九阴真气把$n一些力道转移开去。\n";
				else msg += "结果九阴真气把$n一些力道转移开去。\n";
				j = -j;
			}
			message_vision(msg, me, ob);
			return j;
		}
	}
}
