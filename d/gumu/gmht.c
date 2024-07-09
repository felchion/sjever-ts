// Room: /u/xdd/gumu/gmhout.c
// Modify By River 98/08/29
#include <ansi.h>
#include <room.h>
inherit ROOM;

void create()
{
	set("short", HIG"后堂"NOR);
	set("long", @LONG
堂上也是空荡荡的没什么摆设，正前方写着一行大字（zi），东西两壁都
挂着一幅画。西首 (westpic)的画像是两个女郎，正是古墓派的师尊林朝英和
师祖婆婆的肖像，东侧（eastpic）画像中却是一个道士。 
LONG        );

	set("exits", ([
		"east" : __DIR__"gmqs",
		"west" : __DIR__"gmzt",
		"north" : __DIR__"gmws1",
		"south" : __DIR__"gmws",
	]));

	set("item_desc", ([
		"westpic" : "西壁画中是两个姑娘。一个二十五六岁，正在对镜梳装，另一个是十四五岁的\n"+
			"丫鬟，手捧面盆，在旁侍候。画中镜里映出那年长女郎容貌极美，秀眉入鬓，\n"+
			"眼角之间却隐隐带著一层杀气。\n",          
		"eastpic" : "只见像中道人身材甚高，腰悬长剑，右手食指指着东北角，只是背脊向外，面\n"+
			"貌却看不见。\n",
		"zi" : HIR"\t入我古墓，先唾重阳，欲成神功，拜我祖师。\n"NOR,
	]));
	create_door("south", "石门", "north", DOOR_CLOSED);
	setup();
}

void init()
{
	add_action("do_ketou", "ketou"); 
	add_action("do_tuo", "tuo");
}

int can_betray(object me)
{
	mapping skills = me->query_skills();

	if (sizeof(skills) < 1)
		return 1;
	foreach (string skill in keys(skills))
		if (SKILL_D(skill)->valid_enable("force"))
			return 0;
	return 1;
}

int do_ketou(string arg)
{
	mapping fam;
	object key, me = this_player();
	fam = me->query("family");
	if (me->is_busy() || me->is_fighting())
		return notify_fail("你正忙着哪！\n");
	if( !arg || arg != "westpic") return 0;
		message_vision("$N向西跪倒，双膝及地，恭恭敬敬向祖师婆婆的画像磕了一个响头. \n", me);
	me->add_temp("marks/ketou", 1);
	if( me->query_temp("marks/tuo") == 2 
	 && me->query_temp("marks/ketou") == 4 ){
		if( fam && fam["family_name"] != "古墓派" && !can_betray(me))
			return notify_fail("你是怎么溜进古墓来的？\n"); 
		if( fam && fam["family_name"] == "古墓派")
			return notify_fail("你已经是古墓派的弟子了，就不用多磕头了。\n");
		if( ! fam && me->query("expell"))
			return notify_fail("你是怎么溜进古墓来的？？\n");
		if ( fam && fam["family_name"] != "古墓派") {
			int count = me->add("betrayer", 1);
			log_file("static/BETRAY",
				me->query("name")+"("+me->query("id")+")第"+count+"次从"
				+me->query("family/family_name")+"叛入古墓派\n",
				me
			);
			if (count <= 1)
				count = 10;
			else if (count == 2)
				count = 4;
			else
				count = 2;
			me->add("combat_exp", - me->query("combat_exp") / count);
			me->setup();
		}
		me->create_family("古墓派", 5, "传人");
		me->set("class","sworder");
		tell_object(me,HIR"\n恭喜你自愿成为一名古墓弟子。\n\n"NOR);
		me->start_busy(2);
		me->delete_temp("marks/tuo");
		me->delete_temp("marks/ketou");
		return 1;
	}
	if( me->query_temp("marks/ketou") == 15 ){
		me->delete_temp("marks/ketou");
		if(me->query_temp("marks/goldkey"))
			return notify_fail(HIR"你不是已经拿过了吗？怎么还要拿？\n"NOR);
		key=unew(__DIR__"obj/goldkey");
		if(!clonep(key))
			return notify_fail (HIR"地上露出一方小孔,但里面空空如也,想必有人先你下手了. \n"NOR);  
		if(clonep(key) && key->violate_unique()){
			destruct(key);
			return notify_fail (HIR"地上露出一方小孔,但里面空空如也,想必有人先你下手了. \n"NOR);
		}
		message_vision(HIY"$N面前的石地突然下沉,露出一把金钥匙,$N得到一把金钥匙. \n"NOR, me);
		me->set_temp("marks/goldkey", 1);
		key->move(me);
	}
	return 1;
}

int do_tuo(string arg)
{
	object me = this_player();
	if (me->query_temp("marks/tuo") == 2)
		return notify_fail("你唾够了没有？\n");
	if ( arg=="eastpic") {
		message_vision("$N朝王重阳的画像狠狠唾了一口. \n", me);
		me->add_temp("marks/tuo", 1);
		return 1;
	}
	return notify_fail("你不要随便乱唾口水啊！\n");
}
