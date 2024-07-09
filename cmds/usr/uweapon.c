/*
 * weapon.c
 * 所有关于自制装备的命令。
 * Created by snowman@SJ 01/05/2001
 * Last modified by snowman@SJ 25/06/2001
 * Copyright 1997-2004 ShuJian MUD Wizard Group.
 * All rights reserved. 
 */

#include <ansi.h>
#define NAMED		"/adm/daemons/named"

private void confirm_name_again(string yn, object me, object wp, string name,string id);
private void confirm_color(string yn, object me, object wp, string name,string id);
private void confirm_name(string yn, object me, object wp, string name,string id);

static string *banned_name = ({
// Mud 保留名字
"自己","杀手","闪电","雪人","逍遥","我们","你们","他们","咱们","大家","某人","尸体",
"天神","总管","龙神","仙人","巫师","门客","玩家","书","书剑","名字待定","中神通","蒙面",
"蒙面人","刺客","忍者","镖师","鬼魂","头颅","杀人犯","祈雨","刺猬","爱妻丸","小傻瓜",
// Mud 地名，门派名
"衡山","华山","嵩山","恒山","泰山","雪山","黄山","天山","西域","苗疆","东北","中原",
"南疆","长江","黄河","中国","中华","昆仑山","长白山","白驼山","武当山","峨嵋山",
"冰火岛","桃花岛","黑木崖",
"少林","丐帮","明教","密宗","慕容","古墓","全真","铁掌","大理","白驼","桃花","星宿",
"天龙寺","大轮寺","五毒教","正义","邪恶","五岳剑派","日月神教","官府","武林",
"少林寺","武当派","铁掌帮","普通百姓","峨嵋派","姑苏慕容","古墓派","华山派","嵩山派",
"明教","神龙教","星宿派",
// 重要人物名字
"金庸","邓小平","江泽民","毛泽东","朱榕基","皇帝","总理", "主席",
// 小说人物
"张三丰","张无忌","郭靖","杨过","令狐冲","胡斐","苗人凤","韦小宝","袁承志","胡一刀",
"陈家洛","段誉","萧锋","乔锋","虚竹","东方不败","欧阳锋","洪七公","黄药师","段皇爷",
"一灯大师","王重阳","周伯通","黄蓉","风清扬","独孤求败",
// 脏话
"统一","法轮","法轮研究","法轮大","吃人","他妈的","去你的","操你妈","干你娘","废物",
"混蛋","坏蛋","杀人","抢劫","强奸","妓女","台独","领导","中央","屁眼","屁股","裸体",
"放屁","鸡巴","阴道","婊子","你丫","日你","赤佬","瘪三","傻逼","蠢蛋","操他","我操",
"太监","宦官","阉割","阉人","鸡", "奸淫", "淫荡", "轮奸", "奸", "三级", "政治",
//特殊名字
"淑女","君子",
});

static string* weapon_list = ({ "sword","axe","blade","brush","club","dagger","fork","hammer","hook","spear","staff","stick","xiao","whip" });

int help(object me);
mapping* get_var(object target)
{
	mapping worker;
	if(mapp(worker = target->query("worker")))
	{
		string *ber;
		string var;
		string type;
		mapping* ret= ({});
		mapping temp = allocate_mapping(5);
		ber = keys(worker);
		for(int i=0;i<sizeof(ber);i++)
		{
			if(!mapp(worker[ber[i]])) continue;
			var = worker[ber[i]]["var"];
			if(!stringp(var)) continue;
			sscanf(var,"%s,%*s",type);
			sscanf(ber[i],"%s%*d%*s",temp["maker"]);
			
			temp["name"] = "unnamed";
			if(member_array(type,weapon_list) != -1 ) sscanf(var,"%*s,%*d,%*d,%*d,%*d,%*d,%*d,%*d,%*d,%*d,%*d,%*s,%*s,%*d,%*d,%*d,%*d,%s,%*s",temp["name"]);
			else sscanf(var,"%*s,%*s,%*d,%*d,%*d,%*d,%*d,%*s,%s,%*s",temp["name"]);
			
			if( temp["name"] == "unnamed")
			{
				message("wizard:ciwei",sprintf("Uweapon系统BUG向您汇报：Work:%O,this Var:%s。\n",worker,var),users() );
				continue;
			}
									
			temp["id"]=ber[i];
			temp["type"]=type;
			temp["owner"]=var[strsrch(var,",",-1)+1..<1];
			ret += ({copy(temp)});
		}
		return ret;
	}
	return 0;
}

int confirm_delete (string arg,object me,object wp){
		
	string wid,wlvl;
	if( arg[0]=='y' || arg[0]=='Y' ) 	{
		int ddd;
		wlvl=wp->query("weapon_mp/wlvl");
		ddd=wp->query("weapon_mp/weapon_prop_base/up_dmg");
		wp->delete("weapon_mp/wlvl");
		wp->delete("weapon_mp/wexp");
		wp->delete("weapon_mp/weapon_prop_base/up_dmg");
		if(ddd) wp->add("weapon_mp/weapon_prop_base/damage",-ddd);
		log_file("quest/career/i_delete", sprintf("%s deleted %s(LV:%d,D_Up:%d)\nMap:%O\n\n",
		geteuid(me), wp->name(),wlvl,ddd,wp->query("weapon_prop")));
		
		wp->save_weapon(me);
		wid=wp->query("save_id");
		destruct(wp);
		wp=WORKER_D->load_user_object(me ,me->query("worker/"+wid));
		wp->delete("owner");		
		wp->delete("armor_mp/owner");
		wp->delete("weapon_mp/owner");
		wp->move(environment(me));
		wp->move(me);
		if(mapp (wp->query("weapon_mp"))) WORKER_D->weapon_rank(wp);
		if(mapp (wp->query("armor_mp"))) WORKER_D->armor_rank(wp);
		write("你已经删除了私有装备："+wp->name()+"\n");
		log_file("quest/career/i_delete", sprintf("%s deleted %s(LV:%d,D_Up:%d)\nMap:%O\n\n",
				geteuid(me), wp->name(),wlvl,ddd,wp->query("weapon_prop")));
		me->start_busy(1);	
		return 1;
	}
	else {
		write("\n你决定保留"+ wp->query("name")+"。\n");
	} 		
}

static mapping can_name = ([
	"sword":(["刃":"ren","剑":"jian"]),
	"xiao":(["箫":"xiao",]),
	"axe":(["斧":"fu",]),
	"blade":(["刀":"dao",]),
	"brush":(["笔":"bi",]),
	"club":(["棍":"gun",]),
	"dagger":(["匕":"bi",]),
	"fork":(["叉":"cha",]),
	"hammer":(["锤":"chui",]),//本想加上法轮 。。。算了 xixi
	"hook":(["钩":"gou",]),
	"spear":(["枪":"qiang",]),
	"staff":(["杖":"zhang",]),
	"stick":(["棒":"bang",]),
	"whip":(["鞭":"whip","索":"suo"]),
]);
	//可命名的名字还有很多谁有兴趣就多加点 xixi

int main(object me, string arg)
{
	int i;
	object wp;	
	string id;
	string name,tp;
	mapping* hehe;
	string* ids =({});

	if (!arg) {
		mapping* t=({});
		string* names=({});
		hehe=get_var(me);
		if ((!hehe||!sizeof(hehe)))
			return notify_fail("你目前没有自己的装备。\n");
		//		
		for(i=0;i<sizeof(hehe);i++)
			if(hehe[i]["owner"]==getuid(me)){
				names += ({ hehe[i]["name"] });
				t+= ({ hehe[i]});
			}
		//t=filter_array(hehe, (:$1["owner"]==getuid(me):));
		if(sizeof(t)) write("你目前已经拥有了"+chinese_number(sizeof(t))+"件私有装备："+ implode(names,",")+"。\n");
		else return notify_fail("你目前没有自己的私有装备。\n");
		return 1;
	}
	

	
	if (sscanf(arg, "delete %s", arg)) {
		if (me->is_busy() || me->is_fighting())
			return notify_fail("你正忙着呢！\n");
		if (!objectp(wp = present(arg, me)))
			return notify_fail("你身上没有这样装备。\n");		
		if (!wp->query("imbued") || !stringp(id=wp->query("save_id")) )
			return notify_fail("这不是一件打造的装备。\n");
		hehe=get_var(me);
		if(!hehe || !sizeof(hehe)) return notify_fail("你并没有自己的装备。\n");
		for(i=0;i<sizeof(hehe);i++)
			ids += ({ hehe[i]["id"] });
		if(member_array(id,ids)==-1) return notify_fail("这并不是你的装备。\n");
		if(wp->query("owner")!=getuid(me)) return notify_fail("这并不是你的私有装备。\n");
		
		
		write(HIR+"删除装备将会清除磨砺状态、淬炼经验和强化等级！！！\n"NOR);
		write(HIR+"你要删除装备"+wp->query("name")+HIR+",确定吗？[y|N]："+NOR);
		input_to("confirm_delete",me,wp);
		return 1;
	}
	else if (sscanf(arg, "save %s", arg)) {		
		mapping worker;
		string type;
		if (me->is_busy() || me->is_fighting())
		return notify_fail("你正忙着呢！\n");
		if (!objectp(wp = present(arg, me)))
			return notify_fail("你身上没有这样装备。\n");
		if (!wp->query("imbued") || !stringp(id=wp->query("save_id")) )
			return notify_fail(wp->name()+"不是一件打造的装备。\n");
		
		if(strsrch(id,"weilan tiejiang")==-1) return notify_fail("你目前只能保存韦兰铁匠打造的装备。\n");
		
		hehe=get_var(me);
		if(!hehe || !sizeof(hehe)) return notify_fail("你并没有自己的装备。\n");
		
		for(i=0;i<sizeof(hehe);i++)
			ids += ({ hehe[i]["id"] });
		
		if(member_array(id,ids)==-1) return notify_fail("这并不是你的装备。\n");
		
		type = wp->query("skill_type");
		if(!type) type = wp->query("armor_type");
		
		ids=({});
		for(i=0;i<sizeof(hehe);i++)
			if(hehe[i]["owner"]==getuid(me))  ids += ({ hehe[i]["type"] });
		
		if(member_array(type,ids)!=-1) return notify_fail("你已经保存有相同类型的装备了。\n");
		
		//if( wizardp(me) || SECURITY_D->get_status(me)!="(player)")
                	//return notify_fail("由巫师没有打造的权利同理可得巫师没有保存装备的权利！\n");
                
                if(mapp(wp->query("weapon_mp"))) type ="weapon";
		else if(mapp(wp->query("armor_mp"))) type ="armor";
		else notify_fail("意外错误！！请通知巫师。\n");
		
		if(!mapp(worker = wp->query(type+"_mp"))) return notify_fail("意外错误！！请通知巫师。\n");
		
		wp->move(environment(me));
		worker["owner"] = getuid(me);
		//worker["maker"] +=  "/"+me->query("name")+"("+capitalize(getuid(me))+")";
		wp->set(type+"_mp",worker);
		wp->set("owner",getuid(me));
		//wp->set("maker",wp->query("maker")+"/"+me->query("name")+"("+capitalize(getuid(me))+")");
		
		wp->move(me);
		if(type=="weapon") WORKER_D->weapon_rank(wp);
		if(type=="armor") WORKER_D->armor_rank(wp);
		write("\n"+wp->name()+"的属性已经储存完毕，成为你目前的私有装备！\n");
		log_file("quest/career/i_saved", sprintf("%-8s saved user weapon %s(LV:%d).\n%O\n\n",
			geteuid(me), wp->name(),  wp->query("wlvl"),wp->query("weapon_prop") ));
		me->start_busy(1);
		return 1;
	}
	else if (sscanf(arg, "rename %s %s", arg, name)==2) {
		mapping mp;
		string type;
		if (me->is_busy() || me->is_fighting())
		return notify_fail("你正忙着呢！\n");
		
		if (!objectp(wp = present(arg, me)))
			return notify_fail("你身上没有这样装备。\n");		
		if (!wp->query("imbued") || !stringp(id=wp->query("save_id")) )
			return notify_fail("这不是一件打造的装备。\n");
		hehe=get_var(me);
		if(!hehe || !sizeof(hehe)) return notify_fail("你并没有自己的装备。\n");
		for(i=0;i<sizeof(hehe);i++)
			ids += ({ hehe[i]["id"] });
		if(member_array(id,ids)==-1) return notify_fail("这并不是你的装备。\n");
		if(wp->query("owner")!=getuid(me)) return notify_fail("这并不是你的私有装备。\n");
		
		if( wp->query("imbued") >= 5 )
			return notify_fail("特殊装备不允许改名。\n");
		i = (me->query("user_imbue_modify") + 1) * 1000000;
		if (me->query("balance") < i)
			return notify_fail("为装备改名，你此次需要"+chinese_number(i/10000)+"锭黄金，但是你的银行存款不够！\n");
		
		i = strlen(name);
		if (i < 2 || i > 4)
			return notify_fail("对不起，装备名字必须是 2 到 4 个中文字。\n");


		if (!is_chinese(name))
			return notify_fail("对不起，请您用「中文」取装备名字。\n");		
		
		mp = wp->query("weapon_mp");
				
		if( !mapp(mp) )	{
			return notify_fail("只有武器才可以改名！\n");
		}
		
		type = mp["skill_type"];
		if(wp->id("xiao")) type="xiao";
		if( member_array(name[<4..<1],keys(can_name[type]))==-1 && member_array(name[<1..<1],keys(can_name[type]))==-1 )
			return notify_fail(sprintf("名字的最后几字必须表明装备类型 %s 的最后几字必须为：%s ！\n",type,implode(keys(can_name[type]),",") ));
		
		if (member_array(name[0..<2], banned_name)!=-1)
			return notify_fail("这种装备名字会造成其他人的困扰，想个其它的。\n");
		
		
		write(
			"\n你即将把"+wp->name()+"改名！\n"
			"新名字："+name+"。\n"
			"并且装备ID会变成你的 ID + 装备 ID。\n"
			"本次费用为" + chinese_number((me->query("user_imbue_modify") + 1)*100) + "锭黄金，直接从银行存款扣除，\n"
			"确定请打 "HIY"Y"NOR"，否则取消。\n"
		);		
		input_to((: confirm_name :), me, wp, name,undefinedp(can_name[type][name[<2..<1]])?can_name[type][name[<1..<1]]:can_name[type][name[<2..<1]] );		
	}
	else if (sscanf(arg, "shape %s %s", arg, tp)==2) {
    if (!objectp(wp = present(arg, me)))
			return notify_fail("你身上没有这样装备。\n");		
		if (wp->query("weapon_mp/imbued")!=5)
			return notify_fail("只有传说中的武器才能任意塑形。\n");
		if (member_array(tp,weapon_list) == -1)
			return notify_fail(tp +"是什么东东。\n");
	//	if (me->query("neili") != me->query("max_neili")*2 && me->)
	//	  return notify_fail("你内力不够，不能给武器塑形。\n");

		
		if(me->is_fighting()) {
		  if (me->query_temp("perform_AP") < 40)
		  return notify_fail("你合气度不足无法给武器塑形。\n");
		  else me->add_temp("perform_AP", -40);
	  }	
		else {
			if (me->is_busy())
		  return notify_fail("你正忙着呢。\n");
			me->add_busy(5);
		}
		name = keys(can_name[tp])[0]; 
		me->set("neili",0);
		wp->set("skill_type",tp);
		wp->set_temp("apply/id",({tp}));
		message_vision(HIG"\n$N将$n"HIG"迎风抛起，只见$n"HIG"化做无数的火星又飞回$N的手中凝聚成一把"+HIW+name+HIG"形武器！\n\n"NOR,me,wp);
  }
	else if(arg == "list")
		me->start_more("/cmds/usr/top.c"->show_worker_rank(me,"weapon",10));
	else if (sscanf(arg, "list %d", i))
		me->start_more("/cmds/usr/top.c"->show_worker_rank(me,"weapon",i?i:10));
	else help(me);
	return 1;
}

private void confirm_name(string yn, object me, object wp, string name,string id)
{
	me->add_busy(1);
	if( yn[0]!='y' && yn[0]!='Y' ) {
		write("你取消了给私有装备改名的操作。\n");
	} else {
		write("
您已经确定新名字是【"+name+"】，现在请选取颜色：

RED - "RED"■"NOR"红色              HIR - "HIR"■"NOR"亮红色
GRN - "GRN"■"NOR"绿色              HIG - "HIG"■"NOR"亮绿色
YEL - "YEL"■"NOR"土黄色            HIY - "HIY"■"NOR"亮黄色
BLU - "BLU"■"NOR"深蓝色            HIB - "HIB"■"NOR"亮蓝色
MAG - "MAG"■"NOR"浅紫色            HIM - "HIM"■"NOR"粉红色
CYN - "CYN"■"NOR"蓝绿色            HIC - "HIC"■"NOR"天青色
WHT - "WHT"■"NOR"白色              HIW - "HIW"■"NOR"亮白色

如果不需要颜色，请打 "HIY"Y"NOR"，否则取消。\n");
		input_to((: confirm_color :), me, wp, name,id);
	}
}

private void confirm_color(string yn, object me, object wp, string name,string id)
{
	string color;

	me->add_busy(1);

	switch (yn) {
		case "RED" : color = RED; break;
		case "GRN" : color = GRN; break;
		case "YEL" : color = YEL; break;
		case "BLU" : color = BLU; break;
		case "MAG" : color = MAG; break;
		case "CYN" : color = CYN; break;
		case "WHT" : color = WHT; break;
		case "HIR" : color = HIR; break;
		case "HIG" : color = HIG; break;
		case "HIY" : color = HIY; break;
		case "HIB" : color = HIB; break;
		case "HIM" : color = HIM; break;
		case "HIC" : color = HIC; break;
		case "HIW" : color = HIW; break;
		case "y":
		case "Y": write("请再次确定是否给装备改名，确定请打"WHT" Y"NOR"，否则任意键取消。\n");
			input_to( (: confirm_name_again :), me, wp, name,id );
			return;
		default :
			write("\n并没有这种颜色，所以取消了给私有装备改名的操作。\n");
			return;
	}
	name = color + name;
	write(
		"\n你为装备选择了"+color+"■"NOR"，是否确定改名？\n"
		"请选择 "HIY"Y"NOR"，或任意键取消。\n"
	);
	input_to( (: confirm_name_again :), me, wp, name ,id);
}

private void confirm_name_again(string yn, object me, object wp, string name,string id)
{
	mapping mp;
	string *ids;
	string* newids;
	int i;
	int need_add=0;

	if( yn[0]!='y' && yn[0]!='Y' ) {
		write("你取消了给私有装备改名的操作。\n");
	} else {
		mp = wp->query("weapon_mp");				
		if( !mapp(mp) )	{
			write("只有装备才可以改名！\n");
			return;
		}
		name += NOR;
		
		ids = mp["id"];
		newids = ({geteuid(me) + " " + id});
		for(i=0;i<sizeof(ids);i++)
			if( strsrch(ids[i]," ")==-1)
			{
				newids+= ({ids[i]});
				if(ids[i]==id) need_add=1;
			}
		if(!need_add) newids+= ({id});
		
		i = (me->query("user_imbue_modify") + 1) * 1000000;
		me->add("balance", -i);
		me->add("user_imbue_modify", 1);
		
		log_file("quest/career/i_rename", sprintf("%-8s used %d gold rename %s with %s.\n",
			geteuid(me), i/10000, mp["name"], name,));
		
		//ids[0] = geteuid(me) + " " + ids[2];
		//ids[1] = geteuid(me) + " " + ids[3];
		mp["id"] = newids;
		mp["name"] = name;
		
		wp->set_name(name, newids);				
		wp->move(environment(me));
		wp->move(me);
		wp->set_name(name, newids);		
		if(mapp (wp->query("weapon_mp"))) WORKER_D->weapon_rank(wp);
		if(mapp (wp->query("armor_mp"))) WORKER_D->armor_rank(wp);
				
		write("你花了" + chinese_number(i/10000) + "锭黄金更改装备名字，新的装备名字：" + name + " ！\n");
	}
	me->add_busy(1);
}


int help(object me)
{
	write(@HELP
指令格式 : 
     
       	打 uweapon save <装备>
	确认后，此装备便成为你私人所有，即使丢失下次登陆也会出现在你的身上。

	打 uweapon delete <装备>
	确认后，你的私有装备标志被清除。
	
	打 uweapon rename <装备> <名字>
	为自己的装备改名。
	
	打 uweapon shape <装备> <类型>
	为传说中的武器塑形。
	
        打 uweapon list <排名>
        查看当前装备排名。
	

注意：如果目前游戏里有这件装备，登录时便不会出现在你身上。
      目前只能保存韦兰铁匠为你制作的兵器。

HELP
);
    return 1;

}