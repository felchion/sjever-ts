
inherit NPC;

string ask_me_1(object);
//string ask_me_2(object);

void create()
{
	set_name("方生大师", ({
		"fangsheng dashi",
		"fangsheng",
		"dashi",
	}));
	set("long",
		"他是一位身材矮小的老年僧人，面上皱纹堆垒。\n"
		"慈眉善目，露出悲天悯人之像。\n"
	);
       set("nickname", "心禅堂首座");
	set("gender", "男性");
	set("attitude", "friendly");
	set("class", "bonze");
    set("no_quest",1);

	set("age", 80);
	set("shen", 1);
	set("str", 30);
	set("int", 30);
	set("con", 30);
	set("dex", 30);
	set("max_qi", 3000);
	set("max_jing", 1300);
	set("neili", 3000);
	set("max_neili", 3000);
	set("jiali", 40);
	set("combat_exp", 1600000);
	 
	set_skill("force", 180);
    set_skill("yijin-jing", 180);
	set_skill("dodge", 180);
	set_skill("shaolin-shenfa", 180);
	set_skill("strike", 180);
       	set_skill("parry", 180);
	set_skill("banruo-zhang", 180);
        set_skill("jingang-quan",180); 
set_skill("yizhi-chan",180);
set_skill("ruying-suixingtui",180);
set_skill("cuff",180);
set_skill("finger",180);
set_skill("leg",180);
	set_skill("buddhism", 180);
	set_skill("literate", 100);
    map_skill("force", "yijin-jing");
	map_skill("dodge", "shaolin-shenfa");
	map_skill("strike", "banruo-zhang");
        map_skill("parry", "banrou-zhang");
       //map_skill("cuff","jingang-quan");
	prepare_skill("strike", "banruo-zhang");
       prepare_skill("strike", "banruo-zhang");
	create_family("少林派", 35, "弟子");
        set("inquiry", ([
		"钢杵" : (: ask_me_1, "gangchu" :),
		"柴刀" : (: ask_me_1, "chaidao" :),
//      "挠钩" : (: ask_me_1, "naogou" :),
//		"套索" : (: ask_me_1, "taosuo" :),
	 ]));
	set("weapon_count", 10);
//	set("tools_count", 5);

	setup();

        //carry_object("/d/shaolin/obj/changjian")->wield();
        carry_object("/d/shaolin/obj/xuan-cloth")->wear();
}

string ask_me_1(string name)
{
	mapping fam; 
	object ob;
	
	if (!(fam = this_player()->query("family")) || fam["family_name"] != "少林派")
		return RANK_D->query_respect(this_player()) + 
		"与本派素无来往，不知此话从何谈起？";

	if (  present(name, this_player()))
		return RANK_D->query_respect(this_player()) + 
		"你现在身上不是有这样东西吗，怎麽又来要了？ 真是贪得无餍！";
//	if (!(this_player()->query("pass_xin"))
  //        return "你是偷偷进来的吧，请出去吧!\n";
             if (query("weapon_count") < 1)
		return "抱歉，你来得不是时候，东西已经发完了。";
        ob = new("/d/shaolin/obj/" + name);
	ob->move(this_player());
	add("weapon_count", -1);
	message_vision("方生给$N一件" + ob->query("name") + "。\n", this_player());
	return "拿去吧。用此物为寺中服务。\n";
}
/*
string ask_me_2(string name)
{
	mapping fam; 
	object ob;
	
	if (!(fam = this_player()->query("family")) || fam["family_name"] != "少林派")
		return RANK_D->query_respect(this_player()) + 
		"与本派素无来往，不知此话从何谈起？";
       if (!(this_player()->query("pass_xin"))
          return RANK_D->query_respect(this_player())+"偷偷进来的吧，请出去吧!\n";
       if (  present(name, this_player()) )
       return RANK_D->query_respect(this_player()) + 
	"你现在身上不是有这样东西吗，怎麽又来要了？ 真是贪得无餍！";
	if (query("tool_count") < 1)
		return "已经有人去后山了，你请回吧。";

    ob = new("/d/shaolin/obj/" + name);
	ob->move(this_player());

	add("tool_count", -1);

	message_vision("方生给$N一件" + ob->query("name") + "。\n", this_player());

	return "拿去吧。你可用此物翻过断崖，去拜见几位前辈高僧。";
}*/
void attempt_apprentice(object ob)
{
	object me=this_object(); 
	mapping ob_fam, my_fam;
	string name, new_name;
//	me=this_object();
	my_fam =me->query("family");
	name = ob->query("name");

	if (!(ob_fam = ob->query("family")) || ob_fam["family_name"] != "少林派")
	{
		command("say " + RANK_D->query_respect(ob) + "与本派素无来往，不知此话从何谈起？");
		return;
	}
	if ( (string)ob->query("class")!="bonze" && ob_fam["family_name"] == "少林派") 
	{
		command("say " + RANK_D->query_respect(ob) + "是俗家弟子，不能在寺内学艺。");
		return;
	}

    	if ( !(ob->query("pass_xin")) )
	{
          command("say "+RANK_D->query_respect(ob) +"是怎么进来的?");
           return;
       }
  	if ( ob_fam["generation"] <= my_fam["generation"] )
	{
		command("say " + RANK_D->query_respect(ob) + "，贫僧哪里敢当 !");
		return;
	}
	if ( ob_fam["generation"] == (my_fam["generation"] + 1) && name[0..0] == "玄")
	{
		command("say " + ob_fam["master_name"] + "的徒弟怎么跑到我这儿来了，哈哈哈 !");
		command("recruit " + ob->query("id"));
	}

	if ( ob_fam["generation"] == (my_fam["generation"] + 2) )
	{
           name = ob->query("name");
           new_name = "玄" + name[1..1];
           command("say 老衲垂死之年，又得一可塑之才，真是可喜可贺 !");
           ob->set("name", new_name);
 		ob->set("title", "心禅堂职事僧");
           command("say 今后你的法名叫做"+new_name+",恭喜你成为少林玄字辈圣僧之             
                     一\n");
	    command("say 从今以后你就是心禅堂职事高僧了，望你好好深造!\n");
	    command("recruit " + ob->query("id"));
		}
		else
	command("say " + RANK_D->query_respect(ob) + "，你是从哪蹦出来的，你不能越级拜师。");
	return;
     }
