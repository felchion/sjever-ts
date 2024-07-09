//xiuxishi.c 休息室

#include <ansi.h>
#include <room.h>

inherit ROOM;

void create()
{
	set("short",HIR"休息室"NOR);
	set("long", @LONG
这是试剑山庄比武后的武者休息室，平日这里一片呼喝之声，最重修习
基本，庄主对弟子管教极为严格，也正因为如此，门下弟子武功大都不弱，
而且品行端正，武林之中莫不敬仰，此时休息室上却空无一人，想必有大事
发生。
LONG
        );

	set("indoors", "试剑山庄");
	setup();
}

#include "sjsz_room.h";
