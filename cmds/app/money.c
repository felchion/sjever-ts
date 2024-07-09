// full.c for apprentice
// written by snowman@SJ 29/10/1999

inherit F_CLEAN_UP;
 
int main(object me,string arg)
{
        string kind;
        int amount;
        object n_money;

        if( !arg || sscanf(arg, "%d %s", amount, kind)!=2 )
          return notify_fail("[topless]: wave <多少钱> <钱币种类>\n");

        n_money = present(kind + "_money", me);
        if( !n_money && file_size("/clone/money/" + kind + ".c") < 0 )
                return notify_fail("你挖了半天却什么也没有挖出来。\n");
        if( amount < 1 )
                return notify_fail("你挖了半天却什么也没有挖出来。\n");

        if( !n_money ) {
                n_money = new("/clone/money/" + kind);
                n_money->move(me);
                n_money->set_amount(amount);
        } else
                n_money->add_amount(amount);

	message_vision( sprintf("$N拿出一张VISA卡，从ATM中取出了%s%s%s。\n",
                        chinese_number(amount),
                        n_money->query("base_unit"),
                        n_money->query("name")),me);
        log_file("static/HOWG",sprintf("%s(%s) 从ATM中取出了%s%s%s?on %s\n",me->name(1), geteuid(me),chinese_number(amount), n_money->query("base_unit"),n_money->query("name"), ctime(time()) ) );
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
