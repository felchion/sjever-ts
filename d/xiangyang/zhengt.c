// Room: zhengt.c
// Looklove y2k/4/26

#include <ansi.h>
inherit ROOM;

void create()
{
        set("short",HIG"正厅"NOR);
        set("long", @LONG
这里是薛府的正厅,周围的墙上挂着几幅字画,似乎出自名家手笔,正中
挂有一幅中堂(zhongtang)。屋子打扫的很干净,一张八仙桌,几把椅子放在
墙角。屋子里隐隐有一股药草的香气。
LONG
        );

        set("exits", ([
                  "south" : __DIR__"xue_tjing",
        ]));

        set("objects", ([
                __DIR__"npc/xue" : 1,
        ]));
        
        set("item_desc", ([
                "zhongtang" : "


                    妙手神医阎王敌


\n"
       ]));     
        set("coor/x",-40);
  set("coor/y",70);
   set("coor/z",0);
   setup();
}

