// Room: /d/wizard/zoulang.c

inherit ROOM;

void create()
{
	set("short", "走廊");
	set("long", @LONG
这里的自动扶梯会自动把各位巫师送到自己的工作间。
LONG	);
	set("exits", ([
		"down" : __DIR__"wizard_room"
]));

	set("coor/x",0);
  set("coor/y",0);
   set("coor/z",10000);
   setup();
}

void run(object ob)
{
	if (environment(ob) == this_object() )
	ob->move("/u/"+ob->query("id")+"/workroom");
}

void init()
{
	object ob = this_player();
	if(wiz_level(ob))
    		call_out("run", 5, ob);
}
