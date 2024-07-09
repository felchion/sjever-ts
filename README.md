多站点副本协同相关修改

/feature/damage.c die()副本死亡处理

/feature/move.c  move()副本移动限制 remove()在线标志清除

/feature/save.c 副本存档限制

/feature/shadow.c 副本登录用户屏蔽

/feature/message 消息屏蔽

/feature/alias.c 主副命令分发处理

/adm/simul_efun/message.c 消息屏蔽

/adm/simul_efun/object.c 副本IP显示处理

/adm/daemons/aliasd.c 主副命令分发

/adm/daemons/cpd.c 副本主控文件

/adm/daemons/logind.c 登录处理

/adm/daemons/taskd 统一奖励函数 

/adm/daemons/network/dns_master query_mud_name修改支持为多站点

/include/cp.h 副本相关宏

/include/globals.h 站点状态宏（如移植fluffos2017,要将get_config(256)改为get_config(16)）

/include/mudlib 多站点及副本定义(同上)

/include/net/config 通讯定义

/clone/user/user.c 进出副本函数

/shadow/telnet.c 副本登录

/cmds/std/look.c 修改副本地名为绿色

/cmds/std/go.c 调用副本主控进出副本判断

/kungfu/condition/smy_job 单用户副本示例

/public  为解密源码

master.sj slave.sj cp.sj 为主站、分站、副本启动文件。

只是配置不同端口，但需和globals.h mudlib.h里的宏定义一致。


任务框架说明

/adm/daemons/taskd 任务主控

/inherit/misc/task 任务基类

/include/task.h 任务定义宏(内有事件入口修改文件说明)

/task 标准任务人物、物品模板及任务范例

