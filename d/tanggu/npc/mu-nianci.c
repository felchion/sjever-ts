// mu nianci.c 穆念慈
// By tangfeng 2004

#include <ansi.h>
inherit NPC;

#define NEXT_TIME 7200
//从招亲到结束全部时间
#define WAIT_TIME 10
//从招亲到下次招亲时间间隔
//测试期间

#define ASKTIME 10
#define ZHAOQIN_DIR "/d/tanggu/"
//文件目录
#define BIWU_LEITAI ZHAOQIN_DIR"biwuleitai.c"
//比武擂台
#define BIWU_USER ZHAOQIN_DIR"npc/copy_user.c"
//复制人物

int start_time,over_time,ask_time;
//0 1 开始2 设置完毕 3 美女擂主 4 玩家擂主 5 比赛结束 6 结婚中 
//query("bwzh/host_player") [id/name/object] 美女
//query("bwzh/winner_player") [id/name/object] 擂主
//query("bwzh/challenge_player") [id/name/object] 挑战者
//query("bwzh/fail_player") [id/name/object] 失败者(只做纪录保存用)

int ask_biwu();
int ask_lihun();

int yingzhao(string arg);
int write_qi(string arg);
int chakan (string arg);
int answer (string arg);

int check_player(object me,int i);
void init_over();
void biwu_stop(int i);

void choose_mode(string arg, object me);
void choose_age(string arg, object me);
void choose_per(string arg, object me);
void choose_exp(string arg, object me);
void choose_ok(string arg, object me);
void choose_other(object me,int x);

string query_setting();

void create()
{
	set_name("穆念慈",({"mu nianci", "mu", "nianci"}));
	set("long", "这位姑娘虽然穿着朴素，但秀丽之质难以掩却。\n");
	set("age", 25);
	set("attitude", "friendly");
	set("shen", 0);
	set("per", 30);
	set("str", 20);
	set("int", 20);
	set("con", 20);
	set("dex", 20);
	set("no_get", 1);
	set("no_bark", 1);
	set("job_npc", 1);
	set("no_ansuan", 1);
	set("leitai",BIWU_LEITAI);

	set("max_qi", 4500);
	set("max_jing", 3200);
	set("eff_jingli", 3500);
	set("jiali", 200);
	set("combat_exp", 2500000); 
	set("unique", 1);
	set("no_clean_up",1);//考虑ing

	set_skill("dodge",220);
	set_skill("force", 220);
	set_skill("literate", 200);
	set_skill("strike", 220);
	set_skill("tiezhang-zhangfa", 220);
	set_skill("shuishangpiao", 220);
	set_skill("shenzhao-jing", 220);
	set_skill("parry", 220);
	set_skill("medicine", 160);

	map_skill("force", "shenzhao-jing");
	map_skill("dodge", "shuishangpiao");
	map_skill("parry", "tiezhang-zhangfa");
	map_skill("strike", "tiezhang-zhangfa");
	prepare_skill("strike", "tiezhang-zhangfa");

	set("inquiry", ([
		"比武招亲"	:	(: ask_biwu :),
		"离婚"	:	(: ask_lihun :),
		"郭靖"	:	"郭大侠现在应该还在襄阳，镇守边境。",
		"杨康":	    "...............................冤孽！",
	]));

	setup();
	carry_object(ARMOR_D("pink_cloth"))->wear();
	carry_object(ARMOR_D("shoe"))->wear();

}
void init()
{
	add_action("yingzhao","yingzhao");//over
	add_action("fabu","fabu");//over
	add_action("cancel","cancel");//over
	add_action("chakan","chakan");//over
	add_action("write_qi","write");//over
	add_action("answer","answer");//over
}

#include "ask_biwu.h";
#include "check_player.h";
#include "write_qi.h";
#include "choose_mode.h";
#include "choose.h";
#include "choose_ok.h";
#include "yingzhao.h";
#include "biwu_over.h";
#include "checking.h";
#include "zhaoqin.h";
#include "answer.h";

int fabu(string arg)
{
	object me,ob,who;
	int money = 500000;
	me = this_player();
	ob = this_object();
	if(!me->query_temp("quest/bwzh/hoster")) return 0;
	if(!arg || ( arg!="xiaoxi" && arg!="xiao xi" && arg!="message")) return 0;
	if(!query("station"))
	{
		command("? "+me->query("id"));
		command("say 现在还未有招亲活动，呵呵。");
		return 1;	
	}

	who=find_player(query("bwzh/winner_player/id"));

	switch (MONEY_D->player_pay(me, money)) {
		case 0:
		case 2:
		return notify_fail("发布一次消息需要的费用是"+MONEY_D->money_str(money)+"，你身上带的黄金不够了。\n");
	}
	command("nod");
	if(random(2)) CHANNEL_D->do_channel(this_object(),"rumor",HIY+"听说有人正在比武招亲，欢迎各路豪杰可踊跃报名。"NOR);
	else if(who && random(2)) CHANNEL_D->do_channel(this_object(),"rumor",HIY+"有人正在比武招亲，擂主目前是"+who->query("name")+"，欢迎各路豪杰可踊跃报名。"NOR);
	else CHANNEL_D->do_channel(this_object(),"chat",HIY+"听说"+me->query("name")+"小姐正在比武招亲，欢迎各路豪杰可踊跃报名。"NOR);
	command("say 放心吧，一会小伙子们就都来了。");
	command("haha");
	return 1;
}

int chakan(string arg)
{
	object me;
	string str;
	me = this_player();
	if(arg ) return 0;
	if(!wizardp(me)) return 0;
	str = query_setting();
	tell_object(me,str);    
	return 1;
}
void die()
{
	reincarnate();
	message_vision("$N深深吸了几口气，脸色看起来好多了。\n",this_object());
	return;	
}
int ask_lihun()
{
	object me=this_player();

	if(me->query("marry"))
	{
		if(!wizardp(me))
		{
			command("say 还是好好珍惜你的伴侣吧！"+me->query("marry/name")+"("+capitalize(me->query("marry/id"))+")也不容易啊！");
			command("pat "+me->query("id"));
			return 1;
		}
		if(me->query("gender")=="男性")
			command("chat "+HIY"英雄啊，为了QUEST，你竟然抛弃妻子，真是伟大，感激啊感激！"NOR);
		else 
			command("chat "+HIY"美女啊，为了QUEST，你竟然把老公丢了，到这里再找，感激啊感激！"NOR);
		me->delete("marry");
        command("chat* thank "+me->query("id"));
		if (present("yu pei", me)) present("yu pei", me)->lost(me);
		if (present("xiang nang", me)) present("xiang nang", me)->lost(me);
	}
	else 
	{	
		command("say 单身的幸福人生！");
		command("tsk "+me->query("id"));
	}
	return 1;
}
void init_over()
{
	object ob;
	ob=query("bwzh/host_player/object");
	if(ob)
	{
		ob->delete_temp("quest/bwzh");
		ob->delete_temp("quest/busy");
		ob->delete_temp("apply/short");  
	}
	ob=query("bwzh/winner_player/object");
	if(ob)
	{
		ob->delete_temp("quest/bwzh");
		ob->delete_temp("quest/busy");
		ob->delete_temp("apply/short");  
	}
	ob=query("bwzh/challenge_player/object");
	if(ob)
	{
		ob->delete_temp("quest/bwzh");
		ob->delete_temp("quest/busy");
		ob->delete_temp("apply/short");  
	}
	delete("bwzh");
	over_time=time();
	set("station",0);
	return;
}
string query_setting()
{
	object where;
	
	string str="";
	str += "\n状  态："+query("station")+"";
	str += "|经验："+query("bwzh/exp");
	str += "|相貌："+query("bwzh/per");
	str += "|年龄："+query("bwzh/age");
	str += "\n";
	if(query("bwzh/host_player"))
	{
		str += "\n主  人："+query("bwzh/host_player/name");
		str += "|"+query("bwzh/host_player/id");
		where = environment(query("bwzh/host_player/object"));
		if(where)
		{
			str+="|"+where->query("short");
			str+="|"+base_name(where);
		}
		else
		{
			str+="|"+"不详";
			str+="|"+"none";
		}
	}
	if(query("bwzh/winner_player"))
	{
		str += "\n擂  主："+query("bwzh/winner_player/name");
		str += "|"+query("bwzh/winner_player/id");
		where = environment(query("bwzh/winner_player/object"));
		if(where)
		{
			str+="|"+where->query("short");
			str+="|"+base_name(where);
		}
		else
		{
			str+="|"+"不详";
			str+="|"+"none";
		}
	}
	if(query("bwzh/challenge_player"))
	{
		str += "\n挑战者："+query("bwzh/challenge_player/name");
		str += "|"+query("bwzh/challenge_player/id");
		where = environment(query("bwzh/challenge_player/object"));
		if(where)
		{
			str+="|"+where->query("short");
			str+="|"+base_name(where);
		}
		else
		{
			str+="|"+"不详";
			str+="|"+"none";
		}
	}
	if(query("bwzh/fail_player"))
	{
		str += "\n失败者："+query("bwzh/fail_player/name");
		str += "|"+query("bwzh/fail_player/id");
	}
	str += "\n";
	return str;
}
