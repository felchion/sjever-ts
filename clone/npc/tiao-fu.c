// tiao-fu.c 挑夫 标准垃圾npc, 我转移到NPC_D 目录了, lklv

inherit NPC;

void create()
{
	set_name("挑夫", ({ "tiao fu", "tiaofu", "tiao", "fu", "porter" }) );
	set("gender", "男性" );
	set("age", 20+random(15));
	set("long", "这是个老实巴交的穷挑夫，长得膀阔腰圆，一身肌肉。\n");
	set("combat_exp", 20000 + random(400));
        set("str", 25);
        set("dex", 22);
        set("con", 20);
        set("int", 15);
	set("attitude", "heroism");

	setup();
        carry_object(ARMOR_D("cloth"))->wear();
        carry_object(ARMOR_D("caoxie"))->wear();
	add_money("silver", 3);
}
