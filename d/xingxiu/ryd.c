// Room: /d/xingxiu/ryd.c
// Update by caiji@SJ 8/25/2000

#include <ansi.h>
inherit ROOM;

void create()
{
	set("short", HIY"日月洞"NOR);
	set("long", @LONG
这里是星宿派总舵所在地日月洞。洞内灯火通明，正中一把虎皮椅上端坐
着星宿派掌门人丁春秋。两边各站列着数十位星宿派弟子，不断有各派人士走
进来献(xian)上各种宝物。一片歌功颂德之声，洋洋盈耳，丝竹箫管也跟着吹
奏。丁春秋每听到得意之时，禁不住挥动手中的羽扇，捋着白须，眯起了双眼，
薰薰然，飘飘然，有如饱醉醇酒。洞壁上装饰着虎皮、鹿头、雕羽，及一些字
画 (zihua)。
LONG
        );
        set("exits", ([
		"north" : __DIR__"yaolu",
		"south" : __DIR__"ryd1",
        ]));
        set("item_desc", ([
                "zihua" : "都是些吹捧星宿老仙的诗词和他的画像。\n

               ※※※※※※※※※※※※※※※※※
               ※※※※　　　　　　　　　※※※※
               ※※※※　古　威　德　星　※※※※
               ※※※※　今　震　配　宿　※※※※
               ※※※※　无　寰　天　老　※※※※
               ※※※※　比　宇　地　仙　※※※※
               ※※※※　　　　　　　　　※※※※
               ※※※※※※※※※※※※※※※※※\n"]));

	set("objects", ([
                __DIR__"npc/ding"  : 1,
                __DIR__"npc/zhaixing"  : 1,
                __DIR__"obj/huoyan"  : 1,
        ]));
        setup();
}

void init()
{
        object me = this_player();

	if ( me->query("xx_points")){
		me->add("job_time/星宿", me->query("xx_points"));
		me->delete("xx_points");
	}
	if ( me->query("xx_job5")){
		me->add("job_time/星宿叛徒", me->query("xx_job5"));
		me->delete("xx_job5");
	}
	if ( me->query("xx_job4")){
		me->add("job_time/星宿熬膏", me->query("xx_job4"));
		me->delete("xx_job4");
	}
}

int valid_leave(object me, string dir)
{
	if ( dir == "north" && !userp(me))
		return 0;
	if(dir == "north"
	 && me->query("family/family_name") != "星宿派"
	 && present("ding chunqiu", environment(me))){
		return notify_fail("丁春秋拦住你道：我星宿派的药庐重地，怎么能允许你进去！\n");
	}
	return ::valid_leave(me, dir);
}
