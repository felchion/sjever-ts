// /u/mr/npc/feng.c 风波恶
// this is made by beyond
// modify by leontt 2000/10/26
// Modify by Lklv 2001.9.20

inherit NPC;
inherit F_MASTER;
string ask_yan();

void create()
{
        set_name("风波恶",({"feng boe","feng","boe"}));
        set("title","玄霜庄庄主");
        set("nickname","一阵风");
        set("long",
              "他就是姑苏慕容玄霜庄的庄主。\n");
        set("age", 25);
        set("attitude", "heroism");
        set("unique", 1);
        set("shen_type", 1);
        set("str", 26);
        set("int", 26);
        set("con", 26);
        set("dex", 26);

        set("max_qi", 2000);
        set("max_jing", 2000);
        set("neili", 2500);
        set("eff_jingli", 2000);
        set("max_neili", 2500);
        set("jiali", 100);
        set("combat_exp", 850000);
        set("yan", 3);
        set("score", 85000);
				add_money("silver",10);
        set_skill("strike",130);
        set_skill("dodge",130);
        set_skill("force", 130);
        set_skill("shenyuan-gong", 130);
        set_skill("yanling-shenfa", 130);
        set_skill("parry", 130);
        set_skill("xingyi-zhang", 130);
        set_skill("literate", 130);

        map_skill("force", "shenyuan-gong");
        map_skill("dodge", "yanling-shenfa");
        map_skill("parry", "xingyi-zhang");
        map_skill("strike", "xingyi-zhang");
        prepare_skill("strike", "xingyi-zhang");

        setup();

        carry_object(__DIR__"obj/cloth2")->wear();

        create_family("姑苏慕容",3,"弟子");
	set("inquiry", ([
		"name" : "我就是姑苏慕容玄霜庄庄主风波恶。\n",
		"here" : "这里是大名鼎鼎的燕子坞，难道你没有听说过？\n",
		"rumors" : "慕容公子的“以彼之道，还施彼身”天下闻名。\n",
		"阿珠" : "阿珠住在听香水榭，是慕容公子的贴身丫鬟，跟阿碧两个人一同侍奉公子。\n",
		"阿碧" : "阿碧姑娘住在琴韵小筑，所有要拜见慕容公子的人都要由她那里经过。\n",
		"王语嫣" : "王姑娘是慕容公子的表妹，不知道什么时候他们成亲。\n",
		"邓百川" : "邓百川是我大哥，是燕子坞青云庄的庄主。\n",
		"公孙乾" : "公孙乾是我二哥，是燕子坞赤霞庄的庄主。\n",
		"包不同" : "包不同是我三哥，是燕子坞金凤庄的庄主，他老是喜欢说“非也，非也”。\n",
		"听香水榭" : "听香水榭是阿珠姑娘住的地方，离这里不远。\n",
		"琴韵小筑" : "琴韵小筑是阿碧姑娘住的地方，坐小舟有三九水路就到了。\n",
		"曼佗罗山庄" : "曼佗罗山庄是公子的姑妈住的地方，平时连我都不敢去。\n",
		"还施水阁" : "这是燕子坞的秘密之所在，我可不便对你说。\n",
		"飞燕" : (: ask_yan :),
		"feiyan" : (: ask_yan :),
	]));
	set("chat_chance", 2);
	set("chat_msg",({
		"风波恶道：“语嫣姑娘真是天下无不知，那天真是让我开了眼界。”\n",
		"风波恶道：“要不是公子的玉露清新散三哥的女儿包不靓的命就没了。”\n",
	}));
}

void attempt_apprentice(object ob)
{
        command("say 嗯，看你还是个学武的料，我就收下你吧！");
        command("recruit " + ob->query("id"));
        ob->set("title","姑苏慕容第四代弟子");
}

string ask_yan()
{
        object ob = this_player(),obj;

        if (ob->query("family/family_name") != "姑苏慕容")
                return RANK_D->query_respect(ob) + "与本派素无来往，不知此话从何谈起？";

        if (ob->query("combat_exp") < 300000)
                return RANK_D->query_respect(ob) + "功夫太差，给你也是浪费，还是留给其他人吧。";

        if (this_player()->query("max_neili") < 800)
                return RANK_D->query_respect(ob) + "功力不够，过段时间再来吧。";

        if (present("fei yan", ob))
                return RANK_D->query_respect(ob) + "你现在身上不是有吗，怎么又来要了？真是贪得无厌！";

        if (ob->query_temp("mr/feiyan") > 0)
                return RANK_D->query_respect(ob) + "你刚刚要过，怎么又来要了？真是贪得无厌！";

        if (query("yan") < 1) return "对不起，飞燕已经发完了。";

        obj = new("/clone/weapon/feiyan");
        obj->move(ob);
        ob->set_temp("mr/feiyan", 1);
        add("yan", -1);

        call_out("delete_yan", 30, ob);

        message_vision("$N得到一个飞燕。\n", ob);
        return "好吧，看在你对慕容这么忠心，这个飞燕就送给你吧。";
}

void delete_yan(object me)
{
        if ( objectp(me) ) me->delete_temp("mr/feiyan");
}
