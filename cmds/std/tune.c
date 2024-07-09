// tune.c

inherit F_CLEAN_UP;

mapping ch_name = ([
	"chat":		"闲聊(chat)",
	"rumor":	"谣言(rumor)",
	"party":	"门派(party)",
	"sj":		"书剑(sj)",
	"irc":		"聊天室(irc)",
	"declare":	"信息(declare)",
]);

int main(object me, string arg)
{
	string *tuned_ch;

	if (arg == "all") {
		write("关闭所有频道。\n");
		me->set("channels", ({}));
		return 1;
	}

	tuned_ch = me->query("channels");
	if (!arg) {
		if (!sizeof(tuned_ch))
			write("你现在并没有收听任何频道。\n");
		else
			write("你现在收听的频道：" + implode(tuned_ch, "，") + "。\n");
		return 1;
	}

	if (!tuned_ch) tuned_ch = ({});
	if (member_array(arg, tuned_ch) != -1) {
		tuned_ch -= ({ arg });
		arg = ch_name[arg] ? ch_name[arg] : " " + arg + " ";
		write("关闭" + arg + "频道。\n");
	} else if (ch_name[arg]) {
		tuned_ch += ({ arg });
		write("打开" + ch_name[arg] + "频道。\n");
	} else write("要打开某个频道只要用该频道说话即可。\n");
	me->set("channels", tuned_ch);
	return 1;
}

int help(object me)
{
	write(@HELP
指令格式：tune [<频道名称>]

这个指令让你选择是否要收听某一频道的讯息，如果没有指定频道名称，就会列出
你目前收听中的频道，如果指定了频道，原来收听中的就会关掉，反之打开。

如果你对一个没有收听中的频道讲话，会自动将它打开。
要对一个频道讲话，只要用：

<频道名称> <讯息> ....

例子：
  chat hello everyone!

你可以用 help channels 查询目前有哪些频道可以收听。

see also : shout
HELP
	);
	return 1;
}
