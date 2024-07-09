// Room: d/nanyang/kedian.c
// Lklv Modify 2001.10.10

inherit ROOM;
void create()
{
	set("short", "客栈");
	set("long", @LONG
这是附近远近闻名的悦来客栈，生意非常兴隆。外地游客多选择在这里落
脚，你可以在这里打听到各地的风土人情。店小二里里外外忙得团团转，接待
着南腔北调的客人。墙上挂着一个牌子(paizi)。
LONG
	);

	set("valid_startroom", 1);
	set("item_desc", ([
		"paizi" : "楼上雅房，每夜五两白银。\n",
	]));
	set("objects", ([
		__DIR__"npc/xiao-er" : 1,
	]));
	set("exits", ([
		"west" : __DIR__"xujiaji",
		"up" : __DIR__"kedian2",
	]));
	setup();
}

int valid_leave(object me, string dir)
{
	if ( !me->query_temp("rent_paid") && dir == "up" )
		return notify_fail("店小二一下挡在楼梯前，白眼一翻：怎麽着，想白住啊！\n");
	if ( me->query_temp("rent_paid") && dir == "west" )
		return notify_fail("店小二跑到门边拦住：客官已经付了银子，怎麽不住店就走了呢！\n"
	+"旁人还以为小店伺候不周呢！\n");

	return ::valid_leave(me, dir);
}
