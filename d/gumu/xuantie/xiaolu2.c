// by dubei
// Modify By River@sj 99.5.24
#include <ansi.h>
#include <room.h>

inherit ROOM;

void create()
{
       set("short",YEL"山路"NOR);
       set("long", @LONG
这是一条崎岖的山路，路面上夹杂着一些泥沙。齐膝的野草几乎把路给掩
盖了，开出来的野花飘着阵阵花香。四周不时传来一阵阵的野兽叫声, 头顶有
一些不知名的小鸟在跳来跳去。
LONG
    );
       set("outdoors", "襄阳");

       set("exits", ([
           "northup" : __DIR__"xiaolu3",
           "southdown" : __DIR__"xiaolu1",
       ]));

       setup();
}
