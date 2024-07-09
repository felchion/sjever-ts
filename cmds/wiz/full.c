// full.c for apprentice
// write by Look@SJ

#include <ansi.h>
inherit F_CLEAN_UP;
 
int main(object me,string str)
{
        if (!geteuid()) seteuid(getuid());
        if (!str)
                me = this_player();
        else	me = present(lower_case(str), environment(this_player()));
        
        if (!me) return notify_fail ("这里没有你所要恢复的人。\n");
        
        // reincarnate 已经完全恢复，所以不用再一个个set了。
        me->reincarnate();
	me->set("neili", me->query("max_neili")*2);
        if (wizardp(me)) me->clear_condition();

        if (me != this_player())
            	message_vision(WHT"只见一道光芒从$N处照在$n身上，恢复了$p所有的体力！\n"NOR,this_player(), me);
        else
                write(WHT"你的状态恢复完毕！\n"NOR);
        return 1;
}

int help(object me)
{
        write(@HELP
指令格式: full [玩家ID]
 
恢复玩家所有参数到最大值。

HELP );
        return 1;
}
