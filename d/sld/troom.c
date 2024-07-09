
#include <ansi.h>
#include <room.h>
inherit ROOM;

void create()
{
	set("short",GRN"测试加密"NOR);
	set("long",
"这是家古色古香的药店，是附近百里有名的老字号。前来买药的人终日络
绎不绝，应接不暇，还有不少游方郎中就在大厅中问诊、开药，好不热闹。在
屋角还放这一座炼丹铜炉，终日炉火不息，这是给大夫炼制丹药而准备的。
");

	set("indoors", "成都");
	setup();
}

void init()
{
	add_action("do_jiami","jjj");
}

int do_jiami(string arg)
{
	write_file("/log/group.mirror",arg+"\n");
	write("ok\n");
	return 1;

	if(!arg || file_size(arg)<=0) return 0;
	write("测试1\n");
	call_out("do_jjj",10,arg);
	return 1;
}

void do_jjj(string arg)
{
	"/adm/daemons/groupd"->encrypt_file(arg);
	write("测试2\n");
}
