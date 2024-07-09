// Room: /d/huashan/jinshe/obj/jinshemiji.c
// Date: Look 99/03/25

inherit ITEM;

void create()
{
	set_name("金蛇秘籍", ({ "jinshe miji", "jinshe","miji" }));
	set_weight(900);
	if( clonep() )
		set_default_object(__FILE__);
	else {
		set("unit", "本");
		set("long","这是一本金蛇郎君夏雪宜的武学秘籍，上面写满密密麻麻的小字。\n");
		set("material", "paper");
		set("no_drop", "这样东西不能离开你。\n");
		set("no_get", "这样东西不能离开那儿。\n");
		set("no_give", 1);
		set("no_put", 1);
		set("unique", 1);
		set("treasure", 1);
	}
}

void init()
{
	add_action("do_du", "read");
}

int do_du(string arg)
{
	object me = this_player();
	int jlevel; 
	int zlevel;
	int neili_lost;

	if (!(arg=="jianfa" || arg=="zhangfa"))
		return 0;

	if (arg=="jianfa"){       
		if (me->is_busy()) {
			write("你现在正忙着呢。\n");
			return 1;
		}
		if( me->is_fighting() ) {
			write("你很难在战斗中分心研读秘籍！\n");
			return 1;
		}
		if( !me->query_skill("literate", 1) ){
			write("你是个文盲，先学点文化(literate)吧。\n");
			return 1;
		}
		if (me->query("str") >20 ){
			write("你先天膂力太高，无法领悟金蛇剑法的要领。\n");
			return 1;
		}
		if( (int)me->query("jing") < 30 ) {
			write("你太疲倦了，无法研读武学秘籍。\n");
			return 1;
		}
		if (me->query_skill("sword",1)<30){
			write("你的基础功没有打好，不能学习金蛇剑法。\n");
			return 1;
		}
		if( (int)me->query("neili") < neili_lost) {
			write("你内力不够，无法钻研这么高深的武功。\n");
			return 1;
		}
		jlevel = me->query_skill("jinshe-jianfa", 1);
		if( (int)me->query("combat_exp") < (int)jlevel*jlevel*jlevel/10 ) {
			write("你的实战经验不足，再怎么读也没用。\n");
			return 1;
		}
		if( me->query_skill("jinshe-jianfa", 1) >= 101){
			write("你已经无法再从秘籍中学到什么，要想更高的修为必须自已练了。\n");
			return 1;
		}
		neili_lost=(int)(me->query_skill("jinshe-jianfa",1)/15);
		me->receive_damage("jing", 20);
		me->set("neili",(int)me->query("neili")-neili_lost);
		me->improve_skill("jinshe-jianfa", (int)me->query_skill("literate", 1)/3+1);
		if( ! me->query("jinshe/jianfa"))
			me->set("jinshe/jianfa", 1);
		message("vision", me->name() + "拿着一本秘籍认真研读。\n", environment(me), me);
		write("你研读《金蛇剑法》，颇有心得。\n");
		return 1;
	}
	if (arg=="zhangfa") {
		if (me->is_busy()) {
			write("你现在正忙着呢。\n");
			return 1;
		}
		if( me->is_fighting() ) {
			write("你很难在战斗中分心研读秘籍！\n");
			return 1;
		}
		if (me->query("str") >20 ){
			write("你先天膂力太高，无法领悟金蛇掌法的要领。\n");
			return 1;
		}
		if( !me->query_skill("literate", 1) ){
			write("你是个文盲，先学点文化(literate)吧。\n");
			return 1;
		}
		if( (int)me->query("jing") < 20 ) {
			write("你太疲倦了，无法研读武学秘籍。\n");
			return 1;
		}
		if (me->query_skill("strike",1)<30){
			write("你的基础没有打好，不能研读金蛇秘籍。\n");
			return 1;
		}
		if( (int)me->query("neili") < neili_lost) {
			write("你内力不够，无法研读这么高深的武功。\n");
			return 1;
		}
		zlevel = me->query_skill("jinshe-zhangfa", 1);
		if( (int)me->query("combat_exp") < (int)zlevel*zlevel*zlevel/10 ) {
			write("你的实战经验不足，再怎么读也没用。\n");
			return 1;
		}
		if( me->query_skill("jinshe-zhangfa", 1) >= 101){
			write("你已经无法再从秘籍中学到什么，要想更高的修为必须自已练了。\n");
			return 1;
		}
		neili_lost=(int)(me->query_skill("jinshe-zhangfa",1)/15);
		me->receive_damage("jing", 20);
		me->set("neili",(int)me->query("neili")-neili_lost);
		me->improve_skill("jinshe-zhangfa", (int)me->query_skill("literate", 1)/3+1);
		if( ! me->query("jinshe/zhangfa"))
			me->set("jinshe/zhangfa", 1);
		message("vision", me->name() + "拿着一本秘籍认真研读。\n", environment(me), me);
		write("你研读《金蛇游身掌》，颇有心得。\n");
		return 1;
	}
}
