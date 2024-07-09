// /d/gb/xiao-mushi.c 小墓室
// by Looklove Y2K 5/10

#include <ansi.h>
inherit ROOM;

void create()
{
          set("short", "小墓室");
        set("long", @LONG
这是一个小墓室，墙上挂满了火炬，照得光亮亮的。墙边有一口不太深的
井，隐约可以看见井里面清澈的泉水。角落里有一个摊开的布口袋，上面放着
几个从附近百姓家讨来的剩馒头。花子们靠在角落里的破床边打着瞌睡。
LONG);
        set("resource/water", 1);
        set("sleep_room",1);
        set("no_fight", 1);
        set("exits", ([
                    "east" : __DIR__"mushizl",                
        ]));
        set("objects", ([
                FOOD_D("mantou") : 4
        ]));
        set("coor/x",100);
  	set("coor/y",70);
   	set("coor/z",-10);
   	setup();
}

void init()
{
        add_action("do_drink", "drink");
        add_action("do_get", "get");
}

int do_drink(string arg)
{
        int water, max_water;
        object me = this_player();
        
        water = me->query("water");
        max_water = me->query("str")*10 ;
        
        if (!arg) {
		if ( water < max_water ) {
            		me->set("water", water+30);
              		message("vision", me->name()+"趴在井台边上喝了一口水。\n",environment(me), ({me}) );
            		write("你趴在井边喝了一口水，感觉舒服多了。\n");
                	return 1;
                }
          	else write("你现在口不渴，喝不下水了。\n");
       		return 1;
       	}
	return 0;
}

int do_get(string arg)
{	
	object me = this_player();
	object ob;

	if (arg && (ob = present(arg)) && ob->id("man tou"))
	{
	 	if (present("man tou", me) || me->query("family/family_name")!="丐帮"
		|| me->query("food") > me->query("str")*10)
		{
			write("角落里有个老叫化哼了一声，你不由的缩回了手。\n");
			return 1;
		}
	}
	return 0;
}
