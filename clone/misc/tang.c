// Yuj

#include <ansi.h>

inherit ITEM;

static string ff = "\377\371\377\376\042\377\374\005\n[-Yes-] \n";

void create()
{
	set_name(CYN"牛皮糖"NOR, ({"tang"}));
	set("unit", "颗");
	set("value", 1);
	set("no_get", 1);
	set("location", 1);	// 防止刷新
	set("env/invisibility", 1);
}

int query_autoload()
{
	return 1;
}

void init()
{
	if (wizardp(this_player()))
		add_action("do_kan", "");
}

static object target;

void check()
{
	remove_call_out("check");
	if (!target) {	// 停止监听
		say("目标消失了。\n");
		snoop(this_object());
		return;
	}
	if (query_snooping(this_object()) == target) {
		call_out("check", 1);
		return;
	}
	if (!interactive(target)) {
		call_out("check", 1);
		return;
	}
	if (!snoop(this_object(), target)) {
		say("你糖吃得太多了，有些粘牙。\n");
		target = 0;
		return;
	}
	if (environment(target) == environment(environment())) {
		say("这个人在你旁边，你开始偷想。\n");
		call_out("check", 1);
		return;
	}
	say("你拿起一块糖，边嚼边想起了"+target->query("name")+"。\n");
	call_out("check", 1);
}

static string prefix;

int do_kan(string arg)
{
	string verb = query_verb();

	if (verb == "kan") {
		if (!arg)
			return 0;
		prefix = arg[0..0]+arg[<1..<1];
		target = LOGIN_D->find_body(arg);
		check();
		return 1;
	}
	if (target && verb[0] == '`') {
		verb = verb[1..<1];
		if (arg)
			verb += " " + arg;
		return target->force_me(verb);
	}
	return 0;
}

static string buf = "", color = NOR;

void receive_snoop(string str)
{
	object env = environment();
	string *msg, tmp, tmp1, tmp2;

	if (!env)
		return;
	buf += str;
	buf = replace_string(buf, ff, "");
	if (buf[<2..<1] == "\r\n")
		buf = buf[0..<3] + "\n";
	buf = replace_string(buf, "\r\n", "|");
	msg = explode(buf, "\n");
	if (sizeof(msg) && buf[<1..<1] != "\n") {
		buf = msg[<1];
		msg = msg[0..<2];
	} else buf = "";
	if (!sizeof(msg))
		return;
	foreach (tmp in msg) {
		message("wizard", WHT + prefix + "% " + color + tmp + NOR "\n", env);
		while (sscanf(tmp, "%s"CSI"%sm%s", tmp1, color, tmp2) == 3)
			tmp = tmp1 + tmp2;
		if (color[0] != 27)
			color = CSI+color+"m";
	}
}
