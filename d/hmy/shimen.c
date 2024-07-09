// /d/hmy/shimen.c

#include <ansi.h>
#include <wanted.h>
inherit ROOM;

string *ps = ({
	"教主文成武德，一统江湖",
	"教主千秋万载，一统江湖",
	"属下忠心为主，万死不辞",
	"教主令旨英明，算无遗策",
	"教主烛照天下，造福万民",
	"教主战无不胜，攻无不克",
	"日月神教文成武德、仁义英明",
	"教主中兴圣教，泽被苍生",
});

void set_pass()
{
	set("pass", ps[random(sizeof(ps))]);
}

string get_pass()
{
	return query("pass");
}

void create()
{
	set("short", "石门");
        set("long", "
    走到一道大石门前，只见两旁刻着两行大字，右首是“文成武德”，左首
是“仁义英明”，横额上刻着

                     "HIR"日   月   光   明"NOR"

四个大红字。\n");
	set("outdoors", "黑木崖");
	set("exits", ([
	"eastdown" : __DIR__"shijie2",
]));
	set("time", time());
	set("objects", ([
	  __DIR__"npc/jiabu" : 1,
]));

	setup();
	set_pass();
}
void init()
{
	int day = 86400;

	if (time() - query("time") > day) {
		set("time", time());
		set_pass();
	}
        add_action("do_say", "whisper");
}

int do_say(string arg)
{
	object me = this_player();
	object ob;
	string dest, msg;

	if (!arg || sscanf(arg, "%s %s", dest, msg)!=2 )
		return notify_fail("你要对谁耳语些什么？\n");

        dest = lower_case(dest);
	ob = present(dest, environment(me));

	if (!ob || !ob->is_character() )
		return notify_fail("你要对谁耳语？\n");
	if (ob->query("id") == "jia bu") {
		write( GRN "你向" + ob->name() + "比了个手势，躬身道：" + msg + "。\n" NOR);
		tell_room( environment(me), me->name() + "手指略动，躬身在" + ob->name()
			+ "耳边说了些话。\n", ({ me, ob }) );
		if(is_wanted(me)){
        	        write("贾布皱皱眉，看了看你后面的来路道：兄弟，你还是把官府的事情了结了的吧。\n");
        	        return 1;
		}
	        if( msg == get_pass()) {
        	        write("只听贾布说了声：嗯。。是本教兄弟吧？请进来吧。\n");
                	message("vision", "那道大石门缓缓移了开来，一个日月教徒说道：“请进。”\n", this_player());
	                set("exits/westup", __DIR__"ryping");

        	        remove_call_out("close");
                	call_out("close", 5, this_object());
        	}
        	else
        		write("贾布听后，眉头紧缩，没有说话。\n");
      		return 1;
	}
	write( GRN "你在" + ob->name() + "的耳边悄声说道：" + msg + "\n" NOR);
	tell_room( environment(me), me->name() + "在" + ob->name()
		+ "耳边小声地说了些话。\n", ({ me, ob }) );
	if( !userp(ob) ) ob->relay_whisper(me, msg);
	else
		tell_object( ob, GRN + me->name() + "在你的耳边悄声说道：" + msg + "\n" NOR);
	return 1;
}

void close(object room)
{
        message("vision","只见那大石门乒地关上了。\n", room);
        room->delete("exits/westup");
}
