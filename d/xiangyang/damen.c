// Room: /d/xiangyang/damen.c 大门
// Modify By River 98/10
// Modify By Lklv 2001.9.21
// Modify by tangfeng for 连城诀quest

inherit ROOM;
#include <ansi.h>
#include <room.h>
#include <wanted.h>
#include "/d/suzhou/npc/lchj.h";//连城诀

void create()
{
	set("short", HIY"武馆大门"NOR);
	set("long", "这里是襄阳武馆的大门，两扇黑漆漆的木门向左右敞开着，门口的正上方\n"+
"高悬着一块黑底金字的大匾，匾上写着四个大字：「"+HIY"襄陽武舘"NOR+"」。大门两边立\n"+
"柱上竖着一副对联：\n\n"+
HIC"          「幾多離合兩行帛絹難綉盡」\n\n"NOR+
HIC"          「千古恩怨三尺青鋒一斬空」\n\n"NOR+
"门前的台阶两旁各立着一个石狮，从大门口向里面瞧去，武馆的院子里人来人\n"+
"往，忙忙碌碌。而门口更是车水马龙，人来人往川流不息。大门旁边站着一个\n"+
"门卫，脸上神情威严却又饱含岁月沧桑。\n"
        );
	set("outdoors","武馆");
	set("no_fight", 1);
	set("exits", ([
		"south" : __DIR__"eroad1",
		"enter": "/d/wuguan/menlang",
	]));
	set("objects", ([
                "/d/wuguan/npc/menwei" : 1,
//                "/tmp/ciwei/menwei" : 1,
	]));
	set("incity",1);
	setup();
}

int valid_leave(object me,string dir)
{
        object *obj;
	int x;

	if ( me->query_temp( "wg_help" ) || me->query_temp( "wg_road" ) == 2 )
		return notify_fail("武馆门卫说道：" + RANK_D->query_respect(me) +
			"还是再耐心等一会吧，说不定马上就有人来仗义帮助。\n");
	if ( me->query_temp( "wg_juanzeng" ) )
		me->delete_temp( "wg_juanzeng" );
	if ( me->query_temp( "wg_road" ) == 1 )
		me->delete_temp( "wg_road" );
	if ( dir == "enter" ) {
		if ( is_wanted(me) )
			return 0;
		if ( me->query("combat_exp") > 3000 )
		{
			if(!me->query(QUESTDIR1+"over") || me->query(QUESTDIR2+"over")) //如果连成决第一步没有结束或者第二部分已经完成，都无法进入武官
				return notify_fail("门卫上前把手一伸：你的武功够高了，在武馆学不到什么了。\n");
			if(me->query_temp(QUESTDIR2+"start")) //出来按失败算
			{
				message_vision(HIY"\n门卫疑惑地看着$N道：$N刚才怎么进去又出来了？\n", me);
				me->delete_temp("quest/busy");//
				me->delete_temp(QUESTDIR);
				if(random(2)) return notify_fail("门卫上前把手一伸：那有这么好的事情，在武馆进进出出。\n");
				else return notify_fail("门卫上前把手一伸：你所谓的探秘难道是骗我的！\n");		
			}
			obj = users();
			x = sizeof(obj);
			while(x--)
			{
				if ( obj[x]->query_temp(QUESTDIR2+"start") && obj[x]!=me ) 
					return notify_fail("门卫上前把手一伸："+obj[x]->query("name")+RANK_D->query_respect(obj[x]) +"已经去武馆探密去了，你再等等！\n");		  
			}
	   message_vision(HIY"\n$N低头似乎和门卫说了些什么。门卫啊地一声叫了出来。\n"NOR, me);
     if(me->query(QUESTDIR+"time")&&time()-me->query(QUESTDIR+"time")<86400)
			    return notify_fail("门卫上前把手一伸：不管怎样，今天很忙了，"+RANK_D->query_respect(me) +"先行休息去吧！\n");		  
	   if(me->query(QUESTDIR+"combat_exp")&&me->query("combat_exp")-me->query(QUESTDIR+"combat_exp")<50000)
			   return notify_fail("门卫上前把手一伸：以"+RANK_D->query_respect(me) +"目前的经验就算进去，也做不了什么，还是抓紧练功吧！\n");

	   	message_vision(HIY"只见门卫往旁边一闪。$N从门缝里挤了进去。\n"NOR, me);
		  me->set(QUESTDIR+"time",time());
		  me->set(QUESTDIR+"combat_exp",me->query("combat_exp")); 
		  me->set(QUESTDIR2+"start",1);//开始
		  me->set_temp(QUESTDIR2+"start",1);
		  me->set_temp("quest/busy",1);
	}
	}
	return ::valid_leave(me, dir);
}
