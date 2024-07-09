// zhaoji.c 和 代掌门系统并用
// by lane@SJ 1/10/2005

#include <ansi.h>

inherit F_CLEAN_UP;
int help(object);

int main(object me)
{
	object obj, *enemy;
	string fam, fire;
	int i;

	fam = me->query("family/family_name");

	if( me->is_busy() )
		return notify_fail("你正忙着呢，先歇口气吧。\n");

	if( !fam )
		return notify_fail("你想召集流氓闹事？\n");

	if( !me->query_temp("inherit_master") )
		return notify_fail("就你这小角色，凭什么在"+fam+"中发号施令啊？\n");

	if( environment(me)->query("no_fight") )
		return notify_fail("这里不能战斗，你就换个地方再闹事吧！\n");

	if( !me->is_fighting() )
		return notify_fail("没什么大事就别召集了吧，别人跑来跑去也很累的！\n");

	enemy = me->query_enemy();
	i = sizeof(enemy);
	while (i--) {
		if( enemy[i]->is_killing(me->query("id")) ) break;
		if( me->is_killing(enemy[i]->query("id")) ) break;
	}
	if (i < 0)
		return notify_fail("和别人切磋武艺，没必要这么劳师动众吧！\n");

	if( me->query_temp("zhaoji_start") )
		return notify_fail("你已经放出了召集同门的讯号，耐心等等待吧！\n");

	obj = me->query_temp("zhaoji_npc");

	if( obj && environment(me) == environment(obj) ) {
		message_vision(CYN"$N和$n对视一望，灵犀互通，微笑不语。\n"NOR, me, obj);
		me->start_busy(1);
			return 1;
	}

	switch( random(6) ) {
		case 0:fire = YEL"黄焰"NOR; break;
		case 1:fire = RED"红焰"NOR; break;
		case 2:fire = HIG"绿焰"NOR; break;
		case 3:fire = BLU"蓝焰"NOR; break;
		case 4:fire = WHT"银焰"NOR; break;
		case 5:fire = MAG"紫焰"NOR; break;
	}
	switch( random(5) ) {
		case 0:message_vision(HIM"\n$N突然纵跃而起，紧跟着"+ chinese_number(1+random(3)) +"只白鸽从$N身后振翅飞起，冲入云端。\n\n"NOR, me); break;
		case 1:message_vision(HIW"\n$N猛的纵身而起，双手一托，"+ chinese_number(1+random(3)) +"只白鸽振翅飞起，直冲云霄。\n\n"NOR, me); break;
		case 2:message_vision(HIR"\n$N猛然跳出圈外，从身后掏出一个爆竹，紧跟着"+ chinese_number(1+random(3)) +"道"+fire+HIR"直冲云霄。\n\n"NOR, me); break;
		case 3:message_vision(HIY"\n$N突然的从身后掏出一个爆竹，紧跟着"+ chinese_number(1+random(3)) +"道"+fire+HIY"冲天升起。\n\n"NOR, me); break;
		case 4:message_vision(HIC"\n$N忽然急攻数招，逼退近敌后立即放出呼召同门的讯号，只见"+ chinese_number(1+random(3)) +"道"+fire+HIC"冲天升起。\n\n"NOR, me); break;
	}
	me->set_temp("zhaoji_start", 1);
	me->start_busy(1+random(2));
	call_out("start", random(4)+1, me);
	return 1;
}

void start(object me)
{
	object obj;

	if( !me ) return;

	if( !me->is_fighting() ) {
		me->delete_temp("zhaoji_start");
		return;
	}

	if( !me->query_temp("inherit_master") ) {
		me->delete_temp("zhaoji_start");
		return;
	}

	me->delete_temp("zhaoji_start");
	obj = new("/clone/npc/party_npc");

	if( !obj ) {
		write(HIW"\n不好，听说你的门派被敌人歼灭了，请赶快通知巫师： Party Npc cannot go to here...\n\n"NOR);
		return;
	}

	obj->move(environment(me));
	me->set_temp("zhaoji_npc", obj);
	obj->set_master_player(me);

	switch( random(4) ) {
		case 0:message_vision(CYN"\n远处的突然传来一阵轻啸，一人$n施展轻功飞驰而来。\n"NOR, me, obj); break;
		case 1:message_vision(CYN"\n只见远处匆匆赶来一个$n。\n"NOR, me, obj); break;
		case 2:message_vision(CYN"\n一个$n看见了$N的讯号，匆匆忙忙赶了过来。\n"NOR, me, obj); break;
		case 3:message_vision(CYN"\n突然的远处小路上闪出一个$n，立即跑到你跟前。\n"NOR, me, obj); break;
	}
	message_vision(HIW"$n一见形式顿觉不妙，大叫一声：“代掌门危险！我来帮你……”\n"NOR, me, obj);
	return;
}

int help(object me)
{
	write(@HELP

这个指令让你召集门派弟子帮助你一起战斗，当然你必须是门派中有威信的
人物。门派弟子会接下和你战斗的其它生物（玩家除外）。

HELP);
	return 1;
}
