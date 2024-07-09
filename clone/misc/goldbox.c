// box.c 功德箱

#include <ansi.h>
#define PAY_TIME 259200

inherit ITEM;
inherit F_SAVE;

void create()
{
	set_name(HIW "回收站" NOR, ({ "huishou zhan", "zhan", "box" }) );
	set_weight(999999);
	set_max_encumbrance(9999999);
	//if( clonep() )
	//	set_default_object(__FILE__);
	//else {
		set("unit", "个");                
		set("value", 1000);
		set("material", "wood");
		set("no_get",1);
		set("no_drop",1);
		set("amount",30);
	//}
	set("no_start",0);
	restore();
	set("long", "由于有玩家利用BUG非法复制金钱，这是系统为玩家提供赔偿机会而设置的一个回收站。\n玩家可以通过检查(jiancha)来查看哪些人用了bug，并监督此次收回非法金钱的过程。\n利用过BUG的玩家请在这里put补偿所欠的金钱。\n");
	setup();
}

int is_container() { return 1; }

void init()
{
/*
	if( me->query("combat_exp") <= 5 
	&&  this_object()->query("amount") >=10 
	&&  random(2) == 1 )
	{
		ob=new("/clone/money/silver");
		ob->set_amount(10);
		ob->move(this_object());
		tell_object(me, HIW "\n你忽然看到功德箱里有什么东西在闪闪发光！\n\n" NOR);
		me->add("combat_exp", 1);
	}
*/

	add_action("do_put", "put");
	add_action("do_check",({"check","jiancha"}));
}

int to_num(string str)
{
	string* get;
	int temp;
	int ret=0;
	
	if(!str || strlen(str)<2 ) return 0;
	if(str[0..0]=="十") str = replace_string(str,"十","10|");
	else str = replace_string(str,"十","0|");
		
	str = replace_string(str,"零","");
	str = replace_string(str,"万","0000|");
	str = replace_string(str,"千","000|");
	str = replace_string(str,"百","00|");
	//	
	str = replace_string(str,"九","9");
	str = replace_string(str,"八","8");
	str = replace_string(str,"七","7");
	str = replace_string(str,"六","6");
	str = replace_string(str,"五","5");
	str = replace_string(str,"四","4");
	str = replace_string(str,"三","3");
	str = replace_string(str,"二","2");
	str = replace_string(str,"一","1");
	
	get = explode(str,"|");
	for(int i=0;i<sizeof(get);i++)
	{
		if(!get[i] || get[i]=="") continue;
		if(sscanf(get[i],"%d",temp)!=1) continue;
		ret += temp;
	}
	return ret;
}

string query_save_file()
{
	return "/data/money";
}

string get_msg(int t)
{
	if(!t) return "无";
	return sprintf("%s锭"HIY"黄金"NOR,chinese_number(t) );
}

int do_check(string arg)
{
	//if(!arg) 返回现在补偿结果
	int line,coin;
	string msg,name,id;
	string* ids;
	mapping gold=([]);
	object me = this_player();
	if(arg=="start" && wizardp(me) )
	{
		if(query("no_start")==1) return notify_fail("已经提取过数据了。\n");
		line=0;
		while( msg = read_file("/log/static/HOWG",line,1) )
		{
			if(!msg) continue;
			line++;
			coin=0;
			msg = strip(msg);
			if(sscanf(msg,"%*s2004 %s(%s)造了%s",name,id,msg)!=4) continue;
			
			if(SECURITY_D->get_status(id) !="(player)") continue;
			
			if(undefinedp(gold[id])) gold[id]=0;
			
			/// all change to gold
			if(strsrch(msg,"张壹仟两银票")!=-1)
			{
				msg = replace_string(msg,"张壹仟两银票","");
				gold[id] += to_num(msg)*10;
				continue;
			}
			
			if(strsrch(msg,"锭黄金")!=-1)
			{
				msg = replace_string(msg,"锭黄金","");
				gold[id] += to_num(msg);
				continue;
			}
			/*			
			if(strsrch(msg,"两白银")!=-1)
			{
				msg = replace_string(msg,"两白银","");
				gold[id] += to_num(msg)*100;
				continue;
			}
			
			if(strsrch(msg,"文铜钱")!=-1)
			{
				msg = replace_string(msg,"文铜钱","");
				gold[id] += to_num(msg);
				continue;
			}
			*/
		}
		set("no_start",1);
		set("start_time",time() );
		set("gold",copy(gold));
		save();
		write("已经提取数据完毕！\n");
		return 1;
	}
	gold = query("gold");
	if(!gold || !mapp(gold))
		return notify_fail("流失的资金已经全部取回，多谢支持！\n");
	ids = keys(gold);
	if(!ids || !sizeof(ids)) return notify_fail("流失的资金已经全部取回，多谢支持！\n");
	msg = "目前还欠债玩家以及其欠债金钱：\n";
	for(int i=0;i<sizeof(ids);i++)	
	{
		if(gold[ids[i]]==0)
		{
			map_delete(gold,ids[i]);
			continue;
		}
		if(me && getuid(me)==ids[i]) msg += HIR"";		
		msg += sprintf("玩家:%-8s，还欠%s\n",ids[i],get_msg(gold[ids[i]])   );
		//MONEY_D->money_str(gold[ids[i]])
		msg += NOR"";
	}
	msg += "请及时还清所有债务，逾期将根据连线记录对主ID进行严厉处罚。\n";
	
	if(PAY_TIME-(time()-query("start_time")) >0)
		msg += "离最后期限还有"+CHINESE_D->chinese_time(PAY_TIME-(time()-query("start_time")))+"。\n";
	else msg += "管理员正查询连线记录准备对利用过BUG的主ID进行严厉惩罚。\n";
	
	me->start_more(msg);
	return 1;
}

int do_put(string arg)
{
	object me, obj;
	string item, target; 
	string id;
	int amount;
	mapping gold = query("gold");
	
	me = this_player();

	if( !arg || sscanf(arg, "%s in %s", item, target)!=2 
	||  sscanf(item, "%d %s", amount, item)!=2
	||  !objectp(obj = present(item, me)) )
	{
		write("格式：put [数量] [钱] in huishou zhan。\n");
		return 1;
	}
	
	if(member_array(target,parse_command_id_list())==-1)
	{
		write("你要把钱放到哪里？\n");
		return 1;
	}
			
	if(amount<1 ||amount>99999999)
	{
		write("?别在这里找bug\n");
		return 1;
	}
	
	if(!obj->query("money_id"))
	{
		write("这不是钱！\n");
		return 1;
	}
	if(obj->query_amount() < amount)
	{
		write("你身上没有那么多"+obj->name()+"。\n");
		return 1;
	}
	
	if(obj->query("money_id")!="gold" && obj->query("money_id")!="thousand-cash" )
	{
		write("赔偿的单位必须是 黄金 或者 银票。\n");
		return 1;
	}
		
	id = getuid(me);
	
	if(!gold){
		write("冬瓜西瓜！！！！\n");
		return 1;
	}
		
	if(undefinedp(gold[id]))
	{
		write("你并没有欠钱。\n");
		return 1;
	}
	tell_room(environment(me),me->name()+"拿出一些钱放在"+this_object()->name()+"里。\n",({me}));
	tell_object(me,"你拿出"+chinese_number(amount)+obj->query("base_unit")+obj->name()+"放在"+this_object()->name()+"里。\n");
	log_file("static/HOWG_BACK",sprintf("%-8s(-8%s) 补偿系统 %d value",me->name(1), geteuid(me),amount*(int)obj->query("base_value") ),this_object() );
		
	gold[id] -= amount*(int)obj->query("base_value")/10000;//c
	obj->add_amount(-amount);
	
	if(gold[id]==0)
	{
		write("你的帐已经清了，多谢支持。\n");
		map_delete(gold,id);
		save();
		return 1;
	}
	if(gold[id]<0)
	{
		write("太多了，还你。\n");
		MONEY_D->pay_player(me,-gold[id]*10000 );//c
		message_vision("从$N里突然蹦出些钱，掉在$n怀里。\n",this_object(),me);
		map_delete(gold,id);
		save();
		return 1;
	}
	if(gold[id]>0)
	{
		write("nod,你还欠"+get_msg(gold[id])+"，请尽快还清。\n");
		save();
		return 1;		
	}
	write("还有何事？敲ciwei去。\n");
	return 1;
}



























/*
int do_put(string arg)
{
	object me, obj;
	string item, target; 
	int amount;

	me = this_player();

	if( !arg || sscanf(arg, "%s in %s", item, target)!=2 
	||  sscanf(item, "%d %s", amount, item)!=2
	||  !objectp(obj = present(item, me)) )
		return 0;

	if( obj->query("money_id") == "silver" 
	&&  obj->query_amount() >= 5 && amount >= 5 ) 
	{
		if( me->query("begger") > 0) {
			message_vision( sprintf(HIY "$N将一%s%s放进%s。\n" NOR,
				obj->query("unit"), obj->name(), 
				this_object()->name()),me );
			obj->set_amount(obj->query_amount() - amount);;
			me->add("begger", (-1)*(amount/5));
			if( me->query("begger") < 0) me->set("begger", 0);
			this_object()->add("amount", amount);
			return 1;
		}
	}
	return 0;
}
*/