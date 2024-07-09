// Room: 大道 2
// bbb 1997/06/11
// Modify By River 98/12 
inherit ROOM;

void create()
{
        set("short", "大道");
	set("long", @LONG
一条笔直的石板大道通向前方，路上不时的有南来北往的车马经过，不少
行人手持各种兵器，看来都是习武之人，大理段家上自皇爷，下至王子，数代
保留祖上武林传统，民间习武之风更是盛行。
LONG
	);
        set("outdoors", "大理");
	set("exits", ([ /* sizeof() == 1 */
	    "north" : __DIR__"dadao1",
	    "south" : __DIR__"dadao3",
        ]));
	set("no_clean_up", 0);

	set("coor/x",-310);
  set("coor/y",-340);
   set("coor/z",-20);
   set("coor/x",-310);
  set("coor/y",-340);
   set("coor/z",-20);
   setup();
}
