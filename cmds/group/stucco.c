// stucco.c
// by augx@sj 3/20/2002

#include <ansi.h>
inherit F_CLEAN_UP;

int help(object me)
{
	write(@HELP
指令格式 : stucco/fenshua [房间标题]
房间标题长度为2至10个汉字，可以加入颜色字串。
房间描述第一行最多32个汉字，以下一行最多34个汉字，可以使用颜色字串，
颜色字串不包括在34个汉字的正文长度中。颜色字串的使用方法同nick，请参
加help nick。一间房间最多七行描述，请尽量使用中文，并注意格式整齐。
粉刷房间需要石灰(shi hui)。

！！！注意！！！ 严禁在描述中使用反动、黄色文字，一经发现将严肃处理。

HELP
	);
	return 1;
}

int main(object me, string arg)
{
	object material, env = environment(me);
	string arg2;

	if( !arg ) return help(me);
	if( !env || !base_name(env) )
		return notify_fail("你无法粉刷现在所在的地方。\n");
	if( !me->query("group/class") )
		return notify_fail("你还没有参加帮会，粉刷什么？\n");
	if( env->query("group1") != me->query("group/id") )
		return notify_fail("你只能粉刷自己的帮会中的房屋和道路。\n");
	if( !(material=present("shi hui",me)) || !material->query("group") )
		return notify_fail("你没有石灰，如何粉刷啊。\n");
	if( !GROUP_D->site_now(me->query("group/id")) )
		return notify_fail( GROUP_D->get_last_error() );

	arg = GROUP_D->color_string(arg);
	arg2 = strip(arg);
	if( strlen(arg2)<4 || strlen(arg2)>20 )
		return notify_fail("房间标题长度不符合规定。\n");

	write("请输入房间的详细描述：\n");
	me->edit( (: call_other, this_object(), "input_long" , me, arg :) );
	return 1;
}

void input_long(object me,string arg,string str)
{
	object material, env = environment(me);
	string env_path;
	string * lines;
	int delay,i,j,k;

	env_path = base_name(env) + ".c";
	material = present("shi hui",me);

	str = GROUP_D->color_string(str);
	str = replace_string(str ,"\"", "不可用字符");

	lines = explode(str,"\n");
	i = sizeof(lines);
	k = 0;
	if( i > 7 ) i = 7; else k = 1;
	if( i < 1 ) {
		tell_object(me,"你要把这里粉刷成什么样子？\n");
		return;
	}

	str = "";
	for(j=0;j<i;j++) {
		if( strlen( strip(lines[j]) ) > (j==0?64:68) ) {
			tell_object(me,sprintf("第%d行长度超过限制，请重新输入。\n",j+1));
			return;
		}
		str += lines[j];
		if( (j+1)<i || k==0 ) str += "\n";
	}

	if( !GROUP_D->create_room(
		env_path,
		arg,str,
		GROUP_D->get_room_exits(env),GROUP_D->get_room_objects(env),
		env->query("group1"),env->query("group2"),
		env->query("outdoors")?0:1, env->query("have_board") )
	) {
		tell_object(me,"写文件错误，请报告巫师！\n");
		return;
	}

	message_vision("$N开始用"+material->name()+"粉刷"+(env->query("outdoors")?"道路":"房屋")+"……\n",me);
	if(material) destruct(material);
	delay = 10;
	if( wizardp(me) && me->query("env/test") ) delay = 2;
	me->start_busy(delay);

	GROUP_D->encrypt_file(env_path);
	call_out("make_finish",delay*2-1,me,env_path);
}

void make_finish(object me,string file)
{
	if( !me ) return;
	tell_object(me,"你忙碌了半天，总算粉刷好了。\n");
	if( file && strlen(file)>3 ) GROUP_D->update_room(file);
}
