// message.c
// modify by Yu Jue 1998-3-5

#include <ansi.h>
#include <dbase.h>

#define MAX_MSG_BUFFER 100

static string *msg_buffer = ({});

void receive_message(string msgclass, string msg)
{
	string subclass, irc_channel, *chs;
	object me = this_object();

	if (!stringp(msg))
		return;
	if (!userp(me)) {
		me->relay_message(msgclass, msg);
		return;
	}


	if(query_temp("IN_CP") && (msgclass=="tell_room" || msgclass=="sound" 
	|| msgclass=="emote" || msgclass=="vision") ) return;
	
	if (sscanf(msgclass, "%s:%s", msgclass, subclass) == 2) {
		switch (msgclass) {
			case "channel":
				if (!pointerp(chs = query("channels")) || member_array(subclass, chs)==-1)
					return;
				if ( subclass=="irc" && sscanf(msg,"%s:%s",irc_channel, msg)==2 )
					if ( !(wizardp(me) && me->query("env/test")) &&
					     irc_channel!=(string)query_temp("irc/room1") &&
					     irc_channel!=(string)query_temp("irc/room2") &&
					     irc_channel!=(string)query_temp("irc/room3") &&
					     irc_channel!=(string)query_temp("irc/room4") &&
					     irc_channel!=(string)query_temp("irc/room5") &&
					     irc_channel!="irc_channel" ) 
						return;
				break;
			case "wizard":
				if (subclass != me->query("id") && subclass != wizhood(me))
					return;
			case "outdoor":
				break;
			default:
				return;
		}
	}

	if (msgclass == "outdoor" && (!environment() || !environment()->query("outdoors")))
		return;
	if (msgclass == "wizard" && !wiz_level(me))
		return;
	if (query_temp("block_msg/all")) {
		if (msgclass != "channel")
			return;
		msg = "! " + msg;
	}
	if (query_temp("block_msg/" + msgclass))
		return;
	if (query_temp("big5")) msg = "/adm/daemons/gb_b5d"->gb_b5(msg);
	if (in_input(me) || in_edit(me)) {
		if (sizeof(msg_buffer) < MAX_MSG_BUFFER)
			msg_buffer += ({ msg });
	} else
		receive(msg);
}

void write_prompt()
{
	if (sizeof(msg_buffer)) {
		write(BOLD "[暂存讯息]\n" NOR);
		foreach (string item in msg_buffer)
			receive(item);
		msg_buffer = ({});
		write(BOLD "[暂存讯息显示完毕]\n" NOR);
	}
	if(IN_MAIN && !query_temp("IN_CP")) write(HIY "> " NOR);
}
void receive_snoop(string msg)
{
	receive("%" + msg);
}
