// Room: /d/huashan/zhengqi.c
// Date: Look 99/03/25

inherit ROOM;
#include <ansi.h>
void create()
{
	set("short", "正气堂");
	set("long", @LONG
这是华山掌门岳不群的居所「有所不为轩」，此处是前厅，厅上挂着「正
气堂」三字匾额。厅中很宽敞，却没什么摆设，靠墙放着两把交椅，这里是岳
不群议事、授徒的地方。
LONG
	);

	set("valid_startroom", 1);
	set("no_sleep_room",1);
	set("objects",([
		__DIR__"npc/yuebuqun" : 1,
		__DIR__"npc/ningzhongze" : 1,
	]));
	set("exits", ([
		"north" : __DIR__"qianting",
		"south" : __DIR__"houtang",
	]));

	set("indoors", "华山" );
	setup();
}

void init()
{       
	object me = this_player();

	if ( me->query("hs_job")){
		me->add("job_time/华山", me->query("hs_job"));
		me->delete("hs_job");
	}
}
