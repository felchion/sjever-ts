// purge.c
// modify by looklove 2001.7.4

#include <ansi.h>
#include <mudlib.h>

inherit F_CLEAN_UP;

private int do_purge_players(int day);
int help(object me);

int main(object me, string arg)
{
        string name, reason, msg;
        int day;

        if (me!=this_player(1) || wiz_level(me) < wiz_level("(arch)")||wiz_level(me) < wiz_level(name))
                return notify_fail("你没有权力使用这个指令。\n");

        if( !arg ) return help(me);

        if( sscanf(arg, "%d", day) && day >= 200 && day <= 10000) return do_purge_players(day);

        if( sscanf(arg, "%s because %s", name, reason)==2 ){
                seteuid(getuid());
        	if( file_size(DATA_DIR + "login/" + name[0..0] + "/" + name + SAVE_EXTENSION)<0 )
                	return notify_fail("没有这位使用者。\n");
                "/cmds/wiz/kickout"->main(me, name);

                msg =   ""+name+ "，您好：\n"+
                        "    出于慎重考虑，我们很遗憾的通知您，您在"+CHINESE_MUD_NAME+"的ID已经被系统删除。\n"+
                        "删除原因是“"+reason+"”。如果您准备继续登录书剑游戏，请重新申请ID开始游戏，\n"+
                        "并提请您注意遵守书剑游戏规则。详情请在进入游戏后，用“help rules”指令查看。\n"+
                        "\n"+
                        "此致\n"+
                        "                                书剑巫师组\n"+
                        "\n"+ctime(time());

                SMTP_D->send_mail(me,name,"通知邮件",msg);

                shout(HIR"【书剑通告】"+capitalize(name)+"因为『 "+reason+" 』违反游戏规则消失在这个世界了。\n"NOR);
                write(HIR"【书剑通告】"+capitalize(name)+"因为『 "+reason+" 』违反游戏规则消失在这个世界了。\n"NOR);

                // rm s -f 备份，防止无意中被当做 s -f 的ID而 recovers
                rm(DATA_DIR + "login/" + name[0..0] + "/" + name + ".ooo");
                rm(DATA_DIR + "user/" + name[0..0] + "/" + name + ".ooo");

                rename(DATA_DIR + "login/" + name[0..0] + "/" + name + SAVE_EXTENSION,
                        DATA_DIR + "login/" + name[0..0] + "/" + name + ".ppp");
                rename(DATA_DIR + "user/" + name[0..0] + "/" + name + SAVE_EXTENSION,
                        DATA_DIR + "user/" + name[0..0] + "/" + name + ".ppp");

                log_file("static/PURGE", sprintf("[%s] %s purged %s because %s.\n",
                        ctime(time())[0..15], geteuid(this_player(1)), name, reason));

                return 1;
                }
        else return help(me);
}

private int do_purge_players(int day)
{
        int i, j, k, ppl_cnt = 0, count = 0, t = time() - 86400 * day, login = 0, user = 0;
        string *dir, name, *tmp;
        mixed *info;

        seteuid(getuid());
        message("system", "\n*** 整理玩家储存档中，请稍候.... ***\n", users());
        write("处理登录档案中：");
        dir = get_dir(DATA_DIR + "login/");
        for(i=0; i<sizeof(dir); i++) {
                if (file_size(DATA_DIR + "login/" + dir[i]) != -2) continue;
                reset_eval_cost();
                write(dir[i]);
                flush_messages();
                info = get_dir(DATA_DIR + "login/" + dir[i] + "/", -1);
                tmp = get_dir(DATA_DIR + "user/" + dir[i] + "/");
                j = sizeof(info);
                while (j--) {
                        if (!tmp || member_array(info[j][0], tmp) == -1) {
                                rm(DATA_DIR + "login/" + dir[i] + "/" + info[j][0]);
                                login++;
                                continue;
                        }
                        if (sscanf(info[j][0]+"$", "%s.o$", name)==1) {
                                ppl_cnt++;
                                if (wizhood(name) != "(player)") continue;
                                k = 1;
                        } else k = 0;
                        if (t > info[j][2]) {
                                rm(DATA_DIR + "login/" + dir[i] + "/" + info[j][0]);
                                rm(DATA_DIR + "user/" + dir[i] + "/" + info[j][0]);
                                count += k;
                        }
                }
        }
        write("\n处理数据档案中：");
        dir = get_dir(DATA_DIR + "user/");
        for (i=0; i<sizeof(dir); i++) {
                if (file_size(DATA_DIR + "user/" + dir[i]) != -2) continue;
                reset_eval_cost();
                write(dir[i]);
                flush_messages();
                info = get_dir(DATA_DIR + "user/" + dir[i] + "/", -1);
                tmp = get_dir(DATA_DIR + "login/" + dir[i] + "/");
                j = sizeof(info);
                while (j--) {
                        if (!tmp || member_array(info[j][0], tmp) == -1) {
                                rm(DATA_DIR + "user/" + dir[i] + "/" + info[j][0]);
                                user++;
                                continue;
                        }
                        if (t > info[j][2]) {
                                if (sscanf(info[j][0]+"$", "%s.o$", name)==1) {
                                        if (wizhood(name) != "(player)") continue;
                                        count++;
                                }
                                rm(DATA_DIR + "login/" + dir[i] + "/" + info[j][0]);
                                rm(DATA_DIR + "user/" + dir[i] + "/" + info[j][0]);
                        }
                }
        }
        write("\n\n原来总共有 " + ppl_cnt + " 位使用者。\n");
        write( count + " 个超过 " + day + " 天未上线的使用者被清除掉了。\n");
        write( login + " 个孤立的登录档案被清除掉了。\n");
        write( user + " 个孤立的数据档案被清除掉了。\n");
        write("现在总共有 " + (ppl_cnt - count) + " 位使用者。\n");
        log_file("static/PURGE", sprintf("[%s] %s cleaned up %d characters who didn't login for more than %d days\n"
                                "\t\tResulting statistics: %d characters remaining.\n",
                                ctime(time())[0..15], geteuid(this_player(1)), count, day, ppl_cnt - count));

        return 1;
}

int help(object me)
{
write(@HELP
指令格式：purge [<未上线天数>] | <使用者姓名> because <原因>

清除一个使用者或清除超过一定天数不曾上线地使用者。
请注意删除原因的措辞，因为系统将以此为删除理由，自动向该ID发送通知邮件。
HELP
);
    return 1;
}

