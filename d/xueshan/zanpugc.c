// Room: zanpugc.c
// Lane@ln 2004.7.1

#include <ansi.h>
inherit ROOM;

int sort_file(string file)
{
	int i;
	if( file[<2..<1] != ".c" ) return 0;
	if( !sscanf(file, "%*s.c") )  return 0;
	if( sscanf(file, "%*s_noshow.c") )  return 0;
	i = strlen(file);
	while(i--){
		if( file[i] == '.' ) continue;
		if( file[i] < 'a' || file[i] > 'z' )
			return 0;
	}

	return 1;
}

void create()
{
	set("short", "赞普广场");
	set("long", @LONG
这里是皇城中心的一片广场，广场中心有座吐蕃赞普的石雕像，是名。雕像
下面有不少买卖人起劲的吆喝着。广场东面是正在建筑中的吐蕃新皇宫，一队队
的民夫推着装满石料木料的推车走过去，几名吐蕃武士把守着路口，警惕的注视
着来来往往的行人。西边是佛照门，是通往吐蕃护国禅寺大轮寺的毕经之路。
LONG);

	set("exits", ([
		"west" : __DIR__"fozhaomen",
		"east" : __DIR__"huanggong",
		"north" : __DIR__"jiedao3",
		"south" : __DIR__"jiedao2",
	]));

	set("objects", ([__DIR__"npc/tufanwushi" : 3,]));
	set("outdoors", "雪山");
	set("job_lane", 1);
	setup();
}

//void init()
//{
//	object me = this_player();
//
//	if( (int)me->query("combat_exp") > 100000 && me->query("family/family_name") == "大轮寺" 
//	 && random(me->query("kar")) > 10 && query("job_lane") >= 1 && random(50) == 20 ) {
//		add("job_lane", -1);
//		call_out("do_job", 1 + random(5), me);
//	}
//} 

void do_job(object me)
{
	object *list;
	object target, obj;
	string *file, where;

	if( !me ) return;

	if( query_temp("count") > 10 ) {
		delete_temp("count");
		return;
	}

	list = filter_array(users(), (: $1->query("combat_exp")>=100000 :));
	target = list[random(sizeof(list))];

	if( (!environment(target)) || target->query("env/invisibility")
	 || !interactive(target) || target->query("id") == me->query("id")
	 || target->query("family/family_name") == me->query("family/family_name") ) {
		add_temp("count",1);
		call_out("do_job", 1, me);
		return;
	} else {

		file = get_dir("d/xueshan/");
		if( sizeof(file) > 0 ) file = filter_array(file, (: sort_file :) );

		where = "/d/xueshan/" + file[random(sizeof(file))];

		tell_object(me, YEL+where+"\n"NOR);

		obj = new(__DIR__"job_npc");
		obj->do_copy(target);
		obj->move(where);
		obj->set("family/family_name", target->query("family/family_name"));
		obj->set("kill_party", me->query("family/family_name"));
		CHANNEL_D->do_channel(me, "party", "听说" + target->query("family/family_name") + 
		HIR + target->query("name") + HIW + "(" + target->query("id") + ")" + HIG
		"的"BLU"影子武士"HIG"前来雪山捣乱，传闻在" + HIR + environment(obj)->query("short") + HIG + "一带出现过。\n");
	}
}
