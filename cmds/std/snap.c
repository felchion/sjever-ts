// snap.c
// by augx@sj  6/27/2002

#include <ansi.h>
inherit F_CLEAN_UP;

int main(object me, string arg)
{
	object target,obj;
	string s1,s2;

	if( me->is_busy() )
		return notify_fail("你正忙着呢。\n");

	if( !arg || sscanf(arg,"%s to %s",s1,s2)<2 )
		return notify_fail("你要你的宠物攻击谁？\n");

	if( !(target=present(s1,environment(me))) )
		return notify_fail("你要指挥什么动物啊？\n");

	if( !(obj=present(s2,environment(me))) )
		return notify_fail("你要你的宠物攻击谁？\n");

	if( !living(target) || !living(obj) )
		return notify_fail("那不是活物。\n");

	if( target->query("master") != me->query("id") )
		return notify_fail("你还没有降伏它呢。\n");

	if( userp(obj) )
		return notify_fail("测试期间，禁止攻击玩家。\n");

	if( target->is_busy() || target->is_fighting() )
		return notify_fail("你的宠物正忙着呢。\n");

	message_vision(CYN"$N拍了拍"+target->name()+CYN"的头，指指$n......\n"NOR,me,obj);

	target->kill_ob(obj);
	return 1;
}

int help(object me)
{
write(@HELP
指令格式 : snap <宠物> to <攻击对象>

这个命令用来指挥你的宠物攻击敌人。
HELP
);
	return 1;
}
