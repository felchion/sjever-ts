// zhanggui
// by augx@sj  9/17/2001

#ifndef NPCDATA
#define NPCDATA		"/data/npc/"
#endif
#define SAVE_DIR	NPCDATA + "zhanggui1"

#include <ansi.h>
#include <localtime.h>
inherit NPC;
inherit F_UNIQUE;
inherit F_SAVE;

#define CHECK_TIME	15

string ask_money();

void create()
{
	seteuid(getuid());
	restore();

	set_name("掌柜",({ "zhang gui","zhanggui" }) );
	set("gender", "男性" );
	set("age", 45);
	set("long","这是位精明的生意人。");

	set("str", 55);
	set("dex", 55);
	set("con", 25);
	set("int", 25);
	set("shen_type", 1);
	set("move_seller", 1);
	set("shen", 100);
	set("attitude", "peaceful");
	set("combat_exp", 1000000);
	set_max_encumbrance(10000000);

	set_skill("dodge", 300);
	set_skill("force", 300);
	set_skill("parry", 300);
	set_skill("unarmed", 300);
	set_skill("literate", 150);
	set_skill("duanzao", 150);

	set("max_qi", 99999);
	set("max_neili", 99999);
	set("max_jing", 99999);
	set("max_jingli", 99999);
	set("eff_jingli", 99999);

	set_temp("apply/dodge",888);
	set_temp("apply/force",888);
	set_temp("apply/attack",888);
	set_temp("apply/damage",333);
	set_temp("apply/armor",888);

	set("chat_chance", 3);
 	set("chat_msg", ({
 		"掌柜骄傲的说道：“本店卖的兵器可都是最好的兵器！”\n",
 		"掌柜说道：“那些工匠们可都是靠我才发了大财。”\n",
 	}) );

	set("inquiry", ([
		"货款"		: (: ask_money :),
		"money"		: (: ask_money :),
	]));

	set("no_clean_up",1);

	setup();
	carry_object(ARMOR_D("cloth"))->wear();
	add_money("coin",99);

	if ( clonep() ) call_out("check_var" , CHECK_TIME*60);
}

string query_save_file()
{
	return SAVE_DIR;
}

void kill_ob(object ob)
{
	object me = this_object();
	command("!!!");
	command("say 我只是一个商人，为什么要杀我呢？");
	me->remove_enemy(ob);
	ob->remove_killer(me);
	return;
}

int accept_fight(object ob)
{
	command("say 我只是一个商人，为什么要杀我呢？");
	return 0;
}

void init()
{
	::init();

	//add_action("do_buy", "buy");
	//add_action("do_list", "list");
	//add_action("do_sell", "sell");
	//add_action("do_getback", "getback");
}

int do_sell(string arg)
{
	object ob;
	object me = this_player();
	int value, rank , i;
	string target, saveid, detail, type;

	if (!living(this_object()))
		return notify_fail("还是等对方醒来再说吧。\n");

	if (me->is_busy() || me->is_fighting())
		return notify_fail("掌柜说道：你正忙着呢。\n");

	if (!arg || sscanf(arg,"%d %s detail %s",value,target,detail)<3 || !(ob = present(target, me)))
		return notify_fail("掌柜说道：你要寄卖什么？\n");

	if (query_temp("busy"))
		return notify_fail("掌柜说道：哟，抱歉啊，我这儿正忙着呢……您请稍候。\n");

	if ( !(int)ob->query("imbued") || !stringp(ob->query("save_id")) )
		return notify_fail("掌柜说道：本店只寄卖玩家制造的物品。\n");

	type = WORKER_D->imbued_object_type(ob->query("save_id"));

	if (type=="weapon")
		rank = WORKER_D->weapon_rank(ob);
	if (type=="armor")
		rank = WORKER_D->armor_rank(ob);

	if (type=="drug") {
		if ( (int)ob->query("imbued")>3 )
			return notify_fail("掌柜说道：这件物品的质量太差，放在本店贩卖会降低我们的声誉的。\n");
		rank = ob->query("lvl");
	}
	else {
		if ( (int)ob->query("imbued")<3 || rank<=125 )
			return notify_fail("掌柜说道：这件物品的质量太差，放在本店贩卖会降低我们的声誉的。\n");
	}
	
	if(stringp(ob->query("owner")) && ob->query("owner")!="" && ob->query("owner")!="0" )
		return notify_fail("你先删除这把武器的私有标记。\n");
	
	if ( (int)query("getback/"+ob->query("save_id")) )
		return notify_fail("掌柜说道：这件物品你不是刚刚拿回去吗？怎么又来卖了，玩我啊！\n");

	if ( strlen((string)detail)<1 ) detail = "无";
	if ( strlen((string)detail)>80 )
		return notify_fail("掌柜说道：你的说明太长了，说明不要超过四十个汉字！\n");

	if( value<50) return notify_fail("掌柜说道：你开的价格也太低了吧！\n");
	if( value>9999 ) return notify_fail("掌柜说道：你开的价格也太高了吧！\n");
	if( value>999 ) value = value - value%100;

	if( !ob->move(this_object()) )
		return notify_fail("BUG！请通知巫师！\n");
	me->save();

	if (type=="weapon")
		ob->save_weapon(this_object());
	if (type=="armor")
		ob->save_armor(this_object());
	if (type=="drug")
		ob->save_drug(this_object());
	ob->set_temp("quit_dest",1);

	saveid = "goods/" + ob->query("save_id") + "/";
	if(ob->query("imbued")==3) i=24; else i=36;
	set(saveid+"name",ob->name());
	set(saveid+"time",time() + i*2 * 3600);
	set(saveid+"value",value);
	set(saveid+"rank",rank);
	set(saveid+"detail","说明："+detail);
	set(saveid+"type",type);
	set(saveid+"seller",me->query("id"));
	set(saveid+"seller2",me->query("name"));
	save();

	message_vision("$N以"+MONEY_D->price_str(value*10000)+"的价格寄卖了一"+ob->query("unit")+ob->name() + "给$n。\n", me,this_object());
	log_file("career/jimai", sprintf("sell:%-8s:%-19s:%d:%-16s:%s\n",me->query("id"),ob->query("save_id"),ob->query("weapon_mp/utime")+ob->query("armor_mp/utime"),ob->name(),ob->query("id")), me);
	destruct(ob);

	set_temp("busy", 1);
	call_out("delete_temp", 1, "busy");

	return 1;
}

static string *sym_di = ({ "子","丑","寅","卯","辰","巳","午","未","申","酉","戌","亥" });
string chinese_date(int date)
{
	mixed *local;

	local = localtime(date);
	return sprintf("%s月%s日%s时",
			chinese_number(local[LT_MON] + 1),
			chinese_number(local[LT_MDAY]),
			sym_di[((local[LT_HOUR] + 1) % 24) / 2]);
}

int do_list(string arg)
{
	object me = this_player();
	mapping mp , mp2;
	string * gkeys;
	mixed * gvalues;
	int i,j,k,t;

	if (!living(this_object()))
		return notify_fail("还是等对方醒来再说吧。\n");

	if (me->is_busy() || me->is_fighting())
		return notify_fail("掌柜说道：你正忙着呢。\n");

	if (!arg)
		return notify_fail("掌柜说道：你要看那一类商品？（weapon,armor,drug）\n");

	switch (arg) {
		case "weapon":
		case "armor":
		case "drug": break;
		default: return notify_fail("掌柜说道：你要看那一类商品？（weapon,armor,drug）\n");
	}

	if(!mapp(query("goods"))) return notify_fail("掌柜说道：目前没有此类别的商品卖。\n");

	write("目前寄售的货物有：（价格单位：黄金）\n");
	write("┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓\n");
	write("┃  货  物                        价格       等级 评价  卖主              寄卖进度          ┃\n");
	write("┣━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫\n");

	j = sizeof(query("goods"));
	gkeys = keys(query("goods"));
	gvalues = values(query("goods"));
	t = time() + 18*3600;
	for(i=0;i<j;i++) {
		if( !mapp(gvalues[i]) ) continue;
		mp = gvalues[i];
		if (mp["type"]!=arg) continue;
		mp2 = query("worker/"+gkeys[i]);

		k = (t - mp["time"])/3600;
		if (k < 0) k = 0;
		if (k > 18) k = 18;
		write(sprintf("┃%-32s%-12s%-4s%4d%|20s%'.'-18s┃\n",
			mp["name"]+"("+(arg=="drug"?mp2["yao"]:mp2["id"][0])+")",
			CHINESE_D->chinese_number(mp["value"]),
			arg=="drug"?CHINESE_D->chinese_number(mp2["imbued"]):CHINESE_D->chinese_number(6-mp2["imbued"]),
			mp["rank"],
			mp["seller2"]+"("+mp["seller"]+")",
			repeat_string("o", k)
		));
		write(sprintf("┃└─%-86s┃\n",mp["detail"]));
	}

	write("┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛\n");

	me->start_busy(1);
	return 1;
}

int do_buy(string arg)
{
	object me = this_player();
	string name1,name2;
	mapping mp , mp2;
	string * gkeys;
	mixed * gvalues;
	int num,i,j;

	if (!living(this_object()))
		return notify_fail("还是等对方醒来再说吧。\n");

	if (me->is_busy() || me->is_fighting())
		return notify_fail("掌柜说道：你正忙着呢。\n");

	if (query_temp("busy"))
		return notify_fail("掌柜说道：哟，抱歉啊，我这儿正忙着呢……您请稍候。\n");

	if (!arg)
		return notify_fail("掌柜说道：你要买什么？\n");

	if(!mapp(query("goods")))
		return notify_fail("掌柜说道：你要买什么？\n");

	if( sscanf(arg,"%s %d",name1,num)==2 ) { num--; }
	else if( sscanf(arg,"%s %s %d",name1,name2,num)==3 ) { name1+=" "+name2; num--; }
	else { name1=arg; num=0; }

	j = sizeof(query("goods"));
	gkeys = keys(query("goods"));
	gvalues = values(query("goods"));
	for(i=0;i<j;i++) {
		if( !mapp(gvalues[i]) ) continue;
		mp = gvalues[i];
		mp2 = query("worker/"+gkeys[i]);

		if( (mp["type"]!="drug" && mp2["id"][0] == name1)
		 || (mp["type"]=="drug" && mp2["yao"] == name1) ) {
			if(num) {
				num--;
				continue;
			}

			switch( MONEY_D->player_pay(me, mp["value"]*10000) ) {
				case 0:
				case 2:	return notify_fail("穷光蛋，一边呆着去！\n");
				default: break;
			}

			message_vision("$N以"+MONEY_D->price_str(mp["value"]*10000)+"的价格从$n那里买下了"+mp["name"]+"。\n", me,this_object());
			log_file("career/jimai", sprintf("buy :%-8s:%-19s\n",me->query("id"),gkeys[i]), me);
			me->set("worker/"+gkeys[i],copy(mp2));
			me->save();
			WORKER_D->load_user_object(me,mp2);

			add("reward/"+mp["seller"],mp["value"]*10000);
			delete("worker/"+gkeys[i]);
			delete("goods/"+gkeys[i]);
			save();

			break;
		}
	}

	set_temp("busy", 1);
	call_out("delete_temp", 1, "busy");

	if(i>=j) return notify_fail("掌柜说道：你要买什么？\n");
	return 1;
}

int do_getback(string arg)
{
	object me = this_player();
	string name1,name2;
	mapping mp , mp2;
	string * gkeys;
	mixed * gvalues;
	int num,i,j;

	if (!living(this_object()))
		return notify_fail("还是等对方醒来再说吧。\n");

	if (me->is_busy() || me->is_fighting())
		return notify_fail("掌柜说道：你正忙着呢。\n");

	if (query_temp("busy"))
		return notify_fail("掌柜说道：哟，抱歉啊，我这儿正忙着呢……您请稍候。\n");

	if (!arg)
		return notify_fail("掌柜说道：你要取回什么？\n");

	if(!mapp(query("goods")))
		return notify_fail("掌柜说道：你要取回么？\n");

	if( sscanf(arg,"%s %d",name1,num)==2 ) { num--; }
	else if( sscanf(arg,"%s %s %d",name1,name2,num)==3 ) { name1+=" "+name2; num--; }
	else { name1=arg; num=0; }

	j = sizeof(query("goods"));
	gkeys = keys(query("goods"));
	gvalues = values(query("goods"));
	for(i=0;i<j;i++) {
		if( !mapp(gvalues[i]) ) continue;
		mp = gvalues[i];
		mp2 = query("worker/"+gkeys[i]);

		if( (mp["type"]!="drug" && mp2["id"][0] == name1)
		 || (mp["type"]=="drug" && mp2["yao"] == name1) ) {
			if(num) {
				num--;
				continue;
			}

			if( mp["seller"] != me->query("id") )
				return notify_fail("掌柜说道：这件物品不是你寄卖的。\n");

			message_vision("$N把以前寄卖的"+mp["name"]+"取回去了。\n", me);
			log_file("career/jimai", sprintf("back:%-8s:%-19s\n",me->query("id"),gkeys[i]), me);
			me->set("worker/"+gkeys[i],copy(mp2));
			me->save();
			WORKER_D->load_user_object(me,mp2);

			set("getback/"+gkeys[i],time()+86400); // 1 day
			delete("worker/"+gkeys[i]);
			delete("goods/"+gkeys[i]);
			save();

			break;
		}
	}

	set_temp("busy", 1);
	call_out("delete_temp", 1, "busy");

	if(i>=j) return notify_fail("掌柜说道：你要取回什么？\n");
	return 1;
}

string ask_money()
{
	object me = this_player();
	int money;

	if (me->is_busy() || me->is_fighting())
		return "你正忙着呢。\n";

	if (query_temp("busy"))
		return "哟，抱歉啊，我这儿正忙着呢……您请稍候。";

	money = (int)query("reward/"+me->query("id"));
	if ( money<=0 )
		return "你没有寄卖物品或你的物品还没有卖出去。";

	money = money * 7/8;
	MONEY_D->pay_player(me,money,1);
	delete("reward/"+me->query("id"));
	save();

	message_vision("$N扣除手续费"+MONEY_D->price_str(money/7)+"后，总计付给$n"+MONEY_D->price_str(money)+"。\n",this_object(),me);
	return "以后搞到好东西，可别忘记照顾小店的生意啊。";
}

void check_var()
{
	mapping mp;
	mixed * gvalues;
	string * gkeys;
	object ob;
	int i,j;

	remove_call_out("check_var");
	if( !environment(this_object()) ) return;

	if( mapp(query("getback")) ) {
		mp = query("getback");
		gkeys = keys(mp);
		gvalues = values(mp);
		for(i=0;i<sizeof(mp);i++) {
			if( !intp(gvalues[i]) ) continue;
			if( time() > gvalues[i] )
				delete("getback/"+gkeys[i]);
		}
	}

	if( mapp(query("goods")) ) {
		j = sizeof(query("goods"));
		gkeys = keys(query("goods"));
		gvalues = values(query("goods"));
		for(i=0;i<j;i++) {
			if( !mapp(gvalues[i]) ) continue;
			mp = gvalues[i];
			if( time()<mp["time"] ) continue;

			ob = WORKER_D->load_user_object( this_object() , query("worker/"+gkeys[i]) );
			log_file("career/jimai", sprintf("dele:%s\n",gkeys[i]));
			add("reward/"+mp["seller"],ob->query("value")*8/10);
			delete("worker/"+gkeys[i]);
			delete("goods/"+gkeys[i]);
			destruct(ob);
		}
		save();
	}

	call_out("check_var" , CHECK_TIME*60);
}
