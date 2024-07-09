#include <ansi.h>
inherit ROOM;

void create()
{
        set("short", "断崖");
        set("long", @LONG
前面是一处十分陡峭的断崖，险峻异常，难以爬越(pa)。
LONG
        );

        set("exits", ([ 
          "north" : "/d/shaolin/yidao4",
]));

        set("coor/x",110);
  set("coor/y",90);
   set("coor/z",0);
   set("coor/x",110);
  set("coor/y",90);
   set("coor/z",0);
   setup();
}
void init()
{
        add_action("do_pa", ({ "climb", "pa" }));
} 

int do_pa(string arg)
{
       object me;
       me = this_player(); 

        if (arg != "ya" && arg!="up") 
          return notify_fail("你要往那里爬？\n");
         if ( !arg ) return 0;

        
        if (!living(me)) return 0;
        if ((int)me->query_encumbrance() * 100 / (int)me->query_max_encumbrance() >= 30
|| !me->query_temp("gb_job2") )
         {
                message_vision("$N气喘嘘嘘，感到无法爬上去，摔了下来！\n", me);
		me->receive_wound("qi", 50+random(50));
		me->set_temp("last_damage_from", "从断崖上掉下来摔");
                return 1;
        } 

        if( (int)me->query_skill("dodge",1) < 80 ) 
        {  
		message_vision(HIR"$N想要爬上山顶，无奈轻功不够，摔了下来！\n"NOR, me);
		me->receive_wound("qi", 50+random(50));
		me->set_temp("last_damage_from", "从断崖上掉下来摔");
		return 1;     
        }  
        message_vision("$N扒住突出的岩石，施展轻功爬了上去。\n", me);
        me->move(__DIR__"yading");
        tell_room(environment(me), me->name() + "从下面爬了上来。\n", ({ me }));
        return 1;
}
