//  /shadow/telnet.c
#include <net/socket.h>
#include <ansi.h>
#include <cp.h>
inherit "/feature/shadow";

static string from_user;        // 用户发送来的信息
static int fd;                  // 连接远端机器的套接字
static string dest_addr=CP_IP;        // 连接远端机器的地址
static int port=CP_PORT;                // 连接远端机器的端口号

#define MAX_PENDING_INPUT               16384

protected void send_to_remote();
object owner;
int cmds_enable;
int is_telneting() {return 1;}

string query_dest_addr() { return sprintf("%s %d", dest_addr, port); }

void telnet_input(string str)
{
	object sob;

	if (str == "CLOSE")
	{
		write("用户断开了连接。\n");
		destruct(this_object());
		return;
	}

	//input_to("telnet_input");

	if (strlen(str) + strlen(from_user) < MAX_PENDING_INPUT)
	{
		from_user += str + "\n";
		if (find_call_out("send_to_remote") == -1)
		send_to_remote();
	} else
	write("你输入的太多了...\n");
}

int accept_fight(object ob)
{
	return 0;
	//return notify_fail("正在远程登录中...\n");
}

int accept_hit(object ob)
{
	return accept_fight(ob);
}

int accept_kill(object ob)
{
	return accept_fight(ob);
}

int accept_ansuan(object ob)
{
	return accept_fight(ob);
}

int accept_touxi(object ob)
{
	return accept_fight(ob);
}

void receive_damage(string type, int n, object from)
{
	return;
}

void receive_wound(string type, int n, object from)
{
	return;
}

void heart_beat()
{
	set_heart_beat(0);
}

void fight_ob(object ob)
{
	return;
}

void kill_ob(object ob)
{
	return;
}

void unconcious()
{
	return;
}

void die(object killer)
{
	return;
}

int clean_up()
{
	if (query_shadow_now())
	return 1;

	destruct(this_object());
	return 0;
}

void telnet_resolve_callback(string address, string resolved, int key);

void connect_to(object me){
	owner = me;
	from_user = me->query("id")+"\n";
	telnet_resolve_callback(CP_IP,CP_IP,1);
}

void telnet_resolve_callback(string address, string resolved, int key)
{
	int ret;
	object sob;
	string full_addr;

	if (! objectp(sob = query_shadow_now()))
	{
		destruct(this_object());
		return;
	}

	while(1)
	{
		full_addr = sprintf("%s %d", resolved, port);
		fd = socket_create(STREAM,
		"telnet_read_callback",
		"telnet_close_callback" );
		if (fd < 0)
		{
			message("telnet", "SOCKET 初始化错误。\n", sob);
			break;
		}

		ret = socket_connect(fd, full_addr,	"telnet_read_callback",	"telnet_write_callback");
	
		if (ret != EESUCCESS)
		{
			message("telnet", "网络连接错误。\n", sob);
			if(owner->query("cp_startroom"))
				owner->move("cp_startroom");
			break;
		}

		owner->set_temp("IN_CP",1);
		return;
	}

	destruct(this_object());
}

void telnet_read_callback(int fd, string mess)
{
	object sob;

	if (! objectp(sob = query_shadow_now()))
	{
		destruct(this_object());
		return;
	}

	if(!stringp(mess)) return;

	if(mess[0]==65533 && mess[1]==65533) {
		cmds_enable=1;
		return;
	}
	if(mess[0..0]==CP_RC_START){
		string channel,msg;
		if(mess==CP_CLOSE){
			owner->delete_temp("IN_CP");
			destruct(this_object());
			return;
		}
		else if(mess==CP_REWARD){
			CP_D->cp_update(owner);
		}
		else if(sscanf(mess,CP_MSG"%s==%s",channel,msg)==2){
			CHANNEL_D->do_channel(owner,channel,msg);
		}		
		return;
	}

	message("telnet", mess, sob);
}

void telnet_write_callback(int fd)
{
	if (strlen(from_user))
	send_to_remote();
}

void telnet_close_callback(int fd)
{
	object sob;

	if (objectp(sob = query_shadow_now()))
	message("telnet", HIR "你退出了当前副本。\n" NOR, sob);

	destruct(this_object());
}

void remove(mixed ob){
	if(owner) {
		owner->delete_temp("IN_CP");
		CP_D->cp_update(owner);
	}	
}

void remove_interactive()
{
	destruct(this_object());
}


// 将用户发送来的数据发送到远端服务器上去
varargs void send_to_remote(string arg)
{
	object sob = query_shadow_now();
	if(stringp(arg) && cmds_enable) {
		from_user=arg+"\n";
	}
	switch (socket_write(fd, from_user))
	{
		case EESUCCESS:
		case EECALLBACK:
		from_user = "";
		return;
		case EEWOULDBLOCK:
		return;

		default:
		return;
	}
}

