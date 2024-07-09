inherit ROOM;

void create()
{
	set("short", "长江渡船");
	set("long", @LONG
一叶小舟，最多也就能载七、八个人。一名六十多岁的老艄公手持一根长
竹篙，正在船尾吃力地撑着船。
LONG);
	set("no_fight", 1);
	set("outdoors", "扬州");
	set("no_sleep_room", 1);
	setup();
}

int arrive(string arg)
{
	object room, ob = this_object();

	set("start", 1);
	if (stringp(arg)) set("side", arg);
	if (room = load_object(__DIR__ + query("side"))) {
		tell_room(ob, "\n渡船猛地一震，已经靠岸，船夫说道：“请大伙儿下船吧！”\n",0,1);
		set("exits/out", __DIR__+ob->query("side"));
		tell_room(ob, "说着将一块踏脚板搭上堤岸，形成一个出去(out)的阶梯。\n"),0,1;
		room->set("exits/enter", __FILE__);
		tell_room(room, "\n一艘渡船缓缓地驶了过来，艄公将一块踏脚板搭上堤岸，以便乘客上下(enter)。\n",0,1);
		remove_call_out("on_board");
	  	call_out("on_board", 25);
	}
	return 1;
}

void on_board()
{
	object room, *obj, ob = this_object();
	int i;

	ob = this_object();

	if (room = load_object(__DIR__ + query("side"))) {
		obj = all_inventory(ob);
		for(i = 0; i < sizeof(obj); i++) {
			if (userp(obj[i]) && obj[i]->query_temp("side") != ob->query("side")) {
				if (!living(obj[i])) tell_room(ob, "艄公把"+obj[i]->name()+"抬下了船。\n",0,1);
				else tell_room(ob, obj[i]->name()+"听了艄公的话，乖乖地下了船。\n", obj[i],1);
				obj[i]->move(room);
				obj[i]->delete_temp("side");
				if (!living(obj[i])) tell_room(room, obj[i]->name() + "被抬下了船。\n", obj[i],1);
				else tell_room(room, obj[i]->name() + "走下了船。\n", obj[i],1);
			}
		}
		room->delete("exits/enter");
		tell_room(room, "\n艄公们把踏脚板收了起来，长篙一点，渡船向江心驶去。\n",0,1);
	}
	delete("exits/out");
	tell_room(ob, "\n艄公们把踏脚板收起来，说了一声“坐稳喽”，长篙一点，渡船向江心驶去。\n",0,1);
	set("start", 2);
	if (query("side") == "jiangbei1") set("side", "jiangnan1");
	else set("side", "jiangbei1");
	remove_call_out("arrive");
	call_out("arrive", 10+random(5));
}
