// look.c 更新版本
// Modified by snowman@SJ 11/12/2000
// Inv 显示优化，减少消耗。
// modified by olives@SJ 4/21/2000
// 修正一个look描述跟年龄无关的bug
// query错了
// modified by augx@sj 9/11/2001
// 添加物品损耗显示

#include <room.h>
#include <ansi.h>
#include <combat.h>
#include <level.h>
#include <get_place.h>

inherit F_CLEAN_UP;

int look_room(object me, object env, int night);
int look_item(object me, object obj);
int look_living(object me, object obj);
int look_room_item(object me, string arg);
string getper(object obj);
string tough_level(int power, int hide_level, int self);
string gettof(object obj, int self);
string getdam(object obj);
string inventory_look(object obj, int corpse, int equip);

int r_strlen(string str)
{
   int is_ansi,ansi_len,i;
   is_ansi=0;
   ansi_len=0;
   for (i=0;i<strlen(str);i++){     
     if (str[i]==27) 
     	is_ansi=1;
     if (is_ansi==1)
     	ansi_len++;
     if (str[i]==109 && is_ansi==1) 
     	is_ansi=0;     
   }
	 return strlen(str)-ansi_len;	
}

string delansi(string str)
{
   int is_ansi,ansi_len,i;
   //string ruslt;
   is_ansi=0;
   ansi_len=0;
   
   for (i=0;i<strlen(str);i++){     
     if (str[i]==27)
    	 is_ansi=1;
     if (str[i]==109 && is_ansi==1){ 
     	 str[i]=37;
     	 is_ansi=0;
     }
     if (is_ansi==1){
     	 ansi_len++;
       str[i]=37;
     }     
   }
	 return replace_string(str,"%","");	
}

string  room_relative(object env)
{
	object room_obj;
	mapping exits;
	int i;
	string map_room, map_room2,jiantou;
	string *dirs;
	string info;
	mapping alldirs=([
		"northwest" : "          ",
		"north"     : "          ",
		"northup"   : "          ",
		"northeast" : "          ",
		"west"      : "          ",
		"east"      : "          ",
		"southeast" : "          ",
		"south"     : "          ",
		"southwest" : "          ",
		"southdown" : "          ",
		"eastup" : "          ",
		"southup" : "          ",
		"northdown" : "          ",
		"eastdown" : "          ",
		"westup" : "          ",
		"westdown" : "          ",
		"enter" : "          ",
		"out" : "          ",
		"up" : "          ",
		"down" : "          ",
		"left" : "          ",
		"right" : "          ",
	]);
	if( mapp(exits = env->query("exits")) )
	{
		dirs=keys(exits);
		for(i=0;i< sizeof(dirs);i++)
		{
			if(!room_obj=find_object(exits[dirs[i]]))
			room_obj=load_object(exits[dirs[i]]);
			
			if(room_obj){
				if (room_obj->query("short"))
				alldirs[dirs[i]]=room_obj->query("short");                              ;
			}

		}

		jiantou="  ";
		map_room2=alldirs["enter"];
		if(alldirs["enter"]!="          "){
			map_room2=alldirs["enter"];
			jiantou=HIR+"∧"+NOR;
		}
		if(alldirs["up"]!="          "){
			map_room2=alldirs["up"];
			jiantou=HIC+"〓"+NOR;
		}
		if(alldirs["northdown"]!="          "){
			map_room2=alldirs["northdown"];
			jiantou="↓";
		}
		if(alldirs["northup"]!="          "){
			map_room2=alldirs["northup"];
			jiantou="↑";
		}
		if(alldirs["north"]!="          "){
			map_room2=alldirs["north"];
			jiantou="｜";
		}
		map_room=map_room2;
		for(i=0;i<(10-r_strlen(map_room2))/2;i++)
		{
			map_room =map_room+" ";
			map_room =" "+map_room;
		}
		info=sprintf("             %10s  %7s  %6s\n",
		alldirs["northwest"],map_room,alldirs["northeast"]);

		info+=sprintf("                        %s   %s   %s\n",
		alldirs["northwest"]=="          " ? "  ":"↖",
		jiantou,
		alldirs["northeast"]=="          " ? "  ":"↗");

		jiantou="  ";
		map_room2=alldirs["westdown"];
		if(map_room2!="          ")
		jiantou="→";
		if(alldirs["left"]!="          "){
			map_room2=alldirs["left"];
			jiantou="〈";
		}
		if(alldirs["westup"]!="          "){
			map_room2=alldirs["westup"];
			jiantou="←";
		}
		if(alldirs["west"]!="          "){
			map_room2=alldirs["west"];
			jiantou="--";
		}
		info+=sprintf("             %10s%s",map_room2,jiantou);
		map_room2=env->query("short") ? env->query("short"): "----------";
		map_room=HIG+map_room2+NOR;
		for(i=0;i<(10-r_strlen(map_room2))/2;i++)
		{
			if(alldirs["east"]=="          ")
			map_room =map_room + " ";
			else map_room=map_room + "-";
			if(alldirs["west"]=="          ")
			map_room =" "+ map_room;
			else map_room ="-"+map_room;
		}
		info+=sprintf("%8s",map_room);

		jiantou="  ";
		map_room2=alldirs["eastup"];
		if(map_room2!="          ")
		jiantou="→";
		if(alldirs["right"]!="          "){
			map_room2=alldirs["right"];
			jiantou="〉";
		}
		if(alldirs["eastdown"]!="          "){
			map_room2=alldirs["eastdown"];
			jiantou="←";
		}
		if(alldirs["east"]!="          "){
			map_room2=alldirs["east"];
			jiantou="--";
		}

		info+=sprintf("%s%-10s\n",jiantou,map_room2);

		jiantou="  ";
		map_room2=alldirs["out"];
		if(alldirs["out"]!="          "){
			map_room2=alldirs["out"];
			jiantou=HIR+"∨"+NOR;
		}
		if(alldirs["down"]!="          "){
			map_room2=alldirs["down"];
			jiantou=HIC+"〓"+NOR;
		}

		if(alldirs["southdown"]!="          "){
			map_room2=alldirs["southdown"];
			jiantou="↑";
		}
		if(alldirs["southup"]!="          "){
			map_room2=alldirs["southup"];
			jiantou="↓";
		}
		if(alldirs["south"]!="          "){
			map_room2=alldirs["south"];
			jiantou="｜";
		}
		map_room=map_room2;
		for(i=0;i<(10-r_strlen(map_room2))/2;i++)
		{
			map_room =map_room + " ";
			map_room =" "+map_room;
		}
		info+=sprintf("                        %s   %s   %s\n",
		alldirs["southwest"]=="          " ? "  ":"↙",
		jiantou,
		alldirs["southeast"]=="          " ? "  ":"↘");
		info+=sprintf("             %10s  %7s  %6s\n",
		alldirs["southwest"],map_room,alldirs["southeast"]);


	}
	else
	{
		map_room2=env->query("short") ? env->query("short"): "----------";
		map_room=HIG+map_room2+NOR;
		for(i=0;i<(10-r_strlen(map_room2))/2;i++)
		{
			if(alldirs["east"]=="          ")
			map_room =map_room + " ";
			else map_room=map_room + "-";
			if(alldirs["west"]=="          ")
			map_room =" "+ map_room;
			else map_room ="-"+map_room;
		}
		info+=sprintf("\n                            %s\n",map_room);

	}
	
	info = replace_string(info,"\n","");
	info = replace_string(info," ","");
	return delansi(info);
}



int ob_sk_class(object victim) {
	mapping prepare;
	string vskill;
	int y;
	
	if(!objectp(victim)) return 0;
	
	prepare = victim->query_skill_prepare();
	if (!prepare) prepare = ([]);
	  
	if ( victim->query_temp("weapon") ) { 
		vskill = (victim->query_temp("weapon"))->query("skill_type");
		if (vskill == "xiao") vskill = "sword";	
	 }
	else if (sizeof(prepare)) vskill = (keys(prepare))[0];
	//else if ( sizeof(prepare) == 2) vskill = (keys(prepare))[victim->query_temp("action_flag")];
  else vskill = 0;


 	if(victim->query_temp("apply/skill_class")){
		//write("class_test1\n");
		y=victim->query_temp("apply/skill_class");
	}
	else if(victim->query_temp("apply/action/class")){
	//	write("class_test2\n");
		y=victim->query_temp("apply/action/class");		
	}
	else {
    if(vskill) {
   //		write("class_test3\n");
   		prepare = victim->query_skill_map();   		
   		if(mapp(prepare) && objectp(find_object(SKILL_D(prepare [vskill])))
   		&& objectp(find_object(SKILL_D(victim->query_skill_map()[vskill]))))
	  	y = call_other(SKILL_D(victim->query_skill_map()[vskill]), "skill_class",victim);
	  }
	  else y = 0;			
	}
	return y;
}

void create() { seteuid(getuid()); }

static mapping to_chinses = ([
	"damage":"伤害力+$x",
	"armor":"防御力+$x",
	"str":"力量+$x",
	"con":"根骨+$x",
	"dex":"身法+$x",
	"int":"悟性+$x",
	"pfm_attack":"命中+$x",
	"defense":"闪避+$x",
	"neili":"内力+$x",
	"jingli":"精力+$x",
	"jing":"精血+$x",
	"qi":"气血+$x",
	"re_qi":"气血恢复速度+$x%",
	"re_neili":"内力恢复速度+$x%",
	"re_jing":"精血恢复速度+$x%",
	"re_jingli":"精力恢复速度+$x%",
	"pfm_parry":"招架+$x",
	"pfm_dodge":"闪避+$x",
	"force":"内功+$x",
	"armor_vs_force":"内功抗性+$x",
]);

string class_list = "无正奇刚柔快慢妙险空";
int main(object me, string arg)
{
	object obj;
	int result;

	if( !arg ) result = look_room(me, environment(me), NATURE_D->is_night());
	else if ((obj = present(arg, environment(me))) || (obj = present(arg, me))) {
		if(obj->query_temp("IN_CP")) return notify_fail("你要看什么？\n");
		if( obj->is_character() ) result = look_living(me, obj);
		else result = look_item(me, obj);
	} else result = look_room_item(me, arg);

	return result;
}

void realtime_map(object me,object env)
{
	object room_obj;
	mapping exits;
	int i;
	string map_room, map_room2,jiantou;
	string *dirs;
	mapping alldirs=([
		"northwest" : "          ",
		"north"     : "          ",
		"northup"   : "          ",
		"northeast" : "          ",
		"west"      : "          ",
		"east"      : "          ",
		"southeast" : "          ",
		"south"     : "          ",
		"southwest" : "          ",
		"southdown" : "          ",
		"eastup" : "          ",
		"southup" : "          ",
		"northdown" : "          ",
		"eastdown" : "          ",
		"westup" : "          ",
		"westdown" : "          ",
		"enter" : "          ",
		"out" : "          ",
		"up" : "          ",
		"down" : "          ",
		"left" : "          ",
		"right" : "          ",
	]);
	if( mapp(exits = env->query("exits")) )
	{
		dirs=keys(exits);
		for(i=0;i< sizeof(dirs);i++)
		{
			if(!room_obj=find_object(exits[dirs[i]]))
			room_obj=load_object(exits[dirs[i]]);			
			if(room_obj){
				if(virtualp(room_obj) && !me->query("env/new_map")) continue;
				if (room_obj->query("short"))
				alldirs[dirs[i]]=room_obj->query("short");                              ;
			}

		}

		jiantou="  ";
		map_room2=alldirs["enter"];
		if(alldirs["enter"]!="          "){
			map_room2=alldirs["enter"];
			jiantou=HIR+"∧"+NOR;
		}
		if(alldirs["up"]!="          "){
			map_room2=alldirs["up"];
			jiantou=HIC+"〓"+NOR;
		}
		if(alldirs["northdown"]!="          "){
			map_room2=alldirs["northdown"];
			jiantou="↓";
		}
		if(alldirs["northup"]!="          "){
			map_room2=alldirs["northup"];
			jiantou="↑";
		}
		if(alldirs["north"]!="          "){
			map_room2=alldirs["north"];
			jiantou="｜";
		}
		map_room=map_room2;
		for(i=0;i<(10-r_strlen(map_room2))/2;i++)
		{
			map_room =map_room+" ";
			map_room =" "+map_room;
		}
		printf("             %10s  %7s  %6s\n",
		alldirs["northwest"],map_room,alldirs["northeast"]);

		printf("                        %s   %s   %s\n",
		alldirs["northwest"]=="          " ? "  ":"↖",
		jiantou,
		alldirs["northeast"]=="          " ? "  ":"↗");

		jiantou="  ";
		map_room2=alldirs["westdown"];
		if(map_room2!="          ")
		jiantou="→";
		if(alldirs["left"]!="          "){
			map_room2=alldirs["left"];
			jiantou="〈";
		}
		if(alldirs["westup"]!="          "){
			map_room2=alldirs["westup"];
			jiantou="←";
		}
		if(alldirs["west"]!="          "){
			map_room2=alldirs["west"];
			jiantou="--";
		}
		printf("             %10s%s",map_room2,jiantou);
		map_room2=env->query("short") ? env->query("short"): "----------";
		map_room=HIG+map_room2+NOR;
		for(i=0;i<(10-r_strlen(map_room2))/2;i++)
		{
			if(alldirs["east"]=="          ")
			map_room =map_room + " ";
			else map_room=map_room + "-";
			if(alldirs["west"]=="          ")
			map_room =" "+ map_room;
			else map_room ="-"+map_room;
		}
		printf("%8s",map_room);

		jiantou="  ";
		map_room2=alldirs["eastup"];
		if(map_room2!="          ")
		jiantou="→";
		if(alldirs["right"]!="          "){
			map_room2=alldirs["right"];
			jiantou="〉";
		}
		if(alldirs["eastdown"]!="          "){
			map_room2=alldirs["eastdown"];
			jiantou="←";
		}
		if(alldirs["east"]!="          "){
			map_room2=alldirs["east"];
			jiantou="--";
		}

		printf("%s%-10s\n",jiantou,map_room2);

		jiantou="  ";
		map_room2=alldirs["out"];
		if(alldirs["out"]!="          "){
			map_room2=alldirs["out"];
			jiantou=HIR+"∨"+NOR;
		}
		if(alldirs["down"]!="          "){
			map_room2=alldirs["down"];
			jiantou=HIC+"〓"+NOR;
		}

		if(alldirs["southdown"]!="          "){
			map_room2=alldirs["southdown"];
			jiantou="↑";
		}
		if(alldirs["southup"]!="          "){
			map_room2=alldirs["southup"];
			jiantou="↓";
		}
		if(alldirs["south"]!="          "){
			map_room2=alldirs["south"];
			jiantou="｜";
		}
		map_room=map_room2;
		for(i=0;i<(10-r_strlen(map_room2))/2;i++)
		{
			map_room =map_room + " ";
			map_room =" "+map_room;
		}
		printf("                        %s   %s   %s\n",
		alldirs["southwest"]=="          " ? "  ":"↙",
		jiantou,
		alldirs["southeast"]=="          " ? "  ":"↘");
		printf("             %10s  %7s  %6s\n",
		alldirs["southwest"],map_room,alldirs["southeast"]);


	}
	else
	{
		map_room2=env->query("short") ? env->query("short"): "----------";
		map_room=HIG+map_room2+NOR;
		for(i=0;i<(10-r_strlen(map_room2))/2;i++)
		{
			if(alldirs["east"]=="          ")
			map_room =map_room + " ";
			else map_room=map_room + "-";
			if(alldirs["west"]=="          ")
			map_room =" "+ map_room;
			else map_room ="-"+map_room;
		}
		printf("\n                            %s\n",map_room);

	}
	return;
}
int look_room(object me, object env, int night)
{
	int i;
	int * dirs2d = ({ 0,0,0,0,0,0,0,0,0,0,0 });
	object *inv;
	mapping exits;
	string str, *dirs, arg,str1;

	if( !env ) {
		write("你的四周灰蒙蒙的一片，什么也没有。\n");
		return 1;
	}
	// night ^_^ , added by snowman@SJ
	if (env->query("no_light")
	&& !env->query_night_light()
	&& random(me->query_con()) < 35) {
		if(env->query("night_long")) str = env->query("night_long");
		else str = "这里一片漆黑，你虽然睁大了眼睛仔细观看，可还是看不清楚周围的\n"+
			"事物。一切都被笼罩在黑暗之中，伸手不见五指... \n";
	}
	else if (env->query("outdoors") && night
	&& environment(me) != env
	&& !env->query_night_light()
	&& random(me->query_con()) < 35) {
		if(env->query("night_long")) str = env->query("night_long");
		else str = "夜晚了，你虽然睁大了眼睛仔细观看，可还是看不清楚周围的事物。\n"+
			"一切都被夜色所笼罩，连脚下的道路都模糊不清了... \n";
	}

	str = sprintf( "%s - %s\n    %s%s",
		IN_MAIN?env->query("short"):HIG+env->query("short")+NOR,
		wizardp(me)? file_name(env): "",
		(str?str:replace_string(env->query("long")?env->query("long"):"","\r\n","\n")),
		env->query("outdoors")? NATURE_D->outdoor_room_description() : "" );

	// Client Send
	if( me && userp(me) && CLIENT_D->IsClient(me) ) {
		CLIENT_D->SendRoomLong(me,str);
		str = "";
	}
	
	if (!mapp(exits = env->query("exits")))
		exits = ([ ]);

	if (me->query("env/newlook")) {
		dirs = keys(exits);
		for (i=0; i<sizeof(dirs); i++)
			if (env->query_door(dirs[i], "status") & DOOR_CLOSED)
				dirs[i] = 0;
		dirs -= ({ 0 });
		if (sizeof(dirs) < 1)
			str += "    这里没有任何明显的出路。\n";
		else if ((env->query("no_light")
		|| (night && environment(me) != env && env->query("outdoors")))
		&& !env->query_night_light() && random(me->query_con()) < 15) {
			i = random(sizeof(dirs));
			if (random(me->query_con()) < 10) str += "    这里看不见任何明显的出路。\n";
			else str += "    这里看得见的唯一出口是 " + BOLD + dirs[i] + NOR + "。\n";
		} else {
			for(i=0; i<sizeof(dirs); i++) {
				switch (dirs[i]) {
					case "east"      : dirs2d[0]=1; break;
					case "southeast" : dirs2d[1]=1; break;
					case "south"     : dirs2d[2]=1; break;
					case "southwest" : dirs2d[3]=1; break;
					case "west"      : dirs2d[4]=1; break;
					case "northwest" : dirs2d[5]=1; break;
					case "north"     : dirs2d[6]=1; break;
					case "northeast" : dirs2d[7]=1; break;
					case "up"        : dirs2d[8]=1; break;
					case "down"      : dirs2d[9]=1; break;
					case "enter" 	 : dirs2d[10]=1; break;
					default: continue;
				}
				dirs -= ({ dirs[i] });
				i--;
			}

			if(dirs2d[8])  str+=HIR+"   up  \n"NOR;
			if(dirs2d[5])  str+=HIR; str += "nw "NOR;
			if(dirs2d[6])  str+=HIR; str += "n "NOR;
			if(dirs2d[7])  str+=HIR; str += "ne\n"NOR;
			if(dirs2d[4])  str+=HIR; str += "w  "NOR;
			if(dirs2d[10]) str+=HIR; str += "r "NOR;
			if(dirs2d[0])  str+=HIR; str += " e"NOR;

			if(sizeof(dirs)==1)
				str += "    其他出口有 " + BOLD + dirs[0] + NOR + "。\n";
			else if(sizeof(dirs)> 1)
				str += sprintf("    其他出口有 "+BOLD+"%s"+NOR+" 和 "+BOLD+"%s"+NOR+ "。\n", implode(dirs[0..<2], "、"), dirs[<1]);
			else str += "\n";

			if(dirs2d[3])  str+=HIR; str += "sw "NOR;
			if(dirs2d[2])  str+=HIR; str += "s "NOR;
			if(dirs2d[1])  str+=HIR; str += "se\n"NOR;
			if(dirs2d[9])  str+=HIR"  down  \n"NOR;
		}
	} else {
		dirs = sort_array(keys(exits), 1);
		for (i=0; i<sizeof(dirs); i++)
			if (env->query_door(dirs[i], "status") & DOOR_CLOSED)
				dirs[i] = 0;
		dirs -= ({ 0 });
		if (sizeof(dirs) < 1)
			str += "    这里没有任何明显的出路。\n";
		// night ^_^ , added by snowman@SJ
		else if ((env->query("no_light")
		|| (night && environment(me) != env && env->query("outdoors")))
		&& !env->query_night_light() && random(me->query_con()) < 35) {
			i = random(sizeof(dirs));
			if (random(me->query_con()) < 10) str += "    这里看不见任何明显的出路。\n";
			else if (random(me->query_con()) < 20)
				str += "    这里看得见的唯一出口是 " + BOLD + dirs[i] + NOR + "。\n";
			else str += sprintf("    这里看得清的出口是 " + BOLD + "%s" + NOR + "。\n", implode(dirs[0..i], "、"));
		}
		// hehehe.....
		else if( sizeof(dirs)==1 ) str += "    这里唯一的出口是 " + BOLD + dirs[0] + NOR + "。\n";
		else str += sprintf("    这里明显的出口是 " + BOLD + "%s" + NOR + " 和 " + BOLD + "%s" + NOR + "。\n",
			implode(dirs[0..<2], "、"), dirs[<1]);
	}

	env->delete("move_desc");

	inv = all_inventory(env);
	exits = ([]);
	str1 = str;
	for (i=0; i<sizeof(inv); i++) {
		if( inv[i]->query_temp("IN_CP")) continue;
		if( inv[i]==me ) continue;
		if( !me->visible(inv[i]) ) continue;

		// night.
		if ((env->query("no_light") || (night && env->query("outdoors") && environment(me) != env))
		&& !env->query_night_light()
		&& random(me->query_con()) < 35
		&& !inv[i]->is_character()
		&& !inv[i]->query("location")) continue;
		// hehehehehehe.....
		if (userp(inv[i])) {
			str += "  " + inv[i]->short() + "\n";
			continue;
		}
		arg = base_name(inv[i]) + inv[i]->short();
		if (!mapp(exits[arg])) {
			exits[arg] = ([
				"total" : 1,
				"unit" : (inv[i]->query("unit")?inv[i]->query("unit"):"个"),
				"short" : TASK_D->npc_event_hint(me,inv[i])+inv[i]->short(),
			]);
		}
		else {
		  exits[arg]["short"] = inv[i]->short();
		  exits[arg]["total"]++;
		}
	}
	if (me->query("env/look")){
		string place_name = get_place(file_name(env));
		if(place_name && place_name!="" && place_name!="中原神州" ) write("【你现在正处于" HIG+ place_name + NOR+"】\n");
		realtime_map(me,env);
	}
	
	dirs = keys(exits);
	
	if (strlen(str)>5000) {
		write(str1);
		write("这里挤得前胸贴后背，你什么东西都看不见。\n");
		return 1;
	}
	
	if (!sizeof(dirs)) {
		write(str);	
		return 1;
	}
	for (i = 0; i < sizeof(dirs); i++) {
		if (exits[dirs[i]]["total"] > 1)
			arg = "  "+chinese_number(exits[dirs[i]]["total"]) + exits[dirs[i]]["unit"] + exits[dirs[i]]["short"];
		else arg =  exits[dirs[i]]["short"];
		str += arg + "\n";
	}

	write(str);
	return 1;
}

int look_item(object me, object obj)
{
	mixed *inv;
	string prep;
	mapping skill;
	int i;
	string look_msg = "";
	string sn,num;
	float numa;

	write(obj->long());

	if(mapp(skill = obj->query("skill")))
	{
		sn = to_chinese(skill["name"]);
		if(!undefinedp(skill["exp_required"])){
			if (skill["exp_required"] == 0)
			look_msg += "无经验值要求\n";
			else
			look_msg += sprintf("至少要有 %d 点经验值\n", skill["exp_required"]);

		}
		if(!undefinedp(skill["min_skill"]))
		look_msg += sprintf("「%s」至少要有 %d 级\n", sn,skill["min_skill"]);

		if(!undefinedp(skill["difficulty"]))
		look_msg += sprintf("至少要有 %d 点悟性\n", skill["difficulty"]);

		if(!undefinedp(skill["max_skill"]))
		look_msg += sprintf("可以学习「%s」到 %d 级\n", sn, skill["max_skill"]);
		if(look_msg != "")
		write(HIG"一排古篆字写着「"+obj->query("name")+"」学习要求：\n"NOR
		+
		"┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉\n"
		+look_msg+
		"┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉\n");
	}
	if(mapp(obj->query("material_prop")) || mapp(obj->query("material_props")))
	{
		look_msg ="";
		if(mapp(skill = obj->query("material_prop")))
		{
			inv = keys(skill);
			for(i=0;i<sizeof(inv);i++){
				sn = to_chinses[inv[i]];
				sn = replace_string(sn,"$x",sprintf("%d",skill[inv[i]]));
				look_msg += HIG"\t"+sn+"\n"NOR;
			}
		}
		if(mapp(skill = obj->query("material_props")))
		{
			inv = keys(skill);
			for(i=0;i<sizeof(inv);i++){
				sn = skill[inv[i]];
				sscanf(sn,"status_type%*sjade_show%sitem_show%*sadd_num%scan_wield%*s",sn,num);
				if(sn==" ") continue;
				if(stringp(num)) sscanf(num,"%f",numa);
				else continue;
				sn = replace_string(sn,"$x",sprintf("%d",to_int(numa)));
				look_msg += HIG"\t"+sn+"\n"NOR;
			}
		}
		if(look_msg != "")
		write(HIG"一排古篆字写着「"+obj->query("name")+HIG"」具体功能：\n"NOR
		+
		"┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉\n"
		+look_msg+
		"┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉\n");
	}
	if( !(int)obj->query("broken") && (i = (int)obj->query("dur")) ) {
		if(i>1500) write("看起来没有什么损坏。\n");
		else if(i>750) write("看起来已经使用过一段时间了。\n");
		else if(i>200) write("看起来需要修理了。\n");
		else write("看起来马上就要坏了。\n");
	}
	inv = all_inventory(obj);
	if( !obj->is_closed() && sizeof(inv)) {
		switch( (string)obj->query("prep") ) {
			case "on":prep = "上";break;
			case "under":prep = "下"; break;
			case "behind":prep = "后";break;
			case "inside":prep = "里";break;
			default:prep = "里";
		}
		inv = map_array(inv, "inventory_look", this_object(), 1);
		write(sprintf("%s面有：\n  %s\n", prep, implode(inv, "\n  ")));
	}
	return 1;
}

string getper(object obj)
{
	int per, age;

	string str;

	per = obj->query_per();
   //   age = obj->query("str"); 根据str?不对头吧？
	age = obj->query("age")-obj->query("age_reyoung");

	if (obj->query("gender") == "女性") {
		if (per >= 40) str = "超凡脱俗，娇艳绝伦，貌如西子胜三分！";
		else if (per >= 37) {
			if (age > 22) str = "朱唇不涂一点红，回眸一笑百媚生！";
			else str = "清丽绝俗，冰清玉洁，有如画中天仙！";
		} else if (per >= 33) {
			if (age > 22) str = "容貌丰美，气质高雅，堪称人间仙子！";
			else str = "容貌娇美，姣花照水，堪称人间仙子！";
		} else if (per >= 30) {
			if (age > 22) str = "脸若银盆，眼如水杏，相貌绝美！";
			else str = "美奂绝伦，一笑倾城，再笑倾国！";
		} else if (per >= 27) str = "羞花闭月，宛若天仙！";
		else if (per >= 24) str = "花颜月貌，柔媚娇俏！";
		else if (per >= 20) str = "楚楚动人，有几分姿色！";
		else if (per >= 17) str = "相貌平庸，很是一般。";
		else if (per >= 14) str = "五官挪位，貌似无盐。";
		else str = "一塌糊涂，不是人样！";
	} else {
		if (per >= 40) str = "英姿勃发，一表人才，称为古往今来第一人！";
		else if (per >= 37) str = "玉树临风，风流倜傥，堪称绝世美男！";
		else if (per >= 33) str = "清秀俊雅，相貌非凡，真是人中龙凤！";
		else if (per >= 30) {
			if (age < 16) str = "貌似美女，脸若冠玉，弱不禁风！";
			else  str = "貌似潘安，容比宋玉，仪表堂堂！";
		}
		else if (per >= 27) str = "相貌出众，堪称美男！";
		else if (per >= 24) str = "英俊潇洒，气质非凡。";
		else if (per >= 20) str = "五官端正，相貌平平。";
		else if (per >= 17) str = "五官不正，满脸麻子。";
		else if (per >= 14) str = "牛眼驴唇，面目狰狞。";
		else str = "有如雷公下凡，八戒返魂！";
	}
	return "长得" + str + "\n";
}

string gettof(object ob, int self)
{
	object weapon;
	string skill_type;
	int attack_points, hide_level;
	mapping prepare;

	if (objectp(weapon = ob->query_temp("weapon"))) skill_type = weapon->query("skill_type");
	else {
		prepare = ob->query_skill_prepare();
		if (!sizeof(prepare)) skill_type = "unarmed";
		else skill_type = keys(prepare)[0];
	}

	attack_points = COMBAT_D->skill_power(ob, skill_type, SKILL_USAGE_ATTACK);

	if (intp(hide_level = ob->query_temp("hide_level")) && hide_level > 0)
		return tough_level(attack_points, hide_level, 0);
	return tough_level(attack_points, 0, self);
}

int look_living(object me, object obj)
{
	int y,age;
	string str, pro;
	mixed *inv;

	if( me != obj && obj->visible(me)) {
		if (wizardp(obj))
			tell_object(obj, "("+capitalize(me->query("id"))+")");
		tell_object(obj, me->name() + "正盯着你看，不知道打些什么主意。\n");
	}

	inv = explode(obj->long(), "\n");
	if (sizeof(inv) > 10) {
		str = implode(inv[0..8], "\n") + "\n";
		str += "此处删去"+chinese_number(sizeof(inv)-9)+"行。\n";
	} else str = implode(inv, "\n") + "\n";

	pro = (obj==me) ? gender_self(obj->query("gender")) : gender_pronoun(obj->query("gender"));

	if (!obj->query_temp("apply/long")) {
		if( (string)obj->query("race")=="人类"
		&& intp(age = obj->query("age")-obj->query("age_reyoung"))
		&& !obj->query("no_head")) {
			if (obj->query("gender") != "男性"
			&& obj->query_skill("beauty") > 34) {
				int temp_age = (obj->query_skill("beauty") - 30) / 5;

				if (temp_age > age / 2)
					temp_age = age / 2;
				age -= temp_age;
			}
			if (age < 10) str += sprintf("%s看起来不到十岁", pro);
			else str += sprintf("%s看起来约%s多岁", pro, chinese_number(age / 10 * 10));
			if (wizardp(me) || userp(obj) && obj->query("age") >= 16) str += "，" + getper(obj);
			else str += "。\n";
			if (obj==me || wizardp(me)) age = 1;
			else age = 0;
			// add by snowman@SJ 28/02/2000
			if(!obj->is_corpse())
				if (living(obj) && (me->query("combat_exp") > obj->query("combat_exp")/3*2 || me==obj))
				str +=sprintf("%s的武艺看上去", pro) + gettof(obj, age) +"，出手似乎" + getdam(obj) +"。\n";
				else str += pro+"的武功看不出深浅。\n";
				if (me->query_condition("true_view") && me->is_fighting(obj)) {
				  y=COMBAT_D->ob_sk_class(obj);
				  str += HIR + pro+"的武功属性好象是"+class_list[y*2..y*2+1]+"。\n" NOR;
				}
		}
		// Modified by snowman@SJ 04/04/2000. move family check to rankd.c
		if (obj != me && userp(obj))
			str += pro + "是你的"+RANK_D->query_family_close(me, obj)+"。\n";

		if (userp(obj) && obj->query("id") == me->query("marry/id"))
			str += pro + "是你的"
				+ (obj->query("gender") == "女性"?"娘子":"夫君")
				+ "。\n";
		else if (obj->query_temp("marry") == me)
			str += pro + "是你的未婚"
			+ (obj->query("gender") == "女性"?"妻":"夫") + "。\n";
	}

	if (obj->query("max_qi")) {
		int eratio = (obj->query("eff_qi")+obj->query_temp("apply/qi")) * 100
				/ (obj->query("max_qi")+obj->query_temp("apply/qi"));
		int sratio;
				
		if( obj->query("eff_qi")+obj->query_temp("apply/qi")>0 ) sratio = obj->query("qi") * 100 / (obj->query("eff_qi")+obj->query_temp("apply/qi"));
		
		else sratio=0;
		
		if (sratio < eratio)
			str += pro + COMBAT_D->status_msg(
				obj->query("qi") * 100 / (obj->query("max_qi")+obj->query_temp("apply/qi"))) + "\n";
		else
			str += pro + COMBAT_D->eff_status_msg(eratio) + "\n";
	}

	inv = all_inventory(obj);
	inv = map_array(inv, "inventory_look", this_object(), obj->is_corpse());
	inv -= ({ 0 });
	if( sizeof(inv) )
		str += sprintf( obj->is_corpse() ? "%s的遗物有：\n%s\n" : "%s穿戴着：\n%s\n",
			pro, implode(inv, "\n") );
//	else if (me != obj && obj->query("race")=="人类")
//		str += pro + "身上没有任何穿戴。\n";
	inv = ({ obj->query_temp("weapon") });
	inv -= ({ 0 });
	if( sizeof(inv) ) str += sprintf("%s装备着：\n%s\n",pro, inventory_look(inv[0], 0, 1));
//	else if (me != obj && obj->query("race")=="人类")
//		str += pro + "身上没有任何装备。\n";

	write(str);

//	if (obj != me && obj->visible(me) && userp(me) && living(obj)
//	&& !(userp(me) && userp(obj) && me->query("marry/id") == obj->query("id"))
//	&& obj->query("shen") < -me->query_int() * 100 ) {
//		if( (int)obj->query("combat_exp") > (int)me->query("combat_exp")){
//			write(obj->name() + "突然转过头来瞪你一眼。\n");
//			COMBAT_D->auto_fight(obj, me, "berserk");
//		}
//		else write(obj->name() + "突然转过头来看了你几眼。\n");
//	} 
	return 1;
}

string inventory_look(object obj, int corpse, int equip)
{
	string str;

	if( obj->query("equipped")) {
		if (corpse || (!equip && !obj->query("weapon_prop"))
		|| (equip && obj->query("weapon_prop")))
		    str = HIC "  □" NOR + obj->short();
		else return 0;
	}
	else if( obj->query("embedded") && !corpse && !equip)  str = HIR "  ☆" NOR + obj->short();
	else if( corpse )
		str = "    " + obj->short();
	else return 0;
	return str;
}


int look_room_item(object me, string arg)
{
	object env;
	mapping item, exits;

	if (!objectp(env = environment(me)))
		return notify_fail("这里除了灰蒙蒙的一片，什么也没有。\n");
	if (mapp(item = env->query("item_desc")) && !undefinedp(item[arg])) {
		if (stringp(item[arg]))
			write(item[arg]);
		else if (functionp(item[arg]))
			write((string)(*item[arg])(me));
		return 1;
	}
	if( mapp(exits = env->query("exits")) && !undefinedp(exits[arg]) ) {
		if( objectp(env = load_object(exits[arg])) )
			look_room(me, env, NATURE_D->is_night());
		else return notify_fail("这个方向无法看清，请报告巫师。\n");
		return 1;
	}
	return notify_fail("你要看什么？\n");
}

string *tough_level_desc = ({ LEVEL });

string tough_level(int power, int hide_level, int self)
{
	int lvl, rawlvl;

	if (power < 0) power = 0;
	//power *= 10;
	rawlvl = to_int(pow((float)1.0 * power, 0.333)/10*3.4);
	//lvl = rawlvl;
	lvl = to_int(rawlvl)/2;
	rawlvl = to_int(rawlvl)/2;
	if (hide_level) lvl = hide_level;
	if (rawlvl >= sizeof(tough_level_desc)) rawlvl = sizeof(tough_level_desc)-1;
	if (lvl >= sizeof(tough_level_desc)) lvl = sizeof(tough_level_desc)-1;

	return tough_level_desc[self?rawlvl:lvl];
}

string getdam(object obj)
{
	int level;
	string *heavy_level_desc= ({"极轻","很轻","不重","不轻","很重","极重",});

	level = obj->query("jiali");
	level += obj->query_temp("apply/damage")/3;  // weapon 加力效果等于 unarmed / 3

	level /= 30;
	if( level >= sizeof(heavy_level_desc) ) level = sizeof(heavy_level_desc)-1;
	if(level<0) level=0;
	return heavy_level_desc[level];
}

int help (object me)
{
	write(@HELP
指令格式: look [<物品>|<生物>|<方向>]

这个指令让你查看你所在的环境、某件物品、生物、或是方向。

影响此命令的环境变量：
set brief 1/0 : 简单/详细 显示房屋描述
set newlook 1/0 : 直观/经典 显示方向

HELP
);
	return 1;
}
