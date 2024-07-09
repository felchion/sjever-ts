// retire.c 金盆洗手
// by augx@sj 3/15/2002

#include <ansi.h>
inherit F_CLEAN_UP;


private void confirm_passwd(string pass, object me)
{
	if ( pass != "Y" && pass != "y" ){
		write("放弃永不复出的决定！\n");
		return;
	}
  me->set("no_pk_ever",1);
  write(HIR"你决定从此永不复出！\n");
	return;
}


void ever(object me)
{
	if( !me->query("no_pk") ) {
		tell_object(me,"你并没有金盆洗手啊。\n");
		return;
	}
	
	if( me->query("no_pk_ever") ) {
		tell_object(me,"你已经决定永不复出了。\n");
		return;
	}
	
	write(HIR"你确定要永不复出吗：\n"NOR);
	input_to("confirm_passwd", me);
	return;
}

void abandon(object me)
{
	if( !me->query("prepare_retire") ) {
		tell_object(me,"你并没有准备金盆洗手啊。\n");
		return;
	}
	me->delete("prepare_retire");
	tell_object(me,"你放弃了金盆洗手。\n");
}


int main(object me, string arg)
{
	if( arg && arg=="abandon" ) {
		abandon(me);
		return 1;
	}

	if( arg && arg=="ever" ) {
		ever(me);
		return 1;
	}
		
	//return notify_fail("金盆洗手正在重新整理中，暂时关闭。\n");
	
	if (me->query("no_pk") || me->query("no_pk_disable"))
		return notify_fail("你不是已经金盆洗手了吗？\n");
				
//	if ( me->query("registered") < 3 )
//		return notify_fail("只有贵宾用户才能金盆洗手。\n");
		
	//if (me->query_temp("user_type") == "worker")
		//return notify_fail("你又不是武林人士，如何金盆洗手？\n");

	if ( time()- me->query("repk_time") < 2592000 )
		return notify_fail("你不是想快意恩仇吗？\n");
		
	if (me->query("prepare_retire")) {
		if( time()- me->query("pk_time") < 259200 )
		{
			me->delete("prepare_retire");
			me->delete("prepare_retire_time");
			return notify_fail("由于你又参与了江湖的是非，所以你决定取消你的金盆洗手。\n");
		}
		if( (time()-me->query("prepare_retire_time")) < 259200 )
			return notify_fail("你金盆洗手的时间还未到，你可以在"+CHINESE_D->chinese_time(259200-time()+me->query("prepare_retire_time"))+"后正式金盆洗手。\n");
		CHANNEL_D->do_channel(me,"chat",HBRED+HIY+"我今日正式金盆洗手，从此退出江湖，不再过问江湖纷争！"NOR);
		me->delete("prepare_retire");
		me->delete("prepare_retire_time");
		me->set("no_pk",me->query("combat_exp"));
		me->set("no_pk_time",time());
		me->save();
		log_file("static/RETIRE",sprintf("金盆洗手：%8s(%-8s) ；exp：%d",me->name(1),me->query("id"),me->query("combat_exp")),me);
	} else {
		if( (time()-me->query("prepare_retire_time")) < 259200 )
			return notify_fail("你不是刚刚才放弃金盆洗手吗？\n");
		if( time()- me->query("pk_time") < 259200 ){
			return notify_fail("你正在江湖争斗中恐怕难以自拔。\n");
		}				
		CHANNEL_D->do_channel(me,"chat",HBRED+HIY+"我已厌倦无尽的江湖争斗，决定在三日之后金盆洗手，从此不再过问江湖纷争！"NOR);
		me->set("prepare_retire",1);
		me->set("prepare_retire_time",time());
	}
	
	return 1;
}

int help(object me)
{
	write(@HELP
指令格式 : retire

金盆洗手，退出江湖，从此不再参与江湖争斗。

如果你决定金盆洗手，在第一次输入此命令后，将有三天时间了结江
湖恩怨，在此期间，如果发生主动PK，将视为你放弃金盆洗手。三天
后再次输入本命令，将正式退出江湖。

如果在三天内你决定放弃金盆洗手，请输入“retire abandon”命令。
如果你放弃，在之后的三天里，你将不能再次金盆洗手。

如果你在最近的三天内参与了江湖争斗那将不能金盆洗手，同样你在
准备金盆洗手的三天参与了江湖争斗，那金盆洗手就会被取消。
 
如果你决定永不复出，请输入“retire ever”命令。

注意:
1.发现正式金盆洗手后的玩家以任何形式参与玩家争斗，将给予减少
  10%-30%的经验值处罚。
2.金盆洗手后至少要三十天才能重出江湖。

其他相关指令: rejoin(重出江湖)

HELP);
	return 1;
}
