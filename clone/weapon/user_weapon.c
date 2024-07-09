// user_weapon.c 自制武器
// create by snowman@sj
// modify by augx@sj   9/25/2001
// Modified by Ciwei@SJ

inherit EQUIP;

void save_weapon(object me);
void delete_weapon(object me);

void create()
{
	seteuid(ROOT_UID);
	set_name("普通剑", ({ "user weapon" }));
	set_weight(5000);
	if (clonep())
		set_default_object(__FILE__);
	else {
		set("unit", "柄");
		set("value", 0);
		set("material", "steel");
		set("wield_msg", "$N拿出$n握在手中。\n");
		set("unwield_msg", "$N放下手中的$n。\n");
		set("wield_maxneili", 0);
		set("wield_neili", 0);
		set("wield_str", 20);
		set("imbued", 1);
	}
	set("flag", 4);
	set("verbs", ({ "slash", "hack", "slice", "thrust" }) );
	set("weapon_prop/damage", 1);
	set("skill_type", "sword");
	//setup();
}

int weapon_setup(mapping mp)
{
	string *vb;
	int i;

	if( !clonep(this_object()) )
		return 0;
	if( !mapp(mp) ) return 0;

	vb = keys(mp);

	for( i = 0; i < sizeof(vb); i++ )
		set(vb[i], mp[vb[i]]);

	set_name(mp["name"], mp["id"]);
	set_weight(mp["weight"]);
	set("actions", (: call_other, WEAPON_D, "query_action" :) );

	return 1;
}

int move(mixed dest, int silently)
{
	object src = environment();
	int result, i=0, j;

	if( (result = ::move(dest, silently)) ) {
			if (src && userp(src)) { delete_weapon(src); i=1; }
			if (dest && userp(dest)) { save_weapon(dest); i=1; }
		if (i) {
			if(src)  set("move/src",src->query("id"));   else set("move/src","");
			if(dest) set("move/dest",dest->query("id")); else set("move/dest","");
			set("move/time",time());
		}
	}
	return result;
}

void cant_keep(object me)
{
	if( !me || !userp(me) || environment()!=me || !environment(me) ) return;

		message_vision("但是$N根本拿不住，" + name() + "掉了下来！\n", me);
    if(!(this_object()->move(environment(me)))) {
			message_vision("一下子就失去了踪影！\n",me);
			destruct(this_object());
	}
}

void save_weapon(object me)
{
	mapping mp;
	object wp = this_object();

	mp = wp->query("weapon_mp");
	if ( !mapp(mp) || !mapp(wp->query("weapon_prop")) ) return;
	if(stringp(query("owner"))
	&& query("owner")!="0"
	&& query("owner")!=getuid(me)) return;
	
	mp["user"] = query("user");
	mp["ltime"] = query("ltime");
	me->set("worker/"+mp["save_id"], copy( WORKER_D->convert_weapon2user(mp) ));
	if(userp(me)) me->save();
}

void delete_weapon(object me)
{	
	if(stringp(query("owner")) && query("owner")==getuid(me)) return;
	me->delete( "worker/" + query("weapon_mp/save_id") );
}

void delete_weapon2(object me)
{
  if (query("imbued")>3 || query("wlvl")) return;
	me->delete( "worker/" + query("weapon_mp/save_id") );
	if(stringp(query("owner")) && query("owner")!="" && query("owner")!="0" && query("owner")!=getuid(me))
		WORKER_D->deleteweapon(query("owner"),"你的"+name()+"因为被"+me->name()+"损坏而被自动删除，已经不再是你的私有武器。\n",query("weapon_mp/save_id") );
}

void remove(string euid)
{
	object ob = environment(this_object());

	if(!clonep(this_object())) {
		::remove(euid);
		return;
	}

	if ( !(int)query_temp("quit_dest") ) {
		if( ob && userp(ob) ) delete_weapon(ob);
		//if( query("weapon_mp/imbued") > 3 )
			//WORKER_D->addto_pool("delete",query("weapon_mp/save_id")," ");

		if( query("weapon_mp/imbued") > 2 ) {
			// save_id : time : src : dest : user : euid
			log_file("career/delete", sprintf("%-19s:%d:%s:%s:%s:%s\n",
				query("weapon_mp/save_id"),
				query("move/time"),
				(string)query("move/src"),
				(string)query("move/dest"),
				objectp(ob)?ob->query("id"):"",
				euid
				), ob);
		}
	}
	else {
		if( ob && userp(ob) ) save_weapon(ob);
		//if( query("weapon_mp/imbued") > 3 )
			//WORKER_D->addto_pool("update",query("weapon_mp/save_id"),sprintf("%d",time()));
	}

	::remove(euid);
}
/*在一段时间内，别人不能get Kill NPC后掉在地上的兵器。*/
void delete_owner()
{
   if (query("tmp_owner"))
    {
    remove_call_out("delete_tmpowner");
		call_out("delete_tmpowner",50);
		}
}

void delete_tmpowner()
{
	if (!this_object()) return;
	if (!query("tmp_owner")) return;
	
	delete("tmp_owner");
	return;
}