//man.c for dzd job by Ciwej@SJ
//show one chat robot here ^_^

#include <ansi.h>
#define XKD_D "/d/foshan/xkdd.c"

inherit NPC;

string ask_job();
int debug_trigger(string arg);
int ask_dzd();

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
		"尝试":(:ask_job:),
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
void init()
{
	if(wizardp(this_player())) add_action("debug_trigger","debug");
}

int debug_trigger(string arg)
{	
	if(XKD_D->change_debug())
	{
		write("调试模式打开了！\n");
		return 1;		
	}
	write("关闭调试模式完成。\n");
	return 1;	
}


int ask_dzd()
{
	command("say 危险！危险！据说上岛的人只有一个能活着回来的，但是回报也很丰厚。如果您想尝试一下，我倒是可以带您去！\n");
	this_player()->set_temp("ask_dzd",1);
	return 1;
}

string ask_job()
{
	object me,flag;
	int time;
		
	me = this_player();
	
        //return ("最近大海风浪比较大，暂时无法航行，请今天晚上在来。")
	if(!XKD_D->query_mode() && !me->query_temp("ask_dzd")) return 0;
	if(XKD_D->query_stat()=="ingame")
		return 0;
	if(XKD_D->query_stat()=="overbusy")
		return "我正忙着呢，等会再说。\n";	
	if( !XKD_D->query_mode() && ( me->query_condition("job_busy") ||  me->query("job_name")==XKD_D->query_name())) return "您现在这么忙，还想尝试什么啊？\n";
	if(!XKD_D->query_mode() && mapp(me->query_conditions_by_type("poison"))){
		command("hmm "+getuid(me));
		return "您身中剧毒，怕是承受岛上煎熬。\n";
	}
	if(!XKD_D->query_mode() && mapp(me->query_conditions_by_type("hurt"))){
		command("hmm "+getuid(me));
		return "想占这个现成便宜？\n";
	}//over back I will clear_condition so return of poison for forbid player use bug	
	
	if(!XKD_D->query_mode() && me->query_condition("killer")) return XKD_D->query_name()+"可不想跟官府扯上什么关系。\n";
	if(!XKD_D->query_mode() && me->query("combat_exp") <20000) return "您的武功未成，不去也罢。\n";
	if(!XKD_D->query_mode() && wizardp(me) && getuid(me)!="ciwei") return "您老就别来捣乱了。\n";
	switch(time = XKD_D->add_player(me,1))
	{
		case -3:command("wawa");
			return "船已经开走啦。\n";
		case -2:command("tm3");
			return "人太多了，我怕小船半路上沉了啊。\n";
		case -1:command("hmm");
			return "不是跟您说了么？";
		case 0:command("sorry");//debug
			return "刺猬说了，"+XKD_D->query_name()+"怕是得要修整几天了。\n";
	}
	flag = new(MISC_D("dzd_flag"));	
	if(flag) flag->move(me);
	me->delete_temp("ask_dzd");
	command("hehe zhongnian nanzi");
	command("byebye "+getuid(me));
	command(sprintf("whisper %s %s请与%s前到佛山老渡口，自有人接待贵客上船。"NOR,getuid(me),HIW+"",CHINESE_D->chinese_date(time)));
	//XKD_D->demon_tell(2,me,sprintf(HIW"您看这铜牌后面刻有：%s到小渔村，自有人接待贵客到侠客岛。\n"NOR,CHINESE_D->chinese_date(time)) );
	return RANK_D->query_respect(me)+"乃当时英雄，便请到"+XKD_D->query_name()+"一行，生死各安天命。\n";
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
/*
设置时间：	期间give有效，其他时间不能give了
你给中年男子一个特制月饼。
中年男子叹道：「又是中秋了么？哎，我离开白驼山也很久啦……」
中年男子道：「你小子还真知道孝顺。有没有给老爸老妈送个中秋问候啊……，别就知道找女孩子……。」
中年男子说到：「有什么事情就来找我吧。」

玩家身上设置标记，表示带着月饼做JOB


（自动消除JOB BUSY，就是说中秋月饼可以消除大智岛JOB BUSY，玩家可以连续做。）
*/
//add by tangfeng
int accept_object(object who, object ob,object me)
{
   //9月28日-10月8日
   //1096300800-1097164800
   command("say 你先留着吧，北京时间今天晚上在给我，呵呵");
	   return 0;
   if(ob->query("id")=="xue yuebing"
	   &&ob->query("festival")=="mid-autumn"
	   &&ob->query("owner")==who->query("id"))
   {
		if(time()<1096300800 && !wizardp(who))
		{
			command("ah");
			command("say 这么早就送月饼了啊，快到中秋了么？哎，我离开白驼山也很久啦……");
			command("say 你小子还真知道孝顺。有没有给老爸老妈送个中秋问候啊……，别就知道找女孩子……");
			who->start_busy(1);
			return 0;
		}
		else if(time()>1097164800 && !wizardp(who))
		{
			command("?");
			command("say 中秋好像已经过去了吧？哎，我离开白驼山也很久啦……");
			who->start_busy(1);
			return 0;
		}
     	command("sigh");
     	command("say 又是中秋了么？哎，我离开白驼山也很久啦……");
     	command("say 你小子还真知道孝顺。有没有给老爸老妈送个中秋问候啊……，别就知道找女孩子……");

		if(who->query_temp("quest/gift/mid-autumn/yuebing"))
		{
			command("smile");
			command("say 你刚才给我了，这个还是算了，留给你自己吧。");
			return 0;
		}        
		who->set_temp("quest/gift/mid-autumn/yuebing",1);
     	who->start_busy(1);
		call_out("destroying", 1, ob);        
		return 1;
   }
   else   command("? "+(string)who->query("id"));
   return 0;
}
void destroying(object obj)
{
        if(!userp(obj)) destruct(obj);
        return;
}
