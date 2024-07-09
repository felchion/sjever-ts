// Room: /d/songshan/chanyuan.c
#include <ansi.h>
inherit ROOM;

void create()
{
        set("short", "峻极禅院");
        set("long", @LONG
这里是嵩山峻极宫之峻极禅院，也是是嵩山派的据点。以前曾是寺
院，现在院子却是中古柏森森，殿上并无佛像，大殿虽也极大，可比之
少林寺的大雄宝殿却还有不如，可以容纳近千人。几名嵩山派弟子正提
剑巡逻。
LONG);
        set("exits", ([ 
             "north" : __DIR__"shidao1",
            "south" : __DIR__"damen",
//              "east" : __DIR__"csshi",
        ]));
        set("objects", ([ 
            CLASS_D("songshan") + "/ding" : 1,
            CLASS_D("songshan") + "/fei" : 1,
            __DIR__"npc/ssdizi" : 2,
        ]));
        set("coor/x",40);
  set("coor/y",280);
   set("coor/z",110);
   setup();
        "/clone/board/songshan_b"->foo();
}
