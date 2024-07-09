
#include <ansi.h>

inherit ROOM;
#include "/d/suzhou/npc/lchj.h";
int do_chanhui(string arg);
int do_jump(string arg);

void create()
{
        set("short", HIW"岩石"NOR);
        set("long", @LONG
这里是群山环绕中的一个山谷，人迹罕至。仰面向上看，四面山峰高耸入云，
覆盖着冰雪。这里正巧一块巨大的岩石，遮蔽了许多风雪。
LONG
        );
	set("objects", ([
		__DIR__"npc/diyun" : 1,
		//__DIR__"npc/shuisheng" : 1,
	]));
	set("outdoors", "雪山");
	setup();
}

void init()
{
	object me = this_player();
	if ( me->query("xs_job") > 0) {
		me->add("job_time/雪山", me->query("xs_job"));
		me->delete("xs_job");
	}
	add_action("do_jump", ({"jump", "tiao","yue"}));
	add_action("do_chanhui", ({"mianbi", "chanhui"}));
}
int do_jump(string arg)
{
	object room,me = this_player();
	object obj;
    if( arg == "up")
    {
		if(me->query_skill("dodge")<150) 
		{
			message_vision(HIC"\n$N吸了一口气，尝试跳上雪谷，却突然发现风大山高，却退了回来。\n"NOR,me);
			return 1;
		}
		if (!(room = find_object(__DIR__"xuegu")))
			room = load_object(__DIR__"xuegu");
		if(!room)
		{ 
			tell_object(me,HIR"\n你无论怎么跳，发现都还在原地!\n");
			log_file("quest/LCHJ", sprintf("%s(%s)缺少xuegu.c文件。\n", me->name(1),me->query("id")) );	
		}
		else
		{
			message_vision(HIC"\n$N看准一块半空一块岩石，吸了一口气，突然高高跃起。\n"NOR, me);
			obj=present("di yun",this_object());
			if(!me->query_temp("quest/outxuegu")|| !obj) 
			{
				message_vision(HIC"$N突然感觉真气不足，急忙跃到一边的岩石上。\n"NOR, me);
				tell_object(me,HIY"你试图离开雪谷，但是内力不继，无法成功。\n"NOR);                       
			}
			else
			{
				message_vision(HIC"$N正真气不足，背后传来一股大力，$N借势跃起，跃到半空的岩石，随即窜上雪谷。\n"NOR, me);
				me->move(room);
				me->delete_temp("quest/outxuegu");//
				me->start_busy(1);
                                tell_room(environment(me),HIC"\n只见一个身影飞身而过，原来是"+me->name()+"从雪谷下跳上来。\n"NOR, ({me}));                       
			}
		}
		return 1;
	}
	return 0;
}
int do_chanhui(string arg)
{
	object me = this_player();
	object obj;
	if(arg) return 0;
	obj=present("di yun",this_object());
    if(!obj) return 0;
	
	message_vision(HIY"\n$N想了想过去所作所为，不禁为自己曾经的恶行而羞愧。\n"NOR,me);
	if(me->query("shen")>0) 
	{
		message_vision(HIG"$N拍了拍$n的头道：“人在江湖，身不由己，你已经不错了。”\n"NOR,obj,me);
		return 1;
	}
	if(random(2)) message_vision(HIG"$N缓缓道：“$n真有悔改，也算改过自新。以后兴侠仗义，也是功德无量。”\n"NOR,obj,me);
	else message_vision(HIG"$N对$n点了点头。\n"NOR,obj,me);
        me->start_busy(1+random(3));
	message_vision(HIG"$N拍了拍$n的头，向$n笑了一下。\n"NOR,obj,me);
	tell_object(me,HIY"你默默为自己的行为忏悔，感觉心中一股侠义之气升起。\n"NOR);                       
	me->add("shen",60+random(140));
	return 1;
}
