//flowray 2005.6.20
#include <ansi.h>


inherit SPEC; 

void create()
{
  set_name (RED"洗髓丹"NOR, ({ "xisui dan","dan","xisui"}));
  set("long","这是一颗"RED"洗髓丹"NOR"，你可以服用(fu)增加臂力(str)根骨(con)身法(dex)悟性(int)。\n");

  set("unit","颗");
  set_weight(100);
  set("value",3000000);
    
  set("no_get",1);
  set("no_drop",1);
  set("no_get_from",1);
  
  set("degree",1);
  set("flag","spec/xisui");
  set("desc","消耗经验和声望，提高天赋一点并有驻颜效果。");
  set("credit",50);  
  setup();
}

void init()
{
  add_action("do_read","fu"); 
  
}

int restrict() {
	object me = this_player();

	if(me->query("re_gift") > 39)
	  return notify_fail("你已经无法使用洗髓丸来提高天赋。\n");
	if (me->query("combat_exp") < 30000000)
		return notify_fail("你修为太低无法承受洗髓丹的药力。\n");
	if(me->query("re_gift") > 19 && me->query("combat_exp") < 100000000)
	  return notify_fail("你修为太低无法承受洗髓丹的药力。\n");
	if (me->query("honor_point") < 5000)
		return notify_fail("你江湖声望太低无法服用洗髓丹。\n");	
	
	return  ::restrict();
}


int do_read(string arg)
{
  object me=this_player();
  
  if (!arg)
    return notify_fail("你要服用什么?\n");
  
  if (arg!="dex" && arg!="int" && arg!="con" && arg!="str")
    return notify_fail("你要增加什么天赋?\n");
  
	if(me->query("re_gift") > 39)
	  return notify_fail("你已经无法使用洗髓丸来提高天赋。\n");
	
	if (me->query("combat_exp") < 30000000)
		return notify_fail("你修为太低无法承受洗髓丹的药力。\n");
	
	if (me->query("honor_point") < 5000)
		return notify_fail("你江湖声望太低无法服用洗髓丹。\n");	

  if (me->query(arg) > 29)
    return notify_fail("你已经无法通过服药增加这种天赋了!\n");
      
  if (!restrict()) {return notify_fail("本周你已经不能使用"+this_object()->query("name")+"了。\n");}  

  me->add(arg,1);
  me->add("re_gift",1);
  me->add("combat_exp",-5000000);
  me->add("honor_point",-5000);
  
  if (me->query("age") - me->query("age_reyoung") > 20)
    me->add("age_reyoung",5);
  
  write(HIY"你服下一颗"HIC"洗髓丹"HIY"只觉自己自身经络火烧一样炽热，却又没有半分难受。\n"NOR);
  log_file("player/regift",sprintf("id:%s,gift:%s;total:%d\n",me->query("id"),arg,me->query("re_gift")));
  degree();
  return 1;
}