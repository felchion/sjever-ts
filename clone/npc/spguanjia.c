/**********************************************************************************
//
//      spguanjia.c
//      Created by mychat 02/05/2004
//      保留我的签名情况下 free of use.
//      当然更欢迎修改、优化等。
//
//      Email & MSN: codemake@163.com
//      文件位于/clone/npc下
// 		处理出租房间换招牌的问题
***********************************************************************************/

inherit NPC;

void create()
{
	set_name("商铺管家", ({ "shangpu guanjia" }));

	set("gender", "男性");
	set("age", 65);
	set("no_quest", 1);
	set("no_drop", 1);
	set("no_get", 1);
	set("gender", "男性");

	set("combat_exp", 7500);
        set_skill("unarmed", 35);
        set_skill("force", 35);
        set_skill("dodge", 40);
        set_temp("apply/attack", 30);
        set_temp("apply/defense", 40);

	set("combat_exp", 5000000);

	set("inquiry", ([
		"name": "我是帮韦爵爷干活的，到处看看自家出租的店铺买卖是否还好",
	]) );
	set("attitude", "friendly");

	setup();
	set_weight(0);
}

void init()
{
	::init();
	if(query("work_over")) return;
	set("work_over",1);
	if ( base_name(environment())==query("target") )
		if( environment()->change_rsname() ) message_vision("只见$N冲着伙计吆喝：换老板了，还快不把招牌换了。\n", this_object());	
	call_out("dest", 10, this_object());
}

void kill_ob(object ob)
{
	ob->remove_killer(this_object());
	remove_killer(ob);
	message_vision("$N沮丧地发现商铺管家好像不知道发生了什么事，继续在忙自己的事。\n", ob);
}

void unconcious()
{
	reincarnate();
	set("eff_qi", query("max_qi"));
	set("qi", query("max_qi"));
	set("eff_jing", query("max_jing"));
	set("jing", query("max_jing"));
	set("jingli", query("eff_jingli"));
	say("商铺管家苦笑着说：我只是帮韦爵爷干活的小人物，大侠何苦和我一般见识\n");
	command("hehe");
}

void die()
{
	unconcious();
}


void dest(object ob)
{
	if ( !ob ) return;
    	if (ob->is_character() ){
    		if( environment(ob))
       			message_vision("只见$N忙完手中的事情，不慌不忙地走了。\n", ob);  
       	}
       	
       	destruct(ob);
}