// yubang.c 福州渔港

#include <ansi.h>
inherit ROOM;

void create()
{
	set("short","渔港码头");
	set("long", @LONG
这是福州的一个渔港，岸边停满了渔船。渔夫们正挑着大担大担的鱼走
着，箩里的鱼还在鲜蹦活跳。不远处还有几个乞丐挨在船边，捧着渔夫们扔
下的死鱼在狂吃，你不由得有点恶心。看样子，那几个叫化子身上还别着两，
三个布袋，莫非。。。他们就是丐帮的？
LONG
	);
	set("exits", ([
	"enter" : __DIR__"yuchuan1",
	"west" :  "/d/fuzhou/haigang", 
        ]));
	set("outdoors","福州");
        set("coor/x",120);
  set("coor/y",-470);
   set("coor/z",0);
   setup();
}
