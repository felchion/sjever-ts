// /d/xiangyang/eroad1.c

inherit ROOM;
#include <ansi.h>

void create()
{
	set("short", "青龙门内街");
	set("long", @LONG
这里是军事重镇襄阳城的东门内，不时地有顶盔贯甲的士兵走来走去。向
东出城可以进入安徽、江苏境内。南边有条小巷，小巷两边种着许多大树。北
面是襄阳武馆。
LONG
	);
        set("outdoors", "襄阳");

	set("exits", ([
		"west" : __DIR__"djie2",
		"east" : __DIR__"qinglongmen",
                "north" : __DIR__"damen",
		"south" : __DIR__"ncx5",
	]));

	set("incity",1);
	setup();
}

//void init()
//{
//	object me= this_player();
///*	if ( me->query("oldsj")) {
//		if ( !me->query("bxsj_bonus/age")) {
//			if (me->query("mud_age") > 7840000)
//				me->set("mud_age", 7840000);
//			me->set("bxsj_bonus/age", 1);
//		}
//	}
//*/	// 部分 Skill 调整
////	if ( me->query_skill("literate", 1) > me->query("int") * 20)
////		me->set_skill("literate", me->query("int") * 20);
////	if ( me->query_skill("bangjue", 1) > 200)
////		me->set_skill("bangjue", 200 );
////	if ( me->query_skill("checking", 1) > 200)
////		me->set_skill("checking", 200 );
////	if ( me->query_skill("stealing", 1) > 200)
////		me->set_skill("stealing", 200 );
////	if ( me->query_skill("checking", 1) > 200)
////		me->set_skill("checking", 200 );
////	if ( me->query_skill("dagou-zhen", 1) > 200)
////		me->set_skill("dagou-zhen", 200 );
////	if ( me->query_skill("begging", 1) > 200)
////		me->set_skill("begging", 200 );
////	if ( me->query_skill("poison", 1) > 200)
////		me->set_skill("poison", 200 );
////	if ( me->query_skill("buddhism", 1) > 200)
////		me->set_skill("buddhism", 200 );
////	if ( me->query_skill("taoism", 1) > 200)
////		me->set_skill("taoism", 200 );
////	if ( me->query_skill("huanxi-chan", 1) > 200)
////		me->set_skill("huanxi-chan", 200 );
////
////	if (! me->query("tls")
////	 && me->query_skill("liumai-shenjian", 1)){
////		me->delete_skill("liumai-shenjian", 1);
////		tell_object(me, HIG"天龙寺俗家弟子系统自动去掉六脉神剑。\n"NOR);
////	}
////
////	if ( me->query("oldsj") && me->query("bxsj_bonus/no_pk")){
////		if ( time() - me->query("bxsj_bonus/no_pk") > 1296000 ){
////			me->delete("no_pk");
////			me->delete("bxsj_bonus/no_pk");
////			tell_object(me,HIG"\n15天时间已到，系统自动帮您解除金盆洗手状态。\n"NOR);
////		}
//	}
//}
