// jingshenfeng.c 惊神峰

#include <ansi.h>
#include <room.h>

inherit ROOM;

void init()
{
	object me = this_player();
	int lvl,lvl1;
	lvl = me->query_skill("liangyi-jian", 1);
	lvl1 = me->query_skill("mantian-huayu", 1);
	if (lvl > lvl1 && userp(me)){
		me->delete_skill("liangyi-jian");
		me->set_skill("mantian-huayu", lvl);
		write(HIG "系统已将您的「两仪剑法」转换为「漫天花雨」。\n" NOR);
	}
}

void create()
{
        set("short","惊神峰"NOR);
	set("long",@long
折而向北，过了一岭，只见古柏三把百余章，皆挺直端秀，凌霄托根树旁，
作花柏顶，灿若云荼。走出十余丈，只听得琴声之中杂有无数鸟语，细细听来，
琴声竟似和鸟语互相应答，间间关关，宛转啼鸣。只见三株大松树下一个白衣
男子背向而坐，膝上放这一张焦尾琴，整自弹奏。他身周树木上停满了鸟雀，
黄莺，杜鹃，喜鹊，八哥，还有许多不知其名的，和琴声或一问一答，或齐声
和唱。
long);

        set("objects",([
          CLASS_D("kunlun")+ "/hezudao" : 1,
        ]));

        set("exits", ([
              "south" : __DIR__"sanshengao",
        ]));
	set("outdoors", "昆仑");
	setup();
}
