// /u/beyond/hz/feilaifeng.c 飞来峰
#include <ansi.h>
inherit ROOM;
void create()
{
        set("short", HIC"飞来峰"NOR);
        set("long", @LONG
飞来峰上有枣刻在石壁上的各种造像。左岩壁上刻有阿弥陀佛、观音等
造像，袒腹露胸、笑容满面的弥勒佛，是这里最大的一组雕像。右边石壁上
刻有“多闻天王”的造像，他全身披甲，眼若铜玲，威风凛凛的骑在狮子背
上，十分生动。
LONG
        );
        set("exits", ([
            "west" : __DIR__"qsddao3",
"southeast" : __DIR__"yxsdong",
]));
              set("objects",([
                "/clone/npc/gui" : 1,
                "/clone/npc/gui2" : 1,
        ]));

        setup();
}
