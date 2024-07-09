// baoxiang.c 宝象
// by iceland

#include <job_mul.h>
#include <ansi.h>
#include <get_place.h>

inherit NPC;
inherit F_MASTER;

int check(object ob);

string ask_miji();
string ask_jiaohuan();
string ask_job();
string ask_times();
string ask_fail();
string ask_fail2();
string ask_finish();
string ask_diyun();

#include "/d/suzhou/npc/lchj.h";

void create()
{
	set_name("宝象", ({ "bao xiang", "baoxiang" ,"bao","xiang"}));
	set("long",@LONG
他是血刀老祖座下三弟子，长得肥头大耳，满面暴虐之色，
脸上油腻腻的泛着光，不象守戒修行的出家人。
LONG
	);
	set("title", HIR "大轮寺血刀老祖座下弟子" NOR);
	set("gender", "男性");
	set("class", "huanxi");
	set("age", 39);
	set("attitude", "friendly");
	set("shen_type", -1);

	set("str", 28);
	set("int", 15);
	set("con", 25);
	set("dex", 28);
	set("max_qi", 900);
	set("eff_jing", 300);
	set("max_jing",300);
	set("neili", 1100);
	set("max_neili", 1100);
	set("jiali", 30);
	set("combat_exp", 300000);
	set("score", 30000);

	set("job_npc", 1);
	set("unique", 1);
	set("no_get","宝象对你来说太重了。\n");

	set_skill("huanxi-chan", 80);
	set_skill("literate", 60);
	set_skill("force", 80);
	set_skill("longxiang-boruo", 80);
	set_skill("dodge", 80);
	set_skill("yuxue-dunxing", 80);
	set_skill("hand", 70);
	set_skill("dashou-yin", 70);
	set_skill("claw", 70);
	set_skill("tianwang-zhua", 70);
	set_skill("parry", 80 );

	map_skill("force", "longxiang-boruo");
	map_skill("dodge", "yuxue-dunxing");
	map_skill("hand", "dashou-yin");
	map_skill("parry", "tianwang-zhua");
	map_skill("claw", "tianwang-zhua");

	prepare_skill("hand","dashou-yin");
	prepare_skill("claw","tianwang-zhua");

	set("inquiry", ([
		"血刀秘籍" : (: ask_miji :),
		"交换" : (: ask_jiaohuan :),
		"血刀老祖" : "那是我师父，奸淫捋掠样样在行，真叫人佩服！\n",
		"老祖" : "你说的是我师父血刀老祖吧？他等闲可不容易见到。\n" ,
		"奸淫" : "嘻嘻，看你也是此道中人，有机会我们配合配合。\n",
		"小妞" : "你真是我的知音，知道我兴趣所在，有什么消息我们交流交流？\n",
// xueshan job move to baoxiang 2003.5.12
		"工作":		(: ask_job :),
		"job" :		(: ask_job :),
		"次数":		(: ask_times :),
		"供奉":		(: ask_times :),
		"放弃" :	(: ask_fail :),
		"fangqi":	(: ask_fail :),
		"失败" :	(: ask_fail2 :),
		"shibai":	(: ask_fail2 :),
		"完成":		(: ask_finish :),
		"finish":	(: ask_finish :),
// add quest by tangfeng
		"狄云" : (: ask_diyun :),
		"神照经" : random(2)?"听说是铁骨墨萼梅念笙的内功心法。":"我记得在武馆之内就刻有神照经内功心法。",
		"万震山" : "梅念笙老的弟子。",
		"言达平" : "梅念笙老的弟子。",
		"戚长发" : "梅念笙老的弟子。",
		"梅念笙" : "铁骨墨萼梅念笙，可惜死得冤枉啊。",
	]));
	set("miji_count", 1);

	create_family("大轮寺", 12, "弟子");
	set("class", "huanxi");

	setup();
	carry_object("/d/xueshan/npc/obj/gongde-jiasha")->wear();

	add_money("silver",10);
}

void heart_beat()
{
	::heart_beat();

	clear_conditions_by_type("poison");
	if( !is_fighting() && living(this_object()))
		reincarnate();
}
string ask_diyun()
{
	object me,ob;
	me = this_player();
	ob = this_object();
	if(me->query_condition("killer"))
	{
	   command("fear "+me->query("id"));
	   return ""+RANK_D->query_respect(me) +"还是自己小心通缉吧！";
	}
	if(me->query(QUESTDIR1+"over" ))
	{
	   command("claw "+me->query("id"));
	   return "那个癞痢头狄小子竟然能够打败中原四虾米和我师傅，怎么突然这么厉害了？";
	}
	if(!me->query(QUESTDIR1+"start" ))
	{
	   command("? "+me->query("id"));
	   command("say");
	   return "狄云是谁？";
	}
	if(!me->query_temp(QUESTDIR1+"start" ))
	{
	   command("say");
	   command("claw "+me->query("id"));
	   return "狄云是谁？";
	}
	if(me->query_temp(QUESTDIR1+"askbaoxiang"))
	{
	   command("en");
	   message_vision(HIY"$N脸上露出古怪的神色。\n", ob);
	   return "那个癞痢头狄小子现在应该在雪山雪谷一带！";
	}
	command("sigh");
	message_vision(HIY"$N脸上露出气愤的神色。\n", ob);
	me->set_temp(QUESTDIR1+"askbaoxiang",1);
	return "你说那个癞痢头阿三，被我师傅抓去了，我也不知道现在在哪里！";
}
void attempt_apprentice(object ob)
{
	if ((string)ob->query("family/family_name") != "大轮寺") {
		command("say"+ RANK_D->query_respect(ob) +"不是本寺弟子，在这里捣什么乱？！");
		return;
	}
	if ((int)ob->query_skill("huanxi-chan", 1) < 50) {
		command("say 欢喜禅是各项武功之本，你再勤加修炼吧。");
		command("say"+ RANK_D->query_respect(ob) + "再熬几个通宵，多多钻研钻研本门的心法吧。");
		return;
	}
	if ((string)ob->query("class") != "huanxi") {
		message_vision("宝象看了看你头顶，又摸了摸自己的光头。",this_player());
		command("say 你还没出家，不能表示你是真心侍奉欢喜佛，我不能收你。");
		command("say 这位" + RANK_D->query_respect(ob) + "如能早下决心，献身侍奉我佛再来找我吧。");
		return;
	}
	command("say 好吧，以后你就好好侍奉佛爷我吧。");
	command("recruit " + ob->query("id"));
	ob->set("title", HIR "大轮寺血刀门第六代弟子" NOR);
}

string ask_miji()
{
	this_player()->set_temp("jiaohuan",1); 
	return  "血刀秘籍是本门一本宝书，是本门以前一位高僧笔录下来的，\n"+
		"内容主要是一套刀法，这套刀法狠辣凌厉...你问这个做什么？秘籍在我\n"+
		"这里，可我还没看完，不会给你的。\n" ;
}

string ask_jiaohuan()
{
	if ( this_player()->query_temp("jiaohuan")) {
		this_player()->delete_temp("jiaohuan");
		this_player()->set_temp("miji",1);
		return "还有什么比武功更重要的呢？你能拿什么东西和我换？";
	}
	else {
		message_vision("宝象迷惑的看了看$N。\n",this_player());
	}
}

int accept_object(object who, object ob)
{
	object obj;
        
	if( !who || environment(who) != environment() ) {return 0;}
	if ( !objectp(ob) ) {return 0; }
	if ( !present(ob, who) ) return notify_fail("你没有这件东西。");
	if (! who->query_temp("miji")) return 0;

	if ((string)ob->query("id") == "chungong tu" ){
		obj=unew(MISC_D("xuedao-miji"));
		command("ah");
		message_vision("宝象喜得眉开眼笑，手舞足蹈。\n",this_player());
		command("say 这可真是个宝贝！你从那里搞到的，太棒啦。");
		if(!clonep(obj)) {
			command("say 好，就这么换了!\n");
			remove_call_out("nothing");
			call_out("nothing", 2, who);
			remove_call_out("destroying");
			call_out("destroying", 1, this_object(), ob);
			return 1;
		}
		if(clonep(obj) && ob->violate_unique()){
			destruct(obj);
			command("say 好，就这么换了!\n");
			remove_call_out("nothing");
			call_out("nothing", 2, who);
			remove_call_out("destroying");
			call_out("destroying", 1, this_object(), ob);
			return 1;
		}
		message_vision("宝象生怕$N反悔，赶紧把春宫图揣在怀中，把血刀秘籍塞在$N手中。\n",this_player());
		obj->move(who);
		who->delete_temp("miji");
		command("say 好，就这么换了!\n");
		remove_call_out("destroying");
		call_out("destroying", 1, this_object(), ob);
		return 1;
	}
	else {
		command("say 这个就想换？你做白日梦!");
		return 0;
	}
}

void destroying(object me, object obj)
{
	if(!obj) return;
	destruct(obj);
}

void nothing(object me)
{
	if (!me) return;
	message_vision("宝象伸手在怀中左掏右掏的摸了半天，什么也没掏出来。\n", me);
	command("say 遭了！我忘了血刀秘籍已经给了别人了。");
	command("say 算你倒霉，这本春宫图嘛，就先借给我看几天吧。"); 
	command("shrug");              
}

void init()
{
	object ob;
	ob = this_player();
	if( ob->query("gender")=="女性"
	 && ob->query("family/family_name") != "大轮寺" 
	 && !ob->query("xs/girl")){
		if( ob->query_per() >= 20 ){
			ob->add_busy(1);
			call_out("meeting1", 0, ob);
		}
		else {
			ob->add_busy(1);
			call_out("meeting2", 0, ob);
		}
	}
}

void meeting1(object ob)
{
	if (ob) {
		message_vision(HIY"宝象突然跳到$N面前，拦住了去路。\n"NOR,ob);
		message_vision(HIY"宝象上下打量着$N，嘿嘿淫笑了两声。\n"NOR,ob);
		command("say 这位"+RANK_D->query_respect(ob)+"别走得这么急啊，到那里去呀？");
	}
}

void meeting2(object ob)
{
	if (ob) {
		message_vision(HIY"宝象突然跳到$N面前，拦住了去路。\n"NOR,ob);
		message_vision(HIY"宝象上下打量着$N，脸上现出失望的神色，嘴角一撇。\n"NOR,ob);
		command("say 这位"+RANK_D->query_respect(ob)+"怎么长得如此丑陋，实在令人大失所望。");
	}
}

string ask_finish()
{
	object me = this_player();
	int e,p,shen,count, exp , time,gold;
	string s;

	if( !me->query("xs/girl")) {
		command("? "+ (string)me->query("id"));
		return "你什么意思？";
	}

	if( !me->query_temp("xs/finish")) {
		command("kick "+ me->query("id"));
		command("consider "+ me->query("id"));
		return "你小子没完成，就想到老祖爷爷这里来骗吃骗喝啊？";
	}

	command("hehe");
	command("say 不错不错，今天又有事情干了，你给老祖爷爷做事，老祖爷爷不会亏待你的。");

	count = me->query("job_time/雪山",1);

	e = 280 + random(90);

	if (me->query("xs/girl_super")) {
		e = e * (110 + random(50))/ 100;
		if ( e > 500 ) e = 400 + random(100);
		s = "困难任务";
		gold = 6+random(5);
	}
	else {
		gold = 1+random(5);
		s = "任务";
	}
	time = uptime() - me->query("xs/times");

	if ( e / time > 35/18 ) {
		exp = to_int(35/18.0 * time);
		if (! me->query("xs/girl_super"))
			e = 280 + random(30);
		else
			e = 310 + random(20);
	}
	
	e = me->add_exp_combat(e,this_object(),"雪山");
	p = 25+random(10);
	
	shen = -e * 3/2;

	me->delete("xs/girl");

	me->clear_condition("job_busy");
	me->clear_condition("xs_job");
	me->delete_temp("xs/finish");
	me->apply_condition("xs_job", e/75 );

	me->delete("xs/girl_time");
	me->delete("xs/girl_ob");
	me->delete("xs/girl_super");
	me->delete("xs/cant");

	me->add_temp("xs/xs_job", 1);

	if( me->query("shen") >- 10000 && !wizardp(me)){
		me->add("shen",shen*2);
		me->set("xs/girl_busy", 0 );
		command("hehe");
		return "好样的，可惜你不是我道中人。";
	}

	if (me->query("family/family_name") != "桃花岛")
		me->add("shen",shen);	
	me->add("xs_jobe",e);
	me->add("xs_jobp",p);
	me->add("potential",p);	
	me->add("balance",gold*10000);
	me->set("job_name" , "强抢美女");

	if ( (int)me->query("potential", 1) > (int)me->query("max_pot", 1) )
		me->set("potential" , me->query("max_pot", 1) );

	if ((string)me->query("family/family_name")=="大轮寺"
	  && me->query_skill("huanxi-chan",1) < 200)
		me->improve_skill("huanxi-chan", e );

	tell_object(me, HIC"你被奖励了"+CHINESE_D->chinese_number(e)+"点经验，"+CHINESE_D->chinese_number(p)+"点潜能！你感觉邪恶之气更胜从前！\n"
		+"宝象为你在钱庄里存入"+CHINESE_D->chinese_number(gold)+"锭"+HIY+"黄金。\n"NOR);
		
	log_file( "job/xueshan",sprintf("%8s%-10s第%5d次%s，经验%3d(%3d/%4d)，潜能%2d(%s)；现exp：%d\n",
		me->name(), "("+me->query("id")+")", me->query("job_time/雪山"), s, e, exp, time, p, me->query("xs/girl_place"), me->query("combat_exp")),me);
	me->delete("xs/girl_place");
	return "好好干，老祖爷爷会喜欢你的，嘿嘿！";
}

string ask_job()
{
	object target, *living , me = this_player() , ob , beauty;
	int i,j;
	string str;

	if( me->query("combat_exp") < 100000 )
		return "你的实战经验太少了点吧，派你去我可实在不大放心！";

	if( !(wizardp(me) && me->query("env/test")) && me->query("xs/girl") && ((time()-(int)me->query("xs/girl_time"))<(12*60)) )
		return "我不是叫你到"+me->query("xs/girl_place")+"去给老祖爷爷抢美女了嘛！";

	if( !(wizardp(me) && me->query("env/test")) && me->query("job_name") == "强抢美女")
		return "你要累死你老祖爷爷啊！一边呆着去！";

	if( !(wizardp(me) && me->query("env/test")))
	if( me->query_condition("job_busy") || me->query_condition("xs_job")) {
		if ( me->query("job_name") == "强抢美女") {
		    command("kick "+(string)me->query("id"));
		    return "你要累死你老祖爷爷啊！一边呆着去！";
		}
		else
		    return "身体是革命的本钱啊，同志！你还是先歇息一会儿吧。";
	}
	me->delete("xs/girl_busy");

	if(!wizardp(me) && me->query_condition("task_busy"))
			return "身体是革命的本钱啊，同志！你还是先歇息一会儿吧。";
						
	if(me->query("shen") > -10000 && !wizardp(me)){
		return "我看你不够心狠手辣，爷爷我不喜欢。";
	}
	if(me->query_temp("quest/busy")) //added by tangfeng 与quest冲突
		return "现在我这里没有给你的任务，你还是先处理好你其他事情再说吧。";


	living = livings();
	for (i=0;i < sizeof(living);i++){
		j = random(sizeof(living));
		if (j <= 1 || j == sizeof(living)-1) continue;
		if(check(living[j]) ){
			target = living[j];
			str = environment(target)->query("short");
			if( str == "武僧堂"
			 || str == HIR"心禅堂"NOR
			 || str == "金刚伏魔圈"
			 || str == HIB"戒律院"NOR) continue;
			if( get_place(base_name(environment(target)))!="" )
				break;
		}
	}

	if( !target )
		return "我最近没听说有什么出名的美女，你过一会儿再来吧。";

	me->apply_condition("task_busy",9);
	me->apply_condition("job_busy", 40);
	me->apply_condition("xs_job", 40);
	me->set("xs/times", uptime());
	me->set("xs/girl",1);
	me->set("xs/girl_time",time());
	me->set("xs/girl_place",get_place(base_name(environment(target)))+environment(target)->query("short"));

	beauty = new("/d/xueshan/npc/gbeauty");
	beauty->set("long", beauty->query("long")+
		"看来就是血刀老祖要求"+me->query("name")+"("+capitalize(me->query("id"))+")强抢的美女。\n");
	beauty->move(environment(target));
	me->set("xs/girl_ob",beauty);
//	beauty->move("/d/xueshan/emptyroom");

	ob = new("/d/xueshan/npc/guard");
	ob->set("target", me->query("id"));
	ob->set("beauty", beauty);
	if(wizardp(me)) tell_object(me, base_name(environment(target))+"\n");
	me->set("xs/cant",0);
	if(random(15) != 1 || me->query("combat_exp") < 500000 ) {
		ob->do_copy(me,0);
		me->set("xs/girl_super",0);
	}
	else {
		ob->do_copy(me,1);
		me->set("xs/girl_super",1);
		if ( (me->query("combat_exp") > 1000000) && (random(3)==1) )
		    me->set("xs/cant",1);
	}
	beauty->set("guard", ob);
	ob->move(environment(target));
//	ob->move("/d/xueshan/emptyroom");
	beauty->set_leader(ob);

	if( !objectp( present("hehuan san", me)) ) {
		ob = new("/d/xueshan/npc/hhs");
		ob->move(me);
		message_vision("宝象给$N一包阴阳合欢散。\n",me);
	}
	command("walkby");
	command("whisper " + me->query("id")+ " 听说最近"+me->query("xs/girl_place")+GRN"附近来了个漂亮的小妞，你去给我弄来。");
/*
	log_file( "job/xueshan",sprintf("%8s%-10s开始第%5d次雪山任务，目标：%s，目前经验：%d\n",
		me->name(), "("+me->query("id")+")", me->query("job_time/雪山")+1,
		me->query("xs/girl_place"), me->query("combat_exp")),me);
*/
	return "给老祖爷爷干活，速去速回。";
}

string ask_fail()
{
	int i;
	object me = this_player();

	if( !me->query("xs/girl") )
		return "你根本就没任务，放弃什么呀？";
	if( !me->query("xs/girl_super") )
		return "这么简单的任务你怎么可以放弃呢！快去干！";
	if( me->query("xs/cant") )
		return "拜托，有点专业精神好不好？困难点的任务就不做，你怎么对的起我给你的EXP！";

	me->delete("xs/girl");
	me->delete("xs/girl_time");
	me->delete("xs/girl_ob");
	me->delete("xs/girl_super");
	me->delete("xs/cant");
	me->add("combat_exp", -(30+random(40)));
	i = 40 - me->query_condition("xs_job");
	if ( i >= 9 )
		me->apply_condition("job_busy", 1);
	else
		me->apply_condition("job_busy", 9 - i);
	me->clear_condition("task_busy");
	me->apply_condition("xs_job", 3);
	log_file( "job/xueshan",sprintf("%8s%-10s放弃了第%5d次困难任务(%s)。\n",
		me->name(),"("+me->query("id")+")",me->query("job_time/雪山")+1, me->query("xs/girl_place")), me);
	me->delete("xs/girl_place");
	return "这个任务是比较困难，你完不成也不能全怪你。";
}

string ask_fail2()
{
	object me = this_player();
	int i;

	if( !me->query("xs/girl") )
		return "你根本就没任务，失败什么呀？";

	command("disapp "+me->query("id"));

	me->delete("xs/girl");
	me->delete("xs/girl_time");
	me->delete("xs/girl_ob");
	me->delete("xs/girl_super");
	me->delete("xs/cant");
	me->add("combat_exp", -(30+random(40)));

	i = 40 - me->query_condition("xs_job");
	if ( i >= 9 )
		me->apply_condition("job_busy", 1);
	else
		me->apply_condition("job_busy", 9 - i);

	me->clear_condition("task_busy");
	me->apply_condition("xs_job", 3);

	log_file( "job/xueshan",sprintf("%8s%-10s第%5d次任务失败(%s)。\n",
		me->name(),"("+me->query("id")+")",me->query("job_time/雪山")+1, me->query("xs/girl_place")),me);
	me->delete("xs/girl_place");
	return "你好好反省一下去！";
}

string ask_times()
{
	object ob = this_player();

	if(!ob->query("job_time/雪山"))
		return "对不起，"+ob->name()+"，你好象还没有在我这里领过任务呢，加油干吧！";

	return ""+ob->name()+"，你已经供奉给老祖爷爷"+CHINESE_D->chinese_number((int)ob->query("job_time/雪山"))+"名美女！";
}

int check(object ob)
{
	object room;
	string room_name;

	if(!objectp(ob)
	 || !ob->query("startroom")      // add by Yuj@SJ to void job NPC
	 || ob->query("env/invisibility")
	 || ob->query("no_quest")
	 || userp(ob)
	 || ob->query("attitude")=="aggressive"
	 || ob->query("have_master")
	 || !objectp(room=environment(ob))
	 || room->query("no_fight")
	 || room->query("quest")
	 || room->query("outdoors") == "昆仑翠谷"
	 || strsrch(room_name = file_name(room),"/d/") != 0
	 || strsrch(room_name, "/d/wizard/") == 0
	 || strsrch(room_name, "/d/wuguan/") == 0
	 || strsrch(room_name, "/d/group/") == 0
	 || strsrch(room_name, "/d/death/") == 0
	 || strsrch(room_name, "/d/mingjiao/lsd/") == 0
	 || strsrch(room_name, "/d/mingjiao/bhd/") == 0
	 || strsrch(room_name, "/d/xueshan/") == 0
	 || strsrch(room_name, "/d/shaolin/fumoquan") == 0
	 || strsrch(room_name, "/d/baituo/") == 0)
	 return 0;
	return 1;
}
