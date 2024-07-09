// Write by look
// Modify by bbb
// Modify by snowman@SJ 17/02/2000
// check leitaiuser.

#include <ansi.h>
#include <login.h>
#define SYNTAX  "指令格式：catch <某人> because <原因>\n"

inherit F_CLEAN_UP;

int main(object me, string str)
{
	object ob, where;
	int times, t;
	string name, reason;
 
	if(!str || sscanf(str, "%s because %s", name, reason)!=2 )
		  return notify_fail(SYNTAX);
	       
	if (!objectp(ob = LOGIN_D->find_body(name))) 
		  return notify_fail("这个...这个... 有这个人吗?\n");
	
/*
	if (wiz_level(ob))
		  return notify_fail("对方是共同奋斗的战友噢，你这样做不太好吧？\n");
*/

	if(!objectp(where = environment(ob)))
		  return notify_fail("这个人不知道躲在哪里耶... :-( \n");
	
	if (ob->query("registered") < 2)
		return notify_fail("这个人不是正式注册的玩家。\n");

	if (file_name(where) == RELAX_ROOM) 
		  return notify_fail("他已经在桃花源思过了，不用再抓了。\n");
	
	if (base_name(ob) == "/cmds/leitai/leitaiuser")
		return notify_fail(ob->name() + "这人是比武擂台Clone出来的，在擂台内让他离开就是了。\n");
		
	tell_room(where, MAG"天空中飘落两位美貌仙女，扶起"+ob->query("name")+MAG"腾空而去了。\n"NOR, ({ ob }));
	tell_object(ob, MAG"两位美貌仙女出现在你眼前，扶着你缓缓飘了起来....\n"NOR);
	
	times = ob->add("rules", 1);
	if (times < 6) t = 6;
	else t = times;
	tell_object(ob,"您因为［" + reason + "］，违反书剑游戏规则共计" + CHINESE_D->chinese_number(times) + "次，
	      决定给予关禁闭于桃花源" + CHINESE_D->chinese_number(t*10) + "分钟的处罚。\n");

	message("shout", HIC "\n【纪律】"+ob->query("name")
		+"("+capitalize(ob->query("id"))
		+")因为［" + reason + "］，违反书剑游戏规则共计" + CHINESE_D->chinese_number(times)+ "次，决定给予关禁闭"
		+CHINESE_D->chinese_number(t*10)+"分钟的处罚。\n\n" NOR, users());

/*
	if (times > 3){
		int t = times > 50? 50: times;
		tell_object(ob,"同时减去你经验值的百分之"+CHINESE_D->chinese_number(t)+"作为处罚。\n\n");
		message("channel:chat", HIC "【纪律】同时减去经验值的百分之"+CHINESE_D->chinese_number(t)+"作为处罚。\n\n" NOR, users());
		ob->add("combat_exp",-ob->query("combat_exp")/100*t);
	}
*/
	
	ob->apply_condition("relax", t*30);
	ob->move("/d/wizard/relax");
	ob->set("relax_timeout_room", file_name(where));
	
	tell_object(me, "你把"+(string)ob->query("name")+"抓到了桃花源。\n");
	tell_room(environment(ob), MAG"两位仙女扶着"+ob->name()+MAG"飘来，将"+ob->name()+MAG"轻轻放在这里。\n"NOR, ({ ob }));

	ob->set("startroom", RELAX_ROOM);

	log_file("static/CATCH", sprintf("[%s] %s 因为 [%s] 被 %s(%s) 抓到桃花源。共计违规 %d 次。\n",
		ctime(time())[0..15], name, reason, (string)me->query("name"),me->query("id"), times));
	    
	
	return 1;
}

int help(object me)
{
write(@HELP
指令格式 : catch <某人> because <原因>

此指令可让你将某个违反规则的玩家移送到桃花源。
禁闭时间到后自动释放。
犯规次数自动累加。
HELP
    );
    return 1;
}

