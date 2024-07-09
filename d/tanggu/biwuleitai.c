
#include <ansi.h>
#define ZHAOQIN_DIR "/d/tanggu/"
//文件目录
#define BIWU_USER ZHAOQIN_DIR"npc/copy_user.c"
#define LEITAINPC ZHAOQIN_DIR"npc/npc_leitai.c"
inherit ROOM;
void mu_unset_player(int i);
void mu_set_player(object me,int i);
void mu_set_station(int i);
void create()
{
        set("short", HIG"招亲擂台"NOR);
        set("long", "这里是比武招亲的擂台。斗大的旗帜迎风飘扬。擂台四周挤满了围观的人群。有
吆喝助威的，有嬉笑起哄的，更多是无聊的闲人。擂台一角挂着一个牌子（paizi）。\n");
        set("no_death", 1);
        set("no_get_from", 1); 
        set("no_sleep_room", 1);
        set("dest_room", ZHAOQIN_DIR+"kongchangdi"); 
        set("objects", ([    
               LEITAINPC : 1,
        ]));

        set("item_desc", ([            
			"paizi" : "\n这里规则与擂台相似，在双方都准备好( ok )了以后开始比武。\n",      
        ]));
        setup(); 
}
void init()
{
	object me;

	me = this_player();
	if (!userp(me)) return;
	if (!wizardp(me) && !me->query_temp("quest/bwzh/leitai")){
		message_vision("突然飞出一双大脚，将$N从擂台给踢了出去！\n", me);
		me->move("/d/city/chmiao");
		message_vision("“啊。。。”的一声，$N被人从擂台给踢了出来。\n", me);
		return;
	}
	
	if (!this_object()->query_temp("quest/bwzh/leitai") 
	&& base_name(me) == BIWU_USER)
	{
		me->quit(me);
		return;
	}	
	if (userp(me)) message_vision("$N急急忙忙地走进了招亲擂台。\n", me);
	if(me->query_temp("quest/bwzh/leitai") && !wizardp(me) )
	{
		remove_call_out("leaving");
		call_out("leaving",150);
		remove_call_out("auto_kill");
		call_out("auto_kill",45);
	}	
	add_action("do_open", "xinxi");
	add_action("do_ok", "ok");
	add_action("do_action", ({"dazuo", "tuna", "lian", "learn", "du", "steal", "hit",
	                          "kill", "fight", "study", "practice", "exercise", "halt", 
	                          "quit", "save", "throw", "rumor", "tell", "mm", "reply",
	                          "chat", "rumor", "party", "shout", "suicide", "yield"}));
}

int do_action(string arg)
{
	object ob,me;
	me= this_player();
	if(me->query("link_ob")) 
		ob=me->query("link_ob");
	if(!wizardp(me)){
		if(ob && wizardp(ob))
			return 0;
		else 
		{
			write("擂台上只能比武，请不要做不相关的事！\n");
			return 1;
		}
	}
	else return 0;
}

int open = 1;

int do_open(string arg)
{
	object me = this_player();
	
	if( !wizardp(me))
		return notify_fail("只有巫师才能打开擂台信息。\n");   
	
	if( !arg || arg == "" || arg != "open") 
		return notify_fail("目前的擂台信息状况是："+(open == 0?"close":"open")+"\n");
	
	if( arg == "open" ){
		open = 1;
		message("leitai", HIY +"\n【"+environment(me)->query("short")+"擂台】：开放此擂台结果信息，不希望看见的玩家请 set block_leitai 。\n\n"NOR, users());
		return notify_fail("你打开了擂台的信息状况。\n");
	}
	
	if( arg == "close" ){
		open = 0;
		message("leitai", HIY +"\n【"+environment(me)->query("short")+HIY"】"NOR+"：关闭此擂台结果信息。\n\n"NOR, users());
		return notify_fail("你关闭了擂台的信息状况。\n");
	}
	
	else return notify_fail("你可以设定擂台信息状况是 open (所有人都能看见结果) 还是 close (只有看台的人才看见结果)\n");
}

void end_fight(object me, object ob, string fam)
{
	string fam2, color;
	object link_me,link_ob;
	link_ob = ob->query("link_ob");
	link_me = me->query("link_ob");

	remove_call_out("auto_kill");
	remove_call_out("leaving");

	fam2 = ob->query("family/family_name")?ob->query("family/family_name"):"平民百姓";
	color = ob->query("color")?ob->query("color")+" ":" ";
	fam2 = HIW+fam2+color+NOR+ ob->name(1) + "(" + capitalize(ob->query("id")) + ")";
	message_vision("\n"+fam+" 被 "+fam2+" 击败。\n", me, ob);
	if( open > 0 )
		message("leitai", HIY +"\n【"+environment(me)->query("short")+HIY"】"NOR+"比武已经决出胜负："+ fam+HIY" 被 "+fam2+HIY" 击败！\n\n"NOR, 
		filter_array(users(), (:  !$1->query("env/block_leitai") :)));
	log_file("quest/zhaoqin",sprintf(HIC"%-8s%-10s 击败%s(%s)end_fight"NOR, ob->name(1), ob->query("id"), me->name(1), me->query("id")));

	mu_set_station(5);//比赛结束
    if(link_ob->query_temp("quest/bwzh/biwu_host") || ob->query_temp("quest/bwzh/biwu_host"))//如果ob本来就是擂主,继续作擂主不变化 me变成失败者
	{
		mu_set_player(me,4);//me 失败者
		mu_set_player(ob,2);//ob 继续擂主
		link_ob->set_temp("quest/bwzh/biwu_host",1);
		ob->set_temp("quest/bwzh/biwu_host",1);
		mu_unset_player(3);//挑战者清空
		me->delete_temp("quest/bwzh");//me 消除标志
		me->delete_temp("quest/busy");
		link_me->delete_temp("quest/bwzh");//me 消除标志
		link_me->delete_temp("quest/busy");

		tell_object(ob,HIY"\n恭喜你继续担任擂主！");
		tell_object(me,HIY"\n可惜你挑战擂主未成！");
		log_file("quest/zhaoqin",sprintf("%-8s%-10s 继续担任擂主end_fight\n", ob->name(1), ob->query("id")));
	}
	else//ob挑战者 变为擂主 me 如果不是美女，变为失败者
	{
		mu_unset_player(3);//挑战者清空	
		if(!me->query_temp("quest/bwzh/hoster") && !link_me->query_temp("quest/bwzh/hoster"))//me 不是美女	
		{
			mu_set_player(me,4);//me 失败者
			me->delete_temp("quest/bwzh");
			me->delete_temp("quest/busy");
			link_me->delete_temp("quest/bwzh");//me 消除标志
			link_me->delete_temp("quest/busy");
		}
		else
		{
			me->delete_temp("quest/bwzh/biwu_host");
			link_me->delete_temp("quest/bwzh/biwu_host");
		}
		mu_set_player(ob,2);//ob 变为擂主
		ob->set_temp("quest/bwzh/biwu_host",1);//变为擂主
		ob->delete_temp("quest/bwzh/biwu_challenge");
		link_ob->set_temp("quest/bwzh/biwu_host",1);//变为擂主
		link_ob->delete_temp("quest/bwzh/biwu_challenge");
		tell_object(ob,HIY"\n恭喜你挑战擂主成功！");
		tell_object(me,HIY"\n可惜你守卫擂主失败！");
		log_file("quest/zhaoqin",sprintf("%-8s%-10s 担任新届擂主end_fight\n", ob->name(1), ob->query("id")));
	}
	this_object()->delete_temp("quest/bwzh/leitai");
}
       
int start_biwu(object fighter, object fighter2, int num)
{
	object *ob;
	int i;
	
	if( !fighter || environment(fighter) != this_object()){
		if(fighter2) fighter2->quit(fighter2);
		tell_room(this_object(), HIR"\n"+this_object()->query("short")+"比赛中止！\n"NOR);
		return 1;
	}
	
	if( !fighter2 || environment(fighter2) != this_object()){
		if(fighter) fighter->quit(fighter);
		tell_room(this_object(), HIR"\n"+this_object()->query("short")+"比赛中止！\n"NOR);
		return 1;
	}

	num++;
	
	if( num <= 1 ){
		fighter->set_temp("last_damage_from", fighter2);
		fighter2->set_temp("last_damage_from", fighter);

		ob = all_inventory(this_object());
		i = sizeof(ob);
		while (i--) {
			if(!userp(ob[i]) && !ob[i]->query_temp("leitai"))
				destruct(ob[i]);
		}	
		tell_room(this_object(), HIR+"\n\t"+this_object()->query("short")+HIR"上传来第"+chinese_number(1)+"声锣响！\n\n"NOR);
	}
	
	else if( num >= 3 ){
		message_vision(HIR"只听第三声锣响，$N和$n比武正式开始！\n"NOR, fighter, fighter2);
		if( open > 0 )
			message("leitai", HIR +"\n【"NOR+this_object()->query("short")+HIR"】"NOR+HIY"：各位江湖同道，" NOR+fighter->short(1)
			                    +HIY" \n\t      与"NOR+fighter2->short(1)+HIY" 比武招亲擂台赛正式开始！\n\n"NOR, 
          		filter_array(users(), (:  !$1->query("env/block_leitai") :)));
		else message_vision(HIR +"\n【"+this_object()->query("short")+HIR"】"NOR+HIY"：各位江湖同道，" NOR+fighter->short(1)
			                    +HIY" \n\t      与"NOR+fighter2->short(1)+HIY" 比武招亲擂台赛正式开始！\n\n"NOR, 
          		fighter, fighter2);
          		
		fighter->delete("last_death_time");
		fighter2->delete("last_death_time");
		fighter2->kill_ob(fighter);
		fighter->kill_ob(fighter2);
		return 1;
	}
	else tell_room(this_object(), HIR+"\n\t"+this_object()->query("short")+HIR"上传来第"+chinese_number(num)+"声锣响！\n\n"NOR);

	remove_call_out("start_biwu");
	call_out("start_biwu", 4 , fighter, fighter2, num);
	return 1;
}
void auto_kill()
{
	object *ob_list;
	object me,ob;
	int p;

	remove_call_out("auto_kill");
	ob_list = filter_array(children(BIWU_USER), (: clonep :));

	if(!sizeof(ob_list)) return;

	for(p=0; p<sizeof(ob_list); p++) 
	{
		if(!ob_list[p]->query_temp("quest/bwzh/leitai")) continue;
		if(ob_list[p]->query_temp("quest/bwzh/biwu_host") )
			me=ob_list[p];
		if(ob_list[p]->query_temp("quest/bwzh/biwu_challenge"))
			ob=ob_list[p];
	}
	log_file("quest/zhaoqin",sprintf(HIY"%-10s 执行 auto_kill\n"NOR, this_object()->query("short")));

	if(me && ob && (!me->is_fighting()|| !ob->is_fighting()))
	{
		remove_call_out("auto_kill");
		remove_call_out("start_biwu");
		me->set_temp("quest/bwzh/leitai", 2);
		ob->set_temp("quest/bwzh/leitai", 2);
		message_vision(HIW"\n$N看了看$n，冷笑一声：难道"+ob->query("name")+"连挑战都不敢，真是胆小鬼！\n\n"NOR, me,ob);
		message_vision(HIW"\n突然从角落里窜出一个人影，嘿嘿冷笑：“$N和$n比武现在正式开始！”\n\n"NOR, me,ob);
		message_vision(HIR"\n"NOR, me, ob);
		if( open > 0 )
			message("leitai", HIR +"\n【"NOR+this_object()->query("short")+HIR"】"NOR+HIY"：各位江湖同道，"NOR+me->short(1)
                   			   +HIY" \n\t      与"NOR+ob->short(1)+HIY" 比武招亲擂台赛正式开始！\n\n"NOR, 
          		filter_array(users(), (:  !$1->query("env/block_leitai") :)));
		else message_vision(HIR +"\n【"NOR+this_object()->query("short")+HIR"】"NOR+HIY"：各位江湖同道，"NOR+me->short(1)
			                   +HIY" \n\t      与"NOR+ob->short(1)+HIY" 比武招亲擂台赛正式开始！\n\n"NOR, 
          		me, ob);
          		
		me->delete("last_death_time");
		ob->delete("last_death_time");
		me->kill_ob(ob);
		ob->kill_ob(me);
	}
	return;
}	

int do_ok()
{
	object ob, me = this_player();

	if(!me->query_temp("quest/bwzh/leitai") || base_name(me) != __DIR__"npc/copy_user")
		return notify_fail("此命令只有上台比武的选手可用。\n");
    
	if(me->query_temp("quest/bwzh/leitai") == 2)
		return notify_fail("你已经出言挑战了。\n");
		
	me->set_temp("quest/bwzh/leitai", 2);
	if(!objectp(ob = this_object()->query_temp("fighter")))
	{
		this_object()->set_temp("fighter", me);
		message_vision("\n$N做了个鞠："+RANK_D->query_self(me)+"已经准备好了，愿接受任何挑战！\n", me);
	}
	
	else if(environment(ob) != this_object()){
		ob->quit(ob);
		this_object()->set_temp("fighter", me);
		message_vision("\n$N做了个鞠："+RANK_D->query_self(me)+"已经准备好了，愿接受任何挑战！\n", me);
	}
	else {
		this_object()->delete_temp("fighter");
		message_vision("\n$N对$n做了个鞠："+RANK_D->query_self(me)+"冒昧，挑战"+RANK_D->query_respect(ob)+"！\n", me, ob);
		remove_call_out("auto_kill");	
		start_biwu(me, ob, 0);
	}
	return 1;
}

void leaving()
{
	object *ob_list;
	object link_ob,ob,me;
	string fam1,fam2,color;
	int p,i=0;
	remove_call_out("leaving");
	ob_list = filter_array(children(BIWU_USER), (: clonep :));

	if(!sizeof(ob_list)) return;

	log_file("quest/zhaoqin",sprintf(HIY"%-10s 执行 leaving\n"NOR, this_object()->query("short")));

	for(p=0; p<sizeof(ob_list); p++) 
	{
		if(ob_list[p]->is_fighting()) continue;
		if(!ob_list[p]->query_temp("quest/bwzh/leitai")) continue;
		i=i+1;
		if(ob_list[p]->query_temp("quest/bwzh/biwu_host") )
		{
			ob=ob_list[p];
			tell_object(ob_list[p],HIR"时间到，比武以平手算。你继续保持擂主地位！\n"NOR);
			mu_set_station(5);//比赛结束
			mu_set_player(ob_list[p],2);//ob 继续擂主
			log_file("quest/zhaoqin",sprintf("%-8s%-10s 继续担任擂主leaving\n", ob_list[p]->name(1), ob_list[p]->query("id")));
		}
		if(ob_list[p]->query_temp("quest/bwzh/biwu_challenge"))
		{
			me=ob_list[p];
			tell_object(ob_list[p],HIR"时间到，比武以平手算。你挑战擂主失败！\n"NOR);
			link_ob=ob_list[p]->query("link_ob");
			ob_list[p]->delete_temp("quest/bwzh");//me 消除标志
			ob_list[p]->delete_temp("quest/busy");
			link_ob->delete_temp("quest/bwzh");//me 消除标志
			link_ob->delete_temp("quest/busy");
			mu_unset_player(3);//挑战者清空
			mu_set_station(5);//比赛结束
			mu_set_player(ob_list[p],4);//me 失败者
		}
		ob_list[p]->quit(ob_list[p]);
	}
	if(me && ob) 
	{
		fam1 = me->query("family/family_name")?me->query("family/family_name"):"平民百姓";
		color = me->query("color")?me->query("color")+" ":" ";
		fam1 = HIW+fam2+color+NOR+ me->name(1) + "(" + capitalize(me->query("id")) + ")";

		fam2 = ob->query("family/family_name")?ob->query("family/family_name"):"平民百姓";
		color = ob->query("color")?ob->query("color")+" ":" ";
		fam2 = HIW+fam2+color+NOR+ ob->name(1) + "(" + capitalize(ob->query("id")) + ")";
		message_vision(HIW"\n$N一脸高傲的神态：竟然没人敢来挑战，看来"+RANK_D->query_self(ob)+"真是天下无敌了！\n\n"NOR, ob);
		message_vision("\n"+fam1+" 与 "+fam2+" 平手。\n", me, ob);
	if( open > 0 )
		message("leitai", HIY +"\n【"+environment(me)->query("short")+HIY"】"NOR+""+ fam1+HIY" 被 "+fam2+HIY" 竟然平手，以"NOR+me->query("name")+HIY"失败算！\n\n"NOR, 
		filter_array(users(), (:  !$1->query("env/block_leitai") :)));
	}
	
	if(!i)
	{
		remove_call_out("leaving");
		call_out("leaving",120);
	}
	return;
}	

int load_user(object player)
{
	object user;
	string arg, color;

	arg = player->query("id");
	
	if (!arg 
	|| this_object()->query_temp("quest/bwzh/leitai") > 1
	|| base_name(player) ==  BIWU_USER)
		return 0;  

	seteuid(arg);
	user = new( BIWU_USER);   // 去掉了save()，擂台中的一切和本身无关。
	user->set_name("", ({ arg }));
	if(!user->restore()) {
		destruct(user);
		write("User Restore Error，请通知巫师。\n");
		return 0;
	}

	user->setup();		
	user->set_temp("quest/bwzh/leitai", 1);    
	exec(user, player);
	user->set("link_ob", player);
	this_object()->add_temp("quest/bwzh/leitai", 1);
	user->move(this_object());
	user->clear_condition();
	user->reincarnate();
	user->delete_temp("combat_yield");
	user->set("channels", ({ "rumor", "err" }) );
	tell_object(user, YEL"你进入比武擂台，目前处于最佳状态中。\n"NOR"
需要兵器请从物品架上拿 "YEL"NA"NOR" 。
如果你准备好了请打"YEL" OK "NOR"。\n\n");

	arg = user->query("family/family_name")?user->query("family/family_name"):"平民百姓";
	color = user->query("color")?user->query("color")+" ":" ";
        arg = HIW+arg+NOR+" "+color + user->name(1) + NOR "(" + capitalize(user->query("id")) + ")";
	tell_room(this_object(), arg + YEL"进入比武擂台。\n\n"NOR, ({ user }));
	player->set_leader(0);
	if(player->query_temp("quest/bwzh/hoster"))
	{
		user->set_temp("quest/bwzh/hoster",1);
		user->set_temp("quest/bwzh/biwu_host",1);
		tell_object(user, HIR"你现在是第一届擂主！\n"NOR);
		log_file("quest/zhaoqin",sprintf("%-8s%-10s 美女进入擂台load_user\n", player->name(1), player->query("id")));
	}
	else if(player->query_temp("quest/bwzh/biwu_challenge"))
	{
		user->set_temp("quest/bwzh/biwu_challenge",1);
		tell_object(user, HIR"你现在挑战擂主，要抱得美人归，就必须全力以赴！\n"NOR);
		log_file("quest/zhaoqin",sprintf("%-8s%-10s 挑战者入擂台load_user\n", player->name(1), player->query("id")));
	}
	else if(player->query_temp("quest/bwzh/biwu_host"))
	{
		user->set_temp("quest/bwzh/biwu_host",1);
		tell_object(user, HIR"你现在是擂主，要抱得美人归，就必须全力以赴！\n"NOR);
		log_file("quest/zhaoqin",sprintf("%-8s%-10s 擂主进入擂台load_user\n", player->name(1), player->query("id")));
	}
	else 
	{
		log_file("quest/zhaoqin",sprintf(HIR"%-8s%-10s 无名者入擂台load_user\n"NOR, player->name(1), player->query("id")));
	}
	return 1;
}

void mu_unset_player(int i)
//0 全部 1 host 2 winner 3 challenge 4 fail 
{
	object room,obj;
	if (!(room = find_object(query("dest_room"))))
		room = load_object(query("dest_room"));
	if(!room) return;
	obj=present("mu nianci",room);
	if(!obj) return;
	if(i==0) obj->delete("bwzh");
	else if(i==1) obj->delete("bwzh/host_player");
	else if(i==2) obj->delete("bwzh/winner_player");
	else if(i==3) obj->delete("bwzh/challenge_player");
	else if(i==4) obj->delete("bwzh/fail_player");
	
	if(i==0) log_file("quest/zhaoqin",sprintf("%-8s%-10s 标志全部清除mu_unset_player", obj->name(1), obj->query("id")));
	else if(i==1) log_file("quest/zhaoqin",sprintf("%-8s%-10s 美女标志清除mu_unset_player\n", obj->name(1), obj->query("id")));
	else if(i==2) log_file("quest/zhaoqin",sprintf("%-8s%-10s 擂主标志清除mu_unset_player\n", obj->name(1), obj->query("id")));
	else if(i==3) log_file("quest/zhaoqin",sprintf("%-8s%-10s 挑战者已清除mu_unset_player\n", obj->name(1), obj->query("id")));
	else if(i==4) log_file("quest/zhaoqin",sprintf("%-8s%-10s 失败者已清除mu_unset_player\n", obj->name(1), obj->query("id")));

	return;
}
void mu_set_player(object me,int i)
//1 host 2 winner 3 challenge 4 fail 5 station
{
	object room,obj,link_me;
	if (!(room = find_object(query("dest_room"))))
		room = load_object(query("dest_room"));
	if(!room) return;
	obj=present("mu nianci",room);
	if(!obj) return;
	if(!me) return;
	if(!me->query("link_ob"))
	{
		log_file("quest/zhaoqin",sprintf("%-8s%-10s 没有link_ob标志mu_set_player\n", me->name(1), me->query("id")));
		return;
	}
	link_me = me->query("link_ob");
	if(i==5)
		obj->set("station",5);
	else if(i==1) 
	{
		obj->set("bwzh/host_player/name",me->query("name"));
		obj->set("bwzh/host_player/id",me->query("id"));
		obj->set("bwzh/host_player/object",link_me);
		log_file("quest/zhaoqin",sprintf("%-8s%-10s 设置美女%s(%s) mu_set_player\n", obj->name(1), obj->query("id"),obj->query("bwzh/host_player/name"),obj->query("bwzh/host_player/id")));
	}
	else if(i==2)
	{
		obj->set("bwzh/winner_player/name",me->query("name"));
		obj->set("bwzh/winner_player/id",me->query("id"));
		obj->set("bwzh/winner_player/object",link_me);
		log_file("quest/zhaoqin",sprintf("%-8s%-10s 设置擂主%s(%s)mu_set_player\n", obj->name(1), obj->query("id"),obj->query("bwzh/winner_player/name"),obj->query("bwzh/winner_player/id")));
	}
	else if(i==3)
	{
		obj->set("bwzh/challenge_player/name",me->query("name"));
		obj->set("bwzh/challenge_player/id",me->query("id"));
		obj->set("bwzh/challenge_player/object",link_me);
		log_file("quest/zhaoqin",sprintf("%-8s%-10s 挑战者%s(%s)mu_set_player\n", obj->name(1), obj->query("id"),obj->query("bwzh/challenge_player/name"),obj->query("bwzh/challenge_player/id")));
	}
	else if(i==4)
	{
		obj->set("bwzh/fail_player/name",me->query("name"));
		obj->set("bwzh/fail_player/id",me->query("id"));
		log_file("quest/zhaoqin",sprintf("%-8s%-10s 失败者%s(%s)mu_set_player\n", obj->name(1), obj->query("id"),obj->query("bwzh/fail_player/name"),obj->query("bwzh/fail_player/id")));
	}
	return;
}
void mu_set_station(int i)
{
	object room,obj;
	if (!(room = find_object(query("dest_room"))))
		room = load_object(query("dest_room"));
	if(!room) return;
	obj=present("mu nianci",room);
	if(!obj) return;
	obj->set("station",i);
	log_file("quest/zhaoqin",sprintf("%-8s%-10s 设置状态为%d mu_set_station\n", obj->name(1), obj->query("id"),i));
	return;
}



