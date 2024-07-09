// MarryMessage.c Look@SJ 19990916

#include <ansi.h>
#include <net/dns.h>

inherit F_CLEAN_UP;

int invi;
int help(object me);

void create()
{
	seteuid(getuid());
}

int main(object me, string arg)
{
	object obj;

	if (!stringp(me->query("marry/id")))
	{
		return help(me);
	}
	
	obj = LOGIN_D->find_body(me->query("marry/id"));

	if (!obj)
	{
		return notify_fail("你的伴侣现在不在。\n");
	}
	if (!interactive(obj))
	{
		return notify_fail("你的伴侣现在断线了。\n");
	}
	
	if (!arg)
	{
		if((string)me->query("gender") == "女性")
		{
			write(MAG"你多么思念夫君" + me->query("marry/name") + "啊......\n"NOR);
	 		tell_object(obj, sprintf(MAG"你的爱妻" + obj->query("marry/name") + "在思念你......\n" NOR));
		}
        else
		{
			write(MAG"你多么思念爱妻" + me->query("marry/name") + "啊，可是伊人今在何方呢......\n"NOR);
	 		tell_object(obj, sprintf(MAG"你的夫君" + obj->query("marry/name") + "在思念你......\n" NOR));
		}
	}
	else
	{
		switch (arg)
		{
		case "kiss":
			if (objectp(present(me->query("marry/id"), environment(me))))
			{
				message_vision(MAG "$N拥抱着$n深深地一吻......\n"NOR, me, obj);
			}
			else
			{
				write(MAG "你多想拥抱你的" + me->query("marry/name") + "深深地一吻......\n" NOR);
				tell_object(obj, sprintf( MAG "你的%s多想拥抱你，给你深深的一吻......\n" NOR,
					obj->query("marry/name")));
			}
			break;	
		case "bye":
			if (objectp(present(me->query("marry/id"), environment(me))))
			{
				message_vision(MAG "$N含泪凝望着$n，哽咽道: 又要离开了，"
					"何时才能长相聚不分离呀......\n"NOR, me, obj);
			}
			else
			{
				if (me->query("gender")=="女性")
				{
					write(MAG "你多想再看你的夫君" + me->query("marry/name") + "一眼，"
						"让他知道你是这样的一直念挂着他...\n"NOR);
					tell_object(obj, MAG "你的爱妻" + obj->query("marry/name") 
						+ "多想再多看你一眼，山遥水隔，她会一直记挂你的，你要自己保重啊......\n" NOR);
				}
				else
				{
					write(MAG "你多想再看你的爱妻" + me->query("marry/name") + "一眼，"
						"让她知道你是这样的一直念挂着她...\n");
					tell_object(obj, MAG"你的夫君" + obj->query("marry/name") 
						+ "多想再多看你一眼，山遥水隔，他会一直记挂你的，你要自己保重啊......\n"NOR);
				}
			}
			break;
		default:
			write(MAG "你悄悄告诉你的" + me->query("marry/name") + "：" HIM + arg + "\n" NOR);
			message("channel", sprintf(MAG "你的%s悄悄告诉你：" HIM "%s\n" NOR,obj->query("marry/name"), arg), obj);
			break;
		}
	}

	return 1;
}

int help(object me)
{
	write(@HELP
指令格式：mm [讯息]

	你可以用这个指令对你的伴侣说悄悄话。
HELP);
	return 1;
}
