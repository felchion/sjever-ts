// by xiaoyao

#include <room.h>

inherit ROOM;
void create()
{
        set("short", "石洞");
        set("long", @LONG
这是沙漠中天然形成的一个石洞，洞中光线昏暗，但却十分干燥，
地下铺者几堆稻草，似乎是供人睡卧之用，静心凝听，洞中仿佛有阵阵
水声。洞的一角放着数堆人头骨(skulls)。
LONG );
        set("exits", ([
                "out" : __DIR__"shamo1",
        ]));
        set("item_desc", ([
                "skulls" : "一堆整整齐齐的骷髅头，上一中三下五，不多不少，恰是九颗白骨骷髅头。\n",
        ]));
        set("objects", ([
                __DIR__"npc/mei" :1,
                __DIR__"npc/chen" :1,
        ]));

        set("coor/x",100);
  set("coor/y",120);
   set("coor/z",10);
   setup();
}

void init()
{
        object me;
        me = this_player();
        if( me->query("jiuyin-xj/chenfail") > 0 ){
         me->add("jiuyin/chenfail",me->query("jiuyin-xj/chenfail"));
         me->delete("jiuyin-xj/chenfail");
        }
        if(( me->query("jiuyin/gumu")
         || me->query("jiuyin/emei"))
         && me->query("jiuyin/chenfail") >= 3){
         me->set("jiuyin/chenfail", 2);
        }
        return;
}
