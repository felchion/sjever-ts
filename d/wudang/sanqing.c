// /d/wudang/sanqingdan.c  三清殿
// by ydh
#include <ansi.h>
inherit ROOM;

void create()
{
        set("short",HIY"三清殿"NOR);
        set("long", @LONG
这里是紫霄宫的三清殿，是武当派会客的地点。供着道德天尊、元始天尊
和灵宝天尊的神像。正中间是个敬香的大香案，靠墙放着几张太师椅，地上放
着几个蒲团。北边是练武的地方，南边是武当禁地。
LONG                           
        );
        set("exits", ([
                "south" : __DIR__"xiaolu1",
                "out" :__DIR__"zhenqing",
                "north" :__DIR__"zoulang1",
        ]));

        set("objects",([
                CLASS_D("wudang") + "/song" : 1,"/clone/board/wudang_b":1,
        ]));

	set("valid_startroom", 1);
 
        setup();
       // "/clone/board/wudang_b"->foo();
}

void init()
{
	object me = this_player();
	int i;
	
	i = me->query("wd_job");
	if ( i > 0 ) {
		me->add("job_time/武当", i);
		me->delete("wd_job");
	}
}
