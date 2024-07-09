#include <ansi.h>

int main(object me,string arg)
{
	object obj,weapon;
	int delay,level,flag;
	int cost_jing,cost_money;
	mapping myskill;
	string type;

	if( me->is_busy() ) return notify_fail("你正忙着呢！\n");

	if( me->is_fight()) return notify_fail("你正忙着呢！\n");

	if( !arg ) return notify_fail("你要修什么？\n");

	if( !objectp(obj = present(arg, me)) )
		return notify_fail("你身上没有这样东西。\n");

	if( mapp(obj->query("weapon_prop")) && (int)obj->query("imbued") ) {
// 修理兵器
		if( !objectp(weapon=me->query_temp("weapon")) || weapon->query("id")!="tiechui" )
			return notify_fail("你必须装备铁锤才能来维修兵器。\n");

		if( obj->query("weapon_mp/dur") >= obj->query("weapon_mp/max_dur") )
			return notify_fail("这件兵器完好无损，无需修理。\n");

		if( obj->query("weapon_mp/dur") <= 0  )
			return notify_fail("这件兵器已经彻底损坏了，无法修理。\n");

		flag = strsrch(obj->query("save_id"),getuid(me))==0;

		level = (int)me->query_skill("duanzao",1);
		switch( (int)obj->query("imbued") ) {
			case 1: if(level<50)  return notify_fail("你的锻造技能不够！\n"); break;
			case 2: if(level<80)  return notify_fail("你的锻造技能不够！\n"); break;
			case 3: if(level<100)  return notify_fail("你的锻造技能不够！\n"); break;
			case 4: if(level<150) return notify_fail("你的锻造技能不够！\n"); break;
			case 5: if(level<200) return notify_fail("你的锻造技能不够！\n"); break;
				return notify_fail("传说中的武器你修理不了。\n");
			default:return notify_fail("这个就是传说中的超级武器？\n");               
		}

		myskill = me->query("forging");//当它Quest了

		if (obj->id("xiao")) {
			if ( !mapp(myskill) || member_array("xiao",keys(myskill)) == -1 )
				return notify_fail("对于这种武器，您了解不多，无法修理！\n");
		}
		else if( !mapp(myskill) || member_array(obj->query("skill_type"),keys(myskill)) == -1 )
			return notify_fail("对于这种武器，您了解不多，无法修理！\n");
	
		cost_money = obj->query("value")/2;
	
		if( !flag ) cost_money += cost_money;
		cost_jing = obj->query("weapon_mp/max_dur") - obj->query("weapon_mp/dur");
		if( !flag ) cost_jing +=  cost_jing;
		cost_jing /= 4;
		cost_jing = cost_jing*3/4;
		if( me->query("jingli")<cost_jing || me->query("jing")<cost_jing )
			return notify_fail("你现在精神状态不佳，还是等会再修理吧。\n");
	
		switch(MONEY_D->player_pay(me,cost_money))
		{
			case 1:break;
			default:return notify_fail("你带的零钱不够了！你需要"+MONEY_D->money_str( cost_money )+"修理费用。\n");
		}
	
		write("你本次修理花费"+MONEY_D->money_str(cost_money)+"。\n");
	     
		me->set_temp("last_damage_from", "修理高级武器熬尽心机累");
		me->receive_damage("jing", cost_jing );
		me->receive_damage("jingli", cost_jing );

		message_vision(HIY"$N开始仔细的维修"+obj->query("name")+HIY"，不时用铁锤敲敲打打......\n\n"NOR, me);
		delay = 15 + random(30);
		if( wizardp(me) && (int)me->query("env/test") ) delay = 3;
		me->start_busy(delay);
		weapon->use();
		call_out("xiu_weapon_finish",delay*2-1,me,obj);
		return 1;
        
	} else if( mapp(obj->query("armor_prop")) && (int)obj->query("imbued") ) {
// 修理防具
		if( !objectp(weapon=me->query_temp("weapon")) || weapon->query("id")!="jian dao" )
			return notify_fail("你必须装备剪刀才能来修补防具。\n");

		if( obj->query("armor_mp/dur") >= obj->query("armor_mp/max_dur") )
			return notify_fail("这件防具完好无损，无需修补。\n");

		if( obj->query("armor_mp/dur") <= 0  )
			return notify_fail("这件防具已经彻底损坏了，无法修补。\n");

		flag = strsrch(obj->query("save_id"),getuid(me))==0;

		level = (int)me->query_skill("zhizao",1);
		switch( (int)obj->query("imbued") ) {
			case 1: if(level<80)  return notify_fail("你的织造技能不够！\n"); break;
			case 2: if(level<120)  return notify_fail("你的织造技能不够！\n"); break;
			case 3: if(level<160)  return notify_fail("你的织造技能不够！\n"); break;
			case 4: if(level<200) return notify_fail("你的织造技能不够！\n"); break;
			case 5: //if(level<300) return notify_fail("你的织造技能不够！\n"); break;
				return notify_fail("传说中的防具你修补不了。\n");
			default:return notify_fail("这个就是传说中的超级防具？\n");               
		}

		switch( obj->query("armor_type") ) {
			case "head":type="cap";break;
			case "hands":type="glove";break;
			case "boots":type="boot";break;
			case "waist":type="belt";break;
			case "surcoat":type="mantle";break;
			case "coat":type="coat";break;
			case "armor":type="armor";break;
			default:return notify_fail("对于这种防具，您了解不多，无法修补！\n");               
		}

		myskill = me->query("forging");//当它Quest了

		if( !mapp(myskill) || member_array(type,keys(myskill)) == -1 )
			return notify_fail("对于这种防具，您了解不多，无法修补！\n");

		cost_money = obj->query("value")/2;
	
		if( !flag ) cost_money += cost_money;
		cost_jing = obj->query("armor_mp/max_dur") - obj->query("armor_mp/dur");
		if( !flag ) cost_jing +=  cost_jing;
		cost_jing /= 4;
		cost_jing = cost_jing*3/4;
		if( me->query("jingli")<cost_jing || me->query("jing")<cost_jing )
			return notify_fail("你现在精神状态不佳，还是等会再修补吧。\n");
	
		switch(MONEY_D->player_pay(me,cost_money))
		{
			case 1:break;
			default:return notify_fail("你带的零钱不够了！你需要"+MONEY_D->money_str( cost_money )+"修补费用。\n");
		}
	
		write("你本次修补花费"+MONEY_D->money_str(cost_money)+"。\n");
	     
		me->set_temp("last_damage_from", "修补防具熬尽心机累");
		me->receive_damage("jing", cost_jing );
		me->receive_damage("jingli", cost_jing );

		message_vision(HIY"$N开始仔细的修补"+obj->query("name")+HIY"，不时用剪刀来回裁剪缝纫着......\n\n"NOR, me);
		delay = 15 + random(30);
		if( wizardp(me) && (int)me->query("env/test") ) delay = 3;
		me->start_busy(delay);
		weapon->use();
		call_out("xiu_armor_finish",delay*2-1,me,obj);
		return 1;

	} else return notify_fail("你只能维修由玩家制造的装备。\n");

        return 1;
}

int decvar(object ob,string type,int times,int decmin,int min)
{
        int i,j;
        ///5 10 10
        if( times <= 0 ) return 0;
        i = (int)ob->query(type) * 100;//1000
        if( i == 0 ) return 0;
        decmin *= 100;//1000
        min *= 100;//1000

        j = i / times;
        //200
        if( j < decmin ) j = decmin;
        i = i - j;
        if( i < min ) i = min;

        ob->set(type,i/100);
        return i/100;
}

void xiu_weapon_finish(object me,object obj)
{
        int i;

        if(!objectp(me) || !userp(me) || !objectp(obj) ) return;

        message_vision(HIY"$N仔细的维修"+obj->query("name")+HIY+"，总算大致恢复了它的原貌。\n"NOR, me);
        WORKER_D->check_impove(me,"duanzao",me->query_int()/2,2);

        //i = decvar(obj,"weapon_mp/max_dur",9,10,100);
        i = obj->query("weapon_mp/max_dur");
        obj->set("dur",i);
        obj->set("weapon_mp/dur",i);
                
        obj->save_weapon(me);
}

void xiu_armor_finish(object me,object obj)
{
        int i;

        if(!objectp(me) || !userp(me) || !objectp(obj) ) return;

        message_vision(HIY"$N仔细的修补"+obj->query("name")+HIY+"，总算大致恢复了它的原貌。\n"NOR, me);
        WORKER_D->check_impove(me,"zhizao",me->query_int()/2,2);

        //i = decvar(obj,"weapon_mp/max_dur",9,10,100);
        i = obj->query("armor_mp/max_dur");
        obj->set("dur",i);
        obj->set("armor_mp/dur",i);
                
        obj->save_weapon(me);
}

int help(object me)
{
	write(@HELP
	格式：repair <武器 or 防具>
	
	用于玩家自己修复打造的装备（未彻底损害的装备），
	修理兵器需要装备铁锤，修理防具要装备剪刀。
	需要一定的技能等级支持，
	消耗当前精，精力和金钱。
HELP
	);
	return 1;
}

/*
        decvar(obj,"weapon_prop/damage",(5+random(7))*2,2,10);
        decvar(obj,"weapon_prop/qi",(5+random(7))*2,10,10);
        decvar(obj,"weapon_prop/jing",(5+random(7))*2,10,10);
        decvar(obj,"weapon_prop/jingli",(5+random(7))*2,10,10);
        decvar(obj,"weapon_prop/neili",(5+random(7))*2,10,10);
        decvar(obj,"weapon_prop/attack",(5+random(7))*2,2,1);
        decvar(obj,"weapon_prop/dodge",(5+random(7))*2,3,1);
        decvar(obj,"weapon_prop/parry",(5+random(7))*2,3,1);

        i = decvar(obj,"weapon_mp/rigidity",5,1,1);
        obj->set("rigidity",i);
        i = decvar(obj,"weapon_mp/sharpness",5,1,1);
        obj->set("sharpness",i);
        i = decvar(obj,"weapon_mp/value",6,10,1);
        obj->set("value",i);
*/
