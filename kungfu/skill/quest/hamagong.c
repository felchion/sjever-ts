// hamagong.c 蛤蟆功
// Modified by snowman 01/08/2000


#include <ansi.h>
#include <combat.h>

inherit FORCE;

#include <combat_msg.h>

mapping *action = ({
([
	"action": "$N忽而倒竖，一手撑地，身子横挺，只以一掌向$n的$l拍出",
	"lvl" : 0,
	"damage_type": "瘀伤"
]),
([
	"action": "$N双手在地上一撑，身子忽地拔起，一跃三尺，落在$n的面前，单掌向$p直劈而下",
        "weapon" : "掌缘",
	"lvl" : 0,
	"damage_type": "割伤"
]),
([
	"action": "$N骤然间飞身跃起，双足向$n连环猛踢，$P只是心中装疯，出手却比真疯时更是厉害",
	"lvl" : 20,
	"weapon": "牙齿",
	"damage_type": "拉伤"
]),
([
	"action": "$N突然伸手在自己脸上猛抓一把，又反足在自己肾上狠踢一脚，一掌击出，中途方向必变，实不知将打何处",
	"lvl" : 50,
	"damage_type": "瘀伤"
]),
([
	"action": "突然之间，$N俯身疾攻，上盘全然不守，微微侧头，一口唾沫往$n$l吐去",
        "weapon" : "唾液",
	"lvl" : 70,
	"damage_type": "刺伤"
]),
([
	"action": "$N丝毫不动声色，双腿微曲，右掌平伸，左掌缓缓运起蛤蟆功的劲力，呼的一声推向$n",
	"lvl" : 90,
	"damage_type": "瘀伤"
]),
([
	"action": "$N默念口诀运气经脉，只觉愈转愈是顺遂，当下一个翻身跃起，咕的一声叫喊，双掌对着$n拍出",
	"lvl" : 110,
	"damage_type": "瘀伤"
]),
([
	"action": "$N微微侧头，裂嘴一笑，一口咬向$n",
	"lvl" : 130,
	"weapon": "牙齿",
	"damage_type": "拉伤"
]),
([
	"action": "但见$N双腿微曲，双掌会收，然后向$n一上一下，一放一收，斗然击出",
	"lvl" : 140,
	"damage_type": "瘀伤"
]),
([
	"action": "突然之间，$N俯身疾攻，上盘全然不守，怪异无伦，忽然伸手向$n$l猛抓一把",
	"lvl" : 110,
	"damage_type": "抓伤"
]),
([
	"action": "$N闭眼逼住呼吸，过了片刻，猛跃而起，眼睛也不及睁开，双掌便向$n的方向推了出去",
	"lvl" : 190,
	"damage_type": "内伤"
]),
([
	"action": "$N脚步摇摇幌幌的看上去马上就会跌倒，忽然双腿一弯、一登，口中阁的一声叫喝，手掌向$n猛然推出",
	"lvl" : 220,
	"damage_type": "瘀伤"
]),
});

int valid_enable(string usage) 
{ 
	if (this_player()->query("quest/hama/pass"))
	return usage == "force" || usage=="strike" || usage =="parry"; 
}

mapping exercise_msg(object me)
{
	return ([
		"status_msg" : HIW + me->name()+"半蹲在地上，身冒白气。" NOR,
		"heal_msg" : HIW"$N全身放松，半蹲在地上开始运功疗伤。\n"NOR,
		"heal_finish_msg" : HIW"$N“哇！”的大叫一声，全身的白气渐渐消散。\n"NOR,
		"heal_unfinish_msg" : "$N“哇！”的大叫一声，站了起来，但脸色苍白，看来还有伤在身。\n",
		"heal_halt_msg" : "$N身冒白气，“哇！”的大叫一声，站起身来。\n",
	]);
}

string query_skill_name(int level)
{
        int i = sizeof(action);
	while (i--)
		if(level > action[i]["lvl"])
			return action[i]["skill_name"];
}

mapping query_action(object me)
{
	int i, level, j;
	level   = (int) me->query_skill("hamagong",1);

	if ( me->query_temp("hmg_super_hit")
	&& random(me->query_str(1)) > 35 
	&& random(level) > 150
	&& me->query("max_neili") > 3000 
	&& me->query("neili") > 1300 ) {
		me->add("neili", -50);
		me->delete_temp("hmg_super_hit");
		return ([
			"action":	HIC"$N蹲低身子，两眼微闭，口中咕咕咕的叫了三声，双掌向$n缓缓推出！激得灰泥弥漫，尘土飞扬" NOR,
			"damage_type":	"瘀伤",
			"skill_damage" 	: 100 ,
			"skill_attack" 	: 100,
			"skill_parry" 	: 50,
			"skill_dodge"		: 0,
			"skill_kill" 		: 0,
			"skill_rush" 		: 0 ,
			"skill_hurt" 	  : 50 ,			
			"skill_GP" 			: 2	
			
		]);
	}
	if( !random(10) )
		me->add_temp("hmg_super_hit", 1);

	for(i = sizeof(action); i > 0; i--) {
		if(level > action[i-1]["lvl"]) {
			j = NewRandom(i, 20, level/5);
			return ([
				"action":	action[j]["action"],
				"lvl":		action[j]["lvl"],
				"damage_type":	action[j]["damage_type"],
				"skill_damage" 	: 30 ,
				"skill_attack" 	: 0,
				"skill_parry" 	: 50,
				"skill_dodge"		: 0,
				"skill_kill" 		: 0,
				"skill_rush" 		: 0 ,
				"skill_hurt" 	  : 50 ,			
				"skill_GP" 			: 2	
			]);
		}
	}
}

int valid_learn(object me)
{
	if( me->query_skill("hamagong", 1) < 100 )
		return notify_fail("蛤蟆功不是随便什么地方都可以练习的。\n");
	return 1;
}

string exert_function_file(string func)
{
	return __DIR__"quest/hamagong/" + func;
}

string perform_action_file(string action)
{
	return __DIR__"quest/hamagong/" + action;
}

void skill_improved(object me)
{
	int skill = me->query_skill("hamagong", 1);
	
	if( skill > me->query("hmg/lian_level") )
		me->set("hmg/lian_level", skill);

	else return;
	
	if( skill % 2 == 0 ){
		me->add("max_jingli", random(8)+1);
		tell_object(me, "你的蛤蟆功增加了你的精力修为！\n");
	}
	else	{
		me->add("max_neili", random(8)+1);
		tell_object(me, "你的蛤蟆功增加了你的内力修为！\n");
	}
}

int practice_skill(object me)
{
	if((int)me->query_skill("hamagong", 1) >= 100 ){
		if( (int)me->query("potential", 1) > 5 && (int)me->query("neili", 1) > 200){
			me->improve_skill("hamagong", (int)me->query_int());
			me->add("neili", -150);
			me->add("potential", -1*(1+random(3)));
			return 1;
		}
		else return notify_fail("你现在的修为不足以提高蛤蟆功了。\n");
	}
	else return notify_fail("蛤蟆功不可随便练习。\n");
}

string *parry = ({
"$P突然身形一让，微微侧头，一口唾沫往$p$l吐出，去势猛烈！",
"$P斗然张口急咬，看来似乎滑稽，但因张口快捷，教人难以躲闪！",
"$P竟然料敌机先，发掌击向$p的方位，同时又是一口浓痰吐将过来！",
"但见$p的手掌已相距不及一寸，$P蓦地张口，一副白牙一闪，已向$p手上咬落！",
"$P又是一口唾沫急吐，竟将痰涎唾沫也当作了攻敌利器，夹在拳招之中使用！",
"$P蛤蟆功逆转运用，上者下之，左者右之，一掌拍回$p！",
"$P右手亦出，将$p招术化开，左手向$p猛抓几下！",
"$p这招刚触到$P，$N微微侧头，一口咬回$p！",
"$P虽倒行九阴，耳目四肢只有比平时更为灵敏，眼见$p手掌抹到，立即出掌还击！",
"$P逆运九阴，更是厉害，见$p招来，便口吐白沫，举头猛撞！",
});

int ob_hit(object ob, object me, int damage)
{
	string msg, *limbs;
	mapping prepare;
	int ap, dp, j = 0;


	if( me->query_temp("fanzhen")){
		me->delete_temp("fanzhen");
		return 1;
	}

	if( ob->query("env/invisibility") ) return 0;

	if (!me->query_temp("hmg_nizhuan")
	|| me->query_skill_mapped("force") != "hamagong"
	|| me->query_skill_mapped("strike") != "hamagong"
	|| me->query_skill_mapped("parry") != "hamagong"
	|| me->query("combat_exp") < ob->query("combat_exp")/2
	|| ob->query_temp("weapon")
	|| me->query_temp("weapon") ) return 0;

	prepare = ob->query_skill_prepare();
	if (!sizeof(prepare)) msg = "unarmed";
	else msg = keys(prepare)[0];

	ap = COMBAT_D->skill_power(ob, msg, SKILL_USAGE_ATTACK) / 1000;
	dp = COMBAT_D->skill_power(me, "parry", SKILL_USAGE_DEFENSE) / 1000;
	if ( ob->query("neili") > me->query("neili") * 2 ) ap *= 2;
	if ( random(10) < 3 ) me->set_temp("fanzhen", 1);

	ap = ABS(ap);
	dp = ABS(dp);

	if ( random( dp * 2 ) > ap ) {
		me->add_temp("fanzhen",1);
		if ( random(me->query_skill("hamagong", 1)) < ob->query_skill("parry")/2 )
		msg = "只见这招着实打在$n身上，可$p茫然未觉，全不理会！\n";
		else {
			limbs = ob->query("limbs");
			msg = parry[random(sizeof(parry))];
			msg = replace_string(msg, "$l", limbs[random(sizeof(limbs))]);
			msg += "大出$N意料之外！\n";
			if( random(10) < 8 ){
				ob->receive_damage("qi", damage/2 , me);
				ob->receive_wound("qi", damage/4 , me);
				j = (int)ob->query("eff_qi") * 100 / (int)ob->query("max_qi");
				msg += damage_msg(damage, "伤害") + "( $n"+eff_status_msg(j)+" )\n";
			}

			else{
				msg += "幸而$P顺势倒翻，消解得快未受重伤，但半身酸麻，一时之间已无法再上。\n";
				ob->receive_damage("qi", damage/4 , me);
				ob->receive_wound("qi", damage/8 , me);
				j = (int)ob->query("eff_qi") * 100 / (int)ob->query("max_qi");
				msg += damage_msg(damage, "伤害") + "( $n"+eff_status_msg(j)+" )\n";
				ob->start_busy(2);
			}
		}
		message_vision(msg, ob, me);
		return -8000;
	}

	else if( random(ap + dp) > ap ) {
		msg = parry[random(sizeof(parry))];
		msg += "$n猛然间被$N吓了一大跳，这招不由得慢了几分！\n";
		ob->start_busy(1);
		message_vision(msg, me, ob);
		return -damage/2;
	}

	else return 0;
}

int help(object me)
{
	write(HIC"\n蛤蟆功："NOR"\n");
	write(@HELP
	蛤蟆功为南宋时天下高人欧阳锋所创。此功不同于其它武艺，惯走偏门，另
	辟捷径，常使人防不胜防！欧阳锋曾有一段发疯失忆的时期，也使他练就了
	逆运九阴这种开天辟地的武功，和蛤蟆功相互配合，可称得上是天下第一奇
	功！
	
HELP
	);
	return 1;
}
