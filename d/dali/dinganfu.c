// By River 98/12/
// LinuX@SJ 2004/03/09

#include <ansi.h>
inherit ROOM;

mapping valid_special = ([
        "death"  	:       1800,
        "per"		:	3800,
        "recover"	:	4000,
]);
mapping special_to_chinese = ([
        "death"  	:       "一次"HIY"有效死亡"NOR,
        "per"  		:       "一点"HIG"先天容貌"NOR,
        "recover"	:	"恢复一次"HIB"死亡损失"NOR,
]);

int Change_credit(int credit, int chg, string arg);

void create()
{
        set("short", "定安府");
	set("long", @LONG
这是大理的司法机关，接管的主要是一些民族之间的纠纷和误解，因此府
里有汉族，白族，摆夷族，哈尼族，壮族官员各一名，以示公正。另外，府里
还雇了通译，以便讯问。近年来事态安和，定安府门前清清淡淡。
LONG
	);
	//，不过最近因为书剑积分的流行，这里也变得热闹起来，门前立着一块很大的标牌(sign)
	set("exits",([ 
             "north" : __DIR__"nanjie2",
        ]));
	set("no_fight",1);
        set("objects", ([
                __DIR__"npc/yayi" : 2,
        ]));
    
	set("coor/x",-320);
  	set("coor/y",-470);
   	set("coor/z",-20);
   	setup();
}

/*void init()
{
    add_action( "do_exchange", "exchange" );
    add_action( "do_exchange", "duihuan" );
    add_action( "do_look", "look" );
}
*/
// 获取积分
int GetMyCredit()
{
	object me = this_player();
	mixed ret = dbquery("SELECT U_Credit FROM Users WHERE U_Username=\""+me->query("id")+"\"");

	if (!ret)
	{
		write("数据库连接失败。\n");
		return 0;
	}

	if (sizeof(ret) != 1)
	{
		write("数据库数据有问题，请与管理人员联系。\n");
		return 0;
	}

	return ret[0][0];

}

int do_look(string arg)
{
        object me = this_player();
        string *p;
        int i;
        if( !arg || arg=="" ) return 0;
        
        if( (int)me->is_busy() || me->is_fighting()) 
        	return notify_fail("你正忙着呢。\n");
        	
        if (arg == "sign" ){   
	        p = sort_array(keys(valid_special), 1);
	        write(CHINESE_D->get_title("大理定安王府告示"));
	        write("当前可转换特殊内容如下：("HIG"将不定期添加新的特殊内容"NOR")\n");
	        write("┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉\n");
	        write(sprintf("   %-15s：  %-20s  %-12s\n", "特殊内容ID","特殊内容名字", "需要积分"));
	        write("┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉\n");
	        for (i=0;i<sizeof(p);i++) {
	                write(sprintf("   %-15s：  %-20s  %-12s\n", p[i],special_to_chinese[p[i]], chinese_number(valid_special[p[i]])+"点"));
	        }
	        write("┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉┉\n");
	        write("输入(exchange 特殊内容ID) 进行转换\n\n");
               	return 1;
        }     
        return 0;
}

int do_exchange(string arg)
{
        int credit;
  	object me = this_player();
  	      
        if( me != this_player(1) ) return 0;
        if (me->is_busy())
		return notify_fail("你正忙着呢。\n");

        credit = GetMyCredit();
        if (!arg || arg=="") {
                tell_object(me,"您目前可用书剑积分为："+chinese_number(credit)+"点，累计使用书剑积分为："+ chinese_number(me->query("credit")) +"点。\n");
                return 1;
        }
        
        switch( arg )
    	{
    			case "death":
		        if ( credit > valid_special[arg] )
		        {
		            if (Change_credit(credit, valid_special[arg], arg)){
		            me->add( "death_times", 1 );
		            me->add( "death_count", 1 );
		            return 1;
		            }
		        }
	        	break; 
	        	
	            	case "per":
		        if ( credit > valid_special[arg] )
		        {
		            if ( me->query( "per" ) > 29 ){
	                	tell_object(me,"你已经足够美了。\n" );
	                	return 1;
                	    }	
		            if (Change_credit(credit, valid_special[arg], arg)){
			            me->add( "per", 1 );
			            return 1;
		            }
		        }
	        	break; 
	        	
	        	case "recover":
		        if ( credit > valid_special[arg] )
		        {
		            if (Change_credit(credit, valid_special[arg], arg)){
			            "/cmds/wiz/recover"->do_recover(me, 1);
			            return 1;
		            }
		        }
	        	break; 
        default:                 
        tell_object(me,"你要换什么？\n" );
        return 1;
   	}
    	tell_object(me,"你的书剑积分不够，你当前的积分是：" + chinese_number(credit) + "点，兑换"+special_to_chinese[arg]+"需要"+chinese_number(valid_special[arg])+"点。\n" );
    	return 1;
}
int Change_credit(int credit, int chg, string arg)
{
	mixed ret;
	object me = this_player();
	
	ret = dbquery("UPDATE Users SET U_Credit=U_Credit-"+chg+" WHERE U_Username=\""+me->query("id")+"\"");
	if (!ret)
	{
		write("数据库连接失败。\n");
		return 0;
	}

	me->add("credit",chg); //已经消费的书剑积分
	me->start_busy(1+random(2));
	log_file("static/EXCHANGE", 
		sprintf("%s(%s) use %d credit(%d/%d) to exchange %s, ", 
		 me->name(1), geteuid(me), chg, credit, me->query("credit"), arg));
		 
	tell_object(me,"你将" + chinese_number(valid_special[arg]) + "点书剑积分兑换为"+special_to_chinese[arg]+"。\n");
	tell_object(me,"您目前剩余的书剑积分为："
			+ chinese_number(credit)+"点，累计使用书剑积分为：" 
			+ chinese_number(me->query("credit")) +"点。\n");
	return 1;

}