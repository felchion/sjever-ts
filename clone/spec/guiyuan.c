//flowray 2005.6.16
#include <ansi.h>


inherit SPEC;

void create()
{
	set_name (HIY"归元丹"NOR, ({ "guiyuan dan","guiyuan","dan"}));
	set("long","这是一颗"HIY"归元丹"NOR"，你可以服(fu)恢复特殊内功、精力和内力修为。\n");

	set("unit","颗");
	set_weight(100);
	set("value",3000000);
		
	set("no_get",1);
	set("no_drop",1);
	set("no_get_from",1);
	
	set("degree",1);
	set("flag","spec/guiyuan");
	//set("rest",10);
	set("desc","恢复特殊内功、精力和内力的修为。");
	set("credit",500);
	setup();
}

int do_use(string arg);

int restrict() {
	object me = this_player();
  string skill;
  
  if(!me->query("quest/rp/party_abandon"))
  return notify_fail("你现在无法使用"+query("name")+"！\n");
  
  
  
	if(!mapp(me->query_skill_map()))
	return notify_fail("你现在无法使用"+query("name")+"！\n");
	
	skill = me->query_skill_map()["force"];
	
	if(!stringp(skill) || me->query_skill(skill,1)< 1
	|| me->query("max_pot") - me->query_skill(skill,1) < 200
	//|| me->query("quest/rp/mnl") < me->query("max_neili")
	)
	return notify_fail("你现在无法使用"+query("name")+"！\n");
	
	return 1;
}

void init()
{
	do_use("");
  
}

int do_use(string arg)
{
	object me=this_player();
	string skill;
      	
	if (!restrict()) {return notify_fail("你现在暂时不能使用"+this_object()->query("name")+"了。\n");}
  
  skill = me->query_skill_map()["force"]; 
//	me->set("max_neili", me->query("quest/rp/mnl"));
//	me->set("max_jingli", me->query("quest/rp/mjl"));
//	me->set("eff_jingli", me->query("quest/rp/mejl"));
	me->set_skill(skill,me->query("max_pot")-100);
	
	write(HIW"你服下一颗"HIY"归元丹"HIW"恢复了原来的内功修为！\n"NOR);
	degree(); 
	return 1;
}	 