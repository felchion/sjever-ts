// Room: /d/emei/leidongping.c 
// 峨嵋派 雷洞坪


inherit ROOM;
#include <ansi.h>
void create()
{
        set("short",HIR "雷洞坪" NOR);
        set("long", @LONG
雷洞坪一带，道路艰险骇人，路左是蔽日遮天的冷杉林，路右是深不见底
的绝壁峡谷。这里长年云遮雾罩，空蒙黑暗。岩上建有雷神殿，旁边竖一铁碑。
由此往上是「八十四盘」至接引殿，北下到洗象池。
LONG
        );
        set("outdoors", "峨嵋山");
        set("exits", ([
                "northdown" : __DIR__"lingyunti",
                "southup" : __DIR__"basipan1",
        ]));
	
	set("lingwu", ([
		"skill" : "force",
                "low_limit" : 100,
		"up_limit" : 200,
		"lw_msg": BLU"$N举眼四望，发现这里地势奇特，灵气内涵。于是静下心来专心思考，\n"+
			  "参照自己的$s心得，潜心领悟$b方面的武学问题。\n"NOR,
	]));
	
        setup();
}
int valid_leave(object me, string dir)
{
 	if (dir == "southup" && (int)me->query_skill("dodge", 1) < 101 ) {
      		me->improve_skill("dodge",(int)me->query("dex")/10);
      		me->receive_damage("jingli",  (int)me->query("dex"));
      		return 1; 
      	}     
      	return ::valid_leave(me, dir);

} 
