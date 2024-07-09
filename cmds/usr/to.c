// to.c

// inherit F_CLEAN_UP;
// Don't want this command be cleaned up while typing long string.

int help(object);

int main(object me, string arg)
{
	if (!arg)
		return help(me);
	if (!wiz_level(me)) {
		string cmd;

		if (sscanf(arg, "%s ", cmd)!=1)
			cmd = arg;
		switch(cmd) {
			case "reply":
			case "describe": break;
			default: return help(me);
		}
	}
	me->edit( (: call_other, this_object(), "do_to" , me, arg :) );
	return 1;
}

void do_to(object me, string arg, string str)
{
	seteuid(getuid());
	str = str[0..<2];
	if (strlen(str) > 800) {tell_object(me, "命令太长！\n"); return;}
	if(arg) me->force_me(arg + " " + str);
	else me->force_me(str);
}

int help(object me)
{
write(@HELP
指令格式 : to <指令>

可以一次输出多行以上的文字。
指令范围是：describe、reply。

范例：
to describe
结束离开用 . 取消输入用 q
———————————————————————————————————————
1: 东方神教教主 : 斯诺曼
2: 教主夫人暨发呆部部长 : Xiaoyao
3: 教主大舅子暨摸鱼部部长 : 外优勾
4: 大弟子暨打混部部长 : 碧碧碧
5: .

您对自己的描述设置完成了。

HELP
    );
    return 1;
}
