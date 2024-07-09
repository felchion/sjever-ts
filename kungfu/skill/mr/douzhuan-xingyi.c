// douzhuan-xingyi 斗转星移
// 1997-2-14

inherit SKILL;

#include <ansi.h>
#include <combat.h>

string *parry_msg = ({
YEL"蓦地里一股力道飞来，将$N一引一带，$N身子轻轻越起，身行落在一旁。\n",
HIB"$N当下一股内力冲向$l，$n击向$N的$l的招数莫名奇妙的溜滑向一旁。\n",
HIW"$N双手前探，左臂搭上$n右臂，右臂使出$n的招数将$n此招化去。\n",
HIY"$N星移斗转，$n这招莫名其妙的在中途转了方向，直奔自己袭来。\n",
YEL"$N作壁上观，双手与对方相交数掌，谈笑间竟使出$n成名技艺，使得$n不知如何是好。\n",
HIB"$N凝神运气向$n猛攻快打，使出的招数好似$n的成名绝技，把$n的招数化于无行。\n",
HIW"只见$N身如飘风，恰似漫天星移，诡异地一式，双掌动向无定不可捉摸地拍向$n的$l。\n",
HIY"$N身行飘忽不定，在星移斗转之间反用$n的招数击向$n，却令人$n百思不得其解。\n",
});

string *colors = ({YEL,HIB,HIW,HIY,YEL,HIB,HIW,HIY});

int valid_enable(string usage)
{
	return usage == "parry";
}

int valid_learn(object me)
{
	return 1;
//	return notify_fail("斗转星移只能通过领悟来提高。\n");
}

int practice_skill(object me)
{
	return notify_fail("斗转星移只能通过领悟来提高，无法通过练习来提高。\n");
}

int ob_hit(object ob, object me, int damage)
{
	object weapon, weapon1;
	string msg, *limbs, limb, attack_skill, str, str1;
	int ap, dp, skill, size, i;

	if( ob->query("env/invisibility") ) return 0;

	if( me->query_temp("fanzhen")){
		me->delete_temp("fanzhen");
		return 0;
	}

	weapon1 = me->query_temp("weapon");

	if (me->query_skill("douzhuan-xingyi", 1) < 150
	 || me->query_skill_mapped("parry") != "douzhuan-xingyi"
	 || me->query_skill("shenyuan-gong", 1) < 150
	 || me->query("combat_exp") < ob->query("combat_exp")/2 ) 
		return 0;

	skill = me->query_skill("douzhuan-xingyi", 1);
	weapon = ob->query_temp("weapon");

	if ( weapon) str = weapon->query("skill_type");
	if ( weapon1) str1 = weapon1->query("skill_type");
	if ( str != str1 ) return 0;

	if(objectp(weapon))
		msg = weapon->query("skill_type");
	else {
		mapping prepare = ob->query_skill_prepare();
		if (!sizeof(prepare)) msg = "unarmed";
		else msg = keys(prepare)[0];
	}

	if ( me->query("neili") < ob->query("neili")/5 ) return 0;

	if ( weapon ){
		attack_skill = weapon->query("skill_type");
		attack_skill = ob->query_skill_mapped(str);
	}
	else
		attack_skill = ob->query_skill_prepared(msg);

	if (! attack_skill ) return 0;

  ap = me->query_int()+me->query("int");
	dp = ob->query_int()+ob->query("int");

	if ( random(ap) > dp/2 && !random(3)) {
		limbs = ob->query("limbs");
		limb = limbs[random(sizeof(limbs))];
		size = sizeof(parry_msg);
		i = random(size);
		msg = parry_msg[i];
		msg += SKILL_D(attack_skill)->query_action(ob, weapon)["action"];
		msg = replace_string( msg, "$l", limb);
		if (weapon1)
			msg = replace_string( msg, "$w", weapon1->name()+colors[i]) + "！\n"NOR;
		else
			msg += "！\n"NOR;
		ob->receive_damage("qi", damage , me);
		ob->receive_wound("qi", damage/2, me);
		if (! weapon1)
			msg += COMBAT_D->damage_msg(damage, "瘀伤");
		else 
			msg += COMBAT_D->damage_msg(damage, "刺伤");
		msg = replace_string( msg, "$l", limb);
		if (weapon1)
			msg = replace_string( msg, "$w", weapon1->name());
		message_vision(msg, me, ob);
		COMBAT_D->report_status(ob, ob->query("qi") >= 0 || ob->query("eff_qi")+ob->query_temp("apply/qi") < 0);
		me->set_temp("fanzhen", 1);
		return -6000;
	}
	return 0;
}

string perform_action_file(string action)
{
	return __DIR__"mr/douzhuan-xingyi/" + action;
}
