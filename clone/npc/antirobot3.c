// designed by Yu Jue
// Modified by snowman@SJ 12/06/2000
// Powered by Yu Jue 2000-08/09
// Powered again by YUJ@SJ 2001-05-06

#define HW "[0;37;40m"
#define BK "[30;40m"

#define FONTLIB 3

#include <ansi.h>

inherit NPC;

mixed keypos=({
	({ 0,0,0,0,0,0,0,0,0,0 }),
	({ 0,0,0,0,0,0,0,0,0,0 }),
	({ 0,0,0,0,0,0,0,0,0,0 })
});

mixed keylen=({
	({  8, 4, 8, 8, 8, 8, 8, 8, 8, 8 }),
	({  8, 4, 8, 8, 8, 8, 8, 8, 8, 8 }),
	({ 10, 4,10,10,10,10,10,10,10,10 })
});

mixed key=({
	({
		"■■■　■　■■■　■■■　■　■　■■■　■■■　■■■　■■■　■■■　",
		"■　■　■　　　■　　　■　■　■　■　　　■　　　　　■　■　■　■　■　",
		"■　■　■　■■■　■■■　■■■　■■■　■■■　　　■　■■■　■■■　",
		"■　■　■　■　　　　　■　　　■　　　■　■　■　　　■　■　■　　　■　",
		"■■■　■　■■■　■■■　　　■　■■■　■■■　　　■　■■■　■■■　",
		"　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　"
	}),
	({
		"　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　",
		"■■■　■　■■■　■■■　■　■　■■■　■■■　■■■　■■■　■■■　",
		"■　■　■　　　■　　　■　■　■　■　　　■　　　　　■　■　■　■　■　",
		"■　■　■　■■■　■■■　■■■　■■■　■■■　　　■　■■■　■■■　",
		"■　■　■　■　　　　　■　　　■　　　■　■　■　　　■　■　■　　　■　",
		"■■■　■　■■■　■■■　　　■　■■■　■■■　　　■　■■■　■■■　"
	}),
	({
		"┌───┐┌┐┌───┐┌───┐┌┐　┌┐┌───┐┌───┐┌───┐┌───┐┌───┐",
		"│┌─┐│││└──┐│└──┐│││　│││┌──┘│┌──┘└──┐││┌─┐││┌─┐│",
		"││　││││┌──┘│┌──┘││└─┘││└──┐│└──┐　　　│││└─┘││└─┘│",
		"││　│││││┌──┘└──┐│└──┐│└──┐││┌─┐│　　　│││┌─┐│└──┐│",
		"│└─┘││││└──┐┌──┘│　　　││┌──┘││└─┘│　　　│││└─┘│┌──┘│",
		"└───┘└┘└───┘└───┘　　　└┘└───┘└───┘　　　└┘└───┘└───┘"
	})
});

string ans="", ans1, fakeans="";
string str = "\n　\n　\n　\n　\n　\n　\n　\n　\n银翼杀手说道：请用 answer 回答，上面这行数字是？\n";
int aflag;
int num = 2;

#include "antirobot.h";

void question()
{
	message("channel", HW+str+NOR, ob);
	aflag = 1;
}

int do_answer(string arg)
{
	object me = this_object();
	int len = strlen(ans);

	if (arg) {
		int i = strlen(arg);

		while (i--)
			if (arg[i] == ' ')
				arg = arg[0..i-1] + arg[i+1..<1];
	} else
		arg = "";
	if (strlen(arg) != len)
		return notify_fail("答案的长度不对，再仔细看看。\n");
	if (aflag) {
		aflag = 0;
		if (ans1 == arg) {
			command("wiz "+ob->query("id")+" 极可能使用了自动应答机器人，请检查！");
			return notify_fail("答案不对，如果看不清楚，请将字体设置为 Fixedsys。\n");
		}
		ans1 = arg;
	}
	if (arg==ans) {
		message_vision("$N点了点头，说道：OK，$n继续忙吧。\n", me, ob);
		if (num == 2) give_reward(ob);
		message_vision("$N突然消失不见了。\n", me);
	} else {
		if (arg==fakeans)
			command("wiz "+ob->query("id")+" 绝对使用了自动应答机器人，请逮捕！");
		if (num--)
			return notify_fail("答案不对，如果看不清楚，请将字体设置为 Fixedsys。\n");
		message_vision("$N叹了口气道："+arg+"？"+ans+"才对，这么简单都答不出？去法庭接受聆讯吧！\n", me);
		move_to_court(ob);
	}
	destruct(this_object());
	return 1;
}

void test_robot(object obj)
{
	int len, temp, c, l, font, pos, end;
	string *line=({"", "", "", "", "", ""}), ta;
	string *line1=({"", "", "", "", "", ""}), gap;
	object me = this_object();

	ob = obj;
	if (environment(ob) && !ob->is_fighting()) {
		message("channel", "银翼杀手略带歉意地说：“机器人稽查，例行公事，请回答一个小问题。”\n", ob);
		message("channel", "如果利用机器自动回答问题，档案将被无条件删除。\n", ob);
		for (l = 0; l < FONTLIB; l++)
			for (temp = 1; temp < 10; temp++)
				keypos[l][temp] = keypos[l][temp-1] + keylen[l][temp-1];

		// 生成答案
		temp = len = random(2) + 2;
		while (temp--) {
			ans += "0";
			ans[<1] += random(10);
			fakeans += "0";
			fakeans[<1] += ans[<1] == '1'?1:2+random(8);
		}

		for (temp = 0; temp < len; temp++) {
			gap = repeat_string("　", 1+random(2));
			c = ans[temp] - '0';
			font = random(FONTLIB);
			pos = keypos[font][c];
			end = pos + keylen[font][c] -1;
			for (l = 0; l < 6; l++)
				line[l] += gap + key[font][l][pos..end];
			c = fakeans[temp] - '0';
			pos = keypos[font][c];
			end = pos + keylen[font][c] -1;
			for (l = 0; l < 6; l++)
				line1[l] += gap + key[font][l][pos..end];
		}
		str += SAVEC + "[9A[K　\n";
		str += repeat_string("　\n", 6);
		gap = repeat_string("　", 1+random(2));
		for (l = 0; l < 6;) {
			temp = random(6);
			if (!line[temp]) continue;
			str += "["+(6-temp)+"A[K";
			str += line[temp] + gap;
			str += repeat_string("　\n", 6-temp);
			line[temp] = 0;
			l++;
		}
		// 干扰字符
		for (l = 50; l < strlen(str); l++) {
			if (str[l] < 161) continue;
			if (str[l..l+1] == "　") {
				if (!random(3)) {
					string *tab = key[random(FONTLIB)];

					ta = tab[random(sizeof(tab))];
					c = random(sizeof(ta)/2)*2;
					str = str[0..l-1]
						+ BK + ta[c..c+1]
						+ HW + str[l+2..<1];
					l += strlen(BK+HW);
				}
			}
			l++;
		}
		str += REST;
		timer = 180;
		call_out("test1", 10);
		me->move(ob, 1);
	} else destruct(me);
}
