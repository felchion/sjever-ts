// automake group room , Mon Apr 22 16:55:03 2002
inherit ROOM;
#include <ansi.h>
#include "/d/group/gate.h"
void create()
{
set("short","[1;35m桃花林[0;0m");
set("long","    一大片桃花林。夹岸数百步，中无杂树，芳草鲜美，落英缤纷。你忽
然发现一株桃树上隐隐约约有刻着字迹：走近细看，只见比划字迹歪歪斜
斜没有章法间架，然而很有力道，象是个武夫所写！！
                     [0;33m大蛤蟆（Biaofeng）到此一游
[0;0m");
set("exits",([
"south":__DIR__"1019461907.c",
]));
set("outdoors","[1;35m桃花源[0;0m");
set("group1","taohua");
set("group2","[1;35m桃花源[0;0m");
setup();
setup_var();
}
