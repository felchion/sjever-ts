#include <ansi.h>

int main(object me,string arg)
{
	printf(REF);
	me->set_temp("boss_behind",1);
	input_to("get_id",0,me);
	tell_room(environment(me),me->name()+"惊呼道：老板来啦！！各位等会儿见。\n",({me}));
	return 1;
}

void get_id(string id,object me)
{
	if(!me)
		return;

	if(id != me->query("id"))
	{
		printf(REF);
		input_to("get_id",0,me);
	}
	else
		input_to("get_return",0,me);
	return;
}

void get_return(string ret,object me)
{
	if(!me)
		return;

	if(ret != "")
	{
		printf(REF);
		input_to("get_return",0,me);
	}
	else
	{
		message_vision("$N长嘘一口气道：老板总算走了。\n",me);
		me->delete_temp("boss_behind");
	}
	return;
}

int help(object me)
{
write("这个紧急指令用于单位老板或同事突然来到你身边，来不及作其他处理的时候使用。
输入指令后屏幕立刻会被清空，看不到任何游戏里的讯息。输入任何指令都会被清屏。
"HIM"只有在正确的输入了你的 ID 后再按一次回车键才会恢复正常。
"HIY"切记此指令恢复的方法，如果忘记恢复方法，断线重新连接即可。\n"NOR);
	return 1;
}