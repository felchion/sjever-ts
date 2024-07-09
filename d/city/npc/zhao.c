// /d/city/npc/cheng.c
// this is made by beyond
// update 1997.7.4
// updated by snowman@SJ 13/02/2000
// for change_name()
// change to named for banname list by Lklv 2001.12.6
inherit NPC;
#include <mudlib.h>
#include <guanfu.h>

string *banned_name = ({
// Mud 保留名字
"自己","杀手","闪电","雪人","逍遥","祈雨","我们","你们","他们","咱们","大家","某人",
"尸体","天神","总管","龙神","仙人","巫师","门客","玩家","书剑","名字待定","中神通",
"蒙面","蒙面人","刺客","忍者","镖师","鬼魂","头颅","杀人犯","爱妻丸","留香一剑",
// Mud 地名，门派名
"衡山","华山","嵩山","恒山","泰山","雪山","黄山","天山","西域","苗疆","东北","中原",
"南疆","长江","黄河","中国","中华","昆仑山","长白山","白驼山","武当山","峨嵋山",
"冰火岛","桃花岛","黑木崖",
"少林","丐帮","明教","密宗","慕容","古墓","全真","铁掌","大理","白驼","桃花","星宿",
"天龙寺","大轮寺","五毒教","正义","邪恶","五岳剑派","日月神教","官府","武林",
"少林寺","武当派","铁掌帮","普通百姓","峨嵋派","姑苏慕容","古墓派","华山派","嵩山派",
"明教","神龙教","星宿派",
// 重要人物名字
"金庸","邓小平","江泽民","毛泽东","朱榕基","皇帝",
// 小说人物
"张三丰","张无忌","郭靖","杨过","令狐冲","胡斐","苗人凤","韦小宝","袁承志","胡一刀",
"陈家络","段誉","萧锋","乔锋","虚竹","东方不败","欧阳锋","洪七公","黄药师","段皇爷",
"一灯大师","王重阳","周伯通","黄蓉","风清扬","独孤求败",
// 脏话
"统一","法轮","法轮研究","法轮大","吃人","他妈的","去你的","操你妈","干你娘","废物",
"混蛋","坏蛋","杀人","抢劫","强奸","妓女","台独","领导","中央","屁眼","屁股","裸体",
"放屁","鸡巴","阴道","婊子","你丫","日你","赤佬","瘪三","傻逼","蠢蛋","操他","我操",
"太监","宦官","阉割","阉人"
});

string ask_me();

void create()
{
	set_name("赵城之", ({ "zhao chengzhi", "zhao" }));
	set("title", "扬州知府");
	set("gender", "男性");
	set("age", 43);
	set("str", 20);
	set("dex", 20);
	set("per", 20);
	set("long", "他就是赵城之，扬州现任知府。百姓如有困难可找他解决。(改名换姓....等)\n");
	set("combat_exp", 30000);
	set("shen_type", 0);
	set("attitude", "heroism");

	set_skill("unarmed", 50);
	set_skill("force", 50);
	set_skill("sword", 50);
	set_skill("dodge", 50);
	set_skill("parry", 50);
	set_temp("apply/attack", 50);
	set_temp("apply/defense", 50);
	set_temp("apply/armor", 50);
	set_temp("apply/damage", 15);

	set("neili", 500);
	set("max_neili", 500);
	set("jiali", 10);

	set("inquiry", ([
		"change_name" : (: ask_me :),
		"改名换姓" : (: ask_me :),
		"改名" : (: ask_me :),
		"改名字" : (: ask_me :),
	]));
	setup();
	carry_object(ARMOR_D("cloth"))->wear();
}

void init()
{
	add_action("change_name", "zhuce");
}

int change_name(string arg)
{
	string new_name, name, f_mname, f_class;
	int i, f_gen;
	object me = this_player();

	if (!me->query_temp("name_modify_paid") && !wizardp(me)) {
		command("say 朝廷明文规定，平民不得随意改名换姓，你想坐牢是不是？");
		return 1;
	}

	if ( !arg ) {
		command("say 你到底要改什么名字啊？");
		return 1;
	}

	name = me->query("name");
	new_name = arg;
	f_gen = me->query("family/generation");
	f_class = me->query("class");
	f_mname = me->query("family/master_name");

	switch(me->query("family/family_name")){
		case "桃花岛":
			if (f_gen == 2) {
				if (strlen(arg) != 3|| arg[2..2] != "风")
					return notify_fail("你的名字必须是三个字，而且以风结尾。\n");
			}
			break;
		case "峨嵋派":
		case "天龙寺":
		case "少林派":
			if (f_class == "bonze" || f_class == "huanxi") {	// 给敌人一个花钱去掉 huanxi 的机会
				if (strlen(arg) != 2)
					return notify_fail("你的名字必须是两个字。\n");
				switch (f_gen) {
					case 40:
						if (arg[0..0] == "清")
							break;
						return notify_fail("辈分不对，重来！\n");
					case 39:
						if (arg[0..0] == "道")
							break;
						return notify_fail("辈分不对，重来！\n");
					case 38:
						if (arg[0..0] == "慧")
							break;
						return notify_fail("辈分不对，重来！\n");
					case 37:
						if (arg[0..0] == "澄")
							break;
						return notify_fail("辈分不对，重来！\n");
					case 36:
						if (arg[0..0] == "玄")
							break;
						return notify_fail("辈分不对，重来！\n");
					case 35:
						if (arg[0..0] == "渡")
							break;
						return notify_fail("辈分不对，重来！\n");
					case 14:
						if (arg[0..0] == "了")
							break;
						return notify_fail("辈分不对，重来！\n");
					case 13:
						if (arg[0..0] == "本")
							break;
						return notify_fail("辈分不对，重来！\n");
					case 5:
						if (arg[0..0] == "文")
							break;
						return notify_fail("辈分不对，重来！\n");
					case 4:
						if (arg[0..0] == "静")
							break;
						return notify_fail("辈分不对，重来！\n");
					case 3:
						if (arg[0..0] == "灭")
							break;
						return notify_fail("辈分不对，重来！\n");
					default:
						if (name[0..0] != arg[0..0])
							return notify_fail("你的法号必须是两个字，而且第一个字不能变。\n");
				}
				if (f_class == "huanxi")
					me->set("class", "bonze");
			} else if (f_mname == "段正明")
				if (strlen(arg) > 3 || arg[0..0] != "段")
					return notify_fail("你的姓名必须是两或三个字，而且第一个字必须为段。\n");
			break;
		case "星宿派":
			if (f_gen == 2) {
				if (strlen(arg) != 3 || arg[2..2] != "子")
					return notify_fail("你的名字必须是三个字，而且以子结尾。\n");
			}
			break;
		case "姑苏慕容":
			if (f_gen == 2) {
				if (strlen(arg) < 3 || arg[0..1] != "慕容")
					return notify_fail("你的名字必须至少三个字，而且以慕容开头。\n");
			}
	}

	i = strlen(new_name);

	if (i < 2 || i > 4 ) {
		command("say “"+new_name+"”？");
		command("say 你的中文名字必须是 2 到 4 个中文字才行。");
		return 1;
	}


	if( !is_chinese(new_name) ) {
		command("say 你得用「中文」取名字，本官我可不认识西洋字。");
		return 1;
	}

	if (!wiz_level(me->query("id"))
	&& (!"/adm/daemons/named"->valid_name(new_name) || member_array(new_name, banned_name)!=-1)) {
		command("say 你这种名字会造成其他人的困扰，想个其它的。");
		return 1;
	}

	// Modified by mxzhao 2004/02/18
	{
		mixed ret = dbquery("select U_Name from Users where U_Name='"+new_name+"'");
		
		if (!ret)
		{
			write("数据库连接失败。\n");
			return 0;
		}
		
		if (sizeof(ret) > 0)
		{
			write("此中文名已被其他人使用。\n");
			return 0;
		}
		
		if (dbquery("update Users set "
			"U_Name='"+new_name+"' "
			"where U_Username='"+me->query("id")+"'"))
		{
			write("数据库连接失败。\n");
			return 0;
		}
	}
	/*
	{
		mixed mysql = db_connect("localhost","mud","root");

		if (!intp(mysql))
			return notify_fail("暂时不能修改姓名。\n");
		if (db_exec(mysql, "select U_Name from " + INTERMUD_MUD_NAME + "_Users where U_Name='"+new_name+"'")) {
			db_close(mysql);
			return notify_fail("已经有其他玩家使用了这个姓名。\n");
		}
		db_exec(mysql, "update " + INTERMUD_MUD_NAME + "_Users set "
			"U_Name='"+new_name+"' "
			"where U_Username='"+me->query("id")+"'"
		);
		db_close(mysql);
	}
*/

	me->set("name", new_name);
	me->add("name_modify", 1);
	command("chat 从今以后，"+name+"("+getuid(me)+") 改姓名为：" + new_name + "。");
	log_file("name_modify",
		sprintf("%-18s 第 %d 次改名为：%s\n",
			name+"("+getuid(me)+")", me->query("name_modify"), new_name
		), me
	);
	me->delete_temp("name_modify_paid");
	start_busy(2);
	if (new_name == name)
		command("say 你可真是有钱找我开心来着，改了个一模一样的名字。");
	command("say 你的户籍手续已经办理完毕，你可以走了。");
	return 1;
}

string ask_me()
{
	object me = this_player();
	string mname = (string)me->query("name");

	if(me->query_temp("name_modify_paid")){
		command("nod");
		return "好吧，你只要注册( zhuce <姓名> )你的新名字就行了。\n";
	}

	if(mname == "名字待定"){
		me->set_temp("name_modify_paid", 1);
		return "此民无名无姓，本官同意改名，你要注册( zhuce <姓名> )什么名字？";
	}

	if( this_object()->is_busy())
		return "我正忙着改写户册呢，你等等。";

	if(me->query_temp("name_wait_modify")){
	       command("hmm");
	       return "不是告诉了你，这事不那么好办吗？\n";
	}

	if (!wiz_level(me->query("id"))
	&& (!"/adm/daemons/named"->valid_name(mname) || member_array(mname, banned_name)!=-1)) {
		command("pat");
		me->set_temp("name_modify_paid", 1);
		return "我已经得到通知，不收你的改名费用，你要换个( zhuce <姓名> )什么名字？";
	}
	command("consider");
	command("say "+me->query("name", 1)+"你有名有姓而要修改户籍和姓名，本官觉得这事很难啊......\n");
	me->set_temp("name_wait_modify", 1);
	if(me->query("class") == "bonze")
		command("say 对了，和尚，尼姑可改不了辈分称号。\n\t比如你叫 本一，就必须 zhuce 以本字开头的名字。");
	return "如果你坚持要改，上报和入册费用是"+chinese_number(200*(me->query("name_modify")+1))+"两黄金，交到张帐房那里。\n";
}


void kill_ob(object ob)
{
	command("say 杀人啦！救命啊！");
	ob->add_condition("killer", 15);
	::kill_ob(ob);
}

void arrest_him(object me)
{
	int i;
	object *inv = all_inventory(me);

	me->clear_condition();
	me->apply_condition("city_jail", 200+(me->query("PKS") * 4) );
	me->set("startroom", JAIL);
	for( i=0; i < sizeof(inv); i++ ) {
		if( inv[i]->query_autoload() ) continue;
		if( inv[i]->is_character() ){
			inv[i]->move(environment(me));
			continue;
		}
		if( inv[i]->query("equipped") )
			inv[i]->unequip();
		destruct(inv[i]);
	}
	me->move(JAIL);
	message("vision", "只听“乒”地一声，一个昏沉沉的家伙被扔了进来。\n", environment(me), me);
}

int accept_object(object me, object obj)
{
	string id, name;
	object wenshu;
	mapping target;

	if (is_busy()) {
		command("say 没看见本官正忙着吗？一边等着！");
		me->add_busy(1);
		return 0;
	}

	start_busy(2);

	if (member_array(me->query("id"), GF_WANTED->query_wanted_list() ) != -1) {
		command("say 好哇"+me->query("id")+"，本府正通缉你呢，竟敢来自投落网！");
		command("say 来人啊！给我把这"+RANK_D->query_rude(me)+"打进死牢！！");
		if (userp(me)) {
			tell_object(me, "你一惊，正要有所行动，只听一声：“在我张召重面前还敢耍滑头？倒！”你后脑一震，就晕了过去！\n");
			me->unconcious();
			arrest_him(me);
			log_file("job/gfbug", sprintf("%-18s 自投落网，Obj id：%s\n",
				me->name()+"("+me->query("id")+")", obj->query("id")), me);
		}
		message_vision("\n只听一阵风响，几名高大的捕头已经点倒了$N，拖入内去了。\n",me);
		if (!userp(me)) destruct(me);
		return 0;
	}

	if( !objectp(wenshu = present("tiebu wenshu", me)) ){
		command("say 咦？你平白无故来干什么？本府怎么知道你不是歹人一伙？");
		return 0;
	}

	if( !mapp(target = wenshu->query("target")) ){
		command("say 你拿个什么烂文书来滥竽充数？");
		return 0;
	}

	if (!stringp(id = obj->query("victim_id")))
		id = obj->query("id");

	if (!stringp(name = obj->query("victim_name")))
		name = obj->query("name");

	if (target["id"] != id || target["name"] != name) {
		command("say 要你去拿通缉的犯人，你带个"+obj->query("name")+"回来？好好看看铁捕文书！");
		return 0;
	}

	message_vision("$N给$n一"+obj->query("unit")+obj->name(1)+"。\n", me, this_object());

	if ( obj->is_character() && !obj->is_corpse() ){
		command("say 好！"+me->query("name")+"竟然把"+obj->query("name")+"给抓回来了，真是位大大的英雄！");
		command("say 来人啊！给我把这"+RANK_D->query_rude(obj)+"打进死牢！！");

		message_vision("\n只见几名衙役过来，拖起$N，入内去了。\n",obj);
		command("say 本府代表千万民众多谢这位"+RANK_D->query_respect(me)+"了");
		GF_WANTED->do_give_reward(id, target, me, 0);
		if( userp(obj) )
			arrest_him(obj);
		else 	destruct(obj);
		return notify_fail("");
	}

	if( obj->query("kill_by") != me ){
		command("say 哦，"+obj->name()+"！多谢这位"+RANK_D->query_respect(me)+"了，你下去吧。");
		command("say 下一个！");
		log_file("job/gfbug", sprintf("%-18s 通缉任务，Obj：%s，Killed by %s\n",
			me->name()+"("+me->query("id")+")", obj->query("name"),
			(stringp(obj->query("kill_by"))?obj->query("kill_by"):"Unknow") ), me);
	}
	else {
		command("thumb "+me->query("id"));
		command("say "+RANK_D->query_respect(me)+"铲除此人，真是大快人心啊！");
		command("say 本府代表千万民众多谢这位"+RANK_D->query_respect(me)+"了");
		GF_WANTED->do_give_reward(id, target, me, 1);
	}
	destruct(obj);
	return notify_fail("");
}
