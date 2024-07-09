
inherit ROOM;
void create()
{
        set("short", "老渡口");
        set("long", @LONG
你迎着阵阵海风来到了渡口，这里是一个已经被废弃了很久的渡口。
    这个渡口曾经繁荣过一段很长的时间，因为古时皇帝为了得到长生不
老就从这里出发向传说中的蓬莱航行。
LONG);
	set("outdoors", "佛山");
	set("no_fight",1);
	set("exits",([
		"north" : __DIR__"shulin_dzd",		
	]));	
	setup();
}

#define XKD_D "/d/foshan/xkdd.c"

void init()
{
	object me;
	mapping job;
//    add_action("do_go", "qu");
	me= this_player();
	if(mapp(job = me->query_temp("job_xkd"))){
		if(job["asked"]==1 && !job["ready"] && XKD_D->is_player(me) )			
			call_out("move_to_boat",random(3)+1,me);		
	}
}

void move_to_boat(object me)
{
	mapping job;
	
	if(!XKD_D->is_player(me)) return;	
	if(!mapp(job = me->query_temp("job_xkd"))) return;	
	if(job["asked"]!=1 || job["ready"]==1 ) return;	
	//tell_object(me,file_name(environment(me))+"      "+__FILE__+"\n" );
	if(file_name(environment(me))+".c"!=__FILE__) return;
	if(environment(me)!=this_object()) return;
	job["ready"]=1;
	map_delete(job,"asked");
	XKD_D->player_ready(me);
	tell_object(me,"只见远远有个人想你打了个招呼，你定睛一看，正是那邀你来的中年男子。\n");
	tell_object(me,"那中年男子朗声笑道：还道你不敢来了呢，这就上船吧。\n");
	message_vision("$N略一沉吟，纵身跃上了搁浅在边上的小船。\n",me);
	me->move(XKD_D->query_boat());
	tell_room(environment(me),me->name()+"纵身跳了上来。\n",({me}));	
}