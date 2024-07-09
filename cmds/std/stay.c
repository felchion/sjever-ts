// stay.c
// by augx@sj  6/27/2002

#include <ansi.h>
inherit F_CLEAN_UP;

int main(object me, string arg)
{
	object target;

	if( me->is_busy() )
		return notify_fail("你正忙着呢。\n");

	if( !arg || !(target=present(arg,environment(me))) )
		return notify_fail("你要让什么动物停下来？\n");

	if( !living(target) )
		return notify_fail("那不是活物。\n");

	if( target->query("master") != me->query("id") )
		return notify_fail("你还没有降伏它呢。\n");

	if( target->query_leader() != me )
		return notify_fail("它并没有跟随你啊。\n");

	message_vision(CYN"$N拍了拍$n的头，又指指$n......\n" NOR, me,target);

	target->set_leader(0);
	return 1;
}

int help(object me)
{
write(@HELP
指令格式 : stay <动物id>

这个命令用来使跟随你的动物停在原地。
HELP
);
	return 1;
}
