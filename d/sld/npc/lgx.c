// 陆高轩

inherit F_MASTER;
inherit NPC;
#define	SEAWAR_D	"/d/sld/seawar/seaward"
#include <ansi.h>

int ask_lingpai();
int ask_yaopu();
int ask_yaolu();
int ask_yaozhong();
int ask_makeyao();

string ask_seawar();

void create()
{
	set_name("陆高轩", ({ "lu gaoxuan","lu" }));
	set("long", "他四十来岁年纪，文士打扮，神情和蔼可亲，他是神龙教的护法。\n");
	set("gender", "男性");
	set("title", "神龙教护法");	
	set("age", 42);       

	set("str", 20);
	set("int", 22);
	set("con", 22);
	set("dex", 22);
	set("per", 22);
	
	set("no_ansuan",1);
	set("no_bark",1);
	set("no_quest",1);

	set("combat_exp", 600000);
	set("shen", -5000);
	set("attitude", "peaceful");
	set("max_qi",2500);
	set("max_jing",2500);
	set("neili",2000);
	set("max_neili",2000);
	set("jingli",2000);
	set("eff_jingli",2000);
	set("jiali",50);
	set("score", 2000);

	set_skill("force", 100);
	set_skill("dodge", 100);
	set_skill("parry",100);
	set_skill("strike",100);
	set_skill("throwing",100);
	set_skill("literate",100);
	set_skill("poison",100);
	set_skill("shenlong-yaoli",100);
	set_skill("huagu-mianzhang", 100);
	set_skill("youlong-shenfa",100);
	set_skill("dulong-dafa",100);
      
	map_skill("force", "dulong-dafa");
	map_skill("dodge", "youlong-shenfa");
	map_skill("strike", "huagu-mianzhang");
	map_skill("parry","huagu-mianzhang");
	map_skill("poison","shenlong-yaoli");
      
	create_family("神龙教",2, "护法");
	set("chat_chance", 2);
	set("chat_msg", ({
		CYN"陆高轩说道：最近药丸制得不多，哎！\n",NOR
		CYN"陆高轩说道：神龙岛兵强马壮，还有罗刹人帮忙，谁都不怕。\n"NOR,
		CYN"陆高轩说道：罗刹人的炮船真是厉害，此战必胜无疑！\n"NOR,
		CYN"陆高轩说道：有洪教主护佑，此战必胜！\n"NOR,
	}) );
	set("inquiry", ([
		"令牌" : (: ask_lingpai :),
		"通行令牌" : (: ask_lingpai :),
		"lingpai" : (: ask_lingpai :),
		"ling pai" : (: ask_lingpai :),
		
		"攻打清军" :	(: ask_seawar() :),
		"job" :		(: ask_seawar() :),
		"护法" :	(: ask_seawar() :),
		"清军" :	"我们正在和清朝水师对峙之中，开来参战吧，好处少不了你的。",
		
//		"药圃": (: ask_yaopu :), 
//		"炼药": (: ask_makeyao :),
//		"制药": (: ask_makeyao :),
//		"药炉": (: ask_yaolu :),
//		"药种": (: ask_yaozhong :),
	]));

	setup();
	carry_object(__DIR__"obj/changsan")->wear();     
}


void init()
{
	add_action("do_steal","steal");
}

int do_steal(string arg)
{
	object ob=this_object();
	object me=this_player();
	object lpai;

	if(arg!="令牌" && arg!="通行令牌" && arg!="lingpai" && arg!="ling pai")
		return 0;

	if(!living(ob))
		return notify_fail("陆高轩身上现在没有令牌可以偷。\n");

	lpai = present("ling pai", me);
	if(objectp(lpai) &&  lpai->query("sld")) {
		write("你已经有了令牌，还偷它作甚？\n");
		return 1;
	}
	tell_object(me,"你不动声色地靠近陆高轩，偷偷地把手向陆的身上伸去...\n");
	if(me->query("kar") > random(20)) {
		tell_object(me,"你成功地偷到了块通行令牌!\n");
		lpai= new(__DIR__"obj/lingpai");
		lpai->move(me);
	}
	else {
		tell_room(environment(me),CYN"陆高轩高声叫道："+me->name()+"你竟敢偷令牌，不想活了！\n"NOR);
		tell_room(environment(me),CYN"陆高轩一脚把"+me->name()+"踢了出去！\n"NOR);
		if (me->query("qi") > me->query_skill("force",1))
		me->receive_damage("qi",me->query_skill("force", 1));
		else me->set("qi", 1);
		me->move("/d/sld/kongdi");
	}
	return 1;       
}

int prevent_learn(object me, string skill)
{
	return 0;
}

int recognize_apprentice(object ob)
{
	mapping myfam;

	myfam = (mapping)ob->query("family");
	if(!(myfam["family_name"] != "神龙教")
	 && ob->query("shen",1) < -10000
	 && ob->query_skill("dulong-dafa",1) >= 100 
	 && ob->query_skill("huagu-mianzhang",1)>=100 ) {
		if(random(10) > 7)
		return 1;
	}
	return 0;
}

int ask_lingpai()
{
	object me,ob,zs;
	mapping myfam;
	object* inv;
	int i;

	me=this_player();
	ob=this_object();
	myfam = (mapping)me->query("family");
	if(!myfam || myfam["family_name"] != "神龙教") {
		return 0;
	}
	inv = all_inventory(me);
	for(i=0; i<sizeof(inv); i++)       
	if(inv[i]->query("id")=="ling pai")
		break;
	if(i>=sizeof(inv)) {
		zs= new(__DIR__"obj/lingpai");
		if (!zs)  return notify_fail("异常错误！\n");
		tell_object(me,"既然你要出岛，我就给你块令牌吧。\n");
		tell_room(environment(me), "陆高轩给" + me->name() + "一块令牌。\n");
		zs->move(me);      
	}
	else {
		tell_object(me,"你不是已经有令牌了么？\n");
	}
	return 1;
}

int ask_yaopu()
{
	object me=this_player();
	
	if(!me->query_temp("makeyao",1)) {
		return 0;
	}
	tell_object(me,"陆高轩对你说道：我告诉你一句口诀：先天八卦，顺时而行，辰辰不息，半半相循，乾坤为首，艮兑为尾。\n");
	return 1;
}

int ask_makeyao()
{
	object me=this_player();
	mapping myfam;

	myfam = (mapping)me->query("family");
   	if(!myfam || myfam["family_name"] != "神龙教") {
		return 0;
   	}
	if(me->query_skill("shenlong-yaoli",1)<101 || me->query_skill("poison",1)<101)
		return 0;
	tell_object(me,"陆高轩对你说道：欲炼宝丹，须先埋药种于药圃之中，待其成材后，置于药炉之中，以本教密法炼之即可得药。\n");
	me->set_temp("makeyao",1);
	return 1;
}

int ask_yaolu()
{
	object me=this_player();
	object yl;

	if(!me->query_temp("makeyao",1))
   	{
       		return 0;
   	}
	if(objectp(yl=me->query("myyaolu")))
	{
		if(random(10)>2)
			return notify_fail("不是已经给过你药炉了么？\n");
		else
			destruct(yl);
	}
	yl=new("/d/sld/npc/obj/yaolu");
	if(!yl)
		return notify_fail("现在没有药炉，你等会再来要吧。\n");
	else if(!yl->move(me))
	{
		destruct(yl);
		return notify_fail("你先把身上没用的东西处理掉，然后再来要药炉吧。\n");
	}
	else
	{
		yl->set("owner",me);
		tell_object(me,"陆高轩给了你一只药炉，道：拿这只药炉炼药去吧。\n");
		me->set("myyaolu",yl);
	}
	return 1;
}

int ask_yaozhong()
{
	object me=this_player();
	
	if(!me->query_temp("makeyao",1))
   	{
       		return 0;
   	}
	else if(me->query_temp("yaozhong",1))
	{
		return notify_fail("我不是已经跟你说过了么？你怎么又来了！\n");
	}
	tell_object(me,"陆高轩对你说道：我上次不知道把药种塞到哪里去了，等我找到了再给你。\n");
	me->set_temp("yaozhong",1);
	return 1;
}

void kill_ob(object ob)
{
	object me = this_object();
	command("say 老夫正在筹划对抗清军，没功夫陪你玩。");
	me->remove_enemy(ob);
	ob->remove_killer(me);
	return;
}

int accept_fight(object ob)
{
	command("say 老夫正在筹划对抗清军，没功夫陪你玩。");
	return 0;
}

string ask_seawar()
{
	object * team;
	object me;
	int i;
	
	me = this_player();
	team = me->query_team();
	
	if(wizardp(me) && me->query("env/test"))
		return SEAWAR_D->play(me,me,2);
	
	if( sizeof(team) != 2 )
		return "出海参战，你的队伍必须要有两个人。";
	for (i=0;i<sizeof(team);i++) {
		if ( !team[i] || !userp(team[i]) || !present(team[i],environment(me)) )
			return "你要和谁一起出战？";
		if( team[i]->query_condition("job_busy") || team[i]->query("job_name")=="神龙岛海战" || team[i]->query_condition("killer") )
			return "你们的队伍太忙碌了，休息一会儿再参战吧。";
		if( team[i]->query("combat_exp")<100000 )
			return "你们的队伍的经验太少了，还是先去锻炼一下儿再来参战吧。";
		if( team[i]->query("shen")>-30000 )
			return "你们似乎不是我道中人，我可不放心让你们参战。";
	}

	return SEAWAR_D->play(team[0],team[1],2);
}
