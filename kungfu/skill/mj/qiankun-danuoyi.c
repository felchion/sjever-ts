// qiankun-nuoyi.c 乾坤大挪移
// 1998-4-7
// snowman 1998-3-16

#include <ansi.h>
inherit SKILL;

string  *msg_weapon = ({
YEL"$N回手一挥，想掠斜$w"YEL"来势，哪料到这么轻轻一挥之力，回击的力量竟如此大得异乎寻常！\n"NOR,
RED"哪知$N的$W"RED"微转，平平的搭在$w"RED"之上，一股柔和的劲力送出，将$w"RED"直荡了开去！\n"NOR,
HIY"$N向左踏出两步，右手$W"HIY"挥出，一股劲风扑向$n的后心，方位时刻拿捏得恰到好处！\n"NOR,
HIC"$n攻出的这一招，原是极高明的招数，但被$N手指一拂，劲力又斜斜卸了开去！\n"NOR,
BLU"$N待$n的$w"BLU"将要触及心口时，左足突然踢出，压下$w"BLU"，一借力顺势踏落！\n"NOR,
HIB"$N双手同时施展挪移乾坤心法，一声清啸，拔身而起，在半空中轻轻一个转折，飘然落在$n身旁！\n"NOR,
HIW"$n武功虽高，但于$N刚猛掌力袭体而来，手中$w"HIW"再也难以拆解$N的$W"HIW"招式！\n"NOR,
YEL"$N右手五指犹如抚琴鼓瑟，忽挑忽捻，忽弹忽拨，上身半点不动，片刻间将$n的力道尽数卸了！\n"NOR,
GRN"$N右打出一招泰山剑法「来鹤清泉」，左手却已使出乾坤大挪移的手法，拂向$n手中$w"GRN"！\n"NOR,
CYN"$N单伸$W"CYN"，按在$n$w"CYN"边上，依照所练的乾坤大挪移心法，微一运劲，便将$n的劲力轻轻移开了！\n"NOR,
});

string  *msg_unarmed = ({
HIC"一刹那间$N化刚为柔的急剧转折，已使出属乾坤大挪移心法的第七层神功！\n"NOR,
HIW"$N动也不动，待他进到身前，乾坤大挪移内力斜引，将他力道挪移推卸了开去！\n"NOR,
GRN"岂知刚刚相触，突然一股劲力从$N身上发出，挡住了$n的劲力，$N右手五指顺势按向$n的胸口！\n"NOR,
RED"结果$N使出乾坤大挪移心法，依着八卦方位，倒反了$n这招的去势。\n"NOR,
CYN"$N运使乾坤大挪移，越奔越快，如风如火，似雷似电，跟着纵身而前，左手探出抓向$n！\n"NOR,
BLU"$N此时体内真气流转，一遇外力，自然而然而生反击，比之$n击来的力道又大得多了！\n"NOR,
BLU"$n右手一指戳向$N肩头，却不知如何被轻轻一带，噗的一响，竟戳到了自己手臂，只痛得眼前金星直冒！\n"NOR,
YEL"蓦地里好象旁边有一股力道飞来，将$n的力道一引，从$N身旁斜斜带过！\n"NOR,
YEL"$N当下潜运乾坤大挪移心法，使$n击向$N的一招莫名奇妙的溜滑向一旁。\n"NOR,
HIR"$N再使挪移乾坤心法，一牵一引，托的一响，将$n的力道引偏。\n"NOR,
YEL"$N右手五指犹如抚琴鼓瑟，忽挑忽捻，忽弹忽拔，上身半点不动，将$n此招化于无形！\n"NOR,
WHT"$N巧运乾坤大挪移心法，使$n这招莫名其妙的在中途转了方向，力道化于无形！\n"NOR,
CYN"乾坤大挪移神功变幻，何等奥妙，结果$N手指拂出，已点中了$n双腕穴道！\n"NOR,
GRN"$N使出挪移大乾坤，$n这招莫名其妙的在中途转了方向，打出的力量向自己飞了回来！\n"NOR,
});
int valid_enable(string usage){ return usage == "parry";}

int valid_learn(object me)
{
	if(me->query_skill("qiankun-danuoyi", 1) < 210)
		return notify_fail("乾坤大挪移只能通过研习《乾坤大挪移心法》和领悟来提高。\n");
	return 1;
}

int practice_skill(object me)
{
	if( me->query_skill("qiankun-danuoyi", 1) < 210 )
		return notify_fail("你还没有到能独自练习乾坤大挪移的境界。\n");
	if( me->query("neili") < 50 )
		return notify_fail("你的内力不够了。\n");
	if( (int)me->query("jingli") < 80 )
		return notify_fail("你的体力不够了。\n");
	me->receive_damage("jingli", 70);
	me->receive_damage("neili", 30);
	return 1;
}

void skill_improved(object me)
{
	int skill, lit, med;
	skill = me->query_skill("qiankun-danuoyi", 1);
	lit = me->query_skill("literate", 1);
	med = me->query_skill("medicine", 1);

	if(skill = 50 && !me->query("qkdny") && lit > 50){
		tell_object(me, HIY"你按照羊皮上所书，运气导行、移宫使劲，竟是毫不费力的便做到了。\n"NOR);
		me->add("max_neili", 10+random(30));
		me->add("qkdny", 1);
	}
	if(skill = 80 && me->query("qkdny") == 1 && lit > 80){
		tell_object(me, HIM"你修炼第二层心法，依法施为，片刻真气贯通，只觉十根手指之中，似乎有丝丝冷气射出。\n"NOR);
		me->add("max_jingli", 10+random(30));
		me->add("qkdny", 1);
	}
	if(skill = 100 && me->query("qkdny") == 2 && lit > 100){
		tell_object(me, CYN"你按照羊皮上所书，边读边练，精神为之大振！\n"NOR);
		me->add("max_neili", 10+random(30));
		me->add("qkdny", 1);
	}
	if(skill = 130 && me->query("qkdny") == 3 && lit > 120){
		message_vision(RED"$N半边脸孔胀得血红，半边脸颊却发铁青，但神完气足，双眼精光炯炯，料是无碍。\n"NOR, me);
		me->add("max_jingli", 10+random(30));
		me->add("qkdny", 1);
	}
	if(skill = 150 && me->query("qkdny") == 4 && lit > 140){
		message_vision(HIW"$N脸上忽青忽红，脸上青时身子微颤，如堕寒冰；脸上红时额头汗如雨下。\n"NOR, me);
		me->add("max_neili", 10+random(30));
		me->add("qkdny", 1);
	}
	if(skill = 170 && me->query("qkdny") == 5 && lit > 160){
		tell_object(me, MAG"你觉得体内潜力便如山洪突发，沛然莫之能御。原来是乾坤大挪移心法发挥了你本身所蓄有的潜力！\n"NOR);
		me->add("max_neili", 20+random(30));
		me->add("qkdny", 1);
	}
	if(skill = 190 && me->query("qkdny") == 6 && lit > 180){
		tell_object(me, MAG"你猛地里气血翻涌，心跳加快，终于练成了乾坤大挪移神功！\n");
		me->add("max_neili", 50+random(30));
		me->add("qkdny", 1);
	}
}

string perform_action_file(string action)
{
	return __DIR__"mj/qiankun-danuoyi/" + action;
}

int ob_hit(object ob, object me, int damage)
{
	object wp,wp1, target;
	string msg;
	int p, j, skill, neili, neili1, exp, exp1, xu, qi_wound;

	if( userp(me) && me->query("family/family_name") != "明教" ) return 0;

	target = ob;
	wp = me->query_temp("weapon");
	wp1 = ob->query_temp("weapon");
	skill = me->query_skill("qiankun-danuoyi", 1);
	neili = me->query("neili");
	neili1 = ob->query("neili");
	exp = me->query("combat_exp");
	exp1 = ob->query("combat_exp");
	if( wizardp(me) ) tell_object(me,sprintf("damage = %O\n",damage));

// hua
	if( me->query_temp("qkdny/hua") && living(me) && !me->is_busy() ) {
		if( me->query("jing") < damage/10 || me->query("jingli") < damage/10 ) {
			message_vision(HIW"$N正要把$n的攻势化解，心法却因精力不足运行有误！结果$N虚耗一招。\n"NOR,me, ob);
			me->start_busy(1+random(2));
			return random(damage/5);
		}
		if( COMBAT_D->do_attack(me, target, wp, 0, 1)) {
			message_vision(HIW"$N使用四两拨千斤之术，轻轻的化解了$n的攻势。\n"NOR,me,ob);
			me->add("jing",-damage/10);
			me->add("jingli",-damage/10);
			return -damage;
		}
		else {
			message_vision(HIW"$N使用四两拨千斤之术，化解了$n的大半攻势。\n"NOR,me,ob);
			me->add("jing",-damage/20);
			me->add("jingli",-damage/20);
			return -damage/2;
		}
	}

// yin
	if( me->query_temp("qkdny/yin") && living(me) && !me->is_busy() ) {
		if( me->query("jing") < damage/10
		 || me->query("jingli") < damage/10
		 || me->query("neili") < damage/10 ) {
			message_vision(HIG"$N正要牵引$n的攻势，心法却因精神内力不足运行有误！结果$N虚耗一招。\n"NOR,me, ob);
			me->start_busy(1+random(2));
			return random(damage/5);
		}
		if( COMBAT_D->do_attack(me, target, wp, 0, 1)) {
			message_vision(HIG"$N运起乾坤大挪移引字诀，$n的攻势不知如何攻向了$n自己！\n"NOR,me,ob);
			me->add("jing",-damage/10);
			me->add("jingli",-damage/10);
			me->add("neili",-damage/10);
			ob->receive_damage("qi",damage*2/3, ob);
			ob->receive_wound("qi",damage/5, ob);
			p = ob->query("qi")*100/ob->query("max_qi");
			msg = "( $N"+COMBAT_D->eff_status_msg(p)+" )\n";
			message_vision(msg,ob);
			return -damage;
		}
		else {
			message_vision(HIG"$N运起乾坤大挪移引字诀，牵引开了$n的大半攻势。\n"NOR,me,ob);
			me->add("jing",-damage/20);
			me->add("jingli",-damage/20);
			me->add("neili",-damage/20);
			return -damage/2;
		}
	}

// yi
	if( me->query_temp("qkdny/yi") && living(me) && !me->is_busy() ) {
		if( !objectp(target = me->query_temp("qkdny/yi"))
		|| environment(me) != environment(target) ) {
			me->delete_temp("qkdny/yi");
			tell_object(me, HIY"你发现你想挪移的目标对象已经不在这里了。\n"NOR);
			return 0;
		}
		me->delete_temp("qkdny/yi");

		if( !living(target) ) {
			tell_object(me, HIY"你发现你想挪移的目标对象已经昏迷不醒了。\n"NOR);
			return 0;
		}
		if( neili < 800 ) {
			tell_object(me, HIY"你正要挪移，却发现自己的真气不够了！\n"NOR);
			return 0;
		}

		message_vision(WHT"$N突然双手晃动击出阵阵气流，左拔右带要把$n的招数向旁边众人牵引！\n"NOR,me, ob);
		if( !COMBAT_D->do_attack(me, target, wp, 0, 1)) {
			me->add("jingli", -25);
			me->add("neili", -125);
			message_vision(HIY"结果$n并没有受到干扰，反而顺势来到$N的侧面，发出强力的一击！\n"NOR,me, ob);
			return random(damage);
		}
		me->add("neili", -250);
		me->add("jingli", -50);
		tell_room(environment(me), "结果引得"+ob->name()+"的力道直向"+target->name()+"冲击过去！\n",({ target,ob }));
		tell_object(target, HIR"结果你发现"+ob->name()+"的力道直向自己冲击过来！\n"NOR);
		tell_object(ob, HIR"结果你发现自己的力道直向"+target->name()+"冲击过去！\n"NOR);
		if(target->query_skill("dodge") >= damage)
			msg = HIY"结果$n没有受到任何伤害。\n"NOR;

		else{
			target->receive_damage("qi", damage, ob);
			p = target->query("qi")*100/target->query("max_qi");
			msg = COMBAT_D->damage_msg(damage, "伤害");
			msg += "( $n"+COMBAT_D->eff_status_msg(p)+" )\n";
		}
		if( target->query("shen") < -1000 && !target->is_killing(ob->query("id")) ) {
			if( random(target->query_int()) < 20 )
				target->kill_ob(ob);
			else if( random(target->query_int()) > 30 )
				target->fight_ob(me);
		}
		if( target->query("shen") > -1000 && !target->is_killing(ob->query("id")) ) {
			if( random(target->query_int()) < 20 )
				target->fight_ob(ob);
			else if( random(target->query_int()) > 30 )
				target->fight_ob(me);
		}
		message_vision(msg, me, target);
		return -8000;
	}

// xu
	if( me->query_temp("qkdny/xu") ) {
		if( me->query_temp("qkdny/xu") > 1000
		 || me->query_temp("qkdny/xu") > me->query("neili")/4 ) {
			msg = "\n乾坤大挪移「蓄」字诀已在你体内蓄满了劲力！\n";
			tell_object(me, msg);
			msg = HIY"$N突然间大喝一声，将所积蓄的十余掌的力道归并成为一掌迅急拍向$n。\n"+
				"便如一座大湖在山洪爆发时储满了洪水，猛地里湖堤崩决，洪水急冲而出！世上\n"+
				"原无如此大力，$p倘若受实了，势须立时腕骨、臂骨、肩骨、肋骨一齐折断，连\n"+
				"血也喷不出来，当场成为一团血肉模糊，死得惨不可言！\n"NOR;
			qi_wound = me->query_temp("qkdny/xu") * (4+random(3));
			ob->receive_damage("qi", qi_wound, me);
			ob->receive_wound("qi", qi_wound/3*2+random(qi_wound/3), me);
			if( random(2) ) {
				p = (ob->query("eff_qi")+ob->query_temp("apply/qi"))*100/(ob->query("max_qi")+ob->query_temp("apply/qi"));
				msg += COMBAT_D->damage_msg(damage, "震伤");
				msg += "( $n"+COMBAT_D->eff_status_msg(p)+" )\n";
			}
			else {
				p = ob->query("qi")*100/(ob->query("max_qi")+ob->query_temp("apply/qi"));
				msg += COMBAT_D->damage_msg(damage, "震伤");
				msg += "( $n"+COMBAT_D->status_msg(p)+" )\n";
			}
			me->delete_temp("qkdny/xu");
			message_vision(msg, me, ob);
			return -8000;
		}
		else {
			msg = "乾坤大挪移「蓄」字诀自然而起，将这力量储存了下来。\n";
			tell_object(me, msg);
			xu = ob->query("jiali");
			xu += ob->query_skill("force")/8;
			if( me->query_skill("jiuyang-shengong", 1) )
				xu *= 2;
			if( me->is_busy() ) xu /= 2;
			me->add_temp("qkdny/xu", xu);
		}

		if( wizardp(me) )
			tell_object(me, "Qkdny/xu = "+me->query_temp("qkdny/xu")+"\n");

		if( damage/2 > me->query("neili") ) {
			tell_object(me, HIR"结果对方的力道太大，你一不小心被打散了积蓄的力量！\n"NOR);
			me->delete_temp("qkdny/xu");
			return damage/4;
		}
		ob->add_busy(random(2));
		if( !me->is_busy() )
			me->start_busy(random(2));
		return -damage/3;
	}

	if( me->query_temp("fanzhen") ) {
		me->delete_temp("fanzhen");
		return 1;
	}

	if( me->query("jiali")
	 && living(me)
	 && random(skill) + 20 > ob->query_skill("force", 1)/3
	 && random(exp) > exp1/4 
	 && skill > 100 
	 && neili > 500 
	 && living(ob)
	 && random(2)
	 && !ob->query("env/invisibility") ) {
		if( random(2) ) me->add_temp("fanzhen", 1);
		me->add("neili",  -(random(10)));
		if( wp && wp1 ) {
			msg = msg_weapon[random(sizeof(msg_weapon))];
			msg = replace_string(msg, "$W", wp->name());
			msg = replace_string(msg, "$w", wp1->name());
			if( neili >= neili1*2 && !me->is_busy() ) {
				if( random(me->query_str(1)) > ob->query_str(1)/2 && skill > 150 ) {
					msg += HIW"结果$n一个抵挡不住，手中"+wp1->name()+HIW"差点脱手飞出！\n"NOR,
					me->add("neili", -50);
					ob->add_busy(2+random(2));
					j = -8000;
				}
				else {
					j = damage/2;
					if( j < skill*3/2 ) j = skill*3/2;
					ob->receive_damage("qi", j, me);
					ob->receive_wound("qi", j/5, me);
					p = ob->query("qi")*100/ob->query("max_qi");
					msg += COMBAT_D->damage_msg(j, "伤害");
					msg += "( $n"+COMBAT_D->eff_status_msg(p)+" )\n";
					j = -5000;
				}
			}
			else if( neili >= random(neili1)*2 && !me->is_busy() ) {
				msg += "结果基本上卸掉了$n的力道。\n";
				j = -5000;
			}
			else{
				j = damage/2+random(damage/2);
				if( me->is_busy() ) j = j/2;
				if( j < damage/2 ) msg += "结果卸掉了一些$n的力道。\n";
				else msg += "结果卸掉了$n大半的力道。\n";
				j = -j;
			}
			message_vision(msg, me, ob);
			return j;
		}
		else{
			msg = msg_unarmed[random(sizeof(msg_unarmed))];
			if( COMBAT_D->do_attack(me, ob, wp, 0, 1) ) {
				j = damage/2;
				if( j < skill*3/2 ) j = skill*3/2;
				ob->receive_damage("qi", j, me);
				ob->receive_wound("qi", j/5, me);
				p = ob->query("qi")*100/ob->query("max_qi");
				msg += COMBAT_D->damage_msg(j, "内伤") + "( $n"+COMBAT_D->eff_status_msg(p)+" )\n";
				j = -8000;
			}
			else if( 	COMBAT_D->do_attack(me, ob, wp, 0, 1) ){
				msg += "结果基本上卸掉了$n的力道。\n";
				j = -5000;
			}
			else {
				j = damage/2+random(damage/2);
				if( me->is_busy() ) j = j/2;
				if( j < damage/2 ) msg += "结果卸掉了一些$n的力道。\n";
				else msg += "结果卸掉了$n一半的力道。\n";
				j = -j;
			}
			message_vision(msg, me, ob);
			return j;
		}
	}
}
