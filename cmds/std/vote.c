// vote command..
// Improved by YUJ@SJ, 2000-11-11

#include <ansi.h>

inherit F_SAVE;

string query_save_file()
{
	return "/log/vote";
}

void create() {
	seteuid(getuid());
	restore();
}

int remove()
{
	save();
	return 1;
}

mapping vote=([
	"time":	0,
	"valid": 0,
	"desc": "",
	"num": 0,
	"choice": ([
		"1": "同意",
		"2": "反对"
	]),
	"votes": ([
		"同意": 0,
		"反对": 0
	]),
	"total": 0
]);

void set_vote()
{
	write("现在开始设置投票内容，可以随时输入空行来取消\n");
	write("请输入投票的描述：");
	input_to("get_desc");
}

void get_desc(string desc)
{
	mapping ch = ([]);

	if (desc == "") return;
	write("请输入供选择的内容，. 表示结束\n1：");
	input_to("get_choice", ch, 1, desc);
}

void get_choice(string arg, mapping ch, int i, string desc)
{
	if (arg == "") return;
	if (arg == ".") {
		vote=([
			"desc": desc,
			"time":	time(),			// 将来再加指定时间，lazy yuj 2000-11-11
			"valid": time() + 86400*3,
			"num": i-1,
			"choice": ch,
			"votes": ([]),
			"total": 0
		]);
		save();
		return;
	}
	ch[""+i] = arg;
	write(++i+"：");
	input_to("get_choice", ch, i, desc);
}

void show_vote()
{
	int t = time(), i;
	string item;

	write(sprintf("%-30s%4d\n", "已经收到有效的选票数：", vote["total"]));
	for (i = 1; i <= vote["num"]; i++) {
		item = vote["choice"][""+i];
		write(
			sprintf("%-30s%4d%7.2f%%\n", "选择 " HIG + item + NOR " 的票数：",
				vote["votes"][item],
				vote["total"]?vote["votes"][item]*100.0/vote["total"]:0.0
			)
		);
	}
	if (t < vote["valid"])
		write("距离投票结束还有"+CHINESE_D->chinese_time(vote["valid"]-t)+"\n");
}

int main(object me, string arg)
{
	object victim;
	string act_name, victim_name;
	string file;
	object file_ob;
	int t = time();
	int i;
	string item;

	if (me->is_busy()) return notify_fail("你正忙着呢。\n");
	if (me->query("age") < 16 && !wizardp(me))
		return notify_fail("民主不是儿戏！小孩子一边玩去！\n");

	if (me->query("vote/deprived") && me->query_condition("vote_suspension"))
		return notify_fail("你想起当初玩弄民主、被剥夺投票权的事，追悔莫急。\n");
	me->delete("vote/deprived");
	if (!arg) {
		if (!vote["time"]) return notify_fail("目前没有需要投票的内容。\n");
		write(sprintf("\n%-30s" HIW "%s\n" NOR, "目前投票的内容：", vote["desc"]));
		if (t < vote["time"]) return notify_fail("投票还未开始");
		show_vote();
		me->set_temp("votes", vote["time"]);
		if (t > vote["valid"])
			return notify_fail("投票已经停止。\n");
		if (wiz_level(me)) return notify_fail("巫师没有投票权。\n");
		if (me->query("votes") > vote["time"]) return notify_fail("你已经投过票了。\n");
		if (me->query("mud_age") < (t - me->query("birthday"))/20)
			return notify_fail("你目前还没有投票资格。\n");
		if (!vote["num"]) return 1;
		me->start_busy(1);
		write("你可以不参加投票或者有以下选择：\n");
		for (i = 1; i <= vote["num"]; i++)
			write("vote " + i + " 表示：" HIG + vote["choice"][""+i] + NOR "\n");
		write("票一旦投出就不能更改，请自己注意。\n");
		return notify_fail("这神圣的一票，要想清楚了才能投。\n");
	}

	if (arg == "set") {
		if (wiz_level(me) < 5) return 0;
		set_vote();
		return 1;
	}
	if (arg == "remove") {
		if (wiz_level(me) < 5) return 0;
		vote=([
			"time":	0,
			"valid": 0,
			"desc": "",
			"num": 0,
			"choice": ([
				"1": "同意",
				"2": "反对"
			]),
			"votes": ([
				"同意": 0,
				"反对": 0
			]),
			"total": 0
		]);
		save();
		return 1;
	}

	if (sscanf(arg, "%d", i)) {
		if (wiz_level(me) && wiz_level(me) < 5) return notify_fail("巫师没有投票权。\n");
//		if (me->query("registered") < 3)
//			return notify_fail("正式用户才有投票资格。\n");
		if (me->query_temp("votes") < vote["time"]) {
			write("你还没看投票内容就投票？I 服了 You。\n");
			i = 0;
		}
		if (t < vote["time"])
			return notify_fail("投票还未开始");
		if (t > vote["valid"])
			return notify_fail("投票已经停止。\n");
		if (i < 1 || i > vote["num"])
			return notify_fail("请用 vote 仔细阅读投票说明。\n");
		if (me->query("votes") > vote["time"])
			return notify_fail("你不是已经投过票了吗？\n");
		item = vote["choice"][""+i];
		write("\n" HIW + vote["desc"] + NOR " 你选择了 " HIW + item + NOR "\n");
		vote["votes"][item]++; vote["total"]++;
		me->set("votes", t);
		if (!(vote["total"] % 5))
			save();
		show_vote();
		return 1;
	}

	if (sscanf(arg, "%s %s", act_name, victim_name) != 2)
		return notify_fail("这神圣的一票，要想清楚了才能投。\n");

	victim = LOGIN_D->find_body(victim_name);
	if (!victim || !me->visible(victim))
		return notify_fail("你要投谁的票？\n");
	if (!wizardp(me) && wizardp(victim))
		return notify_fail("你没权力对巫师投票!\n");
	if (!victim->query_condition("vote_clear")) {
		victim->delete("vote/reason");
		victim->delete("vote/juror");
		victim->delete("vote/count");
	}
	if( !stringp(file = __DIR__"vote/"+act_name)
	|| file_size(file + ".c") <= 0 )
		return 	notify_fail("你要投票干什么？\n");

	if ( !(file_ob = find_object(file)) ) file_ob = load_object(file);

	if ((int)file_ob->vote(me, victim) <= 0) {
		if (me->query("vote/abuse")> 50) {
			write( HIG "你因为胡乱投票，投票权被剥夺了！\n" NOR);

			me->set("vote/deprived", 1);
			me->apply_condition("vote_suspension", 120);
			me->delete("vote/abuse");
		}
		return 0;
	};

	return 1;
}

int valid_voters(object me)
{
	object *list;
	int d = 0;
	int j;

	list = users();
	j = sizeof(list);
	while( j-- ) {
		// Skip those users in login limbo.
		if( !environment(list[j]) ) continue;
		if( !me->visible(list[j]) ) continue;
		if (list[j]->query("age")<16 && !wizardp(list[j])) continue;
		if (list[j]->query("vote/deprived")) continue;

		d++;
	}

	return d;
}

int help(object me)
{
	write(@HELP
指令格式 : vote <动议> <某人>

此命令提议对某人采取行动，由大家投票决定。可是如果五分钟内没有人附议，
投票会自动取消。当前可以有如下<动议>：

chblk:   关闭某人交谈频道。
unchblk: 打开某人交谈频道。
其他：对某特定事件投票，要求经验大于五十万。

HELP
	);
	return 1;
}
