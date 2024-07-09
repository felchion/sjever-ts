#include <ansi.h>
#include <combat.h>
inherit F_SSERVER;

int main(object me, string arg)
{
	object target1,target2;
	string str1,str2;

	if( !arg || sscanf(arg,"%s %s",str1,str2)!=2 )
		return notify_fail("你要做什么？\n");

	if( me->is_busy())
		return notify_fail("你现在正忙着呢。\n");

	if( !me->is_fighting() )
		return notify_fail("你没在战斗中呢!。\n");

	if( environment(me)->query("no_fight"))
		return notify_fail("这里不能战斗。\n");

	if( me->query_skill_mapped("parry")!="qiankun-danuoyi")
		return notify_fail("你又不会乾坤大挪移。\n");

	if( !present(str1,environment(me)) )
		return notify_fail("这里没有"+str1+"。\n");

	if( !present(str2,environment(me)) )
		return notify_fail("这里没有"+str2+"。\n");

	target1=present(str1,environment(me));
	target2=present(str2,environment(me));

	if(target1 == target2 || me == target2)
		return notify_fail("还是别白费力气啦。\n");

	if( !me->is_fighting(target1) )
		return notify_fail("你并没有在跟"+str1+"作战。\n");

	if( !living(target2) )
		return notify_fail("那不是个活物。\n");

	if( (int)me->query("max_neili") < 3000 )
		return notify_fail("再努力点打坐吧！\n");

	if( (int)me->query("neili") < 1500 )
		return notify_fail("你的内力不够！\n");

	if(me->query_skill("qiankun-danuoyi",1) < 210 )
		return notify_fail("你必须有乾坤大挪移第七层的实力。\n");

	if ( me->query_skill("jiuyang-shengong", 1) < 210 
	&& me->query_skill("shenghuo-shengong", 1) < 210 )
		return notify_fail("你的内功等级不够。\n");

	tell_object( me, HIG"你暗运乾坤大挪移的第七层心法。\n"NOR );

	if (userp(target1))
		me->add_condition("killer", 10);

	if (userp(target2))
		me->add_condition("killer", 10);

	if( random(me->query_int()) > target1->query_int()/4 &&
	 random(me->query_int()) > target2->query_int()/4) {
		me->start_busy(1+random(3));
		message_vision(HIG"突然间，$N向$n击出一招！\n",target1, target2);
		message_vision(HIG"$n狠狠的瞪了$N一眼，$N也莫名其妙。\n"NOR,target1, target2);
		COMBAT_D->do_attack(target1, target2, target1->query_temp("weapon"), 0);
		target2->add_temp("qkdny/target",1);
		if( target2->query_temp("qkdny/target")*5 > random(target2->query_int())){
			message_vision(HIR"\n$n猛然开始向$N攻击，看来火大了！\n"NOR,target1,target2);
			message_vision(HIR"\n$N莫名其妙，也发了狠向$n杀去！\n"NOR,target1, target2);
			message_vision(HIW"\n$N微微一笑，负手观斗。\n"NOR,me);
			target2->delete_temp("qkdny/target");
			me->remove_all_enemy();
			target1->remove_all_enemy();
			target1->kill_ob(target2);
			target2->kill_ob(target1);
		}
	}
	else {
		tell_object( me, HIB"但是失败了。\n"NOR );
		me->start_busy(1+random(3));
	}
	me->add("neili",-150-random(50));
	me->add("jingli",-50-random(50));
	me->add("jing",-50-random(50));
	return 1;
}
