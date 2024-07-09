// NPC: biaoshi.c
// Date: Look 99/04/25

inherit NPC;

string* names = ({
        "包不同",       "风波恶",       "公冶乾",       "邓百川",
        "殷野王",       "韦一笑",       "周颠",         "范遥",         "张中",
        "殷梨亭",       "万青里",
        "吴长老",       "蒋舵主",       "奚长老",       "宋长老",       "白世镜",
});
string ask_me1();
string ask_me2();

void create()
{
        set_name("镖头", ({ "biao tou", "biaoshi", "biaotou" }));
        set("nickname", "四海为家");
        set("long", 
"这是个上了年纪的镖师，年轻的时候闯荡江湖，后来功成身退，在此地开了一家龙门镖局。\n"
"一身武艺着实了得。\n");
        set("gender", "男性");
        set("age", 68);
        set("attitude", "friendly");
        set("shen_type", 1);
        set("per", 20);
        set("str", 28);
        set("int", 24);
        set("con", 20);
        set("dex", 20);
        
        set("max_qi", 3000);
        set("max_jing", 1800);
        set("neili", 3000);
        set("max_neili", 3000);
        set("eff_jingli", 1800);

        set("jiali", 50);
        set("combat_exp", 500000);
        set("score", 5000);

        set_skill("force", 100);
        set_skill("dodge", 100);
        set_skill("parry", 100);
        set_skill("strike", 100);
        set_skill("cuff", 100);

        set_skill("taizuquan", 100);
        set_skill("tiyunzong", 100);

        map_skill("dodge", "tiyunzong");
        map_skill("parry", "taizuquan");
        map_skill("cuff", "taizuquan");

        set("inquiry", ([
                "保镖" : (: ask_me1 :),
                "护送" : (: ask_me2 :),
        ]));

        setup();
        carry_object("/clone/misc/cloth")->wear();
}

string ask_me1()
{
        if (this_player()->query("combat_exp") <1000000)
                return "保镖可是一件十分危险的工作，你武功尚低，怕是不能胜任吧。";
        this_player()->set_temp("pass",1);
        return "我看"+RANK_D->query_respect(this_player())+"神功初成，最近镖局生意很多，你就替我走一趟镖如何？";
}

string ask_me2()
{
        int number;
        object me,ob,ob_flag;
        me = this_player();
        if (!me->query_temp("pass"))
                return "想当年我闯荡江湖，何等威风，唉。。。老了，不提也罢！";
        if (me->query_temp("mubiao")){
                command("kick " + me->query("id"));
                return "刚才让你护送的人你给送哪里去了？";
        }
        if (me->query_condition("husong_job")>0)
        	return "就这点活，镖局的人自己还闲着呢。";
        if (me->query_condition("job_busy")>0)
        	return "就这点活，镖局的人自己还闲着呢。";
        number = random(sizeof(names));
        ob = new(__DIR__"wife.c");
        ob->move(environment(me));
//      ob->set_leader(me);
        ob_flag = new(__DIR__"flag.c");
        ob_flag -> move(me);
        me->set_temp("mubiao",names[number]);
        ob->set_temp("mubiao",names[number]);
        me->set_temp("killed",0);
        me->set_temp("husongjob",ob);
        me->apply_condition("husong_job", 30);
        tell_object(me, "镖师交给你一面镖旗。\n");
        return "好吧，今天就交给你一个任务，你去帮我把"+ob->query("name")+"送到"+names[number]+"那里。";
}
