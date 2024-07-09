// adstuff.c
// by snowman@SJ 20/05/1999

#include <ansi.h>
inherit F_CLEAN_UP;
int help(object me);

int main(object me, string arg)
{
	object ob;
	int num = 1;
            
	if( !arg || sscanf(arg, "%d %s", num, arg) != 2)
        	return help(me);
        
        if( !intp(num)) 
                return notify_fail("注意：奖励必须为数字。 \n");
        
        if( !objectp(ob = present(arg, environment(me))))
                if( !objectp(ob = present(arg, me)))
                       return notify_fail("这里没有这个物品。\n");
         
        if( ob->is_character() )
		return notify_fail("看清楚一点，那可是生物哦。\n");
		
	if(file_size(base_name(ob)+".c") <= 0 )
		return notify_fail("对不起，此物品的源文件已经不存在了。\n");

	if( num <= 0 ){
		if(TASK_D->delete_status(ob->query("name")))
			return notify_fail("你将"+ob->name()+"("+ob->query("id")+") 从 task_stuffs 里删除....Ok。\n");
		else 	return notify_fail("将"+ob->name()+"从TASKD.C 中删除失败。\n");
	}
	else if(TASK_D->set_stuff(ob->query("name"), ([ "id" : ob->query("id"), "reward" : num ]) ))
		return notify_fail("你将"+ob->name()+"("+ob->query("id")+") ，EXP = "+num+" 加入到 task_stuffs 里....Ok。\n");

	else return notify_fail("TASKD.C 中已经有了此件物品，如要更改请先删。\n");
	return 1;
}

int help(object me)
{
write(@HELP
指令格式：

adstuff <奖励> <物品>

奖励为所值之EXP。
如果奖励为零，将删除此物品。

by snowman@SJ 20/05/1999

HELP
    );
    return 1;
}

