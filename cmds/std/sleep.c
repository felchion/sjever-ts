/* 饿、渴，无法通过睡觉恢复体力。*/
// sleep.c
// yuj
// last updated 6/29/96
// Modified by snowman@SJ 28/02/2001
// added sleep string check.
// Modify by LinuX@SJ

mapping party=([
        "emei":       	"峨嵋派",
        "gaibang":    	"丐帮",
        "gumu":       	"古墓派",
        "huashan":    	"华山派",
        "kunlun":   	"昆仑派",
        "mingjiao":   	"明教",
        "murong":     	"姑苏慕容",
        "quanzhen":   	"全真教",
        "shaolin":    	"少林派",
        "shenlong":   	"神龙教",
        "songshan":   	"嵩山派",
        "taohua":     	"桃花岛",
        "tianlong":   	"天龙寺",
        "tiezhang":   	"铁掌帮",
        "wudang":     	"武当派",
        "xingxiu":    	"星宿派",
        "xueshan":    	"大轮寺",
]);

int main(object me, string arg)
{
	mapping fam;
	mixed sleep;
 	object where = environment(me);

	seteuid(getuid());
	
	if (me->is_busy())
		return notify_fail("你正忙着呢！\n");

	if( me->is_fighting() )
		return notify_fail("战斗中不能睡觉！\n");
		
	if ( (!(fam = me->query("family")) || fam["family_name"] != "丐帮")
             && !(where->query("sleep_room"))
             || (where->query("no_sleep_room")) )
             return notify_fail("这里不是你能睡的地方！\n");
        
	if (where->query("hotel") && !(me->query_temp("rent_paid"))){
		message_vision("店小二从门外对$N大叫: 把这里当避难所啊! 先到一楼付钱后再来睡!\n",me);
		return 1;
	}
	sleep = where->query("sleep_room");
	if (intp(sleep) && sleep) {
		write("你往床上一躺，开始睡觉。\n");
		write("不一会儿，你就进入了梦乡。\n");
		me->set_temp("block_msg/all",1);
		message_vision("$N一歪身，倒在床上，不一会便进入了梦乡。\n",me);
	}
	else if( stringp(sleep) ){
		if (sleep == "女性" && me->query("gender") != sleep)
				return notify_fail("这里是女眷睡房！\n");
		if (sleep == "男性" && me->query("gender") != sleep)
				return notify_fail("这里是男人睡房！\n");
		if (member_array(sleep, keys(party)) != -1) {
			if (me->query("family/family_name") != party[sleep])
				return notify_fail("这里是"+ party[sleep] + "的睡房，你跑来干什么？\n");
			write("你往床上一躺，开始睡觉。\n");
			write("不一会儿，你就进入了梦乡。\n");
			me->set_temp("block_msg/all",1);
			message_vision("$N一歪身，倒在床上，不一会便进入了梦乡。\n",me);
		} else {
			write("你往床上一躺，开始睡觉。\n");
			write("不一会儿，你就进入了梦乡。\n");
			me->set_temp("block_msg/all",1);
			message_vision(sleep,me);
		}
	} else { 
		write("你往地下角落一躺，开始睡觉。\n");
		write("不一会儿，你就进入了梦乡。\n");
		me->set_temp("block_msg/all",1);
		message_vision("$N往地下角落屈身一躺，不一会便做起梦来。\n",me);
	}


	if (where->query("hotel")) 
		me->delete_temp("rent_paid");

	me->disable_player(" <睡梦中>");

	call_out("wakeup",10 + random(30 - me->query("con")), me, environment(me));
        
	return 1;
}

void wakeup(object me, object env)
{
	int cost;
	object where;

	if (!me || living(me) || !(where = environment(me)) ) return;
	cost = me->query_condition("sleep");
	if (!cost)
		me->apply_condition("sleep", 9);
	
        while( environment(me)->is_character() )
                me->move(environment(environment(me)));
	if (environment(me)->query("hotel")) cost /= 2;
	/* 饿、渴，无法通过睡觉恢复体力。*/
	cost =1;
	if((me->query("food") && me->query("water")) || !userp(me))	{	
		me->set("qi",  me->query("eff_qi"));
		me->set("jing", me->query("eff_jing"));
		if (me->query("jingli")<me->query("eff_jingli"))
		  me->set("jingli", me->query("eff_jingli"));
	}
	me->enable_player();	
	

			
	if(!me->query("food"))
	{
		me->delete_temp("block_msg/all");
		tell_object(me,"你肚子饿的叽哩咕噜乱叫，没办法只好爬了起来。\n");
		tell_room(where,me->name()+"揉了揉眼睛爬了起来。\n",({me}));
		return;
	}
	
	if(!me->query("water"))
	{
		me->delete_temp("block_msg/all");
		tell_object(me,"你渴的口干舌燥，没办法只好爬了起来。\n");
		tell_room(where,me->name()+"揉了揉眼睛爬了起来。\n",({me}));
		return;
	}
	
	if (cost > 2)
		message_vision("$N一觉醒来，懒洋洋地活动了一下筋骨。\n",me);
	else
		message_vision("$N一觉醒来，精力充沛地活动了一下筋骨。\n",me);
	
	me->delete_temp("block_msg/all");
	
	if (cost > 2)
		write("你一觉醒来，由于睡得太频繁，精神不是很好。\n");
	else
		write("你一觉醒来，觉得精力充沛，该活动一下了。\n");
		
	if (env != environment(me)) me->move(environment(me), 1);
}
