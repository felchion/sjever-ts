// mayu.c 马钰
// kxkxkx 2004年3月
// Rewrite By Ciwei@SJ

#include <ansi.h>
#include <get_place.h>


#define QQLL_OB "/d/gumu/qqll/qqll"
#define QQLLYU_OB "/d/gumu/qqll/qqllyu"

#define MIN_EXP 1000000
#define MIN_EXP_DIFF 1000000

inherit NPC;

#include <mudlib.h>

private nomask int ask_qqll();

private nomask int ask_job();
private nomask int can_ask_job();

private nomask void assign_job();
private nomask string set_cyl_location(object obj);
private nomask object set_cyl_player(object obj,object pal);

private nomask int ask_fangqi();

private nomask int ask_ok();
private nomask void give_prize();

int ask_gift()
{
	object me = this_player();
	
	if(INTERMUD_MUD_NAME!="TEST") return 0;
	
	me->set_skill("duanzao",230);
	me->set_skill("zhizao",230);
	
	WORKER_D->check_impove(me,"duanzao",999999,3);
	WORKER_D->check_impove(me,"zhizao",999999,3);
}

void create()
{
   set_name("马钰", ({"ma yu", "ma"}));
   set("title", HIR"全真掌教"NOR);
   set("nickname", HIR"丹阳子"NOR);
   set("gender", "男性");
   set("age", 35);
   set("long",
      "一个仙风道骨的中年道人。\n"
      "神态和祥，举止从容，一看就是世外高人。\n");
   set("attitude", "friendly");
   set("shen", 10000);
   set("str", 20);
   set("int", 20);
   set("con", 20);
   set("dex", 20);
   set("per", 30);
   
   //set("unique", 1);
   
   set("no_get", "马钰对你而言太重了。");
   set("combat_exp", 3000000);
   set("max_neili", 2000);
   set("neili", 2000);
   set("jiali", 50);
   
   set_skill("force", 300);
   set_skill("sword", 300);
   set_skill("cuff", 300);
   set_skill("dodge", 300);
   set_skill("parry", 300);
   set_skill("xiantian-gong", 300);
   set_skill("taizu-quan", 300);
   set_skill("quanzhen-jianfa", 300);
   set_skill("jinyan-gong", 300);
   
   set("no_ansuan",1);
   set("job_npc",1);
      
   map_skill("sword", "quanzhen-jianfa");
   map_skill("parry", "quanzhen-jianfa");
   map_skill("dodge", "jinyan-gong");
   map_skill("force", "xiantian-gong");
   map_skill("cuff", "taizu-quan");
   prepare_skill("cuff", "taizu-quan");
   
   create_family("全真教", 2, "弟子");
   
   set("inquiry", ([
      "淳于蓝" : "淳于蓝就是江南神匠！",
      "七眼石" : (:ask_qqll:),
      "job" : (:ask_job:),
      "fangqi" : (:ask_fangqi:),
      "放弃" : (:ask_fangqi:),
      "失败" : (:ask_fangqi:),
      "完成" : (:ask_ok:),
      "finish" : (:ask_ok:),
      
      "gift":(:ask_gift:),///测试用
            
      ]));
      
	set("chat_chance_combat", 100);
	set("chat_msg_combat", ({
		(: perform_action, "sword.sanqing" :),
                (: perform_action, "sword.qixing" :),
	}));	


   setup();

   carry_object(ARMOR_D("caoxie"))->wear();
   carry_object(ARMOR_D("blackrobe.c"))->wear();
   carry_object(BINGQI_D("changjian"))->wield();
}

// 问七窍玲珑
private nomask int ask_qqll()
{
   command("say 先师曾留给我七人各一件宝物。"
      "留给我的是一块名叫『" HIG"七眼石"NOR "』的顽石。");
   command("say 我一直在寻找能够帮我解开先师深意之能工巧匠。");
   
   this_player()->set_temp("qiyanjob_ask",1);
   return 1;
}

// 要七窍玲珑任务
private nomask int ask_job()
{
	if(can_ask_job())
      assign_job();  // 可以要七窍玲珑任务
	return 1;
}

// 判断是否可以要七窍玲珑任务
private nomask int can_ask_job()
{
   object user, pal, *team;
   int user_exp, pal_exp;

   user = this_user();
   user_exp = user->query("combat_exp");
   
   if(!user->query_temp("qiyanjob_ask"))
   {
   	return 0;
   }      
   if(present("qiyan shi",user))
   {
   	command("say 你不是已经接了任务了么？");
   	return 0;
   }
   
   if(user_exp < MIN_EXP) {
      command("say 此事关系到先师遗物，非同小可。" +
      RANK_D->query_respect(user) +
      "你的修为不够，还是不要过问了吧。");
      return 0;
   }
   if(!user->is_team_leader()) {
      message_vision("$N上下打量了$n两眼。\n", this_object(), user);
      command("say 前路茫茫，" + RANK_D->query_respect(user) +
         "竟欲一人前往，壮志可嘉，但贫道却不能放心啊。");
      return 0;
   }
   team = user->query_team();
   if(sizeof(team) > 2) {
      command("say 这一大帮子人，谁知道你是打劫的还是干嘛的？");
      command("nonsense");
      return 0;
   }
   pal = team[1];
   pal_exp = pal->query("combat_exp");
   if(pal_exp - user_exp <= MIN_EXP_DIFF) {
      command("say 你队伍里的伙伴身手不行啊，万一出了事，"
         "到时候是他救你还是你救他啊。");
      return 0;
   }
   if(pal_exp - user_exp >= MIN_EXP_DIFF*4 )
   {
   	command("say 你队伍里的伙伴是江湖高人啊，你不要总是大树底下乘凉好不好……");
   	command("disapp "+user->query("id"));
   	return 0;
   }
   //
   if(!present(pal)) {
      command("say 咦？怎么好象人不全啊？" + pal->query("name") +
         "怎么没来？");
      return 0;
   }
   if(user->query_condition("job_busy") ||
         pal->query_condition("job_busy")) {
         command("say 你们现在这么忙，不用麻烦你们了。");
      return 0;
   }
   if(user->query("job_name") == "七窍玲珑" ||
         pal->query("job_name") == "七窍玲珑") {
      command("say 现在没有事情需要麻烦你们。");
      return 0;
   }
   if(user->query_skill("duanzao",1) < 220 && user->query_skill("zhizao",1) < 220
   && user->query_skill("caikuang",1) < 220 && user->query_skill("nongsang",1) < 220)
   {
   	message_vision(CYN"$N上下打量了$n两眼。\n"NOR,this_object(),user);
   	command("say 我看你自己都对这种玉石技巧了解不多，还来咋呼什么呀？!");   	
   	return 0;
   }      
   // ok
   return 1;
}


private nomask void assign_job()
{
   object user, pal, qqll;
   //string s_area_n;
   int job_busy, r, max_killers;
   
   object s_player;
   
   user = this_user();   
   pal = (user->query_team())[1];      
   //s_area_n = set_cyl_location(qqll);
   qqll = new(QQLL_OB);
   qqll->set("pal", pal);
   s_player = set_cyl_player(qqll,pal);
   	if(!s_player)
   	{
   		destruct(qqll);
   		command("say");
   		return;
   	}
   	
	user->delete_temp("qiyanjob_ask");
	job_busy = random(10) + 35; // need modifying it?
	
	job_busy = job_busy/3;
	
	user->apply_condition("job_busy", job_busy);
	pal->apply_condition("job_busy", job_busy);
	
	if(user->query("combat_exp")<5000000) r = random(30);
	else if(user->query("combat_exp")<10000000) r = random(20);
	else r = random(15);
	
	max_killers = r < 1 ? 5 : (r < 5 ? 3 : 4);
	
	//max_killers = 5;
	qqll->set("killers", 0);
	qqll->set("max_killers", max_killers);
	command("say 既然如此，就麻烦" + RANK_D->query_respect(user) +
	"将此物交付 江南神匠 淳于蓝。");
	command("whisper " + user->query("id") + " 此人神龙见首不见尾，上次"+s_player->query("name")+"("+s_player->query("id")+")"+"曾在" + get_place( base_name(environment(s_player)) ) + "见过他的踪迹。你可以去打听打听。");
	message_vision("$N给$n一" + qqll->query("unit") + qqll->name() + "。\n", this_object(), user);
	qqll->move(user);
}

private nomask object set_cyl_player(object qqll,object pal)
{
	object* all = users();
	object target;
	int i,j;
	string place;
	
	if(!qqll) return 0;
			
	for(i=0;i<sizeof(all);i++)
	{
		j = random(sizeof(all));		
		target = all[j];
				
		if(!objectp(target)
		|| !environment(target)
		|| !living(target)
		|| !interactive(target)
		|| query_idle(target) > 600
		|| getuid(target) == getuid(pal)
		|| getuid(target) == getuid(this_player())
		|| wizardp(target)
		|| target->query("combat_exp") < 10000		
		|| target->is_ghost()
		|| !place = base_name(environment(target))
		|| strsrch(place,"/d/")!=0
		|| strsrch(place,"/d/death")==0
		|| strsrch(place,"/d/wizard")==0
		|| strsrch(place,"/d/wuguan")==0		
		|| time() - target->query_temp("qqll_job_be_ask") < 7200
		|| !userp(target) )
		{
			target = 0;
			continue;			
		}
		break;		
	}
			
	if(!target) return 0;

	target->set_temp("qqll_job_be_ask",time());
	qqll->set("target_player",target->query("id") );
	place = base_name(environment(target));
	place = place[3..<1];
	place = place[0..strsrch(place,"/")-1];
	place = "/d/"+place;
	qqll->set("target_place",place);
			
	return target;
}

private nomask int ask_fangqi()
{
   object user, qqll, pal;
   
   user = this_user();
   qqll = present("qiyan stone", user);
   if(!qqll) {
      command("say 放弃？没领任务你放弃什么？");
      return 1;
   }
   command("say 先师留下的隐喻竟解不开了？");
   message_vision("$N对$n失望极了。\n", this_object(), user);
   command("say 找你帮忙看来是我的错。");
   message_vision("$N从$n那里拿回" + qqll->query("name") + "。\n",
      this_object(), user, qqll);
   user->apply_condition("job_busy", 10);
   pal = qqll->query("pal", 1);
   if(pal) pal->apply_condition("job_busy", 10);
   destruct(qqll);
   return 1;
}

private nomask int ask_ok()
{
   object user, qqll, qqllyu,pal;
   int ok,exp1,exp2;
   
   user = this_user();
   qqll = present("qiyan stone", user);
   if(!qqll) {
      command("say 你还没领任务呢，怎么完成？\n");
      return 1;
   }
   ok = qqll->query("ok");
   pal = qqll->query("pal", 1);
   if(!ok) {
      command("say 你还没有完成任务呢。\n");
      return 1;
   }
   
   if( (!pal || !present(pal) ) && !wizardp(user) )
   {
   	command("say 你的同伴呢？");
   	return 1;
   }
   
   switch(ok) {
      case 1:
         command("say 招摇撞骗之徒…………？你是说你自己吧。");
         break;
      case 2:
         command("say 原来如此，西方七窍玉……，多谢壮士，"
            "贫道自当再参详先师遗训。");
           give_prize();
         break;
      default:
         qqllyu = new(QQLLYU_OB);
         say("马钰长叹一声：「顽石一方，妄言玲珑，何来七窍，"
            "人世纷扰。」\n");
         command("say 多谢壮士，解开贫道胸中之郁结。贫道无以回报，"
            "此玲珑玉就赠于壮士吧。");
         message_vision("$N给$n一块" + qqllyu->query("name") +
            "。\n", this_object(), user);
         qqllyu->move(user);
         give_prize();
         break;
   }
   
   if(ok==1)
   {
   	int exp = 100 + random(100);
   	int pot = exp / 5 + random(exp / 6);
   	
   	user->add("potential", pot);
   	if(user->query("potential") > user->query("max_pot"))
   		user->set("potential", user->query("max_pot"));
   	
   	exp1=user->add_exp_combat(exp,this_object(),"七窍玲珑");
   	//user->add("combat_exp",exp);
   	pal = qqll->query("pal", 1);
   	if(pal)
   	{
   		pal->add("potential", pot);
   		if(pal->query("potential") > pal->query("max_pot"))
   			pal->set("potential", pal->query("max_pot"));
   		exp2=pal->add_exp_combat(exp,this_object(),"七窍玲珑");
   		//pal->add("combat_exp",exp);
   	}
   	
   	if(pal && user)
   		log_file("job/qqll",
   		sprintf("%s(%s)和%s(%s)七窍玲珑各奖励%d点基准经验，%d点潜能\n",
   		user->query("name"),user->query("id"),pal->query("name"), pal->query("id"), exp, pot));
   		
   	if(user) tell_object(user, HIW"任务完成，你被奖励了：\n" +
   	chinese_number(exp1) + "点实战经验\n" +
   	chinese_number(pot) + "点潜能\n"NOR);
   
   	if(pal) tell_object(pal, HIW"任务完成，你被奖励了：\n" +
   	chinese_number(exp2) + "点实战经验\n" +
   	chinese_number(pot) + "点潜能\n"NOR);
   }
   
   GIFT_D->check_count(user,this_object(),"七窍玲珑",40);      
   user->set("job_name", "七窍玲珑");
   user->add("job_time/七窍玲珑",1);
   user->apply_condition("job_busy",1);
   if(pal)
   {
   	pal->set("job_name", "七窍玲珑");
   pal->add("job_time/七窍玲珑",1);
   	pal->apply_condition("job_busy",1);
   }
   
   if(user->query("job_time/七窍玲珑") > 0 && user->query("job_time/七窍玲珑") % 100 == 0 )
   {
   	message_vision(CYN"$N转身要走，$n忽然拉住了$N，说道：「"+RANK_D->query_respect(user)+"，你这么多次帮我，贫道
感激不尽，其实贫道近来自我参详先师遗训，似乎也有所得，"+RANK_D->query_respect(user)+"不妨那
此物去研究一下，看看贫道所参详是否确实」。\n"NOR,user,this_object());
   qqllyu = new(QQLLYU_OB);
   qqllyu->move(user);
   message_vision("$n给$N一"+qqllyu->query("unit")+qqllyu->name()+"。\n",user,this_object());
   	
   }   
      
   destruct(qqll);
   return 1;
}

private nomask void give_prize()
{
   object user, qqll, pal;
   int exp, pot,exp1,exp2;
   
   user = this_user();
   qqll = present("qiyan stone", user);
   exp = qqll->query("ok") * 150 + random(50);
   
   //exp  = exp * 3;
   exp = exp * 10 / 4;
   
   exp += random(exp);
            
   pot = exp / 5 + random(exp / 6);

   user->add("potential", pot);
   if(user->query("potential") > user->query("max_pot")) 
      user->set("potential", user->query("max_pot"));
   exp1=user->add_exp_combat(exp,this_object(),"七窍玲珑");
   	
   pal = qqll->query("pal", 1);
   if(pal){
   	pal->add("potential", pot);
   	if(pal->query("potential") > pal->query("max_pot"))
   		pal->set("potential", pal->query("max_pot"));
	  exp2=pal->add_exp_combat(exp,this_object(),"七窍玲珑");
	 }
	
	
	if(pal && user)
		log_file("job/qqll",
      sprintf("%s %s(%s)和%s(%s)七窍玲珑各奖励%d点基准经验，%d点潜能\n",
         ctime(time())[4..15], 
         user->query("name"), user->query("id"),
         pal->query("name"), pal->query("id"), exp, pot));
                  
   if(user) tell_object(user, HIW"任务完成，你被奖励了：\n" +
      chinese_number(exp1) + "点实战经验\n" +
      chinese_number(pot) + "点潜能\n"NOR);
   if(pal) tell_object(pal, HIW"任务完成，你被奖励了：\n" +
      chinese_number(exp2) + "点实战经验\n" +
      chinese_number(pot) + "点潜能\n"NOR);
}
