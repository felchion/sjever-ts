// chant.c
#include <ansi.h>
 
inherit F_CLEAN_UP;

mapping *word = ({
([      "str"		:	"luojia",
	"name"		:	"摩呼洛迦",
        "cost"  	: 	50,
        "lvl"		:	220,
]),
([      "str"		:	"jinaluo",
	"name"		:	"紧那罗",
        "cost"  	: 	50,
        "lvl"		:	320,
]),
([      "str"		:	"jialou",
	"name"		:	"迦楼罗",
        "cost"  	: 	200,
        "lvl"		:	320,
]),
([      "str"		:	"xiuluo",
	"name"		:	"阿修罗",
        "cost"  	: 	50,
        "lvl"		:	400,
]),
([      "str"		:	"gandapo",
	"name"		:	"乾达婆",
        "cost"  	: 	50,
        "lvl"		:	400,
]),
([      "str"		:	"yecha",
	"name"		:	"夜叉",
        "cost"  	: 	200,
        "lvl"		:	450,
]),
([      "str"		:	"long",
	"name"		:	"龙",
        "cost"  	: 	50,
        "lvl"		:	500,
]),
([      "str"		:	"tian",
	"name"		:	"天",
        "cost"  	: 	300,
        "lvl"		:	550,
]),
});

int luojia(object me,int cost,int lvl);
int jinaluo(object me,object ob,int cost,int lvl);
int jialou(object me,int cost,int lvl);
int xiuluo(object me,int cost,int lvl);
int gandapo(object me,int cost,int lvl);
int yecha(object me,int cost,int lvl);
int long(object me,int cost,int lvl);
int tian(object me,int cost,int lvl);

int do_recover(object me,int cost,int lvl);

int main(object me, string str)
{
        string who;
        object ob;
	int i,j,lvl=me->query_skill("cibei-fofa", 1);
	int this_week=time()/86400/7;
	int this_day=time()/86400;
	
        if (!str) return notify_fail("你要吟诵什么？\n");
        
        if (sscanf(str, "%s %s", str,who)==2)     
        	if (!(ob = present(who, environment(me))))
        		return notify_fail("你要对谁吟诵？\n");

	
	for (i=0;i<sizeof(word);i++)
		if (str == word[i]["str"])  
			j=i+1;	
	if (!j)
		return notify_fail("你嘴里乱言乱语不知道念着些什么？\n");
	j--;	
	
	if (lvl<word[j]["lvl"])
        	return notify_fail("你佛法等级太低，无法吟诵这个佛号。\n");
        
        if (me->query("neili")<word[j]["cost"]*30)
        	return notify_fail("你内力不足，无法吟这个佛号。\n");
        
        if (me->is_busy()) 
		return notify_fail("你正忙着呢，怎可一心两用。\n");
	
	if (me->is_fighting())
		return notify_fail("你还是解决了对手再说吧。\n");
        	
	switch(j)
	{
		case 0:
			luojia(me,word[j]["cost"],lvl);
			break;
		case 1:
			jinaluo(me,ob,word[j]["cost"],lvl);
			break;
		case 2:
			if (me->query("reborn/fofa/jll") == this_week) return notify_fail("你重生的时间还没到呢。\n");	
			if (me->query("last_death") == "已浴火重生") return notify_fail("你已经重生过了。\n");
			me->set("reborn/fofa/jll",this_week);
			jialou(me,word[j]["cost"],lvl);
			break;
		case 3:
			xiuluo(me,word[j]["cost"],lvl);
			break;
		case 4:
			gandapo(me,word[j]["cost"],lvl);
			break;
		case 5:
			yecha(me,word[j]["cost"],lvl);
			break;
		case 6:
			long(me,word[j]["cost"],lvl);
			break;
		case 7:
			tian(me,word[j]["cost"],lvl);
			break;
	}
	
        return 1;
}

int luojia(object me,int cost,int lvl)
{
	me->add("max_neili",-(cost+random(cost)));
        me->add("neili",-(cost+random(cost)*10));
       	message_vision("$N慈悲心动处，佛音自嘹亮。袍袖一舞，口宣佛号：「"+
       	                 HIW"已能修习善巧方便，令此诸众生永离爱缠。"NOR"」\n",me);
	message_vision("$N将身俯卧在地，对周围事物是不感不知，心中却清明如许，正是摩呼罗迦明王之象。\n", me);
	me->set("channels", ({}));
	me->apply_condition("fofa_mfjl",lvl+random(lvl));
}

int jinaluo(object me,object ob,int cost,int lvl)
{       
        me->add("max_neili",-(cost+random(cost)));
        me->add("neili",-(cost+random(cost)*10));
        ob->reincarnate();
	ob->set("neili", me->query("max_neili")*2);
        ob->clear_condition();

       	message_vision("$N慈悲心动处，佛音自嘹亮。袍袖一舞，口宣佛号：「"+
       	                 HIY"皆悉具于清净妙慧。身心快乐自在游戏。"NOR"」阵阵梵音如同天籁传向$n。\n",ob, me);
       	message_vision(WHT"$n身心皆被梵音佛号笼罩，顿时身心恢复了活力。\n"NOR,ob, me);
       
        return 1;
}

int jialou(object me,int cost,int lvl)
{
	
	message_vision("$N慈悲心动处，佛音自嘹亮。袍袖一舞，口宣佛号：「"+
       	                 HIY"悉皆成就不起法忍。善权饶益一切众生。"NOR"」\n", me);
       	message_vision("$N身边顿时火光闪耀，直让人无法睁开双眼，火焰越来越高，越来越高……。\n\n", me);    
       	me->start_busy(5);
       	call_out("do_recover",3,me,cost,lvl);
        return 1;	                 
}

int xiuluo(object me,int cost,int lvl)
{
	int j;
	message_vision("$N慈悲心动处，佛音自嘹亮。面目忽然显修罗怒目象，口颂佛号：「"+
       	                 HIR"善能修习离诸我慢。受持大乘尊重三宝。"NOR"」\n",me);
	j=lvl/7;
	j=j+random(j/2);
	me->add("max_neili",-(cost+random(cost))/2);
        me->add("neili",-(cost+random(cost)*10));
	me->add_temp("apply/attack", j/3*2);
	me->add_temp("apply/damage", j/3);
	me->set_temp("xiuluo/attack",j/3*2);
	me->set_temp("xiuluo/damage", j/3);
	me->apply_condition("fofa_axl",lvl+random(lvl));
	return 1;       
}

int gandapo(object me,int cost,int lvl)
{
	me->add("max_neili",-(cost+random(cost)));
        me->add("neili",-(cost+random(cost)*10));
       	message_vision("$N慈悲心动处，佛音自嘹亮。袍袖一舞，口宣佛号：「"+
       	                 HIG"皆于大乘深生爱敬。利乐众生恒无懈倦。"NOR"」顿时身上散发出一阵飘渺之香气。\n",me);
	me->apply_condition("fofa_gdp",lvl+random(lvl));
	return 1;
}

int yecha(object me,int cost,int lvl)
{
}

int long(object me,int cost,int lvl)
{
	int i;
	message_vision("$N慈悲心动处，佛音自嘹亮。你长身一起，口颂佛号：「"HIY"悉皆爱乐大乘妙法。发弘誓愿恭敬护持。"NOR"」，身形竟然随着佛音高大了许多。\n",me);
	i=lvl/5;
	i=i+random(i/2);
	me->add("max_neili",-(cost+random(cost))/2);
        me->add("neili",-(cost+random(cost)*10));
	me->add_temp("apply/intelligence",i/3);
	me->add_temp("apply/strength",i);
	me->add_temp("apply/dexerity",i/2);
	me->add_temp("apply/constitution",i/5*4);
	me->set_temp("lw/int",i/3);
	me->set_temp("lw/con",i/5*4);
	me->set_temp("lw/str",i);
	me->set_temp("lw/dex",i/2);
	me->apply_condition("fofa_lw",lvl+random(lvl));
	return 1;    
}

int tian(object me,int cost,int lvl)
{
}

int do_recover(object me,int cost,int lvl)
{
	int i;
	mapping skills, learned;
	string *sk;
	
	if (lvl>random(700) && me->query("kar")>random(15))
	{
		message_vision(HIR"\n忽然，仿佛有一只大鸟从烈焰中腾空而起，闪耀到照亮整个天际。\n"NOR, me);  
		write(HIR"你觉得自己如同在烈火中获得了新生一般。\n"NOR);  
		me->add("max_neili",-(cost+random(cost)));
        	me->add("neili",-(cost+random(cost))*10);
        	
       		skills = me->query_skills();
		if (mapp(skills)) {
			sk = keys(skills);
			me->improve_skill(sk[0], 1, 1);
			learned = me->query_learned();
			learned[sk[0]]--;
			foreach (string skill in sk) {
				i = (skills[skill]+1)*(skills[skill]+1);
				if ((learned[skill]+=i/2) > i) {
					skills[skill]++;
					learned[skill] -= i;
				}
			}
		}
		me->add("combat_exp",me->query("combat_exp")/99+1);
		me->add("shen", me->query("shen")/9);
		i = me->query("potential");
		i += i/3;
		if (i > me->query("max_pot")) i = me->query("max_pot");
		me->set("potential", i);
		i = me->query("death_count") - 1;
		if (i < 0) i = 0;
		me->set("death_count", i);
		if (i < me->query("death_times")) me->set("death_times", i);
		me->reincarnate();
		me->set("last_death", "已浴火重生");
		me->delete("last_killer_id");
		me->delete("last_death_time");
	}
	else
	{
		me->add("max_neili",-(cost+random(cost)/2));
        	me->add("neili",-(cost+random(cost))*5);
		message_vision(HIR"\n火焰"NOR"渐渐暗淡了下去，长空中留下几缕空山的罡风。\n"NOR, me);  
	}
        return 1;
}


int help(object me)
{
write(@HELP
指令格式 : chant <佛号> <某人>

此指令可让你吟诵佛号。
HELP
    );
    return 1;
}

