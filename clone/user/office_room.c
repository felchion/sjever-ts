// office_room.c
// Created By Numa@SJ 2001/03/02
// for solve player's problem automaticly.

/*
*** cmds help format :
string help()
{
	return sprintf(HIW"│"GRN"%|20s  "NOR"%|60s"HIW"│\n","recovers [id]","可以自动恢复自杀的玩家。");
}

*** cmds transfer format :
string do_auto(object player, string id)
{
	...;
}
*/

#include <ansi.h>
inherit ROOM;

// define office_room's commands directory
#define DIR "/clone/user/office/"
string *file = ({});
object ob = 0;

// define the limit to player
int player_limit(object ob)
{
//	if (ob->query("age") < 20)
//		return 0;
	return 1;
}

// find all cmds
void check_cmds()
{
	object f;
	mixed *d;
	int i, fl;
	
        d = get_dir(DIR,-1);
        for (i=0;i<sizeof(d);i++) {
                if (d[i][1] == -2)
                        continue;
                fl = strlen(d[i][0]);
                if (d[i][1] <= 0
                 || d[i][0][(fl-2)..(fl-1)] != ".c")
                        continue;
    	        if (!(f = find_object(DIR+d[i][0])))
    			f = load_object(DIR+d[i][0]);
    		file += ({d[i][0][0..(fl-3)]});
        }
}

void create()
{
        set("short", HIW"玩家事务办公室"NOR);
        set("long", "这里是"+MUD_NAME+"的玩家事务办公室室。
在招待台里，一个美丽可人的姑娘正在登记着什么。不少有问题的玩家都
在招待台旁边的长椅上等候解决。在办公桌上有一本厚厚的玩家事务说明
(help)。\n");
        set("exits", ([
               "down" : "/d/wuguan/dayuan"
        ]));
        set("no_death", 1);
        set("no_save", 1);
	set("no_fight", 1);
        setup();
        seteuid(ROOT_UID);
        check_cmds();
}

void init()
{
        object ob = this_player();
        
        add_action("do_help","help");
	if (!player_limit(ob)) return;
	add_action("ido","ido");
}

int check_valid_player(string my_id, string id)
{
	object my_ob, ob;
	
	my_ob = new(LOGIN_OB);
	my_ob->set("id", my_id);
    	if (!my_ob->restore()) {
    		destruct(my_ob);
    		return 0;
    	}

	ob = new(LOGIN_OB);
	ob->set("id", id);
    	if (!ob->restore()) {
    		destruct(ob);
    		return 0;
    	}
//        if (ob->query("email") == my_ob->query("email")) {
        if (ob->query("password") == my_ob->query("password")) {
    		destruct(my_ob);
    		destruct(ob);
    		return 1;
    	}
  	destruct(my_ob);
    	destruct(ob);
    	return 0;
}

int ido(string arg)
{
	object me = this_player();
	object file_ob;
	string file, act, id, msg = "";
	
	if (ob)
		return notify_fail("请稍等，系统正在解决"+ob->name()+"的问题。\n");
		
	if (!arg || sscanf(arg, "%s %s", act, id) != 2)
		return notify_fail("你要解决什么问题？请使用 help 指令查看处理指令。\n");
        seteuid(ROOT_UID);
        act = lower_case(act);
        id = lower_case(id);
	if (id == me->query("id"))
		return notify_fail("你不能在这里处理您自己的id，请重新登陆。\n");
    	if (!stringp(file = (string)DIR+act)
   	 || file_size(file + ".c") <= 0 )
   		return notify_fail("你要解决什么问题？\n");

	// check player's rights
	if (check_valid_player(me->query("id"), id))
		return notify_fail("对不起，你的身份验证失败，无法使用此指令。\n");
	
    	if (!(file_ob = find_object(file)) )
    		file_ob = load_object(file);
	ob = me;
	msg += file_ob->do_auto(me, id);
        // log_file
        log_file("static/office",
                sprintf("%s %s使用( %s )命令成功执行，返回信息：%s",
			ctime(time()), geteuid(me), arg, msg));
	tell_room(this_object(),HIW"\n"+ob->name()+"的事务处理完毕，请下一位继续。\n"NOR);
	ob = 0;
	return notify_fail(msg);

}

int do_help(string arg)
{
	object me = this_player();
	object f;
	string output, *hp;
	int i;
	
	if (arg)
		return 0;
	output = "\n\n\t\t\t\t"HIG"玩家事务说明\n"NOR+
		"\n\n\t本网络游戏即将开始玩家事务自动化，玩家的问题可以在这里得到基本解决。\n"+
		"\n\t目前暂时开放的玩家事务处理指令具体如下：\n\n";
	output += HIW"┌─────────────────────────────────────────┐\n";
	output += sprintf(HIW"│"HIY"%|20s"HIW"│"HIY"%|60s"HIW"│\n","处理指令","指令说明");
	output += HIW"├──────────┴──────────────────────────────┤\n";
        for (i=0;i<sizeof(file);i++) {
    	        if (!(f = find_object(DIR+file[i])))
    			f = load_object(DIR+file[i]);
    		hp = f->help();
    		output += sprintf(HIW"│"GRN"%-20s  "NOR"%-60s"HIW"│\n","ido "+hp[0],hp[1]);
        }
        output += HIW"└─────────────────────────────────────────┘\n";
        output += HIR"注意："HIC"如果你有问题需要解决，那么请注册另一个相同密码的 id ，在注册后来此处\n进行事务处理。 \n\n"NOR;
        output += "\t如有任何问题或有何建议，请向巫师组报告。\n\n\n"+
        	"\t\t\t\t\t\t"HIW"书剑"HIC"巫师组"HIY"玩家事务处理办公室\n"NOR;
        me->start_more(output);
        return 1;
}
