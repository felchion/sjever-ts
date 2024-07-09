// /u/ciwei/tong.c
#include <ansi.h>
#include <get_place.h>
#define CMDS_SET "/cmds/wiz/setask"

inherit NPC;
inherit F_SAVE;

string *msg_dunno;
mapping baowu_list;
string* shuang_list;
string* no_tell;
mapping askwhat;
//add name here rumors

int do_ask(string arg);
int ask_me(object me,string arg);
string askabout(object me,string arg);
string query_save_file();

int set_ask(string pattern, mapping def)
{
	askwhat[pattern] = def;
	save();
	return 1;
}
int delete_ask(string pattern)
{
	if(!stringp(pattern)) return 0;
	map_delete(askwhat,pattern);
	save();
	return 1;
}
mapping query_ask()
{
	return askwhat;
}
string* query_baowu()
{
	return keys(baowu_list);
}

void create()
{
	seteuid(ROOT_UID);	
	//if( !restore() && !mapp(askwhat) )
	//	askwhat = ([]);
	call_out("dest_busy",2);
	
	set_name("万事通",({"wanshi tong","tong","wanshi","liulang"}));
	set("title",HIW"书剑半仙"NOR);
        set("nickname",HIY"无所不知"NOR);
        set("rank_info/rude", "万事通老大");
	set("no_quest",1);
	set("no_ansuan",1);
	set("no_get","只怕你拿了丢不了。\n");
	set("gender", "男性" );
	set("shen_type", 1);
	set("age", 23+random(6));
       	set("str", 15);
       	set("con", 15);
       	set("int", 27);
       	set("dex", 20);
       	set("env/wimpy", 110);
       	set("long", "他是华山吹牛堂流浪的嫡系弟子。\n"
       		    "听说已经青出于蓝而胜于蓝了。" );
       	set("combat_exp", 2500);
       	set("attitude", "friendly");
       	set("max_qi", 200);
       	set("max_jing", 200);       	
       	set_skill("unarmed", 10);
       	set_skill("dodge", 10);
	set_skill("parry", 10);	
	restore();
	call_out("fix_error",1);
			
	setup();
	carry_object(BINGQI_D("sword/gangjian"));
        carry_object("/d/gumu/obj/buxue1")->wear();
        carry_object("/d/gumu/obj/pao1")->wear();
}
void init()
{
	//add_action("do_ask","ask");
}

int ask_busy;

string query_save_file()
{
        return "/log/wanshitong";
}

int do_ask(string arg)
{
	object me;
	string dest,topic;
	
	me = this_player();
	if (me->is_busy())
                return notify_fail("您先歇口气再说话吧。\n");
        if (me->query_temp("ask_tong_times") > 10){
        	message_vision("$n向$N翻了翻白眼，说道：你怎么这么jjww的？继续问我要收费的，给我四两银子先。\n", me, this_object());
        	return 1;
	}
        if (!arg || sscanf(arg, "%s about %s", dest, topic) != 2)
                return notify_fail("你要问谁什么事？\n");
        if(  member_array(dest,my_id) == -1 )
        	return "/cmds/std/ask"->main(me,arg);        
        if (!INQUIRY_D->parse_inquiry(me, this_object(), topic))
                message_vision("$N向$n打听有关『" + topic + "』的消息。\n", me, this_object());
       	return ask_me(me,topic);
}

int accept_object(object who, object ob)
{
	
	if (ob->query("money_id") && ob->value() >= 400) 
	{
		message_vision("$n向$N笑了笑说道：有钱人，继续问吧，嘻嘻。\n", who, this_object());
		who->delete_temp("ask_tong_times");
		return 1;
	}
	return 0;
}

int ask_me(object me,string arg)
{
	object ob = this_object();
	int chang,i;
	string re;
	if (!living(ob)) {
                message_vision("但是很显然的，$n现在的状况没有办法给$N任何答覆。\n", me, ob);
                return 1;
        }
        chang = strlen(arg);        
        for(i=0;i<chang;i++)
        {
        	if(arg[i]<'A' || arg[i]>'z' || ( arg[i]<'a' && arg[i]>'Z' )) break;
        }        
        if (i == chang &&((arg[i-1]>='A' && arg[i-1]<='Z')||( arg[i-1]>='a' && arg[i-1]<='z'))) arg = lower_case(arg);
        
        if (arg == ob->query("name") || ob->id(arg))
        	message_vision(CYN "$N说道：那就是" + RANK_D->query_self_rude(ob) + CYN "呀。\n" NOR, ob);
        else if (arg == me->query("name") || me->id(arg))
        	message_vision(CYN "$N说道：那就是" + RANK_D->query_rude(me) + CYN "你呀。\n" NOR, ob);
        //else if (ask_busy > 0 ) message_vision( CYN "$N对$n不耐烦的说道：“我正忙着，你等会再问吧。”\n" NOR,ob,me );
        else if (random(10) == 1 ) message_vision( CYN "$N对$n答道：“"+(replace_string(no_tell[random(sizeof(no_tell))],"arg",arg))+"”\n" NOR,ob,me );
        else if (stringp(re = askabout(me,arg)))
        {
        	re = replace_string(re,"textn","\n");
        	message_vision( CYN "$N对$n答道：“"+re+"”\n" NOR,ob,me );
        	//加入semote
        	if(mapp(askwhat)&& mapp(askwhat[arg]) && stringp(askwhat[arg]["semote"]) && askwhat[arg]["semote"]!="" ) "/adm/daemons/emoted"->do_emote(ob,askwhat[arg]["semote"]);
        }
        else message_vision(msg_dunno[random(sizeof(msg_dunno))], me, ob);
        
        me->add_temp("ask_tong_times",1);	
	return 1;
}
string askabout(object me,string arg)
{
	object ob;
	object* obs;	
	object user;
	//object where;
	mapping fam;
	string name = "";
	string msg;
	int exp,i;
	int flag = 0;
	ask_busy = 2;
	call_out("dest_busy",2);
	//特殊
	if(arg == "name"){
		return "小人本住在苏州的城边，
家中有屋又有田，生活乐无边， 
谁知那唐伯虎，他蛮横不留情面， 
勾结官府目无天，占我大屋夺我田。
我爷爷跟他来翻脸，惨被他一棍来打扁， 
我奶奶骂他欺善民，反被他捉进了唐府， 
强奸了一百遍，一百遍，最后她悬梁自尽遗恨人间。 

他还将我父子，逐出了家园，流落到江边， 
我为求养老爹，只有独自行乞在庙前， 
谁知那唐伯虎，他实在太阴险 ，
知道此情形，竟派人来暗算，把我父子狂殴在市前。

小人身壮健，残命得留存，可怜老父他魂归天， 
为求葬老爹，唯有卖身为奴自作贱， 
一面勤赚钱，一面读书篇， 
发誓把功名显，手刃仇人意志坚！ 
从此唐寅诗集在身边，我铭记此仇不共戴天！！！ ";
	}
	if(arg == "rumors")
	{
		return "听说万事通即将光临SJ了．";
	}
	if(arg == "here")
	{
		return "...";
	}
	if(arg == "万里长城万里长")
	{
		return "合心齐力建家乡！！";
	}
	if (arg =="job" || arg =="任务" || arg=="quest" )
	{
		obs = children(USER_OB);
		i = random(sizeof(obs));
		msg = obs[i]->query("name") + "(" + obs[i]->query("id") + ")";
		switch(random(7))
		{
			case 0:return "我饿了，你去给我弄点吃的来。";
			case 1:return msg+"最近总跟本帮作对，你去把他们杀了。";
			case 2:return msg+"功夫不错，如果得他相助，一定天下无敌，你给我去把他请来。";
			case 3:return "你正忙着其他任务呢。";
			case 4:return "想累死大爷啊！！一边凉快去！！";
			case 5:return "最近手气不好，身上的钱输光了，你去给我弄点黄金(Gold)去。";
			case 6:return "听说古墓派最近新来了一个MM,你去给我弄来。";
		}
	}
	if (arg =="fangqi" || arg=="放弃" || arg=="cancel")
	{
		switch(random(3))
		{
			case 0:return "完不成也没关系，下次再来给大爷出力吧";
			case 1:return "这么简单都完不成，你太令我失望了";
			case 2:return "你怎么这么笨，让那个恶贼跑了？？";
		}
	}
	//宝物在哪里	huju 未完成
	if(member_array(arg,keys(baowu_list) ) != -1 )
	{		
		if (random(5) != 1 ) return arg+"……什么"+arg+"，没听过，没听过。";
		obs = children(baowu_list[arg]);		
		if(sizeof(obs) >= 1 )
		{
			ob = obs[random(sizeof(obs))];
			ob = environment(ob);
			if(!ob)	return "就是不说话！";
			if(ob->is_character()) return random(2)?(arg+"不知道被谁那走了……"):(arg+"好象是落在"+ob->name()+"("+ob->query("id")+")手中了。");
			return arg+"被人遗失在"+TASK_D->get_regions(base_name(ob))+"附近了。" ;
		}
		else return arg+"……什么"+arg+"，没听过，没听过。";		
	}	
//门派	
//shaer 那里
	if(member_array(arg,shuang_list) != -1 )
	{
		ob = find_object("/d/city/npc/shuang")?find_object("/d/city/npc/shuang"):load_object("/d/city/npc/shuang");
		switch(arg)
		{
			case "美丽":case "beauty":
				return ob->ask_beauty();
			case "丑陋":case "ugly":
				return ob->ask_ugly();
			case "年龄":case "age":
				return ob->ask_age();
			case "经验":case "exp":
				return ob->ask_exp();
			case "身材":case "fat":
				return ob->ask_fat();
			case "杀人":case "pk":
				return ob->ask_pk();
			case "存款":case "balance":
				return ob->ask_balance();			
		}
	}	
// more ..	
	if (mapp(askwhat) && mapp(askwhat[arg]) && stringp(askwhat[arg]["word"])) return askwhat[arg]["word"];
	//is ob return name
	exp = strlen(arg);
	while (exp--) if (arg[exp] < 'a' || arg[exp] > 'z')
		return 0;
	if(!objectp(user = find_player(arg))) 
	{
		flag = 1;
		if(!objectp(user = CMDS_SET->get_player(arg))) return 0;
	}
	if(objectp(user)){	
		name = user->query("name");
		fam = user->query("family");
		exp = user->query("combat_exp");		
		
		/*if (!flag && random(10) > 7)
		{
		       msg = "哦，你说"+name+"("+user->query("id")+")啊，";
		       where = environment(user);
       		       if (!where) 
       		       		msg += "这个小兔崽子好像消失了耶...";  
       		       else
       		       		msg += "有人在"+get_place(base_name(where)) + (string)where->query("short") +CYN+"看到这个人在晒太阳。";
       		       	return msg;
		}*/
			
		if(name!="")
		{	
			msg = "哦，你说"+name+"("+user->query("id")+")啊，这人倒是名不见经传。";
			if (mapp(fam)) msg += "据说是"+fam["family_name"]+(fam["master_name"]?(fam["master_name"]+"手下"):"")+(exp>1000000?"高手。":"新手。");
			if (stringp(user->query("nickname"))) {
				msg+= "此人外号叫「"+user->query("nickname")+CYN+"」，";
				switch(random(3))
				{
				case 0:msg += "够土。";break;
				case 1:msg += "够拽。";break;
				case 2:msg += "够酷。";break;
				}
			}
			if (flag) destruct(user);
			return msg;
		}
		
	}
	return 0;	
}
void dest_busy()
{
	ask_busy--;
	if(ask_busy > 0 ) call_out("dest_busy",2);
}
void fix_error()
{
}