// gate.c

#include <ansi.h>
inherit ROOM;

void create()
{
	set("short", HIW "鬼门关" NOR);
	set("long", @LONG
这里就是著名的阴间入口【鬼门关】，在你面前矗立著一座高大的黑色城
楼，城楼的门口有许多亡魂正哭哭啼啼地列队前进，一进鬼门关就无法再回阳
间了。
LONG);
	set("exits", ([
		"enter" : __DIR__"gateway",
	]) );
	set("no_fight", 1);
	setup();
}

void init()
{
        object ob, me;
        me = this_player();

        //桃花、乾坤一指
        if(me->query("thd/qiankun")) {
                 me->set("max_qi",me->query("qiankun/qi"));
                 me->set("max_neili",me->query("qiankun/neili"));
                 me->delete("qiankun/qi");
                 me->delete("qiankun/neili");
        }
        if (me->query("thd/fugu")) // 桃花、附骨钉
                 me->delete("thd/fugu");

        //神龙、七虫散
        if(me->query("dec/str")) {
            me->add("str",me->query("dec/str"));
            me->delete("dec/str");
        }
        //赵敏、断指
        if (me->query("duanzhi"))
                 me->delete("duanzhi");
        // gb 布袋
        if (me->query("family/family_name")=="丐帮"
         && me->is_ghost() ) {
        	ob = me->query_temp("armor/gaibang");
        	if (objectp(ob))
        		destruct(ob);
        }

        if (me->is_ghost())
                call_out( "run", 1, me);
}

void run(object ob)
{
        if (!ob || !present(ob)) return;
        tell_object(ob, HIW"一股阴冷的浓雾突然出现，很快地包围了你。\n\n"NOR);

        if (ob->query("class") == "bonze" || ob->query("class") == "lama")
		ob->move(__DIR__"pusadian");
	else
                random(2)?ob->move(__DIR__"gateway"):ob->move(__DIR__"mpting");
}

int valid_leave(object me, string dir)
{
        if( wizardp(me) || !userp(me) ) return ::valid_leave(me, dir);
        if( dir == "enter")
                return notify_fail(
"赫见，你身前竟出现为数逾千枯于不堪的人将你包围，使你动弹不得，这
些人有男有女，他们衣衫褴楼，身上的肌肤，早已枯于得不成人形，就像
是一群皮包着骨的骷髅，相当骇人！\n"
                );
        return ::valid_leave(me, dir);
}

