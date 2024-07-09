// /cmds/usr/run.c
// Modify By Mope@SJ /03/01/02
// 骑马的自动化，用到广度优先搜索，将路径按次序列出
// 缺点是room->query("short")相同就只能达到最近的地点
// 中途有迷宫或水则无法到达
// 解密等没有连接出口的房间不可到达

#include <ansi.h>

#define RUN_D "/cmds/imm/runawayd.c"

int main(object ob,string arg)
{
        mixed a;
        object horse;
        string hname;
        mixed flag;

        if (! arg) return notify_fail("你想得到去哪儿的路径？\n");
        a = RUN_D->search_for_path(ob, arg);

        if (! a) return notify_fail("没有这个地方，或者这个地方不可到达。\n");
        // 路程中间是迷宫或水，到达不了

        if (stringp(hname = ob->query_temp("is_riding")))
                horse = ob->query_temp("riding_beast");
        else
                return notify_fail("你没有骑马啊？\n");

        if (ob->is_busy() || ob->is_fighting())
                return notify_fail("你正忙着呢，跑不掉。\n");

        if (environment(ob) != environment(horse))
        {
                if (horse && horse->query_temp("is_rided_by") == ob)
                        horse->delete_temp("is_rided_by");
                ob->delete_temp("is_riding");
                ob->delete_temp("riding_beast");
                return notify_fail("你并没有骑马啊。\n");
        }

        if (! (flag=ob->query("env/brief"))) {
                flag = 0;
                ob->set("env/brief", "YES");
        }
        ob->start_busy(1);
        if(wizardp(ob) && ob->query("env/test"))
        write(sprintf("%O\n",a));
        for(int i = sizeof(a[1])-1;i>=0;i--)
        {
        	if(wizardp(ob) && ob->query("env/test"))
        		write(sprintf("Move to %O %d\n",a[1][i],i));
                ob->move(a[1][i]);
                horse->move(a[1][i]);
                tell_room(environment(ob),ob->name()+"骑着"+hname+"一路招摇疾驰而过。\n", ({ob}));
                if(arg == strip((a[1][i])->query("short")))
                {
                	if(wizardp(ob) && ob->query("env/test"))
        			write(sprintf("breaked\n"));
                	break;
                }
        }

        ob->start_busy(2);
        call_out("stop", 1, ob, hname, arg, flag);

        return 1;
}

void stop(object ob, string hname, string arg, mixed flag)
{
        if (!ob || !hname || !arg) return;
        tell_room(environment(ob),ob->name()+"骑着"+hname+"一路顺利到达"+arg+"。\n", ({ob}));
        tell_object(ob, HIY "你快马加鞭，星夜赶到了"HIY + arg + HIY "。\n" NOR);
        message("vision", "\n"+ob->name()+"紧勒缰绳，姿态优美将"+hname+"停了下来。闲人也给$N一大哄！\n", environment(ob), ob);
        if (flag==0) ob->delete("env/brief");
        else ob->set("env/brief", flag);
        
        return;
}

