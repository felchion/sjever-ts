inherit ROOM;
void create()
{
	set("short", "皇宫正门");
	set("long", @LONG
	这便是皇宫正门。巍峨雄伟的城墙上，'紫禁城'三个大字十分显眼，
气势辉煌。一张发黄的告示(gaoshi)贴在墙上，许多官兵立在两旁。
LONG
	);

	set("exits", ([
                 "north" : "/u/cocktail/workroom",
	]));
        set("objects", ([
                __DIR__"npc/wujiang" : 1,
                __DIR__"npc/bing" : 2,
        ]));
         set("outdoors", "beijing");
	setup();
}

/*
int valid_leave(object me, string dir)
{
    int i;
    object *inv;
    if(dir=="north")
    {
     if( this_player()->query_temp("canenter") )
        {
        inv = all_inventory(me);
        for(i=sizeof(inv)-1; i>=0; i--)
            if(inv[i]->query("weapon_prop") && (inv[i]->query("equipped")))
                if(objectp(present("wujiang", environment(me))))
                    return notify_fail("武将拦住你说道：这位" +
                        RANK_D->query_respect(me) + "请放下兵刃。"
                        "不得持兵刃入宫。\n");
                else if(objectp(present("wujiang", environment(me))))
                    return notify_fail("官兵拦住你说道：这位" +
                        RANK_D->query_respect(me) + "请放下兵刃。"
                        "不得持兵刃入宫。\n");
        }               				
     else {
        if(objectp(present("wujiang", environment(me))))
            return notify_fail("武将伸手拦住你朗声说道：皇宫重地，岂" 
            "容寻常人自由出入," + RANK_D->query_respect(me) +"\n请回，免得"
                "自取其辱。\n");
 }
   return ::valid_leave(me, dir);
}
*/
