// Room: /d/huashan/jinshe/dxue.c
// Date: Look 99/03/25

#include <ansi.h>
inherit ROOM;

void create()
{
	set("short","洞底");
	set("long", @LONG
借着火折光亮，你看到对面石壁上斜倚着一副骷髅骨(haigu )，正前面横 
七竖八放着十几把金蛇锥。石壁上有着几百幅各样图形，每个人形状均不相同，
举手踢足似在练武。图形尽处，石壁上出现了几行字(zi)，也是以利器所刻制。
有把剑柄(sword)凸出在石壁之上。 
LONG                           
        );
	set("valid_startroom", 0);

	set("he_count",1);
	set("sword_count", 1);
	set("xia_count",1);
       
	set("item_desc", ([
		"haigu" : "身上衣服已烂了七八成，宛然尚可见到是个人形。\n",
		"zi": "重宝秘术，付与有缘，入我门来，遇祸莫怨。\n",
		"sword":"一把宝剑直劈入石壁之中，只剩金闪闪的剑柄。\n",
	]));
	set("objects",([
		__DIR__"obj/haigu" : 1,
//                BINGQI_D("throwing/jinshe-zhui") : 1,
	]));
	setup();
}

void init()
{
	object me = this_player();
	add_action("do_zang", ({"zang","zang"}));
	add_action("do_shenzang", ({"shenzang","shenzang"}));
	add_action("do_ba", ({"pull","ba"}));
	add_action("do_pa", ({ "climb", "pa" }));
	me->set("startroom", "/d/city/guangchang");
}
 
int do_ba(string arg)  
{
	object sword,me;
	me = this_player();
	if ( !arg || arg != "bing" )
		return notify_fail("你要拔什麽？\n");
	if((int)me->query_str() < 30)
		return notify_fail("你用了半天力气，也无法拔出宝剑。\n");    
	if( (int)me->query("neili") < 2000  ) 
		return notify_fail("你握住剑柄，用力外拔，可是宝剑纹丝不动。\n");
	if(random(10)!=6) {
		me->add("neili",-200);
			return notify_fail("你抓住剑柄，微一运力，只觉得剑柄稍微活动了一下。\n"); 
	}
	message_vision("$N紧紧握住剑柄，潜运内力，嗤的一声响，把剑拔了出来。\n", me);
	me->add("neili", -1000);
	me->set_temp("mark/sword",1);
	sword = unew(BINGQI_D("sword/jinshesword"));
	if(!clonep(sword)){ 
		message_vision("结果$N拔出了把生锈的铁剑来。铁剑掉在地上，摔得粉碎了。\n", me);
		return 1;
	}
	sword->move(me);       
	return 1;
}

int do_shenzang(string arg)
{ 
	object xia;
	object me;
	me = this_player();
	if (!( present("tie chu", this_player())))
		return notify_fail("太深了，你很难用手挖！\n");
	if ( !arg || arg != "haigu" )
		return notify_fail("你要深葬谁？\n");
	if (!(int)me->query_temp("mark/zang"))
		return notify_fail("你要做什麽？\n");
	if(random(10)!=3) {
		me->add("jingli",-200);
		return notify_fail("你把坑又挖了深许，也不知够不够。\n"); 
	}
//         if(random((int)me->query("kar",1)/4)<5 )
//        return notify_fail("你把坑又挖了深许，也不知够不够。\n"); 
//        me->add("jingli",-200);
	if (query("xia_count") > 0) {
		message_vision("$N你又把坑挖深了几尺，埋葬了骷髅，突然看到一个铁匣(tiexia)。\n",me);

		add("xia_count", -1);
		xia=new(__DIR__"obj/tiexia");
		xia->move(__DIR__"dxue",);
		me->set_temp("mark/shenzang",1);
		me->add("jingli",-200);
		destruct(present("skeleton", this_object()));
	}
	else
		message_vision("$N费力的挖了挖，显然已经被人挖过了。\n", me);
	return 1;
}

int do_zang(string arg)
{      
	object he;
	object me;
	me = this_player();
	if (!( present("tie chu", this_player())))
		return notify_fail("你很难用手挖！？\n");
	if ( !arg || arg != "haigu" )
		return notify_fail("你要埋葬谁？\n");
	if (query("he_count") > 0) {
		message_vision("$N你心下恻然，不禁想安葬这个骷髅，于是抡起锄头，\n"+
				"向地上挖去。没挖几下，突然看到一个大铁合(tiehe)。\n", me);
		add("he_count", -1);
		he=new(__DIR__"obj/datiehe");
		me->set_temp("mark/zang",1);
		me->add("jingli", -200);
		he->move(__DIR__"dxue",);
	}
	else
		message_vision("$N费力的挖了挖，这里似乎已经被人挖过了。\n", me);
	return 1;
}
 
int do_pa(string arg)
{
	object me;

	me=this_player();
	if ( !arg || arg != "up" )
		return notify_fail("你要去向哪？\n");
	if (!( present("kuang", this_player())))
		return notify_fail("你用什麽爬上去啊？\n");
	message_vision("$N抓住藤绳，微一提气，爬了上去。\n", me);
	me->move(__DIR__"biding");
	tell_room(environment(me), me->name() + "抓住藤绳,微一提气,爬了上去。\n", ({ me }));
	me->delete_temp("mark/shenzang");
	return 1;
}
