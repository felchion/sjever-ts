//man.c for dzd job by Ciwej@SJ
//show one chat robot here ^_^

#include <ansi.h>

inherit NPC;

string ask_job();
int ask_dzd();

#define MST		"/clone/npc/hanghai_master"		// 经纬仪，游戏指令主支持

void create()
{
	set_name("中年男子", ({ "zhongnian nanzi","man" }));
	set("gender", "男性");
    	set("age", 37);
    	set("long", @LONG
他是一名年近四旬的中年男子。
脸上带着淡淡的愁容，似乎多年没回故乡了。
LONG);
    	set("combat_exp", 10000000);
    	set("shen", 10000000);
    	set("shen_type", 1);
	set("attitude", "peaceful");
	set("job_npc",1);
	set("no_ansuan",1);
	set("no_get", "中年男子对你而言太重了。\n");
	
	set("unique", 1);
			
	set("inquiry",
	([
		"南海探宝":(:ask_job:),
		"大智岛" : (: ask_dzd :),
		"name":"问小的名字？太久啦，想不起来了",
		//"香蕉":(:debug_trigger:),
		//"rumors":"最近有人手持赏善罚恶令在江湖中行侠仗义，快意江湖！",
		]) );
	
    	set_skill("strike", 470);
	set_skill("dodge", 470);
	set_skill("parry", 470);
	set_skill("tiyunzong", 470);
	set_skill("taizu-quan", 470);
	set_skill("cuff", 470);
	set_skill("force", 470);
	set_skill("hamagong", 470);
	set_skill("literate", 200);
	
	map_skill("force","hamagong");
	map_skill("strike","hamagong");
	map_skill("parry","hamagong");
	map_skill("dodge","tiyunzong");
	map_skill("cuff","taizu-quan");
	
	prepare_skill("strike","hamagong");
	prepare_skill("cuff","taizu-quan");
	
	set("max_qi",20000);
	set("max_jing",20000);
	set("max_jingli",20000);
	set("eff_jingli",20000);
	
	set("oyf/hamagong",3);
	
	set("chat_chance_combat", 99);
	set("chat_msg_combat", ({
 		(: exert_function, "nizhuan" :),
 		(: perform_action, "strike.hama" :),
	}));
	
	set("chat_chance", 99);
	set("chat_msg", ({
		(: exert_function, "nizhuan" :),
 		(: perform_action, "strike.hama" :),
 		//query("name")+"簇了簇眉，似乎想要说些什么。\n",
	}) );
					
    	add_temp("apply/attack", 50);
    	add_temp("apply/defense", 40);
    	add_temp("apply/armor", 40);	
	if(clonep()) 
	{
		CHANNEL_D->register_relay_channel("chat");
		carry_object("/clone/misc/cloth")->wear();		
	}
	setup();
}

int ask_dzd()
{
	command("say 危险！危险！据说上岛的人只有一个能活着回来的，不过听说那地方有很多宝物耶！\n");
	//this_player()->set_temp("ask_dzd",1);
	return 1;
}


string ask_job()
{
	object me = this_player(), mst_obj;

	if( me->query_temp("hanghai_gameing") )
		return "你不是去南海探宝了吗？！";
	if(me->query_condition("killer")) 
		return "我可不想跟官府扯上什么关系。\n";
	if( file_size( MST + ".c" ) < 1 )
		return "现在暂停开放！";

	command("say 好吧，那就试试吧。");
	write("\n========= 请稍候，正在执行操作…… ==========\n\n");

	mst_obj = new( MST );

	mst_obj->new_game(me);
	mst_obj->move(me);
	me->move("/d/foshan/hanghai_room");

	return "";
}
void kill_ob(object ob)
{
	ob->remove_killer(this_object());
	remove_killer(ob);
	message_vision("$N正想对中年男子下毒手，忽然见中年男子眼中精光一闪，吓的忙退了回来！”\n", ob);
	return;
}
void die()
{
	reincarnate();
	message_vision("$N深深吸了几口气，脸色看起来好多了。\n",this_object());
	return;	
}

void relay_channel(object me, string verb, string arg)
{	
	call_out("rerere",random(3)+1,me,verb,arg);
}


void rerere(object me, string verb, string arg)
{	
	string ret;
	int emote = 0;
		
	if(!me) return;	
	if(this_object()->query_temp("chat_busy")) return;
	/*
	switch(arg)
	{
		case "你好":ret = "你也好";break;
		case "fuck":ret = "fuck u 2";break;
		case "i love u":ret = "faint";emote=1;break;
		case "byebye":ret = "wave";emote=1;break;
		//case "fuck";ret = "fuck u 2";break;
		//case "fuck";ret = "fuck u 2";break;	
	} */	
	if(strsrch(arg,"无聊地用手指捅了捅中年男子，可是中年男子象个木人一样，什么反应也没有。")!=-1)
	{
		switch(random(11)){
			case 1:
			case 2:
			case 3:
			case 4: emote = 1;
				ret = "poke zhongnian nanzi";
				break;
			case 5:
			case 6:
			case 7: emote = 1;
				ret ="heng";
				break;
			case 10:
				if(!me->query("marry")) return;
				emote = 0;
				ret ="我是NPC耶，你也捅？不怕我找你的"+me->query("marry/name")+"麻烦？";
				break;
			case 0:
			case 8: emote = 1;
				ret = "hmm "+getuid(me);
				break;
			case 9:return;
		}
	}
	else if(strsrch(arg,"朗声说道：“今番良晤，豪兴不浅，他日江湖相逢，再当杯酒言欢。咱们就此别过。”说罢袍袖一拂，飘然离潭。")!=-1)
	{
		switch(random(4)){
			case 0:	emote = 1;
				ret = "wave "+getuid(me);
				break;
			case 1: emote = 1;
				ret = "byebye "+getuid(me);
				break;
			case 2: emote = 1;
				ret = "wave";
				break;
			case 3: return;
		}
	}
	/*
	【闲聊】(Nipciwei)掐刺猬抱拳道：“青山不改，绿水常流，咱们后会有期！”
	else if(strsrch(arg,"")!=-1)
	{
		emote = 1;
		ret = "wave";
	}
	*/
	if(!undefinedp(ret)){
		CHANNEL_D->do_channel(this_object(),"chat",ret,emote);
		set_temp("chat_busy",1);
		call_out("delete_temp",5+random(4),"chat_busy");
	}
}
