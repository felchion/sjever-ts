// 玄铁剑法
// Modify By River 99.05
// Modified by darken@SJ

#include <ansi.h>;
inherit SKILL;
#include <skill_class.h>

string query_class() {return "刚";}

string  *msg = ({
"$N当胸平平刺出一剑，看似毫无变化，却有轰轰之声，$n只觉一股大力由$w剑身直逼周身",
"$N凌空跃起，身随$w起，借助下行之势向下猛劈一剑，将$n周身笼于剑气之下",
"$N轻叹一声，随手横劈一剑，缓缓指向$n，当真平淡中蕴涵所有变化，无招胜有招",
"$N迅疾挑出一剑，此剑之快，匪夷所思，$n惊恐之际一染、已然无法分辨剑路来去",
"$N提$w顺刺，逆击，横削，倒劈，无不是剑法的入门，当真返朴归真，已不存任何破绽",
"$N仰首长啸，令风云变幻，旋及$w剑气弥漫于$n周身各处，$n但觉身处惊涛骇浪之中", 
"$N祭起手中$w，左手捏成剑诀，咄喝一声，空中满是剑影，宛若千道银链，在$n身边游动",
"$N连出十余剑，一时迅疾如流星划空，一时凝重迟缓，招招无迹可寻，平淡至极又匪思至极", 
"$N手中$w剑势如虹，无可当锋，$n惊怖中更觉剑力有如海潮一般，一波一浪阵阵涌来，绵延不绝",
"$N手中$w向前挑出，看似无甚劲力，启知剑势将老，内力却如海潮一般汹涌，向$n袭去",
"$N远远挥出一剑，$n不料气劲如此之强，防范一疏，一股大力劈头而来",
"$N深吸一口气，手中$w挟带风声，平平淡淡的挥向$n。风声越来越响，竟如同暴风之中一般",
"$N一凝神，$w居然发出淡淡的一层暗青色光芒，如狂风怒潮一边，向$n卷去",
});
//增加古墓飞跃 但是注意 威力没有丝毫调整
//by tangfeng
string  *special_msg = ({
"$N大喝一声，接着你周围布满了密密麻麻的剑气，眼见$n浑身上下被这剑气笼罩",
"$N信手而挥，剑势如风似幻无迹可寻，根本无招却胜过有招，$n惊鄂莫名",
"$N洒出点点剑意，犹如春雨细密柔和，却绵绵不绝无穷无尽，让$n无从招架",
"$N忽然托起手中长剑，在身前化了一个圈犹如太极，$n忽感好似身陷大海漩涡之中，举足艰难",
"$N在四周似是随意挥剑，$n莫名其妙之时却感到剑气似海浪一般，一浪接着一浪一浪高过一浪",
"$N剑意纵横，长剑有若迢迢长龙，宛如秋风绵绵，悠然而不狂烈，但劲力破空，疾奔$n而去",
"$N目光澈如湖水，淡淡的看着$n,手中长剑蓄而不发，$n却觉得身在大海般暗流汹涌难以抗拒",
"$N深吸一口气，手中$w挟带风声，平平淡淡的挥向$n。风声越来越响，竟如同暴风之中一般",
});

string  *super_msg = ({
"$N随手挥出一剑，去势极缓却隐隐含着“隆隆”的风雷之声，已经达到当年独孤求败的至高境界",
"$N手中$w交左手，右手凌空指向$n，$p忽觉凌厉剑气迎面而来，原来$N已入无剑胜有剑的境界",
"$N抬手一剑，急如飞虹闪电却无丝毫声息，寂静以极，已窥剑法之巅",
"$N一剑刺到，泛起淡淡海蓝，无声无息，内力却是极强",
"$N每出一剑都和周围事物相应合，招式毫无征兆随意而创，剑之所达意之所向",
"$N心中竟无半点波澜，纯以意运剑，长剑每挥一剑，便涌出一股气劲，好似要去吞没对手",
"$N突然之间招数全无，只是极平常的一刺，却已经意在剑前。招式越是平平威力却是越大",
"$N剑气如风，含劲而无式，举手投足之间内劲浑厚，剑法已然反璞归真而无招自胜",
});

string *parry_msg = ({
"$p不挡不架，手中$w向$P胸口直刺，$N眼见攻击即将得手，却不得不避开$w的锋芒。\n",
"$p算准$P攻势，一剑横在胸前，$P只得变招后退。\n",
"$p力灌剑身，猛力一击，$P的招式还未近身，便尽数震开。\n",
"$p随手一点，无招无式，只是劲力强极，$P只得弃攻转守，以求自保。\n",
"$p料敌机先，$P处处受制，没一招能够使全，如同小儿习武，手忙脚乱一般。\n",
});

int valid_enable(string usage)
{
	return usage == "sword" || usage == "parry";
}

mapping query_action(object me, object weapon)
{
	int level = me->query_skill("xuantie-jianfa", 1);
	string action_msg;
	if(level> 450
	&& me->query("jiali")
	&& me->query("max_neili") > 5500
	&& me->query("neili") > 5000
	&& !random(5)
	&& me->query_skill_mapped("parry") == "xuantie-jianfa")	{
		return ([
			"action": HBRED"$N潜运内力将毕生功力凝聚在$w"+HBRED"之上缓缓刺出一剑，只是一招挺剑击刺却是挡无可挡避无可避无坚不摧。"NOR,
			"force": 200+random(350),
			"damage": 140+random(80),
			"skill_class"   : GANG,
			"skill_damage"  : 30,
			"skill_attack"  : 10,
			"skill_parry"   : 20,
			"skill_dodge"   : -20,
			"skill_kill"    : 0,
			"skill_rush"    : -20,
			"skill_hurt"    : 30,
			"skill_GP"      : 2,
			"damage_type"		: random(2)?"刺伤":"割伤"
		]);
	}

	if(level<350)
	action_msg = msg[random(sizeof(msg))];
	else if(level<450)
	action_msg = HIC+replace_string(replace_string(special_msg[random(sizeof(special_msg))], "$w", "$w"), "$w", "$w"HIC)+NOR;
	else
	action_msg = HIB+replace_string(replace_string(super_msg[random(sizeof(super_msg))], "$w", "$w"), "$w", "$w"HIB)+NOR;
	return ([
		"action": action_msg,
		"force": 200+random(350),
		"damage": 140+random(80),
		"skill_class"   : GANG,
		"skill_damage"  : 30,
		"skill_attack"  : 0,
		"skill_parry"   : 20,
		"skill_dodge"   : -20,
		"skill_kill"    : 0,
		"skill_rush"    : 20,
		"skill_hurt"    : 0,
		"skill_GP"      : 2,
		"damage_type"		: random(2)?"刺伤":"割伤"
	]);
}

int practice_skill(object me)
{    
	object weapon;
	if (!objectp(weapon = me->query_temp("weapon"))
	 || (string)weapon->query("skill_type") != "sword")
		return notify_fail("你使用的武器不对。\n");
	if (me->query("jingli") < 50)
		return notify_fail("你的体力不够练玄铁剑法。\n");
	if (me->query("neili") < 20)
		return notify_fail("你的内力不够练玄铁剑法。\n");
	me->receive_damage("jingli", 40);
	me->add("neili", -15);
	return 1;        
}

int valid_learn(object me)
{
	if (me->query_skill("xuantie-jianfa", 1) < 200 )
		return notify_fail("玄铁剑法先期只能通过在山洪中修炼来提高技艺。\n");
	return 1;
}

string perform_action_file(string action)
{
	return __DIR__"gm/xuantie-jianfa/" + action;
}

//by tangfeng
//注意这里没有任何效果只增加描述！！！
//也希望其他巫师不要擅自修改破坏平衡
mixed hit_ob(object me, object victim, int damage_bonus)
{
	object weapon;
	int i;
	string msg;

	weapon = me->query_temp("weapon");
	if( !objectp(weapon) ) return;
	i = me->query_skill("xuantie-jianfa", 1);

	if(i> 150
	&& me->query("jiali")
	&& me->query("max_neili") > 1500
	&& me->query("neili") > 1000
	&& random(10) >= 7
	&& (random(me->query_int()) > 30 ||random(me->query_dex()) > 30)
	&& !me->query_temp("xtjf_pfm")
	&& me->query_skill_mapped("parry") == "xuantie-jianfa"
	&& random(me->query("combat_exp")) > victim->query("combat_exp")/3)
	{
		if(i<350)
		msg = HIB"$N怒啸一声，"+weapon->name()+HIB"顿时泛出青蓝色的光彩，一股蓝色剑芒激刺而出！\n"NOR;
		else
		{
			if(!me->query_temp("wujian") || !random(4))
			msg = HBBLU"突然"+weapon->name()+HBBLU"发出耀眼的光芒，整个空间立即充满了汹涌澎湃的杀气！\n"NOR;
			else
			msg = HBBLU"$N"+weapon->name()+HBBLU"的散发出的无形剑气仿佛千万点寒星飞舞，向$n的胸口深深地穿去。\n"NOR;
		}
		message_vision(msg, me,victim);
	}
}
