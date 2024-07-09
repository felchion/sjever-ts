// /d/mr/npc/murong-fu.c  慕容复
// by leontt /12/05/2000
// by tiantian@sj 6/3/2000 fix mr job bug

inherit NPC;
inherit F_MASTER;
#include <ansi.h>
#include <combat.h>
#include "skill.h"
int ask_fuming(string arg);
int ask_bo();
string ask_fenglu();
string ask_sword2();
string ask_skill();
string ask_wang();
string ask_learn();
string ask_dan();
string ask_dan1();
string ask_ge();
string ask_sword();
string ask_book();
string ask_book2();
string ask_times();
string ask_9dan();
int ask_fangqi();

void create()
{
        object weapon;
        set_name("慕容复",({"murong fu","murong","fu"}));
        set("title","姑苏慕容公子");
        set("nick","以彼之道 还施彼身"); 
        set("long",
              "他就是天下号称以彼之道，还彼之身的姑苏慕容复。\n"
              "他脸上带着不可一世的笑容。\n");
        set("age", 27);
        set("attitude", "friendly");
        set("shen_type", 0);
        set("str", 30);
        set("int", 30);
        set("con", 30);
        set("dex", 30);
        set("no_get","慕容复背不起来。");

        set("max_qi",2000);
        set("max_jing", 3000);
        set("eff_jingli", 4000);
        set("neili",3500);
        set("max_neili", 5000);
        set("jiali", 1);
        set("combat_exp",1500000);
        set("dan", 10);
        set("shen", -1000);
        set("no_bark",1);
        set("score", 50000);
        set("chat_chance_combat", 25);
        set("chat_msg_combat", ({
                (: exert_function, "shenyuan" :),
                (: perform_action, "strike.riyue" :),
                (: perform_action, "finger.ci" :),
                (: perform_action, "sword.lianhuan" :),
        }));
        set_skill("strike",170);
        set_skill("finger",170); 
        set_skill("dodge",180);
        set_skill("force", 170);
        set_skill("canhe-zhi", 160);
        set_skill("murong-jianfa",170);
        set_skill("shenyuan-gong", 180);
        set_skill("yanling-shenfa", 160);
        set_skill("xingyi-zhang",170);
        set_skill("douzhuan-xingyi", 180);
        set_skill("parry", 180);
        set_skill("sword", 270);
        set_skill("literate", 170);
        set_skill("murong-daofa", 160);
        set_skill("blade",160);
        set_skill("xingyi-jianzhen",170);

        map_skill("blade", "murong-daofa");
        map_skill("finger", "canhe-zhi");
        map_skill("force", "shenyuan-gong");
        map_skill("dodge", "yanling-shenfa");
        map_skill("strike", "xingyi-zhang");
        map_skill("parry", "murong-jianfa");
        map_skill("sword", "murong-jianfa");
 
        prepare_skill("finger","canhe-zhi");
        prepare_skill("strike","xingyi-zhang");

        create_family("姑苏慕容",2,"弟子");

        set("inquiry", ([
                        "name" : "我就是以彼之道，还施彼身的姑苏慕容复。\n",
                        "here" : "这里是大名鼎鼎的燕子坞，难道你没有听说过？\n",
                        "rumors" : "家父突染恶疾，我连他老人家最后一面都没见到，此事顶有蹊跷！\n",
                        "姑妈" : "我的姑妈住在曼佗罗山庄，那里有闻名天下的娘缳玉洞。\n",
                        "还施水阁" : (: ask_ge :),
                        "慕容博" : (: ask_bo :),
                        "领悟" : (: ask_book :),
                        "俸禄" : (: ask_fenglu :),
                        "王语嫣" : (: ask_wang :),
                        "碧玉剑" : (: ask_sword2 :),
                        "宝剑" : (: ask_sword :),
                        "次数" : (: ask_times :),
                        "斗转星移"  : (: ask_skill :),
                        "玉露清新散" : (: ask_dan1 :),
                        "图解" : (: ask_book2 :),
                        "武学" : (: ask_learn :),
                        "复命" : (: ask_fuming :), 
                                                "放弃" : (: ask_fangqi :),
        ]));
        set("dan_count", 2);
        set("book_count",1);
        set("book2_count" ,1);
        set("dan1_count", 2);
        setup();
        if (clonep()) {
                weapon = unew(BINGQI_D("sword/biyu-jian"));
                if (!weapon) weapon = unew(BINGQI_D("changjian"));
                weapon->move(this_object());
                weapon->wield();
                carry_object(__DIR__"obj/cloth3")->wear();
        }
        add_money("gold", 5);
}
void init()
{
        ::init();
        add_action("do_jiaoliang", "jiaoliang");
}
void attempt_apprentice(object ob)
{
        if (ob->query_skill("parry", 1) < 120) {
                command("say 我姑苏慕容以彼之道、还施彼身对招架要求甚高，小兄弟似乎不宜学习。"); 
               command("say " + RANK_D->query_respect(ob) + "的功力不够，还是请回吧。");
                return;
        }

        if (ob->query_skill("shenyuan-gong", 1) < 100) {
                command("say 我姑苏慕容以彼之道、还施彼身对内功心法要求甚高，小兄弟似乎不宜学习。");
               command("say " + RANK_D->query_respect(ob) + "的功力不够，还是请回吧。");
                return;
        }

       command("say 嗯，看你还是个学武的料，我就收下你吧！");
       command("say 苍天在上，让我姑苏慕容又得一良材，为复兴我大燕多了一份力量。");
       command("chat* 朗声说道：公子我又收一良材，大燕兴复指日可待啦！");
       command("chat* haha");
       command("recruit " + ob->query("id"));
       ob->set("title","姑苏慕容第三代弟子");
}

string ask_fenglu() 
{
        object ob=this_player();
        mapping fam = ob->query("family");
        int gold;       
        if ( !fam || fam["family_name"] != "姑苏慕容")
           return "这位"+ RANK_D->query_respect(ob) +"不是我慕容家臣，此话从何说起？";
        if ( fam["master_name"] == "包不同")
           return "这位"+ RANK_D->query_respect(ob) +"在我慕容世家的身份太低，现在还没有资格领取俸禄。";
        if ( fam["master_name"] == "风波恶")
           return "这位"+ RANK_D->query_respect(ob) +"在我慕容世家的身份太低，现在还没有资格领取俸禄。";
        if ( fam["master_name"] == "公冶乾")
           return "这位"+ RANK_D->query_respect(ob) +"在我慕容世家的身份太低，现在还没有资格领取俸禄。";
        if ( fam["master_name"] == "邓百川")
           return "这位"+ RANK_D->query_respect(ob) +"在我慕容世家的身份太低，现在还没有资格领取俸禄。";
        if (ob->query("fenglu") && ob->query("fenglu") >= ob->query("age"))
           return "这位"+RANK_D->query_respect(ob) +"今年已经领过俸禄了，还是等明年再来吧。";        
         ob->set("fenglu",ob->query("age"));         
         command ("addoil " + ob->query("id"));         
         message_vision(NOR"慕容复给了$N一些"HIY"黄金"NOR"。\n"NOR,ob);
         gold=(int)(ob->query_skill("shenyuan-gong",1)*30000);
         MONEY_D->pay_player(ob, gold);
         return "这里是你今年的俸禄，但愿这位"+ RANK_D->query_respect(ob) +"不要使我失望，好好干吧！";         
}

string ask_sword2()
{
        object me=this_player() , weapon;
        mapping fam;

        if (!(fam = me->query("family")) || fam["family_name"] != "姑苏慕容")
           return RANK_D->query_respect(me)+"与本派素无来往，不知此话从何谈起？";
        weapon = present("biyu jian", this_object());
        if(!objectp(weapon))    weapon = unew(BINGQI_D("sword/biyu-jian"));
        if(!objectp(weapon))
                return "碧玉剑已经被人拿走了, 现在不在我手里。";  
        command("unwield biyu jian");
        weapon->move(me);
        command("wield jian");
        message_vision("\n$N给了$n一把碧玉剑。\n", this_object(), me);
}

string ask_wang()
{
	mapping fam;
	if (!fam = this_player()->query("family") || fam["family_name"] !="姑苏慕容")
        return RANK_D->query_respect(this_player()) + "与本派素无来往，不知此话从何谈起？";
	return "语嫣是我表妹，一直被姑妈管的很严，连我都很长时间没有见到她了。\n";
}

string ask_skill()
{
	mapping fam;
	if (!(fam = this_player()->query("family"))|| fam["family_name"] != "姑苏慕容")
        return RANK_D->query_respect(this_player()) + "与本派素无来往，不知此话从何谈起？";
	return "斗转星移是我慕容家的绝学，最是注重招架和身法。\n";

}

string ask_ge()
{
	mapping fam;
	if (!(fam = this_player()->query("family")) || fam["family_name"] != "姑苏慕容")
        return RANK_D->query_respect(this_player()) + "与本派素无来往，不知此话从何谈起？"; 
	return "还施水阁是我慕容收藏天下秘籍之所在，在我的书.....\n";
}

string ask_sword()
{
	mapping fam;
	if (!(fam = this_player()->query("family")) || fam["family_name"] != "姑苏慕容")
        return RANK_D->query_respect(this_player()) + "与本派素无来往，不知此话从何谈起？";
	return "碧玉剑是老爷子仙逝后的遗物，我让阿朱姑娘收起来好好保管！\n";
}

string ask_book()
{
        object ob;
        mapping fam;
        if (!(fam = this_player()->query("family")) || fam["family_name"] != "姑苏慕容")
                return RANK_D->query_respect(this_player()) +
                "与本派素无来往，不知此话从何谈起？";
        if (!(fam = this_player()->query("family")) || fam["master_name"] != "慕容复")
        return RANK_D->query_respect(this_player()) +
                "功力不够，何以谈及领悟？";

        if (  present("douzhuan xingyi", this_player()) ) 
                return RANK_D->query_respect(this_player()) +
                "你现在身上不是有斗转星移的武籍了嘛，怎么又来要了？ 真是贪得无餍！";

        if (query("book_count") < 1) return "对不起，斗转星移的武籍已经被别人要走了。";

        ob = new("/d/mr/obj/shu1");
        ob->move(this_player());

        add("book_count", -1);

        message_vision("$N得到一本斗转星移的武功秘籍。\n",this_player());
        return "好吧，看你为慕容家忠心耿耿，这本书就赐于你吧。";
}

string ask_book2()
{
        object ob;
        mapping fam;
        if (!(fam = this_player()->query("family")) || fam["family_name"] != "姑苏慕容")
                return RANK_D->query_respect(this_player()) +
                "与本派素无来往，不知此话从何谈起？";
        if (!(fam = this_player()->query("family")) || fam["master_name"] != "慕容复") 
        return RANK_D->query_respect(this_player()) +
                "功力不够，何以谈及领悟？";
        if ( (int)this_player()->query_skill("douzhuan-xingyi",1)<50)
             return RANK_D->query_respect(this_player()) +
                "功力不够，何以谈及领取图解？";
        if (  present("douzhuan xingyi", this_player()) )
                return RANK_D->query_respect(this_player()) +
                "你现在身上不是有斗转星移的武籍了嘛，怎么又来要了？ 真是贪得无餍！";

        if (query("book2_count") < 1) return "对不起，斗转星移的武籍已经被别人要走了。";

        ob = new("/d/mr/obj/shu2");
        ob->move(this_player());

        add("book2_count", -1);

        message_vision("$N得到一本斗转星移的武功秘籍。\n",this_player());
        return "好吧，看你为慕容家忠心耿耿，这本书就赐于你吧。";

}

int ask_bo()
{
        mapping fam;
        if ((int)this_player()->query_temp("marks/失败")) {
                say("慕容复眼中发出两道寒光，直射" + this_player()->name() + "一字一字说道：已经较量过了，你怎麽还\n"
		"不回去安心练功呢!\n");
                return 1;
        }
        if (!(fam =this_player()->query("family")) || fam["family_name"] != "姑苏慕容"){
        	say("你与本派素无来往，不知此话从何谈起？\n");
	        return 1;
        }
        if ((int)this_player()->query_temp("marks/bai_bo")) {
        	say("你已经是家父的弟子了，不用再向我学了!\n");
        	return 1;
        }
        else {
        	say("慕容复眼中发出两道寒光，直射" + this_player()->name() + "一字一字说道：家父现在身处少林，那里\n" 
		"藏龙卧虎，你武功未成，最好不要去自去其辱，如果你有信心，就和我较量一下。(jiaoliang)\n" );
	        this_player()->set_temp("marks/jiao", 1);
        	return 1;
        }
}

int do_jiaoliang()
{
        object me, obj, jiaxin;
        int current_qi;
	int i;
        me = this_object();
        obj = this_player();
        current_qi = (int)me->query("qi");

        say(obj->name() + "对慕容复说：好吧，那就让师傅指点几招 。\n");
        set("max_qi", 3500);
        set("jiali", 10);
        set("neili", 3500);
        set("max_neili", 3500);
        set("max_jing", 2000);
	i=10;
        if( obj->query_temp("marks/jiao") ) { 
                obj->set_temp("marks/jiao", 0);
                while ( ((int)obj->query("qi")*100 / (int)obj->query("max_qi")) > 30) {
                        if ( !present(obj, environment()) )
                                return 1;
		i--;
                        COMBAT_D->do_attack(obj, me, obj->query_temp("weapon"));
               if (((int)me->query("qi")*100/current_qi)< 30|| i==0) {
                                say("慕容复对" + obj->name() + "说道： 不想我慕容世家又出良材，我这就写封家信， 你马上动身前去少林，\n"
				"把它交给他老人家。希望你能不负众望，振兴慕容世家，光复大燕国。\n"
                                );
                                jiaxin = new("/d/mr/obj/jiaxin");
                                jiaxin->move(obj);
                                obj->set_temp("marks/bai_bo", 1);
                                message_vision("$N交给$n一封家信\n", me, obj);
                                return 1;
                        }
                        COMBAT_D->do_attack(me, obj);
                }
                say("慕容复对着" + obj->name() + "冷冷说道：你武功未成，就好高务远,\n"
		"你还是安心练功去吧!\n"
                );
                this_player()->set_temp("marks/失败", 1); 
         }
        return 1;
}

string ask_learn()
{
       object *ob;
       int j=random(sizeof(my_skills));
       mapping skill = my_skills[j];
       mapping fam;
       int i;
     
       i = 7+random(5);
       ob = all_inventory(environment(this_player()));
       if (!(fam = this_player()->query("family")) || fam["family_name"] != "姑苏慕容") // 必须是慕容派弟子
               return RANK_D->query_respect(this_player()) +
               "与本派素无来往，不知此话从何谈起？";
       if ((int)this_player()->query("combat_exp") > 2000000 )
               return "你已经尽数掌握了精深奥妙的武学诀要，我可不敢劳您的大架了。\n";
       if( (int)this_player()->query_skill("parry", 1) < 100 ) // 招架必须大于等于100级
               return RANK_D->query_respect(this_player()) +
               "于本门武学尚不能运用自如，如何谈及天下之武学？";
       if( (int)this_player()->query_skill("douzhuan-xingyi", 1) < 100 )  // 斗转星移必须大于等于100级
               return RANK_D->query_respect(this_player())+"于本门武学尚不能运用自如，如何谈及天下之武学？";
       if( this_player()->query_condition("mr_job") )
               return RANK_D->query_respect(this_player())+"还没完成我交给你的任务呢。";
       if( this_object()->is_fighting())
               return "嚷什么嚷，没看见我这正忙着呢吗？";
       if( (int)this_player()->query_condition("wait_mr_job", 1) > 0)
               return RANK_D->query_respect(this_player())+"先等等吧，我现在没有什么感兴趣的武功想学。";

        if(!skill["skill_name"]){
                        return sprintf("SKILL.H文件在第%d个技能出错，请通知巫师修改。\n",j);
        }
        else{              
        	this_player()->set_temp("skill_target", skill["skill_name"]);  // 此标记表明此次偷学目标技能的中文名字
        	this_player()->set_temp("skill_id", skill["skill_id"]); // 此标记表明此次偷学目标技能的英文名字
       		this_player()->set_temp("skill_type", skill["skill_type"]); // 此标记表明此次偷学目标技能的英文类型
        	this_player()->set_temp("skill_lvl", i); // 此标记表明此次偷学目标技能所需要的级别
        	this_player()->apply_condition("mr_job", 30+random(15)); // 设置此次任务必须在所需时间内完成;
        
        	command( "whisper "+this_player()->query("id")
                        + " 我姑苏慕容傲视天下所有武功，但也不乏为之羡慕的。" );
       		command("whisper "+this_player()->query("id") +
                        " 好吧，"+"「"+skill["skill_name"]+"」"+"「"+skill["skill_id"]+"」我垂涎已久，你们把它学回来，我大大的有赏。");
        	
        	new("/d/mr/obj/paper")->move(this_player());
        	
        	if(random(2))
                        return "今天天气就是好，就是好。";
                else
                        return "今天天气真不错。";
        	
        }
}

int ask_fuming(string arg)
{
       object me, ob;
       int bouns,timebouns,My_exp,Target_exp;
       float rate; 
       
       me = this_player();
       ob = this_object();

       if( !arg)
       {
               command("say 你要展示什么？");
               return 1;
        }

       if( !me->query_condition("mr_job"))
       {
               command("say 你现在没有任务吧？");
               return 1; 
       }
      
       if( me->is_busy())
       {
               command("say 你正忙着呢，有什么事情呆会再说。");
               return 1;
        }

       if( me->is_fighting())
       {
               command("say 你正打架呢，有什么事情呆会再说。"); 
               return 1;
       }
       
       if(!me->query_temp("job_done"))
       {
       		command("say 你学到什么东西了？还有脸敢回来？");
                return 1;
       }
       
       if( this_object()->is_fighting())
       {
               command("say 嚷什么嚷，没看见我这正忙着呢吗？");
               return 1;
       }
	
       Target_exp = (int)me->query_temp("master_exp");
       My_exp= (int)me->query("combat_exp");
       
       me->add("mr_job",1);
       bouns = (int)me->query_temp("skill_lvl")*5/2;
        bouns += ( me->query_skill("douzhuan-xingyi",1) + me->query_skill("literate",1) )*5/4;
       bouns = bouns + (int)me->query_skill("douzhuan-xingyi", 1)/2;
       bouns = bouns + (int)me->query("int")*random(me->query("int"));
       timebouns = me->query("mr_job")/10;
       
       rate = to_float(Target_exp)/to_float(My_exp);
       if (rate > 2) rate = 2;
        if (rate < 1) rate = 0.6;
       bouns = bouns*rate;
       bouns = bouns+timebouns;
        
       message_vision(CYN"慕容复仔细研究着你记在纸卷上的武功精要，情不自禁地说道：好一个"+me->query_temp("skill_target")+"。\n"NOR, me);
       message_vision(CYN"慕容复拍了拍$N的头，对$N说道：辛苦你了，下去休息吧。\n"NOR, me);
       me->add("combat_exp", (int)bouns);
       me->improve_skill("douzhuan-xingyi", me->query_int()*2);
       if( ((int)me->query("potential", 1)+bouns) >= (int)me->query("max_pot", 1))
       {
               me->set("potential", (int)me->query("max_pot"));
       }
       if( ((int)me->query("potential", 1)+bouns) < (int)me->query("max_pot", 1))
       {
               me->add("potential", (int)bouns/6);
       }
       write(GRN"你获得了"+(string)(bouns)+"点经验和"+(string)(bouns/4)+"点潜能的奖励\n"NOR); 
      
       log_file("job/mrjob",
        sprintf("%s %s(%s) touxue %s(%s) on %s, exp=%d。\n",     
                me->query("title"), me->name(1), geteuid(me),
                me->query_temp("skill_target"), me->query_temp("skill_id"), ctime(time()), bouns ));
       
       me->delete_temp("skill_target");
       me->delete_temp("skill_id");
       me->delete_temp("skill_type");
       me->delete_temp("skill_lvl");
       me->delete_temp("job_done");
       
       me->clear_condition("mr_job");
       me->apply_condition("wait_mr_job", random(10)+5);
       
       if(present("paper", me))
       {  
       		destruct(present("paper",me));
       }
       
       return 1;		
        
}

int ask_fangqi()
{
        object me=this_player(),obj;

        if( !me->query_condition("mr_job") )
       {
               command("say 你现在没有任务吧？");
               return 1; 
       }

        command( "say 也好，那么你先下去休息一会吧。" );
        command( "disapp");
        command( "sigh "+me->query("id"));
        obj=me->query_temp("dizi");
        if( objectp(obj) )
                destruct(obj);
        me->delete_temp("skill_target");
        me->delete_temp("skill_id");
        me->delete_temp("skill_type");
        me->delete_temp("skill_lvl");
        me->delete_temp("job_done");
        
        me->clear_condition("mr_job");
        me->apply_condition("wait_mr_job", random(2)+2);
        me->add("combat_exp",-10-random(10));
                return 1;
}

string ask_times()
{
        object ob;
        ob = this_player();
        if(!ob->query("mr_job"))
        {
         return "很遗憾，"+ob->name()+"，你好象没有为慕容做过贡献！";
        }
        return ""+ob->name()+"，你已经完成了"+CHINESE_D->chinese_number((int)ob->query("mr_job"))+"次慕容偷学任务！";
}
