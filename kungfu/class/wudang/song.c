// song.c 宋远桥
// 此文件为bonus的例子文?// bonus/武当 bonus/雪山 这种保存方式保存不同Job下的bonus
inherit NPC;
inherit F_MASTER;

#include <jobround.h>
#include <job_mul.h>
#include <ansi.h>
#include <get_place.h>
string ask_me();
string ask_me1();
string ask_me2();
string ask_me3();
string ask_me4();
string ask_me5();
string ask_miner();
int check(object ob);

void create()
{
	set_name("宋远桥", ({ "song yuanqiao", "song" }));
	set("nickname", "武当首侠");
	set("long",
	"他就是张三丰的大弟子、武当七侠之首的宋远桥。\n"
	"身穿一件干干净净的灰色道袍。\n"
	"他已年过六十，身材瘦长，满脸红光。恬淡冲和，沉默寡言。\n");
	set("gender", "男性");
	set("unique", 1);
	set("age", 61);
	set("attitude", "peaceful");
	set("shen", 10000);
	set("str", 29);
	set("int", 27);
	set("con", 26);
	set("dex", 26);
	set("no_get","宋远桥对你而言太重了。");
	set("no_bark",1);
	set("job_npc", 1);

	set("max_qi", 4500);
	set("max_jing", 4000);
	set("neili", 5000);
	set("max_neili", 5000);
	set("jiali", 50);
	set("combat_exp", 3000000);
	set_skill("blade", 230);
	set_skill("cuff", 230);
	set_skill("force", 230);
	set_skill("yinyun-ziqi", 230);
	set_skill("dodge", 230);
	set_skill("wudang-quan", 230);
	set_skill("tiyunzong", 230);
	set_skill("taoism", 150);
	set_skill("strike", 280);
	set_skill("zhenshan-mianzhang", 280);
	set_skill("parry", 230);
	set_skill("literate", 300);
	set_skill("xuanxu-daofa", 240);
	set_skill("sword", 280);
	set_skill("taiji-jian", 280);
	set_skill("juehu-shou", 200);
	set_skill("hand", 250);
	map_skill("hand", "juehu-shou");
	set_skill("wudang-quan", 240);
	map_skill("force", "yinyun-ziqi");
	map_skill("force", "yinyun-ziqi");
	map_skill("dodge", "tiyunzong");
	map_skill("strike", "zhenshan-mianzhang");
	map_skill("parry", "zhenshan-mianzhang");
	prepare_skill("strike", "zhenshan-mianzhang");
	create_family("武当派", 2, "弟子");

	set("inquiry", ([
		"秘籍" : (: ask_me :),
		"概要" : (: ask_me1 :),
		"工作" : (: ask_me2 :),
		"job" : (: ask_me2 :),
		"巡山" : (: ask_me2 :),
		"fangqi" : (: ask_me3 :),
		"放弃" : (: ask_me3 :),
		"abandon" : (: ask_me3 :),
		"贡献" : (: ask_me4 :),
		"次数" : (: ask_me4 :),
		"功劳" : (: ask_me4 :),
		"gongxian" : (: ask_me4 :),
		"gonglao" : (: ask_me4 :),
		"cishu" : (: ask_me4 :),
		"地点"  : (: ask_me5 :),
		"杂役"  : (: ask_miner :),
	]));

	set("book_count1", 1);
	set("book_count2", 1);
	setup();

	carry_object("/d/wudang/obj/white-robe")->wear();

}
void init()
{
	object me, ob;
	::init();
	ob = this_player();
	me = this_object();
	if (interactive(ob = this_player()) && !is_fighting()) {
		remove_call_out("greeting");
		call_out("greeting", 0, ob);
	}
}

void greeting(object ob)
{
	int e,p,shen,count,j,gold;
	mapping fam;

	if (!ob) return;        // Yuj 19991111

	// Looklove for change job flag
	if (ob->query("wd_job") >0 ){
		ob->add("job_time/武当",ob->query("wd_job"));
		ob->delete("wd_job");
	}

	fam = ob->query("family");

	count = ob->query("job_time/武当",1);

	j = ob->query("bonus/武当");
	/*      e = 110 * j + random(50);*/
	e = 50*j + random(j*100);         //modify by campsun
	gold = 1+(j+random(j*3))/4;
	if (e <= 100) e=100+random(25);
	if( e>=500) e=400+random(20)*j;

	if(ob->query_temp("wd/wd_job_lost2")) {
		command("faint "+ob->query("id"));
		command("say "+ob->name()+"，据门派弟子来报，"+ob->query_temp("wd/wd_job_lost2_man")+"已经帮你把那恶贼给赶跑了！");
		ob->delete_temp("wd/wd_job");
		ob->delete_temp("wd/wd_job_place");
		ob->delete_temp("wd/wd_job_lost2");
		ob->delete_temp("wd/wd_job_lost2_man");
		ob->add("bonus/武当",-3);
		ob->apply_condition("job_busy", 3);
	}
	if(ob->query_temp("wd/wd_job_lost")) {
		command("sigh "+ob->query("id"));
		command("say "+ob->name()+"你怎么搞的，居然让那恶贼给跑了！");
		ob->delete_temp("wd/wd_job");
		ob->delete_temp("wd/wd_job_place");
		ob->delete_temp("wd/wd_job_lost");
		ob->add("bonus/武当",-3);
		ob->apply_condition("job_busy", 3);
	}
	if(ob->query_temp("wd/wd_jobok") ){
		command("thumb "+ob->query("id"));
		command("say 很好，祝贺你完成了任务，下去休息休息吧！");
		count += 1;
		ob->set("job_name","武当锄奸");
		e = ob->add_exp_combat(e,this_object(),"武当");
		p = 20 + random(10);
		shen = e*10 + p*10;
		ob->add("potential",p);
		ob->add("balance",gold*10000);
		if( ob->query("family") && ob->query("family/family_name")!="桃花岛")
		ob->add("shen",shen);
		ob->delete_temp("wd/wd_job");
		ob->delete_temp("wd/wd_job_place");
		ob->delete_temp("wd/wd_jobok");
		//          ob->delete("bonus/武当");	//如果job成功，不删除bonus标记。如果失败，bonus从1再开始
		if ( (int)ob->query("potential", 1) > (int)ob->query("max_pot", 1) )
		ob->set("potential" , ob->query("max_pot", 1) );
		tell_object(ob, GRN"你获得了"+CHINESE_D->chinese_number(e)+"点经验，"+CHINESE_D->chinese_number(p)+"点潜能！你的侠义正气增加了！\n"
		+"宋远桥为你在钱庄里存入"+CHINESE_D->chinese_number(gold)+"锭"+HIY+"黄金。\n"NOR);
		log_file("job/chujian",
		sprintf("%s(%s)第%d次任务，获得%d|%d|%d|%d\n",
		ob->name(),ob->query("id"),	count, e,	p, ob->query("combat_exp"),	j	),ob
		);
		ob->apply_condition("job_busy", 1);
		ob->apply_condition("wd_job", e/100);
	}
	return;
}

void kill_ob(object me)
{
	set("chat_chance_combat", 20);
	set("chat_msg_combat", ({
		(: perform_action, "strike.bingdi" :),
		(: perform_action, "dodge.zong" :),
	}));
	::kill_ob(me);
}

int accept_fight(object me)
{
	if((int)me->query("shen") < 1) {
		command("say 习武之人应志在锄恶济善，"+RANK_D->query_respect(me)+"目露瘴气，有违武学之本？");
		return 0;
	}
	::fight_ob(me);
	return 1;
}

void attempt_apprentice(object ob)
{
	if ((int)ob->query("shen") < 10000) {
		command("say 我武当乃是堂堂名门正派，对弟子要求极严。");
		command("say 在德行方面，" + RANK_D->query_respect(ob) +
		"是否还做得不够？");
		return;
	}
	command("say 好吧，贫道就收下你了。");
	command("recruit " + ob->query("id"));
}

int jvm(int i) {
	int m,n;
	
	for(m=0;m<i;m++) {n+= (m+1)*1000;}
	return n;
}

string ask_miner(){
	mixed ret;
	object me=this_player();
  int i =  me->query("job_time/武当") - jvm(me->query("BBS_Miner_gift")+1);	
	if ( i>0 ) {
	  ret = dbquery(sprintf("select miner,house from ultrax.addon_kuang_member WHERE uid=%d ;",me->query("BBS_UID")));
	  if(!sizeof(ret)) return RANK_D->query_respect(me)+"尚无矿山产业，不知此话从何谈起？";
	  if(ret[0][0]+1 > ret[0][1]*10) return RANK_D->query_respect(me)+"的矿场好象需要扩建了。";
	  if(dbquery(sprintf("update ultrax.addon_kuang_member set miner=miner+1 WHERE uid=%d ;",me->query("BBS_UID")))){
	    me->add("BBS_Miner_gift",1);
	    write(HIY"宋远桥增派了一名杂役弟子到你的社区矿场当矿工！\n");
	    return RANK_D->query_respect(me)+"为武林正道如此劳力劳心，我武当自应为君分忧。";
	  }
	  else return "近日我武当也没有多余的劳力，请"+RANK_D->query_respect(me)+"改日再来。";
  }	
  else return RANK_D->query_respect(me)+"还需为武当效力"+chinese_number(-i)+"次，我才可向掌门报请新的杂役弟子。";
}

string ask_me(){
	mapping fam;
	object ob;
	if (!(fam = this_player()->query("family")) || fam["family_name"] != "武当派")
	return RANK_D->query_respect(this_player()) +
	"与本派素无来往，不知此话从何谈起？";
	if(query("book_count2") < 1)
	return "你来晚了，本派的内功心法不在此处。";
	add("book_count2",-1);
	ob = new(BOOK_D("force_book"));
	ob->move(this_player());
	return "好吧，这本「太极十三式」你拿回去好好钻研。";
}

string ask_me1()
{
	mapping fam;
	object ob,song;
	song = this_object();
	if (!(fam = this_player()->query("family")) || fam["family_name"] != "武当派")
	return RANK_D->query_respect(this_player()) +
	"与本派素无来往，不知此话从何谈起？";
	if (query("book_count1") < 1)
	return "你来晚了，刀法概要已经不在此处了。";
	add("book_count1",-1);
	ob = new(BOOK_D("blade_book"));
	ob->move(this_player());
	return "这本书中讲述的刀法要领，你会去好好研究一下。";
}

string ask_me2()
{
	object me, ob, *living, target;
	string str,str2;
	string str_temp;
	object room, oroom, lastroom;
	int i, j, k, a, b,t;
	string * exits,*dirs;
	string far;

	me=this_player();
	str2 = me->query_temp("wd/wd_job_place");
	str_temp = "";

	if (me->query_temp("wd/wd_job") && !me->query_temp("wd/wd_job_place"))
	return "啊……，对不起，地点有误，你放弃重新再来吧。";

	if (me->query("combat_exp") < 100000 )
	return "这位" + RANK_D->query_respect(me) + "武功未成，还是去做别的工作吧！";

	if(me->query_temp("quest/busy")) //added by tangfeng 与quest冲突
	return "现在我这里没有给你的任务，你还是先处理好你其他事情再说吧。";

	// add for test job
	if(!wizardp(me)){
		if (me->query("menggu"))
		return "哼，你已身属蒙古，还想做什么？";

		if ( me->query_condition("killer"))
		return "你现在是官府捉拿的要犯，还想帮我武当锄奸？";

		if (me->query_temp("wd/wd_job"))
		return "我不是告诉你了吗，有人在捣乱。你就快去吧！";

		if (me->query("job_name") =="武当锄奸")
		return "你刚做完武当锄奸任务，还是先去休息一会吧。";

		if (me->query("shen") < 10000 && !wizardp(me) )
		return RANK_D->query_respect(me) + "的正气还不够，我无法放心让你做这个任务！";

		if ( me->query_condition("job_busy")){
			if (me->query("job_name")=="武当锄奸")
			return "现在暂时还没有工作让你去做。";
			if( me->query("job_name"))
			return "你刚做完"+me->query("job_name")+"任务，还是先休息一会吧。";
			return "你正忙着别的事情呢。";
		}

		if(!wizardp(me) && me->query_condition("task_busy"))
		return "现在暂时没有适合你的工作。";

		if (me->query_condition("wd_job"))
		return "现在暂时没有适合你的工作。";

		// end test if
	}

	ob=new(__DIR__"robber");
	ob->move(environment(this_object()));

	living = livings();
	for (i=0;i<sizeof(living);i++){
		j = random(sizeof(living));
		if (j <= 1 || j == sizeof(living)-1) continue;
		if( check(living[j]) ){
			target = living[j];
			room = environment(target);
			if( !mapp(room->query("exits")) || sizeof(room->query("exits"))<1 ) continue;

			if( get_place(base_name(room))!="" ) {
				oroom = room;
				if ( random(10)<3 && target->query("race")=="人类" ) {
					j = 0;
					break;
				}

				lastroom = room;
				j = 1+random(4);//改得好找些:)
				for(k=0;k<j;k++) {
					if( !mapp(room->query("exits")) || sizeof(room->query("exits"))<1 ) continue;
					exits = values(room->query("exits"));
					dirs = keys(room->query("exits"));
					b = sizeof(exits);
					t =0;
					while( (a=random(b))>=0 && b>1 && t<10 ) {
						t++;
						if(!room->valid_leave(ob, dirs[a])) continue;
						if (b==2)
						if ((exits[0]==base_name(lastroom) && exits[1]==base_name(oroom))
						|| (exits[1]==base_name(lastroom) && exits[0]==base_name(oroom)))
						break;
						if (exits[a]==base_name(lastroom) || exits[a]==base_name(oroom)) continue;
						break;
					}
					//write(sprintf("%d\n",t));
					lastroom = room;
					if( !(room = find_object(exits[a])) )
					if( !(room = load_object(exits[a])) )
					break;
				}
				if(k<j) continue;
				break;
			}
		}
	}

	if( !target || !room )
	return "暂时没有任务需要做，你过一会儿再来吧。";

	me->set_temp("wd/wd_job",1);
	me->set_temp("wd/wd_job_place", get_place(base_name(oroom))+oroom->query("short"));
	me->apply_condition("wd_job",40);
	me->apply_condition("job_busy",40);
	me->apply_condition("task_busy", 8);
	command("nod "+me->query("id"));

	if (me->query("bonus/武当") >= WD_JOB_ROUND || !me->query("bonus/武当") || me->query("bonus/武当")<1 )
	{
		if (me->query("bonus/武当") == WD_JOB_ROUND)
		me->set("bonus/武当",4);
		else
		me->set("bonus/武当",1);
	}
	else
	me->add("bonus/武当",1);

	if(me->query("bonus/武当")>5 && me->query("combat_exp")<2000000)
	me->set("bonus/武当",3);

	ob->set_temp("target", me->query("id") );
	ob->setparty(me->query("bonus/武当"),me->query("max_pot")-100,me->query("combat_exp"));
	ob->move(room);

	log_file("job/wd_job",sprintf("%s(%s)开始做wd job,目前实战经验：%d\n", me->name(),me->query("id"),me->query("combat_exp")),me);
	str = strip(me->query_temp("wd/wd_job_place"));

	if(wizardp(me) && me->query("env/test"))
	tell_object(me,"目标Npc从 "+base_name(oroom)+" 移到-> "+base_name(room)+"    "+room->query("short")+"\n");

	if (j)
	far = HIY+"周围方圆"+CHINESE_D->chinese_number(j)+"里之内"+GRN;
	else
	far = "";

	tell_object(me, GRN"宋远桥在你的耳边悄声说道：据说"+ob->query("title")+GRN+ob->query("name")+"正在"+ me->query_temp("wd/wd_job_place") +GRN + far + "捣乱，你马上去给我巡视一圈。\n");

	if (ob->query("family/family_name")=="武当派")
	str_temp+=GRN"宋远桥在你的耳边悄声说道："+RANK_D->query_self(this_object())+"已追查到此人是我武当出身，尤为擅长"HIC+to_chinese(ob->query("f_w_skill"))+GRN"的功夫。\n"+
	RANK_D->query_respect(me)+"若能将其诛杀，不但为江湖除去一害，也能为"+RANK_D->query_self(this_object())+"讨个公道，自当感激不尽。\n";
	else {
		str_temp+=GRN"宋远桥在你的耳边悄声说道：据门派弟子来报，此人是来自"HIY+ob->query("family/family_name")+GRN"的";
		if (random(2)==0)
		str_temp+="叛徒，";
		else
		str_temp+="高手，";
		str_temp+= "尤为擅长"HIC+to_chinese(ob->query("f_w_skill"))+GRN"的功夫。\n";
	}

	switch ((int)me->query("bonus/武当"))
	{
		case 6..7:
		str_temp+=GRN"宋远桥在你的耳边悄声说道：此人的武功" + HIC + "颇为了得" + GRN + "，你可得小心对付哦。\n"NOR;
		break;
		case 8..9:
		str_temp+=GRN"宋远桥在你的耳边悄声说道：此人的武功" + HIC + "极其厉害" + GRN + "，你可得小心应付。\n"NOR;
		break;
		case 10:
		str_temp+=GRN"宋远桥在你的耳边悄声说道：此人的武功" + HIW + "已入化境" + GRN + "，打不过可不要逞能。\n"NOR;

	}

	tell_object(me, str_temp);

	tell_room( environment(this_object()), this_object()->name() + "在" + me->name() + "耳边小声地说了些话。\n", ({ me, this_object() }) );

	return "你快去快回，一切小心啊。";
}

string ask_me3()
{
	object ob = this_player();

	if(ob->query_temp("wd/wd_job")){
		ob->delete_temp("wd/wd_job");
		ob->delete_temp("wd/wd_job_place");
		ob->clear_condition("task_busy");

		if (ob->query("bonus/武当")<=5)
		{
			ob->apply_condition("job_busy", 4);
			ob->set("bonus/武当",0);
			return ob->name()+"，你太让我失望了，居然这么点活都干不好，先退下吧！";
		}
		else if (ob->query("bonus/武当")>5)
		{
			ob->apply_condition("job_busy", 2);
			ob->add("bonus/武当",-3);
			return ob->name()+"，这个任务确实比较难完成，下次给你简单的，先退下吧！";
		}
	}
	return ob->name()+"，你又没在我这里领任务，瞎放弃什么呀！";
}

string ask_me4()
{
	object ob;
	ob = this_player();
	if(!ob->query("job_time/武当")){
		return "对不起，"+ob->name()+"，你好象还没有在我这里领过任务呢，加油干吧！";
	}
	return ""+ob->name()+"，你已经成功地铲除了"+CHINESE_D->chinese_number((int)ob->query("job_time/武当"))+"名恶贼！";
}
string ask_me5()
{
	object me;
	string str;
	me = this_player();
	str = strip(me->query_temp("wd/wd_job_place"));


	if (!me->query_temp("wd/wd_job"))
	return "你都没有领任务，我怎么知道你要杀的恶贼跑哪里去了呢？";
	if (strwidth(str)<=12) {
		tell_object(me, GRN"宋远桥在你的耳边悄声说道：你要杀的恶贼正在\n"
		+ CHINESE_D->font(str)
		+ GRN + "附近捣乱呢，你还不马上去巡视一圈。\n");

	} else
	tell_object(me, GRN"宋远桥在你的耳边悄声说道：你要杀的恶贼正在"+ me->query_temp("wd/wd_job_place") + GRN +"附近捣乱呢，你还不马上去巡视一圈。\n");
	return "一路小心啊！";


}
void destroying(object obj)
{
	if (!obj) return;
	if (userp(obj)) command("drop "+obj->parse_command_id_list()[0]);
	else destruct(obj);
}

string *no_kill_list = ({
	"mu ren", "jin ren", "yin ren", "zhong shentong", "zhu cong", "shi ren", "huo du","he shiwo"
});

int check(object ob)
{
	object room;
	string room_name;

	if(!objectp(ob)
	|| !ob->query("startroom")      // add by Yuj@SJ to void job NPC
	|| ob->query("env/invisibility")
	|| ob->query("no_quest")
	|| member_array(ob->query("id"), no_kill_list) != -1
	|| userp(ob)
	|| ob->query("attitude")=="aggressive"
	|| ob->query("have_master")
	|| !objectp(room=environment(ob))
	|| room->query("no_fight")
	|| room->query("quest")
	|| room->query("outdoors") == "昆仑翠谷"
	|| room->query("short") == "地下黑拳市"
	|| room->query("short") == "武僧堂"
	|| strsrch(room_name = file_name(room),"/d/") != 0                    //modified by campsun 2004.2.4 为了和bx code兼?	|| strsrch(room_name, "/d/wizard/") == 0
	|| strsrch(room_name, "/d/wuguan/") == 0
	|| strsrch(room_name, "/d/wudang/houshan") == 0
	|| strsrch(room_name, "/d/dali/yideng") == 0
	|| strsrch(room_name, "/d/death/") == 0
	|| strsrch(room_name, "/d/mingjiao/lsd/") == 0
	|| strsrch(room_name, "/d/mingjiao/bhd/") == 0
	|| strsrch(room_name, "/d/shaolin/fumoquan") == 0
	|| strsrch(room_name, "/d/baituo/") == 0)
	return 0;
	return 1;
}
