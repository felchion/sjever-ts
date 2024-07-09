// xiaolu2.c 林间小径
#include <ansi.h>
#include <room.h>
inherit ROOM;

void create()
{
        set("short", WHT"院门"NOR);
        set("long", @LONG
你走在一条小径上，两旁种满了竹子，修篁森森，绿荫满地，除了竹叶声
和鸟鸣声，听不到别的动静。南面似乎有一座简陋的小院。
LONG
        );

        set("exits", ([
                "south" : __DIR__"houyuan.c",
                "north" : __DIR__"xiaolu2",
        ]));
        
        set("outdoors", "武当");
	set("cant_hubiao", 1);

        set("objects",([
                CLASS_D("wudang")+ "/yinlt" : 1,
        ]));

        create_door("south", "竹门", "north", DOOR_CLOSED);
        setup();
}

int get_object(object ob)
{
        return (userp(ob));
}

int valid_leave(object me, string dir)
{
	object obj;
        object *ob, *inv;
        mapping myfam;
        myfam =( mapping)me->query("family");

        if ((!myfam || myfam["family_name"] != "武当派")
         &&  objectp(obj =present("yin liting", environment(me)))
         && dir == "south"
         && living(obj))
           return notify_fail(CYN"殷梨亭拦住去路，抱拳道：我师傅正在闭关，没事不得打扰。\n"NOR);

        if ((int)me->query("shen") < 0
         && me->query("family/family_name") == "武当派"
         && objectp(obj =present("yin liting", environment(me)))
         && dir == "south"
         && living(obj))
           return notify_fail(CYN"殷梨亭把手一拦，劝道：师傅他老人家最讨厌善恶不分之人，你还是去多多做些侠义之事吧！\n"NOR);

        if((int)me->query_skill("yinyun-ziqi", 1) < 80
         && objectp(obj = present("yin liting", environment(me)))
         && dir == "south"
         && living(obj))
           return notify_fail(CYN"殷梨亭把手一拦,劝道：倘若没要紧的事,不要防碍师傅他老人家清修。\n"NOR);

	inv = deep_inventory(me);
	if (dir == "north"){
		write("北面是一条小径，两旁种满了竹子，修篁森森，绿荫满地。\n");
		ob = filter_array(inv,(:get_object:));        
		if (me->query_condition("killer") && me->query("family/family_name") != "武当派") ;
		else if (myfam && myfam["family_name"] =="武当派" && myfam["generation"] == 2 && ! sizeof(ob))
			write("由于你走惯了这条小径，所以信步走出了竹林。\n");
		else {
			tell_room(environment(me), me->name()+"往北面的"YEL"小径"NOR"快步离开。\n"NOR, ({ me }));
			me->move(__DIR__"xiaolu6");
			tell_room(environment(me), me->name()+"从"WHT"院门"NOR"快步走了过来。\n"NOR, ({ me }));
			return notify_fail("");
		}
	}
	return ::valid_leave(me, dir);
}
