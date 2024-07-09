// waiter.c

inherit WAITER;
#include <ansi.h>
#include <get_place.h>

inherit F_VENDOR;

string ask_me();

void create()
{
	set_name("店小二", ({ "xiao er", "xiao", "waiter" }) );
	set("gender", "男性" );
	set("age", 22);
	set("long", "这位店小二正笑咪咪地忙着，还不时拿起挂在脖子上的抹布擦脸。\n");
	set("combat_exp", 100);
	set("attitude", "friendly");
	set("rank_info/respect", "小二哥");

	set("vendor_goods", ({
		(["name": MISC_D("denglong"), "number":10]),
		(["name": MISC_D("fire"), "number":10]),
	}));

	set("inquiry", ([
		"name" : "不敢当，称呼我小二就好了。",
		"rumors" : "听说顾先生收藏了一些失传了的字画，不知是不是真的？",
		"here" : "这里是扬州呀，客官您竟然不知道？",
		"扬州" : "这里就是扬州呀，客官您这不是骑着毛驴找毛驴吗？",
		"苏州" : "在扬州的东南边，出扬州东门去找。",
		"杭州" : "在苏州的东南边。",
		"襄阳" : "从扬州往西走就能走到。",
		"成都" : "从襄阳往西再转西南。",
		"少林" : "在河南少室山，从扬州往北可以走到。",
		"少林寺" : "在河南少室山，从扬州往北可以走到。",
		"少林派" : "在河南少室山，从扬州往北可以走到。",
		"武当" : "在湖北境内，出了扬州往南面走吧。",
		"武当山" : "在湖北境内，出了扬州往南面走吧。",
		"武当派" : "在湖北境内，出了扬州往南面走吧。",
		"星宿" : "在天山西边，从扬州往西去找。",
		"星宿海" : "在天山西边，从扬州往西去找。",
		"星宿派" : "在天山西边，从扬州往西去找。",
		"明教" : "在昆仑山东边，从扬州往西走。",
		"光明顶" : "在昆仑山东边，从扬州往西走。",
		"慕容" : "在姑苏燕子坞，去苏州找找看吧。",
		"姑苏慕容" : "在姑苏燕子圬，去苏州找找看吧。",
		"桃花岛" : "这是东海上的一个小岛，从杭州往东南走，到海边看看能不能雇到船吧。",
		"归云庄" : "在太湖边上，从苏州往南走。",
		"丐帮" : "这可是个秘密呀。",
		"昆仑" : "在遥远的西域，一直往西走吧。",
		"昆仑山" : "在遥远的西域，一直往西走吧。",
		"昆仑派" : "在遥远的西域，一直往西走吧。",
		"古墓" : "在终南山，从扬州向北再折向西南吧。",
		"终南山" : "从扬州向北再折向西南吧。",
		"大理" : "在大宋的南边，所以要从扬州往南走。",
		"华山" : "在陕西境内，从扬州城西向北走可以到达。",
                "猴爪山" : "在川湘交界处，从扬州向南再向西行。",
                "铁掌山" : "在川湘交界处，从扬州向南再向西行。",
                "铁掌帮" : "在川湘交界处，从扬州向南再向西行。",
		"峨嵋山" : "在川西，从成都南门往偏西走。",
		"峨嵋派" : "在川西，从成都南门往偏西走。",
		"峨嵋" : "在川西，从成都南门往偏西走。",
		"查老学士" : (: ask_me :),
		"查老先生" : (: ask_me :),
            	"zhaxueshi" : (: ask_me :),
            	"zha" : (: ask_me :),
            	"查" : (: ask_me :),
            	"行踪" : (: ask_me :),

	]));
	set("per", 15);

        set("chat_chance", 1);
        set("chat_msg", ({
		CYN"店小二嚷声说道，“天下第一大门派就是少林。”\n"NOR,
		CYN"店小二伸出右大姆指，道，“天下第一帮么，自然是丐帮了。”\n"NOR,
		CYN"店小二小声说道，“天下第一教，是人称魔教的明教。”\n"NOR,
		CYN"店小二好奇地问，“武当山，昆仑山，五岳剑派，挑哪一个好? ”。\n"NOR,
		CYN"店小二叹息道，“神雕侠侣，相继亡故，不知古墓一派可有传人。”\n"NOR,
		CYN"店小二神秘一笑，道，“听说桃花岛主黄药师最近云游后，回岛休养去了。”\n"NOR,
		CYN"店小二怒冲冲，高声叫嚷，“非杀了那帮星宿恶贼不可。”\n"NOR,
		CYN"店小二笑嘻嘻地说道，“咱前年做生意到大理，还真碰上了大理国皇帝。”\n"NOR,
		CYN"店小二低声道，“那猴爪山共有五个山峰,就像猴儿的手掌一般。”\n"NOR,
		CYN"店小二指了指东南方，哈哈大笑道，“都什么朝代了，还要什么大燕国。”\n"NOR,
	}));
	setup();
}

string ask_me()
{
       object ob, where;
       object me = this_player();
       ob = find_living("zha xueshi");
       if (!ob) return "查老学士飘忽不定，小二哥我也不知道他去哪里了。\n";
       where = environment(ob);
       if (!where) return "查老学士飘忽不定，小二哥我也不知道他去哪里了。\n";  
       if (objectp(present("zha xueshi", environment(me)))){
                command("disapp "+me->query("id"));
                return "查老学士就在这里呀...\n";   
       }
       command("look "+me->query("id"));
       return "这位" + RANK_D->query_respect(me) + "，听我店里的过客谈起，查老学士曾在" + get_place(base_name(where)) + (string)where->query("short") +"一带出现过。";
}


void init()
{
	object ob;

	::init();
	if (base_name(environment()) != query("startroom")) return;
	if( interactive(ob = this_player()) && visible(ob) && !is_fighting() ) {
		remove_call_out("greeting");
		call_out("greeting", 1, ob);
	}
	add_action("do_list", "menu");
	add_action("do_buy", "buy");
}

void greeting(object ob)
{
	if (!ob || !present(ob, environment())) return;
	switch( random(2) ) {
		case 0:
			say(CYN"店小二笑咪咪地说道：这位" + RANK_D->query_respect(ob)+ "，进来喝杯茶，歇歇腿吧。\n"NOR);
			break;
		case 1:
			say(CYN"店小二用脖子上的毛巾抹了抹手，说道：这位" + RANK_D->query_respect(ob)+ "，请进请进。\n"NOR);
	}
}

int accept_object(object who, object ob)
{

	if (ob->query("money_id") && ob->value() >= 500) {
		tell_object(who, CYN"店小二一哈腰，说道：多谢您老，客官请上楼歇息。\n"NOR);
		who->set_temp("rent_paid","宝昌客栈");
		return 1;
	}
	return 0;
}

void relay_emote(object me,string arg)
{
	command(arg+" "+me->query("id"));
}
