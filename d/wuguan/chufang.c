//chufang.c
#include <room.h>
#include <ansi.h>
inherit ROOM;
void create()
{ 
       set("short","厨房");
       set("long", @LONG
这是武馆的厨房, 虽然不大, 但是锅碗瓢盆, 油盐酱醋, 都很齐全。灶台
上堆满了新鲜瓜果蔬菜, 活鱼活虾, 满屋的果香和肉香，使你一走进这里，闻
得阵阵的香气，就立即想饱餐一顿。有几个厨子正在忙碌着。
LONG);

        set("exits", ([         
                "east" : __DIR__"zoulang1",
        ]));             

        set("objects",([
                FOOD_D("dawancha") : 2,
                FOOD_D("rice") : 1,
                __DIR__"npc/wang1"  : 1,
        ]));

        set("no_fight", 1);

        setup();
}

int valid_leave(object me, string dir)
{
	object *ob = deep_inventory(me);
	int i = sizeof(ob);
	if( dir == "east" ){
		while (i--)
		if(ob[i]->id("tea") || ob[i]->id("rice"))
			return notify_fail(CYN"王厨子哼了一声：吃饱喝足了还要带走呀！\n"NOR);                
	}
	return ::valid_leave(me, dir);
}
