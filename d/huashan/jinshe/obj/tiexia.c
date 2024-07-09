// Room: /d/huashan/jinshe/obj/tiexia.c
// Date: Look 99/03/25

inherit ITEM;

void create()
{
        set_name("小铁匣", ({ "tie xia", "xia" }) );
        set_weight(3000);
        set_max_encumbrance(5000);
        if( clonep() )
                set_default_object(__FILE__);
        else {
                set("unit", "个");
                set("long", "这是一个不大的铁匣子，上面沾满了泥土，看起来年代已十分久远。\n");
                set("value", 1000);
                set("material", "steel");
                set("no_drop", "这样东西不能离开你。\n");
                set("no_get", "这样东西不能离开那儿。\n");
                set("no_get",1);             
                set("no_drop",1);
                set("amount",30);
                }
        setup();
}

int is_container() { return 1; }

void init()
{
        add_action("do_open", "kai");
}

int do_open(string arg)
{
        object me = this_player();
        object ob1;
        object ob;

       
    if (!arg || (arg != "xia" && arg != "tie xia"))
       {
        return notify_fail("你想打开什么？\n");
        return 1;
       }
    else
    {
    if (!present("jinshe miji",me))
       { 
        ob=unew(__DIR__"jinshemiji");
        if(!clonep(ob))
        return notify_fail("你轻轻打开铁匣，结果发现里面空无一物。\n");
        ob1=new(__DIR__"xinjian");
        write("你轻开铁匣，发现里面有一本落满尘土的书籍和一张发黄的信笺。你急忙把它捡起揣在怀中。\n");
        ob->move(me);
        ob1->move(me);
        return 1;
       }
        write("你打开了铁匣，发现里面空空的什么也没有。\n");
        return 1;
    }
}
