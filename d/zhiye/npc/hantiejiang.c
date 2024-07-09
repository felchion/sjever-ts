inherit NPC;

#include "/d/zhiye/npc/forbook.c"
#define JADE    "/d/zhiye/obj/othermaterial" //玉

int ask_yt()
{
	object me = this_player();
	
	if(is_busy()) return 0;
	
	if(me->query_skill("duanzao",1) < 120
	&& me->query_skill("caikuang",1) < 120
	&& me->query_skill("zhizao",1) < 120
	&& me->query_skill("nongsang",1) < 120 )
	{
		command("say 就你这点手艺，也来研究什么倚天屠龙……，你这不是……");
		command("regard "+me->query("id"));
		return 1;
	}
	
	if(me->query("worker/hangift"))
	{
		command("say 你上次不是来问过了么？告诉你了……");
		command("regard "+me->query("id"));
		return 1;		
	}
	
	me->start_busy(100);
	this_object()->start_busy(100);
	
	command("say 当年我年少云游天下，曾经在塞北穷荒之地遇到一位奇人，我们在寒铁玄奇矿脉中不眠不休七昼夜，研究出打造绝世神兵的技术，我们为他取名『倚天屠龙术』……");
	call_out("ytquest_1",1+random(2),me);
	return 1;
}

void ytquest_1(object me)
{
	if(!me) return;
	
	if(me->query("worker/hangift")) return;
	
	if(environment(me)!=environment(this_object())) return;
	
	message_vision(CYN"$N上下打量了$n两眼。\n",this_object(),me);
	message_vision(CYN"$N侃侃而谈：「你可知道什么是『屠龙术』？对……，世上本无龙，屠龙术就是空无一
用的技能，我到了不惑之年，才发现我少年研究的『倚天屠龙术』百无一用，因为神兵利器
虽然能伤人，但是终归是一人敌，而且于天下苍生又有何益处？逞强好胜，结党朋比，呼喝
叫嚣，用技劳心，屠杀无度，自以为英雄盖世，其实终究是庸人一个，我的剑……，呸……，
真是可惜了我剑……」\n"NOR,this_object(),me);
	
	call_out("ytquest_2",1+random(2),me);
}

void ytquest_2(object me)
{
	object obj;
	int level = 1;

	if(!me) return;

	if(me->query("worker/hangift")) return;

	if(environment(me)!=environment(this_object())) return;

	if(!random(2)) level+=1;
	if(!random(3)) level+=1;
	if(!random(4)) level+=1;
	if(!random(5)) level+=1;

	obj = new(JADE);
	obj->set_level(level);

	if(obj)
	{
		message_vision("$N翻箱到柜拿出一块"+obj->name()+"。\n",this_object());
		message_vision("$N给$n一块"+obj->name()+NOR"。\n",this_object(),me);
		command("say 我这儿也没什么好的东西，此物你可以拿去参详，至于真正的『倚天屠龙术』……，等将来你历练多了，或许能有多体会。");
		message_vision(HIY"$N背过身去，喃喃自语：「或许只有那位大侠，才明白什么是真正的『倚天屠龙术』。」\n"NOR,this_object() );

		if(obj->move(me)) {
			me->set("worker/hangift",1);
			me->save();
		}
		me->interrupt_me();
		me->start_busy(-1);
		me->interrupt_me();
		this_object()->interrupt_me();
		this_object()->start_busy(-1);
		this_object()->interrupt_me();
	}
}

void create()
{
	set_name("韩铁匠", ({ "han tiejiang","han", "tie", "jiang","tiejiang" }) );
	set("gender", "男性" );
	set("age", 34);
	set("long",
  "这是一位正在抡着大铁锤，叮叮当当地打铁的铁匠。
看似普通得不能再普通了。
只是偶尔从他沧桑的双目中流露出对往日岁月的留恋。\n");
	set("combat_exp", 300);
	set("attitude", "peaceful");
	set("rank_info/respect", "韩铁匠");


	set("inquiry", ([
		"倚天屠龙术": (:ask_yt:),
	]));

	/*
	set("vendor_goods", ({
	(["name":__DIR__"obj/changjian","number":25]),
	(["name":__DIR__"obj/gangjian","number":15]),
	(["name":__DIR__"obj/tiejia","number":22]),
	(["name":"/d/tiezhang/obj/gangzhang","number":10]),
	(["name":__DIR__"obj/gangdao","number":10]),
	(["name":__DIR__"obj/caidao","number":20]),
	(["name":__DIR__"obj/tiechui","number":10]),
	(["name":__DIR__"obj/bishou","number":12])
	}));
	set("inquiry", ([
	"here" : "这里是城里唯一的打铁铺了，什么人都到我这里来打东西。\n",
	"dazao" : (: ask_dazao :),
	"打造" : (: ask_dazao :),
	"炼制" : (: ask_dazao :),
	"铸剑" : (: ask_dazao :),
	"炼剑" : (: ask_dazao :),
	"炼刀" : (: ask_dazao :),
	"炼杖" : (: ask_dazao :),
	"炼鞭" : (: ask_dazao :),
	"炼锤" : (: ask_dazao :),
	"炼棍" : (: ask_dazao :),
	"炼甲" : (: ask_dazao :),
	"打造" : (: ask_dazao :),
	"铸造" : (: ask_dazao :),
	"制造" : (: ask_dazao :),
	"打制" : (: ask_dazao :),
	"定制" : (: ask_dazao :),
	"定做" : (: ask_dazao :),
	"定造" : (: ask_dazao :),
	"兵器" : (: ask_dazao :),
	"道具" : (: ask_dazao :),
	"装备" : (: ask_dazao :),
	"武器" : (: ask_dazao :),
	"刀剑" : (: ask_dazao :),
	"装甲" : (: ask_dazao :),
	"打造之术": (: ask_learn :),
	"成就":(: ask_cheng  :),
	"成绩":(: ask_cheng  :),
	"搜寻":(: ask_place :),
	]) );
	*/
	set("forg_type","all");
	setup();
	carry_object("/clone/misc/cloth")->wear();
}


void init()
{
	::init();
	add_action("do_taojiao","taojiao");///for tiejiang xx
}