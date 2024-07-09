// wu.c by snowman@SJ 丐帮吴长老
// Modify by Looklove@SJ 2000/8/30
// Last change by Looklove add a log_file 2000/9/1
// add by tangfeng@SJ 2004

#include <ansi.h>
inherit NPC;
inherit F_MASTER;

#define QUESTDIR "quest/天龙八部/"
#define QUESTDIR1 "quest/天龙八部/凌波微步篇/"
#define QUESTDIR2 "quest/天龙八部/天龙营救篇/"
#define QUESTDIR3 "quest/天龙八部/萧峰身世篇/"
#define QUESTDIR4 "quest/天龙八部/大辽救援篇/"
#define QUESTDIR5 "quest/天龙八部/复兴天下篇/"

string ask_job(string arg);
string ask_fangqi();
string ask_gonglao(object who);
int ask_jiuyuan();
int ask_xiaofeng();
int ask_jingbian();
int ask_buzhu();
void change_jobtime(string arg, object me);
void create()
{
	set_name("吴长老", ({"wu zhanglao", "wu", "zhanglao"}));
        set("title", HIW"丐帮九袋长老"NOR);
        set("gb/bags",9);
        set("gender", "男性");
        set_max_encumbrance(1000000);
        set("age", 55);
        set("long", "他乃丐帮六大长老之一,善使一把鬼头刀。\n");
        set("attitude", "friendly");
        set("no_get", "吴长老对你而言太重了。\n");
        set("str", 25);
        set("int", 25);
        set("con", 25);
        set("dex", 25);
        set("job_npc", 1);
	      set("unique", 1);
        set("no_bark",1);
        set("per", 21);

        set("qi", 2600);
        set("max_qi", 2600);
        set("jing", 1400);
        set("max_jing", 1400);
        set("eff_jingli", 2400);
        set("neili", 2600);
        set("max_neili", 2600);
        set("jiali", 100);

        set("combat_exp", 600000);

        set_skill("force", 120); // 基本内功
        set_skill("checking",120);
        set_skill("huntian-qigong", 120); // 混天气功
        set_skill("dodge", 120); // 基本躲闪
        set_skill("xiaoyaoyou", 120); // 千里行乞
        set_skill("parry", 120); // 基本招架
        set_skill("blade", 120);
        set_skill("liuhe-daofa",120);
        set_skill("begging", 90);
        set_skill("suohou-shou",140);
        set_skill("hand",130);

        map_skill("blade","liuhe-daofa");
        map_skill("force", "huntian-qigong");
        map_skill("dodge", "xiaoyaoyou");
        map_skill("parry","liuhe-daofa");
        map_skill("hand","suohou-shou");
        prepare_skill("hand","suohou-shou");

        create_family("丐帮", 18, "九袋长老");

        set("inquiry", ([
                "丐帮" : "我们丐帮是天下第一大帮！\n",
                "带头大哥" : "这里不是黑社会，没什么带头大哥！\n",
                "玄慈" : "那是少林主持！\n",
                "洪七公" : "七指神丐他老人家正是我们丐帮帮主！\n",
                "帮主" : "现在帮主是七指神丐洪七公！\n",
                "name" : "在下吴长风。\n",
                "job"  : (: ask_job :),
                "功劳"  : (: ask_gonglao :),
                "不干了"  : (: ask_fangqi :),
                "fangqi"  : (: ask_fangqi :),
                "放弃"  : (: ask_fangqi :),
				"补助"  : (: ask_buzhu :),//for hs dizi 增加gb job 次数
		            "救援萧峰" : (: ask_jiuyuan :),
		            "解救萧峰" : (: ask_jiuyuan :),
		            "援救萧峰" : (: ask_jiuyuan :),
	            
		            "救援" : (: ask_jiuyuan :),
		            "解救" : (: ask_jiuyuan :),
		            "援救" : (: ask_jiuyuan :),
		            "萧峰" : (: ask_xiaofeng :),
		            "竹林惊变" : (: ask_jingbian :),
		            "丐帮内乱" : (: ask_jingbian :),
		            "丐帮惊变" : (: ask_jingbian :),

                "接引弟子" : "他是我的小弟子，负责考核入帮的好汉。\n",
        ]));
        setup();
        carry_object(BINGQI_D("blade"))->wield();
        carry_object(ARMOR_D("bainayi"))->wear();
        carry_object(ARMOR_D("caoxie"))->wear();
        carry_object("/kungfu/class/gaibang/obj/gb_budai9")->wear();
}

void heart_beat()
{
	::heart_beat();

	clear_conditions_by_type("poison");
	if( !is_fighting() && living(this_object()))
		reincarnate();
}
int ask_buzhu()
{
	object me = this_player();
	int i = me->query("job_time/丐帮");    
    if(!me->query("from_site")||me->query("from_site")!="LN")
	{
		command("shake");
		command("say 你不是LN站转过来的，不能享受服务。");
		return 1;
	}
	if(me->query("family/family_name")!="华山派")
	{
		command("smile");
		command("say 只给华山派的GG MM们增加点任务次数。");
		command("pat "+me->query("id"));
		return 1;
	}
	if(me->query("combat_exp")<2000000)
	{
		command("shrug");
		command("say 你还是加紧练功去吧。");
		command("pat "+me->query("id"));
		return 1;
	}
	if(me->query("job_time/add_gbjob_for_ln")>=3)
	{
		command("shake");
		command("say 你最多只能询问增加三次。");
		message_vision(HIR"$N高举双手大喝：神啊！\n"NOR+
                 "\n" HIW
                 "救救我吧！一道闪电\n"
                 "\n" BLU
                 "         --------\n" HIC
                 "               /\n"
                 "              /\n"
                 "             /\n"
                 "             ----\n"
                 "                /\n"
                 "               /\n"
                 "              /\n"NOR+		
			    "正好落在$n的身上！\n"NOR,this_object(),me);
		command("hehe "+me->query("id"));
		return 1;
	}
	command("nod");
	if(!i)	message_vision(CYN"$N掏出一本册子翻了翻，奇怪地看了看$n，指着上面的墨迹对$n说道，你竟然一次记功也没有。\n"NOR,this_object(),me);
	else message_vision(CYN"$N掏出一本册子翻了翻，指着上面的墨迹对$n说道，你目前已记功" + chinese_number(i) + "次。\n"NOR,this_object(),me);
	if(!me->query("job_time/add_gbjob_for_ln"))
	{
		command("hehe");
		command("say "+HIR"你竟然是辽宁二站转过来的，就给你增加点吧。第一询问可以免费增加一百次丐帮任务次数。"NOR);
	}
	else if(me->query("job_time/add_gbjob_for_ln")==1)
	{
		command("say 你还需要增加？好吧");
		command("say "+HIR"你需要注意的是，虽然本次询问可以给你增加五十次丐帮任务次数，但也同时损失二十万点经验值。"NOR);
	}
	else
	{
		command("oh");
		command("say 你还需要增加？好吧");
		command("say "+HIR"你需要注意的是，虽然本次询问可以给你增加六十次丐帮任务次数，但也同时损失三十万点经验值。"NOR);
	}
	command("say "+HIW"你要是确认就选择Yes（Y） 要是放弃选择No（N） --------"NOR);
	input_to( (: change_jobtime :), me );
	return 1;
}
void change_jobtime(string arg, object me)
{
	int i = me->query("job_time/丐帮");    
	if( arg == "N" || arg == "n" )    
	{
		command("hehe");
		command("say 算我白说......");
		command("shrug");
		return;
	}
	if(arg != "Y"  && arg != "y" ) 
	{
	   command("?");
	   if(!me->query("job_time/add_gbjob_for_ln"))
	   {
		   command("say "+HIR"第一询问可以免费增加一百次丐帮任务次数。"NOR);
	   }
	   else if(me->query("job_time/add_gbjob_for_ln")==1)
	   {
		   command("say "+HIR"虽然本次询问可以给你增加五十次丐帮任务次数，但也同时损失二十万点经验值。"NOR);
	   }
	   else
	   {
		   command("say "+HIR"虽然本次询问可以给你增加六十次丐帮任务次数，但也同时损失三十万点经验值。"NOR);
	   }
	   command("say "+HIW"你要是确认就选择Yes（Y） 要是放弃选择No（N） --------"NOR);
	   input_to( (: change_jobtime :), me );
	   return;
	}
 
 	if(!me->query("job_time/add_gbjob_for_ln"))
	{
		command("nod");
		i=i+100;
		message_vision(CYN"$N吐了口唾沫，抹去原来的痕迹，摸出一个自来水笔，一边写一边对$n说道，你目前已记功" + chinese_number(i) + "次了。\n"NOR,this_object(),me);
		command("hehe");
		
		command("say 你要是愿意，可以再询问的，不过可能要付出点代价....");
		log_file("job/add_gbjob", sprintf("%s(%s)第一次增加丐帮任务，增加100次。经验：%d。\n", me->name(1),me->query("id"),me->query("combat_exp")) ); 
	}
	else if(me->query("job_time/add_gbjob_for_ln")==1)
	{
		i=i+50;
		message_vision(HIY"$N高举双手大喝：神啊！\n"NOR+
                 "\n" HIW
                 "救救我吧！一道闪电\n"
                 "\n" BLU
                 "         --------\n" HIC
                 "               /\n"
                 "              /\n"
                 "             /\n"
                 "             ----\n"
                 "                /\n"
                 "               /\n"
                 "              /\n"NOR+		
			    "正好落在$n的身上！\n"NOR,this_object(),me);
		message_vision(HIY"$n身上冒起丝丝白烟，浑身总感觉不是很舒服。\n"NOR,this_object(),me);
		me->add("combat_exp",-200000);//200K的代价?
		message_vision(CYN"$N嘿嘿两声，吐了口唾沫，抹去原来的痕迹，一边写一边对$n说道，好了好了，你目前已记功" + chinese_number(i) + "次了。\n"NOR,this_object(),me);
		command("hehe");
		command("say 你已经损失200K经验，你要是愿意，可以再询问的，不过可能要付出点代价....");
		log_file("job/add_gbjob", sprintf("%s(%s)第二次增加丐帮任务，增加50次，并减少经验200K。当前经验：%d。\n", me->name(1),me->query("id"),me->query("combat_exp")) ); 
	}
	else
	{
		i=i+60;
		message_vision(HIR"$N高举双手大喝：神啊！\n"NOR+
                 "\n" HIW
                 "救救我吧！一道闪电\n"
                 "\n" BLU
                 "         --------\n" HIC
                 "               /\n"
                 "              /\n"
                 "             /\n"
                 "             ----\n"
                 "                /\n"
                 "               /\n"
                 "              /\n"NOR+		
			    "正好落在$n的身上！\n"NOR,this_object(),me);
		message_vision(HIR"$n身上冒起丝丝白烟，浑身总感觉不是很舒服。\n"NOR,this_object(),me);
		me->add("combat_exp",-300000);//300K的代价?
		message_vision(CYN"$N嘿嘿两声，吐了口唾沫，抹去原来的痕迹，一边写一边对$n说道，好了好了，你目前已记功" + chinese_number(i) + "次了。\n"NOR,this_object(),me);
		command("hehe");
		log_file("job/add_gbjob", sprintf("%s(%s)第三次增加丐帮任务，增加60次，并减少经验300K。当前经验：%d。\n", me->name(1),me->query("id"),me->query("combat_exp")) ); 
	}
	me->add("job_time/add_gbjob_for_ln",1);
	me->set("job_time/丐帮",i);
	return;
}
string ask_job(string arg)
{
	object player, me;
	mapping quest;

	player = this_player();
	me = this_object();

	if (!player->query("gb_pass"))
		return "你没有通过接引弟子的考验，我可不敢用你。";

	if(player->query("job_time/丐帮") > 10000)
		return "你已经为我丐帮出生入死这么多次，在下实在不忍心烦劳您老了。";

	if(player->query("combat_exp") < 100000 && player->query("family/family_name")!="丐帮" )
		return "你的实战经验太低，还是再努力一番再来这里把。";

	if(player->query("combat_exp") > 4000000)
		return "您老这么高的身手，要您出手怕有失您的身份，还是算了吧。";

	if(player->query("job_name") =="丐帮")
		return "现在我这里没有给你的任务，你去其他地方看看吧？";

	if(player->query_temp("quest/busy")) //added by tangfeng 与quest冲突
		return "现在我这里没有给你的任务，你还是先处理好你其他事情再说吧。";

	//if(player->query("shen") < 1000)
	//	return RANK_D->query_respect(player) + "的正气还不够，我无法放心让你做这个任务！";

	if( player->query_temp("gb_wait_job") >= 5 ){
		command("say 你是消遣我来着？叫你等会儿就是等会儿！");
		command("kick "+player->query("id"));
		tell_room(environment(), "\n吴长老飞起一脚，将"+ player->name()+ "踢飞出去！\n"NOR, ({ player }));
		player->move("/d/fuzhou/yuchuan5");
		tell_room(environment(player), "\n只见"+ player->name()+ "大叫一声飞过来，摔了个狗吃屎！\n"NOR, ({ player }));
		player->start_busy(3);
		player->delete_temp("gb_wait_job");
		return "当我是木头，不会踢人啊！";
	}

	if( me->is_busy()) {
		player->add_temp("gb_wait_job", 1);
		return "没看见我正忙着吗,你等一会。";
	}

	if ((int)player->query_condition("gb_busy")){
		player->add_temp("gb_wait_job", 1);
		return "你连上个任务去都不想去，是想找近的吧？等会再来！";
	}

        // now all jobs use this condition.
	if ((int)player->query_condition("job_busy")) {
		if( player->query_temp("quest/id") && player->query_temp("quest/family") == "丐帮")
			return "不是让你去"+HIY+player->query_temp("quest/place")+CYN+"杀"+HIG+player->query_temp("quest/name")+CYN+"了吗，怎么还在这里？\n";
		if( player->query_temp("quest/family") == "丐帮")
			return "给你的任务你还没完成怎么又跑回来了！";
		else
			return "现在我可没有给你的任务，等会再来吧！";
	}

	if ((string)player->query_temp("quest/family") == "星宿派"){
		player->add("job_time/丐帮", -player->query("job_time/丐帮")/10);
		player->delete_temp("quest");
		return "竟敢去为丁老怪效力，扣掉你完成丐帮任务次数的一成！";
	}

	if(!mapp(quest= TASK_D->get_quest(player, 1, "杀", "丐帮")))
		return "现在我可没有给你的任务，等会怎么样？";

	me->start_busy(random(2));
	player->delete_temp("gb_wait_job");
	player->set_temp("quest", quest);
	player->apply_condition("job_busy", 30);
	player->apply_condition("gb_busy", 30);
	player->set_temp("gb_job", 1);
	player->start_busy(random(2));
	command ("say 好吧，最近「"+ HIG + quest["name"]+CYN"」一直和我丐帮作对，你前去设法将此人除掉！");
	return "此人现在在" + HIY + quest["place"]+CYN"一带，务必在"+TASK_D->get_time(quest["time"])+"之前赶回来。\n";
}

int accept_object(object who, object ob, object me)
{
        int type;
        me = this_object();

        if( me->is_busy() ){
                command("say 我现在正忙着呢，你稍等一下吧。");
                return 0;
        }
        if ( !who->query("gb_pass")) return 0;
        if ( !who->query_temp("gb_job")){
                command("say 我好象没有给过你任务啊？");
                return 0;
        }
        if( ob->query("id")!="corpse" && ob->query("id")!="shouji") return 0;
                who->start_busy(1);

        if (who->query_temp("quest/family") == "星宿派"){
                command("chat "+ who->name(1)+"偷偷为星宿派效力，现在把他驱逐出丐帮。\n");
                command("chat "+ who->name(1)+"这等武林败类，人人得而诛之!!!\n");
                who->delete("gb_pass");
                return 0;
        }

        // check job.
        type = TASK_D->is_target(ob, who);

        switch (type){
            case 0: command("say 这东西我要来没用。");
                    return 0;
            case -2:command("say 嘿嘿。。。，你杀错人了。");
                    return 0;
            case -3:command("say 嗯，这么久才办完，你干什么去了？给我滚一边去！");
                    who->apply_condition("job_busy", 9);
                    return 0;
            case -4:command("say " + RANK_D->query_rude(who) + "，居然敢欺骗本长老，拿命来把！");
                    call_out("killing_object",1, who);
                    return 0;
            default:call_out("destroy", 1, ob);
                    me->start_busy(2);
                    call_out("ok", 1, who);
                    return 1;
        }
        return 0;
}

void ok(object apper)
{
	int job_time,pot,exp;
	if (!apper) return;
	if ( apper->query("family/family_name") == "丐帮"
	 && apper->query_skill("huntian-qigong")){
		apper->improve_skill("huntian-qigong",apper->query("int")*5);
	}

	command("thumb " + apper->query("id"));
	command("say 你为丐帮立下了功劳，我们绝不会亏待你的。");

	job_time = (int)apper->query("job_time/丐帮")/800;
	if (job_time <1) job_time = 0;

	tell_room(environment(),HIY "吴长老在"+ apper->name()+ "的耳边低声似乎在讲解着什麽，一会儿"+ apper->name() +"不住点头，面呈喜色。\n"NOR, ({ apper }));
	tell_object(apper, "吴长老伏身在你耳边悄声指点了你一些武功要领...\n");
	//void give_reward(object me, mapping quest, int shen, int extra,mixed giver,string job)
	TASK_D->give_reward(apper, apper->query_temp("quest"), 1, job_time,this_object(),"丐帮");
	
	
	apper->clear_condition("job_busy");
	apper->clear_condition("gb_busy");
        apper->set("job_name", "丐帮");
        this_object()->start_busy(random(2));    
        apper->delete_temp("gb_job");
        pot= apper->query_temp("quest/pot_reward");
        exp= apper->query_temp("quest/exp_reward");
        apper->delete_temp("quest");
	log_file("job/gaibang",sprintf("%8s%-10s第%5d次丐帮任务得%3d点经验，%2d点潜能，经验：%d\n",
		apper->name(),"("+apper->query("id")+")",apper->query("job_time/丐帮"),exp,pot,apper->query("combat_exp")),apper);
        return;
}

void attempt_apprentice(object ob)
{
        if (ob->query("family/family_name")
        && ob->query("family/family_name") != "丐帮") {
                command("say 你还是去别的门派看看吧。");
                return;
        }
        if ((int)ob->query("gb/bags") >= 2
        && ob->query("family/family_name") != "丐帮" ) {
                command("say 丐帮可容不下你这种背信弃义的小人！");
                command("kill "+ob->query("id"));
                return;
        }

	if( ob->query_skill("huntian-qigong", 1) < 50){
		command("say "+RANK_D->query_respect(ob) +"的混天气功是不是还不够？");
		return;
	}

        command("say 好吧，希望" + RANK_D->query_respect(ob) +
        "能好好学习本门武功，将来在江湖中闯出一番作为。");
        command("recruit " + ob->query("id"));

        if (!ob->query("gb/bags")) ob->set("gb/bags", 1);
        ob->set("title",sprintf("丐帮大义分舵%s袋弟子",
                chinese_number(ob->query("gb/bags"))) );
        ob->set("gb/fenduo","大义分舵");
        ob->set("class","beggar");
}

void killing_object(object who)
{
        object me = this_object();
        if(!who || environment(who)!=environment(me)) return;
        me->kill_ob(who);
        who->unconcious();
}

string ask_fangqi()
{
        object me;
        int exp, i;

        me = this_player();
        exp = me->query("combat_exp");
        if(!me->query("gb_pass"))
              return "你非我丐帮英雄,跑这里瞎嚷嚷什麽?";
        if(!me->query_temp("quest") || me->query_temp("quest/family") != "丐帮")
              return "你没有领任务,跑这里瞎嚷嚷什麽?";
        if(me->query("job_time/丐帮") < 1 ){
              me->set("job_time/丐帮", 0);
              return "你从没有好好完成过任务，跑这里瞎嚷嚷什麽？";
        }
        me->delete_temp("gb_wait_job");

	i = 30 - me->query_condition("gb_busy");
	if ( i >= 9 )
		me->apply_condition("job_busy", 1);
	else
		me->apply_condition("job_busy", 9 - i);

	me->clear_condition("gb_busy");
        me->add("combat_exp",-(30+random(40)));
        me->start_busy(random(2));
        me->delete_temp("quest");
        me->delete_temp("gb_job");
        return "既然你干不了也没关系,再去刻苦练功吧,以后再来为丐帮出力!";
}

void destroy(object obj)
{
        if (obj) destruct(obj);
}

string ask_gonglao(object who)
{
        object me;
        int i;

        me = this_player();
        if(!intp(i = me->query("job_time/丐帮")))
               return "你没为我丐帮出过任何力，跑来问什么功劳呢?";
        message_vision(CYN"$N掏出一本册子翻了翻，指着上面的墨迹对$n说道，你目前已记功" + chinese_number(i) + "次。\n"NOR,who,me);
        return "我丐帮向来赏罚分明，继续努力吧！随手将功劳簿揣入怀中。";
}

//以下是新增quest
int ask_jiuyuan()
{
	object room,me = this_player();
	object *obj;
	int x;
 	if(me->query_condition("killer"))
	{
	   command("fear "+me->query("id"));
	   command("say "+RANK_D->query_respect(me) +"还是小心通缉吧！");
	   return 1;
	}
	if(me->query(QUESTDIR3+"bad") )
	{
		command("heng");
		command("say 难道不是当年强迫萧峰走出中原，叫我如何相信你？");
		return 1;
	}
	if(me->query(QUESTDIR3+"good") && !me->query_temp(QUESTDIR4+"dagouzhen")&& !me->query(QUESTDIR4+"start"))
	{
		if(!me->query_temp(QUESTDIR4+"askwu"))
		{
			message_vision(HIC"你急声道：江湖传言，萧峰为避免南征之战，拒绝可汗命令，现被投入铁牢之中。\n"NOR,me);
			command("ah "+me->query("id"));
		}
		//增加时间和经验间隔限制
		//时间一天，经验500K
		if(!me->query_temp(QUESTDIR4+"shaolinzhen") && me->query(QUESTDIR+"time")&&time()-me->query(QUESTDIR+"time")<86400)
		{
			command("say 今天很忙了，"+RANK_D->query_respect(me) +"先行休息去吧！");
			return 1;
		}
		if(!me->query_temp(QUESTDIR4+"shaolinzhen") && me->query(QUESTDIR+"combat_exp")&&me->query("combat_exp")-me->query(QUESTDIR+"combat_exp")<500000)
		{
			command("say 以"+RANK_D->query_respect(me) +"目前的经验就算目前有情况，也帮不上什么忙，还是抓紧练功吧！");
			return 1;
		}
		//只能一个人做
		obj = users();
		x = sizeof(obj);
		while(x--) {
			if ( obj[x]->query_temp(QUESTDIR4+"askwu") && obj[x]!=me) 
			{
				command("nod "+me->query("id"));
				command("say 我也听说这事情，已经有"+obj[x]->query("name")+RANK_D->query_respect(obj[x]) +"前往大辽营救去了，"+obj[x]->query("name")+RANK_D->query_respect(obj[x]) +"武功高强，料想也无大碍！");	
				return 1;
			}
		}
               
		if(!me->query_temp(QUESTDIR4+"askwu"))
		{
			command("whisper "+me->query("id")+" 如果当真，丐帮自当尽力，当年丐帮与萧家误会颇多，这也是我中原武林赎罪的一次良机。\n"NOR);
			command("whisper "+me->query("id")+" 只是，你又如何得知这一消息？"NOR);
			message_vision(HIY"$N朗声说道：“我刚从大辽归来，也罢，依照江湖规矩，还是武功上考量一下吧。”。\n"NOR,me);
			command("nod "+me->query("id"));
		}
		else command("whisper "+me->query("id")+" 可是失败了，也罢，再给你一次机会。\n"NOR);
		command("whisper "+me->query("id")+" 你且随我进这打狗阵法之中。");
		if (!(room = find_object("/d/gb/dagouzhen")))
			room = load_object("/d/gb/dagouzhen");
		if(!room)
		{ 
			tell_object(me,HIR"\n赶快找wiz吧，竟然房间不存在!!!\n");
			log_file("quest/TLBB", sprintf("%s(%s)营救萧峰篇打狗阵法文件缺少！\n", me->name(1),me->query("id")) );
		}
		else
		{
			tell_room(environment(me),HIC"$N随吴长老匆匆出去了。\n"NOR, ({ me }));                       
			tell_object(me,HIR"你随吴长老来到一个小屋之中，却见四端分别站着一个丐帮弟子。\n");
			me->move(room);
			this_object()->move(room);
			"/cmds/std/look.c"->look_room(me,environment(me));  
			tell_object(me,HIY"\n吴长老向你挥了挥手，退了出去。\n"NOR);
			this_object()->move("/d/fuzhou/fenduo1");
			tell_room(environment(this_object()),HIC"吴长老匆匆回来了，神色很是慌张。\n"NOR, ({ this_object()}));                       
			log_file("quest/TLBB", sprintf("%s(%s)营救萧峰篇进入丐帮阵法。经验：%d。\n", me->name(1),me->query("id"), me->query("combat_exp")) ); 
			//设定标志
			me->set(QUESTDIR+"time",time());
			me->set(QUESTDIR+"combat_exp",me->query("combat_exp"));
			me->set_temp("quest/busy",1);//与任务系统冲突标志
			me->set_temp(QUESTDIR5+"askwu",1);
		}
		return 1;
	}

	if(me->query(QUESTDIR3+"good")&& !me->query(QUESTDIR4+"over") &&(me->query(QUESTDIR4+"start")|| me->query_temp(QUESTDIR5+"dagouzhen")))
	{
		command("addoil "+me->query("id"));
		command("say "+RANK_D->query_respect(me) +"却是了得。吴某佩服。还请"+RANK_D->query_respect(me) +"主持大局。");
		return 1;
	}	
	if(me->query(QUESTDIR3+"good") &&me->query(QUESTDIR4+"over"))
	{
		command("admire "+me->query("id"));
		return 1;
	}	
	if(random(2)) command("@@ "+me->query("id"));
	else command("laugh "+me->query("id"));
	return 1;
}
int ask_xiaofeng()
{
	object me = this_player();
	object *obj;
	int x;
 
 	if(me->query_condition("killer"))
	{
	   command("fear "+me->query("id"));
	   command("say "+RANK_D->query_respect(me) +"还是小心通缉吧！");
	   return 1;
	}
	if(me->query(QUESTDIR2+"over") && !me->query(QUESTDIR3+"over") )
	{
		if(!me->query(QUESTDIR3+"start"))
		{
			if(!me->query_temp(QUESTDIR3+"ask"))
			{
				command("sigh");
				message_vision(HIC"$N沉默了一会了，道：“当时在竹林那时，丐帮其他长老都在场，你可以找其他人问问吧。”\n"NOR,this_object());
				return 1;
			}
			if(!me->query_temp(QUESTDIR3+"ask_chen"))
			{
				command("sigh");
				message_vision(HIC"$N沉默了一会了，道：“当时在竹林那时，陈长老也在，你可以找他问问吧。”\n"NOR,this_object());
				return 1;
			}
			if(!me->query_temp(QUESTDIR3+"ask_song"))
			{
				command("sigh");
				message_vision(HIC"$N沉默了一会了，道：“当时在竹林那时，宋长老都在场，你可以找他问问吧。”\n"NOR,this_object());
				return 1;
			}
			if(!me->query_temp(QUESTDIR3+"ask_liang"))
			{
				command("sigh");
				message_vision(HIC"$N沉默了一会了，道：“当时在竹林那时，梁长老都在场，你可以找他问问吧。”\n"NOR,this_object());
				return 1;
			}
			if(!me->query_temp(QUESTDIR3+"ask_bai"))
			{
				command("sigh");
				message_vision(HIC"$N沉默了一会了，道：“当时在竹林那时，白长老都在场，你可以找他问问吧。”\n"NOR,this_object());
				return 1;
			}
		}
		if(me->query_temp(QUESTDIR3+"start"))
		{
			command("sigh");
			command("say 在下这就去一趟聚闲庄。"+RANK_D->query_respect(me) +"一起去吧。");
			message_vision(HIC"$N踌躇了一会了，道：“$n还是先去吧，我后手就到。”\n"NOR,this_object(),me);
			return 1;
		}
	  //增加时间和经验间隔限制
  	  //时间一天，经验500K
      message_vision(HIC"$N脸上露出一幅咬牙切齿的样子。\n"NOR,this_object());
	  command("heng");
	  command("say 萧峰是契丹狗种，还是堂堂汉人，此时还未分明。倘若他真是契丹胡虏，我吴某第一个跟他拚了。"NOR);
      if(me->query_temp(QUESTDIR3+"start"))
	  {
		  command("say 听说雄豪已经集聚南阳聚闲庄，还听说那萧峰竟然也去。萧峰向来行事稳重，这次实在难以估量。"NOR);
		  return 1;
	  }
  	  if(me->query(QUESTDIR+"time")&&time()-me->query(QUESTDIR+"time")<86400)
  	  {
  	  	command("say 不过今天很忙了，"+RANK_D->query_respect(me) +"先行休息去吧！");
  	    return 1;
  	  }
  	  if(me->query(QUESTDIR+"combat_exp")&&me->query("combat_exp")-me->query(QUESTDIR+"combat_exp")<500000)
  	  {
  	  	command("say 不过，以"+RANK_D->query_respect(me) +"目前的经验就算目前有情况，也帮不上什么忙，还是抓紧练功吧！");
  	  	return 1;
  	  }
	  //只能一个人做
	  command("say 听说雄豪已经集聚南阳聚闲庄，还听说那萧峰竟然也去。萧峰向来行事稳重，这次实在难以估量。"NOR);
	  obj = users();
	  x = sizeof(obj);
	  while(x--) {
		  if ( obj[x]->query_temp(QUESTDIR3+"start") && obj[x]!=me) 
		  {
			  command("hehe");
			  command("say 不过，已经有"+obj[x]->query("name")+RANK_D->query_respect(obj[x]) +"这样的高手去了，"+obj[x]->query("name")+RANK_D->query_respect(obj[x]) +"武功高强，这次萧峰肯定跑不了了！");	
			  return 1;
		  }
	  }
	  
	  command("say 在下这就去一趟聚闲庄。"+RANK_D->query_respect(me) +"一起去吧。");
	  log_file("quest/TLBB", sprintf("%s(%s)准备进聚闲庄，特此纪录。经验：%d。\n", me->name(1),me->query("id"), me->query("combat_exp")) ); 
  	  //设定标志
  	  me->set(QUESTDIR+"time",time());
  	  me->set(QUESTDIR+"combat_exp",me->query("combat_exp"));    
	  me->set(QUESTDIR3+"start",1);
	  me->set_temp("quest/busy",1);//与任务系统冲突标志
	  me->set_temp(QUESTDIR3+"start",1);
	  return 1;
	}	

	if(me->query(QUESTDIR3+"over"))
	{
		command("sigh");
		command("say 那萧峰要是还是我们帮主该多好......"NOR);
		return 1;
	}	
	message_vision(HIC"$N沉默了一会了，道：“萧峰正是我丐帮中人。”\n"NOR,this_object());
	return 1;
}
int ask_jingbian()
{
	object me = this_player();
	if(me->query(QUESTDIR2+"over") && !me->query(QUESTDIR3+"start"))
	{
		if(!me->query_temp(QUESTDIR3+"ask"))
		{
			command("sigh");
	    message_vision(HIC"$N沉默了一会了，道：“当时在竹林那时，丐帮其他长老都在场，你可以找其他人问问吧。”\n"NOR,this_object());
			return 1;
		}
		if(!me->query_temp(QUESTDIR3+"ask_chen"))
		{
			command("sigh");
	    message_vision(HIC"$N沉默了一会了，道：“当时在竹林那时，陈长老也在，你可以找他问问吧。”\n"NOR,this_object());
			return 1;
		}
		if(!me->query_temp(QUESTDIR3+"ask_song"))
		{
			command("sigh");
	    message_vision(HIC"$N沉默了一会了，道：“当时在竹林那时，宋长老都在场，你可以找他问问吧。”\n"NOR,this_object());
			return 1;
		}
		if(!me->query_temp(QUESTDIR3+"ask_liang"))
		{
			command("sigh");
	    message_vision(HIC"$N沉默了一会了，道：“当时在竹林那时，梁长老都在场，你可以找他问问吧。”\n"NOR,this_object());
			return 1;
		}
		if(!me->query_temp(QUESTDIR3+"ask_bai"))
		{
			command("sigh");
	    message_vision(HIC"$N沉默了一会了，道：“当时在竹林那时，白长老都在场，你可以找他问问吧。”\n"NOR,this_object());
			return 1;
		}
		message_vision(HIC"$N沉默了一会了，道：“当时在竹林那时，丐帮四大长老都在场，也基本就是这些了。”\n"NOR,this_object());
		return 1;
	}	
	if(me->query(QUESTDIR3+"start") &&!me->query(QUESTDIR3+"over"))
	{
		message_vision(HIC"$N脸上露出一幅咬牙切齿的样子。\n"NOR,this_object());
		command("say 萧峰是契丹狗种，还是堂堂汉人，此时还未分明。倘若他真是契丹胡虏，我吴某第一个跟他拚了。。"NOR);
		return 1;
	}	
	if(me->query(QUESTDIR3+"over"))
	{
		command("sigh");
		command("say 那萧峰要是还是我们帮主该多好......"NOR);
		return 1;
	}	
	command("? "+me->query("id"));
	return 1;
}
