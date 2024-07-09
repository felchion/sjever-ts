// qishang-quan.c 七伤拳
// Modifed damage by snowman@SJ 22/08/2000

#include <ansi.h>
inherit SKILL;
#include <skill_class.h>

string query_class() {return "正";}

mapping *action = ({
([
	"action" : "$N在场中暴走，出手既快且狠，呼呼风响，一拳对准了$n背上的$l直击而至",
	"lvl" : 0,
	"damage_type" : "瘀伤"
]),
([
	"action" : "$N以阴柔之气，左手一翻，格开$n手臂，接着砰的一拳，已结结实实打在$n$l",
	"lvl" : 20,
	"damage_type" : "瘀伤"
]),
([
	"action" : "$N乘$n开口说话，猛然出拳打去，只听砰的一声大响，这一次正巧是打在$n$l上",
	"lvl" : 40,
	"damage_type" : "瘀伤"
]),
([
	"action" : "$N拳出如风，砰砰接连四拳，都打在$n身上，但却似不关痛痒，便如清风拂体，柔丝抚身",
	"lvl" : 60,
	"damage_type" : "内伤"
]),
([
	"action" : "$N双目尽赤，一声吆喝，宛似凭空打了个霹雳，猛响声中，一拳直出打在$n的$l之上",
	"lvl" : 80,
	"damage_type" : "瘀伤"
]),
([
	"action" : "$N暗运几口真气，跨上一步，手臂骨格格作响，接着劈的一声，一拳打向$n$l",
	"lvl" : 100,
	"damage_type" : "瘀伤"
]),
([
	"action" : "$N一拳击出，缩回手臂，又发拳打，退了一步，运气数转，重又上前，挺拳猛击",
	"lvl" : 120,
	"damage_type" : "瘀伤"
]),
([
	"action" : "$N双臂一沉一扬，拳如巨刃开山，划出一道如狱气壁，直逼向$n，$n胸口一紧，只觉烦恶欲呕",
	"lvl" : 140,
	"damage_type" : "瘀伤"
]),
([
	"action" : "$N大喝一声，双拳连环虚击，风声未起，拳上暗劲已如怒潮倒灌，海啸逆卷，袭向$n",
	"lvl" : 160,
	"damage_type" : "内伤"
]),
([
	"action" : "$N双目微阖，步法飘忽，轻飘飘击出数拳，却是半点声息也无，$n蓦地察觉微风拂体，拳上阴劲已袭向后心",
	"lvl" : 180,
	"damage_type" : "内伤"
]),
});

string  *msg = ({
"$N一拳击出，拳风中竟带有刚柔三种不同的劲力，分击$n的全身上下三十六道要穴",
"$N暗吸一口真气，双拳平伸，左刚右柔连续打出四拳，带着四种不同之力击向$n的$l",
"$N默运神功，双拳无形无定，突然于空气中出现五种不同的劲力骤然击向$n的前胸",
"$N单拳一握，默运内息，六种不同劲力随着打出的拳风又低又快的击向$n的$l",
"$N拳出如风，砰砰接连七拳，都打在$n身上，但却似不关痛痒，便如清风拂体，柔丝抚身",
"只见$N双目紧闭深运内力，然后猛的大喝了一声，飞身扑下，以七种无比之劲力击向$n的全身",
"$N一声狂啸，双目赤红，拳路大开大阖，每一拳击出如诸神震怒，石破天惊，皆含催破内家真气的大威力",
"$N紧跟着第二拳又出，出手迅如闪电，快的看不清来路，晃得一晃，却直奔$n前胸",
});

int valid_enable(string usage) { return  usage=="cuff" || usage=="parry"; }

int valid_learn(object me)
{
	if (me->query_temp("weapon") || me->query_temp("secondary_weapon"))
		return notify_fail("练七伤拳必须空手。\n");
	if ( !me->query_skill("jiuyang-shengong"))
	if ( me->query_skill("shenghuo-shengong", 1) < 100)
                return notify_fail("你的圣火神功火候不够，无法学七伤拳。\n");
	if ( me->query("max_neili") < 1000)
		return notify_fail("你的内力太弱，无法练七伤拳。\n");
	if (!me->query("xie_qishang") && me->query_skill("qishang-quan", 1) > 50)
		return notify_fail("你没有学过"HIR"七伤拳经"NOR"，无法再继续练习七伤拳。\n");
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
	level   = (int) me->query_skill("qishang-quan",1);
	for(i = sizeof(action); i > 0; i--) {
		if(level > action[i-1]["lvl"]) {
			j = NewRandom(i, 20, level/5);
			return ([
				"action": action[j]["action"],
				"lvl": action[j]["lvl"],
				"damage_type": action[j]["damage_type"],
				"skill_class" 	: ZHENG,
				"skill_damage" 	: 10 ,
				"skill_attack" 	: 0 ,
				"skill_parry" 	: 20 ,
				"skill_dodge"		: 0 ,
				"skill_kill" 		: 20,
				"skill_rush" 		: -50,
				"skill_hurt" 	  : 50,
				"skill_GP" 			: 2
			]);
		}
	}
}

int practice_skill(object me)
{
	int lvl = me->query_skill("qishang-quan", 1);
	int i = sizeof(action);
	while (i--) if (lvl == action[i]["lvl"]) return 0;

	if ((int)me->query("jingli") < 50)
		return notify_fail("你的体力太低了。\n");
	if ((int)me->query("neili") < 20)
		return notify_fail("你的内力不够练七伤拳。\n");
	me->receive_damage("jingli", 40);
	me->add("neili", -15);
	return 1;
}

string perform_action_file(string action)
{
	return __DIR__"mj/qishang-quan/" + action;
}

void skill_improved(object me)
{
	object *ob;
	int i, skill;
	skill = me->query_skill("qishang-quan", 1);
	if(me->query("max_neili") < skill * 15 && skill < 200){
		switch(random(5)){
			case 0 : 
				tell_object(me, HIR"你肩头云门穴突然一阵轻微隐痛，是肺脉伤了！\n"NOR);
				me->add("max_neili", -(30+random(70)));
				break;      
			case 1 : 
				tell_object(me, HIR"你腿上无里穴突然酸痛难当，是肝脉伤了！\n"NOR);
				me->add("combat_exp", -(2500+random(4000)) );
				break;
			case 2 : 
				message_vision(HIR"$N心脉练伤，突然狂性大发、无法抑制了！\n"NOR, me);
				ob = all_inventory(environment(me));
				for(i=0; i<sizeof(ob); i++) {
					if( ob[i]== me ) continue;
					if(!ob[i]->is_killing(me) ) ob[i]->kill_ob(me);
				}
				break;
			case 3 : 
				tell_object(me, HIR"你上臂青灵穴突然一时麻痒难当，是心脉伤了！\n"NOR);
				me->add("max_jingli", -(30+random(70)));
				break;
			case 4 : 
				tell_object(me, HIR"你突然觉得体内阴阳二气串行金木水火土五行，无法抑制了！\n"NOR);
				me->set_skill("qishang-quan", skill-(2+random(3)));
				break;
		}
		me->unconcious();
	}
}

mixed hit_ob(object me, object victim, int damage_bonus)
{
	string msg;
	int lvl, i;
        
	if( damage_bonus < 100 ) return 0;
	if ( victim->query("job_npc")) return 0;

	lvl= me->query_skill("qishang-quan", 1);
	i = 1;

	if( random(lvl) > 50
	&& random(10) > 6
	&& me->query("jiali") > 5
	&& random( me->query("neili") ) > 500
	&& (me->query("neili") + me->query("con") * 30 ) > victim->query("neili") ){
		if( victim->query("neili")/2 < me->query("neili") )
			victim->add_condition("qishang_poison", 2);
		else
			me->add_condition("qishang_poison", 1);
	}

	if( !random(6)
	&& lvl > 150
	&& !me->query_temp("qsq/duanhun")
	&& random(me->query("neili")) > victim->query("neili")/3*2
	&& me->query("neili") > 1000
	&& me->query("max_neili") > 1500
	&& random(me->query("combat_exp")) > victim->query("combat_exp") /3*2
	&& me->query("jiali") > 5 
	&& me->query_skill("jiuyang-shengong", 1)){
		me->add("neili", -50);
		switch(random(7)){
			case 0 :
				msg = MAG"$N凝神定气，使出七伤拳总诀中的「"HIR"损心诀"MAG"」，双拳一股刚猛之力向$n击去。\n"NOR;
				victim->add_condition("qishang_poison", random(3)+2 );
				break;
			case 1 :
				msg =HIB"$N深吸一口气，左拳再使出七伤拳总诀中的「"HIR"伤肺诀"HIB"」，双拳飘忽不定，$n顿时感觉一股阴柔之力迎面而来。\n"NOR;
				victim->receive_damage("jing", lvl, me);
				victim->receive_wound("jing", lvl, me);
				break;
			case 2 :
				msg =HIC"$N右拳接着使出七伤拳总诀中的「"HIR"摧肝肠诀"HIC"」，双拳刚中有柔，向$n击去。\n"NOR;
				victim->add("jingli", -lvl);
				break;
			case 3 :
				msg =HIG"$N凝神定气，使出七伤拳总诀中的「"HIR"藏离诀"HIG"」，双拳柔中有刚，打出一股内缩之力！向$n击去。\n"NOR;
				victim->add("neili", -lvl*2);
				if( victim->query("neili") < 0) 
					victim->set("neili", 0);
				break;
			case 4 :
				msg =HIY"$N凝神定气，使出七伤拳总诀中的「"HIR"精失诀"HIY"」，双拳势如雷霆，将力道直向$n送去。\n"NOR;
				if( !victim->is_busy() )
					victim->start_busy(2 + random(2));
				else
					victim->add_temp("lost_attack", 2);
				break;
			case 5:
				msg =HIM"$N凝神定气，使出七伤拳总诀中的「"HIR"意恍惚诀"HIM"」，向$n送出一股横出之力。\n"NOR;
				message_vision(msg, me, victim);
				return lvl;
				break;
			case 6:
				msg =HIR"$N大喝一声，须发俱张，使出「七伤总诀」，左右双拳连续击出，威猛无俦，打向$n。\n"NOR;
				victim->add_condition("no_perform", 1);
				victim->add_temp("lost_attack", 2);
				break;
		}
		msg +=HIW"$n只觉得$N这一拳之中共有七股不同劲力，变幻百端，犹豫间已被这股力道笼罩全身，不能动弹。\n"NOR;
		message_vision(msg, me, victim);
	}
}
