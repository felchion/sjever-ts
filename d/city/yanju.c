// Room: /d/city/yanju.c

inherit ROOM;

#define AGE_LIMIT 22
#define LIME "/d/city/obj/lime"
#define DAI "/d/city/obj/budai"

void create()
{
	set("short", "扬州盐局");
	set("long", @LONG
你来到一座结构宏伟的建筑前，左右石坛上各插着一根两丈多高的旗杆，
杆上锦旗飘扬。左首旗子上写着“扬州盐局”四个黑字，银钩铁划，端的是刚
劲非凡。扬州一带是江北淮盐的集散之地，官府为了便于管理, 征得盐税, 特
设了扬州盐局, 直接听命于朝廷。入口处有一堆盐商在说笑，墙角堆着一堆石
灰粉，旁边放着个布袋。
LONG
	);

	set("exits", ([
		"west" : __DIR__"nandajie2",
    "east" : __DIR__"dongting2",
	]));

	set("objects", ([
	        __DIR__"npc/shisong" : 1,
	        __DIR__"npc/yanshang" : 2,
	]));
	
	set("hui",5);
	set("dai",2);

	set("coor/x",110);
  set("coor/y",-30);
  set("coor/z",0);
  set("incity",1);
	setup();
}

void init()
{
	add_action("do_na","na");
  add_action("do_zhua",({"zhua","zhuaqu"}));
}

void set_hui() {set("hui",5);}
void set_dai() {set("dai",2);}

int do_zhua(string arg)
{
	object me = this_player();
	object ob, ob1;
	
	if (!arg || arg != "石灰")
		return 0;
	if (!(ob = present("bu dai", me)))
		return notify_fail("空手捧石灰，你不怕迷了眼睛？！\n");
	if (me->query_temp("zhua_shihui"))
		return notify_fail("刚抓过石灰，小心盐局的人打你！\n");
	if (!ob->query("fill_lime"))
		return notify_fail("你身上这口袋不结实，恐怕装不了石灰。\n");
	if (query("hui") < 1)
		return notify_fail("人家盖房子正要用石灰呢，你偷完了怎么办？！\n");
	if (random(30) < 15)
		return notify_fail("突然，一个盐枭看见了你的动作，大声喝喊着：“有人偷石灰！！”\n");
	message_vision("$N趁人不注意，偷偷从地上抓了一把石灰放在身上的袋子里。\n", me);
	ob1 = new(LIME);
	ob1->move(ob);
	if(add("hui",-1) < 1) call_out("set_hui",86400);
	
	me->set_temp("zhua_shihui",1);
	call_out("do_can_zhua", 50, me);
	return 1;
}

void do_can_zhua(object me)
{
	if (!me) return;
	me->delete_temp("zhua_shihui");
}

int do_na(string arg)
{
	object me = this_player();
	object ob;
	string msg;

	if (!arg || arg != "布袋")
		return 0;
	if (query("dai") < 1)
		return notify_fail("人家装石灰正要用布袋呢，你偷完了怎么办？！\n");

	if (random(30) < 15)
		return notify_fail("突然，一个盐枭看见了你的动作，大声喝喊着：“有人偷袋子！！”\n");

	msg = "$N用脚拨了拨地上的布袋，顺手捡了起来";
	if (ob = present("bu dai", me)) {
		msg += "，将自己身上的破布袋丢在了一旁。\n";
		destruct(ob);
	}
	else
        msg += "。\n";
	message_vision(msg, me);
	ob = new(DAI);
	ob->move(me);
	if(add("dai",-1)<1) call_out("set_dai",86400);;
	return 1;
}
