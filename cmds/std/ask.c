// ask.c
// LinuX@SJ 2004/03/09 

#include <ansi.h>
inherit F_CLEAN_UP;

string query_inquiry(object me,object ob);

string *msg_dunno = ({
	CYN"$n摇摇头，说道：没听说过。\n"NOR,
	CYN"$n睁大眼睛望着$N，显然不知道$P在说什么。\n"NOR,
	CYN"$n耸了耸肩，很抱歉地说：无可奉告。\n"NOR,
	CYN"$n说道：嗯……这我可不清楚，你最好问问别人吧。\n"NOR,
	CYN"$n想了一会儿，说道：对不起，你问的事我实在没有印象。\n"NOR,
	CYN"$n摇摇头，说道：我没工夫闲聊。\n"NOR,
	CYN"$n耸了耸肩，很抱歉地说：我对闲谈不感兴趣。\n"NOR,
	CYN"$n不耐烦的瞪了$N一眼，说道：没看我正忙着吗？\n"NOR,
	CYN"$n看了$N一眼，说道：对不起。又接着忙自己的事去了。\n"NOR,
	CYN"$n笑嘻嘻看着$N，说道：你想聊什么？\n"NOR,
	CYN"$n老大不愿意的看了$N一眼，又转过身去了。\n"NOR,
	CYN"$n恶狠狠地瞪了$N一眼，说道：“烦着呢！别理我！”\n"NOR,
	CYN"$n看了你一眼，显出一付不信任的样子，摇了摇头。\n"NOR,
	CYN"$n耸了耸肩，很抱歉地说：无可奉告。\n"NOR,
	CYN"$n说：“我就是$n，你难道不知道吗？”\n"NOR,
	CYN"$n说：“.............................”\n"NOR,
	CYN"$n说：“练武要有节制，当心走火入魔。”\n"NOR,
	CYN"$N还没来得及同$n交谈，$n就转过身去了。\n"NOR,
	CYN"$n说：“呵呵，这个么。。。。。。今儿天气真好！”\n"NOR,
	CYN"$n说：“练武讲求独辟捷径，如果只是跟着师傅学，是不会有好结果的”\n"NOR
});

string *msg_party = ({
	CYN"$n哼了一声，说道：我们$T你都没听说过？我看你是不想混了罢。\n"NOR,
	CYN"$n挺胸大声道：你要是想学武，早日加入我们$T没错！\n"NOR,
	CYN"$n恶狠狠地对$N说道：你招子放亮点，看看我是干什么的。\n"NOR,
	CYN"$n小声说道：你想加入我们$T吗？\n"NOR,
	CYN"$n得意地说道：江湖上有谁不知道我们$T的名号！\n"NOR,
	CYN"$n警觉地看了$N一眼，沉声道：你打听我们$T的事情干什么？\n"NOR
});

string *msg_foreign = ({
        CYN"$n说道：你在说外国话吧？我不会，你最好带个翻译来。\n"NOR,
        CYN"$n嘻嘻笑道：你说什么鸟语？\n"NOR,
        CYN"$n说道：才阿八热古里古鲁。你看，我也能假装会说外国话。\n"NOR
});

void create()
{
	seteuid(getuid());
}

int main(object me, string arg)
{
	string dest, msg, topic, att;
	object ob;
	int iyes = 0;
	//int m;
	
	if (me->is_busy())
		return notify_fail("您先歇口气再说话吧。\n");

	if (!arg || sscanf(arg, "%s about %s", dest, topic) != 2)
		return notify_fail("你要问谁什么事？\n");

	if (!objectp(ob = present(dest, environment(me))))
		return notify_fail("这里没有这个人。\n");

	if (ob == me) return notify_fail("问自己？\n");
	if (!ob->is_character()) {
		message_vision("$N对着$n自言自语……\n", me, ob);
		return 1;
	}
	if (ob->query("mute"))
		message_vision("但是很可惜，$N不会说话。\n", ob);

	if (!ob->query("can_speak")) {
		message_vision("$N向$n打听有关『" + HIG +topic + NOR + "』的消息，但是$p显然听不懂人话。\n", me, ob);
		return 1;
	}

	if ( iyes = !INQUIRY_D->parse_inquiry(me, ob, topic))
		message_vision("$N向$n打听有关『" + HIG +topic + NOR + "』的消息。\n", me, ob);

	if (userp(ob)) return 1;
	if (!living(ob)) {
		message_vision("但是很显然的，$n现在的状况没有办法给$N任何答覆。\n", me, ob);
		return 1;
	}
	
	

	dest = base_name(environment(ob));

	if ((msg = ob->query("startroom"))
	&& msg != dest
	&& (msg = ob->query("lastroom"))
	&& (msg == dest || msg->is_character())
	&& ob->query("inquiry/" + topic, 1))
		message_vision(CYN "$N说道："+RANK_D->query_self(ob)+"是出来游山玩水的，不回答问题。\n" NOR, ob);
	else if (msg = ob->query("inquiry/" + topic)) {
		if (stringp(msg)) ob->force_me("say " + msg);
	}
	/*else if ( topic=="all" ){	// add by LinuX@SJ
		 query_inquiry(me,ob);
	} */
	else if (topic[0] < 128  && iyes) {
                message_vision(msg_foreign[random(sizeof(msg_foreign))], me, ob);
	}
	else {
		if (ob->query("mute"))
			message_vision("但是很可惜，$N不会说话。\n", ob);
		else switch(topic) {
			case "name":
				att = ob->query("attitude");		
				switch (att)
				{
				case "aggressive":
				case "killer":
					message_vision(YEL"$N对$n把眼一瞪："+RANK_D->query_self_rude(ob)
						+"的名字是可以随便提的吗？！我看你这"+RANK_D->query_rude(me)
						+"是活腻了！\n"NOR, ob, me);
					break;	
				case "heroism":
					message_vision(CYN"$N对$n哈哈一笑："+ob->query("name")+"便是"
						+RANK_D->query_self_rude(ob)+"！\n"NOR, ob, me);
					break;
				case "peaceful":
				case "friendly":
					message_vision(CYN"$N对$n作了一揖：这位"+RANK_D->query_respect(me)
						+"可真会开玩笑，怎么会突然问起"+RANK_D->query_self(ob)
						+"的名字？\n"NOR, ob, me);
					EMOTE_D->do_emote(ob, "sigh", ob->query("id"), 0);
					break;	
				default:
					message_vision(CYN"$N对$n作了一揖：这位"+RANK_D->query_respect(me)
						+"可真会开玩笑，怎么会突然问起"+RANK_D->query_self(ob)
						+"的名字？\n"NOR, ob, me);
					EMOTE_D->do_emote(ob, "sigh", ob->query("id"), 0);
					break;	
				}		
				break;
			case "here":
				message_vision(CYN "$n对$N说道：这里是" + environment(ob)->query("short") + CYN "，其它情况" + RANK_D->query_self(ob) + "也不太清楚。\n" NOR, me, ob);
				break;
			case "rumors":
				EMOTE_D->do_emote(ob, "dunno");
				break;
			case "news":
			case "大事":
			case "新闻":
                                EMOTE_D->do_emote(ob, "nod");
                                msg = "/cmds/usr/news.c"->read_last_news();
                                //message_vision( CYN "$N对你点了点头说道：我最近听说有一件大事情哦，让我悄悄告诉你吧。\n" NOR, ob);
                                ob->force_me("whisper " + me->query("id") +" "+msg);
                                break;
                        case "bbb":
                        case "yuj":
                        case "linux":
                        	EMOTE_D->do_emote(ob, "fear");
                        	message_vision(CYN "$N对$n说道：「别在我面前提这么可怕的名字好不好。」\n" NOR, ob,me);
				break;
			/*case "任督二脉":
				if ((int)ob->query_skill("force",1) < 60 ) 
					message_vision(msg_dunno[random(sizeof(msg_dunno))], me, ob);
				else if(!me->query("family") || me->query("family/master_id") != ob->query("id")) 
					message_vision(CYN "$N说道：「如若能打通任督二脉，内功及各项修为都会大有进展。」\n" NOR, ob);
				else if((int)me->query_skill("force",1) < 100 ) 
					message_vision(CYN "$N对$n说道：「你内功修为不足，如果强行打通任督二脉，只怕有伤经脉。」\n" NOR, ob,me);
				else if(me->query("quest/rendu_2m")) 
					message_vision(CYN "$N看了$n两眼，冷冷地道：「你任督二脉早已打通了吧。」\n" NOR, ob,me);
				else if((int)me->query_skill("force",1) > 120 ) 
					message_vision(CYN "$N对$n说道：「你的内功修为已入佳境，似乎任督二脉已自行打通了。」\n" NOR, ob,me);
				else if((int)ob->query_skill("force",1) < 140 ) 
					message_vision(CYN "$N脸上显出一丝难过，淡淡道：「以我现在的内功修为，只怕还无法打通你的任督二脉。」\n" NOR, ob);
				else
				{
					int m;
					me->set("quest/rendu_2m",1);
					message_vision(HIW "$N看了$n两眼，点了点头，接著在$n头，胸，背前各拍了一掌。\n" NOR, ob,me);  
					m = me->query("con")+ random(me->query("con"))+random((int)me->query_skill("force",1)/10);
					me->set_skill("force",(int)me->query_skill("force",1)+random(5));
					me->add("eff_jingli",m);
					me->add("max_jingli",m);
					me->add("max_neili",m + random(10));
					write(HIG"\n你只觉全身内息突然一滞，接著畅通无比，似乎更可以流向过去所不能及的几处大穴。\n"
						"你微一运气，感到内力比以前好像强了许多，精神也为之一振！\n"NOR);
					me->start_busy(8);
				}
				break;*/
			default:
				if (topic == ob->query("name") || ob->id(topic))
					message_vision(CYN "$N说道：那就是" + RANK_D->query_self_rude(ob) + CYN "呀。\n" NOR, ob);
				else if (topic == me->query("name") || me->id(topic) || topic =="猪头" || topic =="小猪")
					message_vision(CYN "$N说道：那就是" + RANK_D->query_rude(me) + CYN "你呀。\n" NOR, ob);
				else if (topic == me->query("family/family_name"))
					message_vision(CYN "$N奇道：" + RANK_D->query_rude(me) + CYN "你就身在"+topic+"还要问我嘛？\n" NOR, ob);
				else if (topic == ob->query("family/family_name")){
					msg = msg_party[random(sizeof(msg_party))];
					msg = replace_string(msg, "$T", topic);
					message_vision(msg, me, ob);
				}
				else
					message_vision(msg_dunno[random(sizeof(msg_dunno))], me, ob);
		}
	}
	me->add_busy(1);
	return 1;
}
//从npc身上读取inquiry
/*string query_inquiry(object me,object ob)
{
	int i, j;
	mapping inq;
	string str="", msg, *indexs;
	j = 0;
	
	if(mapp(inq=ob->query("inquiry")))
	{
		indexs = sort_array(keys(inq),1);
		i = sizeof(indexs);
		if (i < 5)
		{
			while(i--){
	        	msg = indexs[i]; 
	        	if (i == 0)
	                	str = str + sprintf("%s",msg);
	                else	        	    
	                	str = str + sprintf("%s、",msg);
			}	
			ob->force_me("whisper " + me->query("id") + " 你可以向我打听有关" + HIG +
			str + NOR + "等的事情，我可以一一告诉你。");	
		}
		else
		{
		        while(i--){
		        	msg = indexs[i];
		        	//if (random(3)) continue;      
		                str = str + sprintf("%-10s",msg);
				j++;
				if (j % 8 == 0) str = str + "\n";
		        }
		        ob->force_me("whisper " + me->query("id") + " 你可以向我打听有关\n" + HIG +
			str + NOR + "\n这些事情，我可以一一告诉你。");
		}

		return "";
	}
	ob->force_me("say 实在是对不起,我什么也不知道呀!");
	return "";
}*/
int help(object me)
{
	write( @HELP
指令格式: ask <someone> about <something>

这个指令在解谜时很重要, 通常必须藉由此一指令才能
获得进一步的资讯。
HELP
	);
	return 1;
}
