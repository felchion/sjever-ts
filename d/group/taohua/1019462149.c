// automake group room , Mon Apr 22 16:42:16 2002
inherit ROOM;
#include <ansi.h>
#include "/d/group/gate.h"
void create()
{
set("short","[1;35m桃花林[0;0m");
set("long","一大片桃花林。夹岸数百步，中无杂树，芳草鲜美，落英缤纷。你忽
然发现一株桃树上隐隐约约有刻着字迹：走近细看，只见比划苍劲有力，
竟象是用手指刻上去的！！
                     三秦（Action）到此一游
[0;0m");
set("exits",([
"west":__DIR__"1019461907.c",
]));
set("outdoors","[1;35m桃花源[0;0m");
set("group1","taohua");
set("group2","[1;35m桃花源[0;0m");
setup();
setup_var();
}
