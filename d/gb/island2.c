// island2.c 小岛
// Modify by Looklove 2k/5/18

#include <ansi.h>
inherit ROOM;


void create()
{
        set("short","小岛");
        set("long", @LONG
这是一个美丽的小岛，离岸边很近，似乎游(swim)也游得回去。岸边沙滩
上布满了贝壳，还有不少螃蟹在洞里冒着泡沫。前面处不远是块大岩石，旁边
搭有一个小木棚。
LONG
        );
        set("outdoors","福州");
        set("exits",([
        "west" : __DIR__"island1",
        "east"  : __DIR__"island"+(1+random(3)),
        "north" : __DIR__"island"+(1+random(3)),
        "south" : __DIR__"island"+(1+random(3)),
        ]));
        setup();
}

void init()
{
        add_action("do_swim","swim");
        add_action("do_swim","youyong");
}

int do_swim()
{
        object me = this_player();

        if (me->is_busy()) 
                return notify_fail("你现在正忙着呢！");
        message_vision("$N一个俯冲，跳进海里，往浪里一钻就不见了。\n", me);
        tell_object(me,"你游了一会，看到离岸边不远，往铁锚一按，跃了上去。\n");
        me->move("/d/gb/yugang");
        tell_room(environment(me), me->name() + 
                "抓着岸边铁锚，噔噔噔地爬了上来。\n", me);
        return 1;
}

