// call.c
// Modified by snowman@SJ 28/11/2000
// call xxx->query 的形式不加log

#include <ansi.h>
inherit F_CLEAN_UP;

int main(object me, string arg)
{
	string objname, func, param, euid, reason;
	object obj;
	mixed *args, result;
	int i;

	if( arg ) {
		if( sscanf(arg, "-%s %s", euid, arg)==2 ) {
			if( (string)SECURITY_D->get_status(me) != "(admin)" )
				return notify_fail("你不能设定自己的 euid。\n");
			seteuid(euid);
		}
		else
			seteuid( geteuid(this_player(1)) );

		if (sscanf(arg, "%s->%s(%s) because %s", objname, func, param, reason) != 4) {
			arg += NOR;	// Yuj@SJ 20000204, fixed call xxx->yyy("(zzz)") 's bug.
			if( sscanf(arg, "%s->%s(%s)" NOR, objname, func, param)!=3 )
				return notify_fail("指令格式：call <物件>-><函数>( <参数>, ... )\n");
		}
	} else
		return notify_fail("指令格式：call <物件>-><函数>( <参数>, ... )\n");

	obj = present(objname, environment(me));
	if(!obj) obj = present(objname, me);
	if(!obj) obj = LOGIN_D->find_body(objname);
	if(!obj || !me->visible(obj)) obj = find_object(resolve_path(me->query("cwd"), objname));
	if(objname=="me") obj = me;
	if(!obj) return notify_fail("找不到指定的物件。\n");

	if( (string)SECURITY_D->get_status(me) == "(apprentice)" && obj!=me )
		return notify_fail("你没有直接呼叫这个物件的函数的权力。\n");

	if( wiz_level(me) < wiz_level(obj) )
		return notify_fail("你没有直接呼叫这个物件的函数的权力。\n");

	if (userp(obj) && geteuid(me) != "yuj") {
		if( obj->query("no_call") )
			return notify_fail("这个物件被禁止了被call的权利。\n");

		if (obj != me
		&& strsrch(func, "query") < 0) {
			if (!reason)
				return notify_fail("指令格式：call <物件>-><函数>( <参数>, ... ) because 原因\n");
			log_file("static/CALL_PLAYER",
				sprintf("%s(%s) call %s(%s)->%s(%s) on %s because %s\n",
				me->name(1), geteuid(me), obj->name(1), geteuid(obj), func, param,
				ctime(time()), reason)
			);
		}
	} else if (strsrch(func, "query") < 0 && !master()->valid_write( base_name(obj), me, "set" ))
		return notify_fail("你没有直接呼叫这个物件的函数的权力。\n");

	args = explode(param, ",");

	tell_object(me, sprintf("%O->%s(%s) = ", obj, func, implode(args, ", ")));

	for(i=0; i<sizeof(args); i++) {
		// This removes preceeding blanks and trailing blanks.
		parse_command(args[i], environment(me), "%s", args[i]);
		if( sscanf(args[i], "%d", args[i]) ) continue;
		if( sscanf(args[i], "\"%s\"", args[i]) ) continue;
		//args[i] = restore_variable(args[i]);
	}

	args = ({ func }) + args;

	result = call_other(obj, args);
//	for(i=1; i<sizeof(args); i++)
//		args[i] = sprintf("%O",args[i]);
	tell_object(me, sprintf("%O\n", result));
	return 1;
}

int help(object me)
{
write(@HELP
指令格式 : call <物件>-><函数>(<参数>, ...... )
 
呼叫<物件>里的<函数>并传入相关<参数>.
HELP
    );
    return 1;
}
