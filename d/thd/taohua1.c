// Room: /u/jpei/thd/taohua.c

inherit ROOM;

void create()
{
        int thd1, thd2, thd3, thd4;

        thd1 = random(4) + 1;
        thd2 = random(4) + 1;
        thd3 = random(4) + 1;
        thd4 = random(4) + 1;
        while (thd1 != 4 && thd2 != 4 && thd3 != 4 && thd4 != 4) {
                thd1 = random(4) + 1;
                thd2 = random(4) + 1;
                thd3 = random(4) + 1;
                thd4 = random(4) + 1;
        }
        set("short", "桃花阵");
        set("long", @LONG
这是一片茂密的桃花丛，你一走进来就迷失了方向。
LONG
        );

        set("exits", ([
                "east" : __DIR__"taohua" + thd1,
                "west" : __DIR__"taohua" + thd2,
                "south" : __DIR__"taohua" + thd3,
                "north" : __DIR__"taohua" + thd4,
        ]));
        set("outdoors","桃花岛");
        setup();
}

void init()
{
        object letter, ob=this_player();

        if ((letter = present("recommend letter", ob)) && letter->query("owner") == ob->query("id")) {
                write("你正在疑惑怎么穿过这个桃花阵，一个作仆役打扮的哑巴忽然走了出来，检查了你身上的推荐信后示意你跟着他走。\n");
                write("没过多久，你们就穿过了桃花阵，经过一片绿竹林后来到一座亭子跟前。\n");
                ob->move(__DIR__"jicui", 1);
                tell_room(environment(ob), "一个哑仆领着" + ob->name() + "走了过来。\n", ({ob}));
                return;
        }

        if ((ob->query("family/family_name") == "桃花岛" && ob->query_skill("qimen-bagua", 1) > 19) ||
                ob->query_skill("qimen-bagua", 1) > 130) {
                write("由于你平常看惯了这些简易的阵法，所以不用细想，信步就走出了阵！\n");
	        ob->move(__DIR__"shangang", 1);
                tell_room(environment(ob), "却见花影闪烁，树丛忽然现出一个缺口，" + ob->name() + "闪身而出。\n", ({ob}));
                return;
        }
        if (ob->query_temp("step_count") > 99) {
                ob->delete_temp("step_count");
                write("你在阵中累得精疲力尽，终因体力不支而昏了过去！\n");
                write("迷迷胡胡中似乎有人把你抬出了阵，仿佛有个威严的声音喝道：“扔到海里去”。\n");
                ob->unconcious();
        ob->move(__DIR__"haibin", 1);
                message("vision", "忽然一个大潮涌了过来，将一个被水泡得半死的家伙冲到了岸边。\n", environment(ob), ob);
                return;
        }
        ob->add_temp("step_count", 1);
}
