//新开前往地下生死比武场地
//modified by campsun 2003/09/26
inherit ROOM;
void create()
{
        set("short","赌场大厅");
        set("long", @LONG
你一走进这房间，只听到玎玲玲，玎玲玲骰子落碗之声，说不出的悦耳
动听。房里已聚着五六个人，都是一般的打扮，正在聚精会神的掷骰子，抬
头一望有匾（ｓｉｇｎ）一块。
LONG
        );
        set("exits", ([ /* sizeof() == 4 */
         "east" : __DIR__"duchang",
         "north" : __DIR__"duchang4",
               "down" : __DIR__"dixiashi",
  
]));
        set("item_desc", ([
                "sign": @TEXT


骰子的赌法：

ｂｅｔ　＜种类＞　＜数量＞

种类：　０，１，２，３到１８
０：　          赌小    （３－１０）    一赔一
１：            赌大    （１１－１８）  一赔一
２：　          赌围骰  （三粒骰子同点）一赔三十六
３－１８        赌单点                  一赔八

例子：
ｂｅｔ　０　５０
赌五十两在小上

TEXT
        ]) );
        set("no_fight",1);
        set("no_sleep",1);
        set("objects", ([
        __DIR__"npc/pingwei" : 1,
                        ]) );
        set("coor/x",80);
  set("coor/y",-20);
   set("coor/z",0);
   setup();
}
void init()
{
object me;
me = this_player();
if( me->query("gamble/amount")) {
me->delete("gamble/amount");
me->decrease_skill("betting",1);
}

        add_action("action", ({
                   "dazuo",
                   "du",
                   "exercise",
                   "lian",
                   "practice",
                   "respirate",
                   "study",
                   "tuna",
                   															//防止玩家从平威这里暗算赌场老板
        }));
        add_action("ansuan",({"ansuan"}));
}

int action()
{
        write("你无法静下心来修炼。\n");
        return 1;
}
int ansuan()
{
	string userip;
	userip = query_ip_number(this_player());
	write(userip+"\n");
	write("你想暗算谁？这里可不是打架的地方！\n");
	return 1;

}
int valid_leave(object me, string dir)
{
	if (userp(me)) 
		if( "/d/city/dixiashi"->checkSameIp(me) && dir == "down")
			return notify_fail("你使劲想挤进地下赌场，但人太多了，你根本进不进去！\n");
	if ( !me->query_temp("dxdc/enter") && dir == "down" )
		return notify_fail("平威将手在你面前一横，笑道：抱歉得很，下面不欢迎生客！\n");
	if ( (int)me->query_condition("killer") && dir == "down" )											//禁止杀人犯进入地下比武场
		return notify_fail("平威前后仔细的打量了你一遍：你是朝廷通缉的要犯吧？这里可不欢迎你。\n");
		
	return ::valid_leave(me, dir);
}
