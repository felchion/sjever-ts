// forbook.c

// Modified by mxzhao 2004/03/06 caijian => zhizao

#include <ansi.h>
#define MAX_SKILLS 15

string getscore(string* vb);

static string *weapon_list =
	({"sword","axe","blade","brush","club","dagger","fork","hammer","hook","spear","staff","stick","xiao","whip",});
static string *armor_list =
	({"cap","boot","coat","armor","mantle","belt","glove",});
static string *all_list =
	({"sword","axe","blade","brush","club","dagger","fork","hammer","hook","spear","staff","stick","xiao","whip",
	"cap","boot","coat","armor","mantle","belt","glove",
	});
static mapping get_chinese = ([
	"axe":"斧",
	"brush":"笔",
	"fork":"叉",
	"hook":"钩",
	"spear":"枪",	
	"whip":"鞭",
	"stick":"棒",
	"sword":"剑",
	"xiao":"萧",
	"blade":"刀",
	"dagger":"匕首",
	"club":"棍",
	"staff":"杖",
	"hammer":"法轮",
	"throwing":"暗器",
	"cap":"头盔",
	"glove":"手套",
	"boot":"靴",
	"belt":"腰带",
	"mantle":"披风",
	"coat":"彩衣",
	"armor":"甲胄",
	"cloth":"衣服",
	"finger":"戒指",
	"neck":"项链",	
	"wrists":"护胸",	
	"hands":"指套",
	"mineral":"矿石",
	"jade":"玉",
	"silk":"丝",
	"cloths":"布",
]);

string getscore(string* vb)
{
	int i;
	string str;
	
	if(!arrayp(vb)) return "";
	i = sizeof(vb);	
	if (i<=0) str = HIY "你目前不会任何工匠技能。\n"NOR;
	else {
		str = HIY"你已经学会了";
		while(i--)
			str +=get_chinese[vb[i]]+"("+capitalize(vb[i])+")、";
		str += "的工匠技术。\n"NOR;
		str = replace_string(str,"、的","的");
	}	
	return str;	
}

int accept_object(object who, object ob, object me){
	string title;
	string *vb = ({});
	mapping myskill;
	string skill,skill_name;	
	int i;
	
	me = this_object();
	if(me->id("zhujian shi") && ob->query("dazao_award") && ob->id("weilan's hammer"))
	{
		command("ah");
		command("thank "+getuid(who));
		command("whisper "+getuid(who)+" 我也不好什么都不表示，就请韦兰再帮你打造一次吧。");
		call_out("destroying", 1, ob);
		who->add("imbue_reward",1);
		return 1;
	}
	if(!stringp(me->query("forg_type"))) return 0;
	if(me->query("forg_type")!="weapon" && me->query("forg_type")!="armor" && me->query("forg_type")!="all" ) return 0;
	if(me->query("forg_type")=="weapon"){
		skill = "duanzao";
		skill_name = "锻造";
		vb = weapon_list;
	}
	else if(me->query("forg_type")=="armor"){
		skill = "zhizao";
		skill_name = "织造";
		vb = armor_list;
	}
	else if(me->query("forg_type")=="all"){
		skill = "";
		skill_name = "工匠";
		vb = all_list;
	}
	if (skill=="" && ob->query("name") == HIW"付韩兄书"NOR && me->query("id")=="han tiejiang" && !mapp(who->query("forging")))
	{
		message_vision("$N上下打量了$n两眼。\n$N忽然长叹：「三十多年啦，今日之江湖已非往日之江湖啊。万老头却还在教导娃儿，这份苦心我自愧不如啊。」\n",
		me,who);
		command("say 既如此，我就传授娃儿你一些工匠之术。");
		title = "工匠之术，传自轩辕，武林中更分为";
		for(i=0;i<sizeof(vb);i++)
			title += get_chinese[vb[i]]+"("+vb[i]+") ";
		title +="等";
		command("say "+title);
		tell_object(who, YEL "请输入taojiao 相关打造技能的西文代码 from id 来学习你想要的工匠之术。\n"NOR);
		tell_object(who, YEL "比如你要学习打造剑，你可以输入taojiao sword from han。\n"NOR);
		who->set("dazao/give",1);//避免玩家离线损失，改成永久变量
		call_out("destroying", 1, ob);
		return 1;
	}	
	else if ( ob->query("for_forg") ){
	command("look "+who->query("id",1));
	if (who->query("dazao/give")){
		command("say 你小小年纪，倒是找到很多好东西，不过你前一次机会还没用掉呢！");
		command("say 等你用掉后(taojiao ***)再给我吧！");
		return 0;
	}
	
	title = skill_name+"之术，传自轩辕，武林中更分为";
	for(i=0;i<sizeof(vb);i++)
		title += get_chinese[vb[i]]+"("+vb[i]+") ";
	title +="等";
	command("say "+title);
			
	myskill = who->query("forging");
	if(!mapp(myskill)) myskill =([]);
	vb= keys(myskill);
	i = sizeof(vb);
	if (!mapp(myskill)) i = 0;
	
	if ( who->query_skill(skill,1) / 30 <= i && who->query_skill(skill,1)!=0 ){
	command("say 不过你"+skill_name+"术学艺不精，再多的书也没有用，还是先回去吧，等你"+skill_name+"术高时找我不迟！");
	return 0;
	}		
	command("say 没想到竟有这样的前辈秘籍在你的手上，我也不能白收你，我们可一起参详"+skill_name+"的上乘技术。");

	switch(i){
		case 0:
		command("say "+RANK_D->query_respect(who)+"对于"+skill_name+"之道只知一二却有缘得此宝书，实乃福泽深厚啊。");
		break;
		case 4:
		command("say "+RANK_D->query_respect(who)+skill_name+"技术已经炉火纯青,相信此书能助你一臂之力。");
		break;
		case 8:
		command("say "+RANK_D->query_respect(who)+"已经深谙此道，得此神书更是如虎添翼！");
		command("tsk "+who->query("id",1));
		break;
		case 12:
		command("say "+RANK_D->query_respect(who)+"今日之成就可追当年干将莫邪！");
		command("flatter "+who->query("id",1));
		break;
		case MAX_SKILLS:
		command("say "+RANK_D->query_respect(who)+"已入神匠之列，此书再也用不着了。");
		title = "神匠 " + who->query("name") + "(" + capitalize(who->query("id")) + ")";
		who->set_temp("apply/short", ({title}));
		return 0;
		default:
		command("say 希望"+RANK_D->query_respect(who)+"好好研习此书，日后必得大助");
		command("addoil "+who->query("id",1));
	}
	command("say 那我们就讨教一下有关"+skill_name+"之术吧!");
	tell_object(who,getscore(vb));
	tell_object(who, YEL "请输入taojiao 相关打造技能的西文代码 from id 来学习你想要的"+skill_name+"之术。\n"NOR);
	//tell_object(who, YEL "比如你要学习打造剑，你可以输入taojiao sword。\n"NOR);
	who->set("dazao/give",1);
	call_out("destroying", 1, ob);
	return 1;
	}
       message_vision(CYN "$N" CYN "把$n" CYN "递过去的" + ob->name() +
                                       CYN "推了回来，摇头道：无功不受禄。\n" NOR,
                                       this_object(), who);
	return 0;
}

int do_taojiao(string arg)
{
	object me;
	string *vb;
	object ob;
	mapping myskill;	
	string skill,skill_name;
	string sf;
	int i;
	
	me = this_object();	
	
	if(!stringp(me->query("forg_type"))) return 0;		
	if(me->query("forg_type")!="weapon" && me->query("forg_type")!="armor" && me->query("forg_type")!="all" ) return 0;
	if(me->query("forg_type")=="weapon"){
		skill = "duanzao";
		skill_name = "锻造";
		vb = weapon_list;
	}
	else if(me->query("forg_type")=="armor"){
		skill = "zhizao";
		skill_name = "织造";
		vb = armor_list;
	}
	else if(me->query("forg_type")=="all"){
		skill = "";
		skill_name = "工匠";
		vb = all_list;
	}
	
	me = this_player();	
	if( !me->query("dazao/give")) return 0;
	myskill = me->query("forging");
	if(!mapp(myskill)) myskill=([]);
	if( !arg || sscanf(arg,"%s from %s",arg,sf) !=2 || !sf)
		return notify_fail("你要向谁讨教什么？\n");	
	if(!objectp(ob = present(sf,environment(me))))
		return notify_fail("这里没这个人。\n");
	if(ob->parse_command_id_list()[0] != this_object()->parse_command_id_list()[0]) return 0;
	//if ((int)me->query_skill(skill,1) < 10){
		//command("say "+RANK_D->query_self(this_object())+"这里学习打造项目，至少需要"+skill_name+"术十级。");
		//return 1;
	//}
	if( !arg || arg=="" ) {
		arg = skill_name+"之术，传自轩辕，武林中更分为";
		for(i=0;i<sizeof(vb);i++)
			arg += get_chinese[vb[i]]+"("+vb[i]+") ";
		arg +="等";
		command("say "+arg);
		command("say 那我们就讨教一下有关"+skill_name+"之术吧!");
		vb = keys(myskill);
		tell_object(me,getscore(vb));
		tell_object(me, YEL "请输入taojiao 相关工匠技能的西文代码 from id 来学习你想要的"+skill_name+"之术。\n"NOR);
		//tell_object(me, YEL "比如你要学习打造剑，你可以输入taojiao sword。\n"NOR);
		return 1;
	}
	
	if( member_array(arg,vb) == -1 ){
	command("ah");
	return notify_fail(this_object()->name()+"不曾记得是否还有此技啊……\n");
	}
	if (mapp(myskill)){
		vb = keys(myskill);
		if( member_array(arg,vb) != -1 ){
		command("shake");
		return notify_fail("你既然已经学会此技，何必浪费机遇呢？\n");
		}
		
		if (skill!="" && me->query_skill(skill,1) / 30 <= sizeof(vb) ){
		command("say 你"+skill_name+"之术学艺不精，再多的书也没有用，还是先回去吧，这次机会我给你保留着，等你"+skill_name+"之术高时找我不迟！");
		//command("say 依我看，你目前的技能最多可以学"+HIY+chinese_number(me->query_skill("forging",1) / 30)+NOR+"项打造项目");
		return 1;
		}
	}
	command("ok");	
	if(skill!="") WORKER_D->check_impove(me,skill,me->query_skill(skill,1)*me->query_skill(skill,1),2);	
	me->set("forging/"+arg,1);
	me->delete("dazao/give");
	if(stringp(get_chinese[arg])) arg = get_chinese[arg];
	message_vision("$N向$n仔细讨教了有关"+HIY+arg+NOR+"方面的打造知识。\n",me,  this_object());
	tell_object(me, WHT "你学会了的"+HIY+arg+WHT+"方面打造知识，感觉自己的"+skill_name+"术也有所提高。\n"NOR);
	return 1;
}