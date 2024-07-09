// xxxy.c 含沙射影 心心相印

#include <ansi.h>

inherit F_SSERVER;

string perform_name() {return HIY"心心相印"NOR;}

int perform(object me, object target)
{
        string msg,newroomfile,tarid;
        object weapon = me->query_temp("weapon");
	object room,newroom;
	object * obj;
        int lvl,i,j,local=0;
	string* dir=({ "e","w","n","s","ne","nw","se","sw","u","d","eu","wu","nu","su", 
		       "ed","wd","nd","sd","out","enter" });
	string* ldir=({ "east","west","north","south","northeast","northwest","southeast",
			"southwest","up","down","eastup","westup","northup","southup", 
		        "eastdown","westdown","northdown","southdown","out","enter" });
	string* fdir=({ "东边","西边","北边","南边","东北方","西北方","东南方","西南方",
                        "上方","下方","东上方","西上方","北上方","南上方","东下方","西下方",
			"北下方","南下方","外边","里面" });
	string* tdir=({ "西边","东边","南边","北边","西南方","东南方","西北方","东北方",
                        "下方","上方","西下方","东下方","南下方","北下方","西上方","东上方",
			"南上方","北上方","里面","外边" });
	string lfx,ffx,tfx,* str;

	if( (lvl=(int)me->query_skill("hansha-sheying", 1)) < 120 )
                return notify_fail("你的含沙射影不够娴熟，不会使用心心相印。\n");
        if( (int)me->query_skill("dulong-dafa", 1) < 120 )
		return notify_fail("你的毒龙大法功力不足，不能用心心相印。\n");
        if (!weapon || weapon->query("skill_type") != "throwing")
                return notify_fail("你现在无法使用心心相印。\n");
        if( (int)me->query("neili") < 150 )
                return notify_fail("你现在内力太弱，不能使用心心相印。\n");
	room=environment(me);
	if (room->query("no_fight"))
		return notify_fail("不准在这里战斗。\n");
	if (!me->query_temp("xydir",1)){
            if( !target ) target = offensive_target(me);
            if( !target
              ||  !target->is_character())
               	return notify_fail("你要射谁？\n");
    	    else
		local=1;
	}
	else{
 	  for(i=0;i<sizeof(dir);i++)
 	     if(dir[i]==me->query_temp("xydir",1)) break;
	     if(i>=sizeof(dir)){
		tarid=me->query_temp("xydir",1);
		str=explode(tarid,"&");
		if(sizeof(str)>1){
 		  tarid=str[0];
		  for(i=1;i<sizeof(str);i++){
			tarid+=" ";
			tarid+=str[i];
 		  }
		}
		for(i=0;i<sizeof(ldir);i++){
 		  lfx=ldir[i];
		  if(room->query("exits/"+lfx)){
			newroomfile=room->query("exits/"+lfx);
			if(!objectp(newroom=find_object(newroomfile))) 
			continue;
 		        obj = all_inventory(newroom);
        		for(j=0;j<sizeof(obj);j++){
				string *ids = obj[j]->parse_command_id_list();
				if (member_array(tarid, ids) != -1) 
				break;
			}
			if(j<sizeof(obj)) break;
		  }
		}
		if(i>=sizeof(ldir))
			return notify_fail("你要射谁？\n");
		else{
			target=obj[j];ffx=fdir[i];tfx=tdir[i];
		}
	     }  
	     else{
		lfx=ldir[i];ffx=fdir[i];tfx=tdir[i];
		if(!room->query("exits/"+lfx))
		     return notify_fail("你要往哪里射？\n");
		newroomfile=room->query("exits/"+lfx);
		if(!objectp(newroom=find_object(newroomfile))) 
		newroom=load_object(newroomfile);
		obj = all_inventory(newroom);
        	for(i=0;i<sizeof(obj);i++){
			if(me->is_killing(obj[i]->query("id")))	break;
		}
		if(i>=sizeof(obj))
               		return notify_fail("你要射谁？\n");
		target=obj[i];
	     }
             if(newroom->query("no_fight"))
		   return notify_fail("不能往安全区发暗器。\n");
	}
	if(local == 1)	
        	msg = HIY "$N使出一招「心心相印」，数十枚"+weapon->query("name")+"兵分两路，一路射向$n前胸，另一路飞到$n身后回头击向其后心。\n";
	else
		msg = HIY "$N使出一招「心心相印」，数十枚"+weapon->query("name")+"兵分两路，射向"+ffx+"\n";

        message_vision(msg, me, target);
	me->add("neili", -100);
        me->add("jingli", - 40);

	target->start_busy(1);
        me->start_busy(2+random(2));

	me->set_temp("xxxy",1);

	if(local==0){
	    me->move(newroom);
	    msg = HIY "忽然从"+tfx+"有数十枚"+weapon->query("name")+"兵分两路，射向$n!\n";
	    message_vision(msg, me, target);
	}
	else{
	    me->add_temp("apply/attack", lvl/5);
	}
      	COMBAT_D->do_attack(me, target, me->query_temp("weapon"), 1);  
      	COMBAT_D->do_attack(me, target, me->query_temp("weapon"), 1);
	if(local==0){
	    me->move(room);
	    me->delete_temp("xydir");	
	}
	else
		me->add_temp("apply/attack", -lvl/5);
	if(!me->is_fighting(target)){
	    me->fight_ob(target);
		if (userp(me) && userp(target))
			target->set_temp("other_kill/"+me->query("id"), 1);
	    target->kill_ob(me);
	}
      	me->delete_temp("xxxy");
      	me->start_perform(4,"心心相印");
        return 1;
}
