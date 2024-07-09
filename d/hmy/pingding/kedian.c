// kedian.c

#include <ansi.h>
inherit ROOM;
#include "hmy_def.c"

void create()
{
        set("short", HIY"平定客店"NOR);
        set("long", @LONG       
这里就是平定客店，老板的生意非常兴隆。外地游客多选择这里落脚。店
小二里里外外忙得团团转，接待着南腔北调的客人。当你走了进来，店小二连
忙迎上前去。客栈正面的墙上挂着一块醒目的牌子(paizi)。
LONG
        );

        set("valid_startroom", 1);
        set("no_sleep_room",1);
        set("no_fight",1);

        set("item_desc", ([
                "paizi" : "楼上雅房，每夜五两白银。\n",
        ]));

        set("objects", ([
         __DIR__"npc/xiaoer" : 1,
        ]));

        set("exits", ([
                "northwest" : __DIR__"cj3",
                "north" : __DIR__"kefang",
                "up" : __DIR__"kedian2",
        ]));

        setup();
}

int valid_leave(object me, string dir)
{
        object obj;

        if ( !me->query_temp("rent_paid") && dir == "up" )
                return notify_fail("怎么着，想白住啊！\n");

        if ( dir == "west" ) {
                if(me->query_temp("rent_paid"))
                	return notify_fail("客官已经付了银子，怎么不住店就走了呢！旁人还以为小店伺候不周呢！\n");

                if (me->query_temp("hostage/job_1")==1 && !me->query_temp("hostage/passed_x")) {
                        tell_object(me,YEL"\n你正要动身，只听得长街彼端传来马蹄声响，有人大呼：“拿到风雷堂主了，拿到风雷堂主了！”\n"NOR);
                        tell_object(me,YEL"你急忙走到客店大门之后，只见数十人骑在马上，高举火把，拥着一个身材魁梧的老者疾驰而过。\n"NOR);
                        tell_object(me,CYN"\n你定睛望去，那老者就是童百雄，你急忙跟上前去，尾随而行。\n"NOR);
                        tell_object(me,YEL"\n不一会，你跟随他们来到了一处地道。\n"NOR);
                        tell_room(this_object(),me->name() + "急急忙忙离开了。\n", ({me}));
                	
			obj = find_object("/d/hmy/npc/tong.c");
	                if (!obj)
        	                new(__HMY__"npc/tong")->move(__HMY__"midao6");
			else
				obj->move(__HMY__"midao6");
                	me->move(__HMY__"midao0");
                	return 1;
                }
        }

        return ::valid_leave(me, dir);
}
