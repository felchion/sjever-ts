/* 
   wuxiang-zhi.c 无相劫指
   和少林罗汉拳(luohan-quan)互备
   取自佛法十二字“广大，智慧，真如，性海，颖悟，圆觉”。
   2/3/98 by snowman
*/

#include <ansi.h>
inherit SKILL;
#include <skill_class.h>

mapping *action = ({
([
	"action" : "$N将双手拢在衣袖之中，一招「广大无相劫」，暗劲自袖中悄悄涌出，刺向$n",
	"weapon" : "指力",
	"lvl"   : 0,
	"skill_name" : "广大无相劫" ,           
	"damage_type" : "刺伤"
]),
([
	"action" : "$N微微一笑，一招「智慧无相劫」，衣袖连摆，指劲随机弹出，叫人琢磨不透，无法躲避",
	"weapon" : "指风",
	"lvl"   : 10,
	"skill_name" : "智慧无相劫" ,           
	"damage_type" : "刺伤"
]),
([
	"action" : "$N垂手站立，丝毫不动，但闻$n身前哧的一声，闻声不见物，是那招「真如无相劫」",
	"weapon" : "无形指力",
	"lvl"   : 30,
	"skill_name" : "真如无相劫" ,           
	"damage_type" : "刺伤"
]),
([
	"action" : "$N抬头往天，一指「性海无相劫」暗袭$n$l，本人却见丝毫不动，仿若无事发生一样",
	"weapon" : "指力",
	"lvl"   : 60,
	"skill_name" : "性海无相劫" ,           
	"damage_type" : "刺伤"
]),
([
	"action" : "$N一招「颖悟无相劫」向$n点出，闻得一破空之声，却见$P微笑点头，却再无任何动静",
	"weapon" : "指风",
	"lvl"   : 100,
	"skill_name" : "颖悟无相劫" ,           
	"damage_type" : "刺伤"
]),
([
	"action" : "但闻哧哧两声，$n脸色突变，$N却悠闲自在，并无动过手的迹象，这好象是那招「圆觉无相劫」",
	"weapon" : "指风",
	"lvl"   : 200,
	"skill_name" : "圆觉无相劫" ,           
	"damage_type" : "刺伤"
]),
});

int valid_enable(string usage)
{
	if ( this_player()->query("wxz/quest"))
		return usage=="finger" ||  usage=="parry"; 
}


int valid_learn(object me)
{
	if (!me->query("wxz/done")) 
		return notify_fail("你突然感觉自己‘关元穴’上有点麻木，不禁一阵心惊。\n");
	if (me->query_temp("weapon") || me->query_temp("secondary_weapon"))
		return notify_fail("练无相劫指必须空手。\n");
	if ( me->query_skill("yijin-jing", 1) < 180)
		return notify_fail("你的易筋经内功火候不够，无法学无相劫指。\n");
	if ( me->query("max_neili") < 3000 )
		return notify_fail("你的内力太弱，无法练无相劫指。\n");
	if ( me->query_skill("wuxiang-zhi",1) < 200)
		return notify_fail("你的无相劫指修为不够，目前还无法自行修习。\n");
	if ((int)me->query_int(1) > 47)
               return 1;
	if ( me->query_skill("mohe-zhi", 1) < 160
	|| me->query_skill("nianhua-zhi", 1) < 160
	|| me->query_skill("yizhi-chan", 1) < 160 )
		return notify_fail("你还未掌握基本的指法，无法学习无相劫指。\n");
	return 1;
}

void skill_improved(object me)
{
	int skill;
	int neili=me->query("max_neili");
	int poison;

	skill = me->query_skill("wuxiang-zhi", 1);
	if( skill<=100 && skill%20==0 && me->query("wxz/improve")<skill ) {
		tell_object(me, HIG"你练了一阵，不禁觉得心情有些烦乱。\n"NOR);
		tell_object(me,"你的禅宗心法退步了。\n");
		me->set_skill("buddhism",me->query_skill("buddhism",1)-1);
		me->set("wxz/improve",skill);
		return;
	}
	if( skill>100 && skill<=150 && skill%10==0 && me->query("wxz/improve")<skill ) {
		tell_object(me, HIG"你忽然觉得有些神思不属，不禁站了起来。\n"NOR);
		me->set_skill("buddhism",me->query_skill("buddhism",1)-1);
		tell_object(me,"你的禅宗心法退步了。\n");
		me->set("wxz/improve",skill);
		return;
	}
	if( skill>150 && skill<=200 && skill%5==0 && me->query("wxz/improve")<skill ) {
		tell_object(me, HIG"你心烦意乱，不禁从心底升起一阵杀气。\n"NOR);
		me->set_skill("buddhism",me->query_skill("buddhism",1)-1);
		tell_object(me,"你的禅宗心法退步了。\n");
		me->set("wxz/improve",skill);
		return;
	}

	if( skill > 200 ) me->add("wxz/poison",skill+random(skill));
	else return;
	poison=me->query("wxz/poison");
	if (neili/poison>=10) tell_object(me, HIG"你突然感觉自己小腹上‘梁门’、‘太乙’两穴隐隐疼痛。\n"NOR);
	else if(neili/poison>=5) tell_object(me, HIG"你突然感觉自己‘关元穴’上有点麻木，不禁一阵心惊。\n"NOR);
	else if(neili/poison>=2) {
		tell_object(me, HIG"你突然感觉自己阳白、廉泉、风府三处穴道，如万针攒刺，痛不可当。\n"NOR);
		me->unconcious();
	} else if(neili/poison==1) {
		tell_object(me, HIG"你突然感觉体内内息乱窜，无法克制，眼前一黑，昏倒在地。\n"NOR);
		me->add("max_neili", -skill/2);
		me->set_skill("force",me->query_skill("force",1)-1);
		me->set_skill("yijin-jing",me->query_skill("yijin-jing",1)-1);
		me->unconcious();
	} else if(neili/poison==0) {
		tell_object(me, HIG"你突然感觉体内内息澎湃，如洪水般到处流淌乱窜，不禁眼前一黑，倒在地上。\n"NOR);
	/*		
		me->set("max_neili",0);
		me->delete_skill("force");
		me->delete_skill("yijin-jing");
		me->delete_skill("dodge");
		me->delete_skill("shaolin-shenfa");

		me->unconcious();
		*/
		me->die();
                log_file("skills/wxjz",sprintf("%s(%s)修炼无相劫经脉俱断而亡。\n", me->query("name"),getuid(me)), me);
		message("channel:chat",	HIC"【江湖传闻】"+ me->query("name") +"因为强练少林武功，经脉俱断而亡。\n" NOR,users());
	}
}

string query_skill_name(int level)
{
	int i;
	for(i = sizeof(action); i > 0; i--)
		if(level >= action[i-1]["lvl"])
			return action[i-1]["skill_name"];
}

mapping query_action(object me, object weapon)
{
	int i, level, j;
	level   = (int) me->query_skill("wuxiang-zhi",1);
	for(i = sizeof(action); i > 0; i--) {
		if(level > action[i-1]["lvl"]) {
			j = NewRandom(i, 20, level/5);
			return ([
				"action": action[j]["action"],
				"damage_type": action[j]["damage_type"],
				"lvl": action[j]["lvl"],
				"skill_damage" 	: 150 ,
				"skill_attack" 	: 30 ,
				"skill_parry" 	: 30 ,
				"skill_dodge"		: 30 ,
				"skill_kill" 		: -100,
				"skill_rush" 		: -100,
				"skill_hurt" 	  : 150,
				"skill_GP" 			: 2
			]);
		}
	}
}

int practice_skill(object me)
{
	if (!me->query("wxz/done")) 
		return notify_fail("你突然感觉自己阳白、廉泉、风府三处穴道，如万针攒刺，痛不可当。\n");

	if ( me->query("jingli") < 50)
		return notify_fail("你的体力太低了。\n");
	if ( me->query("neili") < 20)
		return notify_fail("你的内力不够练无相劫指。\n");
	me->receive_damage("jingli", 40);
	me->add("neili", -15);
	return 1;
}

string perform_action_file(string action)
{
	return __DIR__"sl/wuxiang-zhi/" + action;
}

int help(object me)
{
	write(
@HELP
无相劫指
    取自佛法十二字“广大，智慧，真如，性海，颖悟，圆觉”。
    上乘佛门功夫，但是随着级别的提高，体内会积聚较多的因为练功积累的戾气，如果
    不及时化解，会使身体受损，可以向无相禅师请教佛法来化解之，如果积聚多多，将
    会带来各种可怕后果，包括降低内力，直到最后走火入魔，终身残废
    可以和罗汉拳(luohan-quan)互备，互备最低要求后天根骨 46，后天悟性 46，该两门
    功夫各 200 级。
HELP
	);
	return 1;
}
