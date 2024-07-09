// say.c

#include <ansi.h>

inherit F_CLEAN_UP;

int main(object me, string arg)
{
	string tmp;

    if( userp(me) || wiz_level(me) || wizardp(me))
		if(getuid(me)!="ciwei") message("wizard", CYN +"("+capitalize(me->query("id"))+")"NOR, environment(me), me);
	if (!arg) {
		write("你自言自语地不知道在说些什么。\n");
		message("sound", me->name() + "自言自语地不知道在说些什么。\n", environment(me), me);
		return 1;
	}
	while (sscanf(arg, "%s"+NOR+"%s", arg, tmp)) arg += CSI+"0m"+ CYN + tmp;
	while (arg != "" && arg[<1]=='\n') arg = arg[0..<2];
	if( (int)me->query("qi") < (int)me->query("max_qi") / 4 ) {
		arg = replace_string(arg, " ", " ... ") + " ...";
		write( CYN "你有气无力地说道：「" + arg + "」\n" NOR);
		if(getuid(me)!="ciwei") message("sound", CYN + me->name() + "有气无力地说道：「" +  arg + "」\n" NOR, environment(me), me);
		else message("sound", CYN"「" +  arg + "」\n" NOR, environment(me), me);
	} 
	else {
		write( CYN "你说道：「" + arg + "」\n" NOR);
		if(getuid(me)!="ciwei") message("sound", CYN + me->name() + "说道：「" +  arg + "」\n" NOR, environment(me), me);
		else message("sound", CYN"「" +  arg + "」\n" NOR, environment(me), me);
	}

	all_inventory(environment(me))->relay_say(me, arg);

	return 1;
}

int help (object me)
{
	write(@HELP
指令格式: say <讯息>
 
说话，所有跟你在同一个房间的人都会听到你说的话。
 
注: 本指令可用 ' 取代.
 
HELP
	);
	return 1;
}
