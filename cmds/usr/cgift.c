// YUJ@SJ 2001-11-02
// Modified by LOOKLOVE@SJ 引入乘幂计算，大大优化。
// 2002/4/29
// 改为玩家自行修改属性

// Exp在 100K 以上可以修改属性。
#define MIN_EXP 100000
// 剩余EXP比率等于100%减去每点扣除比率
// 当前修改每点属性，扣除exp比率为5%
// #define RATE 0.9500

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
string check(object me)
{
	string msg;

	if (me->query("registered") < 3 && (int)me->query("combat_exp") > 10000)
		msg = HIY"系统提示："NOR"抱歉，只有贵宾才有这个机会。\n";
	if ((int)me->query("combat_exp") < MIN_EXP && (int)me->query("combat_exp") > 10000)
		msg = HIY"系统提示："NOR"抱歉，你的经验不够 "+MIN_EXP+" ，无法修改属性。\n";
	if (me->is_ghost())
		msg = HIY"系统提示："NOR"抱歉，你的状态不稳定，现在无法修改属性。\n";
	return msg;
}

// yuj 新公式，利用循环算法。
int check_exp(int exp, int num,string from,string to)
{
	if (exp < 10000) return exp;
	if (from != "int") return exp;
	while (num--) {
		if (from == "int")
		  exp -= exp / 100 * 3;
		else if (to == "con" || to == "str" || to == "dex")
		  exp -= exp / 100;
		else
		  exp -= exp / 200;
	}
	return exp;
}

int main(object me, string arg)
{
	string msg, from, to;
	int exp, num, newexp, tmpexp;

	if (!me) return 0;
	if (!arg) return help();

	if ( stringp(msg = check(me)) )
		return notify_fail(msg);

  if( me->is_busy() ) return notify_fail("你正忙着呢。\n");

	if ( me->query("potential", 1) > me->query("max_pot", 1) )
	return notify_fail("你现在暂时无法调整天赋。\n");
	
	exp = me->query("combat_exp");

	if (sscanf(arg, "%d %s to %s", num, from, to) == 3){		
		if (member_array(from, gift) < 0)
			return notify_fail("你要扣除什么天赋？\n");
		if (member_array(to, gift) < 0)
			return notify_fail("你要增加什么天赋？\n");
		if (from == to || num<0 )
			return notify_fail("检查一下，输入错误，可能眼花。\n");
	//	if(me->query("double_attack") && to=="int")
	//		return notify_fail("你学会了左右互搏，不能将悟性增加。\n");
		
	//	if (me->query("quest/chance")!=me->query("quest/chance_times") 
	//	&& (to=="int" || from=="int"))
	//		return notify_fail("你已有解迷记录，现在不能调整悟性。\n");
		
		if (me->query(from) <= 10)
			return notify_fail("你的"+gift_desc[from]+"不能再减少了。\n");
		if ( ((int)me->query(from) - num) < 10)
			return notify_fail("你的"+gift_desc[from]+"减少"+num+"点后将少于10点，不能修改。\n");
		if (me->query(to) >= 30 )
			return notify_fail("你的"+gift_desc[to]+"已经大于30点，不能再增加了。\n");
		if ( ((int)me->query(to) + num) > 30)
			return notify_fail("你的"+gift_desc[to]+"增加"+num+"点后将超过30点，不能修改。\n");

		write(HIR"你确定要修改以上属性(y/N)：\n"NOR);
		input_to("confirm_passwd", me, from, to, num);
		return 1;
	}

	if (sscanf(arg, "check %d %s to %s", num, from, to) == 3){

		if ( num > 80 )
			return notify_fail("输入的数值超过了你的隐藏属性合。\n");
		// 计算扣除exp

		newexp = check_exp(exp,num,from,to);
		tmpexp = exp - newexp;

		msg = HIG"提示：如果要修改 "+num+" 点属性，你大约要扣除 "+tmpexp+" 点EXP，还剩 "+newexp+" 点EXP。\n"NOR;

//		if ( newexp < 95000 && (int)me->query("combat_exp") > 1000)
//			msg += HIR"抱歉：修改"+num+"点属性后经验将不够 "+MIN_EXP+" ，建议你一次少修改几点属性。\n"NOR;

		write(msg);
		return 1;
	}
	return help();
}

private void confirm_passwd(string pass, object me, string from, string to, int num)
{
	int exp, newexp, tmpexp;
	string confirm_pass;
	confirm_pass = me->query("confirm_passwd");

	if ( pass != "Y" && pass != "y" ) {
		write("放弃修改！\n");
		return;
	}
	// 计算扣除exp
	exp = me->query("combat_exp", 1);
	newexp = check_exp(exp,num,from,to);
	tmpexp = exp - newexp;

	me->start_busy(3);

	me->add(from, -num);
	me->add(to, num);
	
	if (from != "int")
		tmpexp = 0;
	else 
  	me->add("combat_exp", -tmpexp);
		
	write("你将"+num+"点"+gift_desc[from]+"调整到"+gift_desc[to]+"上。\n", me);
	me->setup();
	me->save();
	log_file("static/ADJUST",
		me->query("name")+"("+capitalize(geteuid(me))+")"
		+"调整"+num+"点"+gift_desc[from]
		+"到"+gift_desc[to]+"上，扣除 "+tmpexp+" 点经验，剩余"+newexp+"\n",
		me,
		({ me })
	);
	return;
}

int help()
{
	write("本指令用来修改先天属性。\n");
	write("每减少1点悟性将扣除3% exp。\n");
	write("增加悟性不补偿经验。\n");
	write("指令格式：\n");
	write("计算扣除损失：cgift check <点数> <要扣除的天赋> to <要增加的天赋>\n");
	write("修改属性：cgift <点数> <要扣除的天赋> to <要增加的天赋>\n");
	write("比如：你要把一点膂力加到悟性上，你可以用："HIG" cgift 1 str to int "NOR"\n");
	write("四项先天属性代号如下:膂力:str,根骨:con,身法:dex,悟性:int\n");
	write("请慎重使用。\n");
	write("经验值小于10000可任意修改属性。\n");
	return 1;
}
