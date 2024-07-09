// tell.c
// Modified by Numa 2000.1.27 For Can tell whom added you in his friend_user.
// modified by snowman@SJ 25/11/2000
// faster friend_user check.

inherit F_CLEAN_UP;

#include <ansi.h>
#include <mudlib.h>
#include <net/dns.h>

int help(object me);
int is_sub(string s_str, string m_str);
void create() {seteuid(getuid());}

int main(object me, string arg)
{
	string target, msg, mud;
	object obj;
	string no_tell,can_tell;
	string my_id;

	if( !arg || sscanf(arg, "%s %s", target, msg)!=2 ) return help(me);

	if( sscanf(target, "%s@%s", target, mud)==2 ) {
		if (me->query("combat_exp") < 500000)
			return notify_fail("你的资历还不够，不能使用千里传音。\n");
		GTELL->send_gtell(mud, target, me, msg);
		write("网路讯息已送出，可能要稍候才能得到回应。\n");
		return 1;
	}

	obj = LOGIN_D->find_body(target);
	if (!obj || !me->visible(obj))
		return notify_fail("没有这个人。\n");

	if( obj == me)
		return notify_fail("告诉自己？\n");

	if( !interactive(obj))
		return notify_fail("这个人断线了。\n");

	//if (!wizardp(me) && userp(me) && me->query("id") == obj->query("env/block"))
	//	return notify_fail(obj->query("name")+"不想听你瞎掰！\n");
	
	my_id = me->query("id");
	no_tell = obj->query("env/block");
	if (! wizardp(me) && (no_tell == "all" || no_tell == "ALL" ||
	    is_sub(my_id, no_tell))){
	    	can_tell = obj->query("env/tell");
                if (! is_sub(my_id, can_tell))
		       	return notify_fail(obj->query("name")+"不想听你瞎掰！\n");
	}

	if (!wizardp(me) && (obj->query("env/tell") == me->query("id")))
		;
	else if (!wizardp(me) && userp(me) && obj->query("env/block")=="YES") {
		// Add Friend_user tell From Here.
		string *friend_list = obj->query("friend_user");

		if (!arrayp(friend_list)) {
			obj->delete("friend_user");
			friend_list = ({ });
		}
		if (member_array(me->query("id"), friend_list) == -1)
			return notify_fail(obj->query("name")+"很忙，有什么事待会再说吧！\n");
	}

	write(GRN "你告诉" + obj->query("name") + "：" + msg + "\n" NOR);
	message("channel", sprintf( GRN "%s告诉你：%s\n" NOR,
		obj->visible(me)?(me->query("name")+"("+capitalize(me->query("id"))+")"):me->name(), msg), obj);

        //增加发呆返回！
        if (query_idle(obj) >= 120)
                write(HIM "可是" + obj->name(1) +
                      HIM "已经发呆有" + chinese_number(query_idle(obj) / 60) +
                      "分钟了，恐怕不能立刻回答你。\n"NOR);

	obj->set_temp("reply", me);
	return 1;
}

string remote_tell(string cname, string from, string mud, string to, string msg)
{
	object ob;
	string no_tell;
	
	from = lower_case(from);
	to = lower_case(to);
	if (!(ob = LOGIN_D->find_body(to))
	|| ob->query("env/tell") != from
	&& wizardp(ob)
	&& ob->query("env/invisibility")
	&& wiz_level(from) < wiz_level(ob))
		return "千里传音@"+INTERMUD_MUD_NAME+"告诉你：没有 "+to+" 这个玩家。";

//	if (from == ob->query("env/block")
//	&& wiz_level(from) < wiz_level(ob))
//		return "千里传音@"+INTERMUD_MUD_NAME+"告诉你："+ob->query("name")+"不想听你瞎掰！";
	
	no_tell = ob->query("env/block");
	if ( no_tell == "all" || no_tell == "ALL" ||
	    is_sub(from, no_tell))
		return "千里传音@"+INTERMUD_MUD_NAME+"告诉你："+ob->query("name")+"不想听你瞎掰！";

	if (ob->query("env/tell") != from
	&& wiz_level(from) < wiz_level(ob)
	&& ob->query("env/block") == "YES") {
		string *friend_list = ob->query("friend_user");

		if (!arrayp(friend_list)) {
			ob->delete("friend_user");
			friend_list = ({ });
		}
		if (member_array(from, friend_list) == -1)
			return "千里传音@"+INTERMUD_MUD_NAME+"告诉你："+ob->query("name")+"很忙，有什么事待会再说吧！";
	}

	if (!interactive(ob))
		return "千里传音@"+INTERMUD_MUD_NAME+"告诉你："+"这个人断线了。";

	if (cname)
		message("channel", sprintf(GRN "%s(%s@%s)告诉你：%s\n" NOR,
			cname, capitalize(from), mud, msg), ob);
	else
		message("channel", sprintf(GRN "%s@%s 告诉你：%s\n" NOR,
			capitalize(from), mud, msg), ob);
	ob->set_temp("reply", from + "@" + mud);
	return sprintf("你告诉%s(%s@%s)：%s", ob->query("name"), capitalize(to), INTERMUD_MUD_NAME, msg);
}

int is_sub(string s_str, string m_str)
{
        if (! m_str || ! s_str) return 0;
        return strsrch("," + m_str + ",", "," + s_str + ",") != -1;
}

int help(object me)
{
	write(@HELP
指令格式：tell <某人> <讯息>

你可以用这个指令和其他地方的使用者说话。
相关环境变量设置：
不准任何人告诉你：set block all|ALL
不准某个人告诉你：set block 此人 id
不准某批人告诉你：set block id1,id2,id3.....
允许某个人告诉你：set tell 此人 id
允许某批人告诉你：set tell id1,id2,id3......

其他相关指令：reply
HELP
);
	return 1;
}
