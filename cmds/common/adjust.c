// YUJ@SJ 2001-11-02
// Modified by LOOKLOVE@SJ 引入乘幂计算，大大优化。
// 2002/4/29

// Exp在 100K 以上可以修改属性。
//#define MAX_EXP 6000000
#define MIN_EXP 100000

// 剩余EXP比率等于100%减去每点扣除比率
// 当前修改每点属性，扣除exp比率为5%
#define RATE 0.9500

inherit F_CLEAN_UP;
#include <ansi.h>

int help();

static mapping gift_desc = ([
	"str":"膂力",
	"con":"根骨",
	"dex":"身法",
	"int":"悟性"
]);

static string *gift = keys(gift_desc);

// 统一过滤 by Lklv
string check(object ob)
{
	string msg;

	if (ob->query("registered") < 3)
		msg = "抱歉：只有正式玩家才有这个机会。\n";
	if ((int)ob->query("combat_exp") < MIN_EXP)
		msg = "抱歉：此人经验不够 "+MIN_EXP+" ，无法修改属性。\n";
//	if ((int)ob->query("combat_exp") > MAX_EXP)
//		msg = "抱歉：此人经验超过了 "+MAX_EXP+" ，不能修改属性了。\n";
	return msg;
}

/* lklv算法：exp * 95%的num次方, pow() 为乘幂计算。
int check_exp(int exp, int num)
{
	int x;
	float i, j, y;

	i = to_float(exp);
	j = to_float(num);
	y = i * pow(RATE, j);
	x = to_int(y);

	return x;
}*/
// yuj 新公式，利用循环算法。
int check_exp(int exp, int num)
{
        while (num--) exp -= exp / 20;
        return exp;
}

int main(object me, string arg)
{
	object ob;
	string arg1, msg, from, to;
	int exp, num, newexp, tmpexp;

	if (wiz_level(me) < wiz_level("(arch)"))
		return 0;
	if (!arg) return help();

	if (sscanf(arg, "%s %d %s to %s", arg1, num, from, to) == 4){
		ob = LOGIN_D->find_body(arg1);
		if (!ob)
			return notify_fail(HIR"抱歉：没有这个玩家，可能眼花。\n"NOR);
		if ( stringp(msg = check(ob)) )
			return notify_fail(msg);

		exp = ob->query("combat_exp");

		if (member_array(from, gift) < 0)
			return notify_fail("你要扣除什么天赋？\n");
		if (member_array(to, gift) < 0)
			return notify_fail("你要增加什么天赋？\n");
		if (from == to)
			return notify_fail("去死吧！\n");

		if (ob->query(from) <= 10)
			return notify_fail(ob->query("name")+"的"+gift_desc[from]+"不能再减少了。\n");
		if ( ((int)ob->query(from) - num) < 10)
			return notify_fail(ob->query("name")+"的"+gift_desc[from]+"减少"+num+"点将少于10点，不能修改。\n");
		if (ob->query(to) >= 30 )
			return notify_fail(ob->query("name")+"的"+gift_desc[to]+"不能再增加了。\n");
		if ( ((int)ob->query(to) + num) > 30)
			return notify_fail(ob->query("name")+"的"+gift_desc[to]+"增加"+num+"点将超过30点，不能修改。\n");

		switch (ob->query("family/family_name")) {
			case "明教":
				if (from == "int" && ( ob->query("int") <= 20 || ((int)ob->query("int") - num) <=20 ) )
					return notify_fail("明教弟子悟性要求至少 20。\n");
				break;
			case "大轮寺":
				if (from == "str" && ( ob->query("str") <= 20 || ((int)ob->query("str") - num) <=20) )
					return notify_fail("大轮寺弟子膂力要求至少 20。\n");
				if (from == "con" && ( ob->query("con") <= 20 || ((int)ob->query("con") - num) <=20) )
					return notify_fail("大轮寺弟子根骨要求至少 20。\n");
				break;
		}

		ob->add(from, -num);
		ob->add(to, num);

		// 计算扣除exp
		newexp = check_exp(exp,num);
		tmpexp = exp - newexp;

		ob->add("combat_exp", -tmpexp);

		// 膂力限制金蛇秘籍
		if ( ob->query("str") > 20) {
			ob->delete_skill("jinshe-zhangfa");
			ob->delete_skill("jinshe-jianfa");
		}

		// 悟性限制读书写字
		if (ob->query_skill("literate", 1) > ob->query("int") * 20)
			ob->set_skill("literate", ob->query("int") * 20);

		// 双手互搏要求悟性 <= 25
		if (ob->query("int") > 25){
			ob->delete("double_attack");
			ob->delete("zhou");
		}

		// 凌波微步要求悟性 >= 22
		if ( ob->query("int") < 22 ){
			ob->delete("dali");
			ob->delete_skill("lingbo-weibu", 1);
		}

		message_vision("$N将$n的"+num+"点"+gift_desc[from]+"调整到"+gift_desc[to]+"上。\n", me, ob);
		ob->setup();
		ob->save();
		log_file("static/ADJUST",
			me->query("name")+"("+capitalize(geteuid(me))+")"
			+"调整"+ob->query("name")+"("+capitalize(geteuid(ob))+")"+"的"+num+"点"+gift_desc[from]
			+"到"+gift_desc[to]+"上，扣除 "+tmpexp+" 点经验，剩余经验"+newexp+"\n",
			me,
			({ ob })
		);
		return 1;
	}

	if (sscanf(arg, "%s check %d", arg1, num) == 2){
		ob = LOGIN_D->find_body(arg1);
		if (!ob)
			return notify_fail("抱歉：没有这个玩家。\n");
		if ( stringp(msg = check(ob)) )
			return notify_fail(msg);

		// 计算扣除exp
		exp = ob->query("combat_exp");
		newexp = check_exp(exp,num);
		tmpexp = exp - newexp;

		msg = HIG"$n提示$N：如果$N要修改 "+num+" 点属性，大约要扣除 "+tmpexp+" 点EXP，还剩 "+newexp+" 点EXP。\n"NOR;

		if ( newexp < 95000 )
			msg += "抱歉：此人修改"+num+"点属性后经验将不够 "+MIN_EXP+" ，无法修改属性。\n";
		if (ob->query_skill("jinshe-zhangfa",1) || ob->query_skill("jinshe-jianfa",1))
			msg += HIR"注意：如果修改臂力高于20，$N的金蛇剑法和金蛇掌法将被删除。\n"NOR;

		if (ob->query("double_attack") && ob->query("zhou"))
			msg += HIR"注意：如果修改悟性高于25，$N的左右互搏将被删除。\n"NOR;

		if (ob->query("dali") && ob->query_skill("lingbo-weibu", 1))
			msg += HIR"注意：如果修改悟性低于22，$N的凌波微步将被删除。\n"NOR;

		message_vision(msg, ob, me);
		return 1;
	}
	return help();
}

int help()
{
	write("修改属性：adjust <id> <点数> <要扣除的天赋> to <要增加的天赋>\n");
	write("计算扣除损失：adjust <id> check <要修改的点数>\n");
	return 1;
}
