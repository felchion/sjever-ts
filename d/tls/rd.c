#include <ansi.h>
#include <room.h>

inherit ROOM;

void create()
{
        set("short", HIW"白石路"NOR);
        set("long", @LONG
走进大门，只见红砖围墙，白石铺路，从这里一直通往天王殿，抬头
望去，寺中亭台楼阁规模宏大，构筑精丽，宅地连云，其气势之盛更胜于
五台，普陀等诸处佛门胜地的名山大寺。路旁立满了无量功德碑，两边各
有一小院，院中满种着松竹。
LONG);
        set("outdoors", "天龙寺");
        set("exits", ([             
               "northup" : __DIR__"twd",
               "west" : __DIR__"yz1",
               "east" : __DIR__"yz2",
               "out" : __DIR__"gate",
        ]));
set("objects",([
                __DIR__"npc/liao-huo" : 1,
                __DIR__"npc/wuseng" : 2,
                __DIR__"npc/guest" : 1,
                ]));

        setup();
        call_other("/clone/board/tianlong_b", "???");
}

int valid_leave(object me, string dir)
{
        mapping fam;
        fam = (mapping)me->query("family");

        if ( fam && fam["family_name"] != "天龙寺" &&
         (dir == "west" || dir == "east" || dir == "northup")&&
                objectp(present("liaohuo chanshi", environment(me))))
                return notify_fail("了惑禅师拦住你，说道：天龙寺内不得随意进出。\n");

        if (fam && fam["family_name"] != "天龙寺" &&
         (dir == "west" || dir == "east" || dir == "northup")&&
                objectp(present("wu seng", environment(me))))
                return notify_fail("武僧拦住你，说道：天龙寺内不得随意进出。\n");

                  return ::valid_leave(me, dir);
}
