// guard.c
// by snowman@SJ 11/03/2000
// Add log By Looklove@SJ 2000,11,23
// Modify By Looklove 2001,1,11
// Modify By augx@sj 9/18/2001
// Modify by Ciwei@SJ add PK log 21/3/2004

#include <ansi.h>
#include <pktime.h>

inherit F_CLEAN_UP;
int help(object);

int main(object me, string arg)
{
	object obj, *enemy;
	string callname, msg;
	int i;
	
	if( !arg )
		return help(me);

	if(environment(me)->query("no_fight")) 
	       return notify_fail("这里不能战斗。\n");

	if(!objectp(obj = present(arg, environment(me))) 
	|| !obj->is_character() 
	|| obj->is_corpse() 
	|| obj->is_ghost() )
		return 0;
	
	if (me->is_busy() && !(int)me->query("i_am_guard")) return notify_fail("你正忙着呢。\n");

	if( obj == me ){ 
		message_vision("$N摆了个造型，一副绝世清高的姿态！\n", me);
		write("你觉得自己的造型帅呆了。\n");
		return 1;
	}

	if (!obj->is_fighting() ){
		write("这人没在战斗中呢。\n");
		return 1;
	}
	
	if (!userp(obj)){
		write("你好象很无聊呀。\n");
		return 1;
	}
	
	if( obj->is_killing(me->query("id")) || me->is_killing(obj->query("id"))){
		write("这人正在和你性命相博，你还有空保护他？\n");
		return 1;

	}
	if( me->is_fighting() ){
		write("你正打得欢呢，先忙完这边再说吧！\n");
		return 1;
	}
	
	enemy = obj->query_enemy();
	i = sizeof(enemy);
	while (i--) {
	  if ( userp(enemy[i]) ) {
	  	write(obj->name()+"和别人在生死相搏呢，你别碍事。\n");
		  return 1;
		}
		if (enemy[i]->is_killing(obj->query("id"))) break;
		if (obj->is_killing(enemy[i]->query("id"))) break;
	}
	if (i < 0) {
		write(obj->name()+"和别人在切磋呢，你别碍事。\n");
		return 1;
	}
	
	if(userp(obj) && obj->query_condition("killer") && !me->query_condition("killer")){
		write("帮助杀人犯？小心被通缉！\n");
		return 1;
	}
	
	if( !(int)me->query("i_am_guard") && me->query("combat_exp")<obj->query("combat_exp") / 3 ){
		write("哎哟哟！你这点本事，还是别去碍手碍脚的吧！\n");
		return 1;
	}

	if( obj->query("env/no_protect") || obj->query("env/no_guard") ){
		msg = CYN"$N想要冲上前去帮$n。\n";
		if( stringp(callname = obj->query("env/no_protect")) )
			msg += "不料$n对着$N一瞪："+callname+"！\n"NOR;
		else msg += "不过看来$n并不想得到$N的帮助！\n"NOR;
		message_vision(msg, me, obj);
		return 1;
	}
	
	callname = RANK_D->query_respect(obj);
	me->start_busy(1);

	if(me->query("race")=="人类"){
		if(me->query("mute") || obj->query("race")!="人类")
			msg = "\n$N突然冲前，横身挡在$n身前，看来是想要保护$p！\n\n";			
		else if(stringp(me->query("env/protect_msg")) && strlen( strip(me->query("env/protect_msg"))) < 50 )
			msg = "\n$N一下子横身挡在$n身前，吼道：「"+me->query("env/protect_msg")+"！」\n\n";			
		else if(me->query("combat_exp") > obj->query("combat_exp")*2){
			if(me->query("shen") < -10000)
				msg = "\n$N一把拉开$n：「"+RANK_D->query_rude(obj)+"你好好学着点，看看"+RANK_D->query_self_rude(me)+"是怎么修理人的！！」\n\n";
			else    msg = "\n$N一下穿插在$n身前：「"+callname+"你让开，让"+RANK_D->query_self(me)+"来过过手瘾，你在一边看着吧！！」\n\n";
		}		
		else if(obj->query("combat_exp") > me->query("combat_exp")*2){
			if(me->query("shen") < -10000)
				msg = "\n$N往$n身前一站：「"+callname+"，你在一旁看着，今天"+RANK_D->query_self_rude(me)+"就要来搅这趟浑水！！」\n\n";
			else    msg = "\n$N面色凝重，抢在$n身前：「"+callname+"请让开，我倒要看看是什么人敢打你的主意！！\n\n";
		}		
		else {
			if(me->query("shen") < -10000)
				msg = "\n$N一下跃到$n身前：「"+callname+"！来来来！让"+RANK_D->query_self_rude(me)+"我来耍耍，你歇歇！！」\n\n";
			else    msg = "\n$N对着$n把胸脯拍得震天响：「有"+RANK_D->query_self(me)+"在，什么人都不能从你那儿讨了好去！」\n\n";
		}		
	}	
	else msg = "\n$N大吼一声，猛然挡在$n面前，看来是要保护$p！！\n\n";
	
	message_vision(CYN+msg+NOR, me, obj);

	if(userp(obj))
		tell_object(obj, HIC "这" + me->name()+ "(" + me->parse_command_id_list()[0]+ ")一下子抢在你面前，拦住了你大部分的对手！\n" NOR);
	
	enemy = obj->query_enemy();

	for(i = 0; i < sizeof(enemy); i++){		
		if(!enemy[i]) continue;
		if(!living(enemy[i])) continue;		
		if(enemy[i]->query("no_ansuan")) continue;
		if(enemy[i]->query("job_npc")) continue;
		if(obj->is_killing(enemy[i]->query("id")) && !enemy[i]->is_killing(obj->query("id"))) continue;
		if((int)me->query("i_am_guard") && !userp(enemy[i])) continue;
		if(userp(me) && userp(enemy[i]))
		{
			if(pktime_limit(me, enemy[i])) continue;
			if(me->query("no_pk")){
				write("你已经金盆洗手，如何还能做此等勾当。\n");
				continue;
				}
                	if(enemy[i]->query("no_pk")){
                		write(enemy[i]->name()+"已经金盆洗手，你还是放过他吧。\n");
                		continue;  		                		
                		}
                	enemy[i]->set_temp("other_kill/"+me->query("id"), 1);
                	
                	log_file("ANSUAN", 
                        sprintf("%-18s guard  %-18s",
                                me->name(1)+"("+capitalize(getuid(me))+")",
                                enemy[i]->name(1)+"("+capitalize(getuid(enemy[i]))+")'s enemy"
                        	),
                        	this_object(),
                        );
		}
		
		if( (int)me->query("i_am_guard") )
			enemy[i]->remove_killer(obj);
		else
			enemy[i]->remove_enemy(obj);
		obj->remove_killer(enemy[i]);
		me->fight_ob(enemy[i]);
		enemy[i]->fight_ob(me);
		
		if (!enemy[i]->is_killing(obj->query("id")) 
		&& random(enemy[i]->query("combat_exp")) > random(me->query("combat_exp")/2))
			enemy[i]->kill_ob(me);
/*
		if (userp(obj))
		log_file("static/guard",sprintf("%s(%s) guard %s(%s) with %s(%s) at %s.\n",
		me->name(),me->query("id"), obj->name(), obj->query("id"),enemy[i]->name(),enemy[i]->query("id"),ctime(time()) ) );
*/
	}       
	return 1;
}

int help(object me)
{
  write(@HELP
指令格式 : guard <人物>
 
这个指令让你保护一个战斗中的人物。
你会接下和此人物战斗的其它生物，至于对方如何反应，就看当时的情况了。

其他相关指令: fight，kill，hit，ansuan

HELP);
    return 1;
}

