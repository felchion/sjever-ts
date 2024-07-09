// linji-zhuang.c 峨嵋  临济十二庄
// Modified by Lane@SJ 2005/4/14


#include <ansi.h>
inherit FORCE;

int valid_enable(string usage)
{
        return usage == "force";
}

#include "../force.h"

int valid_learn(object me)
{
        int nb, nf, nh;
        nb = (int)me->query_skill("dacheng-fofa", 1);
        nf = (int)me->query_skill("force", 1);
        nh = (int)me->query_skill("linji-zhuang", 1);

        if ( nb < 150 && nb <= nh )
                return notify_fail("你的大乘佛法修为不够，无法领会更高深的临济十二庄。\n");

        if ( nf < 15)
                return notify_fail("你的基本内功火候还不够，无法领会临济十二庄。\n");

        if ( me->query("gender") == "无性" )
                return notify_fail("你无根无性，不能练习临济十二庄内功。\n");

        if ((int)me->query_skill("linji-zhuang", 1) > me->query_skill("force", 1) + 10
          && me->query_skill("linji-zhuang", 1) >= 200 )
               return notify_fail("你的基本功火候未到，必须先打好基础才能继续提高。\n");

        return valid_public(me);
}


int practice_skill(object me)
{
       if((int)me->query_skill("linji-zhuang", 1) >= 200 )
       {
           if( (int)me->query("potential", 1) > 5 && (int)me->query("neili", 1) > 200)
           {
                   me->improve_skill("linji-zhuang", (int)me->query_int());
                   me->add("neili", -100); 
                   me->add("potential", -1*(1+random(3)));
                   return 1;
           }
           else return notify_fail("你现在的修为不足以提高临济十二庄了。\n");
       }
        else return notify_fail("你现在的临济十二庄修为只能用学(learn)的来增加熟练度。\n");
}

string exert_function_file(string func)
{
        return __DIR__"em/linji-zhuang/" + func;
}

mapping exercise_msg(object me)
{
	return ([
		"status_msg" : HIR + me->name()+"脸上红光隐现，双手搭落在膝盖上。" NOR,
		"start_my_msg" : HIR"你席地而坐，五心向天，脸上红光时隐时现，内息顺经脉缓缓流动。\n"NOR,
		"start_other_msg" : MAG + me->name() +"席地而坐，五心向天，脸上红光时隐时现。\n"NOR,
		"halt_msg" : HIW"$N长出一口气，将内息急速退了回去，站了起来。\n"NOR,
		"end_my_msg" : HIC"你将内息走了个小周天，流回丹田，收功站了起来。\n"NOR,
		"end_other_msg" : HIR"不一会儿"+me->name()+"便神采弈弈地站了起来。\n"NOR
	]);
}

mixed hit_ob(object me, object victim, int damage_bonus, int factor)
{
	int i=0, j=0, sk;
	object weapon;

	if( ( sk = me->query_skill("linji-zhuang", 1) ) < 300 ) return 1;

	// 女性增加剑法威力
	if( me->query_skill_mapped("sword") == "huifeng-jian"
	 && me->query("jiali") > 0
	 && me->query("gender") == "女性" 
	 && me->query_skill("huifeng-jian", 1) >= 100
	 && objectp(weapon = me->query_temp("weapon"))
	 && weapon->query("skill_type") == "sword" ) {
		i += ( me->query_skill("huifeng-jian", 1)/100 ) * sk/10;	// 100 level 为一个飞跃
		if( me->query_temp("apply/damage") < 100 ) i = i + random(i/3);
		else i += random(i/5);
		if( wizardp(me) ) tell_object(me, "你的剑法增加威力："+i+"。\n"NOR);
	}

	// 男性增加刀法剑威力
	if( me->query_skill_mapped("blade") == "yanxing-daofa"
	 && me->query("jiali") > 0
	 && me->query("gender") == "男性" 
	 && me->query_skill("yanxing-daofa", 1) >= 120
	 && objectp(weapon = me->query_temp("weapon"))
	 && weapon->query("skill_type") == "blade" ) {
		i += ( me->query_skill("yanxing-daofa", 1)/120 ) * sk/10;	// 120 level 为一个飞跃
		if( me->query_temp("apply/damage") < 100 ) i = i + random(i/3);
		else i += random(i/5);
		if( wizardp(me) ) tell_object(me, "你的刀法增加威力："+i+"。\n"NOR);
	}

	// 增加被动反击效果
	if( victim->is_perform() || victim->is_exert() ) {
		j = me->query_str() * me->query_con() / 20;
		j = j + random(j/4);
		if( wizardp(me) ) tell_object(me, "你属于被动反击状态：威力增加 "+j+"。\n"NOR);
	}

	i = i + j;

	if( !userp(me) ) i = i/3;

	return i;
}
