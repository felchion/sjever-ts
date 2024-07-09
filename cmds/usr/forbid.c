// Forbid.c 封停帐号
// 禁止玩家按照要求时间登陆。


#include <ansi.h>
#include <login.h>

#define FORBID_FILE       "/data/forbidlist"

inherit ITEM;
inherit F_SAVE;
mapping *notes;

int help(object me);
string query_save_file();
void list_forbid(object me, int num);

int is_forbid(string arg)
{
	foreach (mapping item in notes)
		if (arg == item["playerid"]) return 1;
	return 0;
}

string delete_forbid(string arg)
{
	foreach (mapping item in notes)
	{
		if (arg == item["playerid"])
		{
			object ob;
			if( file_size(DATA_DIR + "user/" + arg[0..0] 
				+ "/" + arg + SAVE_EXTENSION)<0 )
			{
				return 0;
			}
			
			ob = new(USER_OB);
			
			ob->set_name("", ({ arg }));
			
			seteuid(arg);
			export_uid(ob);
			seteuid(getuid());
			
			if (!ob->restore())
			{
				destruct(ob);
				return 0;
			}
			
			ob->set_name(ob->query("name"), ({ arg }));

			ob->delete("forbid_time");

			ob->set_temp("db_quit", 1);

			if (!ob->save())
			{
				destruct(ob);
				return 0;
			}

			destruct(ob);
			
			notes -= ({ item });
			save();
			
			return item["player"];
		}
	}
	
	return 0;
}

private int delete_forbid_name(object me, string arg)
{
	string name = delete_forbid(arg);

	if (name)
	{
		tell_object(me, "你把" + name + "的帐号解封。\n");
	}
	else
	{
		tell_object(me, name + "的帐号没有封停。\n");
	}

	return 1;
}

void create()
{
	seteuid(getuid());
	restore();
}

int main(object me, string arg)
{
	mapping note,note2;
	string s,arg1,arg2;
	object ob;
	int day;
	
	if (!arg)
	{
		list_forbid(me, 10);
		return 1;
	}
	
	if (sscanf(arg, "%s", s) == 1 && s == "all" )
	{
		list_forbid(me, 0);
		return 1;
	}

	if (wiz_level(me)<4)
	{
		return notify_fail("只有游戏管理员才可以封停帐号。 \n");
	}
		
	if (sscanf(arg, "-d %s", s) == 1 )
	{
		delete_forbid_name(me, s);
		return 1;
	}

	if (sscanf(arg, "%s %d because %s", arg1, day, arg2) != 3)
	{
		return help(me);
	}

	if (day < 0)
	{
		return notify_fail("帐号禁闭时间必须大于零，按照天计算。\n");
	}

	if (is_forbid(arg1))
	{
		return notify_fail("玩家的帐号已经封停，如果已经解封，请上删除该条记录再重新封停。\n");
	}
		
	if (strwidth(arg2)< 8 || strwidth(arg2)>44)
	{
		return notify_fail("请简明扼要地说明原因，字数必须在4-20之间。 \n");
	}
	
	if (objectp(ob = LOGIN_D->find_body(arg1)))
	{
		"/cmds/wiz/kickout"->main(me, arg1);
	}

	if (!objectp(ob = LOGIN_D->find_body(arg1)))
	{
		if( file_size(DATA_DIR + "user/" + arg1[0..0] 
			+ "/" + arg1 + SAVE_EXTENSION)<0 )
		{
			return notify_fail("没有这位使用者。\n");
		}
		
		ob = new(USER_OB);
		
		ob->set_name("", ({ arg1 }));

		seteuid(arg1);
		export_uid(ob);
		seteuid(getuid());
		
		if (!ob->restore())
		{
			destruct(ob);
			return notify_fail("没有这个玩家。\n");
		}

		ob->set_name(ob->query("name"), ({ arg1 }));
	}

	//if (!(int)ob->query("forbid_time"))
	{			
		int forbid_time = time() + day * 24 * 60 * 60; 
		
		if (ob->set("forbid_time", forbid_time))
		{
			tell_object(me,"玩家封闭帐号时间设置成功！\n");
		}
		else
		{
			return notify_fail("玩家封闭帐号时间设置失败，封闭帐号无效！请与高级巫师联系。\n");
		}
		ob->set_temp("db_quit", 1);
		if (ob->save())
		{
			tell_object(me,"玩家封闭帐号时间保存成功！\n");
		}
		else
		{
			return notify_fail("玩家封闭帐号时间保存失败，封闭帐号无效！请与高级巫师联系。\n");
		}
		
		note = allocate_mapping(7);
		note["time"] 	= time();
		note["block"] 	= me->query("id");
		note["player"] 	= ob->query("name") + "(" + capitalize(ob->query("id")) + ")";
		note["playerid"] = ob->query("id");
		note["reason"] 	= arg2;
		note["day"]	= day;
		note["utime"] 	= forbid_time;

		message("channel", HIC "\n【纪律】"+ob->query("name")
			+"("+capitalize(ob->query("id"))
			+")因为［" + arg2 + "］，违反书剑游戏规则，决定给予封停帐号"
			+ CHINESE_D->chinese_number(day) + "天的处罚。\n\n" NOR, users()
			);
			//处罚公开，POST专门留言板
		
	        note2 = (["title":"停封玩家"+ob->query("name")+"("+capitalize(ob->query("id"))+")帐号的处理公告",
	        	"author":me->query("name") + "(" + getuid(me) + ")",
	        	"time":0,
	        	"msg":0,]);
		"/clone/board/wizto_b"->done_post(this_object(),note2,"【纪律】"+ob->query("name")
			+"("+capitalize(ob->query("id"))
			+")因为［" + arg2 + "］，违反书剑游戏规则，决定给予封停帐号"
			+ CHINESE_D->chinese_number(day) + "天的处罚。\n");
		/*
		"/clone/board/wizto_b"->auto_post("停封玩家"+ob->query("name")
		+"("+capitalize(ob->query("id"))
		+")帐号的处理公告","【纪律】"+ob->query("name")
			+"("+capitalize(ob->query("id"))
			+")因为［" + arg2 + "］，违反书剑游戏规则，决定给予封停帐号"
			+ CHINESE_D->chinese_number(day) + "天的处罚。\n");	
		*/
		//save 失败 无须log
		log_file( "static/FORBID", me->query("name")+"("+me->query("id")+") 给 " +
			arg1 + day + " 天封停帐号的惩罚, 因为:" + arg2 + "\n" );
	}
	
	destruct(ob);

	if( !pointerp(notes) || !sizeof(notes) )
	{
		notes = ({ note });
	}
	else
	{
		notes += ({ note });
	}
	
	save();

	tell_object(me, "你成功地把" + note["player"] + "列入封停帐号名单。\n");
	return 1;
	
}


string query(string arg)
{
        if(arg=="id") return "Wizard";
        return ::query(arg);
}

string query_save_file()
{
	return FORBID_FILE ;
}

void list_forbid(object me, int num)
{
	int i;
	string str;
	
	i = sizeof(notes);

	if( i < 1 )
	{
		write("书剑封停帐号名单目前没有任何内容。\n");
		return;
	}
	
	str="书剑封停帐号名单：\n";
	str += repeat_string("┉", 24) + "\n";

	while (i-- && (i>=sizeof(notes)-num || num==0))
	{
		str += sprintf("编号：%d\n玩家：%s\n原因："HIG"%s"NOR
			"\n时间：共 %s 天。%s。\n状态：%s\n时间：%s\n",
			i+1,
			notes[i]["player"],
			notes[i]["reason"],
			CHINESE_D->chinese_number(notes[i]["day"]),
			wizardp(me)?("处理巫师:"+notes[i]["block"]):"",				
			( notes[i]["utime"]-time() > 0 )?("还剩" + 
			CHINESE_D->chinese_time(notes[i]["utime"]-time())):"已到期",
			CHINESE_D->chinese_date(notes[i]["time"],1)
			);

		str += repeat_string("┉", 24) + "\n";
	}
	
	me->start_more( str );
}

int help(object me)
{
	if (!wiz_level(me) )
        write(@HELP
指令格式 :
forbid [all]                 查看封停帐号名单的内容和处理结果

巫师将被怀疑违反规则的玩家登记入封停帐号，在封停帐号期间，该玩家帐
号将被暂时封闭，不能联线游戏。

HELP);
    else 
        write(@HELP
指令格式 :
forbid [all]                       查看黑名单的内容和处理结果
forbid <id> <day> because <原因>   将玩家列入黑名单，等候调查处理
forbid -d <id>                     删除指定编号的黑名单

巫师将被怀疑违反规则的玩家登记入封停帐号，在封停帐号期间，该玩家帐
号将被暂时封闭，不能联线游戏。
HELP);
        return 1;
}
