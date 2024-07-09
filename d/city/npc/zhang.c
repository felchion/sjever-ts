// /d/city/npc/zhang.c 张帐房
// by snowman@SJ 12/02/2000

inherit NPC;
string ask_me();
void create()
{
	set_name("张帐房", ({ "zhang zhangfang", "zhang", "zhangfang" }));
	set("title", "扬州知府帐房主管");
	set("gender", "男性");
	set("age", 63);
	set("str", 20);
	set("dex", 20);
	set("long", "他是扬州现任知府帐房主管。任何公事上的帐务都由他来管。\n");
	set("combat_exp", 2000);
	set("shen_type", 0);
	set("attitude", "peaceful");

	set_skill("unarmed", 10);
	set_skill("force", 10);
	set_skill("sword", 10);
	set_skill("dodge", 10);
	set_skill("parry", 10);

	set("neili", 200); 
	set("max_neili", 200);
	set("jiali", 1);

       	setup();
       	carry_object(ARMOR_D("cloth"))->wear();
}

void destroy(object ob)
{
        if(ob) destruct(ob);
        return;
}


int accept_object(object me, object obj)
{
	int i,j;

	if (!obj->query("money_id")
	|| (!me->query_temp("name_wait_modify")
	 && !me->query_temp("title_wait_modify")))
		return 0;

	j = me->query("registered")<3?2000000:1000000;

	if (me->query_temp("name_wait_modify")) {
		if(i = obj->value() >= j * (me->query("name_modify")+1)){
			command("nod " + me->query("id"));
			command("say "+ me->name()+"交纳足够税金，记入帐薄。");
			me->delete_temp("name_wait_modify");
			me->set_temp("name_modify_paid", 1);
			call_out("destroy", 1, obj);
			return 1;
		}
		else {
			command("say 这点？可不够哦。");
			return 0;
		}
	}
	if (me->query_temp("title_wait_modify")) {
		if(i = obj->value() >= j * 2 * (me->query("title_modify")+1)){
			command("nod " + me->query("id"));
			command("say "+ me->name()+"交纳足够税金，记入帐薄。");
			me->delete_temp("title_wait_modify");
			me->set_temp("title_modify_paid", 1);
			call_out("destroy", 1, obj);
			return 1;
		}
		else {
			command("say 这点？可不够哦。");
			return 0;
		}		
	}
	command("say 没事给我钱干嘛，我这就先收下了。");	
	return 1;

}

void kill_ob(object ob)
{
      ob->remove_killer(this_object());
      remove_killer(ob);
}
