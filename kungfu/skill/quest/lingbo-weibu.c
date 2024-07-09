// lingbo-weibu.c 凌波微步
// Modify By River@SJ 2000.3.1

#include <ansi.h>
inherit SKILL;

mapping *action = ({
([      "action" : "$n足下一个踉跄，乘势向左斜出半步，恰符合了凌波微步中「忽焉纵体」的步子，闪了开来。\n" ,
        "skill_dodge"  : 30
]),
([      "action" : "$n使出「流风回雪」歪歪斜斜退了几步，似乎不成章法，却在间不容缓的空隙中躲过了$N的攻势。\n",
        "skill_dodge"  : 40
]),
([      "action" : "$N的招式明明可以把$n围住，不知怎的，$n一式「轻云蔽月」，又如鬼魅般的跨出了圈外\n",
        "skill_dodge"  : 50
]),
([      "action" : "$n一式「以遨以嬉」，瞻之在前，忽焉在后，$N招式的落点和$n身子间总有厘毫之差。\n",
        "skill_dodge"  : 60
]),
([      "action" : "$n身形闪处，突然转向，使出了「神光离合」，$N加紧攻势，竟无法碰到$n一片衣角。\n",
        "skill_dodge"  : 70
]),
([      "action" : "$n大喝一声，使出「乍阴还阳」，左足向右踏出，不料踏实之时，身子已在西北角上。\n",
        "skill_dodge"  : 80
]),
([      "action" : "$n一式「飘忽若神」，侧身斜进，抢步“既济”，立闪“未济”，闪得几闪，已欺到$N的身后。\n",
        "skill_dodge"  : 90
]),
([      "action" : "$n一式「体迅飞枭」，似乎自管自地踏步，毫不理会，每一步都踏在别人意想不到的所在。\n",
        "skill_dodge"  : 100
])
});

mapping query_action(object me)
{
        int zhaoshu, level;

        zhaoshu = sizeof(action);
        level   = (int) me->query_skill("lingbo-weibu");

        if (level < 80 )
                zhaoshu--;

        if (level < 60 )
                zhaoshu--;

        if (level < 40 )
                zhaoshu--;

        return action[random(zhaoshu)];
}

int valid_enable(string usage)
{
//        if(this_player()->query("quest/dali/shanlu") && this_player()->query("quest/dali/pass"))
//修改标志
        if(this_player()->query("quest/天龙八部/武功/yuxiang") && this_player()->query("quest/天龙八部/武功/pass"))
	return usage == "dodge" || usage == "parry";
}

int valid_learn(object me)
{
//转换标志
        if ( !me->query("quest/天龙八部/武功/yuxiang"))
             return notify_fail("你没有磕头过玉像，怎么能偷学凌波微步。\n");
        if ( !me->query("quest/天龙八部/武功/pass"))
             return notify_fail("你没有成功研究，怎么能偷学凌波微步。\n");
        if ((int)me->query_skill("literate", 1) < 100)
             return notify_fail("你的知识不够，无法领会凌波微步。\n");
        if ((int)me->query_int(1) < 40)
             return notify_fail("你的悟性不够，无法领会凌波微步。\n");
	return 1;
}

string query_dodge_msg(object me)
{
        mapping action;

        action = query_action(me);

        return action["action"];
}

void skill_improved(object me)
{
	if(me->query_skill("lingbo-weibu",1) >= 80
	 && !me->query("lbwb")
	 && me->query_skill("literate",1) > 80){
		tell_object(me, HIY"\n你按照帛卷所画步法，一步步走下来，只觉的心情舒畅，不知不觉内力有所积累。\n"NOR);
		me->add("max_neili", random(80)+80);
		me->add("lbwb",1);
	}
	if(me->query_skill("lingbo-weibu",1) >= 120
	 && me->query("lbwb") == 1 
	 && me->query_skill("literate",1) > 100){
		tell_object(me, HIM"\n你六十四卦走完，刚好绕了一个大圈，回到原地，精神大振。\n"NOR);
		me->add("max_neili", random(80)+100);
		me->add("lbwb",1);
	}
	if(me->query_skill("lingbo-weibu",1) >= 160
	 && me->query("lbwb")==2 
	 && me->query_skill("literate",1) > 120){
		tell_object(me, CYN"\n你越走越快，内息随着步法不住运转，隐隐然自己的内力已经有了提高！\n"NOR);
		me->add("max_neili", random(80)+120);
		me->add("lbwb",1);
	}
	if(me->query_skill("lingbo-weibu",1) >= 200
	 && me->query("lbwb")==3 
	 && me->query_skill("literate",1) > 140){
		tell_object(me,RED"\n你步法娴熟，随意踏出，脚步成圆，只感神清气爽，全身精力弥漫！\n"NOR);
		me->add("max_jingli", random(140)+140);
		me->add("lbwb",1);
	}
}

string perform_action_file(string action)
{
        return __DIR__"quest/lingbo-weibu/" + action;
}


int practice_skill(object me)
{
        if ( !me->query("quest/天龙八部/武功/pass"))
             return notify_fail("你没有成功研究，怎么能偷学凌波微步。\n");
        
	if( (int)me->query_skill("lingbo-weibu",1) < 40 )
		return notify_fail("你的凌波微步步法未熟，无法自行修炼。\n");

	if( (int)me->query("max_neili") < 3000 )
		return notify_fail("你的内力修为太差，强练凌波微步只会走火入魔。\n");

        if( (int)me->query("jingli") < 40 )
                return notify_fail("你的体力太差了，不能练凌波微步。\n");
        if( (int)me->query("jing") < 25 )
                return notify_fail("你的体力太差了，不能练凌波微步。\n");
        if( (int)me->query("qi") < 20 )
                return notify_fail("你的体力太差了，不能练凌波微步。\n");
        if( (int)me->query("neili") < 20 )
		return notify_fail("你的内力太差了，不能练凌波微步。\n");

	me->receive_damage("jing", 15, "练功走火入魔");
	me->receive_damage("jingli", 35);
	me->receive_damage("qi", 10, "练功走火入魔");
	me->receive_damage("neili", 10);
	return 1;
}

int ob_hit(object ob, object me, int damage)
{
	string msg;
	int skill, neili, neili1;
	int j = 0;
	skill = me->query_skill("lingbo-weibu", 1);
	neili = me->query("neili");
	neili1 = ob->query("neili");

	if(me->query_temp("lingbo-weibu")){
		me->delete_temp("lingbo-weibu");
		return j;
	}

	if(skill/2 > random(ob->query_skill("dodge", 1))
	 && me->query("combat_exp") > ob->query("combat_exp")/2	 
	 && me->query_skill_mapped("dodge") == "lingbo-weibu"
	 && me->query_skill_mapped("parry") == "lingbo-weibu"
	 && neili > 400
	 && skill > 80 ) {
		me->add_temp("lingbo-weibu", 1);
		msg =random(2)? HIR + action[random(sizeof(action))]["action"]+NOR :
				HIC + action[random(sizeof(action))]["action"]+NOR ; 
		tell_object(ob, HIR "你只觉眼前一花，不见了对手的身影！\n" NOR);
		if(neili >= neili1+random(neili1)+damage*2){
			msg += "结果$N的力道被躲开了！\n", 
			j = -(damage*2+skill*3);
		}
		else if(neili > neili1+damage){
			msg += "结果$N的力道被躲过了一半！\n", 
			j=-damage/2;               
		}
		else {
			msg += "结果$n只躲过了$N的一小半力道！\n", 
			j = -(damage/2+random(damage/2)); 
		}
		message_vision(msg, ob, me);
		return j;
	}
	return j;   
}
