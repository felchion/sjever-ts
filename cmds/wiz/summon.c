// summon command..
// Modify by snowman@SJ 17/02/2000
// check if ob is a leitaiuser.

inherit F_CLEAN_UP;

int main(object me, string str)
{
        object ob;
        int s = 0;

        if (!str) return notify_fail("<Syntax>: Summon <player id>\n");
        if (sscanf(str, "-s %s", str)) s = 1;
        ob = LOGIN_D->find_body(str);
        if (!ob) return notify_fail("咦... 有这个人吗？\n");
        if( wiz_level(ob) > wiz_level(me) )
                       return notify_fail("你无此权限\n");
        if (ob->query("env/invisibility") > 10000)
        return notify_fail("咦... 有这个人吗？\n");
        if (environment(ob) == environment(me))
                return notify_fail(ob->name() + "就在你身边。\n");
	if (environment(ob)) {
        if (environment(ob)->query("no_summon_out"))
                return notify_fail(ob->name() + "的所在特殊，不可用Summon抓出来。\n");
        if (environment(me)->query("no_summon_in"))
                return notify_fail("你目前所在特殊，不可用Summon命令。\n");
	}
           
        if (base_name(ob) == "/cmds/leitai/leitaiuser")
                return notify_fail(ob->name() + "这人是比武擂台Clone出来的，不可抓出。\n");
        // moving
        if (!s) {
                tell_room(environment(ob),"天空中伸出一只大手把"+
                ob->name()+"抓了起来，然后不见了。\n", ob);
                tell_object(ob,"一只手把你抓了起来, 你眼前一阵黑....\n");
        }
        ob->move(environment(me), s);
        tell_object(me, "你把"+ob->name()+"抓到你的面前。\n");
        if (!s) {
                tell_object(ob,".... 醒来时发现是"+ me->name() + "把你弄过来的。\n");
                tell_room(environment(ob), me->name()+ "伸手把" + ob->name()+"抓了过来。\n",({me,ob}));
        }
        if( !wizardp(ob) ) 
            	log_file("static/SUMMON", sprintf("%s(%s) summoned %s(%s) to %s at %s。\n", me->name(1),
                        me->query("id"), ob->name(1), ob->query("id"), base_name(environment(me)), ctime(time()) ) );
        // ok ..
        return 1;
}

int help(object me)
{
write(@HELP
指令格式 : summon <某人>

此指令可让你(你)将某人抓到你面前。
HELP
    );
    return 1;
}

