// artname.c
// by snowman@SJ 22/05/2000
// bbb 说加点废话，以便更新！！！

inherit F_CLEAN_UP;
inherit F_SAVE;
#define LIMIT 11

#include <ansi.h>
#include <database.h>


static string *banned_sword_name = ({
"杀手", "闪电","逍遥","雪人", "某人","他妈的","去你的", "毛泽东", "邓小平", "江泽民", "巫师",
"书剑", "操你妈", "干你娘","金庸", "我们","你们", "他们", "大家", "刺猬","爱妻丸",
});

mapping artnames;
mapping query_artnames(){ return artnames; }

int remove()
{
        save();
        return 1;
}

string query_save_file()
{
        return "/log/artname_vote";
}

void create()
{
	seteuid(getuid());
        restore();
        if(!artnames) artnames = ([ ]);
}

string clean_all()
{
        artnames = ([ ]);
        remove();
        return "提议全部清楚完毕。\n";
}

string clean_name(string name)
{
	map_delete(artnames, name);
        remove();
        return name+"的提议清楚完毕。\n";
}

int check_legal_name(string name)
{
	int i = strlen(name);
	if (i < 2 || i > 5) {
		write("对不起，您要提议的的外号必须是二到五个中文字。\n");
		return 0;
	}


	if(!is_chinese(name) ) {
			write("对不起，请您用「中文」取外号。\n");
			return 0;
	}

	if( member_array(name, banned_sword_name)!=-1 ) {
		write("对不起，这种外号会造成其他人的困扰。\n");
		return 0;
	}
	return 1;
}

int main(object me, string str)
{
	object ob;
	mapping mp;
	string art, artname;
	int i;
	
	if ( me->is_busy() || me->is_fighting() )
		return notify_fail("你正忙着呢，等闲下来再说吧。\n");	
	
	if ( base_name(environment(me)) != "/d/city/jiulou2") 
		return notify_fail("你必须到江湖人聚集的地方才能提议别人的外号，比如：扬州瘦西湖雅楼。\n");

	if ( !str ) 
		return notify_fail("想好了要为谁提议个响亮的外号没有？格式： artname <id> <外号> \n");	
	
	if (sscanf(str, "%s %s", str, art) != 2 )
	 	return notify_fail("想好了要为谁提议个响亮的外号没有？格式： artname <id> <外号> \n");
	
	if ( str == "-clear" && wizardp(me) ){
		if( art == "all" )
			return notify_fail(clean_all());
		else 	return notify_fail(clean_name(art));
	}
	
		
	if ( str == me->query("id") && !wizardp(me) )
		return notify_fail("给自己起？江湖可不是办家家，得别人认同才行！\n");	
	
	if ( me->query("combat_exp") < 100000 )
		return notify_fail("你在江湖里的名声还不够响亮，怕没资格给别人起外号吧？\n");	
				
	if( !check_legal_name(art) ) 
		return notify_fail(HIW"这个提议不好，你再想过。\n"NOR);
	
	ob = LOGIN_D->find_body(str);
	if ( !ob ) ob = find_living(str);
	
	if( !mapp(mp = artnames[str] ) ){
		if(sizeof(keys(artnames)) >= LIMIT ) 
			return notify_fail("对不起，目前只能允许同时提议十个人的新外号。\n"NOR);
		if ( !ob || !me->visible(ob)) 
			return notify_fail(YEL"\n你要提议的人目前不在游戏中，等此人进入游戏再说吧。\n\n"NOR);
		if ( stringp(artname = ob->query("artname")) 
		&& artname != "" 
		&& ob->query("artname_time")
		&& time()*60 > ob->query("artname_time") ) // 十年才能换一次。
			return notify_fail("此人已经有外号啦，叫做“"+artname+"”！要提议新外号，得等十年一次的机会。\n");	
		
		if ( art == ob->query("name") )
			return notify_fail("这个外号和此人的名字相同耶，想个别的？\n");	

		artnames[str] = ([ art : ({ me->query("id"),query_ip_number(me)  })  ]);
		//artnames[str] = ([ art : ([ me->query("id"): ])  ]);
	}
	
	else {
		if ( sizeof(keys(artnames[str])) >= 9 )
			return notify_fail(YEL"\n对不起，最多可提议九个外号，请在已有的九个里选择。\n\n"NOR);
		if( artnames[str][art] && member_array(me->query("id"), artnames[str][art] )!=-1 ) 
			return notify_fail(YEL"\n对不起，最多可以给一个外号投一次票。\n\n"NOR);
		if( artnames[str][art] && member_array(query_ip_number(me), artnames[str][art] )!=-1 ) 
			return notify_fail(YEL"\n对不起，同一连线地址最多可以给一个外号投一次票。\n\n"NOR);
		if( !artnames[str][art] )
			artnames[str] += ([ art : ({ me->query("id"),query_ip_number(me) }) ]);
		else 	artnames[str][art] += ({ me->query("id"),query_ip_number(me) });
	}
	
	i = sizeof(artnames[str][art]);
	message_vision(MAG"\n$N走上前去，大笔在卷上一挥：「我赞同把“"+art+"”这个外号给 "+capitalize(str)+"！」\n"NOR, me);
	i /=2;	
	write("\n你在卷帛上多记了一道，这个外号目前共有 "+to_chinese(i)+" 人赞成。\n");
	if( i > 30 && ob )
	//if( i > 1 && ob )
	{
		call_out("give_artname", 1, ob, art );
		map_delete(artnames, str);
	}
	
	return 1;
}

void give_artname(object ob, string art)
{
	if ( !ob || !stringp(art) ) return;
	//ob->set("artname", art);
	//ob->set("artname_time", time()*60 + 31536000 );  // 十年。
	//ob->save();
	BBS_D->add_Bbs_Up_Map(WEB_DB_NAME, "UPDATE members SET title = '"+strip(art)+"' WHERE username = '"+ob->query("id")+"@"+lower_case(INTERMUD_MUD_NAME)+"'");
	//message("wizard:linux", "UPDATE members SET title = '"+strip(art)+"' WHERE username = '"+ob->query("id")+"@"+lower_case(INTERMUD_MUD_NAME)+"'", users());
	message("chat", HIY +"\n【江湖传言】：经江湖同道提议通过，"+ob->name(1)+"在书剑论坛("+HIW+U+"http://bbs.mymud.com"+NOR+HIY+")获得新的外号："+HIG+art+HIY+"！\n"NOR, users());
}
