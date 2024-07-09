/**********************************************************************************
//
//      roomleased.c
//      Created by mychat 31/04/2004
//      保留我的签名情况下 free of use.
//      当然更欢迎修改、优化等。
//
//      Email & MSN: codemake@163.com
//      文件位于/cmds/adm下
// 		添加当前房间为可出租的房间
***********************************************************************************/

//#include <roomlease.h>
#include <ansi.h>
#include <room.h>

#ifndef ROOMLEASE_D
#define ROOMLEASE_D "/adm/daemons/roomleased"
#endif

int help(object me);

string id;
string *files;

int main(object me, string arg)
{
	object room; 
	string roomid;
	int valbidtype, valminirent, valminirentadd, valleasetime, 
		valminskill, valmaxskill, valdiscount;
	
	if(id&&id!="*"&&id!=getuid(me))
		return notify_fail("id正在使用该指令 等等哦。。\n");
		
	if(!arg) return help(me);
	
	if(!objectp(room = environment(me)))
		return notify_fail("你目前所在奇特，bug!!\n");
	if(arg=="add here")
	{
		id=getuid(me);
		if(!files || !sizeof(files) ) files = ({base_name(room)});
		else
		{
			if(member_array(base_name(room),files)!=-1)
				return notify_fail("该房间已经添加！\n");			
			files += ({base_name(room)});
		}
		write(sprintf("成功添加%s,目前列表有：%O\n",base_name(room),files));		
		return 1;
	}
	else if(arg=="clear")
	{
		id="*";
		files=({});
		write("列表已经清空。\n");
		return 1;		
	}
	else if(sscanf(arg, "%s %d %d %d %d %d %d %d", 
		roomid, valbidtype, valminirent, valminirentadd, valleasetime, 
		valminskill, valmaxskill, valdiscount)!=8)		
		return notify_fail("参数不齐!!\n");	
	if(!files || !sizeof(files) ) files = ({base_name(room)});	
	if(ROOMLEASE_D->set_room(files, roomid, valbidtype, valminirent*100, valminirentadd*100, 
		room->query("short"), valleasetime*60, 
		valminskill, valmaxskill, valdiscount,base_name(room))) write(sprintf("成功出租该房间，以下是附属房间列表：%O\n",files));
	else write("操作失败，头大了哦！\n");
	id="*";
	files=({});			
	return 1;
//	return 1;
}

int help(object me)
{
  write(@HELP
--------------------------------------------------
指令格式：addlease add here 增加这房间入列表
	  addlease clear 清空列表
	  addlease 	roomid bidtype minirent minirentadd leasetime 
		minskill maxskill discount
		列表入库，以及清空，谨慎使用。
--------------------------------------------------
 将当前房间设为可投标的房间，或修改当前房间投标配置，修改当前房间的配置不
 会影响当前的租借和投标情况，但可能会导致当前投标者在中标后的租借条件变化。
 addlease 使用这个命令，必须处在要配置的房间中
room id 投标代码
bidtype		 0 停止接受投标1可以实名投标2必须匿名投标
minirent		最低投标租金(两银子为单位)
minirentadd		最低加价金额(两银子为单位)
leasetime		出租一次的时间长度(分钟为单位)
minskill		投保需要的最少business skills 暂时无用
maxskill		投保需要的最大business skills，超过本skill不能投标，保护低手 暂时无用
discount		收益的折扣率(万分之)
write by mychat@ln(codemake@163.com)
--------------------------------------------------

HELP
    );
    return 1;
}
