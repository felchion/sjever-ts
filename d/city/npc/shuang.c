// /d/city/npc/shan.c

inherit NPC;

int do_look(string);
int ask_me();
string ask_beauty();
string ask_ugly();
string ask_age();
string ask_exp();
string ask_marriage();
string ask_fat();
string ask_pk();
string ask_balance();
int env_cmp(object o1, object o2);

void create()
{
	set_name("双儿", ({ "shuang er", "shuang", "girl" }) );
	set("long", "她发现你正在看他，脸蛋顿时红了，别看她年纪小，但知道的事挺多。\n"+
		"你可以向她打听：\n"+
		"【美丽】【丑陋】【年龄】【经验】【结婚】\n"+
		"【身材】【杀人】【存款】【婚姻】【爱情】\n"+
		"【男友】【女友】【离婚】【丈夫】【妻子】\n");
	set("title", "民女");
	set("gender", "女性");
	set("age", 18);
	set("attitude", "friendly");
	set("shen_type", 1);
	set("no_get", "双儿对你而言太重了。\n");
	set("str", 18);
	set("int", 23);
	set("con", 19);
	set("dex", 20);
	set("per", 30);

	set("max_qi", 300);
	set("max_jing", 300);
	set("neili", 400);
	set("max_neili", 400);
	set("combat_exp", 80000);

	set_skill("literate", 100);
	set_skill("dodge", 30);
	set_skill("force", 30);
	set_skill("parry", 30);
	set_skill("unarmed", 30);

	set("inquiry", ([
		"name" : "我就是双儿啊。" ,
		"here" : "这是月老亭，请问是来帮着写婚姻指南的还是来找对象的？",
		"婚姻" : "常人说，婚姻就是把水做的女人和石头做的男人放在一起搅和，就是和泥。" ,
		"kiss" : "你无聊不无聊啊，这是婚姻介绍所，你走错地方了吧。",
		"sex" : (: ask_me :),
		"make love" : (: ask_me :),
		"love" : "ＬＯＶＥ？嘻嘻，王八看绿豆，看对眼啦。" ,
		"韦小宝" : "啊！你也知道我家小相公？好久没见他了，不知道他又去哪花心了。" ,
		"爱情" : "爱情？嘻嘻，王八看绿豆，看对眼啦。" ,
		"boyfriend" : "男朋友？你千万不要先说你喜欢他，天下的男人和乌鸦一般黑。" ,
		"男友" : "男朋友？你千万不要先说你喜欢他，天下的男人和乌鸦一般黑。" ,
		"girlfriend" : "想交女朋友？你得先多学些文化，装得甜一点才行。" ,
		"女友" : "想交女朋友？你得先多学些文化，装得甜一点才行。" ,
		"wife" : "不知听谁说的，妻子就是一个你结婚前爱得要命，结婚后讨厌得不得了的女人。" ,
		"妻子" : "不知听谁说的，妻子就是一个你结婚前爱得要命，结婚后讨厌得不得了的女人。" ,
		"丈夫" : "三奶奶说了，丈夫就是一个大娃娃啊。" ,
		"husband" : "三奶奶说了，丈夫就是一个大娃娃啊。" ,
		"离婚" : "离婚，就是新生活的开始了。" ,
		"divorce" : "离婚，就是新生活的开始了。" ,
////		"美丽" : (: ask_beauty :),
////		"beauty" : (: ask_beauty :),
//		"丑陋" : (: ask_ugly :),
//		"ugly" : (: ask_ugly :),
//		"年龄" : (: ask_age :),
//		"age" : (: ask_age :),
//		"经验" : (: ask_exp :),
//		"exp" : (: ask_exp :),
		"结婚" : "男人的一半是女人，但结婚可是爱情的坟墓噢" ,
//	       "身材" : (: ask_fat :),
//	       "fat" : (: ask_fat :),
//	       "杀人" : (: ask_pk :),
//	       "pk" : (: ask_pk :),
//	       "存款" : (: ask_balance :),
//	       "balance" : (: ask_balance :),
	]) );

	setup();
	carry_object("d/city/obj/pink_cloth")->wear();
	carry_object("d/city/obj/flower_shoe")->wear();
}

void init()
{
	object ob;

	::init();
	add_action("do_look","kiss1");
	add_action("do_look","kick");
	add_action("do_look","flirt");
	add_action("do_look","loveshoe");

	if( interactive(ob = this_player()) && !is_fighting()) {
		remove_call_out("greeting");
		call_out("greeting", 1, ob);
	}
}

int ask_me()
{
	object me;

	me = this_player();
	switch( random(5) ) {
		case 0:
			message("vision", "双儿向" + me->name() +"怒道：你无聊不无聊呀。流氓！你出去！\n", me );
			command("slap " + me->query("id"));
			remove_call_out("kicking");
			call_out("kicking", 1, me);
			break;
		case 1:
			message("vision", "双儿向" + me->name() +"怒道：你无聊不无聊呀。流氓！你出去！\n", me );
			command("kick " + me->query("id"));
			remove_call_out("kicking");
			call_out("kicking", 1, me);
			break;
		case 2:
			command("faint " + me->query("id"));
			break;
		case 3:
			command("rascal " + me->query("id"));
			command("kick2 " + me->query("id"));
			break;
		case 4:
			command("xixi " + me->query("id"));
			command("benger " + me->query("id"));
	}
	return 1;
}

int do_look(string target)
{
	object me;

	me = this_player();
	if (target=="shuang" || target=="shuang er")
	if ((string)me->query("gender") != "女性") {
		message("vision", "双儿向" + me->name() +"道：你无聊不无聊呀。\n", me );
		switch( random(5) ) {
			case 0:
				command("club " + me->query("id"));
				remove_call_out("kicking");
				call_out("kicking", 1, me);
				break;
			case 1:
				command("kick " + me->query("id"));
				remove_call_out("kicking");
				call_out("kicking", 1, me);
				break;
			case 2:
				command("faint " + me->query("id"));
				break;
			case 3:
				command("stare " + me->query("id"));
				break;
			case 4:
				command("rascal " + me->query("id"));
				command("kick2 " + me->query("id"));
		}
	}
	return 0;

}

void kicking(object me)
{
	if (!me || environment(me) != environment()
	|| environment(this_object())->query("short") != "月老亭")
		return;

	me->move("/d/city/yueqidian");
	message("vision","只听“嘭”地一声，紧接着" +  me->query("name") +
		"从月老亭里飞了出来，脸上一个红手印，屁股上有一个小鞋印。\n", environment(me), me);
}

int env_cmp(object o1, object o2)
{
	string env1, env2;
	object where;
	env1 = (where = environment(o1)) ? base_name(where) : "none";
	env2 = (where = environment(o2)) ? base_name(where) : "none";
	return strcmp(env1, env2);
}

int sort_per(object ob1, object ob2)
{
	int p1 = ob1->query_per();
	int p2 = ob2->query_per();

	if (wizardp(ob1)) return 1;
	if (wizardp(ob2)) return -1;
	return p2 - p1;
}

string ask_beauty()
{
	object *obj1, *ob_list;
	string msg;
	int i, per;

	msg ="嘻嘻，现在「书剑」里数";
	ob_list = filter_array(children(USER_OB), (: userp :));
	obj1 = filter_array(ob_list, (: $1->query("gender") == "女性" :));
	reset_eval_cost();
	obj1 = sort_array(obj1, (: sort_per :));
	if (sizeof(obj1)) {
		per = obj1[0]->query_per();
		for( i = 0; i < sizeof(obj1); i++) {
			if (obj1[i]->query_per() < per)
				break;
			msg += obj1[i]->query("name")+"("+capitalize(obj1[i]->query("id"))+")、";
		}
		msg = msg[0..<1] + "最漂亮";
	}

	obj1 = ob_list - obj1;
	reset_eval_cost();
	obj1 = sort_array(obj1, (: sort_per :));
	if (sizeof(obj1)) {
		msg += "，";
		per = obj1[0]->query_per();
		for( i = 0; i < sizeof(obj1); i++) {
			if (obj1[i]->query_per() < per)
				break;
			msg += obj1[i]->query("name")+"("+capitalize(obj1[i]->query("id"))+")、";
		}
		msg = msg[0..<1] + "最英俊";
	}
	return msg+"。";
}

string ask_ugly()
{
	object *obj1, *ob_list;
	string msg;
	int i, per;

	msg ="嘻嘻，现在「书剑」里数";
	ob_list = filter_array(children(USER_OB), (: userp :));
	obj1 = filter_array(ob_list, (: $1->query("gender") != "女性" :));
	obj1 = sort_array(obj1, (: sort_per :));
	if (sizeof(obj1)) {
		for (i = 1;i <= sizeof(obj1); i++)
			if (!wizardp(obj1[<i])) break;
		per = obj1[<i]->query_per();
		for(;i <= sizeof(obj1); i++) {
			if (wizardp(obj1[<i])) continue;
			if (obj1[<i]->query_per() > per)
				break;
			msg += obj1[<i]->query("name")+"("+capitalize(obj1[<i]->query("id"))+")、";
		}
		msg = msg[0..<1] + "最丑样。";
	}

	return msg;
}

string ask_age()
{
	object where,obj,*ob_list;
	string msg, name;
	int i, age = 0;

	ob_list = users();

	for( i = 0; i < sizeof(ob_list); i++) {
		where = environment(ob_list[i]);
		if( ob_list[i]->query("combat_exp") > 1000
		&& !wizardp(ob_list[i])
		&& ob_list[i]->query("mud_age") > age
		&& where
		&& strsrch(file_name(where), "/d/") >= 0) {
			obj = ob_list[i];
			age = obj->query("mud_age");
		}
	}

	if ( !obj ) {
		msg ="咦，咋没人了呢？\n";
		return msg;
	}

	obj->set_temp("oldest",1);
	name = obj->query("name")+"("+obj->query("id")+")";
	msg ="嘻嘻，除了巫师外，现在「书剑」里数"+name+"年纪最大。";

	return msg;
}

string ask_exp()
{
	object where,obj,*ob_list;
	string msg, name;
	int i, exp = 0;

	ob_list = users();

	for( i = 0; i < sizeof(ob_list); i++) {
		where = environment(ob_list[i]);
		if( ob_list[i]->query("combat_exp") > exp
		&& !wizardp(ob_list[i])
		&& where
		&& strsrch(file_name(where), "/d/") >= 0) {
			obj = ob_list[i];
			exp = obj->query("combat_exp");
		}
	}

	if ( !obj ) {
		msg ="咦，现在怎么一个高手都没有了？\n";
		return msg;
	}

	obj->set_temp("highhand",1);
	name = obj->query("name")+"("+obj->query("id")+")";
	msg ="啊，现在「书剑」里数"+name+"的武功最高！\n";

	return msg;
}

string ask_fat()
{
	object where,obj,*ob_list;
	string msg, name;
	int i, fat = 0;

	ob_list = users();
	for( i = 0; i < sizeof(ob_list); i++) {
		where = environment(ob_list[i]);
		if( ob_list[i]->max_food_capacity() > fat
		&& !wizardp(ob_list[i])
		&& where
		&& strsrch(file_name(where), "/d/") >= 0) {
			obj = ob_list[i];
			fat = obj->max_food_capacity();
		}
	}

	if ( !obj ) {
		msg ="咦，现在的人怎么都这么瘦？\n";
		return msg;
	}

	obj->set_temp("fat",1);
	name = obj->query("name")+"("+obj->query("id")+")";
	msg ="嘻嘻，现在「书剑」里数"+name+"最能吃。\n";

	return msg;
}

string ask_pk()
{
	object where,obj,*ob_list;
	string msg, name;
	int i, pk = 0;

	ob_list = users();
	for( i = 0; i < sizeof(ob_list); i++) {
		where = environment(ob_list[i]);
		if( ob_list[i]->query("PKS") > pk
		&& !wizardp(ob_list[i])
		&& where
		&& strsrch(file_name(where), "/d/") >= 0) {
			obj = ob_list[i];
			pk = obj->query("PKS");
		}
	}

	if ( !obj ) {
		msg ="现在这里没人杀过人。\n";
		return msg;
	}

	obj->set_temp("pk_most",1);
	name = obj->query("name")+"("+obj->query("id")+")";
	msg ="呃，……，现在这里数"+name+"杀的人最多！\n";

	return msg;
}

string ask_balance()
{
	object where,obj,*ob_list;
	string msg, name;
	int i, balance = 0;

	ob_list = users();
	for( i = 0; i < sizeof(ob_list); i++) {
		where = environment(ob_list[i]);
		if( ob_list[i]->query("balance") > balance
		&& !wizardp(ob_list[i])
		&& where
		&& strsrch(file_name(where), "/d/") >= 0) {
			obj = ob_list[i];
			balance = obj->query("balance");
		}
	}

	if ( !obj ) {
		msg ="现在这里没人杀过人。\n";
		return msg;
	}

	obj->set_temp("richest",1);
	name = obj->query("name")+"("+obj->query("id")+")";
	msg ="嘻嘻嘻，这里数"+name+"最有钱。\n";

	return msg;
}
