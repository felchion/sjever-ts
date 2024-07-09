// /d/xingxiu/npc/ding.c
// ding.c 丁春秋
// Update by caiji@SJ 8/8/2000

#include <ansi.h>
#include <get_place.h>

inherit NPC;
inherit F_MASTER;

string *drugs = ({"/obj/yao","/obj/yao1","/obj/yao","/obj/yao2","/obj/yao","/obj/yao3","/obj/yao",});
int give_reward(object me);
int give_drug();
string ask_job5();
string ask_job4();
string ask_job5_times();
string ask_fail();
int check(object ob);
string ask_job3();
string ask_job();
string ask_times();
string ask_san();
string ask_gou();
string ask_pay();

void create()
{
	set_name("丁春秋", ({ "ding chunqiu", "ding" }));
	set("nickname", "星宿老怪");
	set("long",
	"星宿派开山祖师、令中原武林人士深恶痛绝的星宿老怪丁春秋。\n"
	"他脸色红润，满头白了，颏下三银髯，童颜鹤发。\n"
	"当真便如图画中的神仙人物一般。\n");
	set("gender", "男性");
	set("age", 60);
	set("per", 24);
	set("attitude", "friendly");
	set("no_bark",1);
	set("shen_type", -1);
	set("str", 30);
	set("int", 27);
	set("con", 28);
	set("dex", 32);
	set("max_qi", 6000);
	set("max_jing", 5000);
	set("eff_jingli", 3500);
	set("max_neili", 11000);
	set("jiali", 120);
	set("combat_exp", 15000000);
	set("unique", 1);
	set("shen", -200000);

	set_skill("force", 500);
	set_skill("huagong-dafa", 510);
	set_skill("dodge", 500);
	set_skill("zhaixingshu", 490);
	set_skill("strike", 500);
	set_skill("chousui-zhang", 490);
	set_skill("parry", 500);
	set_skill("claw", 500);
	set_skill("sanyin-zhua", 510);
	set_skill("staff", 500);
	set_skill("tianshan-zhang", 500);
	set_skill("literate", 500);
	set_skill("poison", 450);
	map_skill("force", "huagong-dafa");
	map_skill("dodge", "zhaixingshu");
	map_skill("strike", "chousui-zhang");
	map_skill("parry", "chousui-zhang");
	map_skill("claw", "sanyin-zhua");
	map_skill("staff", "tianshan-zhang");
	prepare_skill("strike", "chousui-zhang");
	prepare_skill("claw", "sanyin-zhua");
	create_family("星宿派", 1, "开山祖师");
	set("inquiry", ([
	       "星宿派" : "我老仙乃是星宿派开山祖师, 还不赶快磕头。",
	       "星宿海" : "你瞎了眼啦, 这儿就是。",
		 "星宿" : "这儿方圆几百里都是老仙的练功房。",
	       "丁春秋" : "没大没小的。再叫我废了你。",
		 "老仙" : "我星宿老仙比起古往今来的圣人怎么样啊？",
	     "星宿老仙" : "以后叫老仙。",
		 "吹捧" : "本来就是麽。",
		 "毒药" : (: give_drug :),
		 "心情" : (: ask_job :),
	       "三笑散" : (: ask_san :),
	   "三笑逍遥散" : (: ask_san :),
		"pantu" : (: ask_job5 :),
	       "毒虫谷" : (: ask_job4 :),
	    "duchonggu" : (: ask_job4 :),
		  "叛徒": (: ask_job5 :),
		  "放弃": (: ask_fail :),
		  "fail": (: ask_fail :),
	  "杀叛徒的次数": (: ask_job5_times :),
	    "杀叛徒次数": (: ask_job5_times :),
	    "job5_times": (: ask_job5_times :),
	     "神木王鼎" : "你敢偷，就废了你。",
		 "木鼎" : "抓虫子练毒用的。",
	]));

	set("chat_chance_combat", 20);
	set("chat_msg_combat", ({
		(: exert_function, "huagong" :),
		(: perform_action, "strike.huoqiu" :),
		(: perform_action, "strike.yinhuo" :),
		(: perform_action, "strike.biyan" :),
	}));

	set("drug_count",15);
	set("class", "taoist");

	set_temp("apply/damage", 20);
	set_temp("apply/defense", 70);
	set_temp("apply/armor", 60);
	setup();

	if (clonep()) {
		UPDATE_D->get_cloth(this_object());
		add_money("gold", 2);
	}
}

int prevent_learn(object ob, string skill)
{
	mapping myfam;
	int shen;
	
	myfam = (mapping)ob->query("family");
	if(!myfam || (myfam["family_name"] != "星宿派")) return 1;
	if(!ob->is_apprentice_of(this_object())) return 1;
	shen = ob->query("shen");
	if(shen > -1000 && shen < 0)
	{
		command("say 你怎么搞的？搅和了一身的白道气息！");
		return 1;
	}
	else if(shen >= 0 && shen < 100000)
	{
		command("say 好哇！你敢和白道的人搅和，不想活了是不是？");
		return 1;
	}
	else if(shen >= (ob->query("combatd_exp")/5)){
		command("say 你竟敢和白道的人搅和！从我这里滚出去吧！");
		command("expell " + ob->query("id"));
		return 1;
	}
	return 0;
}

void init()
{
	object me, ob;
	::init();
	ob = this_player();
	me = this_object();
	me->delete_skill("zhuihun-gou");
		
	if(ob->query("job_time/星宿熬药"))
	{
		ob->add("job_time/星宿熬膏",ob->query("job_time/星宿熬药"));
		ob->delete("job_time/星宿熬药");
	}
		
	if (interactive(ob = this_player()) && visible(ob) && !is_fighting()) {
		remove_call_out("greeting");
		call_out("greeting", 1, ob);
	}
	add_action("do_flatter", "flatter");
	add_action("do_xian", "xian");
	add_action("do_begsan", "begsan");
}

string ask_san()
{

	if((int)this_player()->query("combat_exp") < 200000
	 || (string)this_player()->query("family/family_name") != "星宿派")
		  return "就凭你也想要三笑散？\n";

	if( query("started") != 1 ){
	   set("started", 1);
	   call_out("generate_sxs", 7200);//2 hours
	}

	if((int)this_player()->query_skill("poison", 1) < 200)
		  return "你的毒技还不够高，我不放心把三笑散交给你。";

	if((int)this_player()->query_skill("huagong-dafa", 1) < 240)
		  return "你的化功大法还不够高，我不放心把三笑散交给你。";

	message_vision("丁春秋对$N笑道：你用begsan试试缘分如何。\n", this_player());
	return "快试吧";
}

void generate_sxs()
{
	remove_call_out("generate_sxs");
	set("available", 1);
}

int do_begsan(string arg)
{
	object me, book;
	me = this_player();

	if( query("available") == 1 && clonep(book = unew(MEDICINE_D("sxs")))){
	  set("available", 0);
	  book -> move(me);
	  message_vision("$N咱们真是有缘，这一包三笑散就送给你了。\n", me);
	  command("chat "+ me->query("name")+"有缘，得到我的宝物“三笑逍遥散”！");
	  remove_call_out("generate_sxs");
	  call_out("generate_sxs", 7200);//2 hours
	  return 1;
	}
	return notify_fail("丁春秋说道：你来的不是时候。\n");
}

void greeting(object me)
{
	mapping myfam;
	int shen;

	if(!me || environment(me) != environment(this_object())) return;

	shen = me->query("shen");
        if ( file_name(environment(this_object())) != this_object()->query("startroom")) return;

	if(!myfam = (mapping)me->query("family"))
		command("say 你千里迢迢来天山, 莫不是想加入我星宿派？");
	else if(myfam["family_name"] != "星宿派" ){
		if(!me->query_temp("xx_job"))
			command("say 你千里迢迢来到天山，是仰慕本老仙我的仁德吧？");
		else    command("nod " + me->query("id"));
	}

	else if(shen > -1000 && shen < 0)
		command("say 你怎么搞的？搅和了一身的白道气息！");
	else if(shen >= 0 )//&& shen < 100000
		command("say 好哇！你敢和白道的人搅和，不想活了是不是？");
	/*else if(shen > 100000){
		command("say 你竟敢和白道的人搅和！从我这里滚出去吧！");
		command("expell " + me->query("id"));
	}*/   // I move this to prevent_learn By Ciwei@SJ
	else command("say 好徒儿，功夫练得怎么样了？");
	if(me->query("xx_job5_finished"))call_out("give_reward",1,me);
}

void attempt_apprentice(object me)
{
	if(me->query("family/family_name") != "星宿派"
	 && me->query("expell/family_name") != "星宿派"){
		say("老仙对"+me->name()+"理都不理。\n");
		return;
	}
	if ((int)me->query("shen") > -50000) {
		command("say 老仙越看你越象白道派来卧底的。");
		return;
	}

	if(me->query_skill("huagong-dafa",1) < 150){
		command("say 看来你的化功大法基础还不够啊，先去跟我徒儿学学吧。");
		return;
	}
	if( me->query_temp("pending/flatter") ) {
		command("say 你如此不把老仙放在眼里，老仙怎会收你？");
		return;
	}
	else {
		command("say 我星宿老仙比起古往今来的圣人怎么样啊？");
		message_vision("星宿老怪微闭双眼，手捻长须，一付等$N拍马(flatter)的样子。\n", me);
		me->set_temp("pending/flatter", 1);
	}

}

int do_flatter(string arg)
{
	object me, ob;
	me = this_player();
	ob = this_object();

	if( !arg ) return notify_fail("你要赞扬老仙的什么丰功伟绩？\n");
	if(me->query_condition("gb_mission") || me->query_condition("gb_busy"))
		return notify_fail("老仙最讨厌臭叫化子，先扔掉你的丐帮帮务再说！\n");
	if(this_player()->query("jing") < 10)
		return notify_fail("你太累了，等等再拍吧！\n");

	switch(random(5)) {
		case 0 : message_vision("\n$N对$n大声赞道：您老人家一蹬足天崩地裂，一摇手日月无光！\n", me,ob); break;
		case 1 : message_vision("\n$N对$n大声赞道：星宿老仙大袖摆动，口吐真言，叫旁门左道牛鬼蛇神，一个个死无葬身之地！\n", me,ob); break;
		case 2 : message_vision("\n$N对$n大声赞道：这天下武林盟主一席，非老仙莫属。只须老仙下令动手，小人赴汤蹈火，万死不辞！\n", me,ob); break;
		case 3 : message_vision("\n$N对$n大声赞道：日月无老仙之明，天地无老仙之大，自盘古氏开天辟地以来，更无第二人能有老仙的威德！\n", me,ob); break;
		case 4 : message_vision("\n$N对$n大声赞道：老仙神功盖世，天下第一，战无不胜，功无不克！\n", me,ob); break;
	}

	message_vision(HIY"$N大声唱道：" + arg + "\n"NOR, this_player());
	this_player()->set("jing", 1);
	if( strsrch(arg, "星宿老仙") >=0 && (strsrch(arg, "德配天地") >=0 
	 && strsrch(arg, "威震寰宇") >=0 && strsrch(arg, "古今无比") >=0 )) {
		command("smile");
		command("say 这还差不多。\n");
		if(this_player()->query_temp("pending/flatter")){
			this_player()->delete_temp("pending/flatter");
			command("recruit " + this_player()->query("id"));
			CHANNEL_D->do_channel(this_player(), "chat",  "星宿老仙，德配天地，威震寰宇，古今无比！");
			return 1;
		}
		else{
			command("say 你对老仙我这么恭敬，今后前途不可限量啊！\n");
			this_player()->set_temp("ding_flatter", 1);
			return 1;
		}
	}
	else command("say 没有了吗？");
	return 1;
}

int give_drug()
{
	object drug, me, ob;

	me = this_object();
	ob = this_player();

	if(ob->query("family/family_name") == "星宿派" && (ob->query_skill("huagong-dafa", 1) > 79)){
		if(me->query("drug_count") >= 1 && !ob->query_temp("drug_give")){
			add("drug_count", -1);
			drug = new("/d/xingxiu" + drugs[random(sizeof(drugs))] );
			drug->move(ob);
			ob->set_temp("drug_give", 1);
			message_vision("$N对$n点了点头，交给$n一小包药粉。\n", me, ob);
			return 1;
		}
		if(ob->query_temp("drug_give"))
			command("say 我不是给过你了吗？");
		else command("say 你来晚啦，都被你师兄们拿走了。");
		return 1;
	}
	else return 0;
}

string ask_job()
{
	object me, ob;
	me = this_player();

	if (me->query("family/family_name") == "武当派"
	 || me->query("family/family_name") == "天龙寺"
	 || me->query("family/family_name") == "娥眉派"
	 || me->query("family/family_name") == "华山派"
	 || me->query("family/family_name") == "少林派")
		return "你是白道来卧底的吧，还不快给我滚出去！！！";
	if (me->query("combat_exp") < 1000000)
		return "哈哈哈，你再加把力练功，很快就可以为老仙我分忧了。";
	if (me->query("job_name") == "老仙分忧")
		return "老仙现在心情还好，不用你来为我分忧。";
	if (me->query("combat_exp") > 4000000)
		return "老仙我最近对什么都没兴趣，不用你来为我分忧。";
	if (me->query("shen") >= 0 || me->query("shen") > -(me->query("combat_exp")/10))
		return "徒儿，看来你的心肠还不够黑啊，有些事你不问也罢。";
	if (interactive(me) && me->query_temp("job_busy"))
		return "你说要替老仙分忧，怎么还在这里发呆？";
	if (interactive(me) && me->query_condition("wait_xx_task"))
		return "老仙现在心情还好，不用你来为我分忧。";
	if (me->query_condition("job_busy"))
		return "老仙现在心情还好，不用你来为我分忧。";

	ob = new("/d/xingxiu/obj/di");
	ob->move(me);
	ob->set("name", BLU"蓝玉短笛"NOR);
	ob->set("long", BLU"这玉笛短得出奇，只不来七寸来长、通体碧蓝，晶莹可爱。\n" NOR);
	ob->set("xx_user", getuid(me));
	me->set_temp("xx_job", 1);
	message_vision("$N拿出一只玉制短笛，交给$n。\n", this_object(), me);
	return "老仙我最近心情不佳，徒儿你要多多为我分忧才是！\n";
}

void destroying(object obj, object me)
{
	message_vision("$N拿起$n仔细地端详了一会儿，满意地点了点头，脸上露出了笑容。\n", me, obj);
	destruct(obj);
}

void dest(object ob)
{
	if (!ob) return;
	if (userp(ob)) command("drop "+ob->parse_command_id_list()[0]);
	else destruct(ob);
}

int do_xian(string arg)
{
	object who, me, ob;
	mapping quest;
	who = this_player();
	me = this_object();

	if(who->is_busy() || who->is_fighting())
		return notify_fail("你正忙着呢。\n");
	if(me->is_busy() || me->is_fighting())
		return notify_fail("没看见老仙正忙着吗？\n");
	if(!arg || !objectp(ob = present(arg, who)))
		return notify_fail("你要献给老仙什么东西？\n");
	if(!who->query_temp("xx_job"))
		return notify_fail("连老仙现在的心情还没摸清楚就乱献东西？\n");

	message_vision(HIY"\n$n毕恭毕敬地将"+ob->query("name")+HIY"拿出，双手捧上，想要献给$N。\n\n"NOR, me, who);

	if(me->is_busy()) {
		command("say 我正忙着呢，你等等。");
		return notify_fail("老仙让你等等。\n");
	}
	if(!mapp(quest = who->query_temp("quest")) || !quest["id"]){
		command("say 你拿个"+ob->name()+CYN"给我干嘛？"NOR);
		return notify_fail("老仙对这东西不感兴趣。\n");
	}
	if ( quest["time"] < time()*60){
		command("say 嗯，这么久才办完，你干什么去了？给我滚一边去！");
		who->apply_condition("wait_xx_task", 40);
		return notify_fail("完了，老仙生气了。\n");
	}
	if(userp(ob)){
		command("stare " + who->query("id"));
		command("say 竟敢用假货来欺骗本老仙，给我滚出去！");
		message_vision("\n说完$N长袖一挥，一股内劲拂出，就将$n扫出洞外！\n\n", me, who);
		who->move("/d/xingxiu/ryd1");
		message("vision","只听“呼”地一声，紧接着"+who->query("name")+"从日月洞里直飞了出来，摔了个四脚朝天！\n", environment(who), who);
		who->receive_wound("jing", 200);
		who->receive_wound("qi", 250);
		who->receive_damage("neili", 600);
		who->receive_damage("jingli", 500);
		who->apply_condition("wait_xx_task", 30);
		who->delete_temp("quest");
		who->set_temp("last_id", quest["last_id"]);
		who->start_busy(5);
		return notify_fail("糟糕，穿邦了！\n");
	}
	if(ob->query("name") != quest["name"] || ob->query("id") != quest["id"]) {
		command("hmm " + who->query("id"));
		command("say 你拿个"+ob->query("name")+"给老仙我有什么用？");
		return notify_fail("好象是你献错东西了。\n");
	}

	ob->move(me);
	me->start_busy(random(2));
	who->start_busy(2);
	destroying(ob, me);
	call_out("done", 3, who);
	return 1;
}

void done(object me)
{
	if(!me || environment(me) != environment(this_object())) return;

	command("pat " + me->query("id"));
	command("say 不错不错，既然你找到了老仙我想要的东西，我就指点指点你吧。");
	if(me->query("family/family_name") == "星宿派") {
		if(me->query_skill("huagong-dafa",1) < 180)
			me->improve_skill("huagong-dafa", me->query("job_time/星宿"));
		if(me->query_skill("poison",1) < 180)
			me->improve_skill("poison", me->query("job_time/星宿"));		
		tell_object(me, "你目前已经为老仙分担了"+chinese_number(me->query("job_time/星宿"))+"次忧愁。\n");
	}
	tell_object(me, "你听了老仙的指点，功夫进步了不少！\n");
	
	TASK_D->give_reward(me, me->query_temp("quest"), -1, ABS(me->query("job_time/星宿")/25),this_object(),"星宿");
	//GIFT_D->check_count(me,this_object(),"星宿");
	//me->add("job_time/星宿", 1);
			
	log_file("job/fenyou",sprintf("%-18s 做星宿JOB，得经验 %d，潜能 %d。\n",
		me->query("name")+"("+getuid(me)+")",
		me->query_temp("quest/exp_reward"), me->query_temp("quest/pot_reward")),me);
        me->set("job_name","老仙分忧");
	me->clear_condition("job_busy");
}

string *exclude_place = ({
	"/d/wuguan/","/d/death/","/d/wizard/","/d/sld/",
});

int check(object ob)
{
	object room;
	string room_name;

	if(!objectp(ob)
	 || !ob->query("startroom")      // add by Yuj@SJ to void job NPC
	 || ob->query("env/invisibility")
	 || ob->query("race") !="人类"
	 || userp(ob)
	 || ob->query("have_master")
	 || !objectp(room=environment(ob))
	 || room->query("no_fight")
	 || room->query("quest")
         || room->query("outdoors") == "昆仑翠谷"
	 || member_array(file_name(room), exclude_place) != -1
	 || strsrch(room_name = file_name(room),"/d/") < 0
         || strsrch(room_name, "/d/wizard/") == 0
         || strsrch(room_name, "/d/wuguan/") == 0
         || strsrch(room_name, "/d/death/") == 0
	 || strsrch(room_name, "/d/group") == 0
         || strsrch(room_name, "/d/mingjiao/lsd/") == 0
         || strsrch(room_name, "/d/mingjiao/bhd/") == 0
         || strsrch(room_name, "/d/shaolin/fumoquan") == 0
         || strsrch(room_name, "/d/xingxiu/") == 0
         || strsrch(room_name, "/d/baituo/") == 0
         || strsrch(room_name, "/d/meizhuang/") == 0
	 || present("pan tu", room))
	return 0;
	return 1;
}

string ask_job5()
{
	mapping fam;
	object pp,where,target, *living ,me , ob ,partner,*list;
	int i,j,busy_time,shen;
	string str,menpai;

	if (!me) me= this_player();
	if (!me) return 0;

	shen=me->query("shen");

	if(shen > -10000 ) return "我们星宿派不需要你来帮忙。";

	if( me->query("combat_exp") < 100000 )
		return "你的实战经验太少了点吧，派你去我可实在不大放心！\n";

	if((!pp=me->query("xx/partner")) && me->query("xx/pantu"))
	    return "你和你搭档完成上一次任务后，干吗去了！\n"+
		   "你只能放弃（ask ding about 放弃）当前这次任务了，不过不会受到经验的惩罚！";

	if( me->query("xx/pantu") && ((time()-(int)me->query("xx/pantu_time"))<(12*60)) )
		return "我不是叫你和"+pp->query("name")+"("+capitalize(pp->query("id"))+")到"+me->query("xx/pantu_place")+"去阻击那个叛徒了嘛！";

	if(me->query_condition("gb_mission") || me->query_condition("gb_busy"))
		return "老仙最讨厌臭叫化子，先扔掉你的丐帮帮务再说！";

	if( !wizardp(me) && me->query_condition("job_busy") ) {
		command("pat "+(string)me->query("id"));
		    return "你刚完成一次任务，先去休息一会儿吧！";
	}

	if( me->query("job_name") == "星宿杀叛徒")
	       return "你刚完成一次任务，先去休息一会儿吧！";

	me->delete("xx/pantu_busy");

	living = livings();
	for (i=0;i < sizeof(living);i++){
		j = random(sizeof(living));
		if (j <= 1 || j == sizeof(living)-1) continue;
		if(check(living[j]) ){
			target = living[j];
			str = environment(target)->query("short");
			if (str == "兵营"
			 || str == HIW"长廊"NOR
			 || str == "武僧堂"
			 || str == "松树林"
			 || str == "杂货铺"
			 || str == "密道"
			 || str == HIR"心禅堂"NOR
			) continue;
			if( get_place(base_name(environment(target)))!="" )
				break;
		}
	}
	if (me->query_temp("xx_count") > 50) {
		me->delete_temp("xx_count");
		return "没有合适的搭档，你完成不了这个任务。";
	}

	if( !target )
		return "哪里有那么多任务让你一个人去完成，你过一会再来找我吧。";

	list = filter_array(users(), (: $1->query("combat_exp")>49999 :));
	partner = list[random(sizeof(list))];

	where = environment(partner);

	if ((!environment(partner))
	 || environment(partner)->query("short")=="桃花源"
	 || wizardp(partner)
	 || environment(partner)->query("short")=="监狱"
	 || environment(partner)->query("short")=="黑屋"
	 || environment(partner)->query("short")=="机器人法庭"
	 || ((string)partner->query("family/family_name") == "古墓派" && partner->query_skill("yunu-xinjing", 1) < 101)
	 || (!interactive(partner))
	 || environment(partner)->query("no_fight")
	 || partner->query_temp("have_partner")
	 || partner->query("combat_exp") < me->query("combat_exp")/ 5
	 || partner == me
	 || partner->query_temp("user_type") == "worker" ){
		me->add_temp("xx_count",1);
		call_out("ask_job5", 0, me);
		return "";
	}
	else {
		if(!fam = (mapping)partner->query("family")) menpai="普通百姓";
		else menpai=fam["family_name"];

		me->set("xx/pantu",1);
		me->set("xx/partner",partner);

		partner->delete("env/block");
		partner->set_temp("have_partner",1);

		busy_time = 40;
		me->set("xx/pantu_time",time());
		me->apply_condition("job_busy",busy_time);
		me->set("xx/pantu_place",get_place(base_name(environment(target)))+environment(target)->query("short"));
		if(wizardp(me)) tell_object(me,base_name(environment(target)));

		ob = new(__DIR__"pantu3");
		ob->set("xx_target", me);
		ob->set("xx_partner",partner);
		ob->set("xx_place", environment(target));

		if(random(20) != 1) {
			ob->do_copy(me,0);
			me->set("xx/pantu_super",0);
		}
		else {
			ob->do_copy(me,1);
			me->set("xx/pantu_super",1);
		}
		ob->move("/d/xingxiu/caiji_room");
		me->set("xx/pantu_obj",ob);
		command("hmm "+me->query("id"));

		log_file("job/pantu",sprintf("%8s%-10s第%5d次任务开始，总计e：%d，p：%d；exp：%d\n",
			me->name(),"("+me->query("id")+")",me->query("job_time/星宿叛徒")+1,me->query("xx_jobe"),me->query("xx_jobp"),
			me->query("combat_exp")),me);
		log_file("job/pantu",sprintf("%8s%-10s第%d次任务搭档，总计e：%d，p：%d；exp：%d\n",
			partner->name(),"("+partner->query("id")+")",me->query("job_time/星宿叛徒")+1,partner->query("xx_jobe"),partner->query("xx_jobp"),
			partner->query("combat_exp")),me);

		tell_object(partner,HIY+"星宿老仙丁春秋用千里传音告诉你，请去到"+me->query("xx/pantu_place")+"配合"+HIM+me->name()+HIW+"("+ capitalize(me->query("id")) + ")"+HIY+"完成杀叛徒的任务。\n"NOR);
		return "听说最近那个混蛋在"+me->query("xx/pantu_place")+CYN"附近出没，你和"+
			menpai+ partner->name()+"("+ capitalize(partner->query("id")) + ")一起去阻击这个叛徒吧。";
	}
}

int give_reward(object me)
{
	int e,p,t,t1,partner,high,myexp,paexp,d,low,e2= 100 + random(51),p2= 20+random(9);

	command("welcome");
	command("say 很好，你为星宿派铲除这个败类，我应该好好嘉奖你。");
	command("touch "+me->query("id"));

	if(!partner = me->query("xx/partner")){
		me->delete("xx/pantu");
		me->delete("xx_job5_finished");
		me->delete("xx/panntu_place");
		me->apply_condition("job_busy",6);
		me->delete("xx/pantu_time");
		me->delete("xx/pantu_super");
		command("say 这次任务你完成的相当不好，我非常不满意你的作为！");
		command("say 你和你的搭档完成任务后不直接回来，干吗去了！");
		command("say 你只能放弃当前这次任务了，不过不会收到经验的惩罚！");
		return 1;
	}

	myexp=me->query("combat_exp");
	paexp=partner->query("combat_exp");

	high=(myexp>paexp)?myexp:paexp;
	if(high==paexp)low=myexp;
	else low=paexp;
	t1 = ABS(12-(time()-(int)me->query("xx/pantu_time"))/60);
	if(t1<2)t1=2;
	if(t1>5)t1=5;

	d=ABS(high-low);

	e = 280 + random(70);

	if (me->query("xx/pantu_super"))
		e = e * (100+random(50)) / 100;

	if ( e > 500 ) e = 400 + random(100);
	p = e/5 + random(e/10);

	me->delete("xx/pantu");
	me->delete("xx_job5_finished");
	me->delete("xx/panntu_place");
	t = time()-(int)me->query("xx/pantu_time");

	me->delete("xx/pantu_time");
	me->delete("xx/pantu_super");

	me->add("job_time/星宿叛徒",1);
	me->add("xx_jobe",e);
	me->add("xx_jobp",p);
	me->add("combat_exp",e);
	me->add("potential",p);
	me->set("job_name","星宿杀叛徒");

	me->clear_condition("job_busy");

	partner->add("combat_exp",e2);
	partner->add("potential",p2);
	partner->add("xx_jobe",e2);
	partner->add("xx_jobp",p2);
	partner->set_temp("have_partner",0);

	if ( (int)me->query("potential", 1) > (int)me->query("max_pot", 1) )
		me->set("potential" , me->query("max_pot", 1) );
	if ( (int)partner->query("potential", 1) > (int)partner->query("max_pot", 1) )
		partner->set("potential" , partner->query("max_pot", 1) );

	if ((string)me->query("family/family_name") == "星宿派"
	   && me->query_skill("poison",1) < 180)
		me->improve_skill("poison", e);

	tell_object(partner,HIC"你被奖励了"+CHINESE_D->chinese_number(e2)+"点经验，"+CHINESE_D->chinese_number(p2)+"点潜能！\n"NOR);
	tell_object(me,HIC"你被奖励了"+CHINESE_D->chinese_number(e)+"点经验，"+CHINESE_D->chinese_number(p)+"点潜能！\n"NOR);

	log_file( "job/pantu",sprintf("%8s%-10s第%5d次任务结束，经验：%d，潜能：%d；总计e：%d，p：%d；exp：%d\n",
		me->name(),"("+me->query("id")+")",me->query("job_time/星宿叛徒"),e,p,me->query("xx_jobe"),me->query("xx_jobp"),
		me->query("combat_exp")),me);
	log_file( "job/pantu",sprintf("%8s%-10s第%5d次搭档结束，经验：%d，潜能：%d；总计e：%d，p：%d；exp：%d\n",
		partner->name(),"("+partner->query("id")+")",me->query("job_time/星宿叛徒"),e2,p2,partner->query("xx_jobe"),partner->query("xx_jobp"),
		partner->query("combat_exp")),me);

	return 1;
}

string ask_fail()
{
	int myexp,paexp,high,low,e,d;
	object me = this_player(),partner;

	if (me->query_temp("quest/family") == "星宿派") {
		me->delete_temp("quest");
		me->add("combat_exp", -(30+random(30)));
		me->apply_condition("job_busy", 6);
		return "好吧，算了算了，算你有心，下次再帮我吧。！";
	}

	if( !me->query("xx/pantu") )
		return "你根本就没任务，放弃什么呀？\n";

	if(!partner=me->query("xx/partner")){
		me->delete("xx/pantu");
		me->delete("xx_job5_finished");
		me->delete("xx/panntu_place");
		me->apply_condition("job_busy", 6);
		me->delete("xx/pantu_time");
		me->delete("xx/pantu_super");
		return "好吧，算了算了，算你有心，下次再帮我杀叛徒吧。！";
	}

	myexp=me->query("combat_exp");
	paexp=partner->query("combat_exp");

	high=(myexp>paexp)?myexp:paexp;
	if(high==paexp)low=myexp;
	else low=paexp;

	d=ABS(high-low);

	e = 100 + random(40);

	me->delete("xx/pantu_place");
	me->delete("xx/pantu_time");
	me->delete("xx/pantu");
	me->delete("xx/partner");
	me->delete("xx/pantu_super");
	me->apply_condition("job_busy", 6+random(3));

	partner->set_temp("have_partner",0);

	me->add("combat_exp",-e/2);

	tell_object(me,HIC"你被惩罚了"+CHINESE_D->chinese_number(e/2)+"点经验！\n"NOR);

	log_file( "job/pantu",sprintf("%8s%-10s放弃了第%5d次任务，减少了经验%d。\n",
		 me->name(),"("+me->query("id")+")",me->query("job_time/星宿叛徒")+1,e/2),me);

	return "既然你不想去完成这个任务的话，就要接受一些惩罚了。";
}

string ask_job5_times()
{
	object ob = this_player();

	if(!ob->query("job_time/星宿叛徒"))
		return "对不起，"+ob->name()+"，你好象还没有在我这里领过任务呢，加油干吧！";
	return ""+ob->name()+"，你帮我派杀死了"+HIY+CHINESE_D->chinese_number((int)ob->query("job_time/星宿叛徒"))+NOR+"个叛徒！";
}

string ask_job4()
{
	object me=this_player();
	int shen=me->query("shen");

	if(shen > -10000 ) return "我们星宿派不需要你来帮忙。";
	if( me->query("combat_exp") < 100000 )
		return "你的实战经验太少了点吧，派你去我可实在不大放心！\n";
	if (me->query("family/family_name") !="星宿派"){
		command("? "+ (string)me->query("id"));
		return "你不是星宿弟子，不能随便进毒虫谷。\n";
	}
	if( me->query("combat_exp") > 2000000 )
	if (me->query("job_time/星宿")+me->query("job_time/星宿叛徒")+me->query("job_time/星宿熬膏") > 500 )
		return "你的经验够多了，还是让别人去做吧！";

        if( me->query("job_name") == "星宿抓虫子")
        	return "你刚抓完虫子，还是去休息会吧。";
	if( me->query_condition("job_busy"))
		return "你还忙着干别的呢，呆会再来吧。";
	if(me->query_condition("gb_mission") || me->query_condition("gb_busy"))
		return "老仙最讨厌臭叫化子，先扔掉你的丐帮帮务再说！";

	command("ok "+ (string)me->query("id"));
	me->set_temp("chonggu_allow",1);
	me->set_temp("xx_job4", 1);
	me->set("job_name", "星宿抓虫子");
	me->apply_condition("job_busy", 10);
	return "好吧，你现在可以进毒虫谷去了。";
}
