// qudu.c 「纯阳无极功」
// Write by cht 1999/9/
// rewrite by olives@SJ 4/27/2001
// 大幅度重新调整改写
// 累死我了
// 以后只要是用query_conditions_by_type("poison") query出来的毒都可以解了
// 如果不是在列表里面的都用剧毒来代称
// By Spiderii 既然列表里的毒都可以qudu.为什么xx_poison不可以？
#include <ansi.h>

mapping get_poison(string poison);
mixed get_my_poison(object me);
string get_msg(object me,object target,mapping poison);

string exert_name(){ return HIG"纯阳无极功"NOR; }
//毒的列表
mapping *poi_list = ({
([
"id":		"snake_poison",	// 所中毒的E文名称
"name":		"蛇毒",		// 所中毒的中文名称
"col":		""BLU"",	// 中毒发作的颜色
"con":		"湛蓝之气",	// 中毒发作的状态
]), 
([
"id":		"xx_poison",
"name":		"星宿掌毒",
"col":		""BLU"",
"con":		"湛蓝之气",
]),
([
"id":		"xuanmin_poison",
"name":		"玄冥掌毒",
"col":		""HIW"",
"con":		"霜白之气",
]),
([
"id":		"dsy_poison",
"name": 	"密宗大手印掌毒",
"col":		""CYN"",
"con":		"青气",
]),
([
"id":		"fs_poison",
"name":		"腐尸毒",
"col":		""HIG"",
"con":		"绿气",
]),
([
"id":		"huagu_poison",
"name":		"化骨绵掌剧毒",
"col":		""BLU"",
"con":		"湛蓝之气",
]),
([
"id":		"man_poison",
"name":		"蔓陀萝花毒",
"col":		""HIB"",
"con":		"黑气",
]),
([
"id":		"qinghua_poison",
"name":		"情花毒",
"col":		""MAG"",
"con":		"紫气",
]),
([
"id":		"qishang_poison",
"name":		"七伤拳毒",
"col":		""HIR"",
"con":		"潮红之气",
]),
([
"id":		"qzhu_poison",
"name":		"千蛛万毒手毒",
"col":		""HIR"",
"con":		"潮红之气",
]),
([
"id":		"sl_poison",
"name":		"神龙毒",
"col":		""BLU"",
"con":		"湛蓝之气",
]),
([
"id":		"sy_poison",
"name":		"大手印掌毒",
"col":		""HIR"",
"con":		"潮红之气",
]),
([
"id":		"warm_poison",
"name":		"热毒",
"col":		""RED"",
"con":		"殷红之气",
]),
([
"id":		"hot_poison",
"name":		"火毒",
"col":		""HIR"",
"con":		"潮红之气",
]),
([
"id":		"cold_poison",
"name":		"寒毒",
"col":		""HIC"",
"con":		"绿青之气",
]),
([
"id":		"bing_poison",
"name":		"冰魄银针毒",
"col":		""HIB"",
"con":		"湛蓝之气",
]),
([
"id":		"bt_poison",
"name":		"怪蛇毒",
"col":		""YEL"",
"con":		"泛黄之气",
]),
([
"id":		"hb_poison",
"name":		"寒冰绵掌毒",
"col":		""HIB"",
"con":		"湛蓝之气",
]),
});

mapping get_poison(string poison)
{
	int i;
	for(i=0;i<sizeof(poi_list);i++) {
		if(poi_list[i]["id"]==poison)
			return poi_list[i];
	}
	return ([
		"id":		poison,
		"name":		"剧毒",
		"col":		""GRN"",
		"con":		"绿气",
	]);
}

mixed get_my_poison(object me)
{
	mapping conditions,my_poison;
 	string *names,name;
 	int lvl=0;
	int i;
	if(!objectp(me))
		return 0;
	if(mapp(conditions = me->query_conditions_by_type("poison"))){
		names = keys(conditions);
		for(i=0;i<sizeof(conditions);i++){
			if(conditions[names[i]]>lvl)
				lvl = conditions[names[i]];
			name  = names[i];
			break;
		}
		if(intp(lvl) 
		&& stringp(name)
		&& lvl
		&& name!="")/*有毒*/{
			my_poison = get_poison(name);
			my_poison["lvl"] = lvl;
			return my_poison;
		}
	}
	return 0;//返回0，代表没找到毒。
}

//显示脱衣服的msg
//这个太长了，我单列出来的。
string get_msg(object me,object target,mapping poison)
{		
	object myarmor,urarmor;
	string msg;
	myarmor = me->query_temp("armor/cloth");
	urarmor = target->query_temp("armor/cloth");

	if (objectp(myarmor) && objectp(urarmor)) {
		msg = "$N手指连伸，点了$n身上"+ chinese_number((poison["lvl"]/100)+3) +"处大穴。$p穴道被点，登时不再颤抖，脸上" +poison["col"] + poison["con"] + NOR"却愈来愈浓。\n"
			"$N当下除去$n身上" + urarmor->name() + "，自己也解开" + myarmor->name() + "，胸膛和$p的背心相贴。\n"+
			"开始以"HIY"「纯阳无极功」"NOR"吸取$n身上的"+poison["col"] + poison["name"] + NOR"。\n\n";
			myarmor->unequip();
			urarmor->unequip();
	}
	else if( objectp(myarmor) && !objectp(urarmor)){
		msg = "$N手指连伸，点了$n身上"+ chinese_number((poison["lvl"]/100)+3) +"处大穴。$p穴道被点，登时不再颤抖，脸上" +poison["col"] + poison["con"] + NOR"却愈来愈浓。\n"
			"$N当下解开" + myarmor->name() + "，胸膛和$p的背心相贴。\n"+
			"开始以"HIY"「纯阳无极功」"NOR"吸取$n身上的"+poison["col"] + poison["name"] + NOR"。\n\n";
			myarmor->unequip();
	}

	else if( !objectp(myarmor) && objectp(urarmor)){
		urarmor->unequip();
		msg = "$N手指连伸，点了$n身上"+ chinese_number((poison["lvl"]/100)+3) +"处大穴。$p穴道被点，登时不再颤抖，脸上" +poison["col"] + poison["con"] + NOR"却愈来愈\n"
		"浓。$N当下除去$n身上" + urarmor->name() + "，胸膛和$p的背心相贴。\n"+
		"开始以"HIY"「纯阳无极功」"NOR"吸取$n身上的\n"+poison["col"] + poison["name"] + NOR"。\n\n";
	}
	else
		msg = "$N手指连伸，点了$n身上"+ chinese_number((poison["lvl"]/100)+3) +"处大穴。$p穴道被点，登时不再颤抖，脸上" +poison["col"] + poison["con"] + NOR"却愈来愈浓。\n"
			"$N当下胸膛和$p的背心相贴。\n"+
			"开始以"HIY"「纯阳无极功」"NOR"吸取$n身上的"+poison["col"] + poison["name"] + NOR"。\n\n";

	return msg;
}

int exert(object me, object target)
{
	string msg;//, *No_Save_Id;
	int i, yyzq_lvl, neili_cost, yyzq_reg;
	object *inv,where;
	mapping poison;

	where=environment(me);
	yyzq_lvl = (int)me->query_skill("yinyun-ziqi", 1);

	if ( !present(target,where) )
		return notify_fail("你要给谁疗毒？\n");
	
	if( me->is_fighting() )
		return notify_fail("你无法在战斗中运功疗毒。\n");

	if ( !userp(me))
		return notify_fail("");

	if((int)me->query_skill("yinyun-ziqi", 1) < 160 )
		return notify_fail("你的「氤氲紫气」级别不够，不能运用「纯阳无极功」救人。\n");

	if((int)me->query_skill("taoism", 1) < 160 )
		return notify_fail("你对「道学心法」领悟不深，不能运用「纯阳无极功」救人。\n");

	if( (int)me->query("max_neili", 1) < 3000 )
		return notify_fail("你的内力修为太弱，还不能运用「纯阳无极功」救人。\n"); 

	if((int)me->query_skill("medicine", 1) < 80 )
			return notify_fail("你还得多学点草木药理。\n");

	me->start_exert(1,"「纯阳无极功」");

	if(!target || target == me ){
		poison = get_my_poison(me);
		if(!mapp(poison))
			return notify_fail("你并没有中毒。\n");

	//	if ( poison["id"] == "xx_poison" || poison["id"] == "fs_poison")
	//		return notify_fail("你所中之毒颇为怪异，看来无法疗毒。\n");

		neili_cost = poison["lvl"]*5;
		yyzq_reg = 160 + poison["lvl"]/2;

		if( (int)me->query("neili") < neili_cost )
			return notify_fail("你现在的真气不足"NOR"。\n");
		
		if( yyzq_lvl < yyzq_reg )
			return notify_fail("你的"MAG"「氤氲紫气」"NOR"修为还不能化掉"+poison["col"] + poison["name"] + NOR "。\n");

		me->set("jiali", 0);
		message_vision("$N坐在一旁，慢慢以真气通走三关，鼓荡丹田中的"MAG"「氤氲紫气」"NOR"，将体内的"+poison["col"] + poison["name"] + NOR"一丝丝的化掉。\n", me);
		me->start_busy(10);
		call_out("huadu", poison["lvl"]/30+2, me,  poison,yyzq_lvl, neili_cost);
		return 1;
	}
	else {
		if( !target->is_character() )
			  return notify_fail("这个是个死物，给它疗毒？\n");
/*
		if( file_size("/log/skills/no_lifesave") > 0 ){
			No_Save_Id = explode(read_file("/log/skills/no_lifesave"), "\n");
			if( sizeof(No_Save_Id) > 0 && member_array(getuid(target), No_Save_Id) != -1 )
				return notify_fail("此人为武当大敌，你要背叛师门！？\n");
		}
*/
		if( me->query("gender") == "男性" ){
			if(target->query("gender")=="女性"){
				inv = all_inventory(environment(me));
				for( i=0; i < sizeof(inv); i++ ) 
					if((string)inv[i]->query("race") == "人类"
					&& inv[i] != me 
					&& inv[i] != target)
						return notify_fail("众目睽睽之下脱光衣裳，你也不替人家想想？\n");
			}
		}
		else if( me->query("gender") == "女性" ) {
			if(target->query("gender")=="女性"
			|| target->is_spouse_of(me)) {
				inv = all_inventory(environment(me));
				for( i=0; i < sizeof(inv); i++ ) 
					if((string)inv[i]->query("race") == "人类"
					&& inv[i] != me 
					&& (inv[i]->query("gender")!="女性"
					&&!me->is_spouse_of(inv[i])))
						return notify_fail("身为武当弟子，在众目睽睽之下脱光衣裳，简直是有伤风化！\n");
			}
			else
				return notify_fail("男女授受不亲，武当弟子行走江湖，於女色上人人律己严谨。\n");
		}
		else return notify_fail("你乃无根无性之人，如何使用「纯阳无极功」？\n");
				
		poison=get_my_poison(me);
		if(mapp(poison))
		       return notify_fail("你已身中"+poison["col"] + poison["name"] + NOR"，恐会散及" + target->name() + "。\n");

		poison = get_my_poison(target);

		if(!mapp(poison))
			return notify_fail(target->name() + "并未中毒。\n");

	//	if ( poison["id"] == "xx_poison" || poison["id"] == "fs_poison")
	//		return notify_fail(target->name()+"所中之毒颇为怪异，看来你无法为其疗毒。\n");

		if( target->is_fighting() )
			return notify_fail(target->name() + "正在战斗中，你无法为其疗毒。\n");

		if( target->is_busy() )
			return notify_fail(target->name() + "正忙着呢，你无法为其疗毒。\n");

		neili_cost = poison["lvl"]*5;
		yyzq_reg = 160 + poison["lvl"]/2;

		if( yyzq_lvl < yyzq_reg )
			return notify_fail("你的"HIY"「纯阳无极功」"NOR"修为还不能为"
			 + target->name() + "吸取"+ poison["col"] + poison["name"] + NOR"。\n");

		me->set("jiali", 0);
		
		msg = get_msg(me,target,poison);
		message_vision(msg, me, target);

		me->start_busy(poison["lvl"]/30);
		target->start_busy(poison["lvl"]/30);

		if( (int)me->query("neili") < neili_cost )
			call_out("selfhurt", 1, me, target, neili_cost);
		else
			call_out("xidu", poison["lvl"]/30+2, me, target, poison,yyzq_lvl, neili_cost);

		return 1;
	}
	return 1;
}

int huadu(object me, mapping poison, int yyzq_lvl, int neili_cost)
{
	int poi_amount, cure_amount, cure;

	if( !me || me->is_ghost() || !living(me) )
		return 0;
	if(wizardp(me)) {
		tell_object(me, poison["id"]);
		tell_object(me, "\n");
	}
	poi_amount = (int)me->query_condition(poison["id"]);
	//这里为什么不用poison["lvl"]呢，因为在call_out的时候，可能已经发作过几次了
	//所以poison["lvl"]不准的，可能大
	cure_amount = yyzq_lvl;
	cure = yyzq_lvl/10;

	me->add("neili", -neili_cost);

	if( poi_amount > cure_amount ){
		tell_object(me, "你体内"+poison["col"] + poison["name"] + NOR"太深，现在无法化除。\n");
		return 0;
	}

	tell_object(me, "\n你将内力循环一周，身子如灌甘露，丹田里的真气似香烟缭绕，悠游自在。\n");

	if( poi_amount <= cure )
		me->clear_condition(poison["id"]);
	else
		me->add_condition(poison["id"], -cure);
	return 1;
}

int xidu(object me, object target, mapping poison, int yyzq_lvl, int neili_cost)
{
	int  ur_poi, cure_amount, cure;

	if( !me || me->is_ghost() || !living(me) || !target || target->is_ghost() )
		return 0;
	//能去解毒的人，身上应该没有毒
	//my_poi = (int)me->query_condition(poison["id"]);
	ur_poi = (int)target->query_condition(poison["id"]);

	cure_amount = yyzq_lvl;
	cure = yyzq_lvl/10;

	me->add("neili", -neili_cost);

	if( ur_poi > cure_amount || !interactive(target) || !interactive(me)) {
		tell_object(me, target->name() + "身上的所中的"+poison["col"] + poison["name"] + NOR"太深了你现在还吸不出来。\n");
		return 0;
	}    
	else {
		message_vision("\n过了约莫半个时辰，只见$N脸上隐隐现出"+poison["col"] + poison["con"] + NOR"，手指微微颤动。\n", me);
		if( ur_poi <= cure ) {
			me->add_condition(poison["id"], ur_poi);
			target->clear_condition(poison["id"]);
		}
		else {
			me->add_condition(poison["id"], cure);
			target->add_condition(poison["id"], -cure);
		}
	}
	return 1;
}

int selfhurt(object me, object target, int neili_cost)
{
	if( !me || me->is_ghost() || !living(me) || !target || target->is_ghost() )
		return 0;

	message_vision(HIW"$N将$n一抱入怀，立时大叫一声，全身打颤。\n"NOR, me, target);

	if(me->query("neili") < neili_cost/2) {
		me->set("neili", 0);
		me->receive_damage("qi", (int)me->query("qi")+1, "毒气攻心死了");
		return 1;
	}
	else {
		me->add("neili", -(neili_cost/2));
		me->receive_damage("qi", neili_cost, "毒气攻心死了");
		me->receive_wound("qi", neili_cost/5, "毒气攻心死了");
		return 1;
	}
}

int help(object me)
{
        write(WHT"\n氤氲紫气「纯阳无极功」："NOR"\n");
        write(@HELP
        氤氲紫气对各类毒也有妙法，运用真气驱毒可逼出在体内的毒素，如果是男弟子
        还可以用纯阳无级功帮自己的妻子或者别的男子驱除体内的各种毒素。不过如果
        中毒过深，也回天伐术了。

        要求：氤氲紫气等级 160 以上；
              道学心法等级 160 以上；
              本草术理等级 80 以上；
              最大内力 3000 以上。

HELP
        );
        return 1;
}
