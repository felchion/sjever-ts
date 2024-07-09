//flowray 2005.6.16
#include <ansi.h>


inherit SPEC;


void create()
{
	set_name (HIR"鸳鸯锦帕"NOR, ({ "jinpa"}));
	set("long",HIM" 一方精心绘绣的锦帕，上面刺着两只鸳鸯戏水，旁边绣着几行小令:\n"HIW
   " 四张机
 鸳鸯织就欲双飞 可怜未老头先白
 碧波春草 晓寒深处
 相对浴红衣\n\n"NOR
 "快点把它归还(guihuan)给失主吧。\n");

	set("unit","方");
	set_weight(100);
	set("value",3000000);
		
	set("no_get",1);
	set("no_drop",1);
	set("no_get_from",1);
	
	set("degree",1);
	set("flag","spec/jinpai");
	set("rest",5);
	set("desc","增加两个小时的鬼谷算术时间。");
	set("credit",10);
	setup();
}

void init()
{
	add_action("do_back","guihuan");   
}

int restrict() {
	object me = this_player();

	if (me->query("registered")<3)
		return notify_fail("只有贵宾用户才能使用鬼谷算术。\n");
	
	return  ::restrict();
}

int do_back(string str)
{
	object my_object,me = this_player();
	
	if (!restrict()) {return notify_fail("本周你已经不能使用"+this_object()->query("name")+"了。\n");}
	
	if (!str)
		return notify_fail("你要把东西还谁啊？\n");
    
	if (!(my_object = present(str, environment(me))))
		return notify_fail("你要找的人不在这里。去别处找找看吧。\n");
	
	if ( str != "ying gu")
		return notify_fail("这个人好象不是失主吧！\n");
    
	if (!living(my_object))
		return notify_fail("哦，你得先把她弄醒在说。\n");
	
	me->add("ggs/left_time",7200);
	write("你给瑛姑一方"HIR"鸳鸯锦帕\n"NOR);
	write("瑛姑呢喃到：“可怜未老头先白……头先白……，既然你帮我找回了我孩子的遗物，我自然要传授你更深的鬼谷算术”。\n");
	write("瑛姑给你增加了一个时辰的鬼谷算术时间\n");
	
	degree();
	
	return 1;
}	 