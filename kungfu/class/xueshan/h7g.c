// h7g.c 洪七公
// Looklove@SJ 2000/8/25
// Looklove Modify at 2001/1/19
// Looklove Modify at 2001/4/7
// Looklove Modify at 2001/5/3
// River@SJ For 玉笛谁家听落梅 job 3.29.2002

inherit NPC;
#include <job_mul.h>
#include <ansi.h>
#include <get_place.h>
#include "h7g.h";

int ask_up();
int pass_chuanwei(object me, object ob);
string ask_job();
string ask_finish();
string ask_fangqi();

string *no_kill_list = ({
	"mu ren", "jin ren", "yin ren", "zhong shentong", "zhu cong", "shi ren", "huo du","he shiwo"
});

int filldata(object obj1)
{
	if(wiz_level(obj1))
		return 0;
	if(!environment(obj1))
		return 0;
	else return 1;
}

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
	 || strsrch(room_name = file_name(room),"/d/") != 0
	 || strsrch(room_name, "/d/wizard/") == 0
	 || strsrch(room_name, "/d/group/") == 0
	 || strsrch(room_name, "/d/wuguan/") == 0
	 || strsrch(room_name, "/d/death/") == 0
	 || strsrch(room_name, "/d/mingjiao/lsd/") == 0
	 || strsrch(room_name, "/d/mingjiao/bhd/") == 0
	 || strsrch(room_name, "/d/gb/") == 0
	 || strsrch(room_name, "/d/shaolin/fumoquan") == 0
	 || strsrch(room_name, "/d/baituo/") == 0)
	 return 0;
	return 1;
}

void create()
{
	object ob;
	set_name("洪七公", ({"hong qigong", "hong", "qigong"}));
	set("nickname", HIY"九指神丐"NOR);
	set("gb/bags",10);
	set("gender", "男性");
	set("age", 65);
	set("per",24);
	set("long", "他就是丐帮第十七任帮主，号称“北丐”的洪七公老爷子。\n");
	set("attitude", "friendly");
	set("no_bark",1);
	set("dgb/wugou",6);

	set("str", 40);
	set("con", 35);
	set("dex", 30);
	set("int", 35);

	set("max_qi", 11000);
	set("max_jing", 7600);
	set("neili", 21000);
	set("max_neili", 18500);
	set("jiali", 150);
	set("eff_jingli", 8000);
	set("combat_exp", 15000000);

	set_skill("force", 500);
	set_skill("huntian-qigong", 500);    // 混天气功
	set_skill("strike", 380);            // 基本掌法
	set_skill("xianglong-zhang", 500);   // 降龙十八掌
	set_skill("literate", 200);           // 读写
	set_skill("dodge", 500);             // 基本躲闪
	set_skill("xiaoyaoyou", 500);        // 逍遥游
	set_skill("parry", 500);             // 基本招架
	set_skill("stick", 500);             // 基本棍杖
	set_skill("dagou-bang", 500);        // 打狗棒法
	set_skill("begging", 200);           // 叫化绝活
	set_skill("checking", 200);          // 道听途说
	set_skill("bangjue", 200);            // 棒决
	set_skill("dagou-zhen", 200);	     // 打狗阵
	set_skill("lianhua-zhang", 500);      // 莲花掌
	set_skill("stealing", 200);           // hehe

	map_skill("force", "huntian-qigong");
	map_skill("strike", "xianglong-zhang");
	map_skill("dodge", "xiaoyaoyou");
	map_skill("parry", "dagou-bang");
	map_skill("stick", "dagou-bang");
	prepare_skill("strike", "xianglong-zhang");

	set("chat_chance_combat", 90);
	set("chat_msg_combat", ({
		(: perform_action, "strike.xiao" :),
        	(: perform_action, "strike.paiyun" :),
        	(: perform_action, "stick.chan" :),
		(: perform_action, "stick.zhuan" :),
        	(: exert_function, "huntian" :),
        	(: exert_function, "shentong" :),
	}));
	set("chat_chance", 1);
	set("chat_msg", ({
		CYN"洪七公叹了口气道：“唉……何时能再吃到蓉儿做的「叫化鸡」啊……”\n"NOR,
		CYN"洪七公说道：“老毒物的功夫日见精进，看来我得想点办法对付他。”\n"NOR,
		CYN"洪七公喃喃道：“我们丐帮的「降龙十八掌」是天下最霸道的掌法。”\n"NOR,
	}));

	set("inquiry", ([
		"老毒物" : "老毒物就是我的死对头，江湖上人称西毒的欧阳峰。\n",
		"欧阳峰" : "他是我的死对头，江湖上人称西毒。\n",
        	"西毒" : "这个...这个...\n",
        	"郭靖" : "那是我的好徒儿，就是人笨了点。\n",
        	"黄蓉" : "那小丫头可是个机灵鬼，菜做的可真不赖。\n",
        	"黄药师" : "他啊，够博学多才，不过我还是更喜欢他那个小丫头。\n",
        	"东邪" : "他啊，够博学多才，不过我还是更喜欢他那个小丫头。\n",
        	"南帝" : "大理皇族，不敢高攀。\n",
        	"王重阳" : "那可是个武学奇才，老叫化可不想和他打架。\n",
        	"中神通" : "江湖上现在很多人自命不凡，都以为自己是王重阳，哈哈！\n",
        	"丐帮" : "连我丐帮都不知道，我看你有点秀斗了！\n",
        	"打狗棒" : "这个...这个...你想如何？\n",
        	"降龙掌" : "没事别乱打听，小心我一掌拍死你。\n",
        	"莲花掌" : "没事别乱打听，小心我一掌拍死你。\n",
        	"rumors" : "最近没听说什么消息。\n",
		"亢龙有悔" : "没事别乱打听，小心我一掌拍死你。\n",
        	"叫化鸡" : (: ask_ji :),
        	"天下无狗" : (: ask_wugou :),
        	"传位":    (: ask_up :),
        	"玉笛谁家听落梅": (: ask_job :),
        	"job" : (: ask_job :),
        	"finish": (: ask_finish :),
        	"完成" : (: ask_finish :),
        	"放弃" : (: ask_fangqi :),
        	"fangqi" : (: ask_fangqi :),        	
	]));

	create_family("丐帮", 17, "帮主");
	set_temp("apply/damage", 70);
	set_temp("apply/dodge", 80);
	set_temp("apply/attack", 80);
	set_temp("apply/armor", 120);
	setup();

	if (clonep()) {		
		object *ob_list = filter_array(users(), (: filldata :));
		for (int i=0;i<sizeof(ob_list);i++){
			if (ob_list[i]->query_temp("gb_chuanwei") ){
				set("long", "他就是丐帮的前帮主，号称“北丐”的洪七公老爷子。\n");
				set("title",CYN"丐帮前帮主"NOR);
				break;
			}
		}
		ob = unew(BINGQI_D("yuzhu_bang"));
		if (!ob) ob = unew(BINGQI_D("zhubang"));
		ob->move(this_object());
		ob->wield();
		carry_object(ARMOR_D("bainayi"))->wear();
		carry_object(ARMOR_D("shoes"))->wear();
		carry_object(__DIR__"obj/gb_budai10")->wear();
	}
}

void reset()
{	
	if (clonep() ) {
		int i = 0;
		object *ob_list = filter_array(users(), (: filldata :));
		object ob = unew(BINGQI_D("yuzhu_bang"));
		for (i=0;i<sizeof(ob_list);i++){
			if (ob_list[i]->query_temp("gb_chuanwei") ){
				set("long", "他就是丐帮的前帮主，号称“北丐”的洪七公老爷子。\n");
				set("title",CYN"丐帮前帮主"NOR);
				break;
			}
		}
		if(i==sizeof(ob_list))
		{
			set("long", "他就是丐帮第十七任帮主，号称“北丐”的洪七公老爷子。\n");
			delete("title");
		}
		if(ob){
			ob->move(this_object());
			command("unwield all");
			command("wield yuzhu bang");
		}		
	}
}

void attempt_apprentice(object ob)
{
	if ( ob->query("job_time/丐帮",1) < 400 && ob->query("job_time/抓蛇") < 400){
		command("say  做我的弟子要为丐帮出生入死，" + RANK_D->query_respect(ob) + "在帮中的积功似乎还少了一些。\n");
		return;
	}
	if ( !ob->query("gb/bags") ) {
		command("say " + RANK_D->query_respect(ob) + "你身上一袋也没有，不是我们丐帮弟子吧？\n");
		return;
	}
	if ( ob->query("gb/bags")<6) {
		command("say " + RANK_D->query_respect(ob) + "在帮中职位太低，先升到六袋吧。\n");
		return;
    	}
	command("chat 哈哈哈，老叫化入土之前还能找到"+ob->name()+"这样一个可造之才,丐帮后继有人了。\n");
	command("recruit " + ob->query("id"));
	ob->set("class", "beggar");
	ob->set("gb/fenduo","总舵");

	if (!ob->query("gb/baih7g")){
		ob->set("gb/baih7g",ob->query("combat_exp"));
		ob->set("title",HIY"丐帮洪七公亲传弟子"NOR);
	}
	return;
}

int ask_up()
{
	object obj1, me=this_player(),ob=this_object();
	int max, i, skill, bag = me->query("gb/bags");
	mixed *ob_list;

	skill = me->query_skill("xianglong-zhang",1);
	
	if( me->query_temp("gb_chuanwei") ){
		command("say " + RANK_D->query_respect(me) + "已经是丐帮帮主，何苦又来找我开玩笑。\n");
		return 1;
	}	
	if( !me->query("gb/bags")){
		command("say " + RANK_D->query_respect(me) + "与我丐帮素无来往，此话不知从何说起。\n");
		return 1;
	}
	if( bag < 9 ){
		command("say 你在帮中地位太低，传位给你恐怕无法说服众弟子。\n");
		return 1;
	}
	if ( (me->query("job_time/丐帮") + me->query("job_time/抓蛇")) < 2000){
		command("say " + RANK_D->query_respect(me) + "在帮中的积功似乎还少了一些，恐怕难以折服众弟子。\n");
		return 1;
	}
	if( me->query("shen") < 1500000){
		command("say " + RANK_D->query_respect(me) + "的侠义正气，恐怕无力领导丐帮数万弟子。\n");
		return 1;
	}
	
	if(mapp(me->query("job_time")))
		me->set("job_time/报效国家",me->query("job_time/草料场")+me->query("job_time/刺杀")+me->query("job_time/颂摩崖"));
	
	if( me->query("job_time/报效国家") < 500) {
		command("say 我丐帮乃堂堂名门正派，"+RANK_D->query_respect(me)
		+"应该为国为民多做点贡献才好。");
		return 1;
	}		
	if( me->query_skill("dagou-bang",1) < 280 || me->query_skill("xianglong-zhang",1) < 280 ){
		command("say 打狗棒法和降龙十八掌乃我丐帮祖传绝技，" + RANK_D->query_respect(me) + "在这方面的修为是否稍差了一点。\n");
		return 1;
	}
	if( !present("yuzhu bang",ob) )	{
		command("say 我的玉竹棒跑哪里去了？糟了，这下可没办法传位了。");
		return 1;
	}

	ob_list = users();
	ob_list = filter_array(ob_list, (: filldata :));
	max=sizeof(ob_list);
	for (i=0;i<max;i++){
		obj1 = ob_list[i];
		if (obj1->query_temp("gb_chuanwei") ){
			command( "say 我已经把帮主之位传给" + obj1->query("name") +"了。\n" ) ;
			return 1;
		}
	}
	command("look "+me->query("id"));
	command( "say 欲受我衣钵，先考考你的武功。" );

	if ( !pass_chuanwei(me, ob ) ) return 1;
	if ( !pass_chuanwei(me, ob ) ) return 1;
	if ( !pass_chuanwei(me, ob ) ) return 1;
	if ( !pass_chuanwei(me, ob ) ) return 1;
	if ( !pass_chuanwei(me, ob ) ) return 1;
	if ( !pass_chuanwei(me, ob ) ) return 1;
	if ( !pass_chuanwei(me, ob ) ) return 1;

	me->set_temp("title", "丐帮第十八代帮主");

	command("chat 老叫化今日将丐帮帮主之位传给"+me->query("name")+"，哈哈，丐帮后继有人了。");
	message_vision("洪七公交给$N一根玉竹棒。\n",me);
	me->set_temp("gb_chuanwei",1);
	me->add_temp("apply/strength", random(skill/10));//add up bl ciwei
	present("yuzhu bang", ob)->move(me);
	
	ob->set("long", "他就是丐帮的前帮主，号称“北丐”的洪七公老爷子。\n");
	ob->set("title",CYN"丐帮前帮主"NOR);
	return 1;
}

int pass_chuanwei(object me, object ob)
{
	COMBAT_D->do_attack(ob, me, ob->query_temp("weapon"), 1);

	if (!present(me,environment(ob)) || me->query("qi") <= 0
	|| me->query("jing") <= 0 || me->query("jingli") <= 0){
		command("say 武功如此之差，如何传我衣钵？！");
		ob->reincarnate();
		return 0;
	}
	return 1;
}

string ask_job()
{
	object me, ob, *living, target;
	string str, food, food_id_list;
	string *food_name = ({"羊羔坐臀","小猪耳朵","小牛腰子","獐腿肉","兔肉"});
	string *food_id = ({"yanggao zuotun", "xiaozhu erduo", "xiaoniu yaozi", "zhangtui rou", "tu rou"});
	string *food_type;
	object objwhere, obj;
	mapping food_type_list;
	mapping skills;
	string *sk;
	int i, j, x, y, z, p;

	me = this_player();

	if (me->query("menggu"))
		return "哼，你已身属蒙古，还想做什么？";

	if (me->query_temp("potjob/job") && !wizardp(me))
		return "嗯？我不是告诉你了吗，快去取原料啊，不想做就算了！";
/*
	if (me->query("job_name") == "" && !wizardp(me))
		return "我现在不饿，你还是先去休息一会吧。";
*/
	if (me->query("combat_exp") < 100000 )
		return "这位" + RANK_D->query_respect(me) + "武功未成，让你去江湖闯荡，老叫化子我可不太放心。";

	skills = me->query_skills();
	if (mapp(skills)){
		sk = keys(skills);
		for (i=0; i < sizeof(sk); i++)
			if (skills[sk[i]] > (me->query("max_pot") - 90) && sk[i] != "beauty" && SKILL_D(sk[i])->type2() != "worker")
//			if (skills[sk[i]] > (me->query("max_pot") - 90) && SKILL_D(sk[i])->type() != "knowledge" && SKILL_D(sk[i])->type2() != "worker")
				return "你的"+to_chinese(sk[i])+"等级够高了，无法再做这个任务了。";
	}

	if ( me->query("potential") >= me->query("max_pot") && !wizardp(me))
		return "这位" + RANK_D->query_respect(me) + "的潜能已经这么多了，还是先去用完再来吧！";

	if ( me->query_condition("job_busy") || me->query_condition("pot_job"))
		return "我现在不饿，你还是先去休息一会吧。";

	me->delete_temp("potjob");
	command("say 这「玉笛谁家听落梅」是黄蓉这个小丫头片子做给老叫花子吃的一道菜，\n"+
		"还骗得老叫花子教了郭靖那个傻小子一套降龙十八掌。这道菜说起来也简单，就是由四条\n"+
		"小肉条拼成，一条是羊羔坐臀，一条是小猪耳朵，一条是小牛腰子，还有一条是獐腿肉加\n"+
		"兔肉揉在一起。不说了，不说了，口水都出来了。");

	command("taste");

	for(z=0; z < 5; z ++){
		me->set_temp("potjob/food_type/"+ food_name[z], food_id[z]);
	}

	food_type_list = me->query_temp("potjob/food_type");

	y = random(4) + 2;
	for (x = 0; x < y; x ++){
		living = livings();
		for (i=0;i < sizeof(living);i++){
			j = random(sizeof(living));
			if (!j || j == sizeof(living) -1) continue;
			if(check(living[j]) ){
				target = living[j];
				objwhere = environment(target);
				str = objwhere->query("short");
				if (!sizeof(objwhere->query("exits"))
				|| str == "武僧堂"
				|| str == HIR"心禅堂"NOR
				|| str == "僧舍"
				|| str == "休息室"
				|| str == "龙池"
				|| str == "凤池"
				|| str == "女休息室"
				|| str == "男休息室"
				|| str == "金刚伏魔圈" ) continue;
				if( get_place(base_name(environment(target)))!="" ) break;
			}
		}

		if( !target )
			return "我现在暂时还不饿，你还是先去休息一会吧。";

		p = random(sizeof(food_name));

		food = food_name[p];
		food_id_list = food_id[p];

		food_type = me->query_temp("potjob/type");

		if ( ! sizeof(food_type))
			me->set_temp("potjob/type", ({ food }));
		else {
			for(z=0; z < 5; z ++){
				if (member_array(food, food_type) != -1) {
					food = food_name[z];
					food_id_list = food_id[z];
				}
			}
			food_type = sort_array(food_type + ({ food }), 0);
			me->set_temp("potjob/type", food_type );
		}
		ob = new(__DIR__"pot_target");
		ob->set_temp("target", me);
		ob->set_temp("food_name", food);
		ob->set_temp("food_id", food_id_list);
		ob->move(objwhere);
		for (i=0; i<5; i++)
			ob->random_move();
		obj = new(__DIR__"obj/pot_food");
		obj->set_name(food, ({ food_id_list }));
		obj->set("long", "这是一条"+obj->query("name")+"，看起来味道不错。\n");
		obj->set_temp("food_id", food_id_list);
		obj->move(ob);
		if (me->query_temp("potjob/food_type/"+food))
			me->delete_temp("potjob/food_type/"+food);
		if (strsrch(file_name(environment(ob)), "/d/zhiye/") >= 0
		 || strsrch(file_name(environment(ob)), "/d/group/") >= 0){
		 	if ( environment(ob)->query("outdoors"))
				me->set_temp("potjob/pot_job_place/"+ ob->query("name"), environment(ob)->query("outdoors")+environment(ob)->query("short"));
		 	else if ( environment(ob)->query("indoors"))
				me->set_temp("potjob/pot_job_place/"+ ob->query("name"), environment(ob)->query("indoors")+environment(ob)->query("short"));
			else me->set_temp("potjob/pot_job_place/"+ ob->query("name"), environment(ob)->query("short"));
		}
		else me->set_temp("potjob/pot_job_place/"+ ob->query("name"), get_place(base_name(environment(ob)))+environment(ob)->query("short") );
		me->set_temp("potjob/for_menu/"+ ob->name(), food);
		command("whisper "+ me->query("id")+" 听说"+ me->query_temp("potjob/pot_job_place/"+ob->query("name")) +GRN"的"+ob->name()+"手中有"+food+"，你帮我去找来吧！");
	}

	me->set_temp("potjob/do_time", y);
	me->set_temp("potjob/time", uptime());
	me->set_temp("potjob/job",1);
	me->apply_condition("pot_job", 6);
	me->apply_condition("job_busy", 6);

	if ( !present("hong's menu", me)){
		new(__DIR__"obj/pot_paper")->move(me);
		message_vision("$N给了$n一本关于如何制作「玉笛谁家听落梅」的菜谱。\n", this_object(), me);
	}

	p = sizeof(keys(food_type_list));

	if ( p > 0 ){
		for(z=0; z < p; z ++){
			food = keys(food_type_list)[z];
			food_id_list = values(food_type_list)[z];
			obj = new(__DIR__"obj/pot_food");
			obj->set_name(food, ({ food_id_list }));
			obj->set("long", "这是一条"+obj->query("name")+"，看起来味道不错。\n");
			obj->set_temp("food_id", food_id_list);
			obj->set_temp("job_id", me);
			obj->move(me);
			me->set_temp("potjob/be_get/"+food, 1);
		}
		return "我这里已经有些原料，先给你，其余的你去找吧！";
	}
	else
		return "唉！我这里什么原料都没有，你速速帮我找来吧。";
}

string ask_finish()
{
	object me = this_player();
	object ob;
	int pot, i, times;

	if ( !me->query_temp("potjob/job"))
		return "我有叫你去做什么吗？你完成什么啊？";

	if ( !me->query_temp("potjob/finish"))
		return "你真的完成了？？";

	if ( ! ob =(present("yudi luomei", me)))
		return "你完成了，那么东西呢？";

	destruct(ob);
	message_vision(HIY"$N接过$n做的「玉笛谁家听落梅」，口中啧啧声不断，大笑道：今日可大饱口福了。\n"NOR, this_object(), me);

	command("pat "+ me->query("id"));
	i = me->query_temp("potjob/do_time");
	pot = 50 + i * 100 + random(50);
// 以后去除
	if( me->query("max_neili") < 3500)
		pot += pot/2;
		
       	/*奖励控制开始*/
        if (me->query("registered")==3){
            		pot = pot * H7G_JOB_MUL * VIP_MUL /10000;
		}
		else
		{
           		pot = pot * H7G_JOB_MUL /100;
		}
		/*奖励控制结束*/ 


	tell_object(me, HIW"洪七公指点了你一些武学上的迷津，你获得了"+chinese_number(pot)+"点潜能。\n"NOR);
	times = uptime() - me->query_temp("potjob/time");
	me->delete_temp("potjob");
	me->add("potential", pot);
	me->add("job_time/洪七公",1);
	GIFT_D->check_count(me,this_object(),"洪七公");
	log_file("job/zuocai", sprintf("%8s%-10s 完成做菜，得到%3d点潜能，时间：%d",
		me->query("name"), "("+me->query("id")+")", pot, times), me);
	return "哈哈，下次再让你小子给老叫化子做菜。";
}

string ask_fangqi()
{
	object me = this_player();
	object meat1, meat2, meat3, meat4, meat5, meat6;
	meat1 = present("yanggao zuotun", me);
	meat2 = present("xiaozhu erduo", me);
	meat3 = present("xiaoniu yaozi", me);
	meat4 = present("zhangtui rou", me);
	meat5 = present("tu rou", me);
	meat6 = present("yudi luomei", me);

	if ( ! me->query_temp("potjob/job"))
		return "我有叫你去做什么吗？你放弃什么啊？";

	if ( meat1 ) destruct(meat1);
	if ( meat2 ) destruct(meat2);
	if ( meat3 ) destruct(meat3);
	if ( meat4 ) destruct(meat4);
	if ( meat5 ) destruct(meat5);
	if ( meat6 ) destruct(meat6);

	command("nod "+ me->query("id"));
	me->delete_temp("potjob");
	return "既然做不了，也就不勉强你了。";
}

void dest(object obj)
{
	if(!obj) return;
	destruct(obj);
}

int accept_object(object me, object obj)
{
	object ob_yzz, ob = this_object();

	if (obj->query("food_supply") <= 0) return 0;
	if ((string)obj->query("name") != "叫化鸡" || me->query_temp("marks/get-ji")){
		command("say 老叫化对" + (string)obj->query("name") + "没什么兴趣……");
		return 0;
	}
	if (query("food") >= (max_food_capacity() - obj->query("food_supply"))){
		command("say 老叫化现在饱得很。");
		return 0;
	}
	command("say 啊……这正是我想吃的叫化鸡呀！");
	command("eat ji");
	command("say 想不到你小小年纪，心思却很细，就跟当年的蓉儿一样！");
	call_out("dest", 1, obj);

	if (!me->query("gb/bags")){
		command("say 你还呆在这里干什么？");
		return 1;
	}
	if( ob_yzz = present("yuzhu bang", ob) ){
		ob_yzz->move(me);
		command("say 这根竹棒跟随我多年了，就送给你作见面礼吧。");
		return 1;
	}
	else{
		if( (time()-me->query("gb/jitime")) >= 100 ) {
			me->add("combat_exp",random(200));
			me->add("potential",random(100));
			if(me->query("potential")>me->query("max_pot"))
				me->set("potential",me->query("max_pot"));
			me->set("gb/jitime",time());
		}
		if ( me->query("combat_exp") <= 2000000 ){
			command("say 你现在功夫还太弱，等以后练好了再来找我吧。");
			return 1;
		}
		if ( random(150) == 50
		&& me->query_skill("xianglong-zhang", 1) >= 300
		&& !me->query("xlz/hang")
		&& !me->query_temp("xlz/hang"))
			call_out("kanglong", 10, me, ob);
		return 1;
	}
	return 1;
}
