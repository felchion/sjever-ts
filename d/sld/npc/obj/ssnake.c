// ssnake

#include <ansi.h>
inherit NPC;

int snakehd();
int snake_attack_ob(object ob);

void create()
{
	set_name(HIY"金蛇"NOR, ({ "jin she","snake","she" }) );       
	set("race", "野兽");
	set("long", HIY"这是条金光闪闪的小蛇。\n"NOR);
	set("limbs", ({ "头部", "身体", "七寸", "尾巴" }) );
	set("verbs", ({ "bite" }) );
	set("combat_exp", 100000);  
	set("no_return",1);
	set("no_clear_up",1);
	set("attitude", "friendly");
	
	set("str",30);
	set("int",30);
	set("con",30);
	set("dex",30);
	set("kar",30);
	set_weight(1000);
	
	setup();
}

static string *banned_command = 
	({ "chat", "rumor", "sj", "irc", "party", "give", "get", "vote", "ansuan", "fight","sr" });

void do_command(string arg)     
{
	string dir,*cmds;
	object target,owner = query("owner");
    
	if(!arg || arg=="" || !owner || !userp(owner)) return;
	
	cmds = explode(arg," ");
        cmds[0] = replace_string(cmds[0],"*","");
        cmds[0] = replace_string(cmds[0]," ","");
        if ( member_array(cmds[0], banned_command)!=-1 ) {
        	tell_object(owner, "蛇不能执行此指令！\n");
		return;
	}
	
	switch(arg) {
		case "e":	dir="east"; break;
		case "w":	dir="west"; break;
		case "n":	dir="north"; break;
		case "s":	dir="south"; break;
		case "ne":	dir="northeast"; break;
		case "nw":	dir="northwest"; break;
		case "se":	dir="southeast"; break;
		case "sw":	dir="southwest"; break;
		case "u":	dir="up"; break;
		case "d":	dir="down"; break;
		case "nu":	dir="northup"; break;
		case "eu":	dir="eastup"; break;
		case "su":	dir="southup"; break;
		case "wu":	dir="westup"; break;
		case "nd":	dir="northdown"; break;
		case "ed":	dir="eastdown"; break;
		case "sd":	dir="southdown"; break;
		case "wd":	dir="westdown"; break;
		case "l":	dir="look"; break;
		default:	dir=arg; break;
	}
	
	if( cmds[0]=="kill" || cmds[0]=="hit" ) {
		if( sizeof(cmds) > 1 && strlen(cmds[1])>2 && target=present(cmds[1],environment(this_object())) ) {
			if( userp(target) && !target->is_fighting(this_object()) ) {
				tell_object(owner, "蛇不能伤害对自己没有威胁的人类！\n");
				return;
			}
		}
	}
	
	set_name(HIY+owner->query("name")+"的金蛇"NOR, ({ owner->query("id")+"'s jin she"}) );
	command(dir);
	
	switch(dir) {
		case "east":		
		case "west":		
		case "north":		
		case "south":		
		case "northeast":	
		case "northwest":	
		case "southeast":
		case "southwest":
		case "up":
		case "down":
		case "northup":
		case "eastup":
		case "southup":
		case "westup":
		case "northdown":
		case "eastdown":
		case "southdown":
		case "westdown":
		case "enter":
		case "out": command("look"); break;
		default: break;
	}
	set_name(HIY"金蛇"NOR, ({ "jin she","snake","she" }) );     
}

void relay_message(string msgclass, string msg)
{
	object owner;
	string *ary;
	int i;
	
	if( environment(this_object()) == owner ) return;
	msg = "1\n" + msg;
	ary = explode(msg, "\n");
	owner = query("owner");
	if( objectp(owner) && !owner->is_ghost() ) {
		for (i=1;i<sizeof(ary);i++)
			tell_object(owner,HIY"【蛇】"NOR+ary[i]+NOR+"\n");
	}
}

void die()
{
	object owner = query("owner");
	
	if(objectp(owner) && userp(owner) && owner->query_temp("sld/snake")==this_object()) {
		owner->delete_temp("sld/snake");
	}
	::die();
}

void unconcious()
{ 
	die();
}

int hit_ob(object me, object victim, int damage)
{
	victim->apply_condition("snake_poison", victim->query_condition("snake_poison")+5);
	return 0;
}

string gongji()
{
	object owner = query("owner");	
	if(!owner || !userp(owner)) return "";
	if((int)query("gongji")) return "金蛇已经在攻击敌人了！\n";
	set("gongji",1);
	remove_call_out("check_fight");
	call_out("check_fight",1,owner);
	return HIY"\n你拍拍金蛇的头，指指敌人...\n"NOR;
}

string huti()
{
	object owner = query("owner");
	if(!owner || !userp(owner)) return "";
	if((int)query("huti")) return "金蛇已经在护体之中了！\n";
	set("huti",1);
	owner->set_temp("sld/huti",1);
	remove_call_out("check_fight");
	call_out("check_fight",1,owner);
	return HIY"\n你拍拍金蛇的头，指指自己，金蛇开始在你周身游走...\n"NOR;
}

int snake_attack(object owner,object target)
{
	int lvl,damage;

	message_vision(HIY"\n$N的金蛇突然窜向$n，狠狠地咬了下去...  "NOR,owner,target);
	if( target->is_busy() || random(owner->query_dex()*6/5+target->query_dex())>target->query_dex() ) {
		message_vision(HIR"$N躲闪不及，被金蛇一口咬中！\n"NOR,target);
		
		if( !(int)target->query_temp("sld/snake_posion") ) {
			message_vision(HIB"$N感到一阵剧痛传来，赶忙运功抵御，身法不免慢了下来！\n"NOR,target);
			lvl = owner->query_skill("dulong-dafa",1)/4;
			target->add_temp("apply/dodge",-lvl);
			target->add_temp("apply/parry",-lvl);
			target->set_temp("sld/snake_posion",lvl);
			target->kill_ob(owner);
			call_out("remove_effect",15+random(30),target);
		}
		
		damage = query_temp("apply/attack");
		if(damage>200) damage = 200 + (damage-200)*2;
		target->receive_damage("qi", damage, owner);
		target->receive_wound("qi", random(damage), owner);
		if(wizardp(owner) && owner->query("env/test")) tell_object(owner,sprintf("damage: %d\n",damage));
	}
	else {
		message_vision(HIY"$N急忙往旁边一闪，躲了开去。\n"NOR,target);
	}
	
	return 1;
}                                            

void remove_effect(object target)
{
	if(target && (int)target->query_temp("sld/snake_posion")) {
		target->add_temp("apply/dodge",target->query_temp("sld/snake_posion"));
		target->add_temp("apply/parry",target->query_temp("sld/snake_posion"));
		target->delete_temp("sld/snake_posion");
	}
}

void check_fight(object owner)
{
	object *enemy,target;
	
	if( !owner || owner->query_temp("sld/snake")!=this_object() ) return;
	if( !owner->is_fighting() || environment(this_object())!=owner ) {
		set("gongji",0);
		set("huti",0);
		owner->delete_temp("sld/huti");
		return;
	}
	
	if( (int)query("gongji") && random(10)<2 ) {
		enemy = owner->query_enemy();
		target = enemy[(int)(random(sizeof(enemy)))];
		if(target) snake_attack(owner,target);
	}
	
	call_out("check_fight", 1, owner);
}
