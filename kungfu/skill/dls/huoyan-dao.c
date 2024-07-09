// huoyan-dao.c 火焰刀
// by snowman@SJ

#include <ansi.h>
inherit SKILL;

mapping *action = ({
([
	"action" : "$N内息转动，运劲于双臂，全身骨节一阵暴响，起手一式「示诞生」向$n劈出，将$n全身笼罩在赤热的掌风下",
	"skill_name" : "示诞生",
	"lvl" : 0,
	"damage_type" : "瘀伤",
]),
([
	"action" : "$N面带轻笑，一招「始心镜」，火焰刀内劲由内及外慢慢涌出，$P双掌如宝像合十于胸前，向着$n深深一鞠",
	"skill_name" : "始心镜",
	"lvl" : 20,
	"damage_type" : "震伤",
]),
([
	"action" : "$N双掌合十而又打开，这招「现宝莲」以火焰刀无上功力聚出一朵红莲，盛开的花瓣飞舞旋转，漫布在$n四周",
	"skill_name" : "现宝莲",
	"lvl" : 50,
	"weapon" : "红莲刀气",
	"damage_type" : "割伤",
]),
([
	"action" : "$N面带金刚相，双掌搓圆，使无数炙热的刀气相聚，这招「破法执」犹如一只巨大的手掌，凌空向$n飞抓而下",
	"skill_name" : "破法执",
	"lvl" : 70,
	"damage_type" : "内伤",
]),
([
	"action" : "$N暴喝一声，竟然使出伏魔无上的「开显圆」，气浪如飓风般围着$P飞旋，炎流将$n一步步向着$P拉扯过来",
	"skill_name" : "开显圆",
	"lvl" : 120,
	"damage_type" : "震伤",
]),
([
	"action" : "$N口念伏魔真经，双掌连连劈出，将$n笼罩在炙焰之下，这如刀切斧凿般的「显真常」气浪似乎要将$p从中劈开",
	"skill_name" : "显真常",
	"lvl" : 140,
	"weapon" : "无形刀气",
	"damage_type" : "割伤",
]),
([
	"action" : "$N现宝相，结迦兰，右手「归真法」单掌拍出，半空中刀气凝结不散，但发出炙灼的气浪却排山倒海般地涌向$n",
	"skill_name" : "归真法",
	"lvl" : 160,
	"damage_type" : "瘀伤",
]),
([
	"action" : "$N虚托右掌，一式「吉祥逝」，内力运转，跟着全身衣物无风自动，$P身体微倾，手掌闪电一刀，斩向$n$l",
	"skill_name" : "吉祥逝",
	"lvl" : 180,
	"weapon" : "无形气浪",
	"damage_type" : "割伤",
]),
});

int valid_enable(string usage) { return usage == "strike" || usage == "parry"; }

int valid_learn(object me)
{
	if (me->query_temp("weapon") || me->query_temp("secondary_weapon"))
		return notify_fail("练火焰刀必须空手。\n");
	if ( me->query("max_neili") < 1500)
		return notify_fail("以你的内力修为，还不足以练习火焰刀。\n");
	if ( me->query_con(1) < 30)
		return notify_fail("你的根骨太低，不能驾御火焰刀。\n");
	if ( me->query_skill("longxiang-boruo", 1) < 100)
		return notify_fail("火焰刀需要有龙象般若功第八层以上的火候才能学习。\n");
	if ( me->query_skill("force") < 100)
		return notify_fail("你的基本内功火候太浅，不能学火焰刀。\n");
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
	level = (int) me->query_skill("huoyan-dao",1);

// 	添加 pfm ran
	if( me->query_temp("hyd_ran") ) {
		return ([
			"action" : RED"$N猛的运转内功，聚劲于双掌，火焰刀内劲由内及外慢慢涌出，挟著一股炙热的刀气向$n直涌而去！"NOR,
			"force":300+random(250)+me->query_str(),
			"dodge":random(30)-10,
			"parry":random(10)+10,
			"damage_type" : "瘀伤",
		]);
		me->delete_temp("hyd_ran");
	}

	for(i = sizeof(action); i > 0; i--)
		if(level > action[i-1]["lvl"]) {
			j = NewRandom(i, 20, level/5);
			return ([  
				"action": action[j]["action"],
				"lvl": action[j]["lvl"],
				"damage_type" : action[j]["damage_type"],
				"weapon": action[j]["weapon"],
				"force":250+random(350),
				"dodge":random(30)-10,
				"parry":random(20),
			]);
		}
}

int practice_skill(object me)
{
	int lvl = me->query_skill("huoyan-dao", 1);
	int i = sizeof(action);

	while (i--) if (lvl == action[i]["lvl"]) return 0;
	if ( me->query("jingli") < 50 || (int)me->query("neili") < 20)
		return notify_fail("你的体力不够，练不了火焰刀。\n");
	me->receive_damage("jingli", 40);
	me->add("neili", -15);
	return 1;
}

string perform_action_file(string action)
{
	return __DIR__"dls/huoyan-dao/" + action;
}

int ob_hit(object ob, object me, int damage)
{
	string msg;
	int j, skill, neili, neili1;

	skill = me->query_skill("huoyan-dao", 1);
	neili = me->query("neili");
	neili1 = ob->query("neili");

	if( me->query_temp("fanzhen")) {
		me->delete_temp("fanzhen");
		return 1;
	}

	if( random(skill) > ob->query_skill("parry", 1)/2
	&& me->query("combat_exp") > ob->query("combat_exp")/2
	&& me->query_skill_mapped("parry") == "huoyan-dao"
	&& me->query_skill("longxiang-boruo", 1) > 150
	&& neili > 1000 
	&& skill > 150 
	&& random(neili) > random(neili1)/2
	&& living(me)
	&& !me->query_temp("weapon")
	&& !ob->query_temp("weapon")
	&& !ob->is_visible(me)) {       
		if( me->query_temp("dazhuan_npc") ) neili += neili/2;

		msg = "只听得$N身前嗤嗤声响，「火焰刀」威势大盛，";            
		me->set_temp("fanzhen", 1);
		if ( neili >= neili1+random(neili1)+damage*2){
			msg += "将$n招式上的内力都逼将回来！\n", 
			j = -8000;
		}
		else if(neili >= neili1+damage) {
			msg += "$P手掌扬处，挡住了一些$n攻来的内力！\n", 
			j = -(damage/2+random(damage/2));               
		}
		else {
			msg += "$P展开火焰刀法，封住了一些$n的攻击！\n", 
			j = -damage/3; 
		}
		if ( wizardp(me)) tell_object(me, "Target damage = "+damage+" 。\n");   
		message_vision(msg, me, ob);
		return j;
	}
}

// 添加 pfm ran 的附加威力
mixed hit_ob(object me, object target)
{
	string msg;
	int j;
	j = me->query_skill("strike");

	if( !me->query_temp("hyd_ran") ) return 1;

	if( random(j) > me->query_skill("parry")/2
	 || random(me->query("neili")) > target->query("neili")/2 ) {
		msg = HIR"$n只觉$N的火焰刀排山倒海般的涌来，势如破竹，根本无法抵挡！\n"NOR;
                target->receive_wound("qi", j + random(j/2), me);
	} else {
		msg = HIY"$n只觉$N的火焰刀排山倒海般的涌来，当即运起全内力，准备硬接这一掌！\n"NOR;
		target->recevied_damage("neili", j);
		if( !target->is_busy() )
			target->add_busy(random(2));
	}
	message_vision(msg, me, target);
}

int help(object me)
{
	write(HIR"\n雪山秘技「火焰刀」："NOR"\n");
	write(@HELP
	
	要求：	最大内力 1500 以上；
		后天根骨 30 以上；
		龙象般若功等级 100 以上；
		内功等级 100 以上。     
HELP
	);
	return 1;
}
