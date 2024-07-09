// seek.c Looklove@sj 2002.7.19 传唤玩家，提示在线巫师，目标玩家登录。
// 应放在/cmds/wiz/下面，然后在logind.c 里面加上一句提示该玩家登录的信息。

#include <ansi.h>
#include <login.h>

#define SEEK_FILE       "/data/seek"
#define NULL 0

inherit ITEM;
inherit F_SAVE;

static object ob = 0;
static string rcpt,msg,subject;

mapping *notes;

string query_save_file();

void list_seeklist(object me, int num);

int is_seek(string target, string arg)
{
	foreach (mapping item in notes)
		if (target == item["player"]
		&& arg == item["block"]
		&& LOGIN_D->find_body(arg) )
			return 1;
	return 0;
}

void create()
{
	seteuid(getuid());
	restore();
}

int main(object me, string arg)
{
	mapping note;
	string s, arg1, arg2;
	int num;
	object ob;

	if (!arg) {
		list_seeklist(me, 10);
		return 1;
	}

	if (sscanf(arg, "%s", s) == 1) {
		if ( s == "all" ) {
			list_seeklist(me,0);
			return 1;
		}
	}

	if (sscanf(arg, "%s because %s", arg1, arg2) == 2) {
		if (strlen(arg2)< 8 || strlen(arg2)>44)
			return notify_fail("请简明扼要地说明原因，字数必须在4-20之间。 \n");
		ob = LOGIN_D->find_body(arg1);
		if(ob)
			return notify_fail("此人已经在线，不必等待，请立即处理。\n");

                ob = new(LOADUSER_OB, arg1);
                if (!ob->restore()) {
                        destruct(ob);
                        return notify_fail("没有这个玩家。\n");
                }
		else {
			log_file("static/seeklist",
				sprintf("%s(%s)传唤%s(%s)。\n",
					me->query("name"), me->query("id"),
					ob->query("name"), ob->query("id")
				), me, ({ ob->query("id") })
			);
			destruct(ob);
		}
		note = allocate_mapping(6);
		note["time"] = time();
		note["block"] = me->query("id");
		note["player"] = arg1;
		note["reason"] = arg2;
		note["unblock"] = NULL;
		note["result"] = NULL;
		if( !pointerp(notes) || !sizeof(notes) )
			notes = ({ note });
		else
			notes += ({ note });
		save();
		write("你成功把"+arg1+"加入传唤名单，此人上线系统会提醒你。\n");
		return 1;
	}

	if (sscanf(arg, "-d %d", num)== 1) {
		if( !arrayp(notes) || num < 1 || num > sizeof(notes) )
			return notify_fail("没有这个编号的内容。\n");
		num--;
		arg1=notes[num]["player"];
		ob = LOGIN_D->find_body(arg1);
		if(!ob){
			ob = new(LOGIN_OB);
			ob->set("id",arg1);
			if( !ob->restore() ) {
				destruct(ob);
			}
			else {
				log_file( "static/seeklist",
					sprintf("%s(%s)把%s(%s)从传唤名单上删除。\n",
						me->query("name"), me->query("id"),
						ob->query("name"), ob->query("id")
					), me, ({ ob->query("id") })
				);
				destruct(ob);
				write("用户"+arg1+"被清除传唤名单。\n");
			}
        	}
		notes = notes[0..num-1] + notes[num+1..<1];
		save();
		write("第"+ (num+1) + "号内容已经删除。\n");
		return 1;
	}

	if (sscanf(arg, "-r %d %s", num, arg2)== 2) {
		if( !arrayp(notes) || num < 1 || num > sizeof(notes) )
			return notify_fail("没有这个编号的内容。\n");
		if( notes[num-1]["result"] && notes[num-1]["unblock"] )
			return notify_fail("这个玩家已经有了处理结果了。\n");
		arg1=notes[num-1]["player"];
        ob = LOGIN_D->find_body(arg1);
        if(!ob){
			ob = new(LOGIN_OB);
			ob->set("id",arg1);
			if( !ob->restore() ) {
				destruct(ob);
			}
			else {
				log_file("static/seeklist",
					sprintf("%s(%s)加入传唤名单上关于%s(%s)的处理结果。\n",
						me->query("name"), me->query("id"),
						ob->query("name"), ob->query("id")
					), me, ({ ob->query("id") })
				);
				ob->save();
				destruct(ob);
			}
        }
		notes[num-1]["unblock"]=me->query("id");
		notes[num-1]["result"]=arg2;
		save();
		write("第"+ (num) + "号内容修改完毕。\n");
		return 1;
	}

	if (sscanf(arg, "-dr %d", num)== 1) {
		if( !arrayp(notes) || num < 1 || num > sizeof(notes) )
			return notify_fail("没有这个编号的内容。\n");
		arg1=notes[num-1]["player"];
        ob = LOGIN_D->find_body(arg1);
        if(!ob){
			ob = new(LOGIN_OB);
			ob->set("id",arg1);
			if( !ob->restore() ) {
				destruct(ob);
			}
			else {
				log_file("static/seeklist",
					sprintf("%s(%s)删除传唤名单上关于%s(%s)的处理结果。\n",
						me->query("name"), me->query("id"),
						ob->query("name"), ob->query("id")
					), me, ({ ob->query("id") })
				);
				ob->save();
				destruct(ob);
			}
        }
		notes[num-1]["unblock"]=NULL;
		notes[num-1]["result"]=NULL;
		save();
		write("第"+ (num) + "号内容修改完毕。\n");
		return 1;
	}

	write("语法错误，请help seek。\n");
	return 1;
}

string query_save_file()
{
	return SEEK_FILE ;
}

void list_seeklist(object me, int num)
{
	int i;
	string str;

	i = sizeof(notes);
	if( i < 1 ) write("传唤名单目前没有任何内容。\n");
	else {
		str="传唤名单：\n";
		str += repeat_string("-", 80) + "\n";
		while (i-- && (i>=sizeof(notes)-num || num==0)) {
			str += sprintf("%3d %s %-9s %-=22s %s %s \n",
				i+1,
				ctime(notes[i]["time"])[4..15],
				notes[i]["player"],
				notes[i]["reason"],
				notes[i]["result"] && notes[i]["unblock"]?
					("结果："+notes[i]["result"]+
						(wizardp(me)?
							("，"+notes[i]["unblock"]+"处理")
							:"")
						)
					:"名单",
				wizardp(me)?notes[i]["block"]:"",
			);
			str += repeat_string("-", 80) + "\n";
		}
	}
	me->start_more( str );
}

int help(object me)
{
        write(@HELP
指令格式 :
seek [all]                 查看传唤名单的内容和处理结果
seek <id> because <原因>   将玩家列入传唤名单，等候通知
seek -d <数字>             删除指定编号的传唤名单
seek -r <数字> <结论>      登记传唤名单上指定标号的事件的处理结果
seek -dr <数字>            删除指定编号的传唤名单的处理结果

巫师将需要处理的事务中涉及的玩家登记入传唤名单，
当该玩家登录的时候，系统就会提醒巫师进行处理。

HELP);
        return 1;
}
