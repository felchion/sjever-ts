// strike_book.c 掌法图谱

inherit ITEM;

void create()
{
	set_name("掌法图谱", ({ "tu pu", "tupu", "book", "shu" }));
	set_weight(100);
	if( clonep() )
		set_default_object(__FILE__);
	else {
		set("unit", "本");
		set("long", "这是一本线装书，上面画满了正在练掌法的小人。\n");
		set("unique", 1);
		set("newbie", 1);
		set("treasure", 1);
		set("value", 0);
		set("material", "paper");
		set("skill", ([
			"name":	"strike",	// name of the skill
			"exp_required":	0,	// minimum combat experience required
			"jing_cost":	30,	// jing cost every time study this
			"difficulty":	10,	// the base int to learn this skill
			"max_skill":	51	// the maximum level you can learn
		]) );
	}
}
