
//zongtai 总台

#define SJSZ_D	"/cmds/leitai/bwdh/";

inherit ROOM;
inherit F_SAVE;

#include <ansi.h>
#include <mudlib.h>
#include <room.h>

string query_save_file()
{
	return "/log/shijian";
}

string gaoshi();

mapping sjsz;

void create()
{
	restore();
	if (!mapp(sjsz))
		sjsz = ([ ]);
	set("short",HIR"庄门"NOR);
	set("long", @long
         [21m/J______________________L\
            /||||||||||||||||||||||||||\
        {\/LLLLLLLLLLLLLLLLLLLLLLLLLLLLLL\/}
         ^^^^||^^^^ [24m【试剑山庄】[21m ^^^^||^^^^
          /||||||||||||||||||||||||||||||\
          ^^^||^^^^^^^^^^||^^^^^^^^^^||^^^
             ||          ||          ||
   -I-I-I-I-I||          ||          ||I-I-I-I-I-
   I-I-I-I-I-||        [24m◎[21m||[24m◎[21m        ||-I-I-I-I-I
   -I-I-I-I-I||          ||          ||I-I-I-I-I-
   [21mI-I-I-I-I-||__________||__________||-I-I-I-I-I
  [25m/__|__|__|__|__[25m|________________|[25m__|__|__|__|__\
 /__|__|__|__|_|_[25m|________________|[25m_|__|__|__|__|_\[0m
long
);
	set("exits", ([
		"enter" : __DIR__"gate",
		"south" : "/d/changle/road3",
	]));

	set("item_desc", ([
		"gaoshi":	(: gaoshi :),
	]));

	set("outdoors", "试剑山庄");
	set("no_fight", 1);
	set("valid_startroom", 1);
	set("no_sleep_room", 1);
	setup();
}

void reset()
{
	::reset();
	set("time1", sjsz["time1"]);
	set("time2", sjsz["time2"]);
	set("time3", sjsz["time3"]);
}

void init()
{
	if ( wiz_level(this_player()) == 6 ) {
		add_action("do_jianshi", "jianshi");
		add_action("do_set", "timeset");
	}
	remove_call_out("sjsz_reset");
	call_out("sjsz_reset", 3);
}

int valid_leave(object me,string dir)
{
	object *inv;

	inv = filter_array(deep_inventory(me), (: userp :));

	if (sizeof(inv) && dir == "enter" )
		return notify_fail("你想贩卖人口么？\n");

	if ( me->query("registered") < 3 && dir == "enter" )
		return notify_fail("只有正式用户才能进入试剑山庄。\n");

	if ( me->query("bxsj_bonus/no_pk") && dir == "enter")
		return notify_fail("您现在的状态无法进入试剑山庄。\n");

	if ( me->query("no_quest"))
		return 0;

	return ::valid_leave(me, dir);
}


int do_jianshi()
{
	object me = this_player();
	string str;

	if ( ! sjsz["find_room"] )
		str = "没有任何信息。\n";
	else {
		str = ctime(time())[10..19];
		str += sjsz["find_room"];
		str += ", " + query("jianshi") + "\n";
	}
	tell_object(me, str);
	return 1;
}

int do_set(int time1, int time2, int time3)
{
	object me = this_player();
	mixed *local;

	local = localtime(time());

	time1 = random(9);			// 0-8
	time2 = time1+4+random(13-time1);	// 4-16
	time3 = time2+4+random(16-time2);	// 8-20

	sjsz["time1"] = time1;
	sjsz["time2"] = time2;
	sjsz["time3"] = time3;
	sjsz["day"] = local[6];
	save();

	set("time1", time1);
	set("time2", time2);
	set("time3", time3);

	message("channel", HIW"\n\t"+CHINESE_MUD_NAME+HIW"的试剑山庄挖宝时间已经自动分布完成。\n\n"NOR, users());

	if (objectp(me))
		tell_object(me, "你成功设置时间为："+time1+"点, "+time2+"点, "+time3+"点。\n");
	return 1;
}

int sort_skill(string file)
{
	if ( file[<2..<1] != ".c" ) return 0;
	if ( sscanf(file, "%*s%*d.c") != 2)  return 0;
	if ( sscanf(file, "%*slang%*d.c")) return 0;

	return 1;
}

void sjsz_reset()
{
	mixed *local;
	int time1, time2, time3, i, j;
	string *file;
	object room;

	local = localtime(time());

	if ( local[6] != sjsz["day"])
		do_set(time1, time2, time3);

	if ( local[2]%3 < 1 && local[2] > 8 && sjsz["find_time"]+3600 < time()) {
		file = get_dir("/cmds/leitai/bwdh/");
		file = filter_array(file, (: sort_skill :) );
		j = sizeof(file);

		for (i=0 ; i < j; i ++) {
			if ( !room = find_object("/cmds/leitai/bwdh/"+(file[i])) )
				room = load_object("/cmds/leitai/bwdh/"+(file[i]));
			if ( room->query("have_quest_room")) {
				set("jianshi", file_name(room));
				remove_call_out("sjsz_reset");
				call_out("sjsz_reset", 3);
				return;
			}
		}

		i = random(j);

		if ( !room = find_object("/cmds/leitai/bwdh/"+(file[i])) )
			room = load_object("/cmds/leitai/bwdh/"+(file[i]));

		room->set("have_quest_room", 1);

		sjsz["find_time"] = time();
		sjsz["find_room"] = file_name(room);
		save();

		set("jianshi", i + ", "+j+", "+ ctime(time())[10..19]);
		message("channel", HIW"\n\t"+CHINESE_MUD_NAME+HIW"的试剑山庄内听说有绝世宝物出现！\n\n"NOR, users());
	}
	remove_call_out("sjsz_reset");
	call_out("sjsz_reset", 3);
}

string gaoshi()
{
	string str;

	if ( !query("time1") && !query("time2"))
		return "试剑山庄暂时没有开放挖掘宝物。\n";

	str = sprintf(HIY"\n\t※※※"HIW"今日开放的掘宝时间"HIY"※※※\n");
	str += sprintf("\t※                          ※\n");
	str += sprintf("\t※     %2d:00  -  %2d:00      ※\n", query("time1"), query("time1") + 4);
	str += sprintf("\t※     %2d:00  -  %2d:00      ※\n", query("time2"), query("time2") + 4);
	str += sprintf("\t※     %2d:00  -  %2d:00      ※\n", query("time3"), query("time3") + 4);
	str += sprintf("\t※                          ※\n");
	str += sprintf("\t※※※※※※※※"HIC"★试剑山庄★"HIY"※\n\n"NOR);

	return str;
}
