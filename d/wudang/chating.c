// /d/wudang/chating.c 茶亭

#include <ansi.h>
inherit ROOM;

void create()
{
	set("short", "茶亭");
	set("long", @LONG
这是三叉路口旁边而建的茶亭，喝着香茶，还能欣赏远处的风景，这真所
谓偷得浮生半日闲！没有比这更好的歇脚方法了。亭里三五成群的游人围着桌
子(table)，坐在椅子(chair)上，有的闲聊，有的乘凉，好不逍遥自在。
LONG
	);

	set("exits", ([
		"northwest" : __DIR__"wdroad8",
	]));

	set("resource/water", 1);
	set("drink_msg", "$N端起杯香茶，有滋有味地品了几口。\n");

	set("item_desc", ([
		"table" : "一张典雅的桃木小桌，上面放着水果盘和饮茶器具。\n",
		"chair" : "一只青竹打制的靠椅，躺上去摇摇晃晃，好舒服耶！\n",
	]));

	set("outdoors", "武当山");
	set("objects",([
		FOOD_D("mitao") : 1,
		FOOD_D("cha") : 1,
		__DIR__"npc/taohua" : 1,
	]));

	setup();
}

void init()
{
	add_action("do_tap", "tap");
	add_action("do_tap", "knock");
	add_action("do_sit", "sit");
}

int do_tap(string arg)
{
	object me;
	object cui;

	if (!arg || (arg != "desk" && arg != "table"))
		return notify_fail("你要敲什么？\n");

	me = this_player();

	if( !objectp(cui = present("tao hua", environment(me)))) {
		write("你敲了敲桌子，却还是没人理你。你突然感觉一陈失落。\n");
		return 1;
	}

	if( !me->query_temp("marks/sit")) {
		write("你敲了敲桌子，所有的人都回头愕然地看着你，你突然感觉自己很愚蠢。\n");
		return 1;
	}

	if( me->query_temp("marks/served")){
		message_vision("桃花不耐烦地对$N说道：刚给你上过茶你接着就要，你是个饭桶不是啊！\n", me);
		return 1;
	}

	if (present("mi tao", environment(me)) && present("dawan cha", environment(me))){
		write("你先把外面有的吃了吧！\n");
		return 1;
	}

	message_vision("$N端坐在桌前，轻轻扣了下桌面，桃花一甩小辫，过来招呼。\n", me);

	cui->serve_tea(me) ;

	me->set_temp("marks/served", 1);
//  remove_call_out("delete_served");
	call_out("delete_served", 10, me);

	return 1;
}

void delete_served(object me)
{
	if ( objectp(me) ) me->delete_temp("marks/served");
}

int do_sit(string arg)
{
	if ( !arg || (arg != "chair") )
		return notify_fail("你要坐什么上面？\n");

	if (this_player()->query_temp("marks/sit"))
		return notify_fail("你已经有了个座位了。\n");

	this_player()->set_temp("marks/sit", 1);
	return notify_fail("你找了个空位座下，等着上茶。\n");
}

/*
int valid_leave(object me, string dir)
{
	object *ob = deep_inventory(me);
	int i = sizeof(ob);
	if((dir == "northwest")){
		while (i--)
		if(ob[i]->id("mi tao") || ob[i]->id("xiang cha")){
			if(objectp(present("tao hua", environment(me))))
				return notify_fail("桃花把嘴一撇：吃饱了喝足了还不够，临走怀里还揣上一些，小女子也替您脸红呢！\n");
		else
			return notify_fail("吃饱了喝足了还不够，临走怀里还揣上一些，您不觉得脸红？\n");
		}
	}
	me->delete_temp("marks/sit");
	me->delete_temp("tea_cup");
	return ::valid_leave(me, dir);
}
*/