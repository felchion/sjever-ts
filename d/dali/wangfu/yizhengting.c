//泥潭中国
//http://mud.lovefinal.com
//Final(章鹏)
// Modify By River 98/12 
inherit ROOM;

void create()
{
        set("short","议政厅");
	set("long", @LONG
这里是镇南王办理公事的地方，一张红木大桌摆在厅中央，桌上摆满了各
种文件，四周也都是一个个的大柜子。
LONG
	);
	set("exits", ([ 
	    "east" : __DIR__"lang3",
	]));
	 
	set("coor/x",-320);
  set("coor/y",-450);
   set("coor/z",-20);
   setup();
}
