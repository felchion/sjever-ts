//flowray 2005.6.20
#include <ansi.h>


inherit SPEC; 

void create()
{
	set_name (WHT"哈士奇"NOR, ({"husky"}));
	set("long",WHT"一只看起来看又呆又二的狗狗。\n");

	set("unit","只");
	set_weight(100);
	set("value",3000000);
		
	set("no_get",1);
	set("no_drop",1);
	set("no_get_from",1); 
	
	set("degree",1);
	set("flag","spec/husky");

	set("desc","一只看起来看又呆又二的狗狗，是找东西的能手。");
	set("credit",2);	
	setup();
}

int do_use();


int restrict() {
	object *obj;

	obj=filter_array(children("/clone/weapon/user_weapon"), (: 
	$1->query("owner") == this_player()->query("id") 
	&& environment($1) != this_player()
	:));

	obj+=filter_array(children("/clone/armor/user_armor"), (: 
	$1->query("owner") == this_player()->query("id") 
	&& environment($1) != this_player()
	:));
	
		
	if(!sizeof(obj)) return notify_fail("哈士奇呆呆地瞪着你，好象很不高兴的样子。\n");
	
	return  ::restrict();
}

void init()
{
	do_use();  
}

int do_use(){

	object *obj;
  
	obj=filter_array(children("/clone/weapon/user_weapon"), (: 
	$1->query("owner") == this_player()->query("id") 
	&& environment($1) != this_player()
	:));

	obj+=filter_array(children("/clone/armor/user_armor"), (: 
	$1->query("owner") == this_player()->query("id") 
	&& environment($1) != this_player()
	:));
			
	write(HIW"哈士奇一转眼就跑没影儿了，一会给你叼来了一"+obj[0]->query("unit")+obj[0]->query("name")+HIW"，然后不知道跑哪去了。\n"NOR);
  obj[0]->move(environment(this_player()));
  destruct(this_object());
	return 1;
}