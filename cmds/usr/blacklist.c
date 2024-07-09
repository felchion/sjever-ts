// blacklist.c by tiantian@sj 2000.8.8

#include <ansi.h>
#include <login.h>

#define BL_FILE       "/data/blacklist"
#define NULL 0

inherit ITEM;
inherit F_SAVE;

static object ob = 0;
static string rcpt,msg,subject;

mapping *notes;

string seed = "AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz";

string query_save_file();
string make_password();
void list_bllist(object me, int num);

string _make();
int is_black(string arg)
{
	foreach (mapping item in notes)
		if (arg == item["player"]) {
			if (!item["expiration"] || item["expiration"] > time() )
			return 1;
		}
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
	string s,arg1,arg2;
	int num;
	object ob;
	string password;
	int days;
	
	if (!arg) {
		list_bllist(me, 10);
		return 1;
	}

    if (sscanf(arg, "%s", s) == 1) {
		if ( s == "all" ) {
			list_bllist(me,0);
			return 1;
		}
	}

	if (sscanf(arg, "%s %d because %s", arg1, days,arg2) == 3) {
		if (wiz_level(me)<4)
			return notify_fail("只有arch才可以将玩家列入黑名单。 \n");
		if (strlen(arg2)< 8 || strlen(arg2)>44)
			return notify_fail("请简明扼要地说明原因，字数必须在4-20之间。 \n");
        ob = LOGIN_D->find_body(arg1);
		if(ob)
			"/cmds/wiz/kickout"->main(me, arg1);
        ob = LOGIN_D->find_body(arg1);
		if(!ob) {
			if( file_size(DATA_DIR + "login/" + arg1[0..0] + "/" + arg1 + SAVE_EXTENSION)<0 )
                return notify_fail("没有这位使用者。\n");
			ob = new(LOGIN_OB);
			ob->set("id",arg1);
			if( !ob->restore() ) {
				destruct(ob);
				return notify_fail("没有这个玩家。\n");
			}
			else {
			//	password = make_password();
			//	ob->set("password",crypt(password, "$1$ShuJian"));
				log_file("static/bllist",
					sprintf("%s(%s)把%s(%s)列入黑名单。\n", 
						me->query("name"), me->query("id"),
						ob->query("name"), ob->query("id")
					), me, ({ ob->query("id") })
				);
				ob->save();
				destruct(ob);
			}
		}
		note = allocate_mapping(6);
		note["time"] = time();
		note["block"] = me->query("id");
		note["player"] = arg1;
		note["reason"] = arg2;
		
		note["unblock"] = NULL;
		note["result"] = NULL;
    if (days)	note["expiration"] = time() + 86400 * days;
        if( !pointerp(notes) || !sizeof(notes) )
                notes = ({ note });
        else
                notes += ({ note });
		save();
		write("你把"+arg1+"成功地列入黑名单。\n");
		return 1;
	}

	if (sscanf(arg, "-d %d", num)== 1) {
		if ( wiz_level(me)<4 )
			return notify_fail("只有arch才可以删除黑名单上的内容。 \n");
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
//				password = make_password();
//				ob->set("password",crypt(password, "$1$ShuJian"));
				log_file( "static/bllist",
					sprintf("%s(%s)把黑名单上关于%s(%s)的内容删除。\n", 
						me->query("name"), me->query("id"),
						ob->query("name"), ob->query("id")
					), me, ({ ob->query("id") })
				);
//				ob->save();
				destruct(ob);
//				write("用户"+arg1+"的密码被修改为"+password+"，请通过mail通知该用户。\n");
			}
        }
		else
			return notify_fail("有没有搞错？黑名单上的人怎么上线了？你先把他踢出去再说吧。\n");
		notes = notes[0..num-1] + notes[num+1..<1];
		save();
		write("第"+ (num+1) + "号内容已经删除。\n");
		return 1;
	}

	if (sscanf(arg, "-r %d %s", num, arg2)== 2) {
		if ( wiz_level(me)<4 )
			return notify_fail("只有arch才可以修改黑名单上的内容。 \n");
		if( !arrayp(notes) || num < 1 || num > sizeof(notes) )
			return notify_fail("没有这个编号的内容。\n");
		if( notes[num-1]["result"] && notes[num-1]["unblock"] )
			return notify_fail("这个玩家已经有了调查结果了。\n");
		arg1=notes[num-1]["player"];
        ob = LOGIN_D->find_body(arg1);
        if(!ob){
			ob = new(LOGIN_OB);
			ob->set("id",arg1);
			if( !ob->restore() ) {
				destruct(ob);
			}
			else {
			//	password = make_password();
			//	ob->set("password",crypt(password, "$1$ShuJian"));
				log_file("static/bllist",
					sprintf("%s(%s)加入黑名单上关于%s(%s)的调查结果。\n", 
						me->query("name"), me->query("id"),
						ob->query("name"), ob->query("id")
					), me, ({ ob->query("id") })
				);
				ob->save();
				destruct(ob);
				write("用户"+arg1+"的密码被修改为"+password+"，请通过mail通知该用户。\n");
			}
        }
		else
			return notify_fail("有没有搞错？黑名单上的人怎么上线了？你先把他踢出去再说吧。\n");
		notes[num-1]["unblock"]=me->query("id");
		notes[num-1]["result"]=arg2;
		save();
		write("第"+ (num) + "号内容修改完毕。\n");
		return 1;
	}

	if (sscanf(arg, "-dr %d", num)== 1) {
		if ( wiz_level(me)<4 )
			return notify_fail("只有arch才可以修改黑名单上的内容。 \n");
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
		//		password = make_password();
		//		ob->set("password",crypt(password, "$1$ShuJian"));
				log_file("static/bllist",
					sprintf("%s(%s)删除黑名单上关于%s(%s)的调查结果。\n", 
						me->query("name"), me->query("id"),
						ob->query("name"), ob->query("id")
					), me, ({ ob->query("id") })
				);
				ob->save();
				destruct(ob);
			}
        }
		else
			return notify_fail("有没有搞错？黑名单上的人怎么上线了？你先把他踢出去再说吧。\n");
		notes[num-1]["unblock"]=NULL;
		notes[num-1]["result"]=NULL;
		save();
		write("第"+ (num) + "号内容修改完毕。\n");
		return 1;
	}

	write("语法错误，请help blacklist。\n");
	return 1;
}

string query_save_file()
{
	return BL_FILE ;
}

string make_password()
{
        int i = 8, j;
        string id = "";

        while (i--) {
                j = random(52);
                id += seed[j..j];
        }
//	return id;
	return "";
}

void list_bllist(object me, int num)
{
	int i,j;
	string str;

	i = sizeof(notes);
	if( i < 1 ) write("书剑黑名单目前没有任何内容。\n");
	else {
		str="书剑黑名单：\n";
		str += repeat_string("-", 120) + "\n";
		while (i-- && (i>=sizeof(notes)-num || num==0)) {
			if ( notes[i]["expiration"]>0 && notes[i]["expiration"]<time()) {
				notes = notes[0..i-1] + notes[i+1..<1];
				//i++;
			}
			else {
				++j;
				str += sprintf("%3d %s %-9s因为 %-=22s 被%s列入黑名单，%s\n",
					i+1,
					ctime(notes[i]["time"])[4..15],
					notes[i]["player"],
					notes[i]["reason"],
					wizardp(me)?notes[i]["block"]:"",
					notes[i]["expiration"]?"禁闭时间还剩"+CHINESE_D->chinese_time(notes[i]["expiration"]-time()):HIR"无限期"NOR	
				);
				str += repeat_string("-", 120) + "\n";
			}	
		}
	}
	me->start_more( str );
}

int help(object me)
{
	if (!wiz_level(me) )
        write(@HELP
指令格式 :
blacklist [all]                 查看黑名单的内容和处理结果

巫师将被怀疑违反规则的玩家登记入黑名单接受调查，在接受调查期间，
该玩家id将被暂时封闭，不能联线游戏。
调查结束之后，巫师会将该玩家id的新密码发送到玩家的注册信箱。
如果玩家发现自己密码错误，无法登录，可以使用其他id查看黑名单，看
自己是否正在结束调查。

HELP);
    else 
        write(@HELP
指令格式 :
blacklist [all]                        查看黑名单的内容和处理结果
blacklist <id> <天数> because <原因>   将玩家列入黑名单，等候调查处理(天数为0时，为限期封禁）
blacklist -d <数字>                    删除指定编号的黑名单
blacklist -r <数字> <结论>             登记黑名单上指定标号的事件的调查结果
blacklist -dr <数字>                   删除指定编号的黑名单的调查结果

巫师将被怀疑违反规则的玩家登记入黑名单接受调查，在接受调查期间，
该玩家id将被暂时封闭，不能联线游戏。
调查结束之后，巫师会将该玩家id的新密码发送到玩家的注册信箱。
如果玩家发现自己密码错误，无法登录，可以使用其他id查看黑名单，看
自己是否正在结束调查。

HELP);
        return 1;
}
