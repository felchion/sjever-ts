// lane@ln 2004.7.13 add party job
#include <ansi.h>
#include <command.h>

inherit NPC;
#include <mudlib.h>

string ask_me();
string ask_help();
string ask_give();
string ask_road();
string ask_lead();
string ask_xin();
string ask_yuebing();
string ask_newbie();
string ask_skill();
string ask_delnewbie();
string ask_art();
string ask_nhz();
string ask_em();

int sort_file(string file)
{
        int i;
        if( file[<2..<1] != ".c" ) return 0;
        if( !sscanf(file, "%*s.c") )  return 0;
        if( sscanf(file, "%*s_noshow.c") )  return 0;
        i = strlen(file);
        while(i--){
                if( file[i] == '.' ) continue;
                if( file[i] < 'a' || file[i] > 'z' )
                        return 0;
        }

        return 1;
}

void create()
{
        int age = 20 + random(20);
        set_name("武馆门卫", ({ "men wei", "menwei", "wei" }));
        set("gender", "男性");
        set("age", age);
        set_skill("unarmed", 80);
        set_skill("dodge", 80);
        set("apply/attack", 30);
        set("apply/defense", 30);
        set("apply/damage", 35);
        set("combat_exp", 150+age*10);
        set("shen_type", 1);

        set("inquiry", ([
                "中秋节"        : (: ask_yuebing :),
                "节日礼物"      : (: ask_yuebing :),
                "中秋"          : (: ask_yuebing :),
                "月饼"          : HIY"自古中秋月最明，凉风届候夜弥清。一天气象沉银汉，四海鱼龙跃水精。"NOR,
                "资助"          : (: ask_help :),
                "捐赠"          : (: ask_give :),
                "引路"          : (: ask_road :),
                "信笺"          : (: ask_xin :),
                "信件"          : (: ask_xin :),
                "信"            : (: ask_xin :),
                "letter"        : (: ask_xin :),
//                "新手"          : (: ask_newbie :),
                "元旦经验"        : (: ask_newbie :),
//                "技能"          : (: ask_skill :),
//                "skill"         : (: ask_skill :),
                "xin"           : (: ask_xin :),		
//                "学艺有成"	    : (: ask_delnewbie :),	
//        				"琴棋书画"			:	(: ask_art :),
//        				"指掌双绝"			:	(: ask_nhz :),
//        				"大乘佛法"			:	(: ask_em :),
        ]));

        setup();
        add_money("silver", 3+age/10);
        carry_object(ARMOR_D("cloth"))->wear();
}

string ask_delnewbie()
{
	  object me = this_player();
	  
	  if (me->query("combat_exp")<100000)
    	return("你的武艺还须精进呀！");
    if (!me->query("shizhe_newbie"))
    	return("你已经很不是新手了！");
    me->delete("shizhe_newbie");
    return("既然你学艺有成，那就不再是一个新手了！");
}
	
string ask_art()
{
    object me = this_player();
    mapping myfam;
    myfam = (mapping)me->query("family");
    
    if (!me->query("shizhe_newbie"))
    	return("对不起，我帮不你了！");
    
    if( !myfam || myfam["family_name"] != "昆仑派" ) {
			return("这种补偿只针对昆仑弟子。");
		}
	 	
	 	if (me->query("menwei_art"))
			return("不是已经给你补偿过了吗?");
			
		me->set("menwei_art",1);
		me->set_skill("art",144);
		return ("技能调整好了！");
}

string ask_em()
{
    object me = this_player();
    mapping myfam;
    myfam = (mapping)me->query("family");
    
    if (!me->query("shizhe_newbie"))
    	return("对不起，我帮不你了！");
    
    if( !myfam || myfam["family_name"] != "峨嵋派" ) {
			return("这种补偿只针对峨嵋弟子。");
		}
	 	
	 	if (me->query("menwei_em"))
			return("不是已经给你补偿过了吗?");
			
		me->set("menwei_em",1);
		me->set_skill("yanxing-daofa",144);
		me->set_skill("dacheng-fofa",144);
		return ("技能调整好了！");
}

string ask_nhz()
{
    object me = this_player();
    mapping myfam;
    myfam = (mapping)me->query("family");
    
    if (!me->query("shizhe_newbie"))
    	return("对不起，我帮不你了！");
    
    if( !myfam || myfam["family_name"] != "少林派" ) {
			return("这种补偿只针对少林弟子。");
		}
	 	
	 	if (me->query("menwei_sl_nhz"))
			return("不是已经给你补偿过了吗?");
			
		me->set("menwei_sl_nhz",1);
		me->set_skill("nianhua-zhi",144);
		me->set_skill("sanhua-zhang",144);
		return ("技能调整好了！");
}

string ask_newbie()
{
    object me = this_player();
 
 	
    if (me->query("gift/last_time")>1293800475 && me->query("gift/last_time")<1293801484)
    {
     	me->set("gift/last_time",time());
     	me->add("combat_exp",20000);
    	return("经验已经调整好了。");
    }    
    else
    	return("我是帮不上你了！");
}
  
string ask_skill()
{
    int i,j,level;
    string *skills;
    mapping skill_status;
    object me = this_player();

    level = me->query("combat_exp");
		for (i = to_int(pow(10.0 * level, 0.3333333));to_int(i*i/10.0*i)<=level;i++);
			level = i;
    
    if (!me->query_temp("menwei_newbie"))
    	return("呵呵，今天天气不错呀！");
   
    me->delete_temp("menwei_newbie");
    if (level>160)
    	return("你已经很厉害了，我是帮不上你了。");
   
		if ( !(skill_status = me->query_skills()) || !sizeof(skill_status) )
				return ("你什么都不会，让我怎么帮你。");
		
		skills  = keys(skill_status);
		j = sizeof(skill_status);
		
		for(i=0; i<j; i++) {
				me->set_skill(skills[i], level);
		}
		me->set_skill("medicine",level);
		return ("技能调整好了！");
		
}

string ask_yuebing()
{
        object obj,me = this_player();
		int i;
		if(time()<1096300800 && !wizardp(me))
		{
			command("ah");
			command("say 这么早就中秋了吗……还需要至少"+CHINESE_D->chinese_time(1096300800-time())+"的时间段啊。");
			command("say ");
			me->start_busy(1);
			return "不过要是过中秋，可别忘记给老爸老妈送个中秋问候啊……，别就知道找女孩子……";
		}
		if(time()>1097164800&& !wizardp(me))
		{
			command("?");
			command("say");
			command("pat "+me->query("id"));
			me->start_busy(1);
			return "中秋好像已经过去了吧？……";
		}

        if( (me->query("quest/gift/mid-autumn/yuebing")>=8 && me->query("registered")<3)
			||(me->query("quest/gift/mid-autumn/yuebing")>=15 && me->query("registered")>=3))
		{
			command("pat "+me->query("id"));
			return RANK_D->query_respect(me) +"的月饼今年供应结束了！";
		}

		if (me->query("mud_age") < (time() - me->query("birthday"))/20 && me->query("registered")<3) {
			if (me->query("online_total") < 3600) {
				CHANNEL_D->do_channel(this_object(), "rumor", me->query("name")+"("+capitalize(me->query("id"))+")想骗取礼物的企图被系统识破了。");
				me->set("quest/gift/mid-autumn/yuebing", 8);//直接结束算了
				tell_object(me,HIR"你失去了获得礼物的权利。\n"NOR);
			}
			command("pat "+me->query("id"));
			return "好好努力哦~~~~~~下次给你一个大大的月饼~~~~~~~~";
		}
		/*
		普通玩家，只能要8个，贵宾15个。
		月饼不能给别人用。
		*/

        if(me->query("registered")<3)
			i=8-me->query("quest/gift/mid-autumn/yuebing");
		else i=15-me->query("quest/gift/mid-autumn/yuebing");
		if(present("xue yuebing",me))
		{
			command("pat "+me->query("id"));
			command("say "+HIC"你身上有一个啊。你这个要是用完了，你还有"+chinese_number(i)+"次机会向我询问。"NOR);
			command("say 月饼最好不好直接食用，你可以尝试去大智岛，说不定有什么好处......");
			command("hehe "+me->query("id"));
			return HIR"另外，过中秋啊，可别忘记给老爸老妈送个中秋问候啊……，别就知道找女孩子……"NOR;
		}

		obj=new("/clone/food/yuebing");
		obj->set_name(WHT"雪月饼"NOR, ({ "xue yuebing", "yue bing", "yuebing" }));
		obj->set("no_eat",1);//没用的，嘿嘿
		obj->set("no_sell",1);
		obj->set("no_give",1);
		obj->set("no_drop",1);
		obj->set("no_get",1);
		obj->set("festival","mid-autumn");
		obj->set("owner",me->query("id"));
        obj->set("long","中国人在中秋节吃月饼是一个传统习惯，美味的月饼烤制精细，甜而不腻。\n"
			         +"\n但这个月饼有些奇怪，虽然形状、大小与其它无异，但是上面有八个小字：\n"
			           +HIR"                    〖书剑出品，大智通用〗\n"NOR);
        command("smile "+me->query("id"));
        command("say "+HIY"自古中秋月最明，凉风届候夜弥清。一天气象沉银汉，四海鱼龙跃水精。"NOR);
        command("pat "+me->query("id"));
        command("say 大半年的闯荡江湖，你也辛苦啦，这个月饼是与大智岛联合生产的雪月饼，也许有用……");
		obj->move(me);
		message_vision("$N交给$n一个"NOR+WHT"雪月饼"NOR+"。\n" NOR, this_object(), me );
		me->add("quest/gift/mid-autumn/yuebing",1);
		if(me->query("registered")<3)
			log_file("quest/yuebing", sprintf("%s(%s)领到第%d个月饼。非VIP。\n", me->name(1),me->query("id"),me->query("quest/gift/mid-autumn/yuebing")) );
		else
			log_file("quest/yuebing", sprintf("%s(%s)领到第%d个月饼。VIP。\n", me->name(1),me->query("id"),me->query("quest/gift/mid-autumn/yuebing")) );

        return HIR"不过过中秋啊，可别忘记给老爸老妈送个中秋问候啊……，别就知道找女孩子……"NOR;
}

string ask_road()
{
        object me = this_player();

        if( me->query("combat_exp") > 30000 ) {
		return RANK_D->query_respect(me)+ "的武功已经不低，引路一事我想就不用提起了！";
        }

        if( me->query("combat_exp") < 1000 ) {
                return RANK_D->query_respect(me)+"的武功太差了，还是等练高一点再出门吧！";
        }

        if( me->query_temp( "wg_help" ))
                return "你不是正在找人资助吗？";

        if( me->query_temp( "wg_road" ) == 1)
                return "你不是已经问过了吗？" ;

        if( me->query_temp( "wg_road" ) == 2)
                return "你不是正找人带你去" + me->query_temp( "wg_go" ) + "吗？";

        if( me->query_temp( "wg_go"))
                return "你不是正找人带路吗？\n";
        
        if( me->query( "wg_help_exp" ) && me->query( "wg_help_exp" ) 
                - me->query( "combat_exp" ) > - 2000 )
                return RANK_D->query_respect( me ) 
                + "为人应当自立自强，怎么可以老是找人帮忙呢？";
        
        me->set_temp("wg_road", 1);
        
        return "不知" + RANK_D->query_respect(me)+ "要去哪里("HIY"qu <门派名>"CYN")？";
}

int do_go(string str)
{
        object me = this_player();

        if( me->query_temp("wg_road") != 1 )
                return 0;

        if( !str || str == "" ) {
                write( "你要去哪里？\n" );
                return 1;
        }

        message_vision( sprintf( CYN "$N对武馆门卫躬身施了个礼：" + RANK_D->query_self(me)
                + "意欲前去" + str + "，还请这位" + RANK_D->query_respect( this_object() )
                + "帮忙。\n\n" NOR ), me );

        if( str == "少林" || str == "武当" || str == "峨眉" || str == "明教"
                || str == "星宿" || str == "华山" || str == "丐帮" || str == "慕容"
                || str == "大理" || str == "归云庄" || str == "大轮寺" || str == "神龙岛") {
                message_vision( sprintf( CYN "武馆门卫：" + RANK_D->query_respect(me) + 
                        "耐心等待片刻，相信定会有人仗义相助。\n\n" NOR), me);
                message( "channel:chat", HIC "【闲聊】武馆门卫(Men Wei)：" + me->query("name") +
                        "现在需要找人引路前去" + str + "，不知哪位好汉愿意帮忙？\n" NOR, users() );
                call_out( "remove_effect", 180, me );
                me->set_temp( "wg_road", 2 );
                me->set_temp( "wg_go", str );
                return 1;
        }
        else if( str == "古墓" )
        {
                command( "say 古墓内机关密布，外人不得擅入，" + RANK_D->query_respect( me ) 
                        + "还是自己前去的好？\n" );
                return 1;
        }
        else if( str == "襄阳" )
        {
                command( "laugh " + me->query("id") );
                command( "say 你现在不就是在襄阳吗？" );
                command( "fat " + me->query("id"));
                return 1;
        }
        else {
                command( "say  我只能介绍你前去少林、武当、峨眉、明教、星宿、华山、"
                        + "古墓、丐帮、慕容、大理、归云庄、大轮寺、神龙岛。\n" );
                return 1;
        }

}

void remove_lead( object me, object ob2 )
{
        if( !ob2 )
                return;

        if( ob2->query_temp( "wg_go" ) )
        {
                message_vision( HIY"突然从旁边钻出一个武馆弟子，对$N说到："
                        + RANK_D->query_respect( ob2 ) + "怎么还没有到" + ob2->query_temp("wg_go" )
                        + "，我看还是回去另外找个人帮忙吧。\n"NOR, ob2);
                if( me )
                        message_vision( HIY"突然从旁边钻出一个武馆弟子，对着$N哼哼到：你怎么比猪还"
                        + "慢啊，要么大伙儿拱你算了。\n"NOR, me);
                ob2->delete_temp( "wg_go" );
        }
        return;
}

string ask_give()
{
        object me = this_player();

	if( me->query_temp( "wg_help" ) )
        {
                command("kick " + me->query("id"));             
                return "既要资助，还给别人捐赠？" ;
        }

        if( me->query("combat_exp") < 100000 )
        {
                command("smile " + me->query("id"));
                return  RANK_D->query_respect(me) + "也正在成长阶段，还是留着自己用吧。";
        }
        
        me->set_temp("wg_juanzeng",1);
        return RANK_D->query_respect(me) + "既有此心，便请捐赠(juanzeng)黄金十两，如何？";
}

string ask_help()
{
        object me = this_player();

        if( me->query("combat_exp")>30000 )
        {
                return RANK_D->query_respect(me) + 
                        "的武功已经不低，还是靠自己的双手去挣钱吧！";
        }
        if( me->query("combat_exp")<1000 )
        {
                return RANK_D->query_respect(me) + 
                        "刚来没几天，估计不会有人愿意资助你，还是过几天再来吧！";
        }
        if( me->query_temp( "wg_road" ) || me->query_temp("wg_go" ))
                return "你不是正在找人带路吗？";

        if( me->query_temp( "wg_help" ))
                return "你不是已经问过了吗？" ;

        if( me->query( "wg_help_exp" ) && me->query( "wg_help_exp" ) 
                - me->query( "combat_exp" ) > - 2000 )
                return RANK_D->query_respect( me ) 
                + "为人应当自立自强，怎么可以老是找人帮忙呢？";

        me->set_temp("wg_help", 1);
        call_out( "remove_effect", 180, me );
        message( "channel:chat", HIC"【闲聊】武馆门卫(Men Wei)：" + me->query("name") +
                "现在需要资助，不知哪位好汉愿意慷慨解囊？\n" NOR, users() );
        return RANK_D->query_respect(me) + 
                "耐心等待片刻，相信定会有人仗义相助。";
}

void remove_effect( object me )
{
        if( !me || environment(me) != environment() ) 
                return;

        if( me->query_temp( "wg_help" ) )
        {
                me->delete_temp("wg_help");
                message_vision( sprintf(HIR "武馆门卫失望地对$N说到：" + RANK_D->query_respect(me)
                        + "，看来是不会有人来帮忙了，还是靠自己努力吧。\n" NOR), me );
        }

        if( me->query_temp( "wg_road" ) == 2 )
        {
                me->delete_temp("wg_road");
                me->delete_temp("wg_go");
                message_vision( sprintf(HIR "武馆门卫失望地对$N说到：" + RANK_D->query_respect(me)
                        + "，看来是不会有人来帮忙了，还是靠自己努力吧。\n" NOR), me );
        }
        return;
}

void init()
{
        object me, ob, target, *list;
        int i;

        me = this_player();
        ob = this_object();

	::init();

	if( !ob->query_temp("job_party") ) {
		remove_call_out("test_job");
		call_out("test_job", 1, ob);
	} else {
		if( wizardp(me) ) tell_object(me, HIY"现在运行："+ob->query_temp("job_party")+"次。\n"NOR);
		list = filter_array(users(), (:
			   $1->query("combat_exp") >=100000
			&& $1->query("family/family_name")
			&& !wizardp($1) :));
		i = sizeof(list);
		if( i > 0 ) target = list[(random(i))];
		if( i > 150 ) i = 150;
		if( ob->query_temp("job_party") >= (100 - i/3) && i > 1 ) {
			if( wizardp(me) ) tell_object(me, HIR"开始执行。\n"NOR);
			ob->delete_temp("job_party");
			if( target ) {
				if( wizardp(me) ) tell_object(me, YEL"对象："+target->name()+"("+target->query("id")+")\n"NOR);
                                call_out("do_job", 1, target, me);
			}
		}
	}

        if( interactive(ob = this_player()) && !is_fighting() ) 
        {
                remove_call_out("greeting");
                call_out("greeting", 1, ob);
        }

        if( interactive(me) ) 
        {
                add_action( "do_give", "juanzeng" );
                add_action( "do_go", "qu" );
                add_action( "do_lead", "lead" );
        }
        
        return;
}

int do_give( string str )
{
        object me = this_player();
        object ob1,ob2;
        object ob3, ob4;
        object ob5;
        int exp,pot,shen;

        if( !me->query_temp( "wg_juanzeng" ) )
        {
                return 0;
        }

        if (!str || str=="") return notify_fail ("你想捐赠给谁？\n");
        
        if (!ob1 = find_living(str))
                ob1 = LOGIN_D->find_body(str);
        if (ob1 == me) 
                return notify_fail("捐赠给自己？\n");
        if(!objectp(ob2 = present(str, environment(me))) )
                return notify_fail("这里没 " + str + " 这个人。\n");
        if( !living(ob2) ) 
                return notify_fail("你等" + ob1->name() + "醒来再说吧。\n");            

        if (!userp(ob2) ) 
                return notify_fail("你只能捐赠给玩家。\n");

        if( !ob2->query_temp("wg_help"))
                return notify_fail( "对方不需要捐赠。\n" );

        if( query_ip_number(me) == query_ip_number(ob2) )
        {
                command( "say 二位就在一起，直接给钱便是，又何必来烦我呢？\n" );
                command( "admit " + me->query("id") );
                command( "admit " + ob2->query("id" ) );
                return 1;
        }

        ob3 = present( "gold", me );
        if(!ob3)
                return notify_fail( "你身上没有黄金。\n" );
        if( ob3 -> query_amount() < 10 )
                return notify_fail( "你身上没有这么多的黄金。\n" );


        ob3->set_amount( ob3->query_amount() - 10 );
        ob4 = present( "gold", ob2 );
        if( !ob4 )
        {
                ob5 = new( "/clone/money/gold" );
                ob5->set_amount( 10 );
                ob5->move( ob2 );
        }
        else
        {
                ob4->set_amount( ob4->query_amount() + 10 );
        }
     
        
        message_vision( sprintf( "$N给$n一些黄金。\n" ), me, ob1 );
        command( "thank " + me->query("id") );

        exp = 100 + random( 100 ) + random( me->query( "combat_exp" ) / 30000 ) ;
        if( exp > 250 )
                exp = 200 + random( 50 );
        pot = 50 + random( 50 ) + random( me->query( "combat_exp" ) / 60000 ) ;
        shen = 200 + random( 2 * exp );

        write( sprintf( "你得到了" + CHINESE_D->chinese_number( exp ) 
                + "点实战经验和" 
                //+ CHINESE_D->chinese_number( pot )
                //+ "点潜能和" 
                + CHINESE_D->chinese_number( shen ) + "点正神。\n" ));

        me->add( "combat_exp", exp );
        me->add( "shen", shen );
//      me->add( "potential", pot );

        log_file("job/newbie", sprintf("%8s%-10s资助%8s%-10s，得到%d点经验，现经验%d。\n",
		me->query("name"), "("+me->query("id")+")",
                ob1->query("name"), "("+ob1->query("id")+")", exp, me->query("combat_exp")), me);

        me->delete_temp( "wg_juanzeng" );
        ob1->delete_temp( "wg_help" );
        ob1->set( "wg_help_exp", ob1->query("combat_exp") );
        return 1;
}

int do_lead( string str )
{
        object me = this_player();
        object ob1,ob2;

        if (!str || str=="")
                return notify_fail ("你想给谁带路？\n");
        
        if (!ob1 = find_living(str))
                ob1 = LOGIN_D->find_body(str);
        if (ob1 == me) 
                return notify_fail("给自己带路？\n");
        if(!objectp(ob2 = present(str, environment(me))) )
                return notify_fail("这里没 " + str + " 这个人。\n");
        if( !living(ob2) ) 
                return notify_fail("你等" + ob1->name() + "醒来再说吧。\n");            
        if (!userp(ob2) ) 
                return notify_fail("你只能给玩家带路。\n");

        if( query_ip_number(me) == query_ip_number(ob2) )
        {
                command( "say 二位就在一起，直接指路便是，又何必来烦我呢？\n" );
                command( "admit " + me->query("id") );
                command( "admit " + ob2->query("id" ) );
                return 1;
        }

        if( !ob2->query_temp("wg_road"))
                return notify_fail( "对方并没有找人带路。\n" );
        if( ob2->query_temp( "wg_road" ) == 1)
                return notify_fail( "你知道对方要去哪里了吗？\n" );
        
        if( me->query("combat_exp") < 100000 )
        {
                command("smile " + me->query("id"));
                return  notify_fail( RANK_D->query_respect(me)
                        + "纵有此心，但是把人带丢了可不是好玩的。\n" );
        }

        if( me->query_temp( "wg_lead" ) )
                return notify_fail( "你不是已经在给人带路了吗？\n" );
        
        command( "thank " + me->query("id" ));
        
        if( ob2->query_temp( "wg_go" ) == "少林" )
        {
                message_vision( sprintf(HIC "武馆门卫说到：既然如此，就请" 
                        + RANK_D->query_respect( me ) + "将这位" + RANK_D->query_respect( ob2 ) + "带去"
                        + HIY "少林清法比丘"HIC"处。\n" NOR), me );
                me->set_temp("wg_goto", "qingfa biqiu" );
        }
        else if( ob2->query_temp( "wg_go" ) == "武当" )
        {
                message_vision( sprintf(HIC "武馆门卫说到：既然如此，就请" 
                        + RANK_D->query_respect( me ) + "将这位" + RANK_D->query_respect( ob2 ) + "带去"
                        + HIY "武当谷虚道长"HIC"处。\n" NOR), me );
                me->set_temp("wg_goto", "guxu" );
        }
        else if( ob2->query_temp( "wg_go" ) == "峨眉" )
        {
                message_vision( sprintf(HIC "武馆门卫说到：既然如此，就请" 
                        + RANK_D->query_respect( me ) + "将这位" + RANK_D->query_respect( ob2 ) + "带去"
                        + HIY "峨眉静闲师太"HIC"处。\n" NOR), me );
                me->set_temp("wg_goto", "jingxian" );
        }
        else if( ob2->query_temp( "wg_go" ) == "丐帮" )
        {
                message_vision( sprintf(HIC "武馆门卫说到：既然如此，就请" 
                        + RANK_D->query_respect( me ) + "将这位" + RANK_D->query_respect( ob2 ) + "带去"
                        + HIY "丐帮奚长老"HIC"处。\n" NOR), me );
                me->set_temp("wg_goto", "xi zhanglao" );
        }
        else if( ob2->query_temp( "wg_go" ) == "大理" )
        {
                message_vision( sprintf(HIC "武馆门卫说到：既然如此，就请" 
                        + RANK_D->query_respect( me ) + "将这位" + RANK_D->query_respect( ob2 ) + "带去"
                        + HIY "大理高候爷"HIC"处。\n" NOR), me );
                me->set_temp("wg_goto", "gao shengtai" );
        }
        else if( ob2->query_temp( "wg_go" ) == "慕容" )
        {
                message_vision( sprintf(HIC "武馆门卫说到：既然如此，就请" 
                        + RANK_D->query_respect( me ) + "将这位" + RANK_D->query_respect( ob2 ) + "带去"
                        + HIY "姑苏慕容包不同"HIC"处。\n" NOR), me );
                me->set_temp("wg_goto", "bao butong" );
        }
        else if( ob2->query_temp( "wg_go" ) == "归云庄" )
        {
                message_vision( sprintf(HIC "武馆门卫说到：既然如此，就请" 
                        + RANK_D->query_respect( me ) + "将这位" + RANK_D->query_respect( ob2 ) + "带去"
                        + HIY "归云庄陆乘风"HIC"处。\n" NOR), me );
                me->set_temp("wg_goto", "lu chengfeng" );
        }
        else if( ob2->query_temp( "wg_go" ) == "神龙岛" )
        {
                message_vision( sprintf(HIC "武馆门卫说到：既然如此，就请" 
                        + RANK_D->query_respect( me ) + "将这位" + RANK_D->query_respect( ob2 ) + "带去"
                        + HIY "神龙岛张淡月"HIC"处。\n" NOR), me );
                me->set_temp("wg_goto", "zhang danyue" );
        }
        else if( ob2->query_temp( "wg_go" ) == "明教" )
        {
                message_vision( sprintf(HIC "武馆门卫说到：既然如此，就请" 
                        + RANK_D->query_respect( me ) + "将这位" + RANK_D->query_respect( ob2 ) + "带去"
                        + HIY "明教殷无寿"HIC"处。\n" NOR), me );
                me->set_temp("wg_goto", "yin wushou" );
        }
        else if( ob2->query_temp( "wg_go" ) == "星宿" )
        {
                message_vision( sprintf(HIC "武馆门卫说到：既然如此，就请" 
                        + RANK_D->query_respect( me ) + "将这位" + RANK_D->query_respect( ob2 ) + "带去"
                        + HIY "星宿阿紫"HIC"处。\n" NOR), me );
                me->set_temp("wg_goto", "a zi" );
        }
        else if( ob2->query_temp( "wg_go" ) == "华山" )
        {
                message_vision( sprintf(HIC "武馆门卫说到：既然如此，就请" 
                        + RANK_D->query_respect( me ) + "将这位" + RANK_D->query_respect( ob2 ) + "带去"
                        + HIY "华山岳不群掌门"HIC"处。\n" NOR), me );
                me->set_temp("wg_goto", "yue buqun" );
        }
        else if( ob2->query_temp( "wg_go" ) == "大轮寺" )
        {
                message_vision( sprintf(HIC "武馆门卫说到：既然如此，就请" 
                        + RANK_D->query_respect( me ) + "将这位" + RANK_D->query_respect( ob2 ) + "带去"
                        + HIY "大轮寺桑结喇嘛"HIC"处。\n" NOR), me );
                me->set_temp("wg_goto", "sangjie" );
        }

        command( "say 这是我馆主信物，可为凭证。" );
        message_vision( sprintf(HIW "武馆门卫交给$N一块"HIR"武馆令牌"HIW"。\n" NOR), me);
        new("/d/wuguan/obj/lingpai")->move( me );
        me->set_temp("wg_lead",1);
        ob2->delete_temp("wg_road");
        message_vision( HIW "$N决定跟随$n一起行动。\n" NOR, ob2, me );
        ob2->set_leader(me);
        call_out( "remove_lead", 600, me, ob2 );
        return 1;
}

void greeting(object ob)
{
        if( !ob || environment(ob) != environment() ) 
                return;
        if ( ob->query("combat_exp") < 10000)  
        switch( random( 3 ) )
        {
                case 2:
                        command("say 这位" + RANK_D->query_respect(ob)+"，武功这么差，怎么闯江湖呢？\n");
                        break;
        case 1:
                        command( "say " + RANK_D->query_respect( ob )
                                + "如果需要盘缠或者学费，可以通过我请大家帮忙("HIY"ask wei about 资助"CYN")。" );
                        break;
        case 0:
                        command( "say " + RANK_D->query_respect( ob )
                                + "如果不认识路，我可以代你请人引路("HIY"ask wei about 引路"CYN")。" );
                        break;
        }
}

string ask_xin()
{
	object me = this_player();
	object letter;
	//interactive 返回 0
	
	if (me->query("enter_wuguan"))
		return "信？什么信？没听说过！";
	if( me->query("combat_exp") > 3500000 )
        {
                return RANK_D->query_respect(me) + 
                        "的武功已经不低，此事我想就不用提起了！";
        }
        if(mapp(me->query("forging")))
        {
        	return RANK_D->query_respect(me) + 
                        "，我知道了，你想蒙我，是也不是？！";
        }
        if ( me->query("dazao/give") )
        {
        	return "恩，信你不是已经送过去了么？";
        }
        if( present("shu xin", me))
        {
        	return "你身上不正揣着么？";
        }
        if(objectp(letter = new(__DIR__"obj/letter")))
        {
        command("whisper "+getuid(me)+" 嘿嘿，怎么不小心丢了馆主给你的信，小心馆主打你PP!");
        letter->move(me);
        command("say 我这里还有一封，你这就拿去吧，够照顾你的了吧！");
        letter->move(me);
        message_vision("$n给$N一封「"+letter->query("name")+"」。\n",me,  this_object());   
        return "如果你不解冶铁煅造之术，可以使用 help dazao 获得详细信息。"; 
        }
        return 0;                
}

void test_job(object ob)
{
        if( !ob || environment(ob) != environment() ) 
                return;

        ob->add_temp("job_party", 1);
        call_out("test_job", 10, ob);
}

void do_job(object me, object tar)
{
        object *list;
        object target, obj;
        string *file, where, dir;

        object ob = this_object();

        if( !me ) return;
        if( !me->query("family/family_name") ) return;
        if( wizardp(me) ) return;


        if( ob->query_temp("count") > 10 ) {
                ob->delete_temp("count");
                return;
        }

        list = filter_array(users(), (: $1->query("combat_exp") >= 100000 && !wizardp($1) :));

        if( sizeof(list) > 0 ) target = list[(random(sizeof(list)))];

        if( !target ) {
                if( wizardp(tar) ) tell_object(tar, YEL"错误：没指定对象，返回...\n"NOR);
                ob->add_temp("count",1);
                call_out("do_job", 1, me, tar);
                return;
        } else if( wizardp(tar) ) tell_object(tar, RED"检测对象："+target->name()+"("+target->query("id")+")\n"NOR);

        if( target->query("id") == me->query("id") || wizardp(target)
         || target->query("family/family_name") == me->query("family/family_name") ) {
                if( wizardp(tar) ) tell_object(tar, HIY"不符要求：重新检测...\n"NOR);
                ob->add_temp("count",1);
                call_out("do_job", 1, me, tar);
                return;

        } else {
                switch(me->query("family/family_name")){
                	case "峨嵋派": dir = "/d/emei/"; break;
                	case "少林派": dir = "/d/shaolin/"; break;
                	case "天龙寺": dir = "/d/tls/"; break;
                	case "武当派": dir = "/d/wudang/"; break;
                	case "华山派": dir = "/d/huashan/"; break;
                	case "嵩山派": dir = "/d/songshan/"; break;
                	case "姑苏慕容": dir = "/d/mr/yanziwu/"; break;
                	case "明教": dir = "/d/mingjiao/"; break;
                	case "神龙教": dir = "/d/sld/"; break;
                	case "古墓派": dir = "/d/gumu/"; break;
                	case "丐帮": dir = "/d/gb/"; break;
                	case "铁掌帮": dir = "/d/tiezhang/"; break;
                	case "大理": dir = "/d/dali/"; break;
                	case "大轮寺": dir = "/d/xueshan/"; break;
                	case "桃花岛": dir = "/d/thd/"; break;
                	case "昆仑派": dir = "/d/kunlun/"; break;
                	case "星宿派": dir = "/d/xingxiu/"; break;
                	default: return; break;
                }

                file = get_dir(dir);
                if( sizeof(file) > 0 ) file = filter_array(file, (: sort_file :) );
                where = dir + file[random(sizeof(file))];

                if( wizardp(tar) ) tell_object(tar, YEL"地点："+where+YEL"（"+where->query("short")+YEL"）\n"NOR);
                if( wizardp(tar) ) tell_object(tar, YEL"对象："+target->name()+"("+target->query("id")+")\n"NOR);
                obj = new(__DIR__"job_npc");
                obj->do_copy(target);
                obj->move(where);
                obj->set("family/family_name", target->query("family/family_name"));
                obj->set("kill_party", me->query("family/family_name"));

                CHANNEL_D->do_channel(me, "party", "听说有个"+obj->query("title")+"假冒" + target->query("family/family_name") + 
                HIR + target->query("name") + HIW + "(" + target->query("id") + ")" + HIG
                "之名，在" + me->query("family/family_name") + HIR + where->query("short") + HIG + 
                "一带惹事挑衅。\n");
                CHANNEL_D->do_channel(me, "party", HIY"该恶贼自负武功了得，作恶多端，我"HIC + me->query("family/family_name") + HIY"弟子如若遇见，切不可心慈手软。\n"NOR);
        }
}
