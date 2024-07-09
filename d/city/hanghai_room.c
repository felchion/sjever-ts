#include <ansi.h>

int errs( string msg )
{
     if( !msg ) return 0;
    write( msg );
    return 1;
}

inherit ROOM;

#define MST		"/clone/npc/hanghai_master"		// 经纬仪，游戏指令主支持


void create()
{
	set("short", HIC"【南海探宝】船只"NOR);
	set("long", @LONG
这是一艘南海探宝的船只，多年来不知有多少英雄豪杰尝试而不得其果。船身
上下全是用树木做成。一排排的风浪从大海打来，使之不住地颠簸，船头有一块破
旧的牌子(sign)写着几行字。
LONG
        );
	set("no_fight", "1");
	set("no_steal", "1");
	set("no_sleep_room", "1");
	set("valid_startroom", 1); // 此两条分别为：非连线房间 及 起始房间
        set("item_desc", ([
                        "sign":
                    "┌──────────────"+HIG"南海探宝须知"NOR+"───────────────┐\n"
                        "│ 一：在可以使用(game start)来开始探宝                                 │ \n"
                        "│ 二：在探宝途中可以使用(game over)来结束探宝                          │ \n"
                        "│ 三：请用(help job tanbao)来查询具体说明。                            │ \n"
                        "└───────────────────────────────────┘ \n\n"
                        ]) );
	set("exits", ([
	    "out":  "/d/city/guangchang",
	]));

	setup();
}

void init()
{
	add_action("do_game", "game");
}

int do_game(string arg)
{
	object me = this_player(), mst_obj;

	if( !arg || arg != "start" )
		return 0;

// "game over" 等操作，由 MST 物件进行。


	if( me->query_temp("hanghai_gameing") )
		return errs("你已身在游戏中！\n");

	if( file_size( MST + ".c" ) < 1 )
		return errs("本游戏暂停开放！\n");

	write("\n========= 请稍候，正在执行操作…… ==========\n\n");

	mst_obj = new( MST );

	mst_obj->new_game(me);
	mst_obj->move(me);

	return 1;
}



int valid_leave(object me, string dir)
{
	if( me->query_temp("hanghai_gameing") )
		return notify_fail("你不是正在游戏吗？，不得离开，如果要离开先结束游戏(game over)，在离开！\n");

	return ::valid_leave(me, dir);
}
