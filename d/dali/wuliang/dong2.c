// dong2.c
// bbb 1997/07/16
//COOL@SJ,9808
//tangfeng@SJ,2004

#include <ansi.h>
inherit ROOM;

#define QUESTDIR "quest/天龙八部/"
#define QUESTDIR1 "quest/天龙八部/凌波微步篇/"
#define QUESTDIR2 "quest/天龙八部/天龙营救篇/"
#define QUESTDIR3 "quest/天龙八部/萧峰身世篇/"
#define QUESTDIR4 "quest/天龙八部/大辽救援篇/"
#define QUESTDIR5 "quest/天龙八部/复兴天下篇/"
#define QUESTDIR6 "quest/天龙八部/武功/"

string look_yuxiang();
string look_zi();
string look_shoe();
string look_left();
string look_right();
void dy(object me);

void create()
{
	set("short", "内室");
	set("long", @LONG
眼前突然一亮，一个宫装美女，手持长剑，剑尖对准了你的胸膛。细看之
下，原来是一座玉像(yuxiang)。东壁写了许多字(zi)，笔法飘逸，似以极强
腕力以利器写成，西壁镶了六块大水晶，水晶外绿水隐隐，映得石室中比第一
间石室亮了数倍。
LONG
);

	set("exits", ([
		"out" : __DIR__"dong1",
	]));

	set("valid_startroom",1);

	set("item_desc",([
		"left"		:	(: look_left :),
		"right"	:		(: look_right :),
		"zi"		:	(: look_zi :),
		"yuxiang"	:	(: look_yuxiang :),

	]));

	setup();
}


void init()
{
	object me;
	me = this_player();

	//通缉犯不可逗留；不过通缉犯能来这里，肯定是巫师帮忙
	if(me->query_condition("killer"))
	{
		message_vision(HIC"\n突然，不知道从哪里传来一个声音道：这里不欢迎$N。\n"NOR,me);
		tell_object(me,HIR"跟着，不知道哪里来的一只神脚飞起一脚，将你一脚踢飞出去！\n"NOR);
		tell_room(environment(me),HIR"\n不知道哪里来的一只神脚飞起一脚，将"+ me->name()+ "踢飞出去！\n"NOR, ({ me }));
		if(random(2)) me->move("/d/dali/wuliang/xiaoxi");
		else me->move("/d/xiangyang/conglin3");
		tell_room(environment(me), HIR"\n只见"+ me->name()+ "大叫一声飞过来，摔了个狗吃屎！好帅哦~~~~~~~~\n"NOR, ({ me }));
		me->start_busy(1);
		return;
	}
	if (me->query(QUESTDIR1+"start")
	&& !me->query(QUESTDIR1+"wuliangover")
	&& me->query_temp(QUESTDIR1+"jianying")
	&& !me->query_temp(QUESTDIR1+"see_duanyu1"))
	{
		me->start_busy(2);
		remove_call_out("dy");
		call_out("dy",1,me);
	}
	add_action("do_ketou", "ketou");
	add_action("do_ketou", "kneel");
}
void dy(object me)
{
	object obj;
	if(!me) return;
	if(!present(me,this_object())) return;
	if(present("duan yu",this_object()))
	{
		obj=present("duan yu",this_object());
		destruct(obj);
	}
	if (me->query(QUESTDIR1+"start") && !me->query(QUESTDIR1+"wuliangover")&& me->query_temp(QUESTDIR1+"jianying") && !me->query_temp(QUESTDIR1+"see_duanyu1"))
	{
		tell_object(me,HIG"\n你伸手推门，眼前陡然一亮，失声惊呼：“啊哟！”眼前一个宫装美女，手持长剑，剑尖对准了他胸膛。你定睛看时，见这女子虽\n"
		"是仪态万方，却似并非活人，大着胆子再行细看，才瞧出乃是一座白玉雕成的玉像。这玉像与生人一般大小，身上一件淡黄色绸衫\n"
		"微微颤动；更奇的是一对眸子莹然有光，神彩飞扬。更奇怪的是一对眼珠乃以黑宝石雕成，只觉越看越深，眼里隐隐有光彩流转。\n"
		"这玉像所以似极了活人，主因当在眼光灵动之故。 \n"NOR);
		obj=new(__DIR__"npc/dy");
		if(!present("qingnian nanzi",this_object()) && !present("duan yu",this_object()))
		obj->move(environment(me));
	}
}
int do_ketou(string arg)
{
	object me,obj;
	me=this_player();
	if(present("duan yu",this_object())) return 0;

	if( !arg || arg != "yuxiang" ) return 0;

	if( arg=="yuxiang" ) {
		message_vision("$N口中数着，恭恭敬敬的向玉像磕起头来。\n", this_player());
		if (!me->query_temp(QUESTDIR1+"lbwb/left")|| !me->query_temp(QUESTDIR1+"lbwb/right"))
		{
			tell_object(me,"你对着玉像一个头磕下去，似乎发现玉像的左脚(left)、右脚(right)绣得有字。 \n");
			return 1;
		}
		me->add_temp(QUESTDIR1+"ketou_trigger", 1);
	}
	if( me->query_temp(QUESTDIR1+"ketou_trigger")==16 ) {
		me->delete_temp(QUESTDIR1+"lbwb1");
		me->delete_temp(QUESTDIR1+"ketou_trigger");
		
		if(me->query(QUESTDIR1+"start")
		&& (me->query_temp(QUESTDIR1+"see_duanyu1") || me->query(QUESTDIR1+"over"))
		&&!me->query(QUESTDIR6+"yuxiang"))
		{
			message_vision("$N看见蒲团上的薄草早已破裂，不由伸手进去，里面什么也没有，好象已经被取走了。\n", me);
			if(!me->query(QUESTDIR6+"yuxiang"))  log_file("quest/TLBB", sprintf("%s(%s) 玉像处磕头获得武功标志1。经验：%d。\n", me->name(1),me->query("id"), me->query("combat_exp")) );
			me->set(QUESTDIR6+"yuxiang",1);
			return 1;
		}
		
		if(me->query(QUESTDIR1+"start")
		&&me->query(QUESTDIR1+"wuliangover")
		&&!me->query(QUESTDIR1+"over"))
		{
			message_vision("$N看见蒲团上的薄草早已破裂，不由伸手进去，里面什么也没有，好象已经被取走了。\n", me);
			tell_object(me,HIG"虽然什么都没有，你忍不住不住地磕头，看来你已经形似痴狂。\n"NOR);
			return 1;
		}
		
		if(me->query(QUESTDIR1+"start")
		&&me->query(QUESTDIR6+"yuxiang")
		&&me->query(QUESTDIR1+"over"))
		{			
			if(me->query(QUESTDIR6+"pass")) {
				obj=unew("/d/dali/obj/dodge_book");
			  if(!clonep(obj)){
				  message_vision("$N看见蒲团上的薄草早已破裂，不由伸手进去，里面什么也没有，好象已经被取走了。\n", me);
				  return 1;
			  }
			  message_vision(HIR"$N看见蒲团上的薄草早已破裂，不由伸手进去，里面好像什么也没有，$N不甘心又摸索一会，果然找出一个帛卷。\n"NOR, me);
				obj->move(me);
			  return 1;
			}

			if(me->query("quest/chance")<1)
			{
				message_vision("$N看见蒲团上的薄草早已破裂，不由伸手进去，里面什么也没有，好象已经被取走了。\n", me);
				return 1;
			}
			
			if(present("bo juan",me))
			{
				message_vision("$N看见蒲团上的薄草早已破裂，不由伸手进去，里面什么也没有，好象已经被取走了。\n", me);
				return 1;
			}
			//任何人都可以拿到凌波微步秘籍，但是能不能领悟却只有三次机会（非VIP）

			
			obj=unew("/d/dali/obj/dodge_book");
			if(!clonep(obj)){
				message_vision("$N看见蒲团上的薄草早已破裂，不由伸手进去，里面什么也没有，好象已经被取走了。\n", me);
				return 1;
			}
			if(clonep(obj) && obj->violate_unique()){
				destruct(obj);
				message_vision("$N看见蒲团上的薄草早已破裂，不由伸手进去，里面什么也没有，好象已经被取走了。\n", me);
				return 1;
			}

			me->add("quest/chance",-1);
    	me->set("quest/lbwb/start",1);
			message_vision(HIR"$N看见蒲团上的薄草早已破裂，不由伸手进去，里面好像什么也没有，$N不甘心又摸索一会，果然找出一个帛卷。\n"NOR, me);
			log_file("quest/TLBB", sprintf("%s(%s)开始解凌波微步花费解迷机会一次。\n", me->name(1),me->query("id")) );
			obj->move(me);
			return 1;
		}
	}
	return 1;
}


string look_left()
{
	object me;
	me=this_player();
	me->set_temp(QUESTDIR1+"lbwb/left",1);
	return
	HIC"磕首百遍，供我驱策。\n"NOR;
}

string look_right()
{
	object me;
	me=this_player();
	me->set_temp(QUESTDIR1+"lbwb/right",1);
	return
	HIY"遵行我命，百死无悔。 \n"NOR;
}

string look_zi()
{
	return
	HIW"洞中无日月，人间至乐也 -- 逍遥子为秋水妹书。\n"NOR;
}

string look_yuxiang()
{
	object me;
	me=this_player();
	me->set_temp(QUESTDIR1+"lbwb/yuxiang",1);
	return
	HIR"那是一座白玉雕成的玉像，与真人一般大小，身上淡黄绸衫微微颤动，更奇的是一对眸子莹然有光，神采飞扬。\n"NOR;
}
