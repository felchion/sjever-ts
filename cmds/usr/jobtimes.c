//jobtimes by River@SJ 2003.6
//modify by linux@sj 2003.10.23

#include <ansi.h>
 
inherit F_CLEAN_UP;
/*                me->add("xx_points", 1);           /d/xingxiu/npc/ding.c mmd 太大了cat不了

     ws  dead ws 没有做记录 wo kao slogan stal     
     teach monk 找不到 :(     
*/
mapping *job_type=({
        (["job_name" : "古墓蜂巢",
     "job_long" : "古墓蜂巢任务"]),
     (["job_name" : "古墓护墓",
     "job_long" : "古墓护墓巡逻任务"]),
     (["job_name" : "明教巡逻",
     "job_long" : "明教巡逻任务"]),
     (["job_name" : "明教守卫",
     "job_long" : "明教守卫任务"]),
     (["job_name" : "桃花岛",                              //thd就这一个job。所以桃花岛守墓->桃花岛
     "job_long" : "桃花岛守墓杀贼任务"]),
     (["job_name" : "铁掌",
     "job_long" : "铁掌任务"]),
     (["job_name" : "峨嵋元兵",
     "job_long" : "峨嵋抗元任务"]),
     (["job_name" : "看守铜鼎",
     "job_long" : "武当看守铜鼎任务"]),
     (["job_name" : "嵩山砌石",
     "job_long" : "嵩山砌石任务"]),
     (["job_name" : "华山砍柴",
     "job_long" : "华山砍柴任务"]),
     (["job_name" : "神龙招魂",
     "job_long" : "神龙招魂任务"]),
     (["job_name" : "神龙索命",
     "job_long" : "神龙索命任务"]),
     (["job_name" : "抓蛇",
     "job_long" : "丐帮捉蛇任务"]),
         (["job_name" : "官府",
         "job_long" : "官府六扇门追捕任务"]),              
  //////////////////////////////////////////////
       (["job_name" : "少林熬粥",
         "job_long" : "少林熬粥任务"]),
       (["job_name" : "少林菜地",       
         "job_long" : "少林菜地任务"]),                                 
       (["job_name" : "少林轮值",                               //少林->少林轮值 少林和少林救援任务冲突，并且也不能代表轮值次数，现修正
         "job_long" : "少林轮值任务"]),
       (["job_name" : "佛法降魔",
         "job_long" : "少林渡世济人任务"]),
       (["job_name" : "罗汉堂",                                // 新增加 罗汉堂任务，即少林教和尚任务
         "job_long" : "罗汉堂值勤教习任务"]),
         
       (["job_name" : "草料场",
         "job_long" : NOR"火烧草料场"NOR]),
  
       (["job_name" : "颂摩崖",
         "job_long" : WHT"英雄抗敌颂摩崖"NOR]),
  
       (["job_name" : "武馆",
         "job_long" : "襄阳武馆打杂任务"]),
         
       (["job_name" : "赵敏",
         "job_long" : "赵敏郡主抓人任务",]),
         
       (["job_name" : "巡城",
         "job_long" : "镇南王府巡城任务"]),
         
       (["job_name" : "告急",
         "job_long" : HIR"边关告急任务"NOR]),
         
       (["job_name" : "丐帮",
         "job_long" : "丐帮吴长老杀人任务"]),
         
       (["job_name" : "刺杀",
         "job_long" : RED"刺杀粘而帖任务"NOR]),

       (["job_name" : "丢失的信",
         "job_long" : YEL"失落信笺任务"NOR]),
                 
       (["job_name" : "星宿叛徒",
         "job_long" : MAG"星宿击杀叛徒任务"NOR]),
         
       (["job_name" : "星宿熬膏",
         "job_long" : HIY"星宿熬药任务"NOR]),
         
        (["job_name" : "星宿",
         "job_long" : HIM"星宿老仙分忧任务"NOR]),//add by Ciwei@SJ
         
       (["job_name" : "华山",
         "job_long" : HIC"华山岳不群惩恶扬善任务"NOR]),
         
       (["job_name" : "洪七公",
         "job_long" : "洪七公"HIY"玉笛谁家听落梅"NOR"任务"]),
         
       (["job_name" : "护镖",
         "job_long" : "福威镖局护镖任务"]),
         
       (["job_name" : "送信",
         "job_long" : HIY"大理王府送信任务"NOR]),
         
       (["job_name" : "采石",
         "job_long" : "黄河采石任务"]),
         
       (["job_name" : "嵩山",
         "job_long" : HIM"嵩山派五岳合并任务"NOR]),
         
       (["job_name" : "盐枭",
         "job_long" : HIB"扬州贩盐任务"NOR]),
         
       (["job_name" : "武当",                                       //武当->武当恶贼
         "job_long" : "武当宋远桥杀恶贼任务"]),
              
       (["job_name" : "雪山",
         "job_long" : HIR"宝象抢美女任务"NOR]),        
       
       (["job_name" : "少林救援",
         "job_long" : HIC"少林救援任务"NOR]),  
         //(["job_name" : "少林",
         //"job_long" : HIC"少林救援任务"NOR]),  少林 一样代表不了救援job 现加标记 少林救援  少林目前只能代表总的job Ciwei@SJ
         
         (["job_name" : "大清国海战",
         "job_long" : HIY"大清国海战任务"NOR]),
         
         (["job_name" : "神龙岛海战",
         "job_long" : HIB"神龙岛海战任务"NOR]),
         
         (["job_name" : "长乐帮",
         "job_long" : HIY"长乐帮任务"NOR]),
         
         (["job_name" : "天地会",
         "job_long" : HIG"天地会任务"NOR]),
         
         (["job_name" : "抢劫",
         "job_long" : HIR"星宿抢劫任务"NOR]),
         
         (["job_name" : "慕容",                                 //慕容偷学->慕容
         "job_long" : HIW"慕容偷学任务"NOR]),
         
         (["job_name" : "大智岛",
         "job_long" : HIY"大智岛胜利生存任务"NOR]),
	(["job_name" : "七窍玲珑",
         "job_long" : HIY"七眼石鉴定任务"NOR]),         
});


int main(object me,string arg)
{
        object ob;
        string str;
        int j, i;

        if (!userp(me)) return 0;
        seteuid(getuid(me));

        if (!arg) ob = me;
        else if (!objectp(ob = present(arg, environment(me)))) {
                if (wizardp(me) ) {
                        if (!ob) ob = LOGIN_D->find_body(arg);
                        if (!ob) ob = find_living(arg);
                        if (!ob) return notify_fail("你要察看谁的任务次数？\n");
                        if (wiz_level(me) < wiz_level(ob))
                                return notify_fail("你要察看谁的任务次数？\n");
                }
        }

        if (!wizardp(me) && me!=ob)
                return notify_fail("你要察看谁的任务次数？\n");

        if (ob->is_corpse() || !ob->is_character())
                return notify_fail("那不是活物耶！\n");
        if (!userp(ob))
                return notify_fail("那不是玩家，会有任务？\n");
        if(!ob->query("job_time"))
                return notify_fail((ob!=me?ob->name(1):"你")+"至今还没有完成过任何任务。\n");

        str = (ob!=me?ob->name(1):"你")+"完成的任务次数如下：\n";
        str +="┌────────────────┬───────┬────────┐\n";
        str +="│           "HIW"任务名称"NOR"             │ "HIW"当前任务次数"NOR" │ "HIW"上次领奖励记录"NOR" │\n";
        str +="├────────────────┼───────┼────────┤\n";
        
        //Job_Type=keys(job_type);
        i = sizeof(job_type);
        
        for (j =0; j < i ;j++)
        if (ob->query("job_time/"+job_type[j]["job_name"]))
                str += sprintf("│%-32s│   %-11s│    %-12s│\n",job_type[j]["job_long"],
                sprintf("%d 次",ob->query("job_time/"+job_type[j]["job_name"])),
                (intp(ob->query("job_last_gift/"+job_type[j]["job_name"])) && ob->query("job_last_gift/"+job_type[j]["job_name"])!=0 )?sprintf("%d 次",ob->query("job_last_gift/"+job_type[j]["job_name"])):HIG"无记录"NOR);
                
        str +="└────────────────┴───────┴────────┘\n";
        if (me->query("ll/last_day")!=time()/86400)	{
					me->set("ll/last_day",time()/86400);
					me->set("ll/last_count",me->query("ll/count"));
					me->delete("ll/count");
					me->delete("ll/get");
				}
        str += sprintf(HIY"昨天完成失落信笺任务：%d次，今天完成失落信笺任务：%d/%d次。\n"NOR,me->query("ll/last_count"),me->query("ll/count"),me->query("ll/get"));
        if(stringp(ob->query("job_name")))  str +=HIW""+ (ob!=me?ob->name(1):"你")+"最近刚完成了"+ob->query("job_name")+"任务。\n"NOR;
        
        tell_object(me, str);
        return 1;
}

int help(object me)
{
        write(@HELP
指令格式： jobtimes
           jobtimes <对象名称>             (巫师专用)

这个指令可以显示你已经完成的各种任务次数。

相关指令：score

        Copyright 1997-2003 ShuJian MUD Wizard Group.
        All rights reserved.

        Visit our Homepage at - www.mymud.com

HELP
        );
        return 1;
}
