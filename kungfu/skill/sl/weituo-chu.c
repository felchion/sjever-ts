inherit SKILL;
#include <skill_class.h>

string query_class() {return "妙";} 
 
mapping *action = ({
([
	"action":"$N跃在半空，一招「仙鹤展翅入灵山」，手中$w已化成无数棍影，令$n眼花缭乱，不知所措，连连倒退",
	"lvl" : 0,
	"skill_name" : "仙鹤展翅入灵山",
	"damage_type":"挫伤"
]),
([
	"action":"$N挺$w将$n的招式架住，顺势一招「玉马衔环拜仙宫」，$w上下左右飞快搅动，身随棍走，向$n压了下来",
	"lvl" : 30,
	"skill_name" : "玉马衔环拜仙宫",
	"damage_type":"挫伤"
]),
([
	"action":"$N一招「鸣鹿踏蹄觅仙草」，屈膝俯身，手中$w连点$n下盘，却未等招数用老，猛的一提，向$n的胸腹间戳去",
	"lvl" : 60,
	"skill_name" : "鸣鹿踏蹄觅仙草",
	"damage_type":"挫伤"
]),
([
	"action":"$N突然滚到在地，$n错愕间，一招「金鲤跃水潜天池」，竟从$n的裆下窜过，更不回头，$w反手扫向$n的$l",
	"lvl" : 80,
	"skill_name" : "金鲤跃水潜天池",
	"damage_type":"挫伤"
]),
([
	"action":"$N一招「灵猿献果赴蓬莱」，身形如电，绕着$n飞快奔跑，手中$w一招快似一招，刹那间向$n连打出十六棍",
	"lvl" : 100,
	"skill_name" : "灵猿献果赴蓬莱",
	"damage_type":"挫伤"
]),
([
	"action":"$N大踏步上前，劲贯双臂，手中$w大开大阖，呼呼风声中一招「飞鹰盘旋扫乾坤」扫向$n的腰间",
	"lvl" : 120,
	"skill_name" : "飞鹰盘旋扫乾坤",
	"damage_type":"挫伤"
]),
([
	"action":"$N大喝一声，一招「天龙出水腾宇宙」，$w脱手飞出，夹着劲风射向$n的$l，未等落地，伸手又把$w抄在手中",
	"lvl" : 130,
	"skill_name" : "天龙出水腾宇宙",
	"damage_type":"挫伤"
]),
([
	"action":"$N双目圆睁，口中默诵真言，一招「白象卷云憾天柱」，$w似有千斤，缓缓举起，又缓缓向$n的当头砸落",
	"lvl" : 150,
	"skill_name" : "白象卷云憾天柱",
	"damage_type":"挫伤"
]),
});

int valid_enable(string usage)
{
	if ( this_player()->query("family/family_name") == "少林派")
		return  usage == "club" || usage =="parry"; 
}

int valid_learn(object me)
{
	mapping fam  = me->query("family");
	object weapon;
	int flag = 0;

	if (!objectp(weapon = me->query_temp("weapon"))
	 || (string)weapon->query("skill_type") != "club")
		return notify_fail("你使用的武器不对。\n");

	if (!fam || fam["family_name"] != "少林派" )
		return notify_fail("韦陀杵只能从少林派学到。\n");

	if ( me->query("family/generation") >= 37)
		return notify_fail("你现在的辈份不能修炼少林高级武功韦陀杵。\n");

	//return notify_fail("你已是三长老的及门高弟，如何能修习这种大误禅宗修为的刚猛武功？\n" );
	if( me->query("passdu"))		
		flag = 1;
	//return notify_fail("你已经是心禅堂方生大师的及门高弟，如何能修习这种大误禅宗修为的刚猛武功？\n" );
	if ( me->query("pass_xin"))		
		flag = 1;

	if ( me->query("max_neili") < 1500)
		return notify_fail("你的内力不够。\n");

	if ( me->query_skill("yijin-jing", 1) < 160)
		return notify_fail("你的易筋经内功火候太浅。\n");

	if ( me->query_skill("zui-gun",1) <=120)
		return notify_fail("你的醉棍火候不足，难以领悟韦陀杵。\n");

	if ( me->query_skill("ranmu-daofa",1) || (int)me->query_skill("riyue-bian",1))
		return notify_fail("你如果有达摩老祖的修为，方可同时学习少林三绝技。\n"); 	
	
	if(flag && random(100)==50 ){
		mapping learned = me->query_learned();
		flag = random(3);
		if(flag) tell_object(me,"你苦研佛学，却不得不修习杀人之技，觉得自己与深深在你心中的佛学大师越来越远。\n");
		if(learned["buddhism"]>=flag) learned["buddhism"] -= flag;
		else {
			int level = me->query_skill("buddhism",1);			
			me->set_skill("buddhism",level-1);
			learned["buddhism"] = level*level+learned["buddhism"]-flag;
		}
	}
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
         
	level   = (int) me->query_skill("weituo-chu",1);

	for(i = sizeof(action); i > 0; i--) {
		if(level > action[i-1]["lvl"]) {
			j = NewRandom(i, 20, level/5);
			return ([
				"action": action[j]["action"],
				"damage_type": action[j]["damage_type"],
				"lvl": action[j]["lvl"],
				"skill_class" 	: MIAO,
				"skill_damage" 	: 20 ,
				"skill_attack" 	: 20 ,
				"skill_parry" 	: 30 ,
				"skill_dodge"		: -50 ,
				"skill_kill" 		: 0,
				"skill_rush" 		: 0,
				"skill_hurt" 	  : 50,
				"skill_GP" 			: 2
			]);
		}
	}
}
 
int practice_skill(object me)
{
	if ( me->query("jingli") < 50)
		return notify_fail("你的体力不够练韦陀杵。\n");

	if ( me->query("neili") < 20)
		return notify_fail("你的内力不够练韦陀杵。\n"); 

	me->receive_damage("jingli", 40);
	me->add("neili",-15);
	return 1;
}

string perform_action_file(string action)
{
	return __DIR__"sl/weituo-chu/" + action;
}
