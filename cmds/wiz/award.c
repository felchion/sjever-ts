
inherit F_CLEAN_UP;

#include <ansi.h>
#include <login.h>
#include <mudlib.h>
#include <database.h>

#define SYNTAX  "指令格式：award <某人> because <原因> <积分>\n"

int main(object me, string str)
{
	object ob, where;
	int point;
	string name, reason;
	mapping note;

	if(!str || sscanf(str, "%s because %s %d", name, reason, point)!=3 )
		  return notify_fail(SYNTAX);

	if (!objectp(ob = LOGIN_D->find_body(name)))
		  return notify_fail("这个...这个... 有这个人吗?\n");;

	if (!objectp(where = environment(ob)))
		  return notify_fail("这个人不知道在哪里耶... :-( \n");
		  
	if ( (point < 10) || (point > 50))
		  return notify_fail("奖励积分最少10点，最多50点。\n");

	if (base_name(ob) == "/cmds/leitai/leitaiuser")
		return notify_fail(ob->name() + "这人是比武擂台Clone出来的，等下在奖励吧。\n");

	tell_room(where, MAG"天空中飘落一个美貌仙女，拍了拍"+ob->query("name")+MAG"的头。\n"NOR, ob);

	tell_object(ob, "您因为［" + reason + "］，管理人员给予奖励书剑积分" + CHINESE_D->chinese_number(point) + "点。\n"
	);
 	
 	if (!dbquery("update Users set U_Credit = U_Credit + " + point + " where U_Username = \"" + name+ "\""))
		write("玩家"+ob->query("name")+"("+capitalize(ob->query("id"))+")积分奖励设置失败。\n");
	else
		write("玩家"+ob->query("name")+"("+capitalize(ob->query("id"))+")积分奖励设置成功。\n");
	
	message("channel", HIG "\n【奖励】"+ob->query("name")
		+"("+capitalize(ob->query("id"))
		+")因为［" + reason + "］，决定给予奖励书剑积分" 
		+ CHINESE_D->chinese_number(point) + "点。\n\n" NOR, users()
	);
	//处罚公开，POST专门留言板	
	note = (["title":"奖励玩家"+ob->query("name")+"("+capitalize(ob->query("id"))+")公告",
		"author":me->query("name") + "(" + getuid(me) + ")",
		"time":0,
		"msg":0,]);
	"/clone/board/wizto_b"->done_post(this_object(),note,"【奖励】"+ob->query("name")
		+"("+capitalize(ob->query("id"))
		+")因为［" + reason + "］，决定给予奖励书剑积分" 
		+ CHINESE_D->chinese_number(point) + "点。\n");
		
	BBS_D->add_Bbs_Up_Map(WEB_DB_NAME,"UPDATE members SET credit_total = credit_total+'"+point+"' WHERE username = '"+getuid(ob)+"@"+lower_case(INTERMUD_MUD_NAME)+"' limit 1",0,0);
	///credit_total 	
	/*		
	"/clone/board/wizto_b"->auto_post("奖励玩家"+ob->query("name")
		+"("+capitalize(ob->query("id"))
		+")公告","【奖励】"+ob->query("name")
		+"("+capitalize(ob->query("id"))
		+")因为［" + reason + "］，决定给予奖励书剑积分" 
		+ CHINESE_D->chinese_number(point) + "点。\n");
	*/	

	log_file("static/AWARD",
		sprintf("%s 因为 [%s] 被 %s(%s) 奖励书剑积分 %d 点。\n",
			name, reason, (string)me->query("name"),me->query("id"), point
		), ob, ({ me })
	);

	return 1;
}

string query(string arg)
{
	if(arg=="id") //return now_id;
	return "Wizard";
}

int help(object me)
{
	write(@HELP
指令格式：award <某人> because <原因> <积分>
你可以用这个指令奖励一些给游戏做出贡献的玩家。
HELP
	);
	return 1;
}
