// Room: /d/mr/yanziwu.c
// this is made by beyond
// update 1997.6.20
// Modify by Lklv 2001.9.21

#include <ansi.h>
#include <room.h>
inherit ROOM;
void create()
{
	set("short",WHT"院门"NOR);
	set ("long",@long
燕子坞是幕容氏世居之地, 幕容氏乃夕日大燕国的后裔, 后来日见式微，
但其弟子却一心一意想光复大燕。其武功见长于以彼之道, 还施彼身, 端的十
分厉害。
long);
	set("exits",([
		"south" : __DIR__"xiaojing2",
		"west" : __DIR__"houyuan1",
		"east" : __DIR__"wuchang",
	]));

	set("objects",([
		__DIR__"../npc/murong-fu" : 1,
	]));

	"/clone/board/murong_b.c"->foo();
	setup();
}

void init()
{
	if (this_player()->query("mr_job")){
		this_player()->add("job_time/慕容", this_player()->query("mr_job"));
		this_player()->delete("mr_job");
	}
	
	if(!this_player()->query("change_bug/fenglu")){
		this_player()->delete("fenglu");
		this_player()->set("change_bug/fenglu",1);
	}
}
