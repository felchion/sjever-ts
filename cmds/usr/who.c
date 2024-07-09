/// who.c
// Modified by Yu Jue@JY
// Modify by Yu Jue@SJ 1999-06-24
// Modified by Numa@Sj 1999-11-18 门派重新排列 (不准覆盖了， bite. 4.23.2000)
// Modified by snowman@SJ 14/11/2000.  玩家按游戏时间排列。
// Modified by Looklove@SJ 增加查看在线工匠功能
inherit F_CLEAN_UP;

#include <ansi.h>
#include <mudlib.h>
#include <net/dns.h>

mapping party=([
	"emei":       	"峨嵋派",
	"zaixian":	"在线人员",
	"gaibang":    	"丐帮",
	"gumu":       	"古墓派",
	"huashan":    	"华山派",
	"kunlun":   	"昆仑派",
	"mingjiao":   	"明教",
	"murong":     	"姑苏慕容",
	"quanzhen":   	"全真教",
	"shaolin":    	"少林派",
	"shenlong":   	"神龙教",
	"songshan":   	"嵩山派",
	"taohua":     	"桃花岛",
	"tianlong":   	"天龙寺",
	"tiezhang":   	"铁掌帮",
	"wudang":     	"武当派",
	"xingxiu":    	"星宿派",
	"xueshan":    	"大轮寺",
]);

int sort_user(object,object);

void create() {
	string *pname = values(party);
	int i = sizeof(pname);

	seteuid(getuid());
	while (i--) party[pname[i]] = pname[i];
}

string opt_party;

void remove_busy(object me)
{
	if (me) me->delete_temp("command_busy");
}

mixed main(object me, string arg, int remote)
{
	string str, *option, fname = "";
	object *list;
	int i, ppl_cnt, cnt, count, wcnt = 0;
	int opt_long, opt_id, opt_wiz, opt_wizall, opt_career, opt_zhuanzhan;

	if (!remote) {
		if (me->query_temp("command_busy"))
			return notify_fail("你正忙着呢。\n");
		me->set_temp("command_busy",1);
		call_out("remove_busy", 3, me);
	}

	opt_party = "(null)";

	if( arg ) {
		option = explode(arg, " ");
		option -= ({ "" });
		i = sizeof(option);
		while( i-- )
			switch(option[i]) {
				case "-l": opt_long = 1; break;
				case "-i": opt_id = 1; break;
				case "-wizall": opt_wizall = 1;
				case "-w": opt_wiz = 1; break;
				case "-c": opt_career = 1; break;
				case "-z": opt_zhuanzhan = 1; break;
				case "-f":
				case "-p": opt_party = me->query("family/family_name"); break;
				default:
					if( option[i][0]=='@' ) {
						notify_fail("网路精灵并没有被载入。\n");
						if (find_object(DNS_MASTER) && RWHO_Q->send_rwho_q(option[i][1..<1],
							me, opt_long & wizardp(me), opt_wiz, opt_party )) {
							// 只有工作人员才允许 who @xx -l，Yu Jue@SJ 19990624
							write("网络讯息已送出，请稍候。\n");
							return 1;
						}
						return 0;
					}
					if (option[i][0] == '-' && !undefinedp(party[option[i][1..<1]]))
						opt_party = party[option[i][1..<1]];
					else return notify_fail("指令格式：who [-l|-i|-w|-p|-c]\n");
			}
	}

	if (!remote && opt_wizall && find_object(DNS_MASTER)) {
		string *mud = sort_array(keys(DNS_MASTER->query_muds()), -1);
		int m = sizeof(mud);

		while (m--) RWHO_Q->send_rwho_q(mud[m], me, 0, 1, opt_party);
	}

	if( opt_long && !remote && !wizardp(me)) {
		if( (int)me->query("jing") < 50 )
			return notify_fail("你的精神太差了，没有办法得知所有玩家的详细资料。\n");
		me->receive_damage("jing", 50);
	}

	str = "◎ " + CHINESE_MUD_NAME + (opt_party == "(null)"?"":opt_party)
		+ "在线" + (opt_wiz?"工作人员":opt_career?"工匠":opt_zhuanzhan?"转站人员":"玩家") + "：\n";
	str += repeat_string("━", 40) + "\n";
	list = filter_array(children(USER_OB), (: userp($1) && environment($1) :));
	if (opt_party != "(null)")
		list = filter_array(list, (: $1->query("family/family_name") ==
			opt_party :));
	if ( opt_career )
		list = filter_array(list, (: $1->query_temp("user_type") =="worker" :));

	if ( opt_zhuanzhan && wizardp(me) )
		list = filter_array(list, (: $1->query("bxsj_bonus/oldsj"):));

	list = sort_array(list, (: sort_user :));
	ppl_cnt = 0;
	cnt = 0;
	if( opt_long || opt_wiz ) {
		i = sizeof(list);
		while( i-- ) {
			if (remote && wizardp(list[i]) && list[i]->query("env/invisibility")) continue;
			if (!remote && !me->visible(list[i]) ) continue;
			if (opt_wiz && !wizardp(list[i]) ) continue;
			if (interactive(list[i])) ppl_cnt++;
			else cnt++;
			if (!wiz_level(list[i]) && fname != list[i]->query("family/family_name"))
				fname = list[i]->query("family/family_name");
			if (!wcnt && wiz_level(list[i])) {
			    wcnt = 1;
			}
			str += sprintf("%12s%s%s\n",
				RANK_D->query_rank(list[i]),
				interactive(list[i])?
					(query_idle(list[i]) > 120?HIM"+"NOR:" ")
					:HIG"*"NOR,
				list[i]->short(1)
			);
		}
	} else {
		string name;

		i = sizeof(list);
		count = 0;
		while( i-- ) {
			// Skip those users in login limbo.
			if (remote && wizardp(list[i]) && list[i]->query("env/invisibility")) continue;
			if (!remote && !me->visible(list[i])) continue;
			if (!wiz_level(list[i]) && fname != list[i]->query("family/family_name")) {
				fname = list[i]->query("family/family_name");
				if (count % (opt_id?4:8)) str += "\n";
				str += sprintf(HIY "%-*s" NOR, opt_id?20:10, (fname?fname:"普通百姓") + "：");
				count = 1;
			}
			if (!wcnt && wiz_level(list[i])) {
				if (count % (opt_id?4:8)) str += "\n";
				str += sprintf(HIY "%-*s" NOR, opt_id?20:10, "工作人员：");
				wcnt = 1;
				count = 1;
			}
			name = list[i]->query("name") + (opt_id ? "("+capitalize(list[i]->query("id"))+")" : "");
			str += sprintf(" %-*s%s",
				opt_id?19:9,
				interactive(list[i])?
					(query_idle(list[i]) > 120 ? HIM"+"+name+NOR : " "+name)
					: HIG"*"+name+NOR,
				++count % (opt_id?4:8) ? "" : "\n"
			);
			if (interactive(list[i])) ppl_cnt++;
			else cnt++;
		}
		if( count % (opt_id?4:8) ) str += "\n";
	}
	str += repeat_string("━", 40) + "\n";
	str += sprintf(
		"有 %d 位%s连线中，%d 位%s断线中，系统负担：%s\n"HIG"*"NOR" 表示断线中  "HIM"+"NOR" 表示发呆中\n",
		ppl_cnt, opt_wiz?"工作人员":"玩家", cnt, opt_wiz?"工作人员":"玩家", query_load_average()
	);

	if( remote ) return str;

	me->start_more(str);
	return 1;
}

int sort_user(object ob1, object ob2)
{
	string name1, name2;
	int l1 = wiz_level(ob1), l2 = wiz_level(ob2);

	if (l1 != l2) return l2 - l1;

	if (l1) {
		name1 = ob1->query("id");
		name2 = ob2->query("id");

		if (intp(name1)) name1 = "";
		if (intp(name2)) name2 = "";
		if (name1 > name2) return -1;
		if (name1 < name2) return 1;
		return 0;
	}

	reset_eval_cost();

	name1 = ob1->query("family/family_name");
	name2 = ob2->query("family/family_name");
	if (intp(name1)) name1 = "";
	if (intp(name2)) name2 = "";
	if (name1 > name2) return -1;
	if (name1 < name2) return 1;

	l1 = ob1->query("family/generation");
	l2 = ob2->query("family/generation");
	if (l1 != l2) return l1 - l2;

	// sort by playing time.
	return ob2->query("online_total") - ob1->query("online_total");
	//return (int)ob2->query("combat_exp") - (int)ob1->query("combat_exp");
}

int help(object me)
{
	string *p;
	int i,j;

	write(@HELP
指令格式 : who [-l|-i|-w|-p|-c|-英文代号]

这个指令可以列出所有在线上的玩家及其等级。

-l 选项列出较长的讯息。
-i 只列出玩家的英文代号。
-p 只列出同门的玩家。
-f 同上
-w 只列出线上所有的工作人员。
-c 只列出在线工匠
-英文代号 只列出该门派的玩家。

HELP);
	p = sort_array(keys(party), 1);
	i = sizeof(p);
	j = i;
	write(sprintf("   %-12s%-12s    %-12s%-12s\n","英文代号","门派名","英文代号","门派名"));
	write("----------------------------------------------------------\n");
	for (i=0;i<j/2;i++)
	{
		if (i==(j/2-1))
		{
			write(sprintf("   %-10s：%-12s\n", p[i], party[p[i]]));
			continue;
		}
		if (i/2*2==i)
			write(sprintf("   %-10s：%-12s|", p[i], party[p[i]]));
		else
			write(sprintf("   %-10s：%-12s\n", p[i], party[p[i]]));
	}
	write("----------------------------------------------------------\n");
	write(@HELP

相关指令： finger
HELP);
	return 1;
}
