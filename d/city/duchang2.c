inherit ROOM;

void create()
{
        set("short", "赌场");
        set("long", @LONG
        [1;33m≌≌≌≌≌≌≌≌≌≌≌≌≌≌≌≌≌≌≌≌[0m
            [1;33m≌≌[0m                                [1;33m≌≌[0m
            [1;33m≌≌[0m 　[1;33;41m拱[0m    [1;33;41m猪[0m　　[1;33;41m帮[0m　　[1;33;41m总[0m   [1;33;41m舵[0m  [1;33m  ≌≌[0m
            [1;33m≌≌[0m                                [1;33m≌≌[0m
            [1;33m≌≌≌≌≌≌≌≌≌≌≌≌≌≌≌≌≌≌≌≌[0m

    这里有四条走廊通向不同的拱猪房间。房间里面灯火通明，永远准备
着为拱猪狂们服务。墙上贴着一张发黄的纸(paper)，上面写着拱猪门的
级别。你可以查看各拱猪房(pigroom)的人数状况。
LONG
        );
        
        set("item_desc", ([
                "paper" : 
"【拱猪帮】的级别由低到高分为：\n\n"
"新手，学童，童生，书生，秀才，\n"
"举人，解元，贡士，会元，进士，\n"
"庶吉士，传胪，探花，榜眼，状元，\n"
"翰林，学士，大学士，首辅，领袖，\n"
"大宗师，帮主\n"
        ]));
        
        set("exits", ([
        "east" : __DIR__"eproom",
        "west" : __DIR__"wproom",
        "south" : __DIR__"sproom",
        "north" : __DIR__"nproom",
        "down" : __DIR__"duchang",
        ]));

        set("no_fight", 1);
        set("no_sleep_room", 1);
        set("objects", ([
               __DIR__"npc/qian-laoben" : 1,
        ]));

        set("coor/x",90);
  set("coor/y",-20);
   set("coor/z",10);
   set("incity",1);
	setup();
        "/clone/board/pig_b"->foo();
}


void init()
{
    add_action("do_pigroom", "room");
    add_action("do_pigroom", "pigroom");
}

int do_pigroom()
{
	object env1, env2, env3, env4;
    object *obj1, *obj2, *obj3, *obj4;
    object me = this_player();
    int i, i1, i2, i3, i4;

	env1 = load_object(__DIR__"eproom.c");
	obj1 = all_inventory(env1);

	env2 = load_object(__DIR__"nproom.c");
	obj2 = all_inventory(env2);

	env3 = load_object(__DIR__"sproom.c");
	obj3 = all_inventory(env3);

	env4 = load_object(__DIR__"wproom.c");
	obj4 = all_inventory(env4);


    for (i = 0; i < sizeof(obj1); i++) 
        if (userp(obj1[i]))
            i1++;

    for (i = 0; i < sizeof(obj2); i++) 
        if (userp(obj2[i]))
            i2++;

    for (i = 0; i < sizeof(obj3); i++) 
        if (userp(obj3[i]))
            i3++;

    for (i = 0; i < sizeof(obj4); i++) 
        if (userp(obj4[i]))
            i4++;

        tell_object(me, "现在各拱猪房里的人数有：\n" + 
        "　　　　北" + "\n" +
        "　　　　" + i2 + "\n" +
        "　西 " + i4 + "　　　" + i1 + " 东\n" + 
        "　　　　" + i3 + "\n" +
        "　　　　南 \n");
    return 1;
}

