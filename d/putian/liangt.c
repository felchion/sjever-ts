// /d/putian/liangt.c
// lklv 2001.7.21

inherit ROOM;
int do_sit(string arg);
int do_stand();
void delete_served(object me);

void create()
{
        set("short", "凉亭");
        set("long", @LONG
这是半山腰的一个小凉亭，中央放有一张白色的石头桌(table)，周围零散
摆有四五个石头凳(chair)。有几个劲装打扮的外地人，正坐在那里休息乘凉。
一个小和尚手里捧着茶壶站在一旁。
LONG
        );

        set("exits", ([
                "out" : __DIR__"road07",
        ]));

        set("item_desc", ([
                "table" : "一张石头桌，上面放着几个大茶碗。\n",
                "chair" : "一个石头凳，角上稍微有些破损。\n",
        ]));
        set("objects", ([
                __DIR__"npc/shami" : 1,
        ]));
        set("outdoors", "福州");
        set("no_fight", 1);
        setup();
}

void init()
{
        add_action("do_stand", "stand");
        add_action("do_stand", "zhan");
        add_action("do_sit", "sit");
        add_action("do_sit", "zuo");
}

int serve_tea(object me)
{
        object ob;

        if (!present(me, this_object())) return 0;
        if (objectp(ob = present("xiao shami", environment(me)))){
                message_vision("小沙弥走过来，给$N倒了一杯茶。\n", me);
                new("/d/thd/obj/huacha.c")->move(environment(me));
                me->set_temp("marks/tea_served", 1);
                call_out("delete_served", 120, me);
        }
        return 1;
}

void delete_served(object me)
{
        if ( objectp(me) ) me->delete_temp("marks/tea_served");
}

int do_sit(string arg)
{
        object me = this_player();

        if (me->is_busy() || me->is_fighting()) return notify_fail("你正忙着呢！\n");
        if (arg == "table" || arg == "zhuozi")
                return notify_fail("你正要坐在桌子上，旁边的几个汉子的眼睛一起瞪了过来，你急忙直了直腰。\n");

        if ( !arg || (arg != "chair" && arg != "dengzi") )
                return notify_fail("你正要坐在地上，抬头看见旁边一位异性的目光扫过来，为了不影响形象，你又直腰站了起来。\n");

        if (me->query_temp("marks/sit"))
                return notify_fail("你已经坐在座位上了。\n");

        message_vision("$N找了个空位座下，等着上茶。\n", me);
        if( me->query_temp("marks/tea_served") ) {
                message_vision("小沙弥走过来对$N说：施主，喝饱了的就出去吧，请别占着地方！\n", me);
                message_vision("$N发觉众人的眼光都在盯着自己，忙站起身来尴尬地说：我其实是在看这石凳上的蚂蚁。\n", me);
                return notify_fail("");
        }
        me->set_temp("marks/sit", 1);

        call_out("serve_tea", 1, me);
        return 1;
}

int do_stand()
{
        object me = this_player();

        if (!me->query_temp("marks/sit"))
                return notify_fail("你已经站着了，是不是站久了有点发昏啊？\n");

        message_vision("$N拍拍屁股，站了起来。\n", me);
        me->delete_temp("marks/sit");
        return 1;
}

int valid_leave(object me, string dir)
{
        if (me->query_temp("marks/sit"))
                return notify_fail("你打算连凳子也搬出去？\n");

        if (dir == "out" && present("moli huacha", me))
                return notify_fail("小沙弥走过来说道：施主，茶杯是我们寺里的，请别带走。\n");

        return ::valid_leave(me, dir);
}
