// /d/city/kedian.c

inherit ROOM;
#define WEI_FILE "/d/city/npc/weixiaobao"
#include "cgift.h"

void create()
{
	set("short", "宝昌客栈");
	set("long", @LONG
这里是生意兴隆的宝昌客栈，附近的外地游客多选择在此落脚。一个年轻
的店小二里里外外忙得团团转，接待着南腔北调的客人。客栈北面的墙上挂着
一块醒目的招牌(zhaopai)，南面的墙上挂有一张告示(gaoshi)。
LONG
	);

	set("valid_startroom", 1);
        set("no_sleep_room",1);
	set("no_fight",1);
	set("item_desc", ([
		"paizi" : "楼上雅房，每夜五两白银。\n",
		"zhaopai" : "楼上雅房，每夜五两白银。\n",
		"gaoshi" : (: help() :),
	]));

	set("objects", ([
		__DIR__"npc/xiaoer" : 1,
		"/clone/npc/xu" :1,
		"/clone/npc/tong" :1,
		"/clone/npc/zha" :1,
		//"/clone/npc/gold_npc" :1,
		
		NPC_D("jianke1") : 1,
	]));

	set("exits", ([
		"east" : __DIR__"kedian/pianting",
		"west" : __DIR__"beidajie2",
		"up" : __DIR__"kedian2",
	]));
	set("incity",1);
	setup();
	"/clone/board/kedian_b"->foo();
}

void init()
{
        object *obs = filter_array(children(WEI_FILE), (: clonep :));
        object ob;
        
        if (sizeof(obs) < 1)  
        {
                ob = new(WEI_FILE);
                if (!objectp(ob)) return;
                ob->start_weapon();
                ob->move_other("");
        }
        return;
}


int valid_leave(object me, string dir)
{
	if ((string)me->query_temp("rent_paid") != "宝昌客栈" && dir == "up" )
		return notify_fail("怎么着，想白住我们宝昌客栈啊！\n");
	if ((string)me->query_temp("rent_paid") == "宝昌客栈" && dir == "west" )
		return notify_fail("客官已经付了银子，怎么不住店就走了呢！旁人还以为我们宝昌客栈伺候不周呢！\n");

	return ::valid_leave(me, dir);
}
