// msg.c
// Created by mxzhao 2003/11/21

// 短消息相关数据结构
/******************************************************
sets
([
	"send_max":5,   // 可以同时发送的最多用户数
	"cmd_level":0,  // 可以使用这个指令的用户等级
	"set_level": 4, // 可以更改设置的用户等级
	"wiz_count":20, // wiz的信箱容量
	"vip_count":15, // vip用户的信箱容量
	"std_count":10, // 普通用户的信箱容量
])

users 
([ 
	"mxzhao": 
	([ 
		"cur_id":"",
		"msg_id":
		([ 
			"978594457mxzhao":
			([
				"isread":1,	
				"islock":1, 
			]),
			...
		]), 
	]),
	...
])

messages 
([
	"978594457mxzhao":
	([
		"title":"TEST",
		"text":"TEST TEXT",
		"time":978594457,
		"sender":(["id":"mxzhao","name":"测试"]),
		"addressee": 
		([
			"mxzhao", ...
		]),
	]),
	...
])
*********************************************************/
// 其中 mxzhao 为用户ID，978594457mxzhao消息ID




#define MESSAGE_FILE	DATA_DIR + "msg"
#define MESSAGE_HEAD	HIC +"【"+ CHINESE_MUD_NAME + "短消息系统】："NOR
#define MESSAGE_LINE	HIY+"≡———————————————————————————————————————————≡\n"+NOR 
#define MAX_VALUE		20
#define MIN_VALUE		0

inherit F_SAVE;
inherit F_CLEAN_UP;

#include <ansi.h>
#include <mudlib.h>

string query_save_file();

private string make_result(string str);
private void write_result(string str);
private int load_message();
private string show_sets(object me);
private void start_send(string title, object me, mapping msg);
private void end_send(object me, mapping msg, string text);
private int save_message(object me, mapping msg);
private string make_title(mapping msg_id, mapping msg, int row);
private int do_clear_msg(string msg_id);
private int do_clear_user(string name);
private varargs int delete_user_all(object me, string name, int flag); 
private string make_statistic(object me);

int do_statistic(object me);			// 可以在LOGIN_D中调用
int do_send(object me, string arg);
varargs int do_read(object me, string arg, int flag);
int do_discard(object me, string arg);
int do_lock(object me, string arg, int lock);
int do_show(object me, string arg);
int do_clear(object me, string arg);	// 清除用户的所有短消息，可以在purge、suicide等函数中调用
int do_set(object me, string arg);
int help(object me);

mapping sets;		// 一些短消息的设置
mapping users;		// 短消息用户信息
mapping messages;	// 短消息

// override query_save_file() 
// 在 F_SAVE 中使用
string query_save_file()
{
	return MESSAGE_FILE;
}

// 给字符串加上 MESSAGE_HEAD
private string make_result(string str)
{
	return MESSAGE_HEAD + HIC + str + NOR;
}

// 显示信息
private void write_result(string str)
{
	write(make_result(str) + "\n");
}

// 载入短消息信息，如果没有相关信息则设定初始值
private int load_message()
{
	if ( !mapp(messages) || !mapp(users) || !mapp(sets) )
	{
		restore();

		write_result("载入成功。");
	}
	
	if( !mapp(sets) || sizeof(sets) == 0 )
	{
		sets = ([ "send_max":5, 
			"cmd_level":0,	
			"set_level": 4, 
			"wiz_count":20,	
			"vip_count":15, 
			"std_count":10 ]);
	}

	if( !mapp(users) || sizeof(users) == 0 )
	{
		users = ([]);
	}

	if( !mapp(messages) || sizeof(messages) == 0 )
	{
		messages = ([]);
	}

	return 0;
}

// 返回所有sets的内容
private string show_sets(object me)
{
	string str = "";
	string *keys = sort_array(keys(sets), 1);

	foreach (string key in keys)
	{
		str += sprintf("	%-15s:%5d\n", key, sets[key]);
	}

	return str;
}

// 发送短消息
int do_send(object me, string arg)
{
	string file;
	object ob;
	mapping msg = ([ ]);
	
	string *names = explode(arg, ":");

	msg["addressee"] = ({ });
	
	if (!wizardp(me) && sizeof(names) >  sets["send_max"] )
	{
		write_result("您最多能同时向" + chinese_number(sets["send_max"]) + "位用户发送短消息。");
		return 1;
	}

	foreach (string name in names )
	{
		if ( me->query("id") == name )
		{
			write_result("你不能给自己发送短消息。");
			return 1;
		}
		
		file = DATA_DIR + "login/"+ name[0..0] +"/"+ name;
		
		ob = LOGIN_D->find_body(name);
		
		if( ob && me->visible(ob) ) 
		{
			write_result(ob->name() + "(" + capitalize(name) + ")正在连线中，你还是使用 Tell 指令更方便些。");
			return 1;
		} 
		else if( file_size(file + ".ppp") > 0 )
		{
			write_result(capitalize(name) + "已经被删除了。");
			return 1;
		}
		else if( file_size(file +__SAVE_EXTENSION__) < 0 )
		{
			write_result("没有" + capitalize(name) + "这位用户。");
			return 1;
		}

		{
			int msg_count = 0;
			
			if(getuid(me)==ROOT_UID)			
				msg_count = 9999;//hehe by ciwei				
			else if (wiz_level(name) > 0)
			{
				msg_count = sets["wiz_count"];
			}
			else if (me->query("registered")>2)
			{
				msg_count = sets["vip_count"];
			}
			else
			{
				msg_count = sets["std_count"];
			}
			
			if ( mapp(users[name]) && sizeof(users[name]["msg_id"]) >= msg_count )
			{
				write_result( capitalize(name) + "的信箱已满。" );
				return 1;
			}
		}

		msg["addressee"] += ({ name });
	}
	
	msg["sender"] = ([ "id":me->query("id"), "name":me->query("name") ]);
		
	write(HIG +"请输入您要发送短消息的标题(40个字符以下)："+NOR);
	input_to("start_send", me, msg);
	
	return 0;
}

//系统向player 发送msg
public void system_send(object me,string* geters,string title,string text)
{
	mapping msg =([]);
	if(!me || getuid(me)!=ROOT_UID) return;
	if(!arrayp(geters) || !stringp(title) || !stringp(text)) return;
	msg["addressee"] = geters;
	msg["sender"] = ([ "id":"System", "name":"系统" ]);
	msg["title"] = title;
	msg["text"] = text;
	save_message(me, msg);		
}

// 处理输入的标题(40字符以内)，开始输入本文。
private void start_send(string title, object me, mapping msg)
{
	if ( strlen(title) > 20 )
	{
		title = title[0..19];
	}
	write(HIG+"请输入您要发送的短消息的内容（不能大于 500 字）：\n" + NOR);
	msg["title"] = title;

	me->edit( (: end_send, me, msg :) );
}

// 结束输入，存盘
private void end_send(object me, mapping msg, string text)
{
	msg["text"] = text;
	
	if ( sizeof(text) > 500 ){
		write_result("对不起，您发送的短消息内容大于了 500 字！\n") ;
		return;
	}
	if (!save_message(me, msg))
	{
		write_result("短消息发送成功。");
	}
}

static int seed=0;
private int get_seeds()
{
	seed++;
	return seed;
}
// 存储新短消息
private int save_message(object me, mapping msg)
{
	string msg_id;

	if ( load_message() )
	{
		return 1;
	}

	msg["time"] = time();

	msg_id = sprintf("%s%d%s%d",msg["addressee"][0], msg["time"], msg["sender"]["id"],get_seeds() );
	//增加系统发送msg 功能 而系统有可能同时向多id发msg所以这个改一下msg_id
	//增强唯一性
	messages[msg_id] = msg;

	foreach ( string name in msg["addressee"] )
	{
		mapping user = users[name];

		if ( !mapp(user) || sizeof(user) == 0)
		{
			user = ([ ]);

			user["cur_id"] = 0;
			user["msg_id"] = ([ ]);
		}
		
		user["msg_id"] += ([ msg_id:(["islock":0, "isread":0]) ]);

		users[name] = user;
	}
	
	save();

	return 0;
}

// 返回第 row 行短消息标题等字符串
private string make_title(mapping msg_id, mapping msg, int row)
{
	//		no.) lock title name(id) (time)
	if (!sizeof(msg_id) || !sizeof(msg))
		return "收到错误信息。\n";
	return sprintf("%-2s%s%3d) %-40s %18s (%s)" NOR "\n", msg_id["islock"]?HIR"♀"NOR:"", 
		msg_id["isread"]?"":(HIG BRED), row, msg["title"], msg["sender"]["name"] + "(" + 
		capitalize(msg["sender"]["id"]) + ")", ctime(msg["time"])[4..] );
}

// 如果短消息的收件人("addressee")为空，则删除这个短消息
private int do_clear_msg(string msg_id)
{
	if ( !mapp(messages[msg_id]) )
	{
		return 0;
	}

	{
		string *addressee = messages[msg_id]["addressee"];
		if ( !arrayp(addressee) || sizeof(addressee) == 0)
		{
			map_delete(messages, msg_id);
			return 1;
		}
	}
	
	return 0;
}

// 如果用户资料中没有短消息，则删除用户资料
private int do_clear_user(string name)
{
	if ( !mapp(users[name]) )
	{
		return 0;
	}

	{
		string *msg_id = users[name]["msg_id"];

		if ( !mapp(msg_id) || sizeof(msg_id) == 0)
		{
			map_delete(users, name);

			return 1;
		}
	}
	
	return 0;
}

// 清除用户的短消息
// flag		0：	则不删除被锁定的短消息，
//			1：	则删除所有短消息
private varargs int delete_user_all(object me, string name, int flag)
{
	int count = 0;
	string *msg_ids = sort_array(keys(users[name]["msg_id"]), 1);

	foreach ( string msg_id in msg_ids )
	{
		if ( flag || !users[name]["msg_id"][msg_id]["islock"] )
		{
			map_delete(	users[name]["msg_id"], msg_id );
			messages[msg_id]["addressee"] -= ({ name });
			
			do_clear_msg(msg_id);
			
			count++;
		}
	}
	
	do_clear_user(name);
	
	return count;
}

// 返回用户短消息的统计信息。
private string make_statistic(object me)
{
	int total = 0;
	int read = 0;
	int lock = 0;

	string name = me->query("id");

	if (!mapp(users[name]) || sizeof(users[name]) == 0 
		|| !mapp(users[name]["msg_id"]) || sizeof(users[name]["msg_id"]) == 0 )
	{
		return "您目前没有任何短消息。";
	}

	total = sizeof(users[name]["msg_id"]);
	
	foreach ( mapping msg_id in keys(users[name]["msg_id"]) )
	{
		if ( users[name]["msg_id"][msg_id]["isread"] )
		{
			read++;
		}
		
		if ( users[name]["msg_id"][msg_id]["islock"] )
		{
			lock++;
		}
	}

	return "您目前共有" + chinese_number(total) + "条短消息，其中" + 
		chinese_number(total-read) + "条未读，" + chinese_number(lock) + "条锁定。";
}

// 显示用户短消息的统计信息。
int do_statistic(object me)
{
	if (load_message())
	{
		return 1;
	}

	if ( (string)SECURITY_D->get_status(me) != "(admin)" 
		&& wiz_level(me) < sets["cmd_level"] )
	{
		return 1;
	}

	write_result(HIY + make_statistic(me) + "请用 help msg 命令查看具体使用方法！"+NOR);

	return 0;
}

// 读取短消息
varargs int do_read(object me, string arg, int flag)
{
	mapping user;
	mapping msg_id;
	string *msg_ids;
	string str = "";
	string name = me->query("id");

	if (load_message())
	{
		return 1;
	}
	
	user = users[name];

	if (!mapp(user) || sizeof(user) == 0 )
	{
		write_result("您目前没有任何短消息。");
		return 1;
	}
	
	msg_id = user["msg_id"];

	if ( !mapp(msg_id) || sizeof(msg_id) == 0 )
	{
		return help(me);
	}
	
	msg_ids = sort_array(keys(msg_id), 1);

	str += make_result("");

	if ( arg == "all" )
	{
		int new_count = 0;

		str += "	" + make_statistic(me) + "\n" + MESSAGE_LINE;

		for ( int i=sizeof(msg_ids); i>0; i-- )
		{
			if ( !flag || !msg_id[msg_ids[i-1]]["isread"] )
			{
				str += make_title(msg_id[msg_ids[i-1]], messages[msg_ids[i-1]], i);

				new_count++;
			}
		}

		if ( flag && new_count == 0 )
		{
			return help(me);
			//write_result("您目前没有新的短消息。");
			//return 1;
		}
	}
	else if ( arg == "new" )
	{
		for ( int i=0; i<sizeof(msg_ids); i++ )
		{
			if ( !users[name]["msg_id"][msg_ids[i]]["isread"] )
			{
				return do_read(me, "" + (i+1));
			}
		}

		write_result("您目前没有新的短消息。");
		return 1;
	}
	else if ( arg == "next" )
	{
		string cur_id = users[name]["cur_id"];
		int i = member_array(cur_id, msg_ids);

		if ( i < 0 )
		{
			i = 0;
		}
		else
		{
			i++;

			if ( i >= sizeof(msg_ids) )
			{
				write_result("没有下一个短消息。");
				return 1;
			}
		}

		i++;
		
		return do_read(me, "" + i);
	}
	else
	{
		int num = atoi(arg);
		if ( num <= 0 || num > sizeof(msg_id))
		{
			write_result("没有这个编号的短消息。");
			return 1;
		}
		
		{
			mapping msg = messages[msg_ids[num-1]];
			str += "\n" + make_title(msg_id[msg_ids[num-1]], msg, num) 
				+ MESSAGE_LINE + msg["text"];
		}

		users[name]["msg_id"][msg_ids[num-1]]["isread"] = 1;
		users[name]["cur_id"] = msg_ids[num-1];

		save();
	}

	str += MESSAGE_LINE;

	me->start_more(str);
	
	return 0;
}

// 删除短消息
int do_discard(object me, string arg)
{
	string name = me->query("id");
	int del_count = 0;
	
	if (load_message())
	{
		return 1;
	}
	
	if (!mapp(users[name]) || sizeof(users[name]) == 0 
		|| !mapp(users[name]["msg_id"]) || sizeof(users[name]["msg_id"]) == 0 )
	{
		write_result("您目前没有任何短消息。");
		return 1;
	}
	
	if ( arg == "all" )
	{
		del_count = delete_user_all(me, name);
	}
	else
	{
		string *msg_ids = sort_array(keys(users[name]["msg_id"]), 1);

		int num = atoi(arg);
		if ( num <= 0 || num > sizeof(msg_ids))
		{
			write_result("您没有这个编号的短消息。");
			return 1;
		}
		
		
		if (users[name]["msg_id"][msg_ids[num-1]]["islock"])
		{
			write_result("此短消息已被锁定，请先解除锁定。");
			return 1;
		}

		map_delete(	users[name]["msg_id"], msg_ids[num-1] );
		messages[msg_ids[num-1]]["addressee"] -= ({ name });

		do_clear_msg(msg_ids[num-1]);
		do_clear_user(name);

		del_count++;
	}

	save();

	write_result("您删除了" + chinese_number(del_count) + "条短消息。");

	return 0;
}

// 短消息锁定/解除锁定
int do_lock(object me, string arg, int lock)
{
	string name = me->query("id");

	if (load_message())
	{
		return 1;
	}

	if (!mapp(users[name]) || sizeof(users[name]) == 0 
		|| !mapp(users[name]["msg_id"]) || sizeof(users[name]["msg_id"]) == 0 )
	{
		write_result("您目前没有任何短消息。");
		return 1;
	}

	{
		string *msg_ids = sort_array(keys(users[name]["msg_id"]), 1);
		int num = atoi(arg);
		if ( num <= 0 || num > sizeof(msg_ids))
		{
			write_result("您没有这个编号的短消息。");
			return 1;
		}
	
		if (lock == 0)
		{
			if ( !users[name]["msg_id"][msg_ids[num-1]]["islock"] )
			{
				write_result("此短消息未锁定。");
				return 1;
			}

			users[name]["msg_id"][msg_ids[num-1]]["islock"] = 0;
			write_result("第"+chinese_number(num)+"条短消息解除锁定。");
		}
		else
		{
			if ( users[name]["msg_id"][msg_ids[num-1]]["islock"] )
			{
				write_result("此短消息已锁定。");
				return 1;
			}

			users[name]["msg_id"][msg_ids[num-1]]["islock"] = 1;
			write_result("锁定第"+chinese_number(num)+"条短消息。");
		}
	}
	
	save();
	
	return 0;
}

// 显示短消息系统的统计信息
int do_show(object me, string arg)
{
	if ( !wizardp(me) )
	{
		return help(me);
	}

	if (arg == "users")
	{
		string str = "";

		str += make_result("共有" + sizeof(users) + "位用户，" + sizeof(messages) + "条短消息。\n");

		str += sprintf("        %-10s %10s\n", "用户", "消息数");
		str += MESSAGE_LINE;

		{
			int i = 1;
			foreach (string name in sort_array(keys(users), 1))
			{
				str += sprintf("%4d    %-10s %10d\n", i++, name, sizeof(users[name]["msg_id"]));
			}
		}
		
		str += MESSAGE_LINE;

		me->start_more(str);
	}
	else
	{
		return help(me);
	}

	return 0;
}

// 清除用户的所有短消息
int do_clear(object me, string arg)
{
	if (!wizardp(me) )
	{
		return help(me);
	}

	if (wiz_level(me) < wiz_level(arg))
	{
		write_result("您没有删除这个用户短消息的权限。");
		return 1;
	}

	delete_user_all(me, arg, 1);

	save();

	write_result("清除了用户" + capitalize(arg) + "的所有短消息。");

	return 0;
}

// 设置短消息系统参数
int do_set(object me, string arg)
{
	int value;

	if ( !wizardp(me) )
	{
		return help(me);
	}

	if ( arg == "all" )
	{
		write_result("设置值\n\n" + show_sets(me));

		return 1;
	}

	if ( !arg || sscanf(arg, "%s %d", arg, value) != 2 )
	{
		return help(me);
	}

	if ( (string)SECURITY_D->get_status(me) < "(arch)" 
		&& wiz_level(me) < sets["cmd_leve"] )
	{
		write_result("您没有短消息设定权限。");
		return 1;
	}

	if ( member_array(arg, keys(sets)) < 0 )
	{
		write_result("没有此设置项。");
		return 1;
	}

	if (value < MIN_VALUE || value > MAX_VALUE)
	{
		write_result("您只能设置为" + MIN_VALUE + "到" + MAX_VALUE + "内的整数。");
	}

	sets[arg] = value;

	save();

	write_result("您把" + arg + "设置为" + value + "。");

	return 0;
}

void create()
{
	seteuid(getuid());

	load_message();

	call_out("create", 900);
}

int main(object me, string arg)
{
	string type;

	if (load_message())
	{
		return 1;
	}

	if ( (string)SECURITY_D->get_status(me) != "(admin)" 
		&& wiz_level(me) < sets["cmd_level"] )
	{
		write_result("已经关闭。");
		return 1;
	}

	if( !arg )
	{
		do_read(me, "all", 0);
		return 1;
	}

	if ( sscanf(arg, "%s %s", type, arg) != 2 )
	{
		return help(me);
	}

	switch (type)
	{
	case "send":
		do_send(me, arg);
		break;
	case "read":
		do_read(me, arg);
		break;
	case "discard":
		do_discard(me, arg);
		break;
	case "lock":
		do_lock(me, arg, 1);
		break;
	case "unlock":
		do_lock(me, arg, 0);
		break;
	case "show":
		do_show(me, arg);
		break;
	case "clear":
		do_clear(me, arg);		// 不建议使用。
		break;
	case "set":
		do_set(me, arg);
		break;
	default:
		return help(me);
		break;
	}

	return 1;
}

int help(object me)
{
	string help = @HELP

指令说明 : 
	这个指令可以发送或管理你的短消息。
	发送的短消息即使对象玩家离线也可以收到。

指令格式 : 
	msg <send [id1[:id2[...]]]>            发送短消息到一个或多个玩家。
	msg <read [all | new | next |编号]>    读取短消息。
	msg <discard [all | 编号]>             删除一个或多个短消息。
	msg <lock/unlock> <编号>               锁定或解除锁定指定短消息，如要删除
	                                       锁定短消息须先解除锁定。
		
	其中 msg read all 中高亮显示的为新消息，编号前带红色♀符号的为被锁定短消息。
HELP;

	if ( wizardp(me) )
	{
		help +=@HELP

	msg set <all | Settings <Value> >      查看或更改短消息设置。

	Settings 可以使用 msg set all 查看。
HELP;

		help += "\n" + show_sets(me);
		help += "\n	Value 可以取值" + MIN_VALUE + "～" + MAX_VALUE + "。";
	}

	help += "\n
	Copyright 1997-2003 ShuJian MUD Wizard Group.
	All rights reserved.
	
	Visit our Homepage at - www.mymud.com \n";

	write(help);

	return 1;
}

