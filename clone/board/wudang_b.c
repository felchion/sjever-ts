// /clone/board/wudang_b.c

inherit BULLETIN_BOARD;

void create()
{
	set_name("太极八卦图", ({ "board" }) );
	set("location", "/d/wudang/sanqing");
	set("board_id", "wudang_b");
	set("long", "这是一张武当山的太极八卦图，上面画有一些东西。\n" );
	setup();
	set("capacity", 60);
}

