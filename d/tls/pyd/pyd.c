//普云洞
//cool@SJ,990605

#include <ansi.h>
#include <room.h>
inherit ROOM;
void create()
{
         set("short",HIY"普云洞"NOR);
         set("long",
"原来这普云阁乃是一个小山洞，一进洞，门就无声无息的关闭了，眼前是一个天然形成的石甬道，
洞的顶上有一条狭长的石缝，光线来自那里，这条道周围分布着几个山洞，隐隐有些光从洞中透出。\n"
);
         
set("exits", ([
               "out" : __DIR__"lsy",
               "north" : __DIR__"shangyang",
               "northup" : __DIR__"shaoshang",
               "northwest" : __DIR__"guanchong",
               "northeast" : __DIR__"zhongchong",
               "southwest" : __DIR__"shaoze",
               "southeast" : __DIR__"shaochong",
        ]));
       
setup();
}

