// clone.c

#include "/doc/help.h"

inherit F_CLEAN_UP;

int main(object me, string file)
{
	object obj;
	string err, msg;

	seteuid( geteuid(me) );

	if (!file) file = me->query("cwf");
	if (!file)
		return notify_fail("你要复制什么物件？\n");

	file = resolve_path(me->query("cwd"), file);
	if( sscanf(file, "%*s.c") != 1 ) file += ".c";
	me->set("cwf", file);

	if( file_size(file) < 0 )
		return notify_fail("没有这个档案(" + file + ")。\n");

	if( !find_object(file) ) {
		err = catch(call_other(file, "???"));
		if (err) {
			write("载入失败：" + err + "\n");
			return 1;
		}
	}

    err = catch(obj = new(file));
	if (err) {
		write("复制失败：" + err + "\n");
		return 1;
	}

    obj->set("clone_by_wizard", 1);
    obj->set("clone_by_which_wizard", geteuid(me));
	obj->delete("treasure");
	if( !stringp(msg = me->query("env/msg_clone")) )
		msg = "只见$N伸手凌空一指，变出了$n。";

	if( !obj->is_character() && obj->move(me,1) && !obj->query("no_carry")) {
		write(obj->query("name") + "复制成功，放在你的物品栏。\n");
		message_vision(msg + "\n", me, obj);
		// log it
		if (geteuid(me) != "yuj")
		log_file("static/CLONE_OBJ",sprintf("%s(%s) clone %s(%s) (%s) on %s\n",
			me->name(1), geteuid(me),
			obj->name(1), obj->query("id"),geteuid(obj),
			ctime(time()) ) );
		return 1;
	}
	if( obj->move(environment(me)) ) {
		write(obj->query("name") + "复制成功，放在这个房间。\n");
		message_vision(msg + "\n", me, obj);
		// log for clones
		if (geteuid(me) != "yuj")
		log_file("static/CLONE_OBJ",sprintf("%s(%s) clone %s(%s) (%s) on %s\n",
			me->name(1), geteuid(me),
			obj->name(1), obj->query("id"), geteuid(obj),    
			ctime(time()) ) );
		return 1;
	}

	destruct(obj);
	return notify_fail("无法复制不能移动的物件(" + file + ")。\n");
}

int help(object me)
{
  write(@HELP
指令格式 : clone <档名>

利用此指令可复制任何能移动之物件(含怪物)。
HELP
    );
    return 1;
}
