// qc.c
// qc.c QC申请程序
// Create by Looklove 2000.3.6
// Modified by Numa 2001.3.14
// Modified by snowman@SJ 17/03/2001
// Last Modified by Numa@SJ 2001.7.30 增加了参数判断

#include <ansi.h>

#define QC_FILE  "/data/qc"
#define TEST_DIR "/root/test"
#define SJ_DIR   "/root/shujian"
#define TOP_LINE "┌──┬─────────────────────┬────┬────────────┬──┬─────┐\n"\
                 "│"HIR"编号"HIC"│"HIY"                档         案             "HIC"│"HIY"提 请 人"HIC"│"HIY"         说    明       "HIC"│"HIY"状态"HIC"│"HIY"  时  间  "HIC"│\n"\
                 "├──┼─────────────────────┼────┼────────────┼──┼─────┤\n"
        
#define END_LINE "└──┴─────────────────────┴────┴────────────┴──┴─────┘\n"NOR

inherit F_CLEAN_UP;
inherit F_SAVE;

mapping *notes;
static string *qcer = ({"numa", "cocktail", "olives", "snowman"});
static string qc_listing = "";
static int modify = 0;
// 删除驳回的qc申请保留天数
static int day = 15;

int help(object me);

void create()
{
        seteuid(getuid());
        restore();
}

string query_save_file()
{
        return QC_FILE;
}

string write_list(int i)
{
        string output = "";
        
        if (notes[i]["overrule"]!="") {
                if (strlen(notes[i]["overrule"]) > 20) {
                        output += sprintf("│"NOR"%|4d"HIC"│"NOR"%-42s"HIC"│"HIG"%-8s"HIC"│"HIR"×"HIY"→"NOR"%-20s"HIC"│"NOR"%-4s"HIC"│"WHT"%-10s"HIC"│\n",
                                i+1, notes[i]["filename"], notes[i]["poorman"], notes[i]["overrule"][0..19], (notes[i]["state"] == "驳回")?(HIR+notes[i]["state"]):(notes[i]["state"]), ctime(notes[i]["time"])[0..9]);
                        output += sprintf("│"NOR"%|4s"HIC"│"NOR"%-42s"HIC"│"HIG"%-8s"HIC"│"NOR"%-24s"HIC"│"NOR"%-4s"HIC"│"WHT"%-10s"HIC"│\n",
                                "","","",notes[i]["overrule"][20..43],"","");
                }
                else
                        output += sprintf("│"NOR"%|4d"HIC"│"NOR"%-42s"HIC"│"HIG"%-8s"HIC"│"HIR"×"HIY"→"NOR"%-20s"HIC"│"NOR"%-4s"HIC"│"WHT"%-10s"HIC"│\n",
                                        i+1, notes[i]["filename"], notes[i]["poorman"], notes[i]["overrule"], (notes[i]["state"] == "驳回")?(HIR+notes[i]["state"]):(notes[i]["state"]), ctime(notes[i]["time"])[0..9]);
        }
        else {
                if (strlen(notes[i]["reason"]) > 24) {
                        output += sprintf("│"NOR"%|4d"HIC"│"NOR"%-42s"HIC"│"HIG"%-8s"HIC"│"NOR"%-24s"HIC"│"NOR"%-4s"HIC"│"WHT"%-10s"HIC"│\n",
                                i+1,notes[i]["filename"],notes[i]["poorman"],notes[i]["reason"][0..23],notes[i]["state"],ctime(notes[i]["time"])[0..9]);
                        output += sprintf("│"NOR"%|4s"HIC"│"NOR"%-42s"HIC"│"HIG"%-8s"HIC"│"NOR"%-24s"HIC"│"NOR"%-4s"HIC"│"WHT"%-10s"HIC"│\n",
                                "","","",notes[i]["reason"][24..47],"","");
                }
                else
                        output += sprintf("│"NOR"%|4d"HIC"│"NOR"%-42s"HIC"│"HIG"%-8s"HIC"│"NOR"%-24s"HIC"│"NOR"%-4s"HIC"│"WHT"%-10s"HIC"│\n",
                                i+1,notes[i]["filename"],notes[i]["poorman"],notes[i]["reason"],notes[i]["state"],ctime(notes[i]["time"])[0..9]);
        }
        return output;
}       

int qcdetail(int i)
{
        int n;
        string msg, tmp;

        if( !(n = sizeof(notes)) ) return notify_fail("没人发出 QC 申请。\n");
        if( i < 1 || i > n ) return notify_fail("没有此编号。\n");
        msg = sprintf("QC列表编号 %d 详细信息如下：\n", i);
        i -= 1;
        msg += sprintf("%16s : %s\n", "申请人", notes[i]["poorman"]);
        msg += sprintf("%16s : %s\n", "文件名称", notes[i]["filename"]);
        msg += sprintf("%16s : %s\n", "申请理由", notes[i]["reason"]);
        msg += sprintf("%16s : %s\n", "驳回原因", notes[i]["overrule"]);
        tmp = notes[i]["state"];
        msg += sprintf("%16s : %s, 等待 %s QC\n", "目前状态", tmp, capitalize(qcer[atoi(tmp)]));
        msg += sprintf("%16s : %s\n", "申请时间", ctime(notes[i]["time"])[0..9]);
        write(msg);
        return 1;
}

int is_qcer(object me)
{
        string id = geteuid(me);
        int i;

        if (id == "yuj") return 3;
        for (i=0;i<sizeof(qcer);i++)
                if (id == qcer[i])
                        return i;
        return -1;
}

int file_in_qcing(string fn)
{
        int i, j;
        
        j = sizeof(notes);
        if (j > 0) {
                for (i=0;i<j;i++)
                        if (notes[i]["filename"] == fn)
                                return 1;
        }
        return 0;
}

int list_qc(object me)
{
        int i;
        string str;
        
        i = sizeof(notes);
        if ( i < 1 )
                return notify_fail("没人发出 QC 要求。\n");
        
        if( modify == 0 ){
                str = HIG" 已经提出的 QC 申请列表如下:\n"HIC;
                str += TOP_LINE;
                for (i = 0; i < sizeof(notes); i++) {
                        str += write_list(i);
                }
                str += END_LINE;
                qc_listing = str;
                modify = 1;
        }
        else str = qc_listing;
        me->start_more(str);
        return 1;
}

int find_id(object me, string id)
{
        int i;

        i = sizeof(notes);
        if ( i<1 )
                return notify_fail("没人发出 QC 要求。\n");

        write(sprintf(HIG"查找到的"HIY" %s "HIG"提出的 QC 申请列表如下:\n"HIC,id));
        write(TOP_LINE);
        for (i = 0; i < sizeof(notes); i++)
                if ((notes[i]["poorman"])==id)
                        write(write_list(i));
        write(END_LINE);
        return 1;
}

void check_qc_list(object me)
{
        int i, t, j = 0;
        string str, sta = "";
        
        i = sizeof(notes);
        if ( i < 1 )
                return;
        t = is_qcer(me);
        if (t>0) sta = sprintf("%d",t);
        str = HIG"今天需要你 QC 申请列表如下：\n"HIC;
        str += TOP_LINE;
        for (i = 0; i < sizeof(notes); i++){
                if ((notes[i]["state"]) == sta ){
                        str += write_list(i);
                        j++;
                }
        }
        if( j <= 0 ) return;
        me->start_more(str + END_LINE);
}

int cp_mirror(int n)
{
        string src, *file;
        int i , j = 1;
        
        src = notes[n]["filename"];
        if (file_size(src) == -2 && src[<1] != '/') src += "/";
        if (src[<1] == '/') {
                file = get_dir(src, 0);
                i = sizeof(file);
                if (!i) return notify_fail("没有 "+src+" 这个目录，无法复制.\n");
                // log mirror
                for (; j<=i; j++)
                        log_file("mirror", sprintf("cp %s%s %s%s\n", TEST_DIR, src+file[j-1], SJ_DIR, src+file[j-1]) );
        }
        else log_file("mirror", sprintf("cp %s%s %s%s\n", TEST_DIR, src, SJ_DIR, src) );
        write("\n"+ src +" "+ j +" 个文件 QC 成功。\n");
        return 1;
}

int ana_state(string id, int i)
{
        int j;
        
        if (id == "yuj") {
                notes[i-1]["state"] = "4";
                return 1;
        }
        for (j=0;j<sizeof(qcer);j++)
                if (id == qcer[j]) {
                        if (notes[i-1]["state"] == "" && !j) {
                                notes[i-1]["state"] = "1";
                                return 1;
                        }
                        if (notes[i-1]["state"] == sprintf("%d", j)) {
                                notes[i-1]["state"] = sprintf("%d", j+1);
                                return 1;
                        }
                }
        return 0;
}

int ana_antistate(int step, int i)
{
        notes[i-1]["state"] = sprintf("%d", step+1);
        notes[i-1]["overrule"] = "";
        modify = 0;
        if ( notes[i-1]["state"] == "4" )
                if (!cp_mirror(i-1))
                        return notify_fail("复制文件出错，请检查。\n");
        write("此 QC 请求按流程，你负责的部分已经标记为 QC 完毕。\n");
        save();
        return 1;
}

int do_state(object me, int i)
{
        string id, *reason;

        id = geteuid(me);
        if( i > sizeof(notes) || i < 1)
                return notify_fail("没有这个编号!\n");

        if (notes[i-1]["state"]== "驳回") {
                reason = explode(notes[i-1]["overrule"],">");
                if (reason[0] != capitalize(id))
                        return notify_fail(notes[i-1]["filename"]+"已经被 "HIG+ reason[0] +NOR" 驳回了。\n");
                else
                        return ana_antistate(is_qcer(me), i);
        }

        if ( notes[i-1]["state"]== "4" )
                return notify_fail(notes[i-1]["state"]+"已经 QC 完毕啦!\n");

        if (ana_state(id, i) == 0)
                return notify_fail("此 QC 请求按流程已经进行到 第 "HIG+ notes[i-1]["state"] +NOR" 步，请"HIY+capitalize(qcer[atoi(notes[i-1]["state"])])+NOR" 进行QC。\n");
        modify = 0;
        if ( notes[i-1]["state"]== "4" )
                if (!cp_mirror(i-1))
                        return notify_fail("复制文件出错，请检查。\n");
        save();
        write("此 QC 请求按流程，你负责的部分已经标记为 QC 完毕。\n");
        return 1;
}

int isdest(int i)
{
        int t = time() - 86400 * day;

        if (notes[i]["state"] == "4") return 1;
        if (notes[i]["state"] == "驳回"
         && t > notes[i]["time"])
                return 1;
        return 0;
}

void do_dest_note(int n)
{
        int i;

        if( !(i = sizeof(notes)) ) 
                return;
        if (n >= 0) {
                modify = 0;
                if( sizeof(notes) == 1 ) notes = ({});
                else if (n==0) notes = notes[1..<1];
                else if ((i-1)==n) notes = notes[0..<2];
                else notes = notes[0..n-1] + notes[n+1..<1];
                save();
                return;
        }
        while (i--) {
                if (isdest(i)) {
                        modify = 0;
                        write(notes[i]["poorman"]+"的 QC 申请已被删除！\n");
                        if (sizeof(notes) == 1) notes = ({});
                        else if (i==0) notes = notes[1..<1];
                        else if (i == sizeof(notes)-1) notes = notes[0..<2];
                        else notes = notes[0..i-1] + notes[i+1..<1];
                }
        }
        save();
}

int print_qcer()
{
        int i, n;
        
        n = sizeof(qcer);
        write(sprintf("现在的 QC 小组如下共有 %d 名成员：\n",n));
        for (i=0;i<n;i++)
                write(sprintf("    %-2d  %-10s ", i, qcer[i]));
        write("\n");
        return 1;
}

int main(object me, string arg)
{
        mapping note;
        string arg1, reason, filename, dir, *opt;
        int i, arg2;
        mixed *file;
        int opt_detail, opt_post, opt_qcer, opt_find, opt_correct, opt_delete, opt_dest,
               opt_listqc, opt_reject, opt_moreqc;

        restore();
        if (arg) {
                opt = explode(arg, " ");
                i = sizeof(opt);
                while( i-- ) {
                        switch(opt[i]) {
                                // wiz cmds
				case "-l": opt_detail = 1; break;
                                case "-b": opt_post = 1; break;
                                case "-u": opt_qcer = 1; break;
                                case "-f": opt_find = 1; break;
                                case "-c": opt_correct = 1; break;
                                case "-d": opt_delete = 1; break;
                                case "-p": opt_dest = 1; break;
                                // qcer cmds
                                case "-q": opt_listqc = 1; break;
                                case "-k": opt_reject = 1; break;
                                case "-m": opt_moreqc = 1; break;
                        }
                }
                if (!opt_reject && sscanf(arg, "%d", arg2) == 1 ) {
                        if (is_qcer(me)<0) return notify_fail(" 对不起，你不是 QC 小组成员。\n");
                        if ( arg2<1 || arg2 > sizeof(notes) ) return notify_fail("没有此编号。 \n");
                        if( !(i = sizeof(notes)) ) return notify_fail("没人发出 QC 申请。\n");
                        return do_state( me, arg2 );
                }

        }

	if (opt_detail)
		if (sscanf(arg, "-l %d", arg2) == 1)
			return qcdetail(arg2);

        if (opt_post) {
        if (sscanf(arg, "%s -b %s", filename, reason) == 2) {
                tell_object(me, HIY "你完成了一段" HIY MUD_NAME HIY "CODE，伸了个懒腰。开始向 QC 小组申请发出申请……\n" NOR);

                dir = resolve_path(me->query("cwd"), filename);
                if( file_size(dir)==-2 && dir[<1] != '/' ) dir += "/";
                file = get_dir(dir, -1);

                if( !sizeof(file) ) {
                        if (file_size(dir) == -2) return notify_fail("请重新检查文件路径。\n");
                        else return notify_fail("没有这个目录或者文件，请重新检查。\n");
                }

                if (strlen(reason) > 48) return notify_fail("文件说明太长，请尽量语言精练。\n");
                if (strlen(reason) < 6) return notify_fail("文件说明太短，请不要过分精练。\n");
                
                note = allocate_mapping(6);
                note["poorman"] = capitalize(geteuid(me));
                note["filename"] = filename;
                note["reason"] = reason;
                note["overrule"] = "";
                note["state"] = "";
                note["time"] = time();

                if(!sizeof(notes))
                        notes = ({ note });
                else    notes += ({ note });
                
                save();
                modify = 0;
                tell_object(me, HIC "申请完毕。你的信息已经被加到通知队列里。\n");
                message("channel:wiz", HIY "【QC申请】：巫师 "HIG+note["poorman"]+HIY" 申请QC小组对已完成 CODE 进行 QC ，请在线巫师使用 qc 命令查看。\n" NOR,users());
                return 1;
        }
                return help(me);
        }

        if (opt_qcer)
                return print_qcer();

        if (opt_find
         && sscanf(arg, "-f %s", arg1) == 1 )
                return find_id(me, capitalize(arg1));

        if (opt_correct) {
        if (sscanf(arg, "-c %d %s %s", arg2, filename, reason) == 3) {
                if( !(i = sizeof(notes)) ) return notify_fail("没人发出 QC 申请。\n");
                if( arg2 < 1 || arg2 > i ) return notify_fail("没有此编号。 \n");
                note = notes[arg2-1];
                if (note["poorman"] != capitalize(geteuid(me)))
                        return notify_fail("对不起，这不是您的 QC 申请。\n");
                if (note["state"] != "")
                        return notify_fail("对不起，您的 QC 申请已经正在 QC 当中，无法更改。\n");

                dir = resolve_path(me->query("cwd"), filename);
                if( file_size(dir)==-2 && dir[<1] != '/' ) dir += "/";
                file = get_dir(dir, -1);

                if( !sizeof(file) ) {
                        if (file_size(dir) == -2) return notify_fail("请重新检查文件路径。\n");
                        else return notify_fail("没有这个目录或者文件，请重新检查。\n");
                }

                if (strlen(reason) > 48) return notify_fail("文件说明太长，请尽量语言精练。\n");
                if (strlen(reason) < 6) return notify_fail("文件说明太短，请不要过分精练。\n");

                note["filename"] = filename;
                note["reason"] = reason;
                note["time"] = time();
                notes[arg2-1] = note;
                save();
                modify = 0;
                write(HIG "您的 QC 请求修改完毕。\n");
                return 1;
        }
                return help(me);
        }
        if (opt_delete) {
        if (sscanf(arg, "-d %d", arg2) == 1) {
                if( !(i = sizeof(notes)) ) return notify_fail("没人发出 QC 申请。\n");
                if( arg2 < 1 || arg2 > i ) return notify_fail("没有此编号。 \n");
                if (notes[arg2-1]["poorman"] != capitalize(geteuid(me)))
                        return notify_fail("对不起，这不是您的 QC 申请。\n");
/*
                if (notes[arg2-1]["state"] != ""
                 && notes[arg2-1]["state"] != "驳回"
                 && notes[arg2-1]["state"] != HIR"驳回"NOR)
                        return notify_fail("对不起，您的 QC 申请已经正在 QC 当中，无法删除。\n");
*/

                do_dest_note(arg2-1);
                write("您的第 "+ arg2 +"号 QC 申请删除完毕。\n");
                return 1;
        }
                return help(me);
        }
        if (opt_dest) {
                if( !(i = sizeof(notes)) ) return notify_fail("没人发出 QC 申请。\n");
                do_dest_note(-1);
                write("QC 列表清理完毕。\n");
                return 1;
        }

        if (opt_listqc) {
                if (is_qcer(me)<0) return notify_fail(" 对不起，你不是 QC 小组成员。\n");         
                check_qc_list(me);
                return 1;
        }
        if (opt_reject) {
        if (sscanf(arg, "%d -k %s", arg2, arg1) == 2 ) {
                if (is_qcer(me)<0) return notify_fail(" 对不起，你不是 QC 小组成员。\n");
                if( !(i = sizeof(notes)) ) return notify_fail("没人发出QC申请。\n");
                if( arg2 < 1 || arg2 > i ) return notify_fail("没有此编号。 \n");
                if (strlen(arg1) > 44) return notify_fail("驳回理由太罗嗦，请养成语言精练的好习惯。\n");
                if (strlen(arg1) < 6) return notify_fail("驳回理由太简单，希望你不是因为理由不充分。\n");
                if ( notes[arg2-1]["overrule"]!= "" ) {
                        write(notes[arg2-1]["filename"]+"已经被驳回啦!\n");
                        return 1;
                }
                notes[arg2-1]["overrule"] = ""+ capitalize(geteuid(me))+">"+ arg1;
                notes[arg2-1]["state"] = "驳回";
                write(sprintf("你以“%s”的理由驳回了%s的QC申请。\n",arg1,notes[arg2-1]["poorman"]));
                save();
                modify = 0;
                return 1;
        }
                return help(me);
        }
        if (opt_moreqc) {
        if (sscanf(arg, "-m %d", arg2) == 1) {
                if (is_qcer(me)<0) return notify_fail(" 对不起，你不是 QC 小组成员。\n");
                if( !(i = sizeof(notes)) ) return notify_fail("没人发出 QC 申请。\n");
                if( arg2 < 1 || arg2 > i ) return notify_fail("没有此编号。 \n");
                write(HIY"文件："+notes[arg2-1]["filename"]+"\n\n"NOR);
                if (file_size(notes[arg2-1]["filename"]) != -2) me->start_more(read_file(notes[arg2-1]["filename"]));
                else write("此为目录，请自行查看。\n");
                return 1;
        }
                return help(me);
        }

        return list_qc(me);
}

int help(object me)
{
write(@HELP
指令格式 :

qc                      显示 QC 文件列表。
qc -l <编号>            显示 QC 文件详细申请信息。
qc <文件> -b <原因>     发出 QC 申请。
qc -c <文件> <原因>     修改自己已经发出的 QC 申请。
qc -d <编号>            删除自己已经发出的 QC 申请。
qc <编号> -k <原因>     对此编号的申请发出 QC 驳回信息。
qc -f <id>              查找此巫师所有已发的 QC 申请。
qc -p                   删除已经通过的 QC
qc <编号>               通过此编号的 QC 。
qc -q                   查看自己需要 QC 的项目。
qc -m <编号>            查看需要 QC 的文件。
qc -u                   查看 QC 小组名单。

开发巫师在CODE完成以后，发出QC申请的指令：
        qc <filename> -b <reason> 发出QC申请。
        qc -c <文件> <原因> 修改自己已经发出的 QC 申请。
        qc -d <编号> 删除自己已经发出的 QC 申请。
	qc -l <编号> 显示 QC 文件详细申请信息。
        qc -f <id> 查询某一wiz提出的QC申请。

注意： 你发出申请时的filename可以是路径(不含子目录)或文件，
       请在获取权限自行测试完毕后再行提交 QC 申请。

QC小组用这个指令来标记QC进度：
        qc <编号> 按流程标记QC状态。

        qc <编号> -k <驳回原因>
        驳回一个QC请求，并且说明驳回原因。

        qc -q                   查看自己需要 QC 的项目。
        qc -m <编号>             查看需要 QC 的文件。
        qc -p
        QC结束，文件加入另一张QC文件更新表的时候，用来删除所有已经批准的申请。
HELP
    );
    return 1;
}

