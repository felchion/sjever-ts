// ROOM : /u/jpei/thd/jicui.c
// Modified by Numa 1990828

#include <ansi.h>
#include <room.h>

inherit ROOM;

void create()
{
        set("short", GRN"积翠亭"NOR);
        set("long", @LONG
这是座竹枝搭成的凉亭，亭中放着竹台竹椅，全是多年之物，用得
久了，现出淡淡黄光，两旁悬着一副对联(duilian)。东面不远就是招
待上岛客人的客房，南北两面都是郁郁葱葱的绿竹林。
LONG
        );
        set("exits",([
                "north" : __DIR__"zhulin12",
                "south" : __DIR__"zhulin21",
                "east" : __DIR__"kefang",
                "enter" : __DIR__"room",
        ]) );
//set("no_clean_up",0);
        set("outdoors","桃花岛");
        set("item_desc",([
                "duilian" : HIY"
\t※※※※              ※※※※
\t※    ※              ※    ※
\t※ 桃 ※              ※ 碧 ※
\t※ 花 ※              ※ 海 ※
\t※ 影 ※              ※ 潮 ※
\t※ 落 ※              ※ 生 ※
\t※ 飞 ※              ※ 按 ※
\t※ 神 ※              ※ 玉 ※
\t※ 剑 ※              ※ 箫 ※
\t※    ※              ※    ※ 
\t※※※※              ※※※※\n 
"NOR
        ]));
        set("objects",([
                __DIR__"npc/yaoshi.c" : 1,
        ]) ); 

        setup();
}

void init()
{
	object me = this_player();
	int lvl,lvl1,lvl2;
	lvl = me->query_skill("qimen-bagua", 1);
	lvl1 = me->query_skill("bihai-chaosheng", 1);
	if (lvl > 300 && !me->query("cut/thd") && userp(me)){
		lvl2 = (lvl - 300)/ 4;
		me->set_skill("qimen-bagua", 300);
		me->set_skill("bihai-chaosheng", lvl1 + lvl2);
		me->set("cut/thd", lvl2 * 4);
		write(HIG "系统已将您多余的「奇门八卦」转换为「碧海潮生功」。\n" NOR);
	}
	if ( me->query("thd_job")){
		me->add("job_time/桃花岛", me->query("thd_job"));
		me->delete("thd_job");
	}
}

int valid_leave(object me, string dir)
{
	object *ob, *deep_ob;
	int i, j, drop_flag;

	if (dir == "north" || dir == "east") {
		ob = all_inventory(me);
		drop_flag = 0;
		for(i = 0; i < sizeof(ob); i++) {
			if (userp(ob[i])) {
				if (!drop_flag) {
					message_vision("$N看到黄药师，把背负的人放下，向黄药师行了个礼。\n", me);
					drop_flag = 1;
				}
				message_vision("$N将$n从背上放了下来，躺在地上。\n", me, ob[i]);
				ob[i]->move(this_object());
			}
			else {
				deep_ob = deep_inventory(ob[i]);
				for(j = 0; j < sizeof(deep_ob); j++) {
					if (userp(deep_ob[j])) {
						if (!drop_flag) {
							message_vision("$N看到黄药师，把身上的笨重物品丢下，向黄药师行了个礼。\n", me);
							drop_flag = 1;
						}
						message_vision(sprintf("$N丢下一%s$n。\n", ob[i]->query("unit")), me, ob[i]);
						ob[i]->move(this_object());
						break;
					}
				}
			}
		}
	}
        if( dir =="enter" && objectp(present("huang yaoshi", environment(me)))) 
		return notify_fail("黄药师把手一伸，拦住你的去路，笑道：没有我的同意无人能进入箫房。\n");
	return ::valid_leave(me, dir);
}
