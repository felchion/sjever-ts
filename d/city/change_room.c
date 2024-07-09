//change room

inherit ROOM;
#include <ansi.h>
#include <mudlib.h>

#define FL "/d/city/change_room"
#define WAY "down"

string* names = ({"魔术房","奇形怪状屋","艺术之家","巫师休息室",});

void create()
{
	set("short", names[random(sizeof(names))]);
	set("long",
"\n这是一间神奇的房间，可以帮助您调整各项属性。
调整先天属性：指令为 change <属性> <数值>
    属性对应项：str(膂力)，con(根骨)，dex(身法)，int(悟性)；
    如涉及Quest，在属性调整后，若不符合标准，将会相应去除；
    调整悟性的请注意：如果读书写字超过了悟性限制，多余部分将去除。\n\n"
HIR+BLINK"注意：您只有一次修改属性的机会，请慎重选择，出此房间将不可以反复。\n\n"NOR);
//HIR"转站ID不了解本站的情况，必须查看所有News，否则会带来不必要的损失。\n\n"
	set("indoors", "扬州");
	
	set("no_fight", 1);
	set("no_sleep", 1);

	set("exits", ([
		WAY : "/d/city/jiulou2",
	]));

	set("incity",1);
	setup();
}

void init()
{
	object me = this_player();
		
	me->set("startroom",FL);
	add_action("do_change", ({"change", "gai"}));
	add_action("do_look",({"look","kan","l"}));//hehe
}

int do_look(string arg)
{
	set("short", names[random(sizeof(names))]);
	return 0;	
}
int do_change(string arg)
{
	object me = this_player();
	string str;
	int number;
	mapping gift_change;

	if ( ! arg )
		return notify_fail("请再看看提示。\n");

	if ( sscanf(arg, "%s %d", arg, number)!=2 && arg != "done")
		return notify_fail("指令格式：change <属性> <数字>。\n");

	switch( arg ) {
		case "int":
		case "dex":
		case "str":
		case "con":
			if (!intp(number))
				return notify_fail("您定义的新属性，只能改成数字。\n");
			if ( number < 10)
				return notify_fail("您定义的新属性，各项不得小于十。\n");
			if ( number > 30 )
				return notify_fail("您定义的新属性，各项不得大于三十。\n");
			switch( arg ) {
				case "int":	str = "悟性";	break;
				case "dex":	str = "身法";	break;
				case "str":	str = "膂力";	break;
				case "con":	str = "根骨";	break;
			}
			me->set_temp("gift_change/"+arg, number);
			tell_object(me , "您已经定义新的 "+str+" 为 "+number+"，请在全部定义完毕后，输入 change done 结束。\n");
			break;
		case "done":
			if (! me->query_temp("gift_change/int"))
				return notify_fail("您的悟性还没有定义。\n");
			if (! me->query_temp("gift_change/con"))
				return notify_fail("您的根骨还没有定义。\n");
			if (! me->query_temp("gift_change/str"))
				return notify_fail("您的膂力还没有定义。\n");
			if (! me->query_temp("gift_change/dex"))
				return notify_fail("您的身法还没有定义。\n");

			gift_change = me->query_temp("gift_change");

			if ( gift_change["int"] +  gift_change["str"]+ gift_change["dex"]+ gift_change["con"] != 80)
				return notify_fail("您的各项属性总和必须为八十。\n");
			
			log_file("quest/GIFT", sprintf("%8s%-10s 改属性：int from %d to %d，dex from %d to %d，con from %d to %d，str from %d to %d ",
				me->query("name"), "("+me->query("id")+")",me->query("int"), gift_change["int"],me->query("dex"), gift_change["dex"],
				me->query("con"),gift_change["con"], me->query("str"),gift_change["str"]), me);

			me->set("int", gift_change["int"]);
			me->set("dex", gift_change["dex"]);
			me->set("con", gift_change["con"]);
			me->set("str", gift_change["str"]);

			if ( me->query_skill("literate", 1) > me->query("int")*10)
				me->set_skill("literate", me->query("int") * 10);

			me->delete_temp("gift_change");
			tell_object(me, HIG+BLINK"\n您的新属性已经全部调整完毕。\n"NOR+NOR"如果你觉得不满意还可以再次调整。\n");
			break;
		default:
			return notify_fail("指令格式：change <属性> <数字>。\n请再看看提示。\n");
	}
	return 1;
}

int valid_leave(object me, string dir)
{
	if(dir==WAY) {
		if(mapp(me->query_temp("gift_change")))
			return notify_fail(HIR"你是不是忘了什么啊？看清楚些。\n"NOR);
		if (!me->query_temp("confirm_out")) {
			me->set_temp("confirm_out", 1);
			me->start_busy(2);
			return notify_fail(HIR+BLINK"\n离开这里您将无法再回来调整属性，如果您同意，请再输入一次 "+WAY+"。\n\n"NOR);
		}
		me->delete_temp("confirm_out");
		me->set("oldsj",2);
		me->delete("startroom");
	}
	return ::valid_leave(me, dir);
}