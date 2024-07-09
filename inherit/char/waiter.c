
inherit NPC;

void setup()
{
	::setup();
	set("no_quest",1);
	set("no_ansuan",1);
	set("no_bark",1);
}

void fight_ob(object me)
{
	me->remove_enemy(this_object());
	remove_enemy(me);
}

void kill_ob(object me)
{
	me->remove_killer(this_object());
	remove_killer(me);
	me->remove_enemy(this_object());
	remove_enemy(me);
}

void unconcious()
{
        say( "小二大喊一声：“我不行了，快来接班！”\n");
        say( "结果从楼上又走下来一个小二来，说：“没问题，交给我吧。”\n");
        say( "前一个小二快快乐乐地离开了。\n");
        reincarnate();
        set("eff_qi", query("max_qi"));
        set("qi", query("max_qi"));
        set("eff_jing", query("max_jing"));
        set("jing", query("max_jing"));
        set("jingli", query("eff_jingli"));
}

void die()
{
        unconcious();
}