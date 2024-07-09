// zhou-zhiruo.c
// Lklv 2001.10.18 update hehe 小妞

inherit F_MASTER;
inherit NPC;

int ask_zhang();
int ask_dragon();

void create()
{
	set_name("周芷若", ({ "zhou zhiruo", "zhou", "zhiruo"}));
	set("long", "她是灭绝师太的八大俗家第子之一，一副清色衣衫，显的楚楚动人。\n");
	set("gender", "女性");
	set("attitude", "friendly");
	set("age", 25);
	set("shen_type", -1);
	set("str", 42);
	set("int", 42);
	set("con", 42);
	set("dex", 42);
	set("per", 40);

        set("max_qi", 2000);
        set("max_jing", 2000);
        set("neili", 2000);
        set("max_neili", 2000);
        set("jiali", 50);
        set("combat_exp", 1500000);
        set("score", 100);

        set("chat_chance_combat", 40);
        set("chat_msg_combat", ({
                (: perform_action, "claw.jiuyin" :)
	}));

        set_skill("literate", 180);
	set_skill("sword", 80);
        set_skill("parry", 180);
        set_skill("dodge", 180);
        set_skill("force", 180);
        set_skill("strike", 180);
        set_skill("claw", 180);
        set_skill("anying-fuxiang", 180);
        set_skill("jiuyin-baiguzhua", 180);
	set_skill("huifeng-jian", 80);
        set_skill("dacheng-fofa", 180);
        set_skill("linji-zhuang", 180);

        set("apply/attack", 50);
        set("apply/dexerity", 50);

        map_skill("force", "linji-zhuang");
        map_skill("parry", "jiuyin-baiguzhua");
        map_skill("claw", "jiuyin-baiguzhua");
        map_skill("sword", "huifeng-jian");
        map_skill("dodge", "anying-fuxiang");
        prepare_skill("claw", "jiuyin-baiguzhua");

        set("inquiry",([
		"张无忌" : (: ask_zhang :),
//		"赵敏" : (: ask_zhao :),
		"光大峨嵋" : (: ask_dragon :),
		"光大我派" : (: ask_dragon :),
        ]) );
        create_family("峨嵋派", 4, "弟子");

        setup();
        carry_object("/d/emei/obj/shoes")->wear();
        carry_object("/d/emei/obj/cloth")->wear();
}

void attempt_apprentice(object ob)
{
        if (! ob->query("family")) {
		command("say 你非我峨嵋派弟子，我不能收你。\n");
		return;
        }
        if ((int)ob->query_skill("claw", 1) < 41 ) {
		command("say 你基本爪法火候未到，我不能收你。\n");
		return;
        }
        if ((int)ob->query_skill("strike", 1) < 41 ) {
		command("say 你基本掌法火候未到，我不能收你。\n");
		return;
        }
	if (ob->query("class") == "bonze"){
		command ("blush");
		command ("say 我只收俗家弟子。");
		return;
	}
        if ((int)ob->query_skill("jiuyin-baiguzhua", 1) < 41 ) {
		command("say 我现在不想收徒。\n");
		return;
        }
        command("recruit " + ob->query("id"));
        command("chat* grin");
        command("grin");
}

int ask_zhang()
{
	object me=this_player();
	if( me->query_temp("jiuyin140") == 2 ) {
		command("blush");
		command("say 虽然我已经发誓不再喜欢他，但他的影子无时无刻不出现在我的脑海里。\n");
		command("say 我终究不能欺骗自己，更不能欺骗师父，师傅教给我的倚天屠龙的秘密和光大我派的想法终究是不能实现了。\n");
		command("sigh");
		me->set_temp("jiuyin140", 3 );
		return 1;
	}
	message_vision("周芷若瞪了$N一眼。\n", me);
	return 1;
}

int ask_dragon()
{
	object me=this_player();
	if( me->query_temp("jiuyin140") >= 3 ) {
		if( me->query("family/master_name") != "周芷若"
	 	 && !me->query_skill("jiuyin-baiguzhua",1) ) {
			command("say 你现在不是我的徒弟，也没有从我这里学到什么东西。就凭你现在的武功很难做到武林至尊的，也就更谈不上光大峨嵋了。\n");
			return 1;
		}
		command("say 既然如此，你到我师傅那里询问倚天屠龙的秘密吧。\n");
		command("say 光大峨嵋的希望只能寄托与你了。\n");
		command("sigh");
		me->set_temp("jiuyin140", 4 );
		return 1;
	}
	message_vision("周芷若淡淡地看了$N一眼，摇了摇头。\n", me);
	return 1;
}
