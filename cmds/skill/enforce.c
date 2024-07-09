// enforce.c
// Modified by snowman@SJ 02/08/2000. 加上 200 上限。

inherit F_CLEAN_UP;

int main(object me, string arg)
{
        int pts;

        if( !arg || (arg!="none" && arg!="max" && !sscanf(arg, "%d", pts)) ) 
                return notify_fail("指令格式：enforce|jiali <使出几点内力伤敌>|none|max\n");

        if( !me->query_skill_mapped("force") )
                return notify_fail("你必须先 enable 一种内功。\n");

	if ( me->query_temp("qkdny/hua"))
		return notify_fail("你正在使用「化」字诀，暂时无法加力。\n");

        if( me->query_condition("no_enforce") )
                return notify_fail("你丹田被破，暂时不能加力。\n");
                
        if( arg == "none" )
                me->delete("jiali");
        else {
        	if(arg=="max"){
        		pts = (int)me->query_skill("force") / 3;
        		if(pts>300) pts = 300;
        	}
                if( pts < 0 || pts > (int)me->query_skill("force") / 3 || (pts > 300 && !me->query_temp("jiuyang/yijin")))
                        return notify_fail("你只能用 none 表示不运内力，或数字表示每一击用几点内力，上限是 300。\n");
                me->set("jiali", pts);
        }

        if (pts)
                write("你决定用" + chinese_number(pts) + "点的内力伤敌。\n");
        else
                write("你决定不使用内力伤敌。\n");
        return 1;
}

int help (object me)
{
        write(@HELP
指令格式: enforce|jiali <使出几点内力伤敌>|none
 
这个指令让你指定每次击中敌人时，要发出几点内力伤敌。

enforce none 则表示你不使用内力。 

See Also: enable
HELP
        );
        return 1;
}

