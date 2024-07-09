
#include <ansi.h>
#include <weapon.h>
inherit HAMMER;

void create()
{
        set_name(HIY"韦兰之锤"NOR, ({ "weilan's hammer", "chui"}));
        set_weight(60000);
        if( clonep())
               set_default_object(__FILE__);
        else {
               set("unit", "柄");
               set("long",@LONG
这是韦兰铁匠打造出传奇式的剑“乔依乌斯”所用的铁锤，可不幸被小偷窃去，
LONG
               );
               set("value", 0);
               set("material", "steel");
               set("dazao_award",1);
               set("no_drop",1);
               set("wield_msg", "$N拿起一柄$n。\n");
               set("unwield_msg", "$N将手中的$n放下。\n");
        }
        init_hammer(20);
}

void init()
{
	add_action("do_fix",({"fix","xiu"}));
}

int do_fix(string arg)
{
	object me = this_player();
	object obj,weapon;
	int delay = 15 + random(30);
	
	if(environment(this_object())!=me) return 0;
		
	if (me->is_busy()) return notify_fail("你正忙着呢！\n");
        if (me->is_fight()) return notify_fail("你正忙着呢！\n");
        if (!arg) return notify_fail("你要修什么？\n");
        if ( !objectp(obj = present(arg, me)))
        	return notify_fail("你身上没有这样东西。\n");
        
        if ( !objectp(weapon=me->query_temp("weapon")) || weapon!=this_object() )
                return notify_fail("你必须装备"+name()+"才能来维修兵器。\n");
                
        if(!mapp(obj->query_temp("apply/weapon_mp_old")) && (!mapp(obj->query("weapon_prop")) || !(int)obj->query("imbued") ) )
		return notify_fail(name()+"只能维修由玩家制造的兵器。\n");
        if (!obj->query("broken") &&  obj->query("weapon_mp/dur") >= obj->query("weapon_mp/max_dur") )
                return notify_fail("这件兵器完好无损，无需修理。\n");

	message_vision(HIY"$N开始仔细的维修"+obj->query("name")+HIY"，不时用"+name()+"敲敲打打......\n\n"NOR, me);        
	me->start_busy(delay);
	call_out("xiu_finish",delay*2-1,me,obj);
        return 1;
}


void xiu_finish(object me,object obj)
{
        int i;  
        if(!objectp(me) || !userp(me) || !objectp(obj) ) return;
        if(!this_object() ||  environment(this_object())!=me) return;        
	obj->delete("broken");
	obj->delete("treasure");
				        
        if(obj->query_temp("apply/weapon_mp_old")) obj->set("weapon_mp",obj->query_temp("apply/weapon_mp_old") );
	if(obj->query_temp("apply/name_old")) obj->set("name",obj->query_temp("apply/name_old"));
	if(obj->query_temp("apply/weapon_prop_old")) obj->set("weapon_prop",obj->query_temp("apply/weapon_prop_old"));
	if(obj->query_temp("apply/long_old")) obj->set("long",obj->query_temp("apply/long_old"));
	
	obj->delete_temp("apply");		
        message_vision(HIY"$N仔细的维修"+obj->query("name")+HIY+"，总算大致恢复了它的原貌。\n"NOR, me);
        WORKER_D->check_impove(me,"duanzao",me->query_int()/2,2);
        
        i = obj->query("weapon_mp/max_dur");
        obj->set("dur",i);
        obj->set("weapon_mp/dur",i);
        obj->set("weapon_mp/max_dur",i);
                
        obj->set("weapon_mp/weapon_prop",copy(obj->query("weapon_prop")));
        obj->save_weapon(me);
        
        unequip();
        destruct(this_object());
}