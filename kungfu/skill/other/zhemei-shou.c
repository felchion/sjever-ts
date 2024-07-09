// zhemei-shou 天山折梅手
// cck 14/6/97

inherit SKILL;

int lvl_temp;

string *action_msg = ({
"$N单手立起，一指突出，融合剑法与指法精义，结构严谨，气势宏伟，指指点点戳向$n",
"$N单臂伸出，手掌做刀，掌缘布满真力，砍斫纵横，上劈下切，各路刀法与斧法合成一路急攻$n",
"$N双手显得渐渐沉重，使动时越来越滞涩，犹如舞动一根钢杖，而且越来越重，似无止境",
"$N十指探出，随手挥洒，于轻灵飘逸中化去了$n的招数，却蕴含有猛烈的反击招式",
"$N双臂弯出，甩将开来，柔若无骨，灵动之极，东一绕，西一缠，逼得$n竟连一口气也喘不过来",
});

int valid_enable(string usage)
{
	return usage == "hand" || usage == "parry";
}

int valid_combine(string combo)
{
	return combo=="liuyang-zhang";
}

mapping query_action(object me, object weapon)
{
	lvl_temp = random((int)me->query_skill("zhemei-shou", 1));
	if ( lvl_temp > 80 && ! random(3)) {
		if ( lvl_temp <= 120 )
			return ([
				"action":	"$N右手一弯，掌上凝聚阴柔之力，无声无息劈向$n的「至阳穴」，跟着左拳猛击而出，击向$n胸口的「膻中穴」",
				"damage_type":	"内伤",
				"damage":	60,
				"dodge": 	random(30),
				"parry":	random(30),
				"force":	450 +random(100),
			]);

		if ( lvl_temp <= 180 )
			return ([
				"action":	"$N左手袍袖一拂，激出一股内劲，袖力中夹着掌力，一前一后击向$n",
				"damage_type":	"内伤",
				"damage":	70,
				"dodge":	30,
				"parry":	random(30),
				"force":	500,
			]);

		if ( lvl_temp <= 240 )
			return ([
				"action":	"$N手腕转了个小圈，既快且准，三只手指搭上$n手腕，大拇指和小指跟着便即收拢，扣住$n脉门，$n顿时动弹不得",
				"damage":	80,
				"damage_type":	"瘀伤",
				"parry":	random(30),
				"dodge":	30,
				"force":	550,
			]);

		if ( lvl_temp <= 300 ) {
			return ([
				"action":	"$N双手连抓，使出「天山折梅手」的高深武功，抓中带扣，扣中夹掌，连攻$n的命门大穴",
				"damage_type":	"瘀伤",
				"damage":	90,
				"dodge":	20,
				"parry":	random(30),
				"force":	600,
			]);
		}
		return ([
			"action": "这时$N内力生发，将「逍遥」二字发挥得淋漓尽致，姿势优雅美观，潇洒如意\n"
				 "然而逍遥派武学的诸般狠辣招数自然而然的也使了出来，所使招数渐趋阴险刻毒\n"
				 "$N招招凶险，皆攻向$n要害，直欲致$n死得惨酷无比",
			"damage_type": "瘀伤",
			"dodge": 	50,
			"parry": 	50,
			"damage":	100,
			"force": 	650,
		]);

	}
	return ([
		"action":	action_msg[random(sizeof(action_msg))],
		"damage_type":	random(2)?"瘀伤":"内伤",
		"parry":	random(30),
		"dodge":	random(30),
		"force":	300 + random(350),
		"damage":	50 + random(50),
	]);
}

int practice_skill(object me)
{
	if ((int)me->query("jingli") < 50)
		return notify_fail("你的体力不够练天山折梅手。\n");
	if ((int)me->query("neili") < 30)
		return notify_fail("你的内力不够练天山折梅手。\n");
	me->receive_damage("jingli", 50);
	me->add("neili", -20);
	return 1;
}

int valid_learn(object me)
{
	if(!me->query("tonglao/ok"))
		return notify_fail("你无法学习天山折梅手。\n");
	if (me->query_temp("weapon") || me->query_temp("secondary_weapon"))
		return notify_fail("练天山折梅手必须空手。\n");
	if ((int)me->query("max_neili") < 500)
		return notify_fail("你的内力不够。\n");
//	if ((int)me->query_skill("bahuang-gong", 1) < 100)
//		return notify_fail("你的八荒六合唯我独尊功火候太浅。\n");
	return 1;
}

string perform_action_file(string action)
{
	return __DIR__"other/zhemei-shou/" + action;
}

mixed hit_ob(object me, object target, int damage_bonus, int factor)
{
	int damage, amount;
	if( lvl_temp > 80) {
		if( (lvl_temp <= 100) && (me->query("neili") > 30)) {
			target->add_condition("cold_poison", random(2)+1);
			me->add("neili", -30);
			return 1;
		 }
		 if( (lvl_temp <= 120) && (me->query("neili") > 50) ) {
			damage = (int)me->query_skill("zhemei-shou", 1);
			damage = damage/2 + random(damage);
			target->receive_damage("qi", (random(4)+ 1)*damage);
			target->receive_wound("qi", damage/2);
			me->add("neili", -50);
			return 1;
		 }

		 if( (lvl_temp <= 140) && (me->query("neili") > 50)
		    && (random(me->query("combat_exp")) > target->query("combat_exp")/3)) {
			target->start_busy((int)me->query_skill("zhemei-shou", 1) / 20);
			me->add("neili", -50);
			return 1;
		 }
		 if( (lvl_temp <= 160) && (me->query("neili") > 50)
			 && (target->query_temp("weapon")) 
			 && (random(me->query("combat_exp")) > target->query("combat_exp")/3)) {
//			"cmds/std/drop"->main(target,target->query_temp("weapon"));
			target->query_temp("weapon")->move(me);
			me->add("neili", -50);
			return 1;
		 }

		 if( (lvl_temp > 160) 
		&& (me->query("neili") > 100) 
		&& (me->query_temp("marks/add") != 1)) {
                        amount = (int)me->query_skill("zhemei-shou", 1) / 3;
			me->add_temp("apply/attack", amount);
			me->add_temp("apply/defense", amount);
			me->add_temp("apply/dodge", amount);
			me->add("neili", -100);
			me->set_temp("marks/add", 1);
			call_out("reset_temp", 10, me);	
			return 1;	
		}

	 }
}

int reset_temp(object me)
{
	int amount;

	if (!me) return 0;
	amount = (int)me->query_skill("zhemei-shou", 1) / 3;

	me->add_temp("apply/attack", -amount);
	me->add_temp("apply/defense", -amount);
	me->add_temp("apply/dodge", -amount);
	me->delete_temp("marks/add");
	return 1;
}
