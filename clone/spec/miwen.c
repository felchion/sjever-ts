//flowray 2005.6.16
#include <ansi.h>

inherit SPEC;


void create()
{
	set_name (HIY"珠宝商秘闻"NOR, ({ "miwen","book"}));
	set("long","你可以向扬州珠商店的朱老板出示("HIY"show"NOR")这本秘闻。\n");

	set("unit","本");
	set_weight(100);
	set("value",3000000);
		
	set("no_get",1);
	set("no_drop",1);
	set("no_get_from",1);
	
	set("degree",1);
	set("flag","spec/miwen");
	set("desc","可以到扬州珠宝店索取一个神秘礼物。");
	set("credit",200);
	setup();
}


int restrict() {
	object me = this_player();

	if (me->query_temp("token/maker") || me->query_temp("token/apply"))
		return notify_fail("你要先把现有的礼品处理掉。\n");
	
	return  ::restrict();
}

void init()
{
	add_action("do_show","show");   
}

int do_show(string str)
{
	
	object my_object,me = this_player();
	
	if (!restrict()) {return notify_fail("本周你已经不能使用"+this_object()->query("name")+"了。\n");}
	
	if (!str)
		return notify_fail("你要把东西给谁看啊？\n");
    
	if (present("box", me)){
		return notify_fail("你要先把身上的礼盒处理掉。\n");	
	}	
	
	if (!(my_object = present(str, environment(me))))
		return notify_fail("你要找的人不在这里。去别处找找看吧。\n");
	
	if ( str != "zhu laoban")
		return notify_fail("这个东西不能随便给别人看的！\n");
    
	if (!living(my_object))
		return notify_fail("哦，你得先把他弄醒在说。\n");
	
	if (me->query_temp("token/maker"))
		return notify_fail("先把你身上的一个送出去再说吧。\n");
		
	write("朱老板大吃一惊道：“你这是从什么地方得来的？”\n"NOR);
	write("朱老板叹道：“罢了罢了,这里有几样小东西您就任选一个吧,千万不要把这事告诉别人！”\n");
	write("朱老板到内堂拿出几样东西摆在你的面前。\n");
	write(HIM"紫晶项链"NOR"(necklace),"HIW"白金钻戒"NOR"(ring),"HIG"翡翠玉镯"NOR"(bangle),"+
				HIR"龙凤玉佩"NOR"(jade),"HIY"黄金发饰"NOR"(headgear)\n");	
	write("你要选什么呢：");
	input_to("choose_item", me);
	return 1;
}	 

private void choose_item(string arg, object me)
{
	object ob;
	if (arg!="necklace" && arg!="ring" && arg!="jade" && arg!="bangle" && arg!="headgear")
	{
		write("看清楚点到底要选什么：");
		input_to((: choose_item :), ob,me);
		return;
	}
	
	
	ob=new("/clone/spec/item/box.c");
	ob->set("token/id",arg);
	

	ob->set("token/ap",11+random(30));

	write("你要在刻上什么赠言：");
	input_to("desc_item", me,ob);
	return; 
}

private void desc_item(string arg, object me,object ob)
{
	if (!strlen(strip(arg))) 
	{
		write("你到底要刻的什么内容啊？");
		input_to((: desc_item :),me,ob);
		return;
	}
	
	arg = explode(arg+"\n", "\n")[0];
	arg = replace_string(arg, "$RED$", RED);
	arg = replace_string(arg, "$GRN$", GRN);
	arg = replace_string(arg, "$YEL$", YEL);
	arg = replace_string(arg, "$BLU$", BLU);
	arg = replace_string(arg, "$MAG$", MAG);
	arg = replace_string(arg, "$CYN$", CYN);
	arg = replace_string(arg, "$WHT$", WHT);
	arg = replace_string(arg, "$HIR$", HIR);
	arg = replace_string(arg, "$HIG$", HIG);
	arg = replace_string(arg, "$HIY$", HIY);
	arg = replace_string(arg, "$HIB$", HIB);
	arg = replace_string(arg, "$HIM$", HIM);
	arg = replace_string(arg, "$HIC$", HIC);
	arg = replace_string(arg, "$HIW$", HIW);
	arg = replace_string(arg, "$NOR$", NOR);
	if (strlen(strip(arg)) > 20)
	{
		write("赠言还是要简短一点,请重新输入:");
		input_to((: desc_item :),me,ob);
		return;
	}
	arg = " "+arg+NOR+"   「"+me->query("name")+"("+me->query("id")+") 谨赠」";
	ob->set("token/desc",arg);
	write("片刻朱老板就在你要的东西上刻好了赠言,把它装在一个"HIW"水晶盒子"NOR"里交给你。\n");
	ob->move(me);
	me->set_temp("token/maker",1);
	degree();
}