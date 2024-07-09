//File : /clone/misc/bjzj.c By Ciwei@SJ
// 
#include <ansi.h>
#include <armor.h>

inherit FINGER;

string do_drop();

void create()
{
	set_name(HIW "白金钻戒" NOR, ({ "baijin zuanjie", "zuanjie" }) );
	set_weight(1000);
	if (clonep()) set_default_object(__FILE__);
	{
		set("unit", "枚");
		set("value", 10000000);//千金易得，知己难求
		set("material", "jade");
		set("armor_prop/armor",1);
		set("no_get",1);
		set("no_drop",(:do_drop:));
		set("no_give",1);
		set("remove_msg","$N细心得将"+name()+"摘下，放在贴身处收藏。\n");
		set("wear_msg","$N把"+name()+"戴在自己手上左顾右盼，得意非凡。\n");
	}
	setup();
}

int query_autoload()
{
	return 1;
}

int do_qiuhun(string arg)
{
	object me = this_player();
	object ob = this_object();
	object obj;
	if("/cmds/std/propose"->main(me,arg))
	{
		obj = me->query_temp("pending/marry");
		if(!obj) return 0;
		ob->set_temp("qiu_from",me);
		ob->set_temp("qiu_to",obj);
		message_vision(CYN"说完$N就把"+name()+"放在$n手里，一脸期待地看着$p。\n"NOR,me,obj);
		tell_object(obj,YEL"如果你愿意就戴起这枚戒指，如果不愿意就丢了它吧。\n"NOR);
		move(obj);
		return 1;
	}
	return 0;
}

string do_drop()
{
	object me = this_player();
	if(query_temp("qiu_from") && query_temp("qiu_to") && query_temp("qiu_to")==me)
	{
		if(present(query_temp("qiu_from"),environment(me)))
			message_vision(CYN"$N对$n轻轻但是很坚毅地摇了摇头，把"+name()+"还给了$p。\n"NOR,me,query_temp("qiu_from"));
		else
			tell_object(me,"你把戒指还给了"+query_temp("qiu_from")->query("name")+"。\n");

		tell_object(query_temp("qiu_from"),YEL"哇靠！"+me->name()+"拒绝了你的求婚，No s-f Just Drink(联系地址：长安鼓楼大道一号一弄英豪酒楼)。\n"NOR);
		
		query_temp("qiu_from")->delete_temp("pending/marry");
		move(query_temp("qiu_from"));
		delete_temp("qiu_from");
		delete_temp("qiu_to");
		return "   \n";
	}
	return "这东西你舍得丢么？\n";
}

int wear()
{
	object me = this_player();
	if(query_temp("qiu_from") && query_temp("qiu_to") && query_temp("qiu_to")==me)
	{
		if(!"/cmds/std/marry"->main(me,getuid(query_temp("qiu_from"))))
		{
			//write("还是等会再戴吧。\n");
			return notify_fail(query_notify_fail());
		}
		message_vision("$N喜滋滋地戴上了"+name()+"。\n",me);
		delete_temp("qiu_from");
		delete_temp("qiu_to");		
	}
	return ::wear();
}

void init()
{
	object ob = this_player();
	
	if (environment() != ob	) return;
	
	if(ob->query("gender")=="男性")
	{
		set("long","这是用白金定制的钻戒,戒子上的钻石发出耀眼的光芒~~\n女孩子最喜欢的就是精美的钻石，这个戒指用来求婚很有可能事半功倍。\n");
		add_action("do_qiuhun",({"qiu","propose"}));
		return;
	}
	if(ob->query("gender")=="无性")
	{
		set("long","这是用白金定制的钻戒,戒子上的钻石发出耀眼的光芒~~\n女孩子最喜欢的就是精美的钻石，你这几年性情大变，对这类东西也是特别在意。\n");
		wear();
		return;
	}
	if(query_temp("qiu_from") && query_temp("qiu_to") && query_temp("qiu_to")==ob)
	{
		set("long","这是用白金定制的钻戒,戒子上的钻石发出耀眼的光芒~~\n这是"+query_temp("qiu_from")->query("name")+"向你求婚的证明，如果你愿意和他共结连理就戴上它，如果不愿意就丢了它。\n");
		return;		
	}
	if (!ob->query("marry/id") || !ob->query("marry/name")) {
		ob->delete("marry");
		call_out("lost", 1);
		return;
	}
	set("long","这是用白金定制的钻戒,戒子上的钻石发出耀眼的光芒~~\n这让你一下子回忆起"+ob->query("marry/name")+"向你求婚的那一刻。\n");
	wear();
}

void lost()
{
	object ob = environment();

	if (userp(ob) && (!ob->query("marry/id") || !ob->query("marry/name"))) {
		message_vision(HIB"\n\n$N万念俱灰，将钻戒远远得丢了出去。\n"NOR,ob);
		destruct(this_object());
	}
}
/*
int unequip()
{
	object ob = environment();

	if (ob && userp(ob) && ob->query("marry/id") && ob->query("marry/name"))
		return notify_fail("你不能卸下你的定情信物。\n");

	return ::unequip();
}
*/