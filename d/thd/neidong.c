inherit ROOM;

void create()
{
        set("short", "岩洞");
        set("long", @LONG
这是一个山壁上的小岩洞，虽然不甚宽敞，但也有足够的活动空间。
LONG
        );
        set("exits",([
                "out" : __DIR__"cave.c",
        ]) );
        set("no_clean_up",0);
        setup();
}

