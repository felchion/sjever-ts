// 控制室 ctrl.c
// By River 99/04/15
// Modified by Numa 1999-11-25
// Modifier : Numa@Sj 2000.5.4

#define SAFE_PLACE "/cmds/leitai/bwdh/xiuxishi3.c"
#define NUMBER 3

#include <ansi.h>
#include <room.h>
inherit ROOM;

void create()
{
        set("short",HIW"控制台"NOR);
        set("long", @LONG
这里是书剑组队比赛的巫师控制室，你能看见众多巫师急急忙忙地在干些
什么。正中央放着一张很大的控制台，上面标示着一排按钮，最为明显的是一
个红色的按钮（anniu)，不知道按下去是什么结果，旁边还有一个告示(sign)。
LONG
        );

        set("exits", ([
                "down" : "/d/wizard/wizard_meet",
        ]));
        set("item_desc",([
                "sign" : "指令使用说明：
                
                login    : 登录比武人员信息。
                list     : 显示比武人员信息。
                correct  : 修改比武人员信息。
                send     : 传送两组比武人员去山庄。
                set_item : 设置山庄内有无物品。
                press    : 打开大门，开始比武。
                
                                ----Numa@Sj 2000-5-4\n",
        ]));
        set("objects",([
                __DIR__"obj/snooper1" : 1,
                __DIR__"obj/snooper2" : 1,
                __DIR__"obj/snooper3" : 1,
                __DIR__"obj/snooper4" : 1,
                __DIR__"obj/snooper5" : 1,
                __DIR__"obj/snooper6" : 1,
        ]));
        set("no_fight",1);
        set("ceshi",1);
        set("line",0);
        set("no_clean_up",1);
        setup();
}

void init()
{
        add_action("do_push", "press");
        add_action("do_login", "login");
        add_action("do_correct", "correct");
        add_action("do_list", ({"list","show"}));
        add_action("do_send", ({"send"}));
        add_action("do_set_item",({"set_item"}));
}

int do_set_item(string arg)
{
        object ob = this_object();
        
        if (!arg)
                return notify_fail("格式：set_item no|yes\n\nset_item no : 山庄内所有物品皆消失。\nset_item yes : 山庄内所有物品皆存在。\n");
        if (arg == "no")
                ob->set("bw/set_item","no");
        else if (arg == "yes")
                ob->set("bw/set_item","yes");
        else
                return notify_fail("格式：set_item no|yes\n\nset_item no : 山庄内所有物品皆消失。\nset_item yes : 山庄内所有物品皆存在。\n");
        tell_object(this_player(),"山庄内所有物品现在已设置为："+arg=="no"?"消失":"存在"+"。\n");
        return 1;
}

int do_login(string arg)
{
        object ob = this_object();
        object me = this_player();
        string *group, pl0, pl1, pl2, pl3, msg;
        string re1, re2, re3;
        int i, j, number;

        if( wiz_level(me) < 3 )
                return notify_fail("只有 (wizard) 以上的巫师才能在擂台login其他player。\n");        


        number = NUMBER;
        
        if (!arg)
                return notify_fail("指令格式：login (组队名称) (成员一的Id) (成员二的Id) ....\n注：目前每组为最多"
                        + CHINESE_D->chinese_number(number) +"人。
                        
        示例：如组一为 少林一队 ，成员为 id1, id2, id3
        则填写他们参赛用：
                login 1 少林一队 id1 id2 id3
        如果其中有人无法参加，找人代替的话，设id2无法参加，
        由replace这个玩家代替，则命令为：
                login 1 少林一队 id1(id1) id2(replace) id3(id3)
        
        组二则为
                login 2 xxxxx xxx xxx xxx
        依次类推。

                        \n");

        if (sscanf(arg, "%d %s",i, arg) != 2)
                return notify_fail("你要添加第几行的比武数据？\n");
        
        if (!ob->query("packet"+(i-1)) && (i-1)!=0)
                return notify_fail("请按照顺序填写,第"+CHINESE_D->chinese_number(i-1)+"行暂无数据。\n");
        
        if (stringp(ob->query("packet" + i))) {
                group = explode(ob->query("packet" + i)," ");
                return notify_fail("第"+CHINESE_D->chinese_number(i)+"行已经填入了由"+group[1]+"("+group[2]+"代替)，"+group[3]+"("+group[4]+"代替)和("+group[5]+"("+group[6]+"代替)所组成的"+group[0]+"。\n");
        }
        if (sscanf(arg, "%s %s %s %s", pl0, pl1, pl2, pl3) != 4)
                return notify_fail("指令格式：login (组队名称) (成员一的Id) (成员二的Id) ....\n注：目前每组为最多"
                        + CHINESE_D->chinese_number(number) +"人。
                        
        示例：如组一为 少林一队 ，成员为 id1, id2, id3
        则填写他们参赛用：
                login 1 少林一队 id1 id2 id3
        如果其中有人无法参加，找人代替的话，设id2无法参加，
        由replace这个玩家代替，则命令为：
                login 1 少林一队 id1(id1) id2(replace) id3(id3)
        
        组二则为
                login 2 xxxxx xxx xxx xxx
        依次类推。

                        \n");
        if (!ob->query("packet1")) {
                if (query("ceshi"))
                        tell_object(me,HIY"\n【通告】"+me->name(1)+"大声宣布：我现在开始输入玩家比武数据，大家请稍侯。\n\n"NOR);
                else
                        message("channel:chat",HIY"\n【通告】"+me->name(1)+"大声宣布：我现在开始输入玩家比武数据，大家请稍侯。\n\n"NOR,users());
        }
        ob->add("line",1);
        
                if (sscanf(pl1,"%s(%s)",pl1,re1)==2
                &&  sscanf(pl2,"%s(%s)",pl2,re2)==2
                &&  sscanf(pl3,"%s(%s)",pl3,re3)==2)
                        ob->set_temp("replace"+i,1);
                if (!ob->query_temp("replace"+i)) {
                        if (re1="0")
                                re1 = "无";
                        if (re2="0")
                                re2 = "无";
                        if (re3="0")
                                re3 = "无";
                }
                ob->set("packet"+i,pl0+" "+pl1+" "+re1+" "+pl2+" "+re2+" "+pl3+" "+re3);
                msg = "比赛组："+pl0+"的玩家数据填写完毕。\n";
                msg += "目前所登录的比武数据如下所列：\n";
                msg += "组别  组名      第一位    代替者      第二位    代替者      第三位    代替者      \n";
                msg += "-------------------------------------------------------------------------------------\n";
                for (j=i;j>0;j--) {
                        group = explode(ob->query("packet"+j)," ");
                        if (sizeof(group)!=7) {
                                write("异常错误，替代后的group只有"+sizeof(group)+"项！\n");
                                ob->delete("packet"+i);
                                return 1;
                        }
                        pl0 = group[0];
                        pl1 = group[1];
                        re1 = group[2];
                        pl2 = group[3];
                        re2 = group[4];
                        pl3 = group[5];
                        re3 = group[6];
                        msg += sprintf("%-6d%-10s%-10s◎%-10s%-10s◎%-10s%-10s◎%-10s\n",j,pl0,pl1,re1,pl2,re2,pl3,re3);
                        continue;
                }
                msg += "-------------------------------------------------------------------------------------\n";
                msg += "\n如果数据有误，请输入correct (行数)进行修改，重新登录。\n";
                tell_object(me,msg);
                                
                return 1;
}

int do_correct(string arg)
{
        object ob = this_object();
        object me = this_player();
        int i;

        if( wiz_level(me) < 3 )
                return notify_fail("只有 (wizard) 以上的巫师才能在擂台删除记录。\n");

        if (!arg)
                return notify_fail("请输入correct line (行数)进行修改。\n");
        if (sscanf(arg,"line %d",i)!=1)
                return notify_fail("请输入correct line (行数)进行修改。\n");
        if (!ob->query("packet"+i))
                return notify_fail("这行并没有任何玩家数据。\n");
        
        tell_object(me,"读取数据文件中。。。成功读取第"+CHINESE_D->chinese_number(i)+"行：\n");
        tell_object(me,ob->query("packet"+i));
        tell_object(me,"请输入你欲修改的数据：\n");
        tell_object(me,"格式：\n          少林一队 id1(id1) id2(replace) id3(id3)\n请输入：......>");
        input_to("get_msg", ob, i);
        return 1;
}

int get_msg(string msg, object ob, int i)
{
        string pl0,pl1,pl2,pl3;
        string re1,re2,re3;
        
        if (sscanf(msg,"%s %s(%s) %s(%s) %s(%s)",pl0,pl1,re1,pl2,re2,pl3,re3)!=7) {
                write("数据格式错误，应为： 少林一队 id1(id1) id2(id2) id3(id3)\n请重新输入......>");
                input_to("get_msg", ob, i);
                return 1;
        }
        ob->set("packet"+i,pl0+" "+pl1+" "+re1+" "+pl2+" "+re2+" "+pl3+" "+re3);
        write("第"+CHINESE_D->chinese_number(i)+"行数据修改完毕，请用list查看。\n");
        return 1;
}

int do_list(string arg)
{
        object ob = this_object();
        object me = this_player();
        string *group, msg;
        string pl0, pl1, pl2, pl3;
        string re1, re2, re3;
        int i, j;
        
        if (!ob->query("packet1") || ob->query("line")==0)
                return notify_fail("还没有人输入玩家数据呢！！\n");
        
        i = ob->query("line");
        msg = "目前所登录的比武数据如下所列：\n";
        msg += "组别  组名      第一位    代替者      第二位    代替者      第三位    代替者      \n";
        msg += "-------------------------------------------------------------------------------------\n";
        for (j=i;j>0;j--) {
                group = explode(ob->query("packet"+j)," ");
                pl0 = group[0];
                pl1 = group[1];
                re1 = group[2];
                pl2 = group[3];
                re2 = group[4];
                pl3 = group[5];
                re3 = group[6];
                msg += sprintf("%-6d%-10s%-10s◎%-10s%-10s◎%-10s%-10s◎%-10s\n",j,pl0,pl1,re1,pl2,re2,pl3,re3);
                continue;
        }
        msg += "-------------------------------------------------------------------------------------\n\n";
        if (!arg)
                tell_object(me,msg);
        else if (arg == "all")
                message("channel:chat",HIY"【比武通告】\n" + msg + "\n"NOR,users());
        else return notify_fail("你要给谁看比武数据？(无参数给自己看，参数为all时给所有玩家看)\n");
        return 1;
}

// 替换玩家
object replace_player(string old, object ob)
{
        object user;
//        string temp
//      if(ob->query_temp("leitai"))
//              return notify_fail(ob->name()+"正在别的擂台比武中。\n");
        seteuid(old);
        user = new("/cmds/leitai/leitaiuser");   // 去掉了save()，擂台中的一切和本身无关。
        user->set_name("", ({ old }));
        if(!user->restore()) {
                destruct(user);
                return ob;
        }
        ob->move(SAFE_PLACE);
        user->setup();          
        user->set_temp("leitai", 1);    
        exec(user, ob);
        user->set("link_ob", ob);
        user->set_temp("link_ob", ob->query_temp("link_ob"));
//      user->move(this_object());
        user->clear_condition();
        user->reincarnate();
//      user->set("channels", ({ "rumor", "err" }) );
      tell_object(user, YEL"你代表"+user->name()+"进入擂台，目前处于最佳状态中。\n"NOR"
需要兵器请从物品架上拿 "YEL"NA"NOR" 。
如果你准备好了请打"YEL" OK "NOR"。\n\n");
        if (old == ob->query("id") )
                user->set_temp("replace_msg",ob->query("name")+"("+ob->query("id")+")");
        else
                user->set_temp("replace_msg", ob->query("name")+"("+ob->query("id")+")代表"+user->name()+"("+old+")");
        return user;
}

// 传送玩家
int resolve_player(object me, object ob, int num, int point)
{
        object *o = ({me,me,me});
        string *ms = ({" "," "," "}), *group;
        object gm1, gm2, gm3, room;
        string pl0, pl1, pl2, pl3;
        string re1, re2, re3;
        string msg = "", place;
        int i, where;

        if (point == 0) {
                place = "xiuxishi1";
                where = 1;
        }
        if (point == 1) {
                place = "xiuxishi2";
                where = 2;
        }
        
        if (!ob->query_temp("replace"+num)) {
                group = explode(ob->query("packet"+num)," ");
                pl0 = group[0];
                pl1 = group[1];
                pl2 = group[3];
                pl3 = group[5];
                if (!(gm1 = find_player(pl1)))
                        return notify_fail(pl1+"并不在游戏中，无法参加，请找替身，或放弃。\n");
                if (!(gm2 = find_player(pl2)))
                        return notify_fail(pl2+"并不在游戏中，无法参加，请找替身，或放弃。\n");
                if (!(gm3 = find_player(pl3)))
                        return notify_fail(pl3+"并不在游戏中，无法参加，请找替身，或放弃。\n");

                        if (!(room = load_object(__DIR__ + place)))
                                return notify_fail("ERROR!!!ROOM " + place+" NOT EXISTS! \n");
                        room->set("party",pl0);
                        room->set("packet",pl1 + " " + pl2 + " " + pl3 + " ");
                        ob->set_temp("pp"+where,pl0);
                        ob->set_temp("p" + where + "/i1",pl1);
                        ob->set_temp("p" + where + "/i2",pl2);
                        ob->set_temp("p" + where + "/i3",pl3);
                        
                        gm1 = replace_player(pl1, gm1);
                        tell_room(environment(gm1),gm1->name(1)+"信心十足的走向休息室。\n");
                        gm1->move(__DIR__ + place);
                        gm1->set_temp("leitai", 1);
                        tell_room(environment(gm1),HIY+gm1->name(1)+"兴致冲冲的走了过来。\n"NOR);
                        gm2 = replace_player(pl2, gm2);
                        tell_room(environment(gm2),gm2->name(2)+"信心十足的走向休息室。\n");
                        gm2->move(__DIR__ + place);
                        gm2->set_temp("leitai", 1);
                        tell_room(environment(gm2),HIY+gm2->name(2)+"兴致冲冲的走了过来。\n"NOR);
                        gm3 = replace_player(pl3, gm3);
                        tell_room(environment(gm3),gm3->name(3)+"信心十足的走向休息室。\n");
                        gm3->move(__DIR__ + place);
                        gm3->set_temp("leitai", 1);
                        tell_room(environment(gm3),HIY+gm3->name(3)+"兴致冲冲的走了过来。\n"NOR);
			if (query("ceshi"))
				tell_object(me, HIY"【比武通告】 第"+CHINESE_D->chinese_number(num)+"队〖"+pl0+"〗的"
                                	+gm1->name(1)+"("+gm1->query("id")+")，"
                			+gm2->name(1)+"("+gm2->query("id")+")和"
                			+gm3->name(1)+"("+gm3->query("id")+")，
                                	已经进入试剑山庄休息室，准备就绪，随时可以开始比武。\n");
                        else
                        	message("channel:chat",HIY"【比武通告】 第"+CHINESE_D->chinese_number(num)+"队〖"+pl0+"〗的"
                                	+gm1->name(1)+"("+gm1->query("id")+")，"
                			+gm2->name(1)+"("+gm2->query("id")+")和"
                			+gm3->name(1)+"("+gm3->query("id")+")，
                                	已经进入试剑山庄休息室，准备就绪，随时可以开始比武。\n",users());
//              }
                return 1;
        }
        else {
                group = explode(ob->query("packet"+num)," ");
                pl0 = group[0];
                pl1 = group[1];
                re1 = group[2];
                pl2 = group[3];
                re2 = group[4];
                pl3 = group[5];
                re3 = group[6];
                if (re1 == "无")
                        re1 = pl1;
                if (re2 == "无")
                        re2 = pl2;
                if (re3 == "无")
                        re3 = pl2;
                if (!(gm1 = find_player(re1)))
                        return notify_fail(re1+"并不在游戏中，无法参加，请找替身，或放弃。\n");
                if (!(gm2 = find_player(re2)))
                        return notify_fail(re2+"并不在游戏中，无法参加，请找替身，或放弃。\n");
                if (!(gm3 = find_player(re3)))
                        return notify_fail(re3+"并不在游戏中，无法参加，请找替身，或放弃。\n");
                ms[0] = pl1;
                ms[1] = pl2;
                ms[2] = pl3;
                o[0] = gm1;
                o[1] = gm2;
                o[2] = gm3;
                if (!(room = load_object(__DIR__ + place)))
                        return notify_fail("ERROR!!!ROOM " + place+" NOT EXISTS! \n");
                room->set("packet",re1 + " " + re2 + " " + re3 + " ");
                room->set("party",pl0);
                ob->set_temp("pp" + where,pl0);
                ob->set_temp("p" + where + "/i1",pl1 +"("+re1+"代替)");
                ob->set_temp("p" + where + "/i2",pl2 +"("+re2+"代替)");
                ob->set_temp("p" + where + "/i3",pl3 +"("+re3+"代替)");
                
                for (i=0;i<3;i++) {
                        o[i] = replace_player(ms[i], o[i]);
                        tell_room(environment(o[i]),o[i]->name(1)+"兴致冲冲的步向试剑山庄，准备参加比武。\n");
                        o[i]->move(__DIR__ + place);
                        o[i]->set_temp("leitai",1);
                        tell_room(environment(o[i]),HIY+o[i]->name(1)+"兴致冲冲的走了过来，摩拳擦掌，跃跃欲试。\n"NOR);
                        msg += o[i]->query_temp("replace_msg");
                        continue;
                }
                if (query("ceshi"))
                	tell_object(me,HIY"【比武通告】 第"+CHINESE_D->chinese_number(num)+"队〖"+pl0+"〗的"+msg+
                                "已经进入试剑山庄休息室，准备就绪，随时可以开始比武。\n");
                else
                	message("channel:chat",HIY"【比武通告】 第"+CHINESE_D->chinese_number(num)+"队〖"+pl0+"〗的"+msg+
                                "已经进入试剑山庄休息室，准备就绪，随时可以开始比武。\n",users());
                return 1;
        }

}

int do_send(string arg)
{
        object me = this_player();
        object ob = this_object();
        int num1, num2;
        
        if (!arg)
                return notify_fail("指令格式：send (第几队) with (第几队)\n");
        
        if (sscanf(arg,"%d with %d",num1,num2)!=2)
                return notify_fail("指令格式：send (第几队) with (第几队)\n");
        if (!ob->query("packet"+num1))
                return notify_fail("这里并没有第"+CHINESE_D->chinese_number(num1)+"队。\n");
        if (!ob->query("packet"+num2))
                return notify_fail("这里并没有第"+CHINESE_D->chinese_number(num2)+"队。\n");
                
        if (resolve_player(me, ob, num1, 0))
                tell_object(me,"传送第"+CHINESE_D->chinese_number(num1)+"成功！\n");
        else
                tell_object(me,"传送第"+CHINESE_D->chinese_number(num1)+"失败！\n");
        if (resolve_player(me, ob, num2, 1))
                tell_object(me,"传送第"+CHINESE_D->chinese_number(num2)+"成功！\n");
        else
                tell_object(me,"传送第"+CHINESE_D->chinese_number(num2)+"失败！\n");
        return 1;
}

void check_trigger()
{
       object room;
       object thisroom = this_object();
       if(!(room = find_object(__DIR__"xiuxishi2")))
             room = load_object(__DIR__"xiuxishi2");
       if( room = find_object(__DIR__"xiuxishi2")){
         if( !room->query("exits/enter")){
                 room->set("exits/enter", __DIR__"gate");                 
                 message("vision", HIR"只听见轰然一声，试剑山庄南休息室的大门被打开了。\n"NOR, thisroom);
                 message("vision", HIW"只听见咯吱一声，休息室的大门被打开了，外面就通向试剑山庄了。\n"NOR, room);
                 remove_call_out("close");
                 call_out("close", 100); 
                 }
                else message("vision", HIW"试剑山庄南休息室的大门已经被打开了，再按也没有用了。\n"NOR, thisroom);
           } 
          else message("vision", "ERROR: Room 2 not found\n", thisroom);          
}

void check_trigger1()
{
       object room1;
       object thisroom = this_object();
       if(!(room1 = find_object(__DIR__"xiuxishi1")))
             room1 = load_object(__DIR__"xiuxishi1");
       if( room1 = find_object(__DIR__"xiuxishi1")){
         if( !room1->query("exits/enter")){                 
                 room1->set("exits/enter",__DIR__"houting");
                 message("vision", HIR"只听见轰然一声，试剑山庄北休息室的大门被打开了。\n"NOR, thisroom);
                 message("vision", HIW"只听见咯吱一声，休息室的大门被打开了，外面就通向试剑山庄了。\n"NOR, room1);
                 remove_call_out("close1");
                 call_out("close1", 100); 
                 }
                else message("vision", HIW"试剑山庄北休息室的大门已经被打开了，再按也没有用了。\n"NOR, thisroom);
           } 
          else message("vision", "ERROR: Room 1 not found\n", thisroom);          
}

void close()
{
        object room;
        if( room = find_object(__DIR__"xiuxishi2"))
        room->delete("exits/enter");        
        message("vision", HIW"只听见咯吱一声，休息室的大门又被关上了。\n"NOR, room);
}

void close1()
{
        object room1;
        if( room1 = find_object(__DIR__"xiuxishi1"))
        room1->delete("exits/enter");
        message("vision", HIW"只听见咯吱一声，休息室的大门又被关上了。\n"NOR, room1);
}

int do_push(string arg)
{
       	object me = this_player();
       	object ob = this_object();
       
       	if (userp(me) && !wiz_level(me))
          	return notify_fail("你不是巫师，不要随便乱动，坏了你可赔不起哦！\n");
       	if (!arg || arg !="anniu")
          	return notify_fail("你要按什么？\n");
       	if (arg == "anniu"){
           	message_vision(HIR"$N轻轻嘘了口气，提起食指慢慢地按动了按钮。\n"NOR, me);
           	if (query("ceshi"))
           		tell_object(me, "【比武通告】〖"+ob->query_temp("pp1")+"〗和〖"+ob->query_temp("pp2")+"〗两队正式开始比武。\n\n\t〖"+ob->query_temp("pp1")+"〗成员："+ob->query_temp("p1/i1")+"，"+ob->query_temp("p1/i2")+"，"+ob->query_temp("p1/i3")+"。\n\t〖"+ob->query_temp("pp2")+"〗成员："+ob->query_temp("p2/i1")+"，"+ob->query_temp("p2/i2")+"，"+ob->query_temp("p2/i3")+"。\n");
           	else
           		message("channel:chat","【比武通告】〖"+ob->query_temp("pp1")+"〗和〖"+ob->query_temp("pp2")+"〗两队正式开始比武。\n\n\t〖"+ob->query_temp("pp1")+"〗成员："+ob->query_temp("p1/i1")+"，"+ob->query_temp("p1/i2")+"，"+ob->query_temp("p1/i3")+"。\n\t〖"+ob->query_temp("pp2")+"〗成员："+ob->query_temp("p2/i1")+"，"+ob->query_temp("p2/i2")+"，"+ob->query_temp("p2/i3")+"。\n",users());
           	check_trigger();
           	check_trigger1();
           	return 1;
	} 
        return notify_fail("你要按什么？\n");
}
