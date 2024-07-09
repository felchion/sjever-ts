// guojing.c 郭靖
// Lklv 2001.9.28 update
// add quest by Ciwei@SJ

#define YT_BOOK "/d/zhiye/obj/book.c"
#include <ansi.h>
inherit NPC;

string ask_jiebai() 
{
        object me = this_player();

        if(me->query("quest/chance")<1 
        || me->query("quest/jiebai/start") 
        || me->query("quest/jiebai/pass") ) 
        	return "周伯通是我大哥。";        
        else {
 	   	    me->add("quest/chance",-1);
    		  me->set("quest/jiebai/start",1);
   		    log_file("quest/jiebai",sprintf("%-18s想和周伯通结拜花费解迷机会一次。\n",
			          me->name(1), "("+capitalize(me->query("id"))+")"), me);
   		    return "我大哥还在桃花岛上呢。";
   		  }
}
 
int ask_yt()
{
	object me = this_player();
	
	if(is_busy()) return 0;
	if(is_fighting()) return 0;
	
	if(me->query("worker/guogift"))
	{
		if(me->query("quest/chance")<1) {
			command("say 你上次不是来问过了么？……");
			command("kick3 "+me->query("id"));
			return 1;
		}
		else {
			command("say 这世界上有一种很特别的石头，比玄铁还要坚硬......");
			command("say 这种石头做成砥石，可以提高武器的威力。");
			me->set_temp("ask_gou_stone",1);
			return 1;
		}
	}

	if(me->query_skill("duanzao",1) < 220
	&& me->query_skill("caikuang",1) < 220
	&& me->query_skill("zhizao",1) < 220
	&& me->query_skill("nongsang",1) < 220 )
	{
		command("say 什么屠龙术……？为国为民，侠之大者，世上无龙，研究什么屠龙术？");
		return 1;
	}
		
	me->start_busy(50);
	this_object()->start_busy(50);
	
	command("say 蓉儿曾遇到一位韩前辈，传授她『倚天屠龙术』，据说是锻造兵器之用，蓉儿说将来可以用来保存《九阴真经》和《武穆遗书》，可惜我悟性不高，一直不能领会，……");
	
	message_vision(CYN"$N上下打量了$n两眼。\n",this_object(),me);
	command("say 你有意助我参详？只是此事关系重大，恕我需要考较考较你的功夫。你真有意助我？"HIY"（answer y/n）"NOR);
	me->set_temp("guo_yt",1);
	
	return 1;
}

int ask_stone(){
	object ob,me = this_player();

	if(is_busy()) return 0;
	if(is_fighting()) return 0;
	if(!me->query_temp("ask_gou_stone")) return 0;
	if(!objectp(ob=unew("/clone/gift/whetstone"))) return 0;
	me->delete_temp("ask_gou_stone");
	if(me->query("quest/chance")>0){
		if(random(2)) {
			me->add("quest/chance",-1);
			command("say 天材地宝是很难得的，有缘才能遇到啊。");
			return 1;
		}

		ob->set("owner",me->query("id"));
		ob->move(me);
		me->add("quest/chance",-1);
		command("say 今日有缘，这块石头你就拿去吧。");
		message_vision(CYN"$N给$n一块"+ob->name()+CYN"。\n",this_object(),me);
		return 1;

	}
	return 0;
}

int do_answer(string arg)
{
	object me = this_player();
	
	if(!me->query_temp("guo_yt")) return 0;
	
	if(me->query("worker/guogift")) return 0;

	if( !living(this_object()) ) return 0;
	
	if(!me->is_busy())
	{
		me->delete_temp("guo_yt");
		return 0;
	}
	
	me->delete_temp("guo_yt");
	
	if(arg && (arg=="Y" || arg=="y"))
	{
		command("ok");
		call_out("do_fight",0,me);
		return 1;
	}
	
	command("say 既然如此，那算了吧，将来或许还有机会……");
	this_object()->interrupt_me();
	this_object()->start_busy(-1);
	this_object()->interrupt_me();
	me->interrupt_me();
	me->start_busy(-1);
	me->interrupt_me();
	return 1;
}

void do_fight(object me)
{
	object ob = this_object();
	
	if(!me) return;
	
	if(me->query("worker/guogift")) return;
	
	if(environment(me)!=environment(this_object())) return;	
	
	add_temp("apply/damage",300);
	add_temp("apply/attack",300);
	COMBAT_D->do_attack(ob, me, ob->query_temp("weapon"), 1);
	COMBAT_D->do_attack(ob, me, ob->query_temp("weapon"), 1);
	COMBAT_D->do_attack(ob, me, ob->query_temp("weapon"), 1);
	COMBAT_D->do_attack(ob, me, ob->query_temp("weapon"), 1);
	COMBAT_D->do_attack(ob, me, ob->query_temp("weapon"), 1);
	COMBAT_D->do_attack(ob, me, ob->query_temp("weapon"), 1);
	COMBAT_D->do_attack(ob, me, ob->query_temp("weapon"), 1);
	if(ob)
	{
		add_temp("apply/attack",-300);
		add_temp("apply/damage",-300);
		call_out("do_check",1,me);
	}
}

void do_check(object me)
{
	object ob = this_object();
	object obj;
		
	if(!me) return;
	
	if(me->query("worker/guogift")) return;
	
	if(environment(me)!=environment(this_object())) return;	
	
	if(!living(me) || me->query("qi") < 0 )
	{
		command("say 你的壮志可嘉，但是身手……");
		message_vision(CYN"$N摇了摇头。\n"NOR,ob);
		this_object()->interrupt_me();
		this_object()->start_busy(-1);
		this_object()->interrupt_me();
		me->interrupt_me();
		me->start_busy(-1);
		me->interrupt_me();
		return;
	}
	
	obj = new(YT_BOOK);
	obj->set_level(1+random(2));
	
	if(obj)
	{
		command("say 其实我天生笨拙，不懂那些精致的技巧，我只知道师傅教我，对襄阳百姓，对大宋子民有利的，乃至对凡夫俗子生计安康有利的，就是大道，其他的技能，不过是屠龙之术，看上去煞有介事，其实百无一用");
		command("say "+RANK_D->query_respect(me)+"既然有这般身手，我这里有一篇《"+obj->name()+"》，是当年高人所赠，今天就转赠于你，但愿你能修得屠龙术，造福在苍生……");
		command("bow "+me->query("id"));
		message_vision("$N给$n一"+obj->query("unit")+obj->name()+NOR"。\n",ob,me);
		
		if(obj->move(me)) me->set("worker/guogift",1);
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
        set_name("郭靖", ({"guo jing", "guo", "jing"}));
        set("nickname", HIY"北侠"NOR);
        set("gender", "男性");
        set("age", 40);
        set("long", "他就是威振天下的大侠郭靖，第二次华山论剑后号称“北侠”。\n");
        set("attitude", "friendly");
        set("str", 50);
        set("int", 10);
        set("con", 50);
        set("dex", 25);
        set("unique", 1);
        set("no_get", 1);

        set("max_qi", 9000);
        set("max_jing", 8000);
        set("neili", 18000);
        set("max_neili", 15000);
        set("jiali", 100);
        set("eff_jingli",9000);
        set("combat_exp", 3500000);

        set_skill("force", 300);
        set_skill("finger",300);
        set_skill("tanzhi-shentong",300);
        set_skill("yiyang-zhi",300);
        set_skill("claw",300);
        set_skill("jiuyin-baiguzhua",300);
        set_skill("huntian-qigong", 300);
        set_skill("strike", 300);
        set_skill("xianglong-zhang", 320);
        set_skill("dodge", 300);
        set_skill("jinyan-gong", 300);
        set_skill("parry", 300);

        map_skill("force", "huntian-qigong");
        map_skill("strike", "xianglong-zhang");
        map_skill("dodge", "jinyan-gong");
        map_skill("parry", "xianglong-zhang");
        map_skill("finger","tanzhi-shentong");
        map_skill("claw", "jiuyin-baiguzhua");
        prepare_skill("strike", "xianglong-zhang");

        set_temp("apply/damage",100);
        set_temp("apply/defence",100);
        set_temp("double_attack",1);
        
        set("inquiry", ([
        	"倚天屠龙术": (:ask_yt:),
        	"周伯通": (:ask_jiebai:),
        	"石头": (:ask_stone:),
        ]));

        set("chat_chance_combat", 100);
        set("chat_msg_combat", ({
                (: perform_action, "strike.paiyun" :),
                (: perform_action, "strike.kanglong" :),
        }));
        create_family("丐帮", 18, "帮主亲传弟子");
        setup();

        carry_object(ARMOR_D("cloth"))->wear();
        carry_object(ARMOR_D("shoes"))->wear();
}

void init()
{
	::init();
	
	add_action("do_answer",({"answer","huida"}));
}

void kill_ob(object me)
{
        if (userp(me)) me->apply_condition("kill_guo", 200);
        ::kill_ob(me);
}

void die()
{
        command("chat 蓉....蓉儿，我先走...一步了！");
        command("chat 盼你完成屠龙刀....和....和倚.....");
        ::die();
}

