// douzhuan.c 斗转星移
// by tiantian@sj

#include <ansi.h>
#include <weapon.h>
#include <combat.h>

inherit F_SSERVER;

int main(object me, string arg)
{
	object target1,target2;
	string str1,str2;

	if (!me) return 1;

	if (me->query("family/family_name")!="姑苏慕容")
		return notify_fail("你不是慕容弟子，不能使用姑苏慕容的绝技。\n");

	if (me->is_busy())
		return notify_fail("你现在正忙着呢。\n");

	if (!me->is_fighting())
		return notify_fail("斗转星移只能在战斗中使用!。\n");

	if (me->query_skill_mapped("parry")!="douzhuan-xingyi")
		return notify_fail("你必须用斗转星移做招架，才能使得出慕容绝技。\n");

	if (!arg || sscanf(arg,"%s to %s",str1,str2)!=2)
		return notify_fail("指令格式：douzhuan <id1> to <id2>。\n");

	if (!present(str1,environment(me)))
		return notify_fail("这里没有"+str1+"这个人。\n");

	if (!present(str2,environment(me)))
		return notify_fail("这里没有"+str2+"这个人。\n");

	target1=present(str1,environment(me));
	target2=present(str2,environment(me));

	if (target1==target2)
		return notify_fail("你只能把攻击转移给第三者。\n");

	if (!me->is_fighting(target1) )
		return notify_fail("你并没有在跟"+str1+"作战。\n");

	if (!target2->is_character() || target2->is_corpse() )
		return notify_fail("那不是个活物。\n");

	if (wiz_level(target2)>wiz_level(me))
		return notify_fail("你不能企图暗害巫师级别比你高的玩家。\n");

	if (me->query_temp("bishen_target"))
		return notify_fail("你正在使用以彼之道还施彼身。\n");

	if (me->query_temp("douzhuan_target")||me->query_temp("douzhuan_source"))
		return notify_fail("你正在使用斗转星移。\n");

	if (userp(target2) && !me->is_killing(target2))
		return notify_fail("除非跟对方有深仇大恨，你不能暗害玩家。\n");

	if ( target2->query("job_npc") || target2->query("no_quest"))
		return notify_fail("你无法把攻击转移给"+target2->name(1)+"。\n");

	if (target2->query("family/family_name")=="姑苏慕容" || target2->query_skill("douzhuan-xingyi",1))
		return notify_fail("连自己人也要害，太过分了吧。\n");

	if (me->query("max_neili") < 2000)
		return notify_fail("你的内力还不够高！\n");

	if (me->query("neili") < 1000)
		return notify_fail("你当前的内力不够！\n");

	if (me->query_skill("douzhuan-xingyi",1)<140)
		return notify_fail("你的斗转星移不够熟练，还不能使用斗转星移。\n");

	tell_object( me, HIG"你潜运姑苏慕容斗转星移绝技，想要把"+target1->query("name")
		+"的攻击转移到"+target2->query("name")+HIG"身上。\n\n"NOR );
	if (random(me->query("combat_exp")) > target1->query("combat_exp")*2/3
	|| random(me->query("combat_exp")) > target2->query("cobmat_exp")/3){
		me->set_temp("douzhuan_source",target1);
		me->set_temp("douzhuan_target",target2);
		me->start_busy(1);
	}
	else {
		tell_object( me, HIY"但是对方似乎并没有受到你的影响。\n"NOR );
		me->start_busy(1+random(2));
	}

	me->start_perform(6,"斗转星移");
	me->add("neili",-300);
	call_out("check_fight",1,me,target1,target2,me->query_skill("douzhuan-xingyi",1)/10);

	return 1;
}

int check_fight(object me,object target1,object target2,int i)
{
	if(!me)
		return 0;
	if(!target1
		|| !target2
		|| !me->is_fighting()
		|| environment(me)!=environment(target1)
		|| environment(me)!=environment(target2)
		|| i==0
		|| me->query_skill_mapped("parry")!="douzhuan-xingyi"
	)
	{
		tell_object(me,HIG"你缓缓收功，将内力纳入丹田。\n"NOR );
		me->delete_temp("douzhuan_source");
		me->delete_temp("douzhuan_target");
		return 0;
	}

//	me->start_perform(1,"斗转星移");
	call_out("check_fight",1,me,target1,target2,i-1 );
	return 1;

}
