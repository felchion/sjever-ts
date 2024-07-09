//这个指令几百年没人动了 LinuX@SJ

inherit F_CLEAN_UP;

#include <ansi.h>

int main(object me, string arg)
{
	object ob;
    	string msg;

	if (me->query("marry"))
		return notify_fail("你已经结婚了。\n");

	if (me->query_temp("marry"))
		return notify_fail("你已经订婚了。\n");

	if (me->query_temp("pending/marry"))
		return notify_fail("你已经在求婚了。\n");

	if (me->query("gender") == "女性")
		return notify_fail("求婚只能由男方发起。\n");

	if (!arg || !(ob = present(arg, environment(me))) || me == ob)
		return notify_fail("你要向谁求婚？\n");

	if (!userp(ob))
		return notify_fail("你只能向玩家求婚。\n");

	if (me->query("age") < 18)
		return notify_fail("你想向"+ob->name()+"求婚，不过你还没有成年。\n");

	if (ob->query("age") < 18)
		return notify_fail(ob->name()+"不过的年纪也小了一点，等长成大姑娘，你再求婚也不迟啊。\n");

	/*if (ob->query("registered") < 3)
		return notify_fail("你只能向正式玩家求婚。\n");*/
		

	if (ob->query("gender") != "女性")
		return notify_fail("你求婚的对方不是女性。\n");
	
	if ( stringp( msg = me->query( "env/qiuhun_msg" ) ) ) 
            {
                message_vision( msg+"\n", me, ob );           }	
	else
	message_vision( CYN+"$N勇敢的向$n跪下来，深情的注视着$n的双眸，说道：\n" +
                    "你是我今生唯一爱的、爱过的、会爱的人，" + ob->name() + "嫁给我吧，" +
                    "我们将是江湖最令人艳羡的一对的！\n"+NOR, me, ob );
 	tell_object( ob, YEL + me->name() + "向你求婚，你如果愿意和就点点头, 如果不同意就摇摇头，如果你太为难就不理他了，\n" + 
                "如果你想考验他的诚意，就提出个要求看看他能不能做到。\n" );                                   
	tell_object(ob, YEL "你如果愿意和"+me->name()+"结为连理，请用 (marry) 指令。\n\n" NOR);
	me->set_temp("pending/marry", ob);
	return 1;
}

int help(object me)
{
	write(@HELP
指令格式: qiu <玩家的 ID>

如果你是男性，这个指令是用来向和你在同一个房间的某个玩家求婚。
如果你是女性，这个指令可以看到现在有谁向你求婚。

相关的指令：marry
相关的帮助：help marriage
HELP
	);
	return 1;
}

