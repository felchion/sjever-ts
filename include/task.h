#include <ansi.h>
#ifndef _TASK_H_
#define _TASK_H_
//	File	:  /include/task.h

#define TASK_DIR(x) ("/task/" + x)

/*事件触发类型*/
#define TYP_TALK    1    //NPC对话触发 /cmds/usr/talk
#define TYP_ACTION  2    //动作触发 /feature/command
#define TYP_GO      4    //到达场景触发 /feature/move
#define TYP_HB      8    //心跳触发 /inherit/char/char
#define TYP_EF     16    //战斗结束触发 /adm/daemons/combatd
#define TYP_COLL   32   //物品收集触发
                           
/*事件及任务状态位*/
#define STAT_OFF    0    //
#define STAT_ACT    1    //
#define STAT_PROG   2    //
#define STAT_CNT    4    //
#define STAT_END    8    //
#define STAT_PASS   16   //
#define STAT_FAIL   32   //
#define STAT_FIN    64   //
#define STAT_RST    128  //
#define STAT_PEND   256  //


#define T_LST "/Task/"

/*用户绑定事件映射名*/
#define T_NPC T_LST+"NPC"
#define T_PLC T_LST+"PLC"
#define T_ITM T_LST+"ITM"
#define T_CMD T_LST+"CMD"
#define T_HB  T_LST+"HB"

#define T_E_LST T_LST+"Event"

/*用户事件进程状态标志映射名*/
#define E_LST T_E_LST+"/"+base_name(this_object())[6..<1]+"/"

/*用户任务进程状态标志映射名*/
#define TID T_LST+base_name(this_object())[6..<1]

#define K_ITM "/task/obj/key_item"
#define NPC_M "/task/obj/npc_monster"
#define NPC_H "/task/obj/npc_human"

#define HB_POISON 1
#define HB_HURT 2
#define HB_PROC 3

#endif