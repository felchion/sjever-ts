// item: fire.c
 
inherit ITEM;

void create()
{
	set_name("食物", ({ "food" }));
	set_weight(100);
	if (clonep())
		set_default_object(__FILE__);
	else {
		set("unit", "条");
		set("value", 100);
		set("no_get", 1);
		set("no_give", 1);
		set("no_steal", 1);
		set("no_drop", 1);
	}
	setup();
}

void init()
{
	add_action("do_eat", "eat");
}

int do_eat(string arg)
{
	if (!id(arg))
		return 0;

	write("你想想这是为了洪七公而特意弄的，还是算了，不吃了。\n");
	return 1;
}
