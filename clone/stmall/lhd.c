//flowray 2005.6.20
#include <ansi.h>

inherit ITEM; 

void create()
{
	set_name (BLU"轮回丹"NOR, ({ "lunhui dan","dan"}));
	set("long","这是一颗"HIR"轮回丹"NOR"，使用("HIY"use"NOR")后能让你忘记过往。\n");

	set("unit","颗");
	set_weight(100);
	set("value",3000000);
		
	set("no_get",1);
	set("no_drop",1);
	set("no_get_from",1);
	
	
	//set("rest",4);
	set("desc","散去所有特殊技能，退出现有门派。");
	set("credit",2000);	
	setup();
}

int do_read(string arg);

void init()
{
	add_action("do_read","use");  
	do_read("dan");
}

int restrict() {
	object me = this_player();

  if(!me->query("family") 
  || !sizeof(me->query_skills()) ){
  	return notify_fail("你现在无法使用"+query("name")+"!\n");
  }
  
  if( me->query("env/party_abandon") != me->query("id") ){
    write(BLINK+HIR"使用轮回丹将放弃所有特殊技能。\n"
    +"如果确定请设置环境变量"HIY"set party_abandon "+ me->query("id")+HIR+"后，再次兑换。\n");
  	return notify_fail("你现在无法使用"+query("name")+"!\n");
  }

	return 1;
}

int sort_skill2(string sk) // add by augx@sj 8/27/2001
{
	return (SKILL_D(sk)->type() == "knowledge" && SKILL_D(sk)->type2() == "worker");
}

int sort_skill(string sk)
{
	return (SKILL_D(sk)->type() != "martial" && SKILL_D(sk)->type() != "array");
}

int sort_basic(string sk)
{
	string *valid_type = ({
	"force", "parry",  "dodge", "unarmed","claw",
	"cuff",  "finger", "strike","hand",   "leg",
	"sword", "blade",  "dagger","brush",  "spear",
	"hook",  "stick",  "staff", "club",   "throwing",
	"whip",  "axe",    "hammer","bow",
	});
	return member_array(sk, valid_type)!=-1;
}

int do_read(string arg)
{
	object me=this_player();
  int exp,nl,jl,ejl,i,j;
	string *sname,*allskills;
	

  if(!me->query("family") || !sizeof(me->query_skills()) ){
  	return notify_fail("你现在无法使用"+query("name")+"!\n");
  }
  
  if( me->query("env/party_abandon") != me->query("id") ){
    write(BLINK+HIR"使用轮回丹将放弃所有特殊技能。\n"
    +"如果确定请设置环境变量"HIY"set party_abandon "+ me->query("id")+HIR+"后，再次兑换。\n");
  	return notify_fail("你现在无法使用"+query("name")+"!\n");
  }
  
	me->add("quest/rp/party_abandon",1);
	nl=me->set("quest/rp/mnl",me->query("max_neili"));
	jl=me->set("quest/rp/mjl",me->query("max_jingli"));
	ejl=me->set("quest/rp/mejl",me->query("eff_jingli"));

	me->force_me("enable none");
	me->force_me("enforce none");
	me->force_me("prepare none");

	me->delete("tls");
	me->delete("family");
	me->delete("class");
	me->delete("env/party_abandon");
	
	allskills  = sort_array( keys(me->query_skills()), (: strcmp :) );

	sname  = filter_array( allskills, (: sort_skill2 :) );
	allskills -= sname;

	sname  = filter_array( allskills, (: sort_skill :) );
	allskills -= sname;
	
	sname  = filter_array( allskills, (: sort_basic :) );
	allskills -= sname;

	sname  = allskills;
	
  for(i=0;i<sizeof(sname);i++){
  	me->delete_skill(sname[i]);
  }
  
	write(HIW"你吃下一颗轮回丹，忘记了往昔种种，有若重获新生。\n"NOR);
	
	if(me->query("gender")=="无性") {
    me->set("gender","男性");
    write(HIY"你恢复了男儿真身。\n"NOR);
    log_file("static/reparty",sprintf(" %s(%s) e=-%d;sk=%O;GC=1。\n",me->query("name"),me->query("id"),exp,sname));
  }	
	log_file("static/reparty",sprintf(" %s(%s) e=-%d;sk=%O;GC=0。\n",me->query("name"),me->query("id"),exp,sname));
	
	destruct(this_object());
	return 1;
}