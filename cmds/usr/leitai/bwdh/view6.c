// view
// Creator : Numa@Sj 2000-5-4 14:31

#include <ansi.h>
inherit ROOM;
#include "view.h"

void create()
{
        set("short",HIC"观武东厅"NOR);
        set("long", LONG );
        set("exits", ([
        "west" : __DIR__"view5",
        ]));
        
	set("no_fight",1);
	set("no_sleep",1);

        setup();
}

