// /clone/board/worker1_b.c

inherit BULLETIN_BOARD;

void create()
{
        set_name("经验交流版", ({ "board" }) );
        set("location", "/d/zhiye/jiaoliushi1");
        set("board_id", "worker1_b");
        set("long", "这是供大家交流制造经验和推销产品的留言版，如果有BUG也可以在这里提出。\n" );
        setup();
        set("capacity", 100);
}
