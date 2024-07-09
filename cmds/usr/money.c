// full.c for apprentice
// written by snowman@SJ 29/10/1999
#include <ansi.h>
inherit F_CLEAN_UP;
 
int main(object me,string arg)
{
        string kind;
        int amount;
        object n_money;

        if( !arg || sscanf(arg, "%d %s", amount, kind)!=2 )
		return notify_fail("money <多少钱> <钱币种类>\n");

        n_money = present(kind + "_money", me);
        if( !n_money && file_size("/clone/money/" + kind + ".c") < 0 )
                return notify_fail("你挖了半天却什么也没有挖出来。\n");
        if( amount < 1 )
                return notify_fail("你挖了半天却什么也没有挖出来。\n");

        if( !n_money ) {
                n_money = new("/clone/money/" + kind);
        }
                
	tell_object(me,HIY"以前用这种方法取钱爽吧，但快乐往往是短暂的，你必须对你做出的行为负责！\n"NOR);
	tell_object(me,HIY"再次提醒你多看help rules，另外请不要怀疑书剑巫师组对此类事件处罚的决心。\n"NOR);
	/*message_vision( sprintf("$N拿出一张VISA卡，从ATM中取出了%s%s%s。\n",
                        chinese_number(amount),
                        n_money->query("base_unit"),
                        n_money->query("name")),me);*/
        log_file("static/HOWG_BUG",
		sprintf("%s(%s)企图提取%s%s%s\n",
			me->name(1), geteuid(me),chinese_number(amount),
			n_money->query("base_unit"),n_money->query("name")
		), me
	);
        return 1;
}

int help(object me)
{
        write(@HELP
指令格式: money <多少钱> <钱币种类>
 
 取钱。

HELP );
        return 1;
}
