// 洪安通
// Modify Job By River@SJ 2001.1.21
#include <ansi.h>

inherit NPC;
inherit F_MASTER;
int do_ask();
int do_fail();
int do_yao();
int do_jieyao();
int do_bishou();
int do_zhu();
int do_book();
int checkvaild(mapping ob);
void checkbt();

void create()
{
	set_name("洪安通", ({ "hong antong","hong", "antong" }));
	set("long", "他年纪甚老，白鬓垂胸，脸上都是伤疤皱纹，丑陋已极，他就是神龙教的教主。\n");
	set("gender", "男性");
	set("title", "神龙教教主");     
	set("age", 66);       
	set("str", 35);
	set("int", 30);
	set("con", 35);
	set("dex", 30);
	set("per",10);
	set("combat_exp", 15000000);
	set("shen", -350000);
	set("attitude", "peaceful");
	set("max_qi",50000);
	set("max_jing",15000);
	set("neili",44000);
	set("max_neili",22000);
	set("eff_jingli",15000);
	set("jiali",200);
	
	set_skill("leg", 500);
	set_skill("force", 500);
	set_skill("dodge", 500);
	set_skill("parry",500);
	set_skill("strike",500);
	set_skill("whip",500);
	set_skill("shenlong-yaoli",480);
	set_skill("poison",300);
	set_skill("huagu-mianzhang", 490);
	set_skill("youlong-shenfa",480);
	set_skill("dulong-dafa",490);
	set_skill("literate",560);
	set_skill("dagger",500);
	set_skill("tenglong-bifa",480);
	set_skill("hook",500);
	set_skill("lingshe-bianfa",480);
	set_skill("canglang-goufa",490);
  set_skill("shenlong-tuifa",490);
        
	map_skill("leg", "shenlong-tuifa");
	map_skill("force", "dulong-dafa");
	map_skill("dodge", "youlong-shenfa");
	map_skill("strike", "huagu-mianzhang");
	map_skill("parry","huagu-mianzhang");
	map_skill("poison","shenlong-yaoli");
	map_skill("dagger","tenglong-bifa");
	map_skill("hook","canglang-goufa");
	prepare_skill("strike","huagu-mianzhang");

	create_family("神龙教",1, "教主");
	
	set("env/huagu",10+random(10));
	set("chat_chance_combat", 80);
	set("chat_msg_combat", ({
		(: perform_action, "strike.bujue" :) ,
		(: perform_action, "strike.huagu" :) ,
	}));      
	
	set("inquiry", ([
		"task":         (: do_ask :),
		"quest":        (: do_ask :),
		"任务":         (: do_ask :),
		"renwu":        (: do_ask :),
		"job":          (: do_ask :),
		"取消任务":     (: do_fail :),
		"cancel":       (: do_fail :),
		"豹胎易筋丸":   (: do_yao :),
		"yao":          (: do_yao :),
		"jieyao":       (: do_jieyao :),
		"解药":         (: do_jieyao :),
		"匕首":         (: do_bishou :),
		"bishou":       (: do_bishou :),
		"雄黄珠子":     (: do_zhu :),
		"四十二章经":   (: do_book :),
	]));

	set_temp("apply/damage", 88);
	set_temp("apply/dodge", 188);
	set_temp("apply/attack", 188);
	set_temp("apply/armor", 288);
	
	set("bishou",3);
	set("bstime",time());
	setup();
	carry_object("/d/sld/npc/obj/slp")->wear();       
	
	if(clonep(this_object())) call_out("checkbt",15*60);
}

void attempt_apprentice(object ob)
{
	mapping myfam;

	myfam = (mapping)ob->query("family");
	if(!myfam || myfam["family_name"] != "神龙教")
		command("say 你非本教弟子，还是先去向五门使求教吧。");
	else if(ob->query("shen",1) > -100000)
		command("say 你还不够心黑手辣，再去多杀几个白道中人吧。");
	else if(ob->query_skill("dulong-dafa",1) < 140 || ob->query_skill("huagu-mianzhang",1) < 140)
		command("shake "+ob->query("id"));
	else if(ob->query_temp("marks/hant"))
		command("say 你神龙丹拿到了么？");
	else {
		command("say 如果你能亲手杀了五彩神龙，把它的内丹给我拿来，我可以考虑收你为徒。");
		ob->set_temp("marks/hant",1);
	}
}

int recognize_apprentice(object ob)
{
	mapping myfam;
	object hw;

	myfam = (mapping)ob->query("family");
	if(!myfam || (myfam["family_name"] != "神龙教")) return 0;
	if(!ob->is_apprentice_of(this_object())) return 0;
	if( ob->query("shen",1)>0 ) {
		command("say 哼！你竟然敢和白道中人搞在一起，是不是不想活了？！去黑屋好好反省反省吧！");
		if(!objectp( hw =find_object("/d/sld/heiwu")))
			hw=load_object("/d/sld/heiwu");
		ob->set("in_heiwu",time());
		ob->move(hw);        
		return 0;
	}
	return 1;
}

int accept_object(object me, object ob)
{
	if( ob->query("id") == "shenglong dan" ) {
		if(me->query_temp("marks/dragon",1)) {
			command("say 好吧，你既然亲手拿到了神龙丹，我就收下你做我的弟子了。");
			command("recruit " + me->query("id"));
			if( !me->query("sld/addexp")) {
				me->add("combat_exp", 10000);
				me->set("sld/addexp",1);
			}
			me->delete_temp("marks/dragon");
			me->delete_temp("marks/hant");
			call_out("destructing", 1, ob); 
			return 1;
		}
		else if(me->query_temp("marks/hant",1)) {
			command("say 你从哪里弄了颗假丹来骗我！");
			me->fight_ob(this_object());
			this_object()->kill_ob(me);
			me->delete_temp("marks/hant");
			call_out("destructing", 1, ob); 
			return 1;
		}
	}
	
	if(userp(ob)) return 0;
	if(ob->query("id") == "hook book") return 0;
	if(ob->query("unique")) {
		tell_object(me,"洪安通对你嘿嘿笑道：很好，很好！\n");
		me->add("shen", -(1000 + random(3000)));
		ob->move("/d/sld/md1");
		call_out("destructing", 0, ob); }
	else {
		return 0;
	}
	return 1;
}

void destructing(object ob)
{
	if(ob) destruct(ob);
}

int do_book()
{
	object *obs;
	object me = this_player();
	mapping myfam;
	myfam = (mapping)me->query("family");
	if(!myfam || myfam["family_name"] != "神龙教")
		return notify_fail("你要干什么？\n");
	
	if(me->query_temp("sld/bailong"))
	{
		command("? "+getuid(me));
		return 1;
	}
	if(me->query("family/master_id")!="hong antong")
	{
		command("say 你非我弟子，问这个做什么？");
		return 1;
	}
	obs = filter_array(children(USER_OB),(:clonep:));
	obs = filter_array(obs,(:$1->query_temp("sld/bailong"):));	
	if(sizeof(obs)>2)
	{
		command("say 已经有人帮我寻找四十二章经了。");
	}
	command("ok");
	command("say 亏你有心还记得本座这份心事，现在起你就是本教白龙掌门使。");
	command("whisper "+getuid(me)+" 预祝白龙使早日取得四十二章经为我教建功立业。");
	me->set_temp("sld/bailong",1);
	me->set_temp("sld/bailong_exp",me->query("combat_exp"));
	me->set_temp("sld/bailong_time",uptime());
	me->set("title",HIW"神龙教白龙使"NOR);	
	return 1;
}

int do_zhu()
{
	object ob,me=this_player();
	mapping myfam;
	myfam = (mapping)me->query("family");
	if(!myfam || myfam["family_name"] != "神龙教")
		return notify_fail("你要干什么？\n");
	
        if ( present("xianghuang zhuzi", me) )
        {
        	command("say 有你的，比姓韦的小子还贪。");
        	return 1;
	}
	if(me->query("quest_kill/quests")<100)
	{
		command("say 你为本教做了什么？");
		return 1;
	}
	ob = unew("/d/sld/npc/obj/zhuzi");
	if(!ob)
	{
		command("say 雄黃珠子本座已经给其他弟子了。");
		return 1;
	}
	if(!clonep(ob)){
		command("say 雄黃珠子本座已经给其他弟子了。");
		return 1;
	}
        if(clonep(ob) && ob->violate_unique()){
                destruct(ob);         
		command("say 雄黃珠子本座已经给其他弟子了。");
		return 1;
        }
	command("nod");
	ob->move(this_object());
	command("give 1 to "+getuid(me));
	return 1;
}
 
int do_bishou()
{
	object ob = this_object() , me = this_player();
	object bs;
	mapping myfam;

	myfam = (mapping)me->query("family");
	if(!myfam || myfam["family_name"] != "神龙教")
		return notify_fail("你要干什么？\n");
		
	if((time()-ob->query("bstime")) > 900 ){
		ob->set("bstime",time());
		ob->set("bishou",3);
        }
        
        if( me->is_apprentice_of(ob)
	 && me->query_skill("huagu-mianzhang", 1) >=150
	 && me->query_skill("shenlong-tuifa", 1) >= 150
	 && me->query_skill("canglang-goufa", 1) >= 150
	 && me->query_skill("dulong-dafa", 1) >= 150 ){
		if(me->query_temp("get_bishou"))
			return notify_fail("洪安通说道：不是已经给过你匕首了么？怎么又来要了？\n");
		if(ob->query("bishou") <= 0)
			return notify_fail("洪安通说道：我身边现在没有匕首，你等下来要吧。\n");
		bs = new(BINGQI_D("dagger/sld_bishou"));
		if(!objectp(bs))
			return notify_fail("洪安通说道：我身边现在没有匕首，你等下来要吧。\n");
		bs->move(me);
		ob->add("bishou",-1);
		message_vision("洪安通说道：好好地为本教出力，不会亏待你的！话毕，给了$N一柄匕首。\n",me);
		me->set_temp("get_bishou",1);
		return 1;
	}
	else
		return notify_fail("你要干什么？\n");
}
   
int do_ask()
{
	object me,lp,hw;
	mapping myfam,quest;
	string prev;

	me=this_player();
		
	myfam = (mapping)me->query("family");
	if (!myfam || myfam["family_name"] != "神龙教")
		return notify_fail("你要干什么？\n");

	if(me->query("shen",1) > 0){
		command("say 哼！你竟然敢和白道中搞在一起，是不是不想活了？！去黑屋好好反省反省吧！");
		if(!objectp(hw=find_object("/d/sld/heiwu")))
			hw=load_object("/d/sld/heiwu");
		me->set("in_heiwu",time());
		me->move(hw);        
		return 1;
	}

	if( me->query_condition("job_busy")) {
		command ("say 你小子竟敢偷偷做其他门派的任务，还想在神龙教里呆下去么？");
		return 1;
	}
        
	if(me->query("quest_kill/questing")==1) {
		command("say 你先把前一个任务完成再说。"); 
		return 1;
	}

	if((time()-me->query("quest_kill/ok_time")) < 300) {
		command("say 你先休息一下吧。"); 
		return 1;
	}   

        if ( me->query("job_name") == "神龙教" && me->query("combat_exp") >= 100000){
		command("say 你刚做完任务，先休息一下吧。");
		return 1;
	}
		
	if(random(me->query("quest_kill/quests"))>200
	&& random(me->query("pur"))<10
	&& !me->query("quest/baibian")
	&& !me->query_temp("quest/baibian")
	&& random(me->query("kar"))>20
	)
	{
		command("bite");
		command("say 本座突然想起本教的一个大仇人，你去帮我杀了他！");
		command("whisper "+getuid(me)+" 此人名叫韦小宝，为祸本教不小。");
		me->set_temp("quest/baibian",1);
		if ( present("su quan", environment(me)) )
		{
			present("su quan", environment(me))->force_me("iddle");
			command("kill1");
		}
		log_file("quest/baibian",sprintf("%-8s(%-8s) 得到领会神行百变的机会。",me->query("name"),getuid(me)),this_object());
		return 1;
	}
		
	if(me->query("combat_exp") > 2000000) {
		command("say 已经没有什么任务适合你做了。");
		return 1;       
	}
	
	if( random(2) == 1 ) {
		if(mapp(quest=TASK_D->get_quest(me, 0, "索命", "神龙教"))) {
			if(checkvaild(quest) && (!(prev=me->query_temp("prev",1)) || prev!=quest["id"])) {
				if(me->query("quest_kill/cancel"))
                                        me->set("quest_kill/ok_time",me->query("quest_kill/start_time"));
                                else
                                        me->set("quest_kill/ok_time",time());

                                me->delete("quest_kill/cancel");
                                me->set("quest_kill/id",quest["id"]);
                                me->set("quest_kill/name",quest["name"]);
                                me->set("quest_kill/start_time",time());
                                me->set("quest_kill/exp",1);
                                me->set("quest_kill/questing",1);
                                
                                lp=new("/d/sld/npc/obj/suomp");
                                lp->set("long",lp->query("long")+HIR"上面刻着"+quest["place"]+quest["name"]+"几个字。\n"NOR);
                                lp->move(me);   
                                command("say "+quest["name"]+"常与本教做对，你速去"+quest["place"]+"把他杀了！");
                                message_vision("洪安通拿出一块索命牌，在上面刻下了"+quest["place"]+quest["name"]+"几个字，交给了$N。\n",me);
                                me->apply_condition("job_busy", 9);
                                me->set("job_name","神龙教");
                                me->set_temp("prev",quest["id"]);
                                return 1;                               
                        }
                }      
        }
        if(mapp(quest=TASK_D->get_quest(me, 0, "招魂", "神龙教")))
        {
                if(checkvaild(quest) && 
                  (!(prev=me->query_temp("prev",1)) || prev!=quest["id"])) {
                        if(me->query("quest_kill/cancel"))
                                me->set("quest_kill/ok_time",me->query("quest_kill/start_time"));
                        else
                                me->set("quest_kill/ok_time",time());
                        me->delete("quest_kill/cancel");
                        me->set("quest_kill/id",quest["id"]);
                        me->set("quest_kill/name",quest["name"]);
                        me->set("quest_kill/start_time",time());
                        me->set("quest_kill/exp",1);
                        me->set("quest_kill/questing",1);
                        me->set_temp("zhaohun/times",0);
                        if(me->query_temp("quest/huilu",1)!=0)
                                me->delete_temp("quest/huilu");  
                        lp=new("/d/sld/npc/obj/zhaohp");
                        lp->set("long",lp->query("long")+HIR"上面刻着"+quest["place"]+quest["name"]+"几个字。\n"NOR);
                        lp->move(me);
                        command("say "+"现在令你速去"+quest["place"]+"设法让"+quest["name"]+"归顺本教！");
                        message_vision("洪安通拿出一块招魂牌，在上面刻下了"+quest["place"]+quest["name"]+"几个字，交给了$N。\n",me);
                        me->set_temp("prev",quest["id"]);
                        me->set("job_name","神龙教");
                        me->apply_condition("job_busy", 9);
                        return 1;                               
                }
        }                                       
        command("say 现在没有任务，你等下再来吧。");
        return 1;
}

int do_fail()
{
        int times,cf,qi;
        object me=this_player();
        object obj,hw;

        if(me->query("quest_kill/questing")!=1)
                return notify_fail("你想干什么？\n");

        times=(time()-me->query("quest_kill/start_time"))/60;

        if(times<10) {
                if(me->query_temp("quest_kill/failin5",1)>1) {
                        me->delete_temp("quest_kill/failin5");
                        command("say 你三番五次抗命不遵，是不是不想活了？！给我到黑屋呆着去！");
                        if(!objectp(hw=find_object("/d/sld/heiwu")))
                                hw=load_object("/d/sld/heiwu");
                        me->set("in_heiwu",time());
                        me->move(hw);
                        return 1;
                }

                if(me->query_temp("quest_kill/failin5",1)==0)
                        me->set_temp("quest_kill/failin5",1);
                else
                        me->set_temp("quest_kill/failin5",2);    
                command("say 怎么？！你想抗命不成？！");
                return 1;
        }

        if(times > 30)   times=30;
        cf = 2000*30/times;
        qi=me->query("max_qi");
        if(cf>=qi) cf=1; else cf=qi-cf;
        me->set("qi", cf);

        command("say 这么点事情都办不了，要你这种无用之人何用！");
        message_vision("洪安通一掌把$N打得飞了出去！\n",me);

        me->set("quest_kill/questing",0);
        if(me->query_temp("quest_kill/failin5",1)!=0)
                me->delete_temp("quest_kill/failin5");
        if(objectp(obj = present("suomingpai", me))) {
                destruct(obj);
        }
        if(objectp(obj = present("zhaohunpai", me))) {
                destruct(obj);
        }
        me->set("quest_kill/cancel",1);
        return 1;
}

int checkvaild(mapping ob)
{
        string* ffid= ({ "young monk","mu ren","zhu cong",
                       "zhu wanli","ping wei","gui tong","lao ren","qiao zhujiang",
                             "shen laoban","leitai npc","lao ban","mengmian nuzi",
                             "daojue chanshi","huang zhe","jin ren","wu zhanglao",
                             "zhu danchen","yang guo","xiao longnu","biaoshi","biaotou", 
                             "dizi","wulun npc","huang yaoshi","feng qingyang" });
        string* ffname= ({ "小和尚","木人","朱聪","褚万里","平威",
                           "青鬏龟童","白髯老人","劁猪将","沈老板","擂台直播员","当铺老板",
                           "蒙面女子","道觉禅师","黄真","金人","吴长老","朱丹臣","杨过",
                           "小龙女","镖师","镖头","弟子","木瓜","黄药师","风清扬" });
        int i;
        object target;
        string npcfile;
        object* living;

        living=livings(); 
        for(i=0;i<sizeof(living);i++)
        {
                if(living[i]->query("id")==ob["id"] && living[i]->query("name")==ob["name"])
                {
                        target=living[i];
                        break;
                }
        }
        if(i>=sizeof(living))
                return 0;       
        if((target->query("combat_exp")<100000) && 
           (target->query("family/family_name")=="少林派"))
                return 0;
        npcfile=file_name(target);
        if(strsrch(npcfile,"/kungfu/class/shaolin/")>=0)
        {
                if(strsrch(target->name(1),"圆")>=0)
                        return 0;
        }
        if(strsrch(npcfile,"/clone/npc/zhong-shentong")>=0)
                return 0;
        if(strsrch(npcfile,"gumu/")>=0) return 0;
        for(i=0;i<sizeof(ffid);i++)
        {
                if(ffid[i]==ob["id"] && ffname[i]==ob["name"])
                {
                        return 0;
                }
        }
        if((ob["id"]=="killer") || (ob["id"]=="stealer")
          || (ob["id"]=="cateran") 
          || (ob["id"]=="menmianren") || (ob["id"]=="mengmian ren"))
                return 0;
        return 1;
}

int do_yao()
{
        object me=this_player();
        object yao;
        int jobs;
        mapping myfam;
        
        myfam = (mapping)me->query("family");
        if(!myfam || myfam["family_name"] != "神龙教")
                return notify_fail("你要干什么？\n");
	
	if(present("baotaiyijinwan",me))
	{
		command("say 你身上不正有么？");
		return 1;
	}
        if(me->query("quest_kill/btyao")){
                command("say 你已经吃过豹胎易筋丸，不能再吃了。");
                return 1;       
        }
        if(me->query("combat_exp") <= 1500000){
                if(!me->query("quest_kill/quests")) {
                        command("say 你最近好象没完成过什么任务，居然还敢要豹胎易筋丸？！");
                        return 1;
                }
                jobs=me->query("quest_kill/quests");
                tell_object(me,"你最近完成了"+jobs+"项任务。\n");
                jobs=300-jobs;
                if(jobs > 0) {
                        command("say 你最近完成的任务不够多，尚不足以得到豹胎易经丸。");
                        return 1;
                }
        }
        me->set("quest_kill/quests",0);
        yao = new("/d/sld/npc/obj/btyao");
        command("nod");
        if(!yao->move(me))
                yao->move(environment(me));
        return 1;
}

int do_jieyao()
{
        object me=this_player();
        object yao;
        int jobs;
        mapping myfam;
        
        myfam = (mapping)me->query("family");
        if(!myfam || myfam["family_name"] != "神龙教")
                return notify_fail("你要干什么？\n");
	if(present("jie yao",me))
	{
		command("say 你身上不正有么？");
		return 1;
	}
	
        if(me->query("combat_exp")<=2000000){
                if(!me->query("quest_kill/quests")){
                        command("say 你最近好象没完成过什么任务，居然还敢要豹胎易筋丸的解药？！");
                        return 1;
                }
                jobs=me->query("quest_kill/quests");
                tell_object(me,"你最近完成了"+jobs+"项任务。\n");
                if(jobs < 100){
                        command("say 你最近完成的任务不够多，尚不足以得到豹胎易筋丸的解药。");
                        return 1;
                }
                if(!me->query("quest_kill/fazuo")) {
                        command("say 你现在好象不需要豹胎易筋丸的解药么。");
                        return 1;
                }
                if(me->query("age")==me->query("quest_kill/fz_year")
                && me->query("month")==me->query("quest_kill/now_month") )
                {
                	command("en "+getuid(me));
                	return 1;
                }
        }
        me->set("quest_kill/quests",0);
        command("nod");
        yao=new("/d/sld/npc/obj/jieyao");
        if(!yao->move(me))
                yao->move(environment(me));
        return 1;
}

void checkbt()
{
	object * usrlist;
	int i;
	//,dis;
	string tit;

	remove_call_out("checkbt");
	call_out("checkbt",15*60);
	
	usrlist = filter_array(users(),(:$1->query("family") == "神龙教":));
        for(i=0;i<sizeof(usrlist);i++) {
        	if(usrlist[i]->query_temp("sld/bailong"))
        	{
        		if(((usrlist[i]->query("combat_exp")-usrlist[i]->query_temp("sld/bailong_exp")+100)*60/(uptime()-usrlist[i]->query_temp("sld/bailong_time")))>=20)
        		{
        			usrlist[i]->set("title",HIW"神龙教白龙使"NOR);
        			continue;        			
        		}
        		usrlist[i]->delete_temp("sld/bailong");
        		usrlist[i]->delete_temp("sld/bailong_exp");
        		usrlist[i]->delete_temp("sld/bailong_time");
        		tell_object(usrlist[i],HIR"洪教主飞鸽传书通知你：你因为长时间偷懒，白龙使身份已经被取消。\n");
        	}
                if(usrlist[i]->query("family/generation")<=2 && usrlist[i]->query("combat_exp")>=500000) {
	                if(usrlist[i]->query("combat_exp")>=4000000) tit=HIB"神龙教副教主"NOR;
        	        else if(usrlist[i]->query("combat_exp")>=3600000) tit=HIG"神龙教长老"NOR;
                	else if(usrlist[i]->query("combat_exp")>=3200000) tit=HIR"神龙教长老"NOR;
                	else if(usrlist[i]->query("combat_exp")>=2800000) tit=HIY"神龙教长老"NOR;
                	else if(usrlist[i]->query("combat_exp")>=2400000) tit=HIW"神龙教长老"NOR;
                    	else if(usrlist[i]->query("combat_exp")>=2000000) tit="神龙教长老";
                	else if(usrlist[i]->query("combat_exp")>=1800000) tit=HIG"神龙教护法"NOR;
                	else if(usrlist[i]->query("combat_exp")>=1600000) tit=HIR"神龙教护法"NOR;
                	else if(usrlist[i]->query("combat_exp")>=1400000) tit=HIY"神龙教护法"NOR;
                	else if(usrlist[i]->query("combat_exp")>=1200000) tit=HIW"神龙教护法"NOR;
                	else if(usrlist[i]->query("combat_exp")>=1000000) tit="神龙教护法";
                	else if(usrlist[i]->query("combat_exp")>=900000) tit=HIG"神龙教护法弟子"NOR;
                	else if(usrlist[i]->query("combat_exp")>=800000) tit=HIR"神龙教护法弟子"NOR;
                	else if(usrlist[i]->query("combat_exp")>=700000) tit=HIY"神龙教护法弟子"NOR;
                	else if(usrlist[i]->query("combat_exp")>=600000) tit=HIW"神龙教护法弟子"NOR;
                    	else tit="神龙教护法弟子";
                       	usrlist[i]->set("title",tit);
                }
                /*                
                if(usrlist[i]->query("quest_kill/btyao") && !usrlist[i]->query("quest_kill/jieyao")) {
                        usrlist[i]->set("quest_kill/jieyao",1);
                        usrlist[i]->set("quest_kill/bt_year",usrlist[i]->query("age"));
                        usrlist[i]->set("quest_kill/bt_month",usrlist[i]->query("month"));
                }
        	else if(usrlist[i]->is_ghost())  continue;
                else if(usrlist[i]->is_fighting())  continue;
                else if(usrlist[i]->query("quest_kill/jieyao"))
                {
                        me=usrlist[i];
                        dis=(me->query("age")-me->query("quest_kill/bt_year"))*12+
                            (me->query("month")-me->query("quest_kill/bt_month"));
               		if(dis>=13)
                        {
                   		if(me->query("death_times")>me->query("quest_kill/dts"))
                   		{
                       			me->set("quest_kill/dts",me->query("death_times"));
                       			me->add("quest_kill/bt_year",1);
                   		}
                                if(me->query_temp("jytell",1)==0)
                                {
                                        tell_object(me,"你已经超过一年没吃解药了！\n");
                                        me->set_temp("jytell",1);
                                }
    				if((me->query("quest_kill/jieyao")==1)
    				|| ((time()-me->query_temp("jyfz",1))>900))
                                {
                                        tell_object(me,"你突然感到一阵剧痛从腹部向全身迅速扩散！\n");
                                        me->add("eff_jingli", -me->query("eff_jingli")/10);
                                        me->set("jingli",0);
                                        me->set("neili",0);
       					me->set_temp("jyfz",time());
                                        me->receive_wound("qi", me->query("eff_qi")/2);
                                        me->receive_wound("jing", me->query("eff_jing")/2);
                                        me->unconcious(); 
                                        me->set("quest_kill/jieyao",2);
                                }
                        }
                   else if (dis==12) {
                                if(!me->query_temp("jytell"))
                                {
                       			tell_object(me,"你已经一年没吃解药了！\n");
                                        me->set_temp("jytell",1);
                                }
                   		if(!me->query("quest_kill/dts"))
                   			me->set("quest_kill/dts",me->query("death_times"));
                        }
                }
                */
        }
}