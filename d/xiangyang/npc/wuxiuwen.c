// wuxiuwen.c 武修文

inherit NPC;
int ask_ji();
void consider();

void create()
{
        set_name("武修文", ({ "wu xiuwen","wu","xiuwen" }) );
        set("gender", "男性" );
        set("age", 26);
        set("long", "身材修长，面目俊朗，一看就是名门后代。\n");
        set("attitude", "friendly");
        set("str", 35);
        set("con", 30);
        set("int", 20);
        set("dex", 30);


        set("max_qi", 1500);
        set("max_jing", 1500);
        set("neili", 2000);
        set("eff_jingli", 1500);
        set("max_neili", 2000);
        set("jiali", 100);
        set("shen", 6500);
        set("combat_exp", 850000);
        set("startroom","/d/xiangyang/cross2");

        set_skill("force", 140);
        set_skill("strike", 140);
        set_skill("blade", 140);
        set_skill("dodge", 140);
        set_skill("parry", 140);
        set_skill("huntian-qigong", 140);
        set_skill("literate", 140);
        set_skill("xianglong-zhang", 150);
        set_skill("liuhe-daofa", 140);
        set_skill("xiaoyaoyou", 140);
        set("chat_chance_combat", 50);
        set("chat_msg_combat", ({
                (: perform_action, "strike.xiao" :)
        }));

        map_skill("force", "huntian-qigong");
        map_skill("blade","liuhe-daofa");
        map_skill("dodge", "xiaoyaoyou");
        map_skill("strike", "xianglong-zhang");
        map_skill("parry", "xianglong-zhang");
        prepare_skill("strike", "xianglong-zhang");

        create_family("丐帮", 19, "挂名弟子");

	set("inquiry", ([
		"叫化鸡" : (: ask_ji :),
		"郭靖" : "你也知道郭伯伯的名字嘛，他的功夫最是厉害，我要是能学上那么两……\n",
		"郭芙" : "她对我一向很好，我准备将来讨她做老婆。\n",
		"郭襄" : "这个小丫头，整天不知道在想些什么。\n",
		"黄蓉" : "郭伯母的名字不要随便乱叫！\n",
		"洪七公" : "他老人家最爱吃叫化鸡了，这次出来我本来是要给他带只叫化鸡回去的。\n",
		"武敦儒" : "他也喜欢郭芙，只是我们都不知道郭芙心里到底怎么想。\n",
		"武三通" : "你认得家父？可有他老人家的消息？\n",
		"一灯大师" : "咱们皇爷的一阳指名闻天下。\n",
		"老婆" : "抱歉，和你不熟悉，我不想对你说。\n",
		"rumors" : "听说蒙古人又要来打襄阳了。\n",
	]));
        set("chat_chance", 5);
        set("chat_msg", ({
                "武修文念道：“要能学得郭伯伯两三成功夫，那也可也...\n",
                "武修文喃喃自语：“不知郭芙心里到底怎么想的……\n",
                 (: random_move :)
        }) );

        setup();
        carry_object(ARMOR_D("changshan"))->wear();
        carry_object(ARMOR_D("shoes"))->wear();
}

int ask_ji()
{
	object ob;
	ob=this_player();
	if (ob->query("shen")<=0){
		command("?");
		command("say 看你满脸戾气就不象好人，我们认识嘛？\n");
		return 1;
	}
	if (ob->query_temp("marks/jiaohua-ji") || ob->query_temp("marks/lost-ji") ){
		command("look "+ob->query("id")+"\n");
		command("say 刚刚在扬州..在扬州，我把叫化鸡弄丢了!我不敢去见老爷子了!\n");
		command("cry "+ob->query("id")+"\n");
		ob->delete_temp("marks/jiaohua-ji",1);
		ob->set_temp("marks/lost-ji",1);
		return 1;
	} else {
		command("dunno");
		return 1;
	}
}