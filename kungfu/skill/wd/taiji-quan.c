// taiji-quan.c 太极拳
// write by snowman@SJ
// modify by snowman@SJ 23/02/2000
// Modified by snowman@SJ 24/05/2001

#include <ansi.h>
#include <combat.h>
#include <skill_class.h>
inherit SKILL;

#include <combat_msg.h>
//#define TAIJI    "/kungfu/skill/yinyun-ziqi/taiji"

mapping taiji_names = ([
        "luanhuan":     "「乱环诀」",
        "yinyang":      "「阴阳诀」",
        "dongjing":     "「动静诀」",
        "liangyi":      "「两仪诀」",
        "gangrou":      "「刚柔诀」",
        "zhannian":     "「沾黏诀」",
        "ji":           "「挤字诀」",
        "yin":          "「引字诀」",
        "an":           "「按字诀」",
        "zhuanfan":     "钻翻",
        "luoxuan":      "螺旋",
        "jingheng":     "静恒",
        "kaihe":        "开合",
        "ruanshou":     "软手",
        "xuling":       "虚灵",
]);

int taiji(object me, object victim, object weapon, int damage);

string *parry_msg = ({
"$p双掌一翻，将$P的来势尽数化去，$N急劲就似钻入了一片粘稠之物中间一般。\n",
"$p右肩斜闪，左手手指凭空划了几个圈子，以四两微力，拨动$N千斤巨力。\n",
"$p眼见$P攻到，当即使出一招「揽雀尾」，右脚实，左脚虚，运起“挤”字诀，粘连粘随，右掌已搭住$P左腕，横劲发出。$N身不由主的向前一冲，跨出两步，方始站定。\n",
"不料$p双手一圈，如抱太极，一股雄浑无比的力道组成了一个旋涡，只带得$P在原地急转七八下。\n",
"$p连消带打，双手成圆形击出，随即左圈右圈，一个圆圈跟着一个圆圈，大圈、小圈、平圈、立圈、正圈、斜圈，一个个太极圆圈发出，登时便套得$P跌跌撞撞，身不由主的立足不稳。\n",
"$p使出一招「云手」，左手高，右手低，一个圆圈已将$P套开。\n",
"$p觉气息微窒，当下一招「斜飞势」，将$P引偏。\n",
"$p吸一口气，体内真气流转，右掌挥出，一拒一迎，将$P的力道尽行碰了回去。\n",
"$p双掌一翻，使出一招「云手」，双掌柔到了极处，空明若虚，将$P的急劲尽数化去。\n",
"$p当即双掌一扬，迎着$P接去，待得手掌将触未触之际，施出「揽雀尾式」，脚下“金鸡独立式”，左足关地，右足悬空，全身急转，宛似一枚陀螺。\n",
"就在这电光石火的一瞬之间，$p身子一弓，正是「白鹤亮翅」的前半招，$P的劲力登时落空。\n",
});

string query_parry_msg(object weapon, object victim)
{
	if (victim->query_temp("weapon"))
		return SKILL_D("parry")->query_parry_msg(weapon);
	else
		return parry_msg[random(sizeof(parry_msg))];
}

string *att_msg = ({
"$N接着一招「揽雀尾」，双手划了个半圈，左阴右阳，按向$n的$l",
"$N面对$n，一招「单鞭」，左右两拳由下至上划弧，一前一后摆至平肩",
"$N变招「提手上势」，两臂慢慢提起至胸前，左臂半环，右臂直伸，插向$n的$l",
"$N单脚独立，左手虚按，右手使一招「白鹤亮翅」，变拳成抓向$n的$l插去",
"$N左腿微曲成虚步，一记「搂膝勾步」，左手向前、向下划弧按于$n$l旁",
"$N右臂半环，左手内旋，一招「手挥琶琶」，十指虚张，缓缓挥向$n的$l",
"$N右腿伸直，左手虚划，接着右臂外旋搬出一记「进步搬拦锤」击向$n的$l",
"$N立半马步，左手内旋虚划，右手向下划弧握拳，一记「指裆锤」击向$n的裆部",
"$N双手下划握拳，向后向前划弧成圈，一招「双风贯耳」，连消带打击向$n两太阳穴",
"$N左步成虚，左手向下虚按，右手成阳，使一招「白蛇吐信」，向$n的$l插去",
"$N右脚虚立，左手画圈后抚，一招「高探马」，右手圆转向前，朝$n面门挥去",
"$N施出「伏虎式」，右手阳外划抱住$n，左手阴虚张五指，夹向$n的$l",
"$N左手变掌横于胸前，右手握拳由肘下穿出，一招「肘底看锤」，锤向$n的$l",
"$N左脚前踏半步，右手虚张使一招「海底针」，五指由下向上戳向$n的裆部",
"$N一招「闪通臂」，左脚一个弓箭步抢到$n身前，右手上举外撇，发出太极劲力震向$n",
"$N右脚踏出，左右手握拳虚划，好似怀中抱月，这记「揽月势」一出，便将$n网在太极圈内",
"$N立半马步，坐腿、松胯、旋腰、扣膝，力发于腰，一招「马步靠」，靠向$n的$l",
"$N左腿收起，右脚独立，使一招「金鸡独立」，双拳划开，两个太极圈圈向$n的$l",
"$N进步虚踏，双手握拳划弧挥出，使一式「玉女穿梭」，顺着$n手臂直上，打向$n前胸",
"$N左手划圈，右手经腹前经左肋向前撇出，使一招「反身撇锤」，向$n的$l锤去",
"$N左手虚按，左脚直立，右腿虚划一招「转身蹬腿」，向后旋出踢向$n的$l",
"$N进步一招「栽锤」，左手划太极圈搂左膝，右手握拳向下划弧，锤向$n的$l",
"$N脚踏七星方位，左虚右，双拳交叉成十字，一招「进步七星」，向$n的$l推去",
"$N身体微转，左手略直，右臂微曲，阴阳相合，使一招「倒撵猴」，扛起$n向后直甩出去",
"$N双手伸开，整个上身划出一个大圆弧，一招「转身摆莲」，将$n浑身上下笼罩在重重太极圈中",
"$N右手缓缓后收，左手握拳向前推出，拳意如箭，一招「弯弓射虎」，直奔$n$l而去",
"$N使出一招「云手」，左手高，右手低，一个圆圈已将$n$l套住，太极柔劲随即发出",
"$N轻跳一步「猢狲跃」，双手划太极圈拂开$n的攻击，借力打力抓起$n就往后掷出！",
"当下$N一招「斜飞势」，将$n力量引偏，双拳内划，合阴阳两股劲力一齐推向$n",
"跟着$N换招「抱虎归山」，双拳划开，身体斜转，在$n身后一推，借力打力要将$n推出",
"$N来个「野马分鬃」，转向$n身后，右前左后，两臂平伸在$n身上一靠，想将他震飞",
"$N左右手臂外旋变掌，两手交叉，继而左右分开，内旋下至腹前，屈臂坐腕，再由下向上，出「如封似闭」划前弧线推向$n",
"$N上体微右转，屈膝坐胯，出招「十字手」，两只手臂微内旋，从体两侧向前划弧合抱于胸，两腕搭成十字形，分别向体两侧划弧分掌",
});

string *taiji_level = ({
"揽雀尾","单鞭","提手上势","白鹤亮翅","搂膝勾步","手挥琶琶","进步搬拦锤","指裆锤",
"双风贯耳","白蛇吐信","高探马","伏虎式","肘底看锤","海底针","闪通臂","揽月势","马步靠",
"金鸡独立","玉女穿梭","反身撇锤","转身蹬腿","栽锤","进步七星","倒撵猴","转身摆莲",
"弯弓射虎","云手","猢狲跃","斜飞势","抱虎归山","野马分鬃","如封似闭","十字手",
});

string *taiji_msg = ({
"$N喝道：“看招”，一抱一合双掌推出，去势甚缓，掌力却暗蓄内劲",
"$N转过身来，左掌拍向$n，右掌微颤，吞吐不定",
"$N轻飘飘的一掌拍出，掌力若有若无，若即若离，圈圈连环，氤氲紫气从一个个或正或斜的圆圈中透将出来",
"$N双手圆转，每一招都含着太极式的阴阳变化，精微奥妙，以慢打快、以静制动",
"$N双手一圈，如抱太极，一股雄浑无比的力道组成了一个旋涡",
"$N双手左拨右带，一卷一缠，已化敌于先",
"$N右手虚划，手背贴向$n",
"$N双手翻动，有如流云翻舞",
"$N双手随意划弧，划圆，不成规章，没有一丝轨迹可寻",
"$N双手一翻，连消带打，成圆形击出，随即左圈右圈，一个圆圈跟着一个圆圈发出",
"$N吸一口气，体内真气流转，右掌挥出，一拒一迎，要搭住$n",
"$N右肩斜闪，左手凭空划了几个圈子",
"$N身子半斜，右掌划个圈子向外拖引，拉得$n马上斜身应招",
"$N双掌轻翻，柔到了极处，空明若虚，在$n面前来回划弧",
"$N走一虚步，右手在$n面前连划了几个圆圈",
"$N双掌一摆一抄，靠着$n连走几个虚步，转了个大太极圈",
"$N心平气静，双手自然挥动，宛如在水中回旋，极轻极柔",
"$N划了个「云手」式，双手如云中拨雾，变得忽虚忽实，漂渺不定",
"$N眼看$n攻来，不慌不忙地将双手内旋，如怀抱太极",
"$N左掌阳，右掌阴，目光凝视$n，两掌向$p慢慢合拢，竟是凝重如山，却又轻灵似羽",
"$N左手转了两个圆圈，右手突从圈中穿出，钩向$n",
"$N看得真切，划个圆圈，右臂一伸，便要逮住$n的手臂",
"$N将左手从右至左划了个大圈，右掌顺着左臂也缓缓推出一个小太极圈",
"$N虚步后撤，双手划弧止于胸前，左前右后，缓缓向$n划出",
"$n一招打出，却被$N左手高，右手低，一个太极圆圈套出",
"$N右脚实，左脚虚，运起太极内劲，粘连粘随，一个个圆圈要搭住$n",
"$N轻轻一笑，右臂伸出，在$n身前上下连划了"+chinese_number(1+random(10))+"个圆圈",
"$N轻轻一笑，左臂伸出，在$n身前左右连划了"+chinese_number(1+random(10))+"个圆圈",
});

string *taiji_hit_msg = ({
"$n从未见过这等轻柔无力的掌势，不敢硬接，刚想斜身闪开，已被$N雄浑掌力牢牢制住！\n",
"$p身不由主的向前一冲，却正好跨进了$P圈成的旋涡之中！\n",
"借着$n劲力将$p套住，这一招手势，却是张三丰所传的武当派太极心法，劲成浑圆！\n",
"猛然，$P搭上了$p双手，顺势一圈一推，将$p甩出！\n",
"$n知道厉害，要想挡拒，却被$N顺势一带一震，飞了出去！\n",
"$p一不小心，被圆圈套住，被带得如陀螺般急转数圈！\n",
"$n奋力抵挡，总算脱出了$N的太极圆圈，却发现自己小腹已被无声无息地按了一掌！\n",
"正好$n一招打来，被$N四两拨千斤地一带，凭空震飞出去！\n",
"$P双臂一圈一转，使出太极中的“六合劲”，已将$n圈住！\n",
"就这一下子的接触，$p便被太极劲力套住，身不由己地飞了出去！\n",
"但见$n正要上前抓住$N，却被$P反手一带一推，被自己的劲力震出数步！\n",
"$n想要挡住$N，却招招落空，反被一下圈住，甩了出去！\n",
"$n冷笑著搭上$N的双手，却被身不由己地一带，按在自己身上！\n",
"这一太极圈，如一股雄浑无比的力道，形成的旋涡带得$p无法立足！\n",
"$n一惊之下手部被套，只觉一股大力冲来，把$p震开数步！\n",
"这圆圈一套一转，便有如长江大河，一波接一波震向$n！\n",
"这太极圆转，无使断绝，$n被套住，只觉得一只手掌已按在了$p胸前大穴上！\n",
"太极之势节节贯串，如长江大河，滔滔不绝，带得$n不知所以！\n",
"这式无可闪避，无可抵御，将$n上盘各路全处在$N双掌的笼罩之下！\n",
"轻而易举将$n套住，组成了一个旋涡，把$p身子带得斜移而出！\n",
"这圈虚而又实，套住$n即送即拉，一下将$p扯动！\n",
"双方刚一接触，太极之力忽变，化虚为实，套住了$n！\n",
});

string *super_msg = ({
"一指戳向$n，内劲冲出，发出尖锐的破空之声",
"手臂回转，画出一个大圈，破空之声随之而来，已是无形剑气",
"凝力指尖，顺着右手剑身向前一探，指上发出的劲力宛如一把利剑，直插$n$l",
"右手剑后撤，左手顺势一摆，一道剑气破空而出",
"右手将剑倒转贴臂，左手一探，破空声起，宛如一把锐利的宝剑",
});

string query_class() {return "柔";}

int valid_enable(string usage) { return usage=="cuff" || usage=="parry"; }

int valid_learn(object me)
{
	mapping fam  = me->query("family");
	if (!fam || fam["family_name"] != "武当派" || fam["generation"] != 2)
		return notify_fail("太极拳只能从张三丰真人处学到。\n");
	if (me->query_temp("weapon") || me->query_temp("secondary_weapon"))
		return notify_fail("练太极拳必须空手。\n");
	if ((int)me->query_skill("yinyun-ziqi", 1) < 100)
		return notify_fail("你的氤氲紫气火候不够，无法学习太极拳。\n");
	if ((int)me->query("max_neili") < 500)
		return notify_fail("你目前的内力太弱，无法练太极拳。\n");
	if(me->query("taiji_quest") > me->query("taiji_zsf"))
		return notify_fail("你觉得对太极拳理还不够理解，无法继续练习太极拳。\n");
	return 1;
}

string query_skill_name(int level)
{
	if (level < sizeof(taiji_level)-1)
		return taiji_level[level/6];
	else return taiji_level[sizeof(taiji_level)-1];

}

void skill_improved(object me)
{
	int skill = me->query_skill("taiji-quan", 1);

	if(skill == 50 && !me->query("taiji_quest")) me->set("taiji_quest", 1);
	if(skill == 80 && me->query("taiji_zsf") < 2) me->set("taiji_quest", 2);
	if(skill == 110 && me->query("taiji_zsf") < 3) me->set("taiji_quest", 3);
	if(skill == 140 && me->query("taiji_zsf") < 4) me->set("taiji_quest", 4);
	if(skill == 170 && me->query("taiji_zsf") < 5) me->set("taiji_quest", 5);
	if(skill == 200 && me->query("taiji_zsf") < 6) me->set("taiji_quest", 6);
	me->add("shen", me->query("combat_exp")/30);
	if(skill > 100 && random(3) == 1){
		tell_object(me, CYN"你双手来回比划，将全身护得水泄不通，对防御之术的认识大增！\n"NOR);
		me->improve_skill("parry", skill * me->query_int(), 1);
	}
}

mapping query_action(object me, object weapon)
{
	int i, level, force;
	object ob, victim;
	string msg, type = "瘀伤";

	level   = me->query_skill("taiji-quan",1);
	force = 200;


	if( !me->query_temp("tjq/start") && me->query_temp("taiji")){
		me->set_temp("tjq/start", 1);
		msg = CYN"只见缓缓$N使出太极拳「起手式」：$P双手下垂，手背向外，手指微舒，两足分开平行，接着两臂慢慢提起至胸前，左臂半环，掌与面对成阴掌，右掌翻过成阳掌" NOR;
		type = "震伤";
	}

	else if ( me->query_temp("tj/太极") || level > 350 ){
		if (random( level) > 400) {
			msg = random(2)?"只见$N虚灵顶劲、涵胸拔背、松腰垂臀、沉肩坠肘，双手抱了个太极式的圆圈，纯以意行太极，已达形神合一，心动气动的境界":
					"刹时间$N悟到了太极拳旨中“似松非松，将展未展，劲断意不断”的精微奥妙之处，使出一招犹如行云流水，潇洒无比";
			level *= 2;
			force += 400;
		}
		else if (level > 300)  {
			msg = taiji_msg[random(sizeof(taiji_msg))];
			force += random(350);
			if( level > 400 ) level = 400;
		}
		else {
			msg = att_msg[random(sizeof(att_msg))];
			force += random(350);
			if( level > 400 ) level = 400;
		}
		type = i != 0?"瘀伤":i != 1?"内伤":"震伤";
	}
	else {
		i = level > 198?33:level/6;
		msg = att_msg[random(i)];
	}
	ob = me->select_opponent();
	if( objectp(victim = me->query_temp("tjq/zhannian")) ) {
		if( me->query("neili") < 400 || me->query("jingli") < 100 ) {
			tell_object(me, "\n你发觉自己力气不接，拳行滞缓，「沾黏诀」顿时中断。\n");
			me->delete_temp("tjq/zhannian");
		}
		else if( ob == victim && !ob->query_temp("lost_attack") && random(3) == 1) {
			i = random(me->query("int")/3);
			ob->set_temp("lost_attack", i);
			me->receive_damage("neili", me->query("int"), "内力透支过度");
			me->receive_damage("jingli", me->query("int")/2, "精力透支过度");
			message_vision(RED"\n$n被$N「沾黏诀」带动，行动滞缓，简直无法出招！\n"NOR, me, ob);
			if(wizardp(me) && (string)me->query("env/combat")=="verbose")
				tell_object(me, "Zhannian = "+i+"\n");
		}
	}

	if ( random(level) > 120
	&& level > 350 )
		return ([
			"action" : msg,
			"force" :  force + random(150),
			"damage_type" : random(2)==1?"瘀伤":"内伤",
			"skill_class" 	: ROU,
			"skill_damage" 	: 30,
			"skill_attack" 	: 20,
			"skill_parry" 	: 150,
			"skill_dodge"		: -80,
			"skill_kill" 		: 0,
			"skill_rush" 		: 0,
			"skill_hurt" 	  : 30,			
			"skill_GP" 			: 2	
		]);

	return ([
		"action" : msg,
		"force" :  force,
		"damage_type" : random(2)==1?"瘀伤":"内伤",
		"skill_class" 	: ROU,
		"skill_damage" 	: 10,
		"skill_attack" 	: 10,
		"skill_parry" 	: 100,
		"skill_dodge"		: -80,
		"skill_kill" 		: 0,
		"skill_rush" 		: 0,
		"skill_hurt" 	  : 0,			
		"skill_GP" 			: 2	
	]);
}

int practice_skill(object me)
{
	int i, lvl = me->query_skill("taiji-quan", 1);

	if (lvl < sizeof(taiji_level)-1)
		if (!i = lvl % 6)
			return notify_fail("你对太极拳的「"+taiji_level[lvl/6]+"」这一招还没有什么认识，练习不了。\n");

	if ((int)me->query("jingli") < 40)
		return notify_fail("你的体力太低了。\n");
	if ((int)me->query("neili") < 20)
		return notify_fail("你的内力不够练太极拳。\n");

	me->receive_damage("jingli", 40);

	me->receive_damage("neili", 15);
	return 1;
}

string perform_action_file(string action)
{
	return __DIR__"wd/taiji-quan/" + action;
}


int help(object me)
{
	write(WHT"\n太极拳："NOR"\n");
	write(@HELP
	太极拳，为宋末元初武当张三丰真人所创，合道家之说，冲虚自然，主以慢
	打快，以静制动，後发制人，无为至虚之念的上乘武学。拳出时双手圆转，
	每一招都含着太极式的阴阳变化，精微奥妙，实是开辟了武学中从所未有的
	新天地。诀窍为「虚灵顶劲、涵胸拔背、松腰垂臀、沉肩坠肘」十六个字，
	纯以意行，最忌用力。形神合一，是这路拳法的要旨。
	太极拳劲首要在似松非松，将展未展，劲断意不断，其接力打力为武林一绝。

	要求：	氤氲紫气等级 100 以上；
		内力 500 以上；
		有张三丰亲传拳招。
HELP
	);
	return 1;
}
