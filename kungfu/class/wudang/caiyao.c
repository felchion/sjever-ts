// caiyao.c 采药道长
#include <ansi.h>;
inherit NPC;

string ask_me();
int ask_me1();
int ask_me2();
int ask_me3();
int ask_me4();
int accept_object(object who, object ob);
void destroying(object me, object obj);

void create()
{
	set_name("采药道长", ({ "caiyao daozhang", "caiyao", "daozhang" }));
	set("long", "他是武当山的采药道长。\n");
	set("gender", "男性");
	set("age", 60);
	set("attitude", "peaceful");
	set("shen", 1000);

	set("str", 20);
	set("int", 20);
	set("con", 25);
	set("dex", 20);

	set("max_qi", 500);
	set("max_jing", 500);
	set("neili", 500);
	set("max_neili", 500);

	set("combat_exp", 50050);
	set("score", 5000);

	set_skill("force", 60);
	set_skill("dodge", 40);
	set_skill("cuff", 60);
	set_skill("taiji-quan", 60);
	set_skill("unarmed", 40);
	set_skill("parry", 40);
	set_skill("sword", 40);
	set_skill("taiji-jian", 40);
	set_skill("taoism", 40);

	map_skill("cuff", "taiji-quan");
	map_skill("parry", "taiji-quan");
	map_skill("sword", "taiji-jian");
	prepare_skill("cuff", "taiji-quan");
	create_family("武当派", 4, "弟子");

	set("inquiry", ([
		"古道" : (: ask_me :),
		"药典" : (: ask_me1 :),
		"只是" : (: ask_me2 :),
		"结果" : (: ask_me3 :),
		"药材" : (: ask_me4 :),
	]));

	set("book5_count", 1);
	set("book6_count", 1);

	set("chat_chance", 3);
	set("chat_msg", ({
		CYN"采药道长感慨道：想不到这深山里还有一条如此隐蔽的古道。\n"NOR,
		CYN"采药道长自我埋怨道：真是大头虾，居然把药锄给弄丢了，哎!还得回去再找一把。\n"NOR,
	}));

	setup();
	carry_object("/d/wudang/obj/greyrobe")->wear();
	carry_object("/d/wudang/obj/daolv")->wear();
}

void reset()
{
	set("book5_count", 1);
	set("book6_count", 1);
}

int accept_object(object who, object ob)
{
         object me;
         object obzi;
         me = this_player();
         if( !who || environment(who) != environment() ) {return 0;}
         if ( !objectp(ob) ) return 0; 

         if ( !present(ob, who) ) return notify_fail("你没有这件东西。");

         if ((string)ob->query("id") == "yao chu"){
                if ( file_name(environment(this_object())) != this_object()->query("startroom")){
                     command("say 我现在不在武当山路，你给我药锄也没有用。");
                     return 0;
                }
                command("say 这正是我需要的，为了报答你我就带你到一奇异的地方去吧!");
                remove_call_out("destroying");
                call_out("destroying", 1, this_object(), ob);
                command("say 这捆绳子是我采药用的，你也一并那拿去吧，说不定有用!");
                obzi=new("/d/wudang/obj/shengzi");
                obzi->move(me);
                me->move("/d/wudang/houshan/gudao1");
                return 1;
        }
        return 0;
}

string ask_me()
{
        return "为了发现这古道，我连锄头都丢了，可不能轻易告诉你!\n";
}

void destroying(object me, object obj)
{
        destruct(obj);
        return;
}

int ask_me1()
{
        command("say 我壮年之时，曾到武当后山采药，不想身坠悬崖，幸蒙一位\n"
                "异人所救，此人搭救我之后，就悄悄离去，我已年近暮年，我所渴求，只是...");
        return 1;
}

int ask_me2()
{
        object book5, me, ob;

        me = this_object();
        ob = this_player();

	if(ob->query_temp("marks/ask2")){
		command("say " + RANK_D->query_respect(ob) + "是否打探到了结果？\n");
		return 1;
        }
	if (query("book5_count") < 1){
		command("say 已经有人在帮我打探，回头在说吧。\n");
		return 1;
        }
	if (query("book5_count") >0){
		command("say 如果" + RANK_D->query_respect(ob) + "能帮我寻找到这位恩人的姓名，我必将感激\n"
			"不尽，这里有一本痘疹定论，虽不是什麽稀有宝物，但平常间也很难找到，请" + RANK_D->query_respect(ob) + "收下，\n"
			"略表我的心意。");
		book5 = new(MEDICINE_D("m-book5"));
		book5->move(ob);
		add("book5_count", -1);
		message_vision("$N交给$n一本痘疹定论。\n", me, ob);
		ob->set_temp("marks/ask2",1);
		return 1;
	}
}

int ask_me3()
{       
        object book6, me, ob;

        me = this_object();
        ob = this_player();

	if(!ob->query_temp("marks/jieguo")){
		command("say " + RANK_D->query_respect(ob) + "并没打探到，何谈结果？做人岂能言而无信呢？\n");
		command("disapp " + ob->query("id"));
		return 1;
        }   
        if (query("book6_count") < 1){
		command("say 你来晚了，已经有人在帮我打探出了结果。\n");
		return 1;
        } 
        if (query("book6_count") >0){
		command("thank " + ob->query("id"));
		command("say 这位" + RANK_D->query_respect(ob) + "言而有信，帮我找到昔日恩人，老道无以回报，我这里有一本\n"
			"先祖手抄明代郭子章所著的博集稀痘方，讲述有关本草药理的精意。" + RANK_D->query_respect(ob) + "务必受下，也许\n"
			"对你会有一些用。");
		book6 = new(MEDICINE_D("m-book6"));
		book6->move(ob);
		add("book6_count", -1);
		message_vision("$N交给$n一本博集稀痘方。\n", me, ob);
		ob->delete_temp("marks/jieguo");
		ob->delete_temp("marks/ask2");
		return 1;
	}
}

int ask_me4()
{    
	int roomn,yaon;
	object me, who;
	me = this_object();
	who = this_player();
        
	roomn = who->query_temp("caiyao_room",1);
	yaon = who->query_temp("caiyao_name",1);

	if(!who->query_temp("caiyao_room")) {
		command("say 武当山山上药材众多，你若有意自去寻寻！\n");
		return 1;
	}   
	if (roomn < 5){
		command("say 你是说「" +yaon+ "」吗？前些时候我曾在烈火丛林采集到一些。\n");
		return 1;
	} 
	if (4 < roomn && roomn < 9){
		command("say 你是说「" +yaon+ "」吗？前些时候我曾在落叶丛林采集到一些。\n");
		return 1;
	} 
	if (8 < roomn && roomn < 13){
		command("say 你是说「" +yaon+ "」吗？前些时候我曾在积雪丛林采集到一些。\n");
		return 1;
	}
	if (12 < roomn){
		command("say 你是说「" +yaon+ "」吗？前些时候我曾在阔叶丛林采集到一些。\n");
		return 1;
	}  
	return 1;
}
