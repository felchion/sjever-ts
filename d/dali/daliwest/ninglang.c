// Room: /d/dali/ninglang.c
// bbb 1997/06/10 
// Modify By River 98/12
inherit ROOM;

void create()
{
	set("short", "宁浪泸沽湖");
	set("long", @LONG
整个湖泊，状若马蹄，南北长而东西窄。晨曦初露，湖水如染，一片金红，
朝阳徐徐上升，则为翠绿；待夕阳西下，又成一片墨绿。风静时，平滑若镜，
积万顷碧玉；微风起处，波光林林。湖泊周围山峦起伏，东北，峭拔壁立的肖
家火山，高三千七百八十七米；西北，状若雄狮狮蹲踞的格姆山高三千七百五
十五米。湖东，有条山梁蜿蜒而下直插湖心，
LONG
	);
        set("outdoors", "大理");

	set("exits", ([
             "southup" : __DIR__"yuelong.c",
             "northeast" : __DIR__"futiao",
	]));

	set("coor/x",-380);
  set("coor/y",-440);
   set("coor/z",-10);
   setup();
}
