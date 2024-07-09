// Room: /d/shaolin/dmyuan2.c
// Date: YZC 96/01/19
// LinuX@SJ 04/03/01

#include <job_mul.h>
#include <ansi.h>

inherit ROOM;

void init()
{
	add_action("lingwu","lingwu");
}

void create()
{
	set("short", "达摩院后殿");
	set("long", @LONG
这是一座古朴的殿堂。西首的墙上挂满了各类武功图谱，不少白须白眉
的老僧们正端坐在图谱画轴之前，似乎在苦苦思索。南北山墙是高及屋顶的
大书架，走近细看，它们是各门各派的武功秘笈，屋正中摆着几张矮几诃和
几个蒲团，几位老僧正在入定中。
LONG
);

	set("no_fight", 1);
	set("exits", ([
		"south" : __DIR__"dmyuan",
	]));

	setup();
}

string *valid_type = ({
"force",	"parry",	"dodge",	"unarmed",	"claw",
"cuff",		"finger",	"strike",	"hand",		"leg",
"sword",	"blade",	"dagger",	"brush",	"spear",
"hook",		"stick",	"staff",	"club",		"throwing",
"whip",		"axe",		"hammer",
});

int lingwu(string arg)
{
	object me=this_player();
	string special;
	int bl,sl,i;
	mapping skills = me->query_skills();
	int j;
	
	if (!me->query("luohan_winner")) {
		write("你的十八罗汉大阵还没过，不能在此领悟。\n");
		return 1;
	}
	if (!arg) {
		write("你要领悟什么？\n");
		return 1;
	}
	if (!me->query_skill(arg,1)) {
		write("你不会这种技能。\n");
		return 1;
	}
	if (me->is_busy() || me->is_fighting()) {
		write("你正忙着呢。\n");
		return 1;
	}
	if (arg == "force" && me->query_skill("force",1)<200) {
		write("以你现在的基本内功修为，尚无法领悟基本内功。\n");
		return 1;
	}
	if (me->query("potential") < 2) {
		write("你从实战中得到的潜能已经用完了。\n");
		return 1;
	}
	if (!(special=me->query_skill_mapped(arg))) {
		write("你只能从特殊技能中领悟。\n");
		return 1;
	}

	bl=me->query_skill(arg, 1);

	if (bl > me->query("jing") || me->query("jing") * 100 / me->query("max_jing") < 50) {
		me->receive_damage("jing",bl/2);
		write("你没办法集中精神。\n");
		return 1;
	}

	sl=me->query_skill("literate",1);
	if ( bl > sl + 20) {
		write("你的读书写字等级不够，无法领悟更深一层的"+to_chinese(arg)+"。\n");
		return 1;
	}

	write("你瞑思苦想，对"+to_chinese(arg)+"的体会又深了一层。\n");
	
	if (me->query("registered") ==3 && me->query_temp("ggs/started") && me->query("ggs/started") ){
			if (!random(5))
			write("你参照自己在"HIG"鬼谷算术"NOR"上的修为，运神通数，觉得自己领悟起来事半功倍。\n");
			if (!random(10))
			tell_room(environment(me), me->name() + "正在参照"HIG"鬼谷算术"NOR"上的修为，运神通数，对着墙上进行领悟。\n", ({ me }));
		}
		
	i = me->query_int(1);

	foreach (string skill in keys(skills))
		if (member_array(skill, valid_type) != -1 && me->query_skill(skill, 1) > me->query("max_pot") - 105)
			j++;

	if ( j < 5 ) j = 5;

	me->receive_damage("jing", bl);
	me->add("potential", -2);

	i *= 4;

	if (me->query("age") < 30)
		i += 30-me->query("age");
	if (me->query("age") > 60)
		i += me->query("age") - 60;
	j = to_int( i * j * me->query("int") / 75.0);
	
	/*奖励控制开始*/
  if (me->query("registered")>2) j = j * SL_LINGWU_MUL * VIP_LINGWU_MUL /10000;
  else j = j * SL_LINGWU_MUL /100;
  /*奖励控制结束*/ 
  
  //工匠学武控制
  if(me->worker_skill("highest")>350) j = j*7/10;
	else if(me->worker_skill("highest")>220) j = j*8/10;
	if(me->worker_skill("limit")>600) j = j*8/10;
	//控制结束                
	
	if (me->query("registered") >2 && me->query_temp("ggs/started") && me->query("ggs/started") )
		me->improve_skill(arg, j*2);
	else
		me->improve_skill(arg, j);
		
	if (wizardp(me) && me->query("env/test"))
	write (sprintf("测试信息：鬼谷术增加 %d 点，其他增加 %d 点\n",j*2,j));
	return 1;
}
