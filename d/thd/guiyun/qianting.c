// /u/jpei/guiyun/qianting.c
// 加入转换师傅的代码 Modified by Numa

#include <ansi.h>
inherit ROOM;

void create()
{
        set("short","前厅");
        set("long",@LONG
这是归云庄的前厅。平常招待客人都在这里进行。厅中陈设华美，雕梁画
栋，极穷巧思。
LONG
);
        set("objects",([
                __DIR__"npc/lugy" :     1,
                __DIR__"npc/zhuangding" : 1,
        ]));  
        set("exits",([
                "east" : __DIR__"shiqiao",
                "west" : __DIR__"zhongting",
        ]) );
        set("no_clean_up",0);
        setup();
        "/clone/board/guiyun_b.c"->foo();
}

void init()
{
        object me = this_player();

        if (me->query("family/family_name")=="桃花岛")
        {
                if ((int)me->query("family/generation")==2)
                {
                        if ((int)me->query_skill("bihai-chaosheng",1) < 130 && me->query_skill("bihai-chaosheng",1) >= 80)
                        {
                                me->set("family/master_id","lu chengfeng");
                                me->set("family/master_name","陆乘风");
                                me->set("family/generation",3);
                		me->set("title","桃花岛第三代弟子");
                                write( HIY "由于您的碧海潮生等级小于一百三十级，系统已将您转换为桃花岛第三代弟子。\n" NOR);
                                return;
                        }
                        else if ((int)me->query_skill("bihai-chaosheng",1) < 80)
                        {
                                me->set("family/master_id","lu guanying");
                                me->set("family/master_name","陆冠英");
                                me->set("family/generation",4);
                		me->set("title","桃花岛第四代弟子");
                                write( HIY "由于您的碧海潮生等级小于八十级，系统已将您转换为桃花岛第四代弟子。\n" NOR);
                                return;
                        }
                }
                else if ((int)me->query("family/generation")==3)
                {
                        if ((int)me->query_skill("bihai-chaosheng",1) < 80)
                        {
                                me->set("family/master_id","lu guanying");
                                me->set("family/master_name","陆冠英");
                                me->set("family/generation",4);
                		me->set("title","桃花岛第四代弟子");
                                write( HIY "由于您的碧海潮生等级小于八十级，系统已将您转换为桃花岛第四代弟子。\n" NOR);
                                return;
                        }
                }
        }
}
