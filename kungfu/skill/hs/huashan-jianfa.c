// huashan-jianfa.c 华山剑法
// modified by darken@SJ

inherit SKILL;
#include <combat.h>
#include <ansi.h>
#include <skill_class.h>

string query_class() {return "正";}  

mapping *action_male = ({
([
	"action" : "$N一招「白云出岫」，做一起首试，顿时豪情一片，手中$w蓄试点向$n的$l",
	"lvl" : 0,
	"skill_name" : "白云出岫",
	"damage_type" : "刺伤"
]),
([
	"action" : "$N使出「有凤来仪」，剑势飞舞而出，轻盈灵动，$w闪烁不定，刺向$n的$l",
	"lvl" : 10,
	"skill_name" : "有凤来仪",
	"damage_type" : "刺伤" 
]),
([
	"action" : "$N一招「天绅倒悬」，满场游走，$w自上而下划出一个大弧，向$n劈砍下去",
	"lvl" : 30,
	"skill_name" : "天绅倒悬",
	"damage_type" : "刺伤"
]),
([
	"action" : "$N向前跨上一步，手中$w看似来势既缓，又无力道，但是$N突然剑锋一转，使出「白虹贯日」直刺$n的$l",
	"lvl" : 50,
	"skill_name" : "白虹贯日",
	"damage_type" : "刺伤"
]),
([
	"action" : "$N手中的$w一晃，满场游走，一身青衫化成了一片青影，一招「苍松迎客」直刺$n的$l",
	"lvl" : 80,
	"skill_name" : "苍松迎客",
	"damage_type" : "刺伤"
]),
([
	"action" : "$N手中$w斜晃反挑，一式「无边落木」，这一下出招急快，抑且如梦如幻，直逼$n的$l",
	"lvl" : 110,
	"skill_name" : "无边落木",
	"damage_type" : "刺伤"
]),
([
	"action" : "$N突然右手后缩，跟着剑柄疾收，似乎要撞上自己的胸膛，突然一式「青山隐隐」,反身刺向$n",
	"lvl" : 140,
	"skill_name" : "青山隐隐",
	"damage_type" : "刺伤"
]),
([
	"action" : "$N右臂轻提，手中$w提了两尺，剑尖一抬，一招「古柏森森」只等$n撞了上来",      
	"lvl" : 180,
	"skill_name" : "古柏森森",
	"damage_type" : "刺伤"
]),
});

mapping *action_famale = ({
([
	"action" : "$N使出「有凤来仪」，剑势飞舞而出，轻盈灵动，$w闪烁不定，刺向$n的$l",
	"lvl" : 0,
	"skill_name" : "有凤来仪",
	"damage_type" : "刺伤"
]),
([
	"action" : "$N向前跨上一步，手中$w看似来势既缓，又无力道，但是$N突然剑锋一转，使出「白虹贯日」直刺$n的$l",
	"lvl" : 10,
	"skill_name" : "白虹贯日",
	"damage_type" : "刺伤"
]),
([
	"action" : "$N手中的$w一晃，满场游走，一身青衫化成了一片青影，一招「苍松迎客」直刺$n的$l",
	"lvl" : 30,
	"skill_name" : "苍松迎客",
	"damage_type" : "刺伤"
]),
([
	"action" : "$N手中$w斜晃反挑，一式「无边落木」，这一下出招急快，抑且如梦如幻，直逼$n的$l",
	"lvl" : 50,
	"skill_name" : "无边落木", 
	"damage_type" : "刺伤"
]),
([
	"action" : "$N威仪整肃一式「玉井天池」，$w轰然一动，犹如千军万马奔驰而来，疾疾刺向$n的$l",
	"lvl" : 80,
	"skill_name" : "玉井天池",
	"damage_type" : "刺伤"
]),
([
	"action" : "$N剑转一式「叠翠浮青」，俨然长枪大戟，黄沙千里，$w电闪雷鸣，将$n笼罩于重重剑气之中",
	"lvl" : 110,
	"skill_name" : "叠翠浮青",
	"damage_type" : "刺伤"
]),
([
	"action" : "$N左手向外一分，右手$w向右掠出一招「无双无对」，剑气森然地向$n的$l挥去",
	"lvl" : 140,
	"skill_name" : "无双无对",
	"damage_type" : "刺伤"
]),
([
	"action" : "$N轻叹一声，手中$w怀中一抱，一式「落花无意」，$w锵然跃出倏的化作几点带露的桃花，飘然飞向$n的$l",
	"lvl" : 180,
	"skill_name" : "落花无意",
	"damage_type":  "刺伤"
]),
});

mapping *action_19 = ({
([
	"action":HIW"\n$N使一式「悠悠顺自然」，手中$w"+HIW"嗡嗡微振，幻成一条白光刺向$n的$l"NOR,
	"lvl" : 0,
	"skill_name" : "悠悠顺自然",
	"damage_type":  "刺伤"
]),
([
	"action":HIC"\n$N错步上前，使出「来去若梦行」，剑意若有若无，$w"+HIC"淡淡地向$n的$l挥去"NOR,
	"lvl" : 8,
	"skill_name" : "来去若梦行",
	"damage_type":  "割伤"
]),
([
	"action":HIM"\n$N一式「志当存高远」，纵身飘开数尺，运发剑气，手中$w"+HIM"遥摇指向$n的$l"NOR,
	"lvl" : 15,
	"skill_name" : "志当存高远",
	"damage_type":  "刺伤"
]),
([
	"action":HIG"$N纵身轻轻跃起，一式「表里俱澄澈」，剑光如水，一泻千里，洒向$n全身"NOR,
	"lvl" : 20,
	"skill_name" : "表里俱澄澈",
	"damage_type":  "割伤"
]),
([
	"action":BLU"$N手中$w"+HIY"中宫直进，一式「随风潜入夜」，无声无息地对准$n的$l刺出一剑"NOR,
	"lvl" : 30,
	"skill_name" : "随风潜入夜",
	"damage_type":  "刺伤"
]),
([
	"action":HIG"$N手中$w"+HIR"一沉，一式「润物细无声」，无声无息地滑向$n的$l"NOR,
	"lvl" : 40,
	"skill_name" : "润物细无声",
	"damage_type":  "割伤"
]),
([
	"action":WHT"\n$N手中$w"+WHT"斜指苍天，剑芒吞吐，一式「云龙听梵音」，对准$n的$l斜斜击出"NOR,
	"lvl" : 50,
	"skill_name" : "云龙听梵音",
	"damage_type":  "刺伤"
]),
([
	"action":RED"$N左指凌空虚点，右手$w"+RED"逼出丈许雪亮剑芒，一式「万里一点红」刺向$n的咽喉"NOR,
	"lvl" : 60,
	"skill_name" : "万里一点红",
	"damage_type":  "刺伤"
]),
([
	"action":CYN"$N合掌跌坐，一式「我心化云龙」，$w"+CYN"自怀中跃出，如疾电般射向$n的胸口"NOR,
	"lvl" : 70,
	"skill_name" : "我心化云龙",
	"damage_type":  "刺伤"
]),
([
	"action":HIC"\n$N呼的一声拔地而起，一式「日月与同辉」，$w"+HIC"幻出万道光影，将$n团团围住"NOR,
	"lvl" : 80,
	"skill_name" : "日月与同辉",
	"damage_type":  "割伤"
]),
([
	"action":MAG"$N随风轻轻飘落，一式「清风知我意」，手中$w"+MAG"平指，缓缓拍向$n脸颊"NOR,
	"lvl" : 90,
	"skill_name" : "清风知我意",
	"damage_type":  "刺伤"
]),
([
	"action":HIB"$N剑尖微颤作龙吟，一招「高处不胜寒」，切骨剑气如飓风般裹向$n全身"NOR,
	"lvl" : 100,
	"skill_name" : "高处不胜寒",
	"damage_type":  "割伤"
]),
([
	"action":YEL"$N募的使一招「红叶舞秋山」，顿时剑光中几朵血花洒向$n全身"NOR,
	"lvl" : 110,
	"skill_name" : "红叶舞秋山",
	"damage_type":  "刺伤"
]),
});

mapping *action_kf = ({
([
	"action":HBCYN"$N手上$w"+HBCYN"慢慢点出，突然间在空中一颤，一招「泉鸣芙蓉」，剑刃暗发嗡嗡之声，$w"+HBCYN"来路如鬼如魅，顷刻间竟然已绕到了$n背后"NOR,
	"lvl" : 0,
	"skill_name" : "泉鸣芙蓉",
	"damage_type":  "刺伤"
]),
([
	"action":HBCYN"$N全不理会对方攻势来路，使出一招「鹤翔紫盖」，变化莫测地攻向$n，剑势竟无丝毫迹象可寻"NOR,
	"lvl" : 10,
	"skill_name" : "鹤翔紫盖",
	"damage_type":  "刺伤"
]),
([
	"action":HBCYN"$N手中$w"+HBCYN"晃动，「石廪书声」使将出来，剑招奇变横生，$n瞧得心惊神眩，不知$N究竟出了多少招"NOR,
	"lvl" : 20,
	"skill_name" : "石廪书声",
	"damage_type":  "割伤"
]),
([
	"action":HBCYN"$N$w"+HBCYN"如云雾变化，正是一招「天柱云气」，极尽诡奇之能事，动向无定，不可捉摸，$n觉一阵眼花缭乱，顿感穷於应付"NOR,
	"lvl" : 30,
	"skill_name" : "天柱云气",
	"damage_type":  "割伤"
]),
([
	"action":HBCYN"$N突然间身子往斜里窜出，使出「雁回祝融」，听$w"+HBCYN"陡发铮然之音，无数光环乱转，霎时之间已将$n裹在一团剑光之中"NOR,
	"lvl" : 50,
	"skill_name" : "雁回祝融",
	"damage_type":  "割伤"
]),
});

int valid_enable(string usage) { return (usage == "sword") || (usage == "parry"); }

int valid_learn(object me)
{
	object weapon;

	if (!objectp(weapon = me->query_temp("weapon"))
	|| (string)weapon->query("skill_type") != "sword")
		return notify_fail("你使用的武器不对。\n");
	if ( me->query("max_neili", 1) < 100)
		return notify_fail("你的内力不够。\n"); 
	if ( me->query_skill("zixia-gong", 1) < 20)
		return notify_fail("你的紫霞神功火候太浅。\n");
	if ( me->query_skill("sword", 1) < 10 )
		return notify_fail("你的基本剑法还不够娴熟，无法学习华山剑法。\n");
	if ( me->query_skill("poyu-quan", 1) < 15)
		return notify_fail("你的破玉拳太弱，无法学习到华山剑法的精髓。\n");
	return 1;
}

mapping query_action(object me, object weapon)
{
	int i, level, j;

	level = me->query_skill("huashan-jianfa",1);
  if( me->query_temp("hsj_19")) {
		for(i = sizeof(action_19); i > 0; i--) { 
			if(level >= action_19[i-1]["lvl"]) {
				j = NewRandom(i, 20, level/5);
				return ([
					"action": action_19[j]["action"],
					"lvl": action_19[j]["lvl"],
					"damage_type": action_19[j]["damage_type"],
		      "skill_class"   : ZHENG,
		      "skill_damage"  : 0,
		      "skill_attack"  : 0,
		      "skill_parry"   : 0,
		      "skill_dodge"   : 0,
		      "skill_kill"    : 30,
		      "skill_rush"    : 0,
		      "skill_hurt"    : 0,
		      "skill_improve" : 2
				]);
			}
		}
	}
	if( me->query_temp("hsj_kf")) {
		for(i = sizeof(action_kf); i > 0; i--) {
			if(level >= action_kf[i-1]["lvl"]) {
				j = NewRandom(i, 20, level/5);
				return ([
					"action": action_kf[j]["action"],
					"lvl": action_kf[j]["lvl"],
					"damage_type": action_kf[j]["damage_type"],
		      "skill_class"   : ZHENG,
		      "skill_damage"  : 0,
		      "skill_attack"  : 0,
		      "skill_parry"   : 0,
		      "skill_dodge"   : 0,
		      "skill_kill"    : 0,
		      "skill_rush"    : 50,
		      "skill_hurt"    : 0,
		      "skill_improve" : 2
				]);
			}
		}
	}

	if( me->query("gender") != "女性" ) {
		for(i = sizeof(action_male); i > 0; i--) {
			if(level >= action_male[i-1]["lvl"]) {
				j = NewRandom(i, 20, level/5);
				return ([
					"action": action_male[j]["action"],
					"lvl": action_male[j]["lvl"],
					"damage_type": action_male[j]["damage_type"],
		      "skill_class"   : ZHENG,
		      "skill_damage"  : 20,
		      "skill_attack"  : 0,
		      "skill_parry"   : 30,
		      "skill_dodge"   : 0,
		      "skill_kill"    : 0,
		      "skill_rush"    : 0,
		      "skill_hurt"    : 0,
		      "skill_improve" : 2
				]);
			}
		}
	}

	for(i = sizeof(action_famale); i > 0; i--) {
		if(level >= action_famale[i-1]["lvl"]) {
			j = NewRandom(i, 20, level/5);
			return ([
				"action": action_famale[j]["action"],
				"lvl": action_famale[j]["lvl"],
				"damage_type": action_famale[j]["damage_type"],
	      "skill_class"   : ZHENG,
	      "skill_damage"  : 0,
	      "skill_attack"  : 30,
	      "skill_parry"   : 0,
	      "skill_dodge"   : 30,
	      "skill_kill"    : 0,
	      "skill_rush"    : 0,
	      "skill_hurt"    : 0,
	      "skill_improve" : 2
			]);
		}
	}
}

int practice_skill(object me)
{
	int lvl = me->query_skill("huashan-jianfa", 1);
	int i;

	if ( me->query("gender") != "女性") {
		i = sizeof(action_male);
		while (i--) if (lvl == action_male[i]["lvl"]) return 0;
	}
	else {
		i = sizeof(action_famale);
		while (i--) if (lvl == action_famale[i]["lvl"]) return 0;
	}

	if ( me->query("jingli") < 50)
		return notify_fail("你的体力不够练华山剑法。\n");
	if ( me->query("neili") < 20)
		return notify_fail("你的内力不够练习华山剑法。\n");
	me->receive_damage("jingli", 40);
	me->add("neili", -15);
	return 1;
}

string perform_action_file(string action)
{
	return __DIR__"hs/huashan-jianfa/" + action;
}

int ob_hit(object ob, object me, int damage)
{
	object weapon = me->query_temp("weapon");
	int p, ap, dp;
	string msg;

	ap = me->query("combat_exp") /1000 * me->query_int(1);
	dp = ob->query("combat_exp") /1000 * ob->query_int(1);

	p = (int)me->query("qi")*100/(int)me->query("max_qi");

	if ( me->query_skill("huashan-jianfa", 1) < 120
	|| me->query_skill_mapped("sword") != "huashan-jianfa"
	|| me->query_skill_mapped("parry") != "huashan-jianfa"
	|| me->query_skill_prepared("cuff") != "poyu-quan" 
	|| p > 20 
	|| me->is_busy())
		return 0;

	if ( (20 - p) > random(10) && weapon ) {
		msg = "$N手中"+weapon->name()+"被$n一击，脱手飞出！\n"NOR;
		weapon->move(environment(me));
		msg += HIW"$N灵机一动，右手五指急刺，以手指使出一招“金玉满堂”！\n"NOR;
		if (random(ap) > dp/3 ) {
			msg += HBRED"结果正中$n的膻中穴！\n"NOR;
			ob->add_busy(2+random(3));
		} 
		else {
			msg += HBGRN"结果$n早已料到，轻轻闪过。\n"NOR;
		}
		message_vision(msg, me, ob);
		return -damage;
	}
	return 0;
}
