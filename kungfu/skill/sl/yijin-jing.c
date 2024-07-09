// yijin-jing 达摩易筋经
// Modified by snowman@SJ 28/09/2000
// Modified by olives@SJ 5/2/2001
// for exert jingang的百毒不侵的问题
#include <ansi.h>
inherit FORCE;

#include <combat_msg.h>

int valid_enable(string usage)
{
    return usage == "force";
}

#include "/kungfu/skill/force.h"

int valid_learn(object me)
{
	int nb, nh;
	nb = (int)me->query_skill("buddhism", 1);
	nh = (int)me->query_skill("yijin-jing", 1);

	if ( me->query("gender") != "男性" )
		return notify_fail("你非童男之体，不能练习易筋经内功。\n");

	if (me->query("class") && me->query("class") != "bonze")
		return notify_fail("你非禅宗弟子，不能修炼易筋经。\n");

	if ((int)me->query_skill("buddhism", 1) < 100 && nb <= nh )
		return notify_fail("你的禅宗心法修为不够，无法领会更高深的易筋经内功。\n");

	if ((int)me->query_skill("force", 1) < 15)
		return notify_fail("你的基本内功火候还不够，无法领会易筋经内功。\n");

	if ((int)me->query_skill("yijin-jing", 1) > me->query_skill("force", 1) + 10
	  && me->query_skill("yijin-jing", 1) >= 200 )
	       return notify_fail("你的基本功火候未到，必须先打好基础才能继续提高。\n");

	if( me->query("menggu"))
		return notify_fail("你这种佛门败类，如何能够领会佛门神功。\n");

	if ((int)me->query("guilty") > 0 )
		return notify_fail("你屡犯僧家数戒，无法领会更高深的易筋经内功。\n");

	return valid_public(me);
}

void skill_improved(object me)
{
	int skill, lit;
	skill = me->query_skill("yijin-jing", 1);
	lit = me->query_skill("literate", 1);
	if(skill >= 200 && !me->query("yjj/200")){
		tell_object(me, HIW"\n忽然，你感到体内内息奔走，越转越快，瞬时间已经在全身上下运行了一周天，复汇聚于丹田之中！\n"NOR);
		me->add("max_neili", random(lit + skill));
		me->set("yjj/200", 1);
	}
}

int practice_skill(object me)
{
	if((int)me->query_skill("yijin-jing", 1) >= 200 ){
		if( (int)me->query("potential", 1) > 5 && (int)me->query("neili", 1) > 200){
			me->improve_skill("yijin-jing", (int)me->query_int());
			me->add("neili", -100);
			me->add("potential", -1*(1+random(3)));
			return 1;
		}
		else return notify_fail("你现在的修为不足以提高易筋经内功了。\n");
	}
	else return notify_fail("易筋经内功不可随便练习。\n");
}

string exert_function_file(string func)
{
	return __DIR__"sl/yijin-jing/" + func;
}
mapping exercise_msg(object me)
{
	return ([
		"status_msg" : HIR + me->name()+"脸上红光隐现，气象庄严。" NOR,
		"start_my_msg" : "你五心向天，排除一切杂念，内息顺经脉缓缓流动。\n",
		"start_other_msg" : me->name()+"五心向天，一会工夫，从口鼻处缓缓吐出白气，白气越吐越浓，渐渐笼罩了全身。\n",
		"halt_msg" : "$N长出一口气，将内息急速退了回去，站了起来。\n",
		"end_my_msg" : "你将内息走了个小周天，流回丹田，收功站了起来。\n",
		"end_other_msg" : "只见笼罩"+me->name()+"的白气渐渐消散，又从口鼻处吸了回去。\n",
		"heal_msg" : HIY"$N双手合什，盘膝而坐，口中念起“往生咒”，开始运功疗伤。\n"NOR,
		"heal_finish_msg" : HIY"$N缓缓站起，只觉全身说不出的舒服畅快，便道：“善哉！善哉！本门易筋经当真是天下绝学！”\n"NOR,
		"heal_unfinish_msg" : "$N吐出瘀血，缓缓站起，但脸色苍白，看来还有伤在身。\n",
		"heal_halt_msg" : "$N一声：“阿弥陀佛”双袖挥动，压下内息，站起身来。\n",
	]);
}

string *fanzhen_msg = ({
"只震得$n胸内腹中，有如五脏一齐翻转！\n",
"抢先反震，将$n震得脸如白纸，全无血色！\n",
"震得$n心肺欲裂，腾腾腾连退数步！\n",
"功力已布满全身，将$n的力量反震回去！\n",
"震得$n天旋地转，腾腾腾连退七八步！\n",
"和$n一撞，只震的$p胸口气血翻涌！\n",
"$n便如撞在一堵棉花作面，钢铁为里的厚墙上一般，震退数步！\n",
"$n刚碰到$P，突然身子一震，登时飞了出去！\n",
});

int ob_hit(object ob, object me, int damage)
{
	string msg;
	int p, skill, neili, neili1, i = 1, ap, dp;
	object weapon;
	skill = me->query_skill("yijin-jing", 1);
	neili = me->query("neili") + 1 ;
	neili1 = ob->query("neili") + 1;

	if( skill > 300 ) {
		i += me->query_temp("combat_yield");
	}

	if ( me->query_temp("cant_fanzhen")){
		me->delete_temp("cant_fanzhen");
		return 0;
	}

	if(living(me)
	&& me->query_temp("fanzhen")
	&& random(skill) * i + 30 > ob->query_skill("parry", 1)/2
	&& me->query("combat_exp") > ob->query("combat_exp")/2
	&& skill > 199
	&& neili > 500
	&& !me->query("menggu")
	&& living(ob)
	&& !ob->is_visible(me)
	&& random(2)) {
		if ( random(2)) me->set_temp("cant_fanzhen", 1);
		if(!objectp(weapon = ob->query_temp("weapon"))){
			me->add("neili", -(10+random(70)));
			msg = random(2)?HIR"$N身上生出一股反震之力，":HIR"$N身上金刚不坏体神功随心而起，";
			if(neili1 > neili*4*i || random(damage) > (5000 + me->query_con(1) * 40 * i)){
				msg += "但却被$n的劲力一招震碎！\n"NOR;
				me->delete_temp("fanzhen");
				damage = random(damage/2);
			}
			else if(neili1 > neili*3*i) {
				msg += "但却根本无法阻挡$n的劲力！\n"NOR;
				damage = 0;
			}

			else if(neili1 > neili*3/2*i){
				msg += "结果抵消了部分$n的劲力！\n"NOR;
				damage = -damage/2;
			}
			else{
				msg += fanzhen_msg[random(sizeof(fanzhen_msg))]+NOR;
				p = (neili / neili1) * i;
				if( p < 1) p = 1;
				p *= damage/3;
				if(ob->query_skill("force") > me->query_skill("force")*3/2) p = p*2/3;

				if ( me->is_busy())
					p /= 2;

				if(p < 10) p = 10;

				ob->receive_damage("qi", p, me);
				ob->receive_wound("qi", p/5*i, me);

				if(wizardp(me)) tell_object(me, "Fanzhen damage: "+p+"。\n");

				p = (int)ob->query("qi") * 100 / (int)ob->query("max_qi");
				msg += "( $n"+eff_status_msg(p)+" )\n";
				damage = -8000+(-damage);
			}
			message_vision(msg, me, ob);
		}
		else{
			msg = HIR"好似有一堵无形的墙在$N面前阻挡着，";
			switch(weapon->query("skill_type")) {
				case "whip"   : p = 10; break;
				case "sword"  : p = 8; break;
				case "hook"   : p = 6; break;
				case "blade"  : p = 5; break;
				case "club"   :
				case "stick"  : p = 2; break;
				case "staff"  :
				case "axe"    :
				case "hammer" : p = 1; break;
				default       : p = 3; break;
			}
			p *= weapon->query("rigidity");

			ap = me->query("combat_exp", 1) / 100 * (me->query("str") + p);
			dp = ob->query("combat_exp", 1) / 100 * (ob->query("str") + p);

			if( random(ap + dp ) > dp && neili*i > neili1*2/3) {
				message_vision(HIR"结果$N身上生出一股反震之力，$n只觉虎口一热，" + weapon->name() + HIR"差点脱手飞出！\n\n"NOR, me, ob);
				ob->add_busy(1+random(2));
				return -8000+(-damage);
			}
			else if(neili*i > neili1*2){
				msg += "结果$n"HIR"一下子被反弹，震得"+ob->name()+HIR"手臂发麻！\n"NOR;
				ob->start_busy(1+random(3));
				damage = -8000+(-damage);
			}
			else if(neili*i > neili1*3/2){
				msg += "结果$n"HIR"不能前进一丝一毫！\n"NOR;
				damage = -8000+(-damage);
			}
			else if(neili*i > neili1){
				msg += "结果减缓了$n"HIR"前进的速度！\n"NOR;
				damage = -damage*3/2;
			}
			else if(neili*2*i > neili1){
				msg += "但并没有起到什么作用！\n"NOR;
				damage = -damage/4;
			}
			else{
				msg += "但却被$n"HIR"一下打破！\n"NOR;
				me->delete_temp("fanzhen");
				damage = random(damage/2);
			}
			message_vision(msg, me, weapon);
		}
		return damage;
	}
}


int help(object me)
{
	write(HIY"\n易筋经："NOR"\n");
	write(@HELP
	易筋经乃少林寺头等绝技，由内及外，为无上伏魔神功。此内功是少林镇寺
	之宝，毅力一般者无法学成，其三阶段分别为：0-200，200-300，>300。
	初级易筋经可强身健体，提高修为；中级可降妖伏魔，成金刚之体；高级可
	砍妖除魔于无形，御力修身，百毒不侵。


HELP
	);
	return 1;
}
