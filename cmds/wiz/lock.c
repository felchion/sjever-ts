// by darken@SJ

#include <ansi.h>
#include <login.h>
#define SYNTAX "指令格式：lock <某人> because <原因>\n"
#define BLOCK "/d/death/block"

inherit F_CLEAN_UP;

int main(object me, string str)
{
	object ob, where;
	string name, reason;
 
	if(!str || sscanf(str, "%s because %s", name, reason)!=2 )
		  return notify_fail(SYNTAX);
       
	if (!objectp(ob = LOGIN_D->find_body(name))) 
		  return notify_fail("没有这个人吧?\n");

	if (wiz_level(ob))
		 return notify_fail("抓巫师？你以为你是darken啊？\n");

	if(!objectp(where = environment(ob)))
		  return notify_fail("这个人不知道躲在哪里耶... :-( \n");
	
	if (ob->query("registered") < 2)
		return notify_fail("这个人不是正式注册的玩家。\n");

	if (file_name(where) == BLOCK)
		  return notify_fail(ob->query("name") + "已经在死刑房思过了，不用再抓了。\n");

	if (base_name(ob) == "/cmds/leitai/leitaiuser")
		return notify_fail(ob->name() + "这人是比武擂台Clone出来的，在擂台内让他离开就是了。\n");

	tell_room(where, WHT HBMAG"突然出现了两个刽子手，把"+ob->query("name")+"架到死刑房去了。\n"NOR, ob);
	tell_object(ob, MAG"两个刽子手把你推进了死刑房?...\n"NOR);

	message("shout", YEL HBRED "\n【纪律】"+ob->query("name")+"("+capitalize(ob->query("id"))
		+")因为［" + reason + "］，违反书剑游戏规则，决定给予关进死刑房的处罚。\n\n" NOR, users());

	log_file("static/LOCK", sprintf("[%s] %s(%s) 因为 [%s] 被 %s(%s) 抓到死刑房。\n",
		ctime(time())[0..15], ob->query("name"),ob->query("id"), reason, (string)me->query("name"),me->query("id")));
	ob->delete("enter_wuguan");
	ob->move(BLOCK);
	return 1;
}
