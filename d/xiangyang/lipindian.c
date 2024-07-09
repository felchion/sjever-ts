#include <ansi.h>
inherit ROOM;

void create()
{
        set("short", "礼品店");
        set("long", @LONG
一个小小的礼品店，柜台后面扔着四个大箱子，上面分别写着
臂力、根骨、悟性、身法。箱子里面盛着大家赠送的礼品，你可
以放置(put)礼品进去。
LONG
        );

		set("item_desc",([
        "put" : "语法：put <属性>。允许的属性是臂力(str)、根骨(con)、悟性(int)、身法(dex)。\n",
        ]));

        set("exits", ([ 
          "out" : "/d/xiangyang/cross2",
]));

        setup();
}
void init()
{
	object me=this_player();
	
	if( userp(me) ) {
		if( me->query("str")>30 ) {
			me->set("str",30);
			tell_object(me,"你的臂力已经被自动更新为30。\n");
		}
		if( me->query("con")>30 ) {
			me->set("con",30);
			tell_object(me,"你的根骨已经被自动更新为30。\n");
		}
		if( me->query("int")>30 ) {
			me->set("int",30);
			tell_object(me,"你的悟性已经被自动更新为30。\n");
		}
		if( me->query("dex")>30 ) {
			me->set("dex",30);
			tell_object(me,"你的身法已经被自动更新为30。\n");
		}
	}
    add_action("do_put", ({ "put", "fang" }));
	return;
} 

int do_put(string arg)
{
       object me;
       me = this_player(); 

	   if(!arg) return notify_fail("你要放进去什么礼品？\n");
	   if( me->query("str")+me->query("con")+me->query("int")
		   +me->query("dex") <= 80 ) {
		   tell_object(me,"你疯啦？这么穷还给人送礼？\n");
		   return 1;
	   }
	   if( arg=="臂力" || arg=="str" ) {
		   if( me->query("str")<=10 ) {
				tell_object(me,"你已经无法继续降低自己的臂力了。\n");
				return 1;
		   }
			me->set("str",me->query("str")-1);
			message_vision("$N从身上拿出一点臂力放在箱子里。\n",me);
			return 1;
	   }
	   else if( arg=="根骨" || arg=="con" ) {
		   if( me->query("con")<=10 ) {
				tell_object(me,"你已经无法继续降低自己的根骨了。\n");
				return 1;
		   }
			me->set("con",me->query("con")-1);
			message_vision("$N从身上拿出一点根骨放在箱子里。\n",me);
			return 1;
	   }
	   else if( arg=="悟性" || arg=="int" ) {
		   if( me->query("int")<=10 ) {
				tell_object(me,"你已经无法继续降低自己的悟性了。\n");
				return 1;
		   }
			me->set("int",me->query("int")-1);
			message_vision("$N从身上拿出一点悟性放在箱子里。\n",me);
			return 1;
	   }
	   else if( arg=="身法" || arg=="dex" ) {
		   if( me->query("dex")<=10 ) {
				tell_object(me,"你已经无法继续降低自己的身法了。\n");
				return 1;
		   }
			me->set("dex",me->query("dex")-1);
			message_vision("$N从身上拿出一点身法放在箱子里。\n",me);
			return 1;
	   }
	   else {
			tell_object(me,"这里接受的礼品只有臂力(str)、根骨(con)、悟性(int)、身法(dex)。\n");
			return 1;
		   }
}


int get_object(object ob)
{ 
        if(userp(ob))
        return 1;
        return 0;
}

int valid_leave(object me, string dir)
{
	object *obj;

        if(!wizardp(me)&& (me->query("str")+me->query("con")+me->query("int")
                +me->query("dex")>80))
        return notify_fail("你送的礼还不够，不能出去。\n");
	 obj = filter_array(deep_inventory(me),(:get_object:));
    if( sizeof(obj) ) 
		return notify_fail("想作弊？没那么容易。\n");
	
	return ::valid_leave(me, dir);
}
