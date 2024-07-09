// close.c
// modify by augx@sj 3/27/2002

#include <room.h>

inherit F_CLEAN_UP;

int main(object me, string arg)
{
	mapping doors;
	string *dirs, dir,str;
	object room, door;
	int i,delay;

	if( !arg ) return notify_fail("你要关闭什么？\n");

	if( sscanf(arg,"%s at %s",arg,dir) == 2 ) {
		if( !environment(me)->query("group1") )
			return notify_fail("你只能在自己的帮派里开关自己的大门。\n");
		if( !(str=environment(me)->query("exits/"+dir)) )
			return notify_fail("没有这个方向。\n");
		if( !(room=find_object(str)) )
			if( !(room=load_object(str)) )
				return notify_fail("这个出口有问题，请报告巫师。\n");
		if( !room->query("group1") || room->query("group1")!=me->query("group/id") )
			return notify_fail("你只能用这种方式关闭你自己帮派的大门。\n");
		if( !(door=present(arg,room)) )
			return notify_fail("那个地方没有门啊？\n");
		if( !door->query("open") )
			return notify_fail("门已经关闭了。\n");

		message_vision("$N使尽力气去拉动"+door->name()+"，关上了"+door->name()+"......\n",me);
		tell_room(room,me->name()+"使尽力气去拉动"+door->name()+"，关上了"+door->name()+"......\n");
		door->set("open",0);
		door->do_name();

		delay = 5;
		if( wizardp(me) && me->query("env/test") ) delay = 2;
		me->start_busy(delay);
		return 1;
	}

	doors = environment(me)->query_doors();
	if( mapp(doors) ) {
		dirs = keys(doors);
		if( member_array(arg, dirs)!=-1 ) dir = arg;
		else for(i=0; i<sizeof(dirs); i++)
			if( arg==doors[dirs[i]]["name"] || member_array(arg, doors[dirs[i]]["id"])!=-1 ) {
				dir = dirs[i];
				break;
			}
		if( !dir ) return notify_fail("你要关闭什么？\n");

		if( environment(me)->close_door(dir) ) {
			message_vision("$N将" + doors[dir]["name"] + "关上。\n", me);
			return 1;
		}
	}

	return 0;
}

int help(object me)
{
        write(@HELP
指令格式 : close <door>
           close <door> at <方向>

这个指令可以让你关上门.

HELP
    );
    return 1;
}
