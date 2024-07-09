//by Ciwei@SJ 2004-2-20

#include <ansi.h>
#include <room.h>
#define MISCD "/adm/daemons/miscd"

#define SAVE_VALUE 0.01
inherit ROOM;

int do_show(object me);
int get_value(object me,mapping list);
int get_value2(float savetime,float value);
string money_str2(int v);

void init()
{
	object me = this_player();
	add_action("do_save",	"baocun");
	add_action("do_save",	"cun");
	add_action("do_get",	"tiqu");
	add_action("do_get",	"qu");
	add_action("do_list",	"dlist");
	add_action("do_delete",	"delete");
	add_action("do_delete",	"shanchu");
	if(!query("item_desc/sign"))
	set("item_desc/sign",
        	"
                    书剑个人储物箱 全国联号 使用说明
        ============================================================
                             命 令 说 明
        ------------------------------------------------------------
                       显示物品：dlist
                       保存物品：baocun/cun     [物品名]
                       提取物品：tiqu/qu        [物品名]
                       删除物品：delete/shanchu [物品名]

        ============================================================
        	\n",
        );
	if (userp(me) && getuid(me)=="ciwei" && me->query("env/test")==1 || getuid(me)=="linux")
	add_action("do_debug", "dd");

}
int do_delete(string arg)
{
	object me = this_player();
	object ob;
	int num = 1;
	string id1,id2,id;
	mapping list;
	int *v,i;
	int f = 1;	
	
	if(me->is_busy() || me->is_fighting()) return notify_fail("你正忙着呢！\n");
	if(!arg) return do_show(me);
	if(sscanf(arg,"%s %s %d",id1,id2,num)==3) id = sprintf("%s %s",id1,id2);
	else if(sscanf(arg,"%s %d",id1,num)==2) id = id1;
	else id = arg;//Ps: 应该没有什么东西id 的格式超过 三格吧？hehe
	list = MISCD->query_show_list(me,1);
	notify_fail("你并没有保存该物品。\n");
	v = keys(list);
	if(sizeof(v)==0)
		return 0;	
	for(i=0;i<sizeof(v);i++)
	{
		if( strip(list[v[i]]["name"])==id || list[v[i]]["id"]==id )
		{
			if(f==num) {				
				ob = MISCD->load_this_object(me,list[v[i]]["save_id"],v[i]);	
				if(objectp(ob)) {				
				message_vision(CYN"$N将$n"+CYN"从自己的个人储物箱里删除了。\n"NOR,me,ob);
				destruct(ob);
				me->start_busy(1+random(2));//防止bug
				}
				else {return 0;}
				return 1;
			}
			else f++;
		}
	}
	return 0;
}


int do_list(string arg)
{
	object me = this_player();		
	return do_show(me);
}

int do_save(string arg)
{	
	object obj;
	object me = this_player();
	mapping list;
	
	if(me->is_busy() || me->is_fighting()) return notify_fail("你正忙着！\n");
	if(!arg) return notify_fail("你要保存什么东西？\n");
	if(!objectp(obj = present(arg, me))) return notify_fail("你身上没有这样东西。\n");
	
	if(me->query_condition("killer")) return notify_fail("你正在被官府通缉，小店可不敢保存你的物品。\n");
	
	notify_fail(obj->name()+"不可以被保存。\n");
	
	if(!clonep(obj)
	||obj->query("unique")
	||obj->query("no_get")
	||obj->query("no_give")
	||obj->query("no_drop")
	||obj->query("forg_id")
	||obj->query("quest")///////////
	||obj->is_corpse()
	||obj->query("food_remaining")
	||obj->query("liquid")
	||obj->query("material")=="meat"//肉？
	||obj->query("money_id")
	||strsrch(file_name(obj),"/d/city/weapon/")==0
	||strsrch(file_name(obj),"/clone/weapon/user_weapon_sn")==0
	||obj->is_character()
	||(stringp(obj->query("owner")) && obj->query("owner")!="0")
	||obj->query("credit")
	//||obj->query("")
	//||obj->query("")
	//||obj->query("")
	) return 0;	
	//虽然可以保存任何物品 不过总得有些限制...
	
	if(obj->query("equipped")) return notify_fail("你还是先把"+obj->name()+"拿下来吧。\n");
	
	list = MISCD->query_show_list(me,1);
	if(mapp(list) && sizeof(keys(list))>=MISCD->get_save_num(me)){
		if(me->query("registered")<3) write(HIG"申请贵宾用户将享受更高保存数量。\n"NOR);
		return notify_fail("你的个人储物箱已满了。\n");
	}
        if (MISCD->save_this_object(me,obj)) {
        	message_vision(CYN"$N从身上拿出一"+obj->query("unit")+"$n"+CYN"，放入自己的个人储物箱。\n"NOR,me,obj);
        	obj->move(this_object());
        	destruct(obj);
        	me->start_busy(1+random(2));//防止bug
        	return 1;
        }
        //log
        return 0;
}
int do_get(string arg)
{
	object me = this_player();
	object ob;
	int num = 1;
	string id1,id2,id;
	mapping list;	
	int *v,i;
	int f = 1;
	int value;
	
	if(me->is_busy() || me->is_fighting()) return notify_fail("你正忙着！\n");
	if(!arg) return do_show(me);
	if(sscanf(arg,"%s %s %d",id1,id2,num)==3) id = sprintf("%s %s",id1,id2);
	else if(sscanf(arg,"%s %d",id1,num)==2) id = id1;
	else id = arg;//Ps: 应该没有什么东西id 的格式超过 三格吧？hehe
	list = MISCD->query_show_list(me,1);	
	if(!mapp(list)) return notify_fail("你没有保存任何物品。\n");
	notify_fail("你并没有保存该物品。\n");
	v = keys(list);
	if(sizeof(v)==0)
		return 0;	
	for(i=0;i<sizeof(v);i++)
	{
		if( strip(list[v[i]]["name"])==id || list[v[i]]["id"]==id )
		{
			if(f==num) {				
				value = get_value(me,list[v[i]] );				
			//	if(!me->query("balance")|| value >me->query("balance"))
			//		return notify_fail("你的存款不够支付该物品的保管费。\n");
				ob = MISCD->load_this_object(me,list[v[i]]["save_id"],v[i]);	
				if(objectp(ob)) {
		//		me->add("balance",-1*value);
				message_vision(CYN"$N把$n"+CYN+"从个人储物箱中提取出来。\n"NOR,me,ob);
		//		tell_object(me,"你的存款中扣除了"+MONEY_D->money_str(value)+"来支付保管费。\n"NOR);
				ob->move(me);
				me->start_busy(1+random(2));//防止bug
				
				//一些bug物品的清除
				if(strsrch(base_name(ob),"/cmds")==0 )
				{
					message_vision("$N一个不小心把"+ob->name()+"弄丢了。\n",me);
					destruct(ob);
				}
								
				}
				else {return 0;}
				return 1;
			}
			else f++;
		}
	}
	return 0;
}
int do_show(object me)
{
	mapping list;
	int i;
	int *v;
	int savetime;
		
	list = MISCD->query_show_list(me,1);
	v = keys(list);
	if(sizeof(v)<=0){
		write("你没有保存任何物品。\n");
		return 1;
	}
	
	write(WHT"·你的个人储物箱保存有"+HIB+chinese_number(sizeof(v))+WHT+"件物品·\n"NOR);
	write("┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓\n");
	write("┃       ID             货  物                保存时间        ┃\n");
	/////////12345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890	
	write("┣━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫\n");
	for(i=0;i<sizeof(v);i++)
	{		
		sscanf(list[v[i]]["save_id"],"%*s%ds",savetime);
		write(sprintf("┃%|16s%|18s%|26s┃\n",
		list[v[i]]["id"],
		list[v[i]]["name"],
		chinese_number((time()-savetime)/86400)+"天",
		));
	}
	write("┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛\n");
	write(WHT"·你的个人储物箱一共可以保存("+HIY+chinese_number(MISCD->get_save_num(me))+WHT+"件物品)·\n"NOR);
	return 1;
}
string money_str2(int v)
{
	string re;
	
	if(!v || v==0) return HIW"一文不名"NOR;
	if(!intp(v)) return "";
	re = MONEY_D->money_str(v);
	while(strwidth(strip(re))>24)
	{
		if(strsrch(re,"铜钱")!=-1){
			if(strsrch(re,"白银")!=-1){
				sscanf(re,"%s白银%*s",re);
				re = re+"白银";
				continue;
			}
			else{
				if(strsrch(re,"黄金")!=-1){
					sscanf(re,"%s黄金%*s",re);
					re = re+"黄金";
					continue;
				}
				else break;
			}
		}
		else if(strsrch(re,"白银")!=-1){
			if(strsrch(re,"黄金")!=-1){
				sscanf(re,"%s黄金%*s",re);
				re = re+"黄金";
				continue;
			}
			else break;
		}
		else break;
	}
	return re+NOR;
}

int get_value(object me,mapping list)
{
//	float savetime;
//	float value;
//	int back;
//	
//	if(!mapp(list)) return 0;
//	sscanf(list["save_id"],"%*s%ds",savetime);
//	value = list["value"];
//	if(!value || value==0) value = 1;
//	if( value < 10000) value = 10000;
//	savetime = (time()-savetime);
//	if(me->query("registered")>2) savetime = savetime / 2;
//	if(me->query("registered")>3) savetime = 1;
//	if(!savetime ||savetime==0) savetime = 1;
//	back = get_value2(savetime,value);
//	if(back<0){
//		MISCD->delete_user_item(me,list["save_id"],"保管费用远大于货品本身价值");
//		return 99999999999;
//	} //防止利用bug hehe
//	back = (back+10000)/100
	return 100;
}

int get_value2(float savetime,float value)
{
	float back;
	value = value /10000;
	back = savetime*value;
	back = back*SAVE_VALUE;	
	return to_int(back);
}

int do_debug(string arg)
{
	//MISCD->debuging();
	float i,j;
	if (sscanf(arg,"%f %f",i,j)==2)
		write(get_value2(i,j)+"\n");
	return 1;
}