#include <ansi.h>

void create()
{
	seteuid(getuid());
}

int main(object me, string arg)
{
	object ob;
	int iyes = 0;
	string msg,dest;//int m;
	
	if (me->is_busy())
		return notify_fail("您先歇口气再说话吧。\n");

	if (!arg || !objectp(ob = present(arg, environment(me))))
		return notify_fail("你想要和谁交谈。\n");

	if (ob == me) return notify_fail("问自己？\n");
	if (!ob->is_character()) {
		message_vision("$N对着$n自言自语……\n", me, ob);
		return 1;
	}
	
	if (ob->query("mute"))
		message_vision("但是很可惜，$N不会说话。\n", ob);

	if (!ob->query("can_speak")) {
		message_vision("$N想和$n交谈，但是$p显然听不懂人话。\n", me, ob);
		return 1;
	}

	if (!living(ob)) {
		message_vision("但是很显然的，$n现在的状况没有办法给$N任何回应。\n", me, ob);
		return 1;
	}

	dest = base_name(environment(ob));

	if ((msg = ob->query("startroom"))
	&& msg != dest
	&& (msg = ob->query("lastroom"))
	&& (msg == dest || msg->is_character()))
	message_vision(CYN "$N说道："+RANK_D->query_self(ob)+"是出来游山玩水的，暂时没空聊天。\n" NOR, ob);	
	
  me->add_busy(1);
 
  TASK_D->talk_event(me,ob);
   
  return 1;
}

int help(object me)
{
	write( @HELP
指令格式: talk <someone>

这个指令在游戏进行中很重要, 通常必须藉由此一指令才能
获得进一步的资讯。
HELP
	);
	return 1;
}
