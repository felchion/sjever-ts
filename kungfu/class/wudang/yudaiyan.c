// Modify By River@SJ

#include <ansi.h>

inherit NPC;

string ask_me_1();
string ask_me_2();
string ask_wan1();
string ask_dan1();
string ask_dan2();

void create()
{
        set_name("俞岱岩", ({ "yu daiyan", "yu" }));
        set("nickname", "武当三侠");
        set("long", 
"他就是张三丰的三弟子、武当三侠俞岱岩。因被奸人所害，可能
下半辈子只能在躺椅上渡过了！他身穿一件洗得发白的蓝色长衫。
形骸憔悴，脸色惨白，眼神深沉，象是回忆着什么！\n");
        set("gender", "男性");
        set("age", 33);
        set("attitude", "peaceful");
        set("shen_type", 1);
        set("str", 25);
        set("int", 25);
        set("con", 25);
        set("dex", 5);
        set("max_qi", 900);
        set("max_jing", 900);
        set("neili", 2500);
        set("max_neili", 2500);
        set("jiali", 60);
        set("combat_exp", 750000); 
	set("unique", 1);
        set("no_bark",1);

        set_skill("force", 120);
        set_skill("yinyun-ziqi", 120);
        set_skill("dodge", 60);
        set_skill("tiyunzong", 60);
        set_skill("cuff", 90);
        set_skill("taiji-quan", 90);
        set_skill("parry", 60);
        set_skill("sword", 60);
        set_skill("taiji-jian", 60);
        set_skill("literate", 100);

        map_skill("force", "yinyun-ziqi");
        map_skill("dodge", "tiyunzong");
        map_skill("cuff", "taiji-quan");
        map_skill("parry", "taiji-quan");
        map_skill("sword", "taiji-jian");
        prepare_skill("cuff", "taiji-quan");

        create_family("武当派", 2, "弟子");

        set("inquiry", ([
                "炼丹" : (: ask_me_2 :),
                "九转结气丸" : "对不起，我现在身上没这种药。", 
                "天心解毒丹" : "对不起，我现在身上没这种药。", 
        ]));

        set("wan_count1", 3);
        set("wan_count2", 3);
        set("letter_count", 1 + random(1));

        setup();
        carry_object("/d/wudang/obj/greyrobe")->wear();
}

string ask_me_2()
{
	string *yao_name = ({"生地","茯苓","红花","灵仙","桃仙","没药","五灵脂","千金子","当归","远志","独活","防风"});
	mapping fam;
	object applicant;
	string name;
	name = yao_name[random(sizeof(yao_name))];
	applicant=this_player();

	if (!(fam = applicant->query("family")) || fam["family_name"] != "武当派")
		return RANK_D->query_respect(this_player())+"讲笑话了，炼丹悟道乃我道家之事，你还是请回吧！！";

	if((int)applicant->query("combat_exp", 1) > 2000000) {
		command("shake");
		return "你已经可以在江湖中独当一面了，炼丹的事还是让别人去做吧！\n";
	}

	if( applicant->query("job_name") == "武当炼丹" 
         || applicant->query_condition("job_busy"))
		return "现在还没有研悟出什么新的配方，还是过些时日再说吧。\n";

	if((int)applicant->query("combat_exp", 1) < 500000){
		command("hmm");
		return "你还是在江湖中多磨砺磨砺吧，炼丹的事以后再说！\n";
	}
	if((int)applicant->query_skill("medicine", 1) < 80){
		command("shake "+(string)applicant->query("id"));
		return "没有高深的药理知识，岂能炼出好的丹药，你还是在本草药理上多下点功夫吧!\n";
	}
	if(applicant->query("gender")  =="无性"){
		command("sneer");
		return "你阴阳不调，精气不生，辱我武当师门，休提炼丹之事!\n";
	}
	if (fam["family_name"] != "武当派" || fam["generation"] != 2 )
		return "汝非吾师之徒，无须习习练丹之术！！";

	if(present("wudang danyao", applicant))
		return "新炼之丹尚不知成否，修道之人岂能如此贪婪！！\n";

	if(interactive(applicant) && applicant->query_temp("caiyao_room"))  
		return "找到需要的药材了吗？\n";

	if(interactive(applicant) && applicant->query_temp("caiyao_ok"))  
		return "找到需要的药材了吗？\n";

	if(interactive(applicant) && applicant->query_temp("yao_get"))  
		return "你还是赶快把你的药研细再说吧！！\n";

	if(interactive(applicant) && applicant->query_temp("yanyao_ok"))  
		return "你还是赶快炼药吧！！！\n";

	if(interactive(applicant) && applicant->query_temp("liandan"))  
		return "你现在正在炼丹，岂同儿戏！！！？\n";

	if(interactive(applicant) && applicant->query_condition("wd_busy"))  
		return "现在还没有研悟出什么新的配方，还是过些时日再说吧。\n";

	command("nod");
	applicant->set_temp("caiyao_room",random(16)+1);
	applicant->set_temp("caiyao_name",name);
	applicant->set("job_name", "武当炼丹");
	applicant->apply_condition("job_busy", 10);
	applicant->apply_condition("wd_busy",10+random(5));
	if(applicant->query("gender")  =="女性")
		return "数日来我瞑思苦想，终于悟得一方，然则近来不知何故，\n"+
			"方圆百里难寻「" +name+ "」,师妹若是有意，不妨前去寻寻！\n";
	if(applicant->query("gender")  =="男性")
		return "数日来我瞑思苦想，终于悟得一方，然则近来不知何故，\n"+
			"方圆百里难寻「" +name+ "」,师弟若是有意，不妨前去寻寻！\n";
}

int accept_object(object who, object ob,object me)
{
	mapping fam;
        object ob_yaodai;
        int exp,pot,p;
        object ob_letter;
       
	if (!(fam = this_player()->query("family"))
	 || fam["family_name"] !="武当派"){
		command("say 你与本派素无来往，不知为何送此物于我? \n");
		return 0;
        }        
	if( !who || environment(who) != environment() ) return 0;
	if ( !objectp(ob) ) return 0; 
	if ( !present(ob, who) ) return notify_fail("你没有这件东西。");  
	 
	if(who->query_temp("mark/wd_mission")) {
		if (ob->query("id") != "yao lou" ){
			command("? "+(string)who->query("id"));
			return 0;
		}
		if(!present("ma huang", ob)){
			command("say 你好象没有采到麻黄啊？！");
			return 0;
		}
		if(!present("teng huang", ob)){
			command("say 你好象没有采到藤黄啊？！");
			return 0;
		}
		if(!present("xiong huang", ob)){
			command("say 你好象没有采到雄黄啊？！");
			return 0;
		}
//		if(!who->query_temp("mark/wd_mission")){
//			command("say 我并没让你去采药啊？！");
//			return 0;
//		}
		command("pat " + who->query("id"));
		command("say 你如此勤劳，他日必有所成。\n");
		call_out("destroying", 1, ob);
		if( (int)who->query("potential", 1) < (int)who->query("max_pot", 1) ) {
			exp =(3+random(10))*12;
			pot = exp/5 + random(exp/10);
			who->add("combat_exp",exp);
			who->add("potential",pot);
		}
		who->add("job_time/武当",1);
		who->delete_temp("mark/wd_mission");
		return 1;
	} 
	if(who->query_temp("caiyao_ok")) {        
		if (ob->query("id") != "yao cai" 
		|| strip(ob->query("name")) != who->query_temp("caiyao_name")){
			command("pat "+(string)who->query("id")); 
			command("say 这些东西若是对你没用，那我就替你留在这里吧！");
			command("hehe "+(string)who->query("id"));
			remove_call_out("destroying");
			call_out("destroying", 1, ob);
			return 1;
		}
		command("congra " + who->query("id"));
		command("say 你能觅得此药，当必历尽艰辛，此方药剂你就拿去一试吧。");
		write("俞岱岩给你一个"WHT"药袋。\n"NOR);
		remove_call_out("destroying");
		call_out("destroying", 1, ob);
		ob_yaodai=new("/d/wudang/obj/yaodai");
		ob_yaodai->move(who);
		who->add("job_time/武当",1);
		who->delete_temp("caiyao_name");
		who->delete_temp("caiyao_room");
		who->delete_temp("caiyao_ok");
		who->set_temp("yao_get",1); 
		p = 60+random(20);
		who->add("potential", p);
		if ( who->query("potential") > who->query("max_pot"))
			who->set("potential", who->query("max_pot"));
		log_file("job/liandan",sprintf("%8s%-10s武当炼丹获得了%3d点潜能，现经验：%d。\n",
			who->name(),"("+who->query("id")+")",p, who->query("combat_exp")), who);
		return 1;
	} 
	else {
		if ((string)ob->query("id") == "changsheng guo"){
			if (query("letter_count") > 0){
				command("ah "+(string)who->query("id"));
				command("say "+"你心地淳朴，学武定有大成!可惜我是个残废，不能教你武功。\n");
				ob_letter=new("/d/wudang/obj/letters");
				ob_letter->move(who);
				add("letter_count", -1);
				message_vision("$N获得一封书信。\n",this_player());
				command("say 你拿我这封信去拜我师兄俞莲舟为师吧，也算是我们结交一场!\n");
				remove_call_out("destroying");
				call_out("destroying", 1, ob);
			} 
			else {
				command("touch "+(string)who->query("id"));
				command("say 可是已有人给过我这仙果了!\n");
			}
			return 1;
		}
		command("hmm "+ (string)who->query("id"));
		command("say 无缘无故送我东西？");
		return 0;     
	}
}

void destroying(object obj)
{
	destruct(obj);
	return;
}
