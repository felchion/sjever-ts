
inherit BULLETIN_BOARD;

void create()
{
        set_name("丐帮-扬州大勇分舵留言板", ({ "board" }) );
        set("location", "/d/gb/fenduo2.c");
        set("board_id", "gb_board2");
        set("long", "这是一个供丐帮弟子使用的留言板。\n" );
        setup();
        set("capacity", 60);
}
