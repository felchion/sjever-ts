 // by Xiaoyao

inherit NPC;
#include <ansi.h>
void destroying(object, object);
#include "/d/suzhou/npc/lchj.h";//连城诀

string ask_shifu();
int ask_skills();
int ask_sword();
void kill_hua(object me);
int ask_outxuegu();
string ask_yuanqu();
string ask_dingdian();

void create()
{
        set_name("狄云", ({ "di yun", "di","yun" }));
        set("long", "这人满脸虬髯，头发长长的直垂至颈，衣衫破烂不堪，简直如同荒山中的野人。\n");
        set("gender", "男性");
        set("age", 35);
        set("unique", 1);
		set("str", 30);
		set("int", 30);
		set("con", 30);
		set("dex", 30);
		set_skill("cuff", 250);
        set_skill("dodge", 250);
        set_skill("parry", 250);
        set_skill("force", 300);
        set_skill("longxiang-boruo", 250);
        set_skill("yuxue-dunxing", 250);
        set_skill("taizu-quan", 200);
        set_skill("blade", 250);
        set_skill("xuedao-jing", 300);
        set_skill("shenzhao-jing", 400);
		map_skill("force", "longxiang-boruo");
		map_skill("dodge", "yuxue-dunxing");
        map_skill("parry", "xuedao-jing");
        map_skill("blade", "xuedao-jing");
        map_skill("cuff","taizu-quan");
        prepare_skill("cuff","taizu-quan");
    set_temp("xs/longxiang",1);
		set("chat_chance_combat", 100);
		set("chat_msg_combat", ({
			(: perform_action, "blade.shendao" :),
		}));
        set("max_neili",20000);
        set("neili",25000);
        set("max_qi",26800);
        set("qi",26800);
        set("eff_jingli",18000);
        set("jingli",18000);
        set("jiali",200);
		set("no_quest",1);//
	set("inquiry", ([
		"name": "我就是狄云。",
		"here": "这里是雪山雪谷。",
		"神照经" : (: ask_skills :),//
		"躺尸剑法" : (: ask_sword :),//
		"戚长发" : (: ask_shifu :),//
		"祸变" : (: ask_yuanqu :),//
		"冤屈" : (: ask_yuanqu :),//
		"冤案" : (: ask_yuanqu :),//
		"丁典" : (: ask_dingdian :),//
		"出谷" : (: ask_outxuegu :),//
		"离开" : (: ask_outxuegu :),//
		"连城诀" : "连城决？......我没听说过。",
		"连城剑法" : "什么连城剑法？我只学过躺尸剑法。",
		"万震山" : "他是我师伯。",
		"言达平" : "他是我师伯。",
		"梅念笙" : "铁骨墨萼梅念笙老先生，可惜死得冤枉啊。",
		"菊友" : "她是凌霜华的贴身丫鬟。",
		"凌退思" : "他就是凌霜华的爹爹，真是苍天弄人啊。",
		"凌霜华" : "她就是翰林府大小姐，也是我最心爱的人。",
		"落花流水":"你说的什么南四奇？除了花老头，其他都被血刀老贼害死了！",
		"花铁干":"“中平枪花铁干”，这是个无耻的懦夫！",
		"刘乘风":"“柔云剑刘乘风”，可怜已经被血刀老贼杀害了！",
		"陆天抒":"“仁义陆大刀”，可怜已经被血刀老贼杀害了！",
		"水岱":"“冷月剑水岱”，可怜已经被血刀老贼杀害了！",
		"戚芳":random(2)?"曾经有一份纯真的爱情放在我面前我没有珍惜，等到失去的时候才后悔莫及！":"曾经的......只是一切都过去了。",
		"水笙":random(2)?"水姑娘，也是我现在最亲近的人了！":"我会珍惜这次爱情的。",
	]));
    
	set("combat_exp", 1500000);
	set("shen_type", 1);
	setup();
	carry_object("/clone/armor/cloth")->wear();
	carry_object(BINGQI_D("xinyuedao"))->wield();
}
/*
void heart_beat()
{
	::heart_beat();
	clear_conditions_by_type("poison");
}
*/
int accept_object(object me, object obj)
{
		int i;
        if ((string)obj->query("id")=="wucan yi") {
			message("vision",me->name()+"给狄云一件乌蚕衣。\n",environment(me),({me}));
			remove_call_out("destroying");
			call_out("destroying", 1, this_object(), obj);
			if(obj->query("owner")!=me->query("id")) return notify_fail (CYN"狄云似乎被如此大礼吓了一跳。\n狄云对"+me->name()+"表示衷心感谢。\n"NOR);  
			command("ah "+me->query("id"));
			command("say 这位"+RANK_D->query_respect(me)+"应该是丁大哥的朋友吧。");
			command("touch "+me->query("id"));
			if(me->query(QUESTDIR1+"start") && me->query_temp(QUESTDIR1+"asklaozu") && !me->query_temp(QUESTDIR1+"trust") 
				&& !me->query_temp(QUESTDIR1+"kill_hua") && !me->query(QUESTDIR1+"over"))
			{
				me->set_temp(QUESTDIR1+"trust",1);
				me->start_busy(2);
				if(!me->query(QUESTDIR1+"give_yi") )
				{
					i=500+random(300);
					me->add("combat_exp",i);//
					tell_object(me,"你给狄云一件"HIY"乌蚕衣"NOR+"获得部分经验奖励。\n");
					log_file("quest/LCHJ", sprintf("%8s(%8s)赠送乌蚕衣获得%d经验奖励。\n", me->name(1),me->query("id"),i) );	
				}
				me->set(QUESTDIR1+"give_yi",1);//
				remove_call_out("kill_hua");
				call_out("kill_hua", 2, me);
				command("say 这位"+RANK_D->query_respect(me)+"真是太感谢你了吧。");
				me->start_busy(1);
				return 1;
			}
			else
			{                
				command("say 这件衣服现在我先拿着，这位"+RANK_D->query_respect(me)+"很感谢你啊。");
				command("goodman");
				me->start_busy(1);
				return 1;
			}
		}
       if ((string)obj->query("id")=="tangshi xuanji") {
			message("vision",me->name()+"要给狄云一本"HIY"唐诗选辑"NOR+"。\n",environment(me),({me}));
			call_out("destroying", 1, this_object(), obj);
			if(obj->query("owner")!=me->query("id") 
				|| !obj->query("look_zi") 
				|| me->query(QUESTDIR2+"baozang")
				|| !me->query_temp(QUESTDIR2+"find_zi")) 
			{
				command("? "+me->query("id"));
				command("points");
				message_vision(HIW"$n很疑惑地看着$N。\n"NOR, me, this_object());
				return 1;  
			}
			command("ah "+me->query("id"));
			command("say "+RANK_D->query_respect(me)+"莫非见过戚芳？也不知道现在她生活怎么样了。");
			tell_object(me,"你给狄云一本"HIY"唐诗选辑"NOR+"获得部分经验奖励。\n");
			command("whisper "+me->query("id")+" "+HIY"这本书实际上涉及到一个宝藏的秘密。哦，你好像已经发现了。"NOR);
			command("whisper "+me->query("id")+" "+HIY"其实这也是一个陷阱，所谓的宝藏根本不存在。"NOR);
			command("whisper "+me->query("id")+" "+HIY"当年凡是与宝藏有关的人只怕都被间接害死了。"NOR);			
			command("say 这本书，我就收下吧。很感谢这位"+RANK_D->query_respect(me)+"。");
			i=300+random(300);
			me->add("combat_exp",i);//
			me->set(QUESTDIR2+"baozang",1);
			me->delete_temp("quest/busy");//
			me->delete_temp(QUESTDIR);
			me->start_busy(1);
			me->set(QUESTDIR+"time",time());
			me->set(QUESTDIR+"combat_exp",me->query("combat_exp")); 
			log_file("quest/LCHJ", sprintf("%8s(%8s)唐诗选辑获得%d经验奖励。\n", me->name(1),me->query("id"),i) );	
			me->start_busy(1);
			return 1;
		}
		return 0;
}
void kill_hua(object me)
{
	object obj,ob;
	ob = this_object();
	if(!me) return;
	if(!ob) return;
	if(!me->query_temp(QUESTDIR1+"trust")) return;
	if(!me->query(QUESTDIR1+"give_yi")) return;
	if(me->query_temp(QUESTDIR1+"kill_hua")) return;
	if(me->query(QUESTDIR1+"over")) return;
	if(environment(ob)!=environment(me)) return;

	ob->reincarnate();
	ob->set("neili",25000);
	obj=new("/d/xueshan/npc/huatiegan");
	obj->set("no_quest",1);//
	obj->set("quest_object",me);//
	obj->move(environment());
	message_vision(HIW"突然，从旁边跳出一个人，举起右掌，往狄云身上击落。\n"NOR, me, this_object());
	message_vision(HIY"$N定眼一看，竟然是南四奇老二“中平枪”花铁干。\n"NOR, me, this_object());
	command("dead "+me->query("id"));
        message_vision(CYN"$N怒视着$n，一字一顿地从牙缝中冷冷挤出三个字来：“鄙视你！！！”\n"NOR, ob, obj);
        message_vision(CYN"$N也怒视着$n，一字一顿地从牙缝中冷冷挤出三个字来：“鄙视你！！！”\n"NOR, me, obj);
	obj->kill_ob(ob);
        message_vision(HIR"\n$N对着$n冷笑道：小小鼠辈，也敢和我较量！\n" NOR, obj,ob);
	command("kill "+obj->query("id"));
	ob->kill_ob(obj);
  return;
}
string ask_yuanqu()
{
	object me,ob;
	me = this_player();
	ob = this_object();
	if(me->query_condition("killer"))
	{
	   command("fear "+me->query("id"));
	   return ""+RANK_D->query_respect(me) +"还是自己小心通缉吧！";
	}
	if(me->query(QUESTDIR1+"over"))
	{
	   command("shout");
	   message_vision(HIY"$N脸上露出异常的神色。\n", ob);
	   return "为什么是这样.........！";
	}
	if(!me->query(QUESTDIR1+"start" ))
	{
	   command("?");
	   return "你在说什么？";
	}
	if(!me->query_temp(QUESTDIR1+"kill_hua" ))
	{
	   command("sigh");
	   message_vision(HIY"$N沉默不语。\n", ob);
	   return "。。。。。。。";
	}
	if(!me->query_temp(QUESTDIR1+"askshifu" ))
	{
	   command("shout");
	   message_vision(HIY"$N一直沉默不语。\n", ob);
	   return "。。。。。。。";
	}
	command("flush");
	command("sigh");
	command("say 我受了多年的冤屈，是一点头绪也没有。当年万师伯大寿，也不知道为什么师傅竟然和万师伯冲突起来，\n"
		+"师父要杀万师伯，更不知道怎么师傅竟然莫名失踪。后来他们更是冤枉我，关我进了监狱多年，后来我在丁大哥的\n"
		+"帮助下逃了出来。这些年来，我无时无刻在回想当年场景，就是搞不明白。");
	message_vision(HIY"$N脸上露出复杂的神色。\n", ob);
	command("qrj");
	me->set(QUESTDIR1+"over",1);
	me->set(QUESTDIR+"time",time());
	me->set(QUESTDIR+"combat_exp",me->query("combat_exp")); 	
	me->delete_temp(QUESTDIR);
	me->delete_temp("quest/busy");//
	return "我也不知道师傅现在在哪里！而且什么时候能够离开这里，也是问题！";
}
string ask_dingdian()
{
	object me,ob;
	me = this_player();
	ob = this_object();
	if(me->query(QUESTDIR1+"over"))
	{
	   command("sigh");
	   message_vision(HIY"$N脸上露出怀念的神色。\n", ob);
	   return "也不知道丁大哥现在怎么样了！";
	}
	if(!me->query(QUESTDIR1+"start" ))
	{
	   command("?");
	   return "你找丁大哥干什么？";
	}
	if(!me->query_temp(QUESTDIR1+"trust" ))
	{
	   command("sigh");
	   message_vision(HIY"$N疑惑得看了看$n。\n", ob,me);
	   return "你找丁大哥干什么？";
	}
	command("sigh");
	message_vision(HIY"$N脸上露出怀念的神色。\n", ob);
	return "也不知道丁大哥现在怎么样了！";
}
string ask_shifu()
{
	object me,ob;
	me = this_player();
	ob = this_object();
	if(me->query_condition("killer"))
	{
	   command("fear "+me->query("id"));
	   return ""+RANK_D->query_respect(me) +"还是自己小心通缉吧！";
	}
	if(me->query(QUESTDIR1+"over"))
	{
	   command("en");
	   message_vision(HIY"$N脸上露出复杂的神色。\n", ob);
	   return "师傅啊，师傅.........！";
	}
	if(!me->query(QUESTDIR1+"start" ))
	{
	   command("say");
	   return "那是我师傅，找他什么事情？";
	}
	if(!me->query_temp(QUESTDIR1+"kill_hua" ))
	{
	   command("say");
	   return "那是我师傅，找他什么事情？";
	}
	if(me->query_temp(QUESTDIR1+"askshifu"))
	{
	   message_vision(HIY"$N脸上露出复杂的神色。\n", ob);
	   return "我也不知道师傅现在在哪里！只是当年一场祸变，我至今也是莫名其妙！";
	}    
	command("say 那是我师傅。");
	command("say");
	message_vision(HIY"$N脸上露出复杂的神色。\n", ob);
	me->set_temp(QUESTDIR1+"askshifu",1);
	return "我也不知道师傅现在在哪里！只是当年一场祸变，我至今也是莫名其妙！";
}
int ask_outxuegu() 
{
	object me = this_player();
	if(this_object()->is_fighting()) return 0;

	if(me->query("shen")<0)
	{
		command("heng");
		command("say "+HIY"我看" + RANK_D->query_respect(me) +"面带阴邪之气，恐怕是也是血刀老贼一伙的吧。"NOR);
		return 1;
	}  
	if(me->query_temp(QUESTDIR1+"outxuegu"))
	{
		command("pat "+me->query("id"));
		command("say "+HIY+ RANK_D->query_respect(me) +"只管自己跳起来。我会及时帮你的。"NOR);
		return 1;
	}  
	command("nod");
	command("say 大雪纷纷，如果没有外力帮忙的确很难独自离开这里的。");
	command("pat "+me->query("id"));
	command("say "+HIY"我看还是我来帮" + RANK_D->query_respect(me) +"一把吧，" + RANK_D->query_respect(me) +"只管自己跃起来。"NOR);
	me->set_temp("quest/outxuegu",1);//
	return 1;
} 
int ask_skills() 
{
	object me = this_player();
	int i;
	if(this_object()->is_fighting()) return 0;

	message_vision("$N向$n请教有关神照经的奥妙。\n", me, this_object());
	if(!me->query(QUESTDIR1+"start"))
	{
		command("?");
		command("say 我正忙着，你一边去。"NOR);
		return 1;
	}
	if(!me->query(QUESTDIR1+"over"))
	{
		command("pat");
		command("say 我现在正忙着，你先等等。"NOR);
		return 1;
	}
	if(me->query(QUESTDIR4+"shenzhao")>=2 || !me->query(QUESTDIR4+"shenzhao"))
	{
		command("pat");
		command("say "+HIY"你最好去丁大哥那里，其实我对神照经有些地方还是不能融会贯通。"NOR);
		command("addoil");
		return 1;
	}
	command("look "+me->query("id"));
	
	//普通玩家三次
	if(me->query(QUESTDIR4+"shenzhao_fail2")>=3 && me->query("registered")<3)
	{
		command("shrug "+me->query("id"));
		command("say 已经多次传授，你怎么都还没有领悟！");
		command("puke "+me->query("id"));
		return 1;
	}
	//武功学习之间需要间隔一天，经验需要间隔50K
	if(me->query(QUESTDIR4+"time") && time()-me->query(QUESTDIR4+"time")<86400)
	{
		command("shake "+me->query("id"));
		command("say 今天先教到到这里吧，明天吧。");
		return 1;
	}
	if(me->query(QUESTDIR4+"combat_exp") && me->query("combat_exp")-me->query(QUESTDIR4+"combat_exp")<50000)
	{
		command("look "+me->query("id"));
		command("say 以你当前的经验恐怕还是难以领悟，还是抓紧去练功去吧。");
		return 1;
	}
    
	me->set(QUESTDIR4+"time",time());
	me->set(QUESTDIR4+"combat_exp",me->query("combat_exp")); 
	i= random(20);
	command("say 丁大哥当年的确教我一套神照经的内功。我就把一些经验传授给你吧。");
	command("pat "+me->query("id"));
	if(i==1)
	{
		tell_object(me,HIY"\n你听了狄云的指点，对神照经的奥妙似乎有些心得。\n"NOR);
		tell_object(me,HIG"\n你试图将内息冲到百会穴中，突然只觉颜面上一阵清凉，一股凉气从额头、鼻梁、口唇下来，通到唇下\n"
			             +"的“承浆穴”。跟着一路下行，自廉泉、天突而至璇玑、华盖、紫宫、玉堂、膻中、中庭、鸠尾、巨阙，\n"
						 +"经上、中、下三脘，而至水分、神厥、气海、石门、关元、中极、曲骨诸穴，又回到了“会阴穴”。如此\n"
						 +"一个周天下来，郁闷之意全消，说不出的畅快受用。内息第一次通行时甚是艰难，任督两脉既通，道路熟\n"
						 +"了，第二次、第三次时自然而然的飞快运输，顷刻之间，连走了一十八次。\n"NOR);
		me->improve_skill("shenzhao-jing", 1000+random(me->query_int()*2));
		me->add("max_neili",10);//增加10点内力
		log_file("quest/LCHJ", sprintf("%8s(%8s) 失败%d次学到神照经更高绝技。i取值：%d|经验：%d。\n", me->name(1),me->query("id"), me->query(QUESTDIR4+"shenzhao_fail2"),i,me->query("combat_exp")) );
		command("pat "+me->query("id"));
		me->add(QUESTDIR4+"shenzhao",1);//
	}
	else
	{
		log_file("quest/LCHJ", sprintf("%s(%s) 高级神照经失败%d次。i取值：%d|经验：%d。\n", me->name(1),me->query("id"),me->query(QUESTDIR4+"shenzhao_fail2"), i,me->query("combat_exp")) );
		tell_object(me,HIY"你听了狄云的指点，可是对神照经的奥妙全然不得要领。\n\n"NOR);
		command("sigh "+me->query("id"));
		command("disapp "+me->query("id"));
		me->add(QUESTDIR4+"shenzhao_fail2",1);//
	}
	return 1;
} 
int ask_sword() 
{
	object me = this_player();
	int i;
	if(this_object()->is_fighting()) return 0;

	message_vision("$N向$n请教有关躺尸剑法的奥妙。\n", me, this_object());
	if(me->query(QUESTDIR1+"over") && me->query(QUESTDIR4+"sword") )
	{
		if(me->query("potential")>4)
		{
			me->receive_damage("jing", 10);
			tell_object(me,HIY"你听了狄云的指点，对躺尸剑法的奥妙似乎有些心得。\n"NOR);
			me->add("potential",-(random(3)+1));
			if( me->query("potential")<0)
				me->set("potential",0);
			me->improve_skill("tangshi-jianfa", 30+random(me->query_int()));
			me->improve_skill("sword", random(me->query_int())/2);
		}
		else 
			tell_object(me,HIY"你听了狄云的指点，可是还是不能理解。\n"NOR);
		return 1;
	}
	if(!me->query(QUESTDIR1+"over"))
	{
		command("?");
		command("say 我正忙着，你一边去。"NOR);
		return 1;
	}
	command("look "+me->query("id"));	
	//普通玩家三次
	if(me->query(QUESTDIR4+"sword_fail")>=3 && me->query("registered")<3)
	{
		command("shrug "+me->query("id"));
		command("say 已经多次传授，你怎么都还没有领悟！");
		command("puke "+me->query("id"));
		return 1;
	}
	//武功学习之间需要间隔一天，经验需要间隔50K
	if(me->query(QUESTDIR4+"time") && time()-me->query(QUESTDIR4+"time")<86400)
	{
		command("shake "+me->query("id"));
		command("say 今天先教到到这里吧，明天吧。");
		return 1;
	}
	if(me->query(QUESTDIR4+"combat_exp") && me->query("combat_exp")-me->query(QUESTDIR4+"combat_exp")<50000)
	{
		command("look "+me->query("id"));
		command("say 以你当前的经验恐怕还是难以领悟，还是抓紧去练功去吧。");
		return 1;
	}
    
	me->set(QUESTDIR4+"time",time());
	me->set(QUESTDIR4+"combat_exp",me->query("combat_exp")); 
	i= random(10);
	command("say 这是我师门的武功。我就把一些经验传授给你吧。");
	command("smile "+me->query("id"));
	if(i==1)
	{
		tell_object(me,HIY"\n你听了狄云的指点，对躺尸剑法的奥妙似乎有些心得。\n"NOR);
		tell_object(me,HIW"\n你一招‘哥翁喊上来’，跟着一招‘是横不敢过’，接着两招是‘忽听喷惊风，连山若布逃’，剑势就象\n"
			             +"一匹布那样逃了开去。而后两招便是‘落泥招大姐，马命风小小’。每一招出去，敌人似乎就要躺下成为\n"
						 +"一具死尸。虽然名字有些不雅，但是招数的确狠辣异常。\n"NOR);
		me->improve_skill("tangshi-jianfa", 10+random(me->query_int()));
		command("look "+me->query("id"));
		command("buddha "+me->query("id"));
		command("haha "+me->query("id"));
		me->set(QUESTDIR4+"sword",1);//
		log_file("quest/LCHJ", sprintf("%8s(%8s) 失败%d次学到躺尸剑法。i取值：%d|经验：%d。\n", me->name(1),me->query("id"), me->query(QUESTDIR4+"sword_fail"),i,me->query("combat_exp")) );
	}
	else
	{
		me->add(QUESTDIR4+"sword_fail",1);//
		command("buddha");
		tell_object(me,HIY"你听了狄云的指点，可是对躺尸剑法的奥妙全然不得要领。\n\n"NOR);
		command("sigh "+me->query("id"));
		command("disapp "+me->query("id"));
		log_file("quest/LCHJ", sprintf("%8s(%8s) 躺尸剑法失败%d次。i取值：%d|经验：%d。\n", me->name(1),me->query("id"), me->query(QUESTDIR4+"sword_fail"),i,me->query("combat_exp")) );
	}
	return 1;
} 

void destroying(object me,object obj)
{
        destruct(obj);
        return;
}

