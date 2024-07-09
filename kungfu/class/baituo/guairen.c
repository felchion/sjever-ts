// guairen.c 欧阳锋
// by snowman 12/02/98
// Modified by snowman@SJ 2000.

#include <ansi.h>
inherit NPC;
void create()
{
       set_name("怪人", ({ "guai ren", "ren", "man" }) );
       set("gender", "男性" );
       set("shen_type", -1);
       set("age", 70);
       set("per", 20);
       set("long","一位用头支在地上，双脚并拢，撑向天空的怪人。
看上去疯疯癫癫的。但见他高鼻深目，满脸雪白短
须，根根似铁，又听他喃喃自语，说著叽哩咕噜的
怪话，极是难听。\n" );
       set_temp("hide_level", 45+random(6));	
       set("inquiry", ([
                "欧阳锋" : "欧阳锋、欧阳锋，听起来这名字挺熟的。。。\n",
                "洪七公" : "洪七公，这名字也熟。\n",
                "蛤蟆功" : "蛤蟆、蛤蟆、蛤蟆。。。\n",
                "name" : "我…我是谁？我知道我是谁就好啦。\n",
       ]));
       setup();
       carry_object(BINGQI_D("gangzhang"));
       carry_object(ARMOR_D("cloth"))->wear();
       add_money("silver", 10);
}

void kill_ob(object me)
{     
      me->remove_all_enemy();
      this_object()->remove_all_enemy();
}

void do_back(object me)
{     
      if(!me || environment(me) != environment(this_object())) return;
      message_vision(HIY"\n 空中忽然几声雕唳，两头大雕在半空飞掠而过。\n\n"NOR,me);
      call_out("do_leave", 5, me);             
}

void init()
{
	object *ob, me = this_player();
	int i;
	::init();

	//if( wizardp(me) || SECURITY_D->get_status(me) == "(immortal)" ) return;

	if( base_name(environment(this_object()))!="/d/jiaxing/river1" ) return;

	if( me->query_temp("oyf/ok") ){
		command("look " + me->query("id"));
		command("hehe");
		command("say 你越是东奔西跑，身上的毒越是发作得快。");
		call_out("do_wait", 360, me);
	}
	else if(sizeof(ob = all_inventory(environment(this_object()))) > 2){
		//command("say inv="+sizeof(ob));
		for(i=0; i<sizeof(ob); i++) {
			if( ob[i]==this_object() || ob[i]==me ) continue;
			if(wizardp(ob[i]) || SECURITY_D->get_status(ob[i]) == "(immortal)")
			log_file("quest/hamagong", sprintf("当%8s%-10s非法进入欧阳峰处时，%8s%-10s在现场。\n",
			me->name(1),"("+me->query("id")+")", ob[i]->name(1), "("+ob[i]->query("id")+")" ), me);
		}
	}
	add_action("do_kneel","kneel");
	add_action("do_kneel","gui");
	add_action("do_turn","turn");
	add_action("do_jiao","jiao");

}

int do_kneel(string arg)
{
	object me, oyf;
	int number;
	me = this_player();
	oyf = this_object();

	number = me->query_temp("oyf/kneel");

	if( !me->query_temp("oyf/ok") || !arg) return 0;

	if(arg == "guai ren" || arg == "ren" || arg == "man") {
		if(!number) {
			message_vision(CYN"\n$n猛然福至心灵，向$N双膝跪倒，叫道：「求老公公救我性命。」\n"NOR,oyf,me);
			me->set_temp("oyf/kneel", 1);
			if ((string)me->query("gender") == "女性") {
				command("hmm " + me->query("id"));
				command("say 好吧，我就帮你解了这毒。");
				message_vision("只见$N翻过身来，捏住$n手臂推拿几下，已经将$n中的冰魄神针毒解了。\n",oyf,me);
				me->clear_condition("bing_poison");
				call_out("do_back", 2, me);
			}
			else 	message_vision("怪人摇了摇头道：「难救，难救！」\n",me);
		}
		else if (number == 1) {
			message_vision(CYN"\n$n跪在地上对$N道：「你本事这麽大，定能救我。」\n"NOR,oyf,me);
			message_vision("$N甚是高兴，微微一笑，道：「你怎知我本事大？」\n",oyf);
			me->set_temp("oyf/kneel", 2);
		}
		else {
			message_vision(CYN"\n$n忙对$N道：「你倒转了身子还跑得这麽快，天下再没第二个及得上你。」\n"NOR,oyf,me);
			message_vision("$N哈哈大笑，声震林梢，叫道：「好！好！娃娃倒(turn)过身来，让我瞧瞧。」\n",oyf);
		}
	}
	return 1;

}

int do_turn()
{
	object me, oyf;
	int age;
	me = this_player();
	oyf = this_object();
	if( me->query_temp("oyf/kneel") != 2) return 0;

	tell_object(me, HIC "你心想不错，自己直立而他倒竖，确是瞧不清楚，他即不愿顺立，只有自己倒立了。\n" NOR );
	message_vision("\n$n当下倒转身子，将头顶在地下，用右手牢牢的在旁撑住。向$N求道：「好公公，你救救我。」\n\n", oyf, me);

	
	age = me->query("age")-me->query("age_reyoung");

	if(age < 18 ) call_out("oyf_ask", 4, me);
	return 1;
}

void oyf_ask(object me)
{
	if(!me || environment(me) != environment(this_object()) ) return;
	message_vision("$N斜眼瞧向$n，脸有喜色，显得极是满意。\n",this_object(),me);
	command("say 好，救你不难，但你须得答应我一件事。");
	command("say 我要收你为义儿。不许叫我公公，要叫(jiao)爸爸。");
	me->set_temp("oyf/say", 1);
	call_out("do_wait", 320, me);
}

void do_wait(object me)
{
	if(!me || environment(me) != environment(this_object()) ) return;
	if(!me->query_temp("oyf_son")){
		command("hmm");
		do_back(me);
	}
}

int do_jiao(string arg)
{
	object me, oyf;
	me = this_player();
	oyf = this_object();
	if( me->query_temp("oyf/kneel") != 2 || !arg
	|| !me->query_temp("oyf/say") ) return 0;

	if(me->query_temp("oyf/say") >=4 ){
		do_wait(me);
		return 1;
	}
	me->add_temp("oyf/say", 1);

	if(arg == "爸爸"){
		message_vision(HIC"$N心中一动，真情流露，一声“爸爸，爸爸”冲口而出！\n"NOR, me );
		remove_call_out("do_wait");
		me->delete_temp("oyf/say");
		call_out("do_cure", 4, me);
	}
	else {
		command("hmm");
		command("say 快叫爸爸。");
		me->add_temp("oyf/say", 1);
	}
	return 1;
}

void do_cure(object me)
{
	object oyf = this_object();

	if(!me || environment(me) != environment(this_object())) return;

	command("haha");
	command("say 乖儿子，来，我给你除去身上的毒气。");
	message_vision(CYN"\n只见$N翻过身来，捏住$n手臂推拿几下，已经将$n中的冰魄银针毒解了。\n"NOR,oyf,me);
	me->clear_condition("bing_poison");
	command("say 好啦！毒已经解了，为父再把生平最得意的武功《蛤蟆功》也传给你。");
	tell_object(me, HIC "\n怪人开始给你讲解蛤蟆功的入门功夫。。。\n" NOR );
	me->set("oyf_son", 1);
	me->start_busy(5);
	call_out("do_teach", 5, oyf, me);

}

int add_exp(object me)
{
	int i,lvl=1;
	mapping skills;
	string *sk;

	skills = me->query_skills();
	if (mapp(skills)) {
		sk = keys(skills);
		for (i=0;i<sizeof(sk);i++)
		skills[sk[i]]+=lvl;
	}
	me->add("combat_exp",me->query("combat_exp")/(50-lvl)+lvl);
	tell_object(me, "你的功夫提高了！\n");
	return 1;
}

void do_teach(object oyf, object me)
{
	if(!me || environment(me) != environment(this_object())) return;


	tell_object(me, HIC "\n怪人开始给你讲解蛤蟆功的入门功夫。。。\n" NOR );
	log_file("quest/hamagong", sprintf("%8s%-10s成为欧阳峰义子，并学会hmg。\n",
	me->name(1),"("+me->query("id")+")"  ), me);
	me->set("quest/hama/pass", 1);
	me->set_skill("hamagong", 10);
	me->delete("quest/hama/start");
	me->delete("quest/hama/move");
	add_exp(me);
	tell_object(me, "你学会了蛤蟆功！\n");
	command("chat 哈哈哈哈！我找到我的乖儿子啦！");

	call_out("do_back", 15, me);
}

void oyf_cure1(object me, object oyf)
{

	if(!me || !oyf || environment(me) != environment(this_object()) ) return;

	command("hmm");
	command("say 看起来你这娃娃还不赖，我就帮你解了毒吧。");
	message_vision(CYN"\n只见$N翻过身来，捏住$n手臂推拿几下，已经将$n中的冰魄银针毒解了。\n"NOR,oyf,me);
	me->clear_condition("bing_poison");
	command("say 好啦！毒已经解了。");
	call_out("do_back", 5, me);
}


void do_leave(object me)
{
	object oyf = this_object();

	if(!me || !oyf || environment(me) != environment(this_object()) || !living(me)) return;

	me->delete_temp("oyf");

	message_vision("$N两眼呆望，以手击额，皱眉苦苦思索，突然间似乎想起了甚麽，登时脸色大变。\n",oyf);
	command("whisper " + me->query("id") + " 我好象记得一个叫“白陀山”的地方，但、但。。。");
	if(me->query("oyf_son"))
	command("whisper " + me->query("id") + " 好象是个大戈壁，然后往西五百里，往北六百里。");
	command("chat* pat " + me->query("id"));
	message_vision("\n接着$N一步跨了出去。这一步迈得好大，待得第二步跨出，人已在丈许之外，连跨得十来步，身子早在桑树林後没了。\n\n",oyf);
	if(!me->query("oyf_son")){
		me->set("quest/hama/fail", 4);
		tell_object(me, "你被奖励了50000点经验点！\n");
		me->add("combat_exp", 50000);
		me->add("max_neili", 200);
		tell_object(me, "你的内功修为提高了两百点！\n");
		log_file("quest/hamagong", sprintf("%8s%-10s见到欧阳峰，并得到奖励。\n",
		me->name(1),"("+me->query("id")+")" ), me);
	}
	destruct(oyf);
}
