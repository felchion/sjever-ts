// 鹤嘴劲点龙跃窍yinyun-ziqi/lifesave.c
// Write by Cht 1999.9.2
// Modify by snowman@SJ 20/01/2000
// exp 只恢复一半。

#include <ansi.h>
inherit F_SAVE;

string exert_name(){ return HIG"鹤嘴劲点龙跃窍"NOR; }

int exert(object me,object corp)
{
	object obj, where;
	int i;
//	string *No_Save_Id;

	where = environment(me);

	if ( !present(corp,where) )
		return notify_fail("指令格式：yun lifesave corpse\n");

	if( !corp )
		return notify_fail("指令格式：yun lifesave corpse\n");

	if ( corp==me )
		return notify_fail("你现在还不能救自己！\n");

	if ( userp(corp) )
		return notify_fail("你看清楚，那是活人！！！\n");

	if ( !corp->is_corpse() )
		return notify_fail("那不是尸体耶，脑子有毛病啊？\n");

	if (corp->query("no_head"))
		return notify_fail("头已经不见了，算了吧。\n");

	if ((string)corp->query("name") == "腐烂的男尸" || (string)corp->query("name") == "腐烂的女尸")
		return notify_fail("都烂了，你就打消这个念头吧。\n");

	if( userp(me) && ((string)me->query_skill_mapped("force") != "yinyun-ziqi"))
		return notify_fail("你的内功不对。\n");

	if ( !userp(me))
		return notify_fail("");

	if((int)me->query_skill("yinyun-ziqi", 1) < 180 )
		return notify_fail("你的「氤氲紫气」级别不够，不能运用「鹤嘴劲点龙跃窍」救人。\n");

	if((int)me->query_skill("taoism", 1) < 180 )
		return notify_fail("你对「道学心法」领悟不深，不能运用「鹤嘴劲点龙跃窍」救人。\n");

	if((int)me->query_skill("medicine", 1) < 120 )
		return notify_fail("你还得多学点草木药理。\n");

	if( (int)me->query("max_neili", 1) < 3000 )
		return notify_fail("你的内力修为太弱，还不能运用「鹤嘴劲点龙跃窍」救人。\n");

	if( me->query("family/family_name") != "武当派")
		return notify_fail("大胆！敢偷学武当绝技。去死吧……！\n");

	if( !corp->query("victim_user"))
		return notify_fail(corp->name()+"可不是玩家尸体哦。\n");

	if ( corp->query("group_war"))
		return notify_fail(corp->name()+"是在帮派战斗中死亡的，你不能运用「鹤嘴劲点龙跃窍」救人。\n");

	if ( !objectp( obj = find_player(corp->query("victim_id")) ))
		return notify_fail(corp->query("victim_name")+"的魂魄不在游戏里。\n");

	if (!interactive(obj))
		return notify_fail("此人没有连线。\n");

	if (!obj->is_ghost())
		return notify_fail(obj->name() + "已经活过来了，不用你救。\n");

	if( me->query_temp("lifesaved"))
		return notify_fail("可一而不可再，你本次在线已经救过人了。\n");

	i = me->query("lifesave_times");
	if ( me->query("combat_exp", 1)/ 500000 <= i)
		return notify_fail("上次救人，你消耗过多，现在的能力还不能运用「鹤嘴劲点龙跃窍」救人。\n");

/*
	if( file_size("/log/skills/no_lifesave") > 0 ){
		No_Save_Id = explode(read_file("/log/skills/no_lifesave"), "\n");
		if( sizeof(No_Save_Id) > 0 && member_array(getuid(obj), No_Save_Id) != -1 )
			return notify_fail("此人为武当大敌，你要背叛师门！？\n");
	}
*/
	me->add_busy(5);

	message_vision(HIY"只见$N双手食指和拇指虚拿，成“"HIW"鹤嘴劲"NOR""HIY"”势，以食指指尖点在$n耳尖三分处的“龙跃窍”，\n"
			"运起内功，微微摆动。这“"NOR""HIW"鹤嘴劲点龙跃窍"NOR""HIY"”使将出来，便是新断气之人也能还魂片刻。。。\n"NOR, me, corp);

	call_out("do_save",3,me,corp,obj);
	return 1;
}

void do_save(object me,object corp,object obj)
{
	mapping learned;
	int j,yyzq_drop,yyzq_lvl;

	if (!me || !corp || !obj) return;
	if (!present(corp, environment(me))) return;
	if (!interactive(obj)) return;
	if (!obj->is_ghost()) return;

	j = 40 + random(me->query_skill("taoism"));
	yyzq_lvl = me->query_skill("yinyun-ziqi",1);
	yyzq_drop = yyzq_lvl*yyzq_lvl;
	learned = me->query_learned();

	tell_object(obj, HIY "\n一道天罡阳气击中你，痛得你几乎晕死过去！\n\n" NOR);

	message_vision(HIC"\n不一会，$N额头上冒出豆大汗珠，$n也似乎动了一下……\n\n"NOR,me,corp);

	me->add("max_neili",-j );
	learned["yinyun-ziqi"] -= yyzq_drop;
	me->add("shen", yyzq_drop / 10);

	if( learned["yinyun-ziqi"] < 0) {
		me->set_skill("yinyun-ziqi",yyzq_lvl-1);
		learned["yinyun-ziqi"] += yyzq_drop;
	}

	obj->add("lifesaved", 1);

	if( me->query("neili") < 2000 || random(me->query_skill("taoism", 1)) < 50){
		me->set("neili", 0);
		tell_object(me, HIR"猛然，你发现自己已经内力消耗殆尽，无法再支持了！\n"NOR);
		log_file("skills/lifesave",sprintf("%-18s 没有救活(1)%s，消耗了 %d 点最大内力。\n",
			me->name(1)+"("+me->query("id")+")", obj->name(1)+"("+obj->query("id")+")", j), me);
		me->unconcious();
		return;
	}

	me->add("neili",-2000);
	if( !interactive(me) 
//	|| query_ip_number(me) == query_ip_number(obj)
	|| me->query("last_lifesave") == obj->query("id")
	|| obj->query("lifesaved") % 2 == 0 ){
		tell_object(me, HIR"突然你全身一震，一下子失去了"+obj->name()+"的魂魄！\n"NOR);
		me->unconcious();
		me->delete("last_lifesave");
		tell_object(obj, HIR"你觉得四周天罡阳气越来越盛，突然凭空传来一声厉鬼巨喝：“此人不得再离开此地！”\n"NOR);
		log_file("skills/lifesave",sprintf("%-18s 没有救活(2)%s，消耗了 %d 点最大内力。\n",
			me->name(1)+"("+me->query("id")+")", obj->name(1)+"("+obj->query("id")+")", j), me);
		obj->unconcious();
		return;
	}

	tell_room(environment(obj), HIY + obj->name(1)+"化作一道天罡阳气消失了！\n"NOR, obj);
	message_vision("一缕魂魄归到尸体上，$n竟然晃晃悠悠地站起来！\n\n", me, corp);
	destruct(corp);
	obj->move(environment(me));
	"/cmds/wiz/recover"->do_recover(obj, 1);
	obj->unconcious();
	obj->set("last_death", "已由"+me->query("name")+"("+me->query("id")+")救活");

	tell_object(me, "你此次救人共耗费了"+CHINESE_D->chinese_number(j)+"点最大内力，「氤氲紫气」下降了"+CHINESE_D->chinese_number(yyzq_drop)+"点。\n");
	me->set("last_lifesave", obj->query("id"));
	me->set_temp("lifesaved", 1);
	me->add("lifesave_times", 1);
	if ( me->query("lifesave_exp"))
		me->delete("lifesave_exp");
	log_file("skills/lifesave",sprintf("%-18s 救活 %s，耗费了 %d 点最大内力。\n",
		me->name(1)+"("+me->query("id")+")",obj->name(1)+"("+obj->query("id")+")", j), me);
}

int remove()
{
	save();
	return 1;
}

string query_save_file()
{
	return "/log/mine_rank";
}

int help(object me)
{
	write(WHT"\n氤氲紫气「鹤嘴劲点龙跃窍」："NOR"\n");
	write(@HELP
	武当一派，擅长救死扶伤，即便是新断气人，用鹤嘴劲点龙跃窍的功夫，未
	必不可还魂，此法虽妙，但对救人者大伤真元，被救者亦需休养生息。

	要求：	氤氲紫气等级 180 以上；
		道学心法等级 180 以上；
		本草术理等级 120 以上；
		最大内力 3000 以上。

HELP
        );
        return 1;
}
