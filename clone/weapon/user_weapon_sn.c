// user_weapon.c 自制武器

inherit EQUIP;

int wield()
{
	object owner;
	if(!(owner = environment()) return 0;
	if( !owner->is_character() ) return 0;
	if (query("saved") != me->query("id")) return notify_fail("这不是你的兵器。\n");
	
	return ::wield();
}

mixed no_get()
{
	object me = this_player();

	if (!me) return 0;
	if (query("saved") != me->query("id")) return "这不是你的兵器。\n";
	return 0;
}

void create()
{
	set_name("普通剑", ({ "user weapon" }));
	set_weight(5000);
	if (clonep()) {
		set_default_object(__FILE__);
		set("no_get", (: no_get :));
	} else {
		set("unit", "柄");
		set("value", 0);
		set("material", "steel");
		set("wield_msg", "$N拿出$n握在手中。\n");
		set("unwield_msg", "$N放下手中的$n。\n");
		set("wield_maxneili", 100000);
		set("wield_neili", 10);
		set("wield_str", 20);
		set("imbued", 1);
	}
	set("flag", 4);
	set("verbs", ({ "slash", "hack", "slice", "thrust" }) );
	set("weapon_prop/damage", 1);
	set("skill_type", "sword");
	setup();
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

int weapon_fix()
{	
	delete("weapon_mp/broken");
	delete("broken");	
}
