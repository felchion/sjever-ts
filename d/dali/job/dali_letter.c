#include <ansi.h>
#include <get_place.h>
#include <job_mul.h>
#include <jobround.h>

inherit ITEM;

string do_look();
int check(object ob);

void create()
{
	set_name(RED"信件"NOR, ({ "letter","xin","secret letter" }));
	set_weight(10);
	if( clonep() )
		set_default_object(__FILE__);
	else {
		set("unit", "封");
		set("long","这是一封信件，镇南王亲笔手谕，信封的很严实，封口盖着红漆印戳。\n");
		set("value", 0);
		set("material", "paper");
		set("no_drop",1);
	}
	set("long", (: do_look :));
}

void init()
{
	call_out("dest", random(200)+1200);
	add_action("do_send", "songxin");
}

string do_look()
{
	int now_time, time;
	object me;
	string ret;
	string where;
	
	me = this_player();
	where = me->query_temp("songxin/where");
//	if (me->query_temp("songxin/twice") && strlen(where) < 14)
//		where = "\n"+CHINESE_D->font(me->query_temp("songxin/where"))+"\n";
//	if (where == "错误的输入字符串！")
//		where = me->query_temp("songxin/where");

	now_time = me->query_condition("dali_songxin");
	time = me->query_temp("songxin/time");

	if( me->query_temp("dali_songxin")) {
		ret = "\n这是一封由大理国镇南王发出的信件，上面写着\n\n"
		"    「"HIR + where + " " +me->query_temp("songxin/target_name") + NOR"」 亲启。\n\n"NOR;
		if (now_time > time*3/4) ret += "你现在有充足的时间去送信。\n";
		else if (now_time > time/2 && now_time <= time*3/4) ret += "已经过去一小半时间了。\n";
		else if (now_time > time/4 && now_time <= time/2) ret += "你还有一半的时间去送信。\n";
		else if (now_time > time/8 && now_time <= time/4) ret += "你的时间已经不多了。\n";
		else if (now_time > 0 && now_time <= time/8) ret += "时间将至，任务即将失败。\n";
	}
	return ret;
}

object other;

int do_send(string arg)
{
	int exp, exps, pot, job_time, i, j, time, times,getshen;
	object me, target, ob, *living, obj;
	string t_name, str, m_name,where;

	me = this_player();
	if( me->is_busy() || me->is_fighting() )
		return notify_fail("你正忙着呢。\n");
	if( !arg ) return notify_fail("你要送给谁？\n");
	if(!objectp(target = present(arg, environment(me))))
		return notify_fail("你要送给谁？\n");

	t_name = target->query("name");
	job_time = me->query_temp("songxin/time");

	if( !living(target) )
		return notify_fail("看清楚点，那是活人吗？！\n");
	if( userp(target) )
		return notify_fail("嘿嘿，想作弊？！\n");
	if( me->query_temp("dali_songxin_late"))
		return notify_fail("你任务已经失败了，重新再领一个吧。\n");
	if( !wizardp(me) && !me->query_temp("dali_join") )
		return notify_fail(t_name+"道：你不是大理国的随从，怎么会送镇南王手谕的信给我？\n");
	if( !me->query_temp("dali_songxin"))
		return notify_fail(t_name+"道：这是大理国镇南王府亲自交给你的？你是不是从别人那里抢来的？\n");
	if( me->query_temp("songxin/target_name") != t_name)
		return notify_fail(t_name+"道：这封信不是给我的，你是不是送错了？\n");
	if (me->query_temp("songxin/areab") != target->query("startroom"))
		return notify_fail(t_name+"奇怪道：你看清楚，是否送错人了，和我同名的人可不少哦。\n");

	me->add_busy(random(2)); // hehe, 防止利用bug，简单有效 :) snowman@SJ

	m_name = me->query_temp("songxin/twice") ? me->query_temp("songxin/twice"): "段王爷";

	message_vision(HIY"$N擦了一把额头的汗，从怀中掏出信交给$n说道，这是"+m_name+"让在下送给您的信，请您收好。\n"NOR, me, target);
	message_vision(HIY "$N接过信看了看，点了点头说道：这位"+RANK_D->query_respect(me)+"辛苦你了。\n"NOR, target, me);

	if (!me->query_temp("zuji"))
		exp = (4+random(3))* 10 + job_time/8;
	else if (me->query_temp("songxin/twice"))
		exp = (6+random(5))* 25 + job_time/3;
	else
		exp = (6+random(5))* 25 + job_time/3; //这里原来是8
		
	if(me->query("combat_exp")<950000 && !me->query_temp("songxin/twice") && me->query_temp("zuji") ) exp += 25+random(50);//exp < 700K 成长快些好了 hehe
		
	if(exp > 250) exp = 200 + random(50);

	if (me->query_temp("songxin/twice") && me->query_temp("zuji"))	{
		pot = 60+random(30);	
		exp = exp + 25* me->query("bonus/送信");
	}
	else
		pot = 40+random(20);	

	times = uptime() - me->query_temp("songxin/times");

	if(times<1) times = 1;
	if ( exp/ times > 35/18.0 && me->query_temp("zuji"))
		exps = to_int(35/18.0 * times);
	// add	
	if(exp > 1000) exp = 950 + random(50);
		
	exp = me->add_exp_combat(exp,target,"送信");
	
	
	me->add("potential",pot);
	if(me->query("potential") > me->query("max_pot"))
		me->set("potential",me->query("max_pot"));
			
	getshen = 100 + random((int)me->query("age",1)) + random((int)me->query("job_time/送信",1)/15);
	if( me->query("family") && me->query("family/family_name") =="桃花岛") getshen = 0;
	if (me->query("shen")<0) getshen=-getshen;
	me->add("shen", getshen );
	
	
	
	me->add("job_time/大理", 1);
	
	log_file( "job/songxin",
		sprintf("%8s%-10s第%-5d次(%d)送信奖励%4d(%3d)点经验%3d点潜能，受阻：%d(%4d)。Now exp：%d。\n",
			me->query("name"),
			"("+me->query("id")+")",
			me->query("job_time/大理"),
			me->query_temp("songxin/twice")?2:1,
			exp,
			exps,
			pot,
			me->query_temp("zuji"),
			times,
			me->query("combat_exp")
		), me
	);

	tell_object(me,HIW"好！任务完成，你被奖励了：" +
		chinese_number(exp) + "点实战经验，" +
		chinese_number(pot) + "点潜能。"+
		CHINESE_D->chinese_number(getshen)+"点"+(getshen<0?"":"正")+"神。\n" +
		"你为镇南王府成功送信"+chinese_number(me->query("job_time/送信"))+"次。\n"NOR);
				
	me->clear_condition("job_busy");
	me->clear_condition("dali_songxin");
	me->delete_temp("zuji");
	me->delete_temp("songxin_zuji");
	
	me->set("job_name", "大理送信");
	
	if (!me->query_temp("songxin/other")) {
		me->delete_temp("dali_songxin");
		me->delete_temp("songxin/time");
		me->delete_temp("songxin/times");
		me->delete_temp("songxin/area");
		me->delete_temp("songxin/target_name");
		me->delete_temp("songxin/where");
		me->delete_temp("letter_steal");
		me->set_temp("songxin_ok",1);		
		if (me->query_temp("songxin/twice")) {
			if(target->query("no_quest")) destruct(target);
			me->delete_temp("songxin/twice");
		}
		destruct(this_object());
	} else {
		living = livings();
		for (i=0;i < sizeof(living);i++){
			j = random(sizeof(living));
			if (j <= 1 || j == sizeof(living) -1) continue;
			if(check(living[j]) ){
				ob = living[j];
				str = environment(ob)->query("short");
				if( str == "武僧堂"
				 || str == HIR"心禅堂"NOR
				 || str =="地下黑拳市"
				 || str == "金刚伏魔圈" ) continue;
				if( environment(ob) == environment(target)) continue;
				if (get_place(base_name(environment(target))) == get_place(base_name(environment(ob)))) continue;
				if( get_place(base_name(environment(ob)))!="" ) break;
			}
		}

		if( !ob ) {
			if(target->query("no_quest")) destruct(target);
			me->delete_temp("dali_songxin");
			me->delete_temp("songxin/time");
			me->delete_temp("songxin/times");
			me->delete_temp("songxin/area");
			me->delete_temp("songxin/target_name");
			me->delete_temp("songxin/where");
			me->delete_temp("letter_steal");
			me->set_temp("songxin_ok",1);
			me->delete_temp("songxin/other");
			destruct(this_object());
			return 1;
		}

		time = random(10)+ 25;
		me->delete_temp("songxin_ok");
		me->delete_temp("dali_songxin_late");
		me->delete_temp("songxin_zuji");
		me->set_temp("dali_songxin", 1);
		me->set_temp("songxin/time", time);
		me->set_temp("songxin/times", uptime());
		me->apply_condition("dali_songxin", time*9/10+1);
		me->apply_condition("job_busy", time);
		me->apply_condition("task_busy", 6);
		
		me->delete_temp("zuji");
		me->delete_temp("letter_steal");

		obj = new(__DIR__"sxtarget");
		obj->set("startroom", ob->query("startroom"));
		obj->move(environment(ob));
		other = obj;
		me->set_temp("songxin/target_name", obj->query("name"));
		me->set_temp("songxin/area", load_object(ob->query("startroom"))->query("short"));
		me->set_temp("songxin/areab", ob->query("startroom"));
		me->set_temp("songxin/id", obj->query("id"));
		me->set_temp("songxin/where", get_place(me->query_temp("songxin/areab"))+me->query_temp("songxin/area"));
		me->set_temp("songxin/twice", get_place(base_name(environment(target)))+target->query("name"));
		where = me->query_temp("songxin/where");
//		if (strlen(where) < 14)
//			where = "\n"+CHINESE_D->font(me->query_temp("songxin/where"))+"\n";
//		if (where == "错误的输入字符串！")
//			where = me->query_temp("songxin/where");
		
			
		message_vision(HIY"$N顺手在信上勾画了几下，然后合上信封，又交还给了$n。\n"NOR, target, me);
		tell_object(me, GRN+target->name()+"在你的耳边悄声说道：你赶紧把它送到「"+HIY+where+GRN"」的「"+me->query_temp("songxin/target_name")+GRN"」手上。\n"NOR);
		
		if (me->query("bonus/送信") >= SX_JOB_ROUND || !me->query("bonus/送信") || me->query("bonus/送信")<1 )
		{
			if (me->query("bonus/送信") == SX_JOB_ROUND)
				me->set("bonus/送信",4);
			else
				me->set("bonus/送信",1);
		}
		else
			me->add("bonus/送信",1);  
			
		switch ((int)me->query("bonus/送信"))
	       	{
	       		case 1:
	       			tell_object(me, GRN+target->name()+"在你的耳边悄声说道：可能有个武功和你相比" + BLU + "微不足道" + GRN + "的家伙要来抢你的信，你应该随便对付几下就可以了吧。\n"NOR);
	       		break;
	       		case 2:
	       			tell_object(me, GRN+target->name()+"在你的耳边悄声说道：可能有个武功和你相比" + HIB + "马马虎虎" + GRN + "的家伙要来抢你的信，你及时送到应该没问题吧。\n"NOR);
	       		break;
	       		case 3:
	       			tell_object(me, GRN+target->name()+"在你的耳边悄声说道：可能有个武功和你相比" + CYN + "小有所成" + GRN + "的家伙要来抢你的信，你一路小心便是。\n"NOR);
	       		break;
	       		case 4:
	       			tell_object(me, GRN+target->name()+"在你的耳边悄声说道：可能有个武功和你相比" + HIC + "融会贯通" + GRN + "的家伙要来抢你的信，你可得小心对付哦。\n"NOR);
	       		break;
	       		case 5..7:
	       			tell_object(me, GRN+target->name()+"在你的耳边悄声说道：可能有个武功和你相比" + HIC + "颇为了得" + GRN + "的家伙要来抢你的信，你要小心点哦。\n"NOR);
	       		break;
	       		
	       		case 8..9:
	       			tell_object(me, GRN+target->name()+"在你的耳边悄声说道：可能有个武功和你相比" + HIW + "极其厉害" + GRN + "的家伙要来抢你的信，你可得小心应付。\n"NOR);
	       		break;
	       		case 10:
	       			tell_object(me, GRN+target->name()+"在你的耳边悄声说道：可能有个武功和你相比" + HIR + "已入化境" + GRN + "的家伙要来抢你的信，打不过可不要逞能。\n"NOR);
	       	}
	       		
		tell_room( environment(me), target->name() + "在" + me->name() + "耳边小声地说了些话。\n", ({ target, me }) );
		write("你可以选择同意或者放弃不做(no)。\n");
		add_action("do_no", "no");
		if(target->query("no_quest")) destruct(target);
		me->delete_temp("songxin/other");
		remove_call_out("dest");
		call_out("dest", random(200)+1200);
	}
	return 1;
}

int do_no()
{
	object me = this_player();

	message_vision("$N看了看信，决定还是不去了。\n", me);
	me->clear_condition("job_busy");
	me->clear_condition("dali_songxin");
	me->clear_condition("task_busy");
	me->delete_temp("zuji");
	me->delete_temp("songxin_zuji");

	me->delete_temp("dali_songxin");
	me->delete_temp("songxin/time");
	me->delete_temp("songxin/times");
	me->delete_temp("songxin/area");
	me->delete_temp("songxin/target_name");
	me->delete_temp("songxin/where");
	me->delete_temp("letter_steal");
	me->set_temp("songxin_ok",1);
	me->set("job_name", "大理送信");
	me->delete_temp("songxin/twice");
	
	me->add("bonus/送信",-3);
	
	if (other) {
		destruct(other);
	}
	destruct(this_object());
	return 1;
}

void dest()
{
	message_vision("$N忽然掉落在路上不见了。\n", this_object());
	destruct(this_object());
}

string *no_kill_list = ({
	"mu ren", "jin ren", "yin ren", "zhong shentong", "zhu cong", "shi ren", "huo du","he shiwo"
});

int check(object ob)
{
	object room;
	string room_name, start_room;

	if (!objectp(ob)
	|| userp(ob)
	|| ob->query("env/invisibility")
	|| ob->query("no_quest")
	|| member_array(ob->query("id"), no_kill_list) != -1
	|| ob->query("attitude")=="aggressive"
	|| ob->query("have_master")
	|| !(start_room = ob->query("startroom"))	// add by Yuj@SJ to void job NPC
	|| !objectp(room=environment(ob))
	|| room->query("no_fight")
	|| room->query("quest")
	|| room->query("outdoors") == "昆仑翠谷"
	|| strsrch(room_name = base_name(room), "/d/") != 0
	|| ob->query("lastroom") == room_name && start_room != room_name
	|| start_room[0..5] != room_name[0..5]
	|| strsrch(room_name, "/d/wizard/") == 0
	|| strsrch(room_name, "/d/wuguan/") == 0
	|| strsrch(room_name, "/d/death/") == 0
	|| strsrch(room_name, "/d/mingjiao/bhd/") == 0
	|| strsrch(room_name, "/d/mingjiao/lsd/") == 0
	|| strsrch(room_name, "/d/group/") == 0
	|| strsrch(room_name, "/d/shaolin/fumoquan") == 0
	|| strsrch(room_name, "/d/baituo/") == 0 )
		return 0;
	return 1;
}

int move(mixed dest, int silently)
{
	int i;
		
	i = ::move(dest, silently);
	if(i && stringp(this_object()->query("ever_owner"))){
		this_object()->set("owner",this_object()->query("ever_owner"));
		remove_call_out("delete_owner");
		call_out("delete_owner",90+random(30));
	}
	return i;
}
                                                                                                                                     
