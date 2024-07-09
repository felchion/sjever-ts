// shanpo.c 
// By River 98.08.29
inherit ROOM;
#include <ansi.h>

void create()
{
        set("short", HIC"山坡"NOR);
        set("long",@long
此处的山路显得稍有陡峭，四周杂草丛生，看上去危机四伏。由于江湖上
传闻古墓以前叛逆妄想前来偷取古墓秘籍，所以古墓弟子加强了巡防。一些古
墓弟子正在山坡附近巡逻。
long);

        set("outdoors","古墓");

        set("exits",([
          "westdown" : __DIR__"guoyuan",
          "northeast" : __DIR__"shanpo1",        
        ]));

        setup();
}


void init()
{        
       add_action("do_xunluo","xunluo");
}

int do_xunluo()
{
        mapping fam;
        object me=this_player();
        if (!living(me)) return 0;
        if (!(fam = me->query("family")) || fam["family_name"] != "古墓派")
           return notify_fail("你不是古墓传人，如何能在古墓巡逻？\n");
        if (me->is_busy() || me->is_fighting())
           return notify_fail("你正忙着呢。\n");
        if (me->query_temp("gm/job"))
           return notify_fail("你不是已经在巡逻了么？\n");
        if ( me->query_condition("gm_job" ) > 0 )
           return notify_fail("你挑眼四望，发现周围一片安静，好象没有什么危险。\n");
        if ( me->query("combat_exp") < 30000)
           return notify_fail("你的经验不够，还不能担当巡逻的重任。\n");
        if (me->query("combat_exp") > 200000 )
           return notify_fail("你武功已经够高，巡逻的事就有小弟子们去操办吧。\n");
        message_vision(HIY"$N警惕地四周打量着，突然发现东南方向有一些动静。\n"NOR,me);
        me->apply_condition("gm_job", 5+random(5)); 
        me->set_temp("gm/job", 1);
        return 1;
}
